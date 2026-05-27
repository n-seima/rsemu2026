//	2005-09-22 Kwon Yong Jae (amadeusj@gmail.com)
//
//	cConsoleOption		: ÄÜ¼Ö ¿É¼Ç
//  cUnicode			: À¯´ÏÄÚµå °ü¸®
//  cMemoryPool			: MemoryPool
//	cSort				: Á¤·Ä
//	cList				: LinkedList
//	cQueue				: Queue
//	cStack				: Stack
//	cDataManager		: µ¥ÀÌÅ¸ °ü¸®
//	cIOCPQueue			: IOCP Queue
//	cHashTable			: ÇØ½ÃÅ×ÀÌºí
//	cArray				: ¹è¿­ÀÇ Å©±â°¡ µ¿ÀûÀ¸·Î ´Ã¾î³­´Ù (ÁÖ¼Ò´Â º¯ÇÏÁö ¾Ê´Â´Ù)
//	cCS					: CRITICAL_SECTION °ü¸®
//	cThreadDataRef		: ÇØ´çµ¥ÀÌÅ¸ÀÇ Á¢¼Ó È½¼ö¸¦ °ü¸®
//	cFileManager		: ÆÄÀÏ°ü¸®
//	cLog				: ·Î±×
//	cThread				: Thread
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


#include <assert.h>
#include <tchar.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>	// std::sort() ¿¡¼­ »ç¿ë
using namespace std;



// ===============================================================================================================================
// ======================================================== ¹®ÀÚ°ü·Ã ============================================================
// ===============================================================================================================================
//	º¸¾È¿¡ °ü·ÃµÈ ¹®ÀÚÃ³¸®
// ===============================================================================================================================
#define _TCSCPY(_dst, _src) _tcsncpy_s(_dst, _countof(_dst), _src, _TRUNCATE)
#define _TCSCAT(_dst, _src) _tcsncat_s(_dst, _countof(_dst), _src, _TRUNCATE)
#define _SPRINTF(_buf, _fotmat, ...) _sntprintf_s(_buf, _countof(_buf), _TRUNCATE, _fotmat, __VA_ARGS__);		// VS 2005 ¹öÁ¯ÀÌ»ó¿¡¼­¸¸ »ç¿ë°¡´ÉÇÏ´Ù




// ===============================================================================================================================
// ========================================================= cConsoleOption ======================================================
// ===============================================================================================================================
//	2006-03-26 Kwon Yong Jae (amadeusj@gmail.com)
//	ÄÜ¼Ö¿¡ °ü·ÃµÈ ¿É¼Ç
//	static À¸·Î Á¤ÀÇ
// ===============================================================================================================================
class cConsoleOption
{
private:
	static HANDLE m_hConsole;

public:
	cConsoleOption();
	~cConsoleOption();
	static void disableCloseBtn();
	static void setConsoleTitle(TCHAR *_pszTitle);
	static void getConsoleTitle(TCHAR *_pszTitle, int _iSize);

	inline static void setColor(WORD _wColor = NULL)
	{
		// no color specified, reset to defaults (white font on black background)
		if(m_hConsole){
			if(_wColor != NULL){
				SetConsoleTextAttribute(m_hConsole, _wColor);
			}else{
				SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // white text on black bg	
			}
		}
	}	
};




// ===============================================================================================================================
// ========================================================= cUnicode ============================================================
// ===============================================================================================================================
//	2006-06-10 Kwon Yong Jae (amadeusj@gmail.com)
//	À¯´ÏÄÚµå
//	ÇöÀç extern À¸·Î Á¤ÀÇµÇ¾îÀÖ´Ù (°´Ã¤»ý¼º¾øÀÌ g_cUnicode °´Ã¼¸¦ »ç¿ëÇÏ¸éµÈ´Ù)
//	¾²·¹µå¿¡¼­ È£ÃâµÈ´Ù¸é init(TRUE); ¸¦ È£ÃâÇØ¾ß ³»ºÎÀûÀ¸·Î CS Ã³¸®°¡ µÈ´Ù
// ===============================================================================================================================

// ÄÜ¼Ö ÅØ½ºÆ® »ö»ó
typedef enum _enumCONSOLECOLOR
{
	eCONSOLECOLOR_NOMAL		= 0,				// Nomal
	eCONSOLECOLOR_NOTICE	= 6,				// Yello	
	eCONSOLECOLOR_WARNING	= FOREGROUND_RED,	// Red	
	eCONSOLECOLOR_ERROR		= FOREGROUND_GREEN,	// Grren	
	eCONSOLECOLOR_OTHER		= 4,
}enumCONSOLECOLOR;

class cUnicode
{
private:
	static BOOL m_bUseCriticalSection;
	static CRITICAL_SECTION m_CS;	// Æ÷ÀÎÅÍ·Î ¼±¾ðÇÏÁö ¾Ê´Â´Ù, new Ã³¸®ÇÏ¸é delete ÇÒ°÷ÀÌ ¾ø´Ù
	static TCHAR m_szPrint[2048];	// print() ÇÔ¼ö¿¡¼­ ¸Å¹ø Áö¿ªº¯¼ö·Î ¼±¾ðÇÏ±â°¡ Á» ±×·¡¼­ static À¸·Î ¼±¾ð (¿©·¯¾²·¹µå¿¡¼­ µ¿½Ã¿¡ Á¢±ÙÇÒ¼ö ¾ø´Ù)
	
private:
	// static À¸·Î¸¸ »ç¿ëÇÑ´Ù
	cUnicode(){}	
	~cUnicode(){}

public:	
	static void init(BOOL _bUseCriticalSection)
	{
		m_bUseCriticalSection = _bUseCriticalSection;
		if(m_bUseCriticalSection){
			InitializeCriticalSection(&m_CS);
		}
	}

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Name : 
	// Desc : À¯´ÏÄÚµå¸¦ »ç¿ëÇÒ°æ¿ì ÇÑ±ÛÀº ÄÜ¼Ö·Î Ãâ·ÂÀÎ¾ÈµÇ¼­ ´Ù¸¥ ¹æ¹ýÀ» »ç¿ë
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline static void print(TCHAR *_pszMessage, ...)
	{
		if(m_bUseCriticalSection){			
			EnterCriticalSection(&m_CS);			
		}

		va_list va;
		va_start(va, _pszMessage);
		_vstprintf_s(m_szPrint, _countof(m_szPrint), _pszMessage, va);
		va_end(va);	

		#if defined _UNICODE  ||  defined UNICODE
			HANDLE hConsoleOutput;
			DWORD dwWritten;
			hConsoleOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
			::WriteConsole(hConsoleOutput, m_szPrint, (int)_tcslen(m_szPrint), &dwWritten, NULL);
			TCHAR *p = _T("\n");			
			::WriteConsole(hConsoleOutput, p, (int)_tcslen(p), &dwWritten, NULL);
		#else
			printf(m_szPrint);
			printf("\n");
		#endif

		fflush(stdout);

		if(m_bUseCriticalSection){
			LeaveCriticalSection(&m_CS);
		}		
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Name : 
	// Desc : À¯´ÏÄÚµå¸¦ »ç¿ëÇÒ°æ¿ì ÇÑ±ÛÀº ÄÜ¼Ö·Î Ãâ·ÂÀÎ¾ÈµÇ¼­ ´Ù¸¥ ¹æ¹ýÀ» »ç¿ë
	//        eCONSOLECOLOR_NOMAL   : Nomal
	//        eCONSOLECOLOR_NOTICE	: Yello
	//        eCONSOLECOLOR_WARNING : Red
	//        eCONSOLECOLOR_ERROR	: Grren
	//        eCONSOLECOLOR_OTHER
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline static void print(int _iColor, TCHAR *_pszMessage, ...)
	{
		if(m_bUseCriticalSection){			
			EnterCriticalSection(&m_CS);			
		}

		if(_iColor){
			cConsoleOption::setColor(_iColor | FOREGROUND_INTENSITY);
		}

		va_list va;
		va_start(va, _pszMessage);
		_vstprintf_s(m_szPrint, _countof(m_szPrint), _pszMessage, va);
		va_end(va);	

		#if defined _UNICODE  ||  defined UNICODE
			HANDLE hConsoleOutput;
			DWORD dwWritten;
			hConsoleOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
			::WriteConsole(hConsoleOutput, m_szPrint, (int)_tcslen(m_szPrint), &dwWritten, NULL);
			TCHAR *p = _T("\n");			
			::WriteConsole(hConsoleOutput, p, (int)_tcslen(p), &dwWritten, NULL);
		#else
			printf(m_szPrint);
			printf("\n");
		#endif

		fflush(stdout);

		if(_iColor){
			cConsoleOption::setColor();
		}

		if(m_bUseCriticalSection){
			LeaveCriticalSection(&m_CS);
		}		
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Name : 
	// Desc : Wide => Multi
	//        _pszMulti ¹öÆÛÀÇ »çÀÌÁî´Â _pszWide ÀÇ »çÀÌÁîº¸´Ù ³Ë³ËÈ÷ÀâÀ¸¸é 2¹è´Â ÁÖ¾î¾ßÇÑ´Ù (wchar_t °¡ char ÀÇ 2¹èÀÌ¹Ç·Î)
	//       return : »ç¿ëÇÑ ¹öÆÛÀÇ »çÀÌÁî¸¦ ¸®ÅÏ(NULL Æ÷ÇÔ)
	//       ÇÁ·ÎÁ§Æ®°¡ À¯´ÏÄÚµå°¡ ¾Æ´Ï¶ó¸é Æ¯º°È÷ ÇØÁÙ°ÍÀÌ ¾ø´Ù (TCHAR = char)
	//
	//       TCHAR szWide[8] = _T("±ÇPath");
	//       char szMulti[sizeof(szWide)];		// Unicode ¶ó¸é szMulti Àº szWide »çÀÌÁîÀÇ 2¹è
	//       int iRe = W2T2(szWide, szMulti, sizeof(szMulti));
	//       UNICODE => iRe = 7, NoUNICODE = 6
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline static int W2T2(const TCHAR *_pszWide, char *_pszMulti, int _iMultiSize)
	{	
		#if defined _UNICODE  ||  defined UNICODE
			int iResult = WideCharToMultiByte(CP_ACP, 0, _pszWide, -1, _pszMulti, _iMultiSize, NULL, NULL);
			if(iResult == 0){
				DWORD dwRe = GetLastError();
				if(dwRe == ERROR_INSUFFICIENT_BUFFER){
					print(_T("Error!, WideCharToMultiByte() \nError : ERROR_INSUFFICIENT_BUFFER"), eCONSOLECOLOR_ERROR);
					return 0;
				}
			}
			return iResult;
		#else
			if(_iMultiSize >= (int)_tcslen(_pszWide)){
				_tcscpy_s(_pszMulti, _iMultiSize, _pszWide);
				return (int)_tcslen(_pszMulti);
			}
			return 0;
		#endif
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Name : 
	// Desc : Multi => Wide
	//       _pszWide ¹öÆÛÀÇ »çÀÌÁî´Â _pszMulti ÀÇ »çÀÌÁî¿Í °°À¸¸éµÈ´Ù (wchar_t °¡ char ÀÇ 2¹èÀÌ¹Ç·Î)
	//       return : »ç¿ëÇÑ ¹öÆÛÀÇ »çÀÌÁî¸¦ ¸®ÅÏ(NULL Æ÷ÇÔ), ¸®ÅÏÀÌ 10 ÀÌ¶ó¸é TCHAR[10] ÀÌ¸éµÈ´Ù
	//       ÇÁ·ÎÁ§Æ®°¡ À¯´ÏÄÚµå°¡ ¾Æ´Ï¶ó¸é Æ¯º°È÷ ÇØÁÙ°ÍÀÌ ¾ø´Ù (TCHAR = char)
	//
	//       char szMulti2[16] = "±ÇPath";
	//       TCHAR szWide2[sizeof(szMulti2)];
	//       iRe = T2W2(szMulti2, szWide2, size(sizeof(szWide2)));
	//       UNICODE => iRe = 6, NoUNICODE = 6
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline static int T2W2(const char *_pszMulti, TCHAR *_pszWide, int _iWideSize)
	{	
		#if defined _UNICODE  ||  defined UNICODE                      
			int iResult = MultiByteToWideChar(CP_ACP, 0, _pszMulti, -1, _pszWide, _iWideSize);
			if(iResult == 0){
				DWORD dwRe = GetLastError();
				if(dwRe == ERROR_INSUFFICIENT_BUFFER){
					print(_T("Error!, MultiByteToWideChar() \nError : ERROR_INSUFFICIENT_BUFFER"), eCONSOLECOLOR_ERROR);
					return 0;
				}
			}
			return iResult;
		#else
			if(_iWideSize >= (int)_tcslen(_pszMulti)){
				_tcscpy_s(_pszWide, _iWideSize, _pszMulti);
				return (int)_tcslen(_pszWide);
			}
			return 0;		
		#endif
	}
};



// ===============================================================================================================================
// ========================================================= cSort ===============================================================
// ===============================================================================================================================
// 2008-08-25 Kwon Yong Jae (amadeusj@gmail.com)
// ¿Â¶óÀÎ °ÔÀÓ ¼­¹ö ÇÁ·Î±×·¡¹Ö
// Ex)
// class cTest : public cMemoryPool<cTest, 40>
// {
//    ...
// }
// ===============================================================================================================================
template <class T, int ALLOC_BLOCK_SIZE = 50>
class cMemoryPool
{
private:
	static unsigned char *m_freePointer;

private:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Name : 
	// Desc : 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void	allocBlock()
	{
		m_freePointer = new unsigned char[sizeof(T) * ALLOC_BLOCK_SIZE];

		unsigned char **ppCurrent = reinterpret_cast<unsigned char **>(m_freePointer);
		unsigned char *pNext = m_freePointer;

		for(int i=0; i<ALLOC_BLOCK_SIZE-1; ++i)
		{
			pNext += sizeof(T);
			*ppCurrent = pNext;
			ppCurrent = reinterpret_cast<unsigned char**>(pNext);
		}

		*ppCurrent = 0;
	}

protected:
	~cMemoryPool(){}

public:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Name : 
	// Desc : 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void *operator new(size_t _allocLen)
	{
		assert(sizeof(T) == _allocLen);
		assert(sizeof(T) >= sizeof(unsigned char *));

		if(!m_freePointer){
			allocBlock();
		}

		unsigned char *pReturnPointer = m_freePointer;
		m_freePointer = *reinterpret_cast<unsigned char **>(pReturnPointer);

		return pReturnPointer;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Name : 
	// Desc : 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void	operator delete(void *_pDeletePointer)
	{
		* reinterpret_cast<unsigned char **>(_pDeletePointer) = m_freePointer;
		m_freePointer = static_cast<unsigned char *>(_pDeletePointer);
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T, int ALLOC_BLOCK_SIZE>
unsigned char* cMemoryPool<T, ALLOC_BLOCK_SIZE>::m_freePointer;




// ===============================================================================================================================
// ========================================================= cSort ===============================================================
// ===============================================================================================================================
//	2006-11-10 Kwon Yong Jae (amadeusj@gmail.com)
//	QSort
//	Hoare ¶ó´Â »ç¶÷ÀÌ 1962³â¿¡ ¹ßÇ¥ÇÑ Äü ¼ÒÆ®´Â ¿¹¿Ü´Â ÀÖÁö¸¸ ¸¹Àº °æ¿ì¿¡¼­ °¡Àå ºü¸£¹Ç·Î ÇöÀç·Î¼­´Â 
//	°¡Àå ´ëÁßÀûÀÎ ºÐ·ù±â¹ýÀÌ µÇ¾îÀÖ½À´Ï´Ù ÇÑ¸¶µð·Î ®g ¼ÒÆ® ¶ó°í ÇØµµ ½ÇÁ¦·Î´Â È¿À²À» °³¼±ÇÏ±â À§ÇÑ ¿©·¯°¡Áö ¹öÀüÀÌ ÀÖ½À´Ï´Ù
//	ÃÖ±Ù¿¡´Â ¸ðµç °æ¿ì¿¡¼­ ¿¹¿Ü¾øÀÌ Äü ¼ÒÆ®ÀÇ Æò±ÕÀûÀÎ ¼±´ÉÀ» ¹ßÈÖÇÒ¼ö ÀÖ´Ù°í ÇÏ´Â º»·¡ÀÇ Äü ¼ÒÆ®¸¦ 
//	»óÈ¸ÇÏ´Â ºÐ·ù ±â¹ýµµ ÃâÇöÇÏ°í ÀÖ½À´Ï´Ù
//	Äü¼ÒÆ®´Â Ãà(pivot) ÀÌ¶ó°í ÇÏ´Â ±âÁØ°ªÀ» »ç¿ëÇÏ¿© ¿ä¼Ò µ¥ÀÌÅ¸¸¦ 2°³ÀÇ ±×·ìÀ¸·Î ºÐÇÒÇÏ¸é¼­ ºÐ·ù¸¦ ÁøÇàÇÏ´Â
//	ºÐÇÒ °ø·«(Divide and Conquer) ÀÌ¶ó´Â ±â¹ýÀ» ÀÌ¿ëÇÕ´Ï´Ù
//	ÀÌ ºÐÇÒ °ø·«Àº 1È¸ÀÇ Á¶ÀÛÀ¸·Î ¹è¿­ ¿ä¼Ò¸¦ Pivot º¸´Ù ÀÛÀº ´ëÀÇ ±×·ì °ú Pivot º¸´Ù Å« ´ëÀÇ ±×·ì À¸·Î 2ºÐÇÒÇÕ´Ï´Ù
//	ÀÌ¿Í °°ÀÌ ºÐÇÒ °ø·«ÀÇ 1È¸ Á¶ÀÛÀº ´Ü¼øÇÑ ±×·ì ºÐÇÒÀ» ÇÒ»ÓÀÌÁö¸¸ ºÐÇÒ °ø·«¿¡¼­´Â ºÐÇÒÈÄÀÇ ¼Ò±×·ì°ú ´ë±×·ì¿¡ ´ëÇØ¼­µµ 
//	¶È°°Àº ¹æ¹ýÀ» ¹Ýº¹ÇØ °©´Ï´Ù
//	Recursive ¸¦ ÀÌ¿ëÇÏÁö ¾Ê°í Function Pointer ¸¦ ÀÌ¿ëÇÑ´Ù
//	cSort ¸¦ »ç¿ëÇÏ¶§º¸´Ù STL ÀÇ sort() ¸¦ ÀÌ¿ëÇÒ¶§°¡ Release ÀÏ¶§ ´õºü¸£´Ù (Debug ÀÏ¶§´Â cSort °¡ ´õºü¸£´Ù)
// ===============================================================================================================================

template <class T>
class cSort
{
private:
	
public:
	typedef enum _enumBASERYPE
	{
		eNOTFINDBINARYSEARCH	= -1,
	}enumBASERYPE;

public:
	cSort(){}
	~cSort(){}
	template <class D>
	static void qSort(T *_pArr, int _iDataCnt, D _d);
	template <class D>
	static void STLSort(T *pArrFirstData, T *pArrLastData, D _d);
	template <class T2, class D1, class D2>
	static int binarySearch(T *_pArr, int _iArrMinIdx, int _iArrMaxIdx, T2 _SearchValue, D1 _d1, D2 _d2);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : Recursive ¸¦ ÀÌ¿ëÇÏÁö ¾ÊÀ¸¹Ç·Î qSortRecursiveDES, qSortRecursiveASC º¸´Ù ´õ ºü¸£´Ù
//        Ex)
//        ÇÔ¼öÀÚ¸¦ ÀÌ¿ëÇÑ´Ù
//        typedef struct _stASC			// ¿À¸§Â÷¼ø
//        {
//             BOOL operator()(const stTEST &_iValue1, const stTEST &_iValue2) const 
//             {
//                 return _iValue1.aa < _iValue2.aa;
//             }
//        }stASC;
//        cSort<stTEST>::qSort(m_pArr, 10, stASC())
///////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
template <class D>
void cSort<T>::qSort(T *_pArr, int _iDataCnt, D _d)
{
	int h = 1;	
	T B;
	int j;

	// find the largest h value possible 
	while((h * 3 + 1) < _iDataCnt){
		h = 3 * h + 1;
	}

	// while h remains larger than 0 
	while(h > 0){
		// for each set of elements (there are h sets)
		for(int i=h-1; i<_iDataCnt; i++){
			// pick the last element in the set
			B = _pArr[i];
			j = i;

			// compare the element at B to the one before it in the set
			// if they are out of order continue this loop, moving
			// elements "back" to make room for B to be inserted.
			for(j=i; (j>=h) && _d(B, _pArr[j-h]); j-=h){
				_pArr[j] = _pArr[j-h];
			}

			// insert B into the correct place
			_pArr[j] = B;
		}

		// all sets h-sorted, now decrease set size
		h = h / 3;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : STL ÀÇ algorithm À» ÀÌ¿ëÇÑ Sort
//        pArrLastData ´Â ¸¶Áö¸· µ¥ÀÌÅ¸ÀÇ ÁÖ¼Ò°¡ ¾Æ´Ñ ¸¶Áö¸· ÀÇ ´ÙÀ½ µ¥ÀÌÅ¸ ÁÖ¼Ò
//        Ex)
//        ÇÔ¼öÀÚ¸¦ ÀÌ¿ëÇÑ´Ù
//        typedef struct _stASC			// ¿À¸§Â÷¼ø
//        {
//             BOOL operator()(const stTEST &_iValue1, const stTEST &_iValue2) const 
//             {
//                 return _iValue1.aa < _iValue2.aa;
//             }
//        }stASC;
//        cSort<stTEST>::STLSort(&m_pArr[0], &m_pArr[10], stASC())
///////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
template <class D>
void cSort<T>::STLSort(T *pArrFirstData, T *pArrLastData, D _d)
{
	std::sort(pArrFirstData, pArrLastData, _d);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : ¸®Ä¿½Ãºê, È£Ãâ½Ã _iArrMinIdx´Â ¹è¿­ÀÇ Ã¹¹øÂ° ÀÎµ¦½º _iArrMaxIdx´Â ¹è¿­ÀÇ ³¡ÀÌ ¾Æ´Ñ ¸¶Áö¸· ÀÎµ¦½º, °Ë»öµÇ¸é ÀÎµ¦½º¸¦ ¸®ÅÏÇÑ´Ù
//        Ex)
//        ÇÔ¼öÀÚ¸¦ ÀÌ¿ëÇÑ´Ù
//        typedef struct _stEQUAL
//        {
//            BOOL operator()(const stTEST &_iValue1, const int &_iValue2) const 
//            {
//                return _iValue1.aa == _iValue2;
//            }
//        }stEQUAL;
//        typeder struct _stASC
//        {
//            BOOL operator()(const stTEST &_iValue1, const int &_iValue2) const 
//            {
//                return _iValue1.aa < _iValue2;
//            }
//        }stASC;
//        cSort<int>::binarySearch(m_piBinarySearchData, 0, m_iUseArrCnt - 1, &_uniqueKey, stEQUAL(), stASC());
///////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
template <class T2, class D1, class D2>
int cSort<T>::binarySearch(T *_pArr, int _iArrMinIdx, int _iArrMaxIdx, T2 _SearchValue, D1 _d1, D2 _d2)
{
	if(_iArrMinIdx > _iArrMaxIdx){
		return eNOTFINDBINARYSEARCH;
	}

	// Áß°£À§Ä¡¸¦ ±¸ÇÑ´Ù
	int iMiddle; 
	iMiddle = (_iArrMinIdx + _iArrMaxIdx) >> 1;

	if(_d1(_pArr[iMiddle], _SearchValue)){	// ¹ß°ß
		return iMiddle;						// ÀÎµ¦½º¸¦ ¸®ÅÏ
	}

	if(_d2(_pArr[iMiddle], _SearchValue)){	// A < B
		_iArrMinIdx = iMiddle + 1;
	}else{
		_iArrMaxIdx = iMiddle - 1;		
	}

	return binarySearch(_pArr, _iArrMinIdx, _iArrMaxIdx, _SearchValue, _d1, _d2);
}




// ===============================================================================================================================
// ====================================================== cFileManager ===========================================================
// ===============================================================================================================================
//	2008-12-01 Kwon Yong Jae (amadeusj@gmail.com)
//	ÆÄÀÏ°ü¸®
// ===============================================================================================================================
class cFileManager
{
private:
	HANDLE m_hFile;
	TCHAR m_szFileFullName[_MAX_PATH + _MAX_FNAME];	// °æ·Î¸¦ Æ÷ÇÔÇÑ ÆÄÀÏ¸í (Ex: "d:\\TestDirectory\\Name.exe")
	TCHAR m_szFileName[_MAX_FNAME];					// È®ÀåÀÚ¸¦ Á¦¿ÜÇÑ ÆÄÀÏ¸í (Ex: "Name")
	TCHAR m_szDirectory[_MAX_PATH];					// ÆÄÀÏ¸íÀ» Á¦¿ÜÇÑ °æ·Î (Ex: "d:\\TestDirectory\\")
	TCHAR m_szFormat[8];							// È®ÀåÀÚ (Ex: "exe")

public:	
	// ÆÄÀÏ ±âº»Á¤º¸
	typedef struct _stFILEBASEINFO
	{
		DWORD dwFileSize;
		TCHAR szDirectory[_MAX_PATH];
		TCHAR szFileName[_MAX_FNAME];
	}stFILEBASEINFO;

	// ÆÄÀÏ µðÅ×ÀÏÁ¤º¸
	typedef struct _stFILEDETAILINFO
	{
		stFILEBASEINFO stBaseInfo;
		SYSTEMTIME systimeCreation;
		SYSTEMTIME systimeLastAccess;
		SYSTEMTIME systimeLastWrite;		
	}stFILEDETAILINFO;

public:
	cFileManager(void);
	~cFileManager(void);

	BOOL createFile(const TCHAR *_pszFileName, int _iDesiredAccess, int _CreationDisposition);
	void closeFile();
	DWORD writeFile(const void *_pData, int _iDataSize);
	DWORD readFile(void *_pBuffer, int _iBufferSize);
	DWORD readFile(char *_pTempBuffer, int _iTempBufferSize, TCHAR *_pBuffer, int _iBufferSize);
	DWORD readFileLine(void *_pBuffer, int _iBufferSize);
	DWORD readFileLine(char *_pTempBuffer, int _iTempBufferSize, TCHAR *_pBuffer, int _iBufferSize);
	BOOL moveFilePoint(DWORD _dwMoveMethod);
	DWORD getFileSize();

	static DWORD getFileSize(const TCHAR *_pszFileName);
	static int getFileLineCnt(const TCHAR *_pszFileName);
	static void getFileNameExist(const TCHAR *_pszFileName, TCHAR *_pszNewFileName, int _iNewFileNameSize);
	static BOOL operationFile(int _iOperationType, const TCHAR *_pszSourceFileName, const TCHAR *_pszDestFileName);
	static BOOL createDirectory(const TCHAR *_pszDirectory);
	static BOOL isFileExist(const TCHAR *_pszFileName);
	static BOOL isDirectoryExist(const TCHAR *_pszPath);
	static void getFileList(const TCHAR *_pszPath, vector<stFILEBASEINFO> *_pvecFileBaseInfo, vector<stFILEDETAILINFO> *_pvecFileDetailInfo);

	inline const TCHAR *getFileFullName(){return m_szFileFullName;}
	inline BOOL active(){return (m_hFile ? TRUE : FALSE);}
};




// ===============================================================================================================================
// ========================================================= cLog ================================================================
// ===============================================================================================================================
//	2005-09-22 Kwon Yong Jae (amadeusj@gmail.com)
//	File Log
//	cLog ´Â ³»ºÎÀûÀ¸·Î CRITICAL_SECTION À» »ç¿ëÇÑ´Ù
//	À¯´ÏÄÚµåÀÏ¶§ ÆÄÀÏÀÛ¾÷ÀÌ ¾à°£ º¹ÀâÇØ¼­ ±×³É Ascii ·Î ÀÛ¼º
//	¿À´ÃÀÇ ³â¿ùÀ» °¡Áö°í Æú´õ¸¦ »ý¼ºÇØ¼­ ¿À´ÃÀÇ ³¯Â¥º°·Î ÆÄÀÏÀ» »ý¼ºÇÑ´Ù, 
//	ÇÁ·Î±×·¥ÀÌ ½ÇÇàÁß¿¡ ³¯Â¥°¡ ¹Ù²î¸é ÇØ´çÆÄÀÏÀ» ´Ý°í »õ·Î¿î ³¯Â¥·Î ÆÄÀÏÀ» »ý¼ºÇÑ´Ù
// ===============================================================================================================================

#define dLOGFILEFOLDER	_T("LOGFILE\\")

class cLog  
{
private:
	typedef enum _enumBASETYPE
	{
		eMAXLOGLENGTH		= 1024, 
		//eMAXFILESIZE		= 10000000,	// ÇöÀç »ç¿ë¾ÈÇÔ
	}enumBASETYPE;

	BOOL m_bUseCriticalSection;
	CRITICAL_SECTION *m_pCS;
	BOOL m_bActive;
	cFileManager m_cFileManager;
	TCHAR m_szOriginalFileName[256];	// ¿ø·¡ÀÇ ÆÄÀÏ¸í
	TCHAR m_szFileTotalName[512];		// ¸ðµç °æ·Î¸¦ Æ÷ÇÔÇÑ ÆÄÀÏ¸í (ÆÄÀÏÀÌ Á¸ÀçÇÏ¸é ÆÄÀÏ¸íÀÌ º¯°æµÉ¼ö ÀÖ´Ù)
	SYSTEMTIME m_systimeCurDate;		// ±ÝÀÏÀÇ ³¯Â¥

private:
	BOOL fileBackup();

public:	
	cLog();
	~cLog();
	BOOL openFile(const TCHAR *_pszFileName, BOOL _bUseCriticalSection);
	void clearData();
	void closeFile();
	BOOL writeLog(const TCHAR *_pszMessage, BOOL _bWriteDate = TRUE);		
	inline const TCHAR *getFileTotalName(){return m_cFileManager.getFileFullName();}
};




// ===============================================================================================================================
// ========================================================= cList ===============================================================
// ===============================================================================================================================
//	2005-09-22 Kwon Yong Jae (amadeusj@gmail.com)
//	LinkedList
//	Ã³À½ »ç¿ëÇÒ¶§ init() ¸¦ È£ÃâÇØ¾ßÇÑ´Ù
//	³»ºÎÀûÀ¸·Î CS Ã³¸® ¾ÈÇÔ (¾²·¹µå Áö¿ø¾ÈÇÔ)
// ===============================================================================================================================

template <class T>
class cList  
{
private:
	struct stNODE
	{
		T data;
		stNODE *pNext;
	};	

	stNODE *m_pHead;	
	int m_iCnt;									// µ¥ÀÌÅ¸ ¼ö
	cLog *m_pcLog;								// Log

public:		
	cList();
	~cList();	
	void init(cLog *_pcLog);
	T *getData(int _iNum);						// 0 ºÎÅÍ½ÃÀÛ
	T m_dummyData;								// getData() ¿¡¼­ ¿¡·¯½Ã ÁÖ¼Ò¸¦ ¸®ÅÏÇØ¾ßµÇ´Âµ¥ ´õ¹Ì·Î ¿¡·¯Ã³¸®½Ã »ç¿ë
	void addHead(const T *_pItem);
	void addTail(const T *_pItem);
	void insertData(int _iNum, const T *_pItem);// 0 ºÎÅÍ½ÃÀÛ
	void deleteData(int _iNum);					// 0 ºÎÅÍ ½ÃÀÛ
	int getCount();	
	void deleteAllData();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
cList<T>::cList()
{
	m_pcLog = NULL;
	m_pHead = NULL;	
	memset(&m_dummyData, 0, sizeof(T));
	m_iCnt = 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
cList<T>::~cList()
{
	deleteAllData();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
void cList<T>::init(cLog *_pcLog)
{
	if(m_pHead){
		TCHAR szPrint[256];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cList::init(), Already call cList::init()"));
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return;
	}
	m_pcLog = _pcLog;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
void cList<T>::insertData(int _iNum, const T *_pItem)
{
	if(_iNum > m_iCnt  ||  _iNum < 0){	
		TCHAR szPrint[256];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cList::insertData(), m_iCnt: %d, _iNum: %d"), m_iCnt, _iNum);
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return;
	}

	if(_iNum == 0){				// ¸ÇÃ³À½ÀÏ¶§
		addHead(_pItem);		// ³»ºÎÀûÀ¸·Î CS Ã³¸®
	}else if(_iNum == m_iCnt){	// ¸Ç¸¶Áö¸·¿¡ ³ÖÀ»¶§
		addTail(_pItem);		// ³»ºÎÀûÀ¸·Î CS Ã³¸®
	}else{
		stNODE *pTemp = m_pHead;
		for(int i=0; i<_iNum-1; i++){
			pTemp = pTemp->pNext;
		}
		
		stNODE *pImsi;
		pImsi = pTemp->pNext;
		pTemp->pNext = new stNODE;

		pTemp = pTemp->pNext;
		memcpy(&pTemp->data, _pItem, sizeof(T));
		pTemp->pNext = pImsi;
		++m_iCnt;		
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
void cList<T>::addHead(const T *_pItem)
{
	if(m_iCnt == 0){	// µ¥ÀÌÅ¸°¡ ÇÏ³ªµµ ¾øÀ»°æ¿ì
		m_pHead = new stNODE;
		memcpy(&m_pHead->data, _pItem, sizeof(T));
		m_pHead->pNext = NULL;		
	}else{
		stNODE *pTemp = new stNODE;
		memcpy(&pTemp->data, _pItem, sizeof(T));
		pTemp->pNext = m_pHead;
		m_pHead = pTemp;		
	}
	++m_iCnt;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
void cList<T>::addTail(const T *_pItem)
{
	if(m_iCnt == 0){	// µ¥ÀÌÅ¸°¡ ÇÏ³ªµµ ¾øÀ»°æ¿ì
		m_pHead = new stNODE;
		memcpy(&m_pHead->data, _pItem, sizeof(T));
		m_pHead->pNext = NULL;		
	}else{
		stNODE *pTemp = m_pHead;
		for(int i=0; i<m_iCnt - 1; i++){
			pTemp = pTemp->pNext;
		}
		pTemp->pNext = new stNODE;	
		pTemp = pTemp->pNext;
		memcpy(&pTemp->data, _pItem, sizeof(T));		
		pTemp->pNext = NULL;				
	}
	++m_iCnt;	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
void cList<T>::deleteData(int _iNum)
{
	if(_iNum >= m_iCnt  ||  _iNum < 0){		
		TCHAR szPrint[256];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cList::deleteData(), m_iCnt: %d, _iNum: %d"), m_iCnt, _iNum);
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return;
	}

	stNODE *pImsi;
	if(_iNum == 0){
		pImsi = m_pHead->pNext;
		delete m_pHead;
		m_pHead = NULL;
		m_pHead = pImsi;		
	}else{
		stNODE *pTemp = m_pHead;
		for(int i=0; i<_iNum-1; i++){
			pTemp = pTemp->pNext;
		}

		pImsi = pTemp->pNext->pNext;
		delete pTemp->pNext;
		pTemp->pNext = NULL;
		pTemp->pNext = pImsi;
	}
	--m_iCnt;	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
int cList<T>::getCount()
{
	return m_iCnt;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
T *cList<T>::getData(int _iNum)
{
	if(_iNum >= m_iCnt  ||  _iNum < 0){	
		TCHAR szPrint[256];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cList::getData(), m_iCnt: %d, _iNum: %d"), m_iCnt, _iNum);
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return &m_dummyData;	// ±×³É °ªÀÌ 0À¸·Î Ã¤¿öÁø°ÍÀÌ ¸®ÅÏµÊ(´õ¹Ì µ¥ÀÌÅ¸)
	}

	stNODE *pTemp = m_pHead;
	for(int i=0; i<_iNum; i++){
		pTemp = pTemp->pNext;
	}
	return &pTemp->data;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
void cList<T>::deleteAllData()
{
	stNODE *pTemp = m_pHead;
	for(int i=m_iCnt; i>1; i--){
		m_pHead = pTemp;
		for(int k=0; k<i-2; k++){
			m_pHead = m_pHead->pNext;
		}

		delete m_pHead->pNext;		// ÇöÀç µ¥ÀÌÅ¸¸¦ Áö¿ì´Â °ÍÀÌ¾Æ´Ï¶ó ´ÙÀ½ µ¥ÀÌÅ¸¸¦ delete
		m_pHead->pNext = NULL;
	}

	delete m_pHead;
	m_pHead = NULL;
	m_iCnt = 0;
}



// ===============================================================================================================================
// ========================================================= cQueue ==============================================================
// ===============================================================================================================================
//	2005-09-22 Kwon Yong Jae (amadeusj@gmail.com)
//	Queue
//	³»ºÎÀûÀ¸·Î CS Ã³¸® ¾ÈÇÔ (¾²·¹µå Áö¿ø¾ÈÇÔ)
//	Ã³À½ »ç¿ëÇÒ¶§ init() ¸¦ È£ÃâÇØ¾ßÇÑ´Ù
// ===============================================================================================================================

template <typename T>
class cQueue  
{
private:
	T *m_pData;
	int m_iTail;
	int m_iFront;
	int m_iSize;
	int m_iDataCnt;
	cLog *m_pcLog;

public:
	cQueue();
	~cQueue();
	void init(int _iSize, cLog *_pcLog);
	void destroy();
	BOOL push(T *_pData);
	BOOL pop(T *_pData, BOOL _bKeepData = FALSE);	
	BOOL isEmpty();
	BOOL isFull();
	int getCount();
	void deleteAllData();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
cQueue<T>::cQueue()
{
	m_pData = NULL;
	m_pcLog = NULL;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
cQueue<T>::~cQueue()
{
	destroy();	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
void cQueue<T>::init(int _iSize, cLog *_pcLog)
{
	if(m_pData){
		TCHAR szPrint[256];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cQueue::init(), Already call cQueue::init()"));
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return;
	}
	m_pcLog = _pcLog;
	m_iSize = _iSize;
	m_pData = new T[m_iSize];
	deleteAllData();	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
void cQueue<T>::destroy()
{
	if(m_pData){
		delete[] m_pData;
		m_pData = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
int cQueue<T>::getCount()
{
	return m_iDataCnt;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
BOOL cQueue<T>::isEmpty()
{
	if(m_iDataCnt == 0){
		return TRUE;
	}else{
		return FALSE;
	}	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
BOOL cQueue<T>::isFull()
{
	if(m_iSize == m_iDataCnt){
		return TRUE;
	}else{
		return FALSE;
	}	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
BOOL cQueue<T>::push(T *_pData)
{
	if(isFull()){
		TCHAR szPrint[512];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cQueue::push(), Queue Full, Size: %d"), m_iSize);
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return FALSE;
	}	

	memcpy(&m_pData[m_iTail], _pData, sizeof(T));		
	++m_iTail;	
	if(m_iTail == m_iSize){		// m_iTail °¡ °¡Àå »ó´Ü¿¡ ÀÖÀ»°æ¿ì (¹è¿­ÀÇ ³¡)
		m_iTail = 0;
	}
	++m_iDataCnt;
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
BOOL cQueue<T>::pop(T *_pData, BOOL _bKeepData)
{
	if(isEmpty()){
		TCHAR szPrint[512];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cQueue::pop(), Queue Empty"));
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return FALSE;
	}
	memcpy(_pData, &m_pData[m_iFront], sizeof(T));	
	if(!_bKeepData){
		++m_iFront;
		if(m_iFront == m_iSize){	// m_iFront °¡ °¡Àå »ó´Ü¿¡ ÀÖÀ»°æ¿ì (¹è¿­ÀÇ ³¡)
			m_iFront = 0;
		}
		--m_iDataCnt;
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
void cQueue<T>::deleteAllData()
{
	m_iTail = 0;
	m_iFront = 0;	
	m_iDataCnt = 0;
	memset(m_pData, 0, sizeof(T) * m_iSize);
}



// ===============================================================================================================================
// ========================================================= cStack ==============================================================
// ===============================================================================================================================
//	2005-09-22 Kwon Yong Jae (amadeusj@gmail.com)
//	Stack
//	³»ºÎÀûÀ¸·Î CS Ã³¸® ¾ÈÇÔ (¾²·¹µå Áö¿ø¾ÈÇÔ)
//	Ã³À½ »ç¿ëÇÒ¶§ init() ¸¦ È£ÃâÇØ¾ßÇÑ´Ù
// ===============================================================================================================================

template <typename T>
class cStack  
{
private:	
	T * m_pData;
	int m_iSize;
	int m_iTop;	
	int m_iDataCnt;
	cLog *m_pcLog;

public:
	cStack();
	~cStack();
	void init(int _iSize, cLog *_pcLog);
	void destroy();
	BOOL isEmpty();
	BOOL isFull();	
	BOOL push(T *_pData);
	BOOL pop(T *_pData, BOOL _bKeepData = FALSE);
	int getCount();
	void deleteAllData();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
cStack<T>::cStack()
{
	m_pData = NULL;
	m_pcLog = NULL;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
cStack<T>::~cStack()
{
	destroy();	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
void cStack<T>::init(int _iSize, cLog *_pcLog)
{
	if(m_pData){
		TCHAR szPrint[256];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cStack::init(), Already call cStack::init()"));
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return;
	}
	m_pcLog = _pcLog;
	m_pData = new T[_iSize];
	m_iSize = _iSize;
	deleteAllData();	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
void cStack<T>::destroy()
{
	if(m_pData){
		delete[] m_pData;
		m_pData = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
int cStack<T>::getCount()
{
	return m_iDataCnt;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
BOOL cStack<T>::isEmpty()
{
	return (m_iTop == 0);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
BOOL cStack<T>::isFull()
{
	return (m_iTop == m_iSize);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
BOOL cStack<T>::push(T *_pData)
{
	if(m_iTop < m_iSize){
		memcpy(&m_pData[m_iTop++], _pData, sizeof(T));		 
		++m_iDataCnt;
		return TRUE;
	}else{
		TCHAR szPrint[512];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cStack::push(), Stack Full, Size: %d"), m_iSize);
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return FALSE;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
BOOL cStack<T>::pop(T *_pData, BOOL _bKeepData)
{
	if(m_iTop > 0){
		if(!_bKeepData){
			memcpy(_pData, &m_pData[--m_iTop], sizeof(T));		
			--m_iDataCnt;
		}else{
			memcpy(_pData, &m_pData[m_iTop - 1], sizeof(T));		
		}
		return TRUE;
	}else{
		TCHAR szPrint[512];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cStack::pop(), Stack Empty"));
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return FALSE;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
void cStack<T>::deleteAllData()
{
	m_iTop = 0;
	m_iDataCnt = 0;
	memset(m_pData, 0, sizeof(T) * m_iSize);
}




// ===============================================================================================================================
// =================================================== cDataManager ==============================================================
// ===============================================================================================================================
//	2007-04-02 Kwon Yong Jae (amadeusj@gmail.com)
//	cDataManager
//	Ã³À½ »ç¿ë½Ã init() ¸¦ È£ÃâÇØ¾ßÇÑ´Ù
//	»ç¿ëµÈ ¹è¿­ÀÇ ÀÎµ¦½º¿Í ½¬°í ÀÖ´Â ¹è¿­ÀÇ ÀÎµ¦½º¸¦ °ü¸®ÇÑ´Ù (½ÇÁ¦ µ¥ÀÌÅ¸µµ ³»ºÎ¿¡ °¡Áö°í ÀÖ´Ù)
//	»ç¿ëµÈ ¹è¿­¿¡¼­ Áß°£¿¡ µ¥ÀÌÅ¸°¡ ºüÁö¸é °¡ÀåµÚÀÇ µ¥ÀÌÅ¸¸¦ ºó Áß°£À¸·Î ³¢¿ö³Ö´Â´Ù
//	»ç¿ë¾ÈµÈ µ¥ÀÌÅ¸´Â Å¥¸¦ »ç¿ëÇÑ´Ù (ÇÑ¹ø »ç¿ëÇÑ ÀÎµ¦½º°¡ ÇØÁ¦µÇ¸é ±×ÀÎµ¦½º´Â °¡Àå ¸¶Áö¸·¿¡ »ç¿ëÇÏ±â À§ÇØ)
//	³»ºÎÀûÀ¸·Î map À»»ç¿ëÇÑ´Ù
//	Ex)
//  manager.enterCS();	// getUseArrIdx() »ç¿ë½Ã Áß¿äÇÑ °÷ÀÏ¶§ CS Ã³¸®¸¦ ÇØÁÖ¾î¾ß ÇÑ´Ù
//	const int *pArrIdx = manager.getUseArrIdx();
//	for(int i=0; i<manager.getUseArrCnt(); i++){
//		pArrIdx[i];
//      if(pArrIdx[i] == 2){
//          manager.releaseArrIdx(&aa);
//          --i;		// µ¥ÀÌÅ¸¸¦ Á¦°ÅÇßÀ¸¹Ç·Î --i
//      }
//	}
//  manager.leaveCS();
// ===============================================================================================================================
template <typename DataType, typename UniqueKey>
class cDataManager
{
private:
	typedef	std::map<UniqueKey, int> MapSearch;
	typedef	std::pair<UniqueKey, int> PairSearch;	

	int *m_piUseArrIdx;				// »ç¿ëµÈ ¹è¿­ ÀÎµ¦½º
	int m_iUseArrCnt;				// »ç¿ëµÈ ¹è¿­ ¼ö	
	int m_iArrSize;
	int m_iArrStartIdx;				// ¹è¿­ÀÇ ½ÃÀÛ ÀÎµ¦½º (0 ÀÌ ¾Æ´Ò¼ö ÀÖ´Ù)
	BOOL m_bUseCriticalSection;
	cQueue<int> m_cRestArrIdx;		// ¹è¿­¿¡¼­ ºó µ¥ÀÌÅ¸ °ü¸®, Å¥¸¦ »ç¿ëÇÑ ÀÌÀ¯´Â ÇÑ¹ø »ç¿ëÇÑ ÀÎµ¦½º°¡ ÇØÁ¦µÇ¸é ±×ÀÎµ¦½º´Â °¡Àå ¸¶Áö¸·¿¡ »ç¿ëÇÏ±â À§ÇØ
	cLog *m_pcLog;	
	CRITICAL_SECTION *m_pCS;
	DataType *m_pData;				// ½ÇÁ¦ µ¥ÀÌÅ¸

	// map °ü·Ã µ¥ÀÌÅ¸
	BOOL m_bUseSearchMap;
	MapSearch m_mapSearch;
	UniqueKey *m_pUniqueKey;		// map ¿¡ »ç¿ëµÇ´Â Key¸¦ °¡Áö°í ÀÖ´Â´Ù releaseArrIdx() ¿¡¼­ ¹è¿­ÀÎµ¦½º¸¸À¸·Î Ã³¸®ÇÏ±â À§ÇØ
	
public:
	cDataManager();
	~cDataManager();
	
	void init(int _iArrSize, BOOL _bUseSearchMap, int _iArrStartIdx, BOOL _bUseCriticalSection, cLog *_pcLog);
	void destroy();
	void deleteAllData();
	DataType *getData(int _iArrIdx);

	int getUseArrCnt();
	const int *getUseArrIdx();	

	int allocArrIdx(UniqueKey _uniqueKey);
	BOOL releaseArrIdx(int _iArrIdx);
	int searchMap(UniqueKey _uniqueKey);

	void enterCS();
	void leaveCS();
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
cDataManager<DataType, UniqueKey>::cDataManager()
{
	m_bUseCriticalSection = FALSE;
	m_pData = NULL;
	m_pCS = NULL;
	m_pcLog = NULL;
	m_piUseArrIdx = NULL;
	m_pUniqueKey = NULL;
	m_bUseSearchMap = FALSE;
	m_iUseArrCnt = 0;
	m_iArrSize = 0;
	m_iArrStartIdx = 0;	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
cDataManager<DataType, UniqueKey>::~cDataManager()
{
	destroy();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: ¹è¿­ÀÇ ½ÃÀÛÀÎµ¦½º°¡ 0ÀÌ ¾Æ´Ò¼öµµ ÀÖ´Ù
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
void cDataManager<DataType, UniqueKey>::init(int _iArrSize, BOOL _bUseSearchMap, int _iArrStartIdx, BOOL _bUseCriticalSection, cLog *_pcLog)
{
	if(m_piUseArrIdx){
		TCHAR szPrint[256];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cDataManager::init(), Already call cDataManager::init()"));
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return;
	}

	m_bUseCriticalSection = _bUseCriticalSection;
	// if(m_bUseCriticalSection){		// »ç¿ë¾ÈÇÏ´õ¶óµµ ÀÏ´Ü »ý¼ºÇÑ´Ù, ¿ÜºÎ¿¡¼­ enterCS()¸¦ µû·Î »ç¿ëÇÒ¼ö ÀÖÀ¸¹Ç·Î
		m_pCS = new CRITICAL_SECTION;
		InitializeCriticalSection(m_pCS);		
	// }
	
	m_pcLog = _pcLog;
	m_bUseSearchMap = _bUseSearchMap;
	m_iArrSize = _iArrSize;		
	m_iArrStartIdx = _iArrStartIdx;
	m_cRestArrIdx.init(_iArrSize, _pcLog);
	m_piUseArrIdx = new int[_iArrSize];	

	// ½ÇÁ¦ »ç¿ëÇÏ´Â ¹è¿­ÀÇ À§Ä¡¿Í °°Àº À§Ä¡¿¡ µ¥ÀÌÅ¸°¡ µé¾î°¡¾ßÇÏ¹Ç·Î ¹è¿­ÀÇ Å©±â´Â _iArrSize + _iArrStartIdx (¾à°£ÀÇ ³¶ºñ°¡ ÀÖÀ»¼ö ÀÖÁö¸¸ ±×³É ¹«½ÃÇÑ´Ù)
	m_pUniqueKey = new UniqueKey[_iArrSize + _iArrStartIdx];
	m_pData = new DataType[_iArrSize + _iArrStartIdx];	

	deleteAllData();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
void cDataManager<DataType, UniqueKey>::destroy()
{
	if(m_pCS){
		DeleteCriticalSection(m_pCS);
		delete m_pCS;
		m_pCS = NULL;
	}				

	if(m_piUseArrIdx){
		delete[] m_piUseArrIdx;
		m_piUseArrIdx = NULL;
	}

	if(m_pData){
		delete[] m_pData;
		m_pData = NULL;
	}

	if(m_pUniqueKey){
		delete[] m_pUniqueKey;
		m_pUniqueKey = NULL;
	}

	m_cRestArrIdx.destroy();
	m_mapSearch.clear();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
int cDataManager<DataType, UniqueKey>::getUseArrCnt()
{
	return m_iUseArrCnt;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
const int *cDataManager<DataType, UniqueKey>::getUseArrIdx()
{
	return m_piUseArrIdx;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: map À» »ç¿ëÇÏÁö ¾ÊÀ» °æ¿ì _uniqueKey ´Â 0À» ÁÖ¸éµÈ´Ù
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
int cDataManager<DataType, UniqueKey>::allocArrIdx(UniqueKey _uniqueKey)
{
	if(m_bUseCriticalSection){
		EnterCriticalSection(m_pCS);
	}
	
	if(m_iUseArrCnt == m_iArrSize){
		if(m_bUseCriticalSection){
			LeaveCriticalSection(m_pCS);
		}
		TCHAR szPrint[256];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cDataManager::allocArrIdx(), m_iUseArrCnt: %d, m_iArrSize: %d"), m_iUseArrCnt, m_iArrSize);
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		return -1;
	}

	if(m_bUseSearchMap){
		// ÀÌ¹Ì µî·ÏµÇ¾îÀÖ´ÂÁö È®ÀÎÇØ º»´Ù;
		MapSearch::iterator iter = m_mapSearch.find(_uniqueKey);	
		if(iter != m_mapSearch.end()){
			if(m_bUseCriticalSection){
				LeaveCriticalSection(m_pCS);
			}
			TCHAR szPrint[256];
			_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cDataManager::allocArrIdx(), Already registered"));
			cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
			if(m_pcLog){			// Log
				m_pcLog->writeLog(szPrint);
			}
			return -1;
		}
	}

	int iArrIdx = 0;
	m_cRestArrIdx.pop(&iArrIdx);
	m_piUseArrIdx[m_iUseArrCnt++] = iArrIdx;
	m_pUniqueKey[iArrIdx] = _uniqueKey;

	if(m_bUseSearchMap){
		m_mapSearch.insert(PairSearch(_uniqueKey, iArrIdx));		
	}

	if(m_bUseCriticalSection){
		LeaveCriticalSection(m_pCS);
	}

	return iArrIdx;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
BOOL cDataManager<DataType, UniqueKey>::releaseArrIdx(int _iArrIdx)
{
	if(_iArrIdx < m_iArrStartIdx  ||  _iArrIdx >= (m_iArrSize + m_iArrStartIdx)){
		assert(0);
		return FALSE;
	}

	if(m_bUseCriticalSection){
		EnterCriticalSection(m_pCS);
	}

	if(m_iUseArrCnt == 0){
		if(m_bUseCriticalSection){
			LeaveCriticalSection(m_pCS);
		}
		TCHAR szPrint[256];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cDataManager::releaseArrIdx(), m_iUseArrCnt: 0"));
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return FALSE;
	}

	if(m_bUseSearchMap){		
		// µî·ÏµÇ¾îÀÖ´ÂÁö È®ÀÎÇØ º»´Ù;		
		MapSearch::iterator iter = m_mapSearch.find(m_pUniqueKey[_iArrIdx]);	
		if(iter == m_mapSearch.end()){
			TCHAR szPrint[256];
			_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cDataManager::releaseArrIdx(), Not find data"));
			cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
			if(m_pcLog){			// Log
				m_pcLog->writeLog(szPrint);
			}
			assert(0);
			return FALSE;
		}	
		m_mapSearch.erase(iter);		
	}

	// °¡ÀåµÚÀÇ µ¥ÀÌÅ¸¸¦ ºó Áß°£À¸·Î ³¢¿ö³Ö´Â´Ù
	BOOL bFind = FALSE;
	for(int i=0; i<m_iUseArrCnt; i++){
		if(m_piUseArrIdx[i] == _iArrIdx){
			if(m_iUseArrCnt - 1 == i){		// °¡Àå ¸¶Áö¸· µ¥ÀÌÅ¸ÀÏ°æ¿ì
				m_piUseArrIdx[i] = 0;
				//if(m_bUseSearchMap){		// m_pUniqueKey µ¥ÀÌÅ¸´Â Æ¯º°È÷ ÃÊ±âÈ­ÇÒÇÊ¿ä ¾ø´Ù
				//	m_pUniqueKey[i] = 0;
				//}
			}else{
				m_piUseArrIdx[i] = m_piUseArrIdx[m_iUseArrCnt - 1];
				m_piUseArrIdx[m_iUseArrCnt - 1] = 0;					
				//if(m_bUseSearchMap){		// m_pUniqueKey µ¥ÀÌÅ¸´Â Æ¯º°È÷ ÃÊ±âÈ­ÇÒÇÊ¿ä ¾ø´Ù
				//	m_pUniqueKey[i] = m_pUniqueKey[iUseArrCnt - 1];
				//	m_pUniqueKey[iUseArrCnt - 1] = 0;											
				//}
			}
			bFind = TRUE;
			--m_iUseArrCnt;
			break;	
		}
	}

	// »ç¿ëµÇÁö ¾ÊÀº ÀÎµ¦½ºÀÌ´Ù
	if(!bFind){
		if(m_bUseCriticalSection){
			LeaveCriticalSection(m_pCS);
		}
		TCHAR szPrint[512];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cDataManager::releaseArrIdx, Unused ArrIdx: %d"), _iArrIdx);
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return FALSE;
	}

	// Rest Queue ¿¡ µ¥ÀÌÅ¸¸¦ ³Ö¾îµÐ´Ù
	m_cRestArrIdx.push(&_iArrIdx);

	if(m_bUseCriticalSection){
		LeaveCriticalSection(m_pCS);
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: ¹è¿­ÀÎµ¦½º¸¦ ¸®ÅÏÇØÁØ´Ù
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
int cDataManager<DataType, UniqueKey>::searchMap(UniqueKey _uniqueKey)
{
	if(!m_bUseSearchMap){
		return -1;
	}

	if(m_bUseCriticalSection){
		EnterCriticalSection(m_pCS);
	}

	MapSearch::iterator iter = m_mapSearch.find(_uniqueKey);	
	if(iter == m_mapSearch.end()){
		if(m_bUseCriticalSection){
			LeaveCriticalSection(m_pCS);
		}
		return -1;
	}		

	if(m_bUseCriticalSection){
		LeaveCriticalSection(m_pCS);
	}

	return iter->second;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
void cDataManager<DataType, UniqueKey>::deleteAllData()
{	
	if(m_bUseCriticalSection){
		EnterCriticalSection(m_pCS);
	}

	m_iUseArrCnt = 0;
	memset(m_piUseArrIdx, 0, sizeof(int) * m_iArrSize);
	m_cRestArrIdx.deleteAllData();

	// Rest Queue ¿¡ µ¥ÀÌÅ¸¸¦ ³Ö¾îµÐ´Ù
	int iMin = m_iArrStartIdx;
	int iMax = m_iArrSize + m_iArrStartIdx;	
	for(int i=iMin; i<iMax; i++){		
		m_cRestArrIdx.push(&i);
	}

	if(m_bUseSearchMap){
		m_mapSearch.clear();
		// memset(m_pUniqueKey, 0, sizeof(UniqueKey) * m_iArrSize);	// ÇØ´ç µ¥ÀÌÅ¸´Â ÃÊ±âÈ­ÇÏÁö ¾Ê´Â´Ù, string ÀÏ¼öµµ ÀÖÀ¸¹Ç·Î
	}

	if(m_bUseCriticalSection){
		LeaveCriticalSection(m_pCS);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
DataType *cDataManager<DataType, UniqueKey>::getData(int _iArrIdx)
{
	if(_iArrIdx < 0  ||  _iArrIdx >= (m_iArrSize + m_iArrStartIdx)){		// _iArrIdx < m_iArrStartIdx °¡ ¾Æ´Ñ ÀÌÀ¯´Â Å×½ºÆ® ¸ñÀûÀ¸·Î getData(0) À»ÇÒ¼ö ÀÖ´Ù
		assert(0);
		return FALSE;
	}
	return &m_pData[_iArrIdx];
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
void cDataManager<DataType, UniqueKey>::enterCS()
{
	EnterCriticalSection(m_pCS);	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
void cDataManager<DataType, UniqueKey>::leaveCS()
{
	LeaveCriticalSection(m_pCS);	
}





// ===============================================================================================================================
// ===================================================== cIOCPQueue ==============================================================
// ===============================================================================================================================
//	2009-02-23 Kwon Yong Jae (amadeusj@gmail.com)
//	cIOCPQueue
//	http://blog.naver.com/ayrun?Redirect=Log&logNo=100035477757
//	ÇØ´ç »çÀÌÆ®¸¦ ±â¹ÝÀ¸·Î ¸¸µë
//	pop() µ¥ÀÌÅ¸°¡ ÀÖÀ»¶§±îÁö Block »óÅÂ¸¦ À¯ÁöÇÑ´Ù
//	cIOCPQueue ¸¸À¸·Î´Â push() ÇßÀ»°æ¿ì µ¥ÀÌÅ¸¸¦ °¡Áö°í ÀÖ´Â°Ô ¾Æ´Ï¶ó Æ÷ÀÎÅÍ¸¸ °¡Áö°í ÀÖÀ¸¹Ç·Î ´Ù¸¥ ¹è¿­¿¡ µ¥ÀÌÅ¸¸¦ °ü¸®ÇÑ´Ù
// ===============================================================================================================================
template<typename T>
class cIOCPQueue
{
private:	
	HANDLE m_hIOCP;	
	cLog *m_pcLog;
	BOOL m_bUseCriticalSection;
	CRITICAL_SECTION *m_pCS;
	cDataManager<T, int> m_cArrManagerData;

public:
	cIOCPQueue();
	~cIOCPQueue();
	void init(int _iSize, BOOL _bUseCriticalSection, cLog *_pcLog);
	BOOL push(T *_pData);
	BOOL pop(T *_pData, DWORD *_pdwErrorCode, DWORD _dwTimeout = INFINITE);

	inline int getDataCount(){return m_cArrManagerData.getUseArrCnt();}
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
cIOCPQueue<T>::cIOCPQueue()
{
	m_hIOCP = NULL;
	m_pcLog = NULL;	
	m_bUseCriticalSection = FALSE;
	m_pCS = NULL;	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
cIOCPQueue<T>::~cIOCPQueue()
{
	if(m_hIOCP){
		CloseHandle(m_hIOCP);
		m_hIOCP = NULL;
	}
	if(m_pCS){
		DeleteCriticalSection(m_pCS);
		delete m_pCS;
		m_pCS = NULL;
	}
	m_cArrManagerData.destroy();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
void cIOCPQueue<T>::init(int _iSize, BOOL _bUseCriticalSection, cLog *_pcLog)   
{
	if(m_hIOCP){
		TCHAR szPrint[256];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cIOCPQueue::init(), Already call cIOCPQueue::init()"));
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return;
	}

	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	assert(m_hIOCP != NULL);

	m_cArrManagerData.init(_iSize, 0, 0, TRUE, _pcLog);		// cIOCPQueue::m_bUseCriticalSection ¿Í´Â º°°³·Î cDataManager ´Â ¹«Á¶°Ç TRUE ¸¦ Àû¿ëÇÑ´Ù (cIOCPQueue ´Â ¾²·¹µå¸¦ »ç¿ëÇÏ´õ¶óµµ FALSE ¸¦ ÁÙ¼ö ÀÖ´Ù)

	m_bUseCriticalSection = _bUseCriticalSection;
	if(m_bUseCriticalSection){
		m_pCS = new CRITICAL_SECTION;
		InitializeCriticalSection(m_pCS);
	}	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
BOOL cIOCPQueue<T>::push(T *_pData)
{
	// µ¥ÀÌÅ¸ ³Ö´Â ¼ø¼­¶§¹®¿¡ CS Ã³¸®¸¦ ÇÑ´Ù
	if(m_bUseCriticalSection){
		EnterCriticalSection(m_pCS);
	}

	int iArrIdx = m_cArrManagerData.allocArrIdx(0);
	if(iArrIdx == -1){
		if(m_bUseCriticalSection){
			LeaveCriticalSection(m_pCS);
		}
		return FALSE;
	}	

	memcpy(m_cArrManagerData.getData(iArrIdx), _pData, sizeof(T));
	BOOL dwRet = PostQueuedCompletionStatus(m_hIOCP, 0, (ULONG_PTR)iArrIdx, NULL);		// ¹è¿­ÀÎµ¦½º¸¦ ÀúÀåÇÑ´Ù
	
	if(m_bUseCriticalSection){
		LeaveCriticalSection(m_pCS);
	}

	if(!dwRet){
		m_cArrManagerData.releaseArrIdx(iArrIdx);		// Ãë¼Ò

		TCHAR szPrint[512];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cIOCPQueue::push(), ErrorCode: %d"), GetLastError());
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return FALSE;
	}
	return dwRet;	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: ¿©±â¿¡´Â CS Ã³¸®°¡ ÇÊ¿ä ¾ø´Ù
//       *_pdwErrorCode == WAIT_TIMEOUT   // ½Ã°£ÃÊ°ú
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
BOOL cIOCPQueue<T>::pop(T *_pData, DWORD *_pdwErrorCode, DWORD _dwTimeout)
{
	LONGLONG llArrIdx;		// 64 Bit ÀÏ°æ¿ì Æ÷ÀÎÅÍ´Â 8¹ÙÀÌÆ®ÀÌ¹Ç·Î
	DWORD dwTemp = 0;	   
	LPOVERLAPPED pTempOverlapped = NULL;
	BOOL ret = GetQueuedCompletionStatus(m_hIOCP, &dwTemp, (PULONG_PTR)&llArrIdx, &pTempOverlapped, _dwTimeout);
	if(ret){  
		int iArrIdx = (int)llArrIdx;
		*_pdwErrorCode = 0;
		memcpy(_pData, m_cArrManagerData.getData(iArrIdx), sizeof(T));
		m_cArrManagerData.releaseArrIdx(iArrIdx);
		return TRUE;
	}else{
		*_pdwErrorCode = GetLastError();
		if(*_pdwErrorCode == WAIT_TIMEOUT){		// ½Ã°£ ÃÊ°ú
			return FALSE;
		}
		
		TCHAR szPrint[512];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cIOCPQueue::pop(), ErrorCode: %d"), *_pdwErrorCode);
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return FALSE;
	}
}




// ===============================================================================================================================
// ===================================================== cHashTable ==============================================================
// ===============================================================================================================================
//	2008-08-23 Kwon Yong Jae (amadeusj@gmail.com)
//	cHashTable
//	http://gpgstudy.com/forum/viewtopic.php?t=795&postdays=0&postorder=asc&highlight=%C7%D8%BD%C3&start=0
//	ÇØ´ç »çÀÌÆ®ÀÇ Å¬·¡½º¸¦ ±â¹ÝÀ¸·Î ¸¸µë
//	2Â÷¿ø ¹è¿­À» ÀÀ¿ëÇÏ¿© ¸ÕÀú ÇÑÂÊÀº HashTable ÀÇ ÄÚµå°ªÀ» ³Ö´Â´Ù
//	³ª¸ÓÁö ÇÑÂÊÀº °°Àº HashCode ÀÇ µ¥ÀÌÅ¸ °ªÀÌ µé¾î°£´Ù (¹è¿­ÀÇ ±æÀÌ°¡ ³ÑÄ¡¸é ¹è¿­À» ´Ù½Ã »ý¼ºÇÑ´Ù)
//	°°Àº Key °ªÀ» ³ÖÀ»¼ö ¾ø´Ù
//	³»ºÎÀûÀ¸·Î CS Ã³¸® ¾ÈÇÔ (¾²·¹µå Áö¿ø¾ÈÇÔ)
//	Ã³À½ »ç¿ëÇÒ¶§ init() ¸¦ È£ÃâÇØ¾ßÇÑ´Ù
//	find() Ã³¸®½Ã ÇØ´çµ¥ÀÌÅ¸°¡ ¾øÀ»¶§ NULL À» ¸®ÅÏÇÑ´Ù
//	inert() ¿¡¼­¸¸ new »ý¼ºÇÑ´Ù, remove() ¿¡¼­´Â delete Ã³¸®ÇÏÁö ¾Ê°í ¼Ò¸êÀÚ¿¡¼­ Ã³¸®ÇÑ´Ù (new »ý¼ºµÈ°ÍÀº °è¼ÓÇØ¼­ ÀçÈ°¿ëÇÑ´Ù)
// 
//	bucket_0: slot_0, slot_1, slot_2, ..., slot_n
//	bocket_1: slot_0, ...
//	...
//	bocket_n: slot_0, ...
// ===============================================================================================================================
template <class T>
class cHashTable{
private:
	typedef enum _enumBASETYPE
	{
		eDEFAULTBUCKET_SIZE		= 400, 	
	}enumBASETYPE;

	// ½ÇÁ¦ µ¥ÀÌÅ¸°¡ µé¾î°¡´Â ±¸Á¶Ã¼
	typedef struct _stSLOTINFO
	{
		TCHAR *pszKey;
		T data;		
	}stSLOTINFO;

	// HashTable À» °ü¸®ÇÏ´Â ±¸Á¶Ã¼
	typedef struct _stBUCKETINFO
	{
		int iDataCnt;				// Bucket ÀÇ ÇöÀç µ¥ÀÌÅ¸¼ö
		int iArrReserveSize;		// Bucket ÀÇ »ý¼ºµÇ¾îÀÖ´Â ¹è¿­ÀÇ¼ö
	}stBUCKETINFO;

	stSLOTINFO **m_ppstBucket;
	stBUCKETINFO *m_pBucketInfo;
	int m_iBucketSize;
	int m_iDataCnt;
	int m_iKeySize;
		
private:		
	T find(const TCHAR *_pszKey, int _iHashCode);
	
public:
	cHashTable();
	~cHashTable();	
	void init(int _iKeySize, int _iBucketSize = eDEFAULTBUCKET_SIZE);	
	BOOL insert(const TCHAR *_pszKey, T *_pValue);
	BOOL remove(const TCHAR *_pszKey);
	void clear();
	T find(const TCHAR *_pszKey);

	static long hash(const TCHAR *_pszKey);	
	static long hash(const void *_pData, int _iDataSize);	
	
	inline int size(){return m_iCnt;}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
cHashTable<T>::cHashTable()
{
	m_iDataCnt = 0;
	m_iKeySize = 0;
	m_ppstBucket = NULL;
	m_pBucketInfo = NULL;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
cHashTable<T>::~cHashTable()
{
	if(m_ppstBucket){
		for(int i=0; i<m_iBucketSize; i++){
			for(int k=0; k<m_pBucketInfo[i].iArrReserveSize; k++){
				if(m_ppstBucket[i][k].pszKey){				// ÇÒ´çÇÑ°Í¸¸ delete Ã³¸®ÇÑ´Ù
					delete []m_ppstBucket[i][k].pszKey;
				}
			}
			delete []m_ppstBucket[i];
		}
	}
	delete []m_ppstBucket;

	if(m_pBucketInfo){
		delete []m_pBucketInfo;
	}
}

	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
void cHashTable<T>::init(int _iKeySize, int _iBucketSize)
{
	m_iKeySize = _iKeySize;
	m_iBucketSize = _iBucketSize;
	int iArrReserveSize = m_iBucketSize / 2;
	m_iDataCnt = 0;
	m_ppstBucket = new stSLOTINFO *[m_iBucketSize];
	m_pBucketInfo = new stBUCKETINFO[m_iBucketSize];
	for(int i=0; i<m_iBucketSize; i++){
		m_pBucketInfo[i].iDataCnt = 0;
		m_pBucketInfo[i].iArrReserveSize = iArrReserveSize;
		m_ppstBucket[i] = new stSLOTINFO[iArrReserveSize];
		memset(m_ppstBucket[i], 0, sizeof(stSLOTINFO) * iArrReserveSize);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : ¹®ÀÚ¿­ Hash
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
long cHashTable<T>::hash(const TCHAR *_pszKey)
{
	// djb2 hash algorithm, http://www.cs.yorku.ca/~oz/hash.html
	unsigned long hash_code = 5381;
	int c;
	while(c = *_pszKey++){
		hash_code = ((hash_code << 5) + hash_code) + c;		// hash * 33 + c
	}
	return labs(hash_code);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : ±âÅ¸ µ¥ÀÌÅ¸ Hash
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
long cHashTable<T>::hash(const void *_pData, int _iDataSize)
{
	// djb2 hash algorithm, http://www.cs.yorku.ca/~oz/hash.html
	char *p = (char *)_pData;
	unsigned long hash_code = 5381;
	for(int i=0; i<_iDataSize; i++){
		hash_code = ((hash_code << 5) + hash_code) + p[i];
	}
	return labs(hash_code);	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
BOOL cHashTable<T>::insert(const TCHAR *_pszKey, T *_pValue)
{
	if((int)_tcslen(_pszKey) >= m_iKeySize){
		return FALSE;
	}

	// °°Àº µ¥ÀÌÅ¸°¡ ÀÖ´ÂÁö È®ÀÎÇÑ´Ù
	int iHashCode = hash(_pszKey) % m_iBucketSize;
	if(find(_pszKey, iHashCode)){
		return FALSE;
	}
	
	int iDataCnt = m_pBucketInfo[iHashCode].iDataCnt;

	// µ¥ÀÌÅ¸°¡ µé¾î°¥ °ø°£ÀÌ ÀÖ´ÂÁö È®ÀÎÇÑ´Ù
	if(iDataCnt == m_pBucketInfo[iHashCode].iArrReserveSize){	
		int iOldArrReserveSize = m_pBucketInfo[iHashCode].iArrReserveSize;
		int iArrReserveSize = iOldArrReserveSize * 2;									// 2¹èÀÇ Å©±â¸¦ ÇÒ´çÇÑ´Ù
		m_pBucketInfo[iHashCode].iArrReserveSize = iArrReserveSize;
		stSLOTINFO *pTemp = m_ppstBucket[iHashCode];									// ±âÁ¸ÀÇ µ¥ÀÌÅ¸ ÀÓ½Ã ÀúÀå
		m_ppstBucket[iHashCode] = new stSLOTINFO[iArrReserveSize];
		memset(m_ppstBucket[iHashCode], 0, sizeof(stSLOTINFO) * iArrReserveSize);
		memcpy(m_ppstBucket[iHashCode], pTemp, sizeof(stSLOTINFO) * iOldArrReserveSize);// ±âÁ¸ÀÇ µ¥ÀÌÅ¸ Àû¿ë
		delete []pTemp;		
	}

	// insert() ÇÒ¶§ »ý¼ºÀÌ ¾ÈµÇÀÖ´Ù¸é »ý¼ºÇØÁØ´Ù, delete ´Â ¼Ò¸êÀÚ¿¡¼­ ÇØÁØ´Ù
	if(!m_ppstBucket[iHashCode][iDataCnt].pszKey){
		m_ppstBucket[iHashCode][iDataCnt].pszKey = new TCHAR[m_iKeySize];
	}

	_tcscpy_s(m_ppstBucket[iHashCode][iDataCnt].pszKey, m_iKeySize, _pszKey);
	memcpy(&m_ppstBucket[iHashCode][iDataCnt].data, _pValue, sizeof(T));
	++m_pBucketInfo[iHashCode].iDataCnt;	

	++m_iDataCnt;

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
BOOL cHashTable<T>::remove(const TCHAR *_pszKey)
{
	int iArrIdx = -1;
	int iHashCode = hash(_pszKey) % m_iBucketSize;
	int iDataCnt = m_pBucketInfo[iHashCode].iDataCnt;	
	for(int i=0; i<iDataCnt; i++){
		if(_tcscmp(m_ppstBucket[iHashCode][i].pszKey, _pszKey) == 0){
			iArrIdx = i;
			break;
		}
	}
	if(iArrIdx == -1){		// ÇØ´ç µ¥ÀÌÅ¸°¡ ¾ø´Ù
		return FALSE;
	}

	// insert() ÇÒ¶§ pszKey ¸¦ new ·Î »ý¼ºÇÏ¿´Áö¸¸ ¿©±â¼­ delete Ã³¸®¸¦ ÇÏÁö¾Ê°í
	// ¼Ò¸êÀÚ¿¡¼­ Ã³¸®ÇÑ´Ù, ÇÑ¹ø »ý¼ºµÈ°ÍÀº delete Ã³¸®ÇÏÁö ¾Ê°í °è¼Ó »ç¿ëÇÑ´Ù

	// ¹è¿­ÀÇ °¡ÀåµÚ¿¡µ¥ÀÌÅ¸¸¦ ¿Å°ÜÁØ´Ù
	if(iArrIdx + 1 == iDataCnt){		// °¡ÀåµÚ¿¡ µ¥ÀÌÅ¸
		// ÃÊ±âÈ­ Ã³¸®ÇÏÁö ¾Ê´Â´Ù
		// memset(&m_ppstBucket[iHashCode][iArrIdx].data, 0, sizeof(T));							
		// memset(&m_ppstBucket[iHashCode][iArrIdx].pszKey, 0, sizeof(TCHAR) * m_iKeySize);
	}else{
		memcpy(&m_ppstBucket[iHashCode][iArrIdx].data, &m_ppstBucket[iHashCode][iDataCnt - 1].data, sizeof(T));
		_tcscpy_s(m_ppstBucket[iHashCode][iArrIdx].pszKey, m_iKeySize, m_ppstBucket[iHashCode][iDataCnt - 1].pszKey);
		
		// ÃÊ±âÈ­ Ã³¸®ÇÏÁö ¾Ê´Â´Ù
		// memset(&m_ppstBucket[iHashCode][iDataCnt - 1].data, 0, sizeof(T));
		// memset(&m_ppstBucket[iHashCode][iDataCnt - 1].pszKey, 0, sizeof(TCHAR) * m_iKeySize);
	}
	
	--m_pBucketInfo[iHashCode].iDataCnt;	
	--m_iDataCnt;
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
void cHashTable<T>::clear()
{
	if(m_iDataCnt == 0){
		return;
	}

	for(int iBucketIdx=0; iBucketIdx<m_iBucketSize; iBucketIdx++){		
		// ÃÊ±âÈ­ Ã³¸®ÇÏÁö ¾Ê´Â´Ù
		// for(int i=0; i<m_pBucketInfo[iBucketIdx].iDataCnt; i++){
		//	memset(&m_ppstBucket[iBucketIdx][i].data, 0, sizeof(T));						
		//	memset(&m_ppstBucket[iBucketIdx][i].pszKey, 0, sizeof(TCHAR) * m_iKeySize);
		// }
		m_pBucketInfo[iBucketIdx].iDataCnt = 0;			
	}	
	m_iDataCnt = 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
T cHashTable<T>::find(const TCHAR *_pszKey) 
{
	int iResult = hash(_pszKey) % m_iBucketSize;
	return find(_pszKey, iResult);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
T cHashTable<T>::find(const TCHAR *_pszKey, int _iHashCode) 
{
	int iDataCnt = m_pBucketInfo[_iHashCode].iDataCnt;	
	for(int i=0; i<iDataCnt; i++){
		if(_tcscmp(m_ppstBucket[_iHashCode][i].pszKey, _pszKey) == 0){
			return m_ppstBucket[_iHashCode][i].data;
		}
	}
	return NULL;
}





// ===============================================================================================================================
// ========================================================= cArray =============================================================
// ===============================================================================================================================
//	2006-12-29 Kwon Yong Jae (amadeusj@gmail.com)
//	µ¥ÀÌÅ¸ÀÇ Å©±â°¡ ÇÒ´çµÈ Å©±âº¸´Ù Ä¿Áö¸é ±âÁ¸ÀÇ µ¥ÀÌÅ¸´Â °Çµå¸®Áö ¾Ê°í(ÁÖ¼Ò¸¦ °Çµå¸®Áö ¾ÊÀ½)
//	»õ·Î¿î µ¥ÀÌÅ¸¸¦ ¶Ç ÇÒ´çÇÑ´Ù (Vector ÀÌ¿ë)
//	µ¥ÀÌÅ¸ÀÇ ¾çÀÌ Àû¾îÁö¸é ±âÁ¸ÀÇ µ¥ÀÌÅ¸ÀÇ Å©±â´Â °è¼Ó À¯ÁöµÈ´Ù
//	³»ºÎÀûÀ¸·Î CS Ã³¸® ÇÔ
//	T2 Key Value ´Â Unique ÇØ¾ßÇÔ (eDATAKEYNULL °ªÀÌ¸é ¾ÈµÈ´Ù), ÇÑ¹ø ÀÔ·ÂµÈ Key °ªÀº º¯°æµÇ¸é ¾ÈµÈ´Ù
//	Ã³À½ »ç¿ë½Ã init() ¸¦ È£ÃâÇØ¾ßÇÑ´Ù
//	Áß°£ÀÇ µ¥ÀÌÅ¸°¡ ¾ø¾îÁú¶§ »èÁ¦ÇÏ¸é¼­ µÚÀÇ µ¥ÀÌÅ¸¸¦ ¶¯±âÁö ¾Ê´Â´Ù ±×³É µ¥ÀÌÅ¸¸¸ 0À¸·Î ¸¸µç´Ù (cDataManager ¸¦ »ç¿ë)
//	cDataManager ¿¡¼­ ³»ºÎÀûÀ¸·Î µ¥ÀÌÅ¸ °Ë»ö½Ã BinarySearch ¸¦ ÀÌ¿ëÇÑ´Ù (µ¥ÀÌÅ¸¸¦ ÀÔ·Â, »èÁ¦¶§´Â ´À¸±¼ö ÀÖÀ¸³ª °Ë»ö½Ã ¼Óµµ°¡ ºü¸£´Ù), µ¥ÀÌÅ¸¸¦ ³ÖÀ»¶§´Â QuickSort ¸¦ ÀÌ¿ëÇÑ´Ù
//	Ç×»ó getData() ÈÄ¿¡´Â °ªÀÌ NULL ÀÎÁö È®ÀÎÇØ¾ßÇÑ´Ù (°ªÀ» Ã£À»¼ö ¾øÀ»¶§¸¦ À§ÇØ)
//	Ex)
//	int iArrIdx;
//	pstTest = cData.getData(&iKey, &iArrIdx);
//	if(!pstTest){
//		return 0;
//	}
//
//	¿ÜºÎ¿¡¼­ ¸ðµçµ¥ÀÌÅ¸¸¦ °Ë»çÇÒ¶§ (¸ðµç ¹è¿­À» °Ë»çÇÏ´Â°Ô ¾Æ´Ï¶ó µ¥ÀÌÅ¸°¡ µé¾îÀÖ´Â ¹è¿­¸¸ °Ë»çÇÑ´Ù)
//	Ex)
//	int iCnt = 0;
//	int iArrIdx;
//	int *piData = NULL;
//  cA.enterCS();			// getUseAllData() »ç¿ë½Ã Áß¿äÇÑ °÷ÀÏ¶§ CS Ã³¸®¸¦ ÇØÁÖ¾î¾ß ÇÑ´Ù
//	while(cA.getUseAllData(iCnt, &iArrIdx, &piData)){
//		int aa = *piData;		
//      if(aa == 5){
//         cA.deleteData(&aa);			// µ¥ÀÌÅ¸¸¦ »èÁ¦ÇÒ°æ¿ì¿¡´Â ++iCnt ¸¦ ÇÏÁö ¾Ê´Â´Ù
//      }else{
//         ++iCnt;
//      }
//	}
//  cA.leaveCS();
// ===============================================================================================================================

template <typename DataType, typename UniqueKey>
class cArray
{
private:
	typedef enum _enumBASETYPE
	{
		eDATAKEYNULL			= -1,							// DataÀÇ Å°°¡ 0ÀÌ »ç¿ëµÉ¼öµµ ÀÖÀ¸¹Ç·Î 
		eDATAPTRMANAGERSIZE		= 100,							// m_vecDataPtrManager ÀÇ ±âº» Å©±â (³Ë³ËÇÏ°Ô Àâ´Â´Ù, Å©±â¸¦ ³ÑÄ¡¸é ¹®Á¦µÉ°Å´Â ¾øÁö¸¸, º¤ÅÍÀÇ Å©±â°¡ ÇÒ´çµÈ Å©±â¸¦ ³ÑÄ¥¶§ º¤ÅÍÀÇ ÁÖ¼Ò°¡ º¯°æµÇ¸é ¹®Á¦°¡ »ý±è)
	}enumBASETYPE;

	// m_iDataDefaultSize »çÀÌÁî¸¸Å­ µ¥ÀÌÅ¸ÀÇ°¡ ÇÒ´çµÈ Å©±â¸¦ ³Ñ¾î°¡¸é ±âÁ¸ÀÇ µ¥ÀÌÅ¸´Â °Çµå¸®Áö ¾Ê°í 
	// »õ·Î ¶Ç m_iDataDefaultSize ¸¸Å­ÀÇ ¹è¿­À» »ý¼ºÇÑ´Ù, »õ·Î »ý¼ºµÈ ¹è¿­ÀÇ ÁÖ¼Ò¸¦ °ü¸®
	vector<DataType *> m_vecDataPtrManager;						// ½ÇÁ¦ µ¥ÀÌÅ¸

	vector<cDataManager<DataType, UniqueKey> *> m_vecArrayManagerPtrManager;	// ¹è¿­ÀÇ »ç¿ëÁßÀÎ ÀÎµ¦½º¿Í ºó ÀÎµ¦½º¸¦ °ü¸®
	
	BOOL m_bUseCriticalSection;
	CRITICAL_SECTION m_csData;
	vector<CRITICAL_SECTION *> m_vecCSPtrManager;				// ÇÏ³ªÀÇ µ¥ÀÌÅ¸¿¡ ´ëÇÑ ÇÏ³ªÀÇ CS		
	
	DataType m_dummyData;										// ÀÇ¹Ì ¾ø´Â µ¥ÀÌÅ¸
	int m_iDataDefaultSize;										// Data ÀÇ Å©±â(¹è¿­ÀÇ Å©±â¸¦ ´Ù»ç¿ëÇÏ¸é ´Ù½Ã m_iDataDefaultSize Å©±â¸¸Å­À» ´õÇÒ´çÇÑ´Ù)
	int m_iCurDataCnt;											// ÇöÀç »ç¿ëÁßÀÎ µ¥ÀÌÅ¸ÀÇ ¼ö
	int m_iMaxDataCnt;											// ¹è¿­ÀÇ ÃÑÅ©±â
	cLog *m_pcLog;
	BOOL m_bBinarySearch;
	
private:
	void resetDataSize();										// µ¥ÀÌÅ¸ÀÇ Å©±â°¡ º¯°æµÇ¾î¼­ »õ·Î µ¥ÀÌÅ¸¸¦ ÇÒ´çÇÑ´Ù(±âÁ¸ÀÇ µ¥ÀÌÅ¸´Â À¯Áö)
	void getArrIdx(int _piVirtualArrIdx, int *_piVecIdx, int *_piArrIdx);
	
public:
	cArray();
	~cArray();
	void init(int _iSize, BOOL _bBinarySearch, BOOL _bUseCriticalSection, cLog *_pcLog);
	int insertData(DataType *_pData, UniqueKey *_pKey);						// µ¥ÀÌÅ¸ µî·Ï	
	void deleteAllData();
	void deleteData(UniqueKey *_pKey);									// µ¥ÀÌÅ¸ »èÁ¦ (Å°°ªÀ» ÀÌ¿ë)
	void deleteData(int _iArrIdx);								// µ¥ÀÌÅ¸ »èÁ¦ (ÀÎµ¦½º¸¦ ÀÌ¿ë)
	DataType *getData(UniqueKey *_pKey, int *_piArrIdx);						// µ¥ÀÌÅ¸ °¡Á®¿À±â (Å°°ªÀ» ÀÌ¿ë)
	DataType *getData(int _iArrIdx);									// µ¥ÀÌÅ¸ °¡Á®¿À±â (ÀÎµ¦½º¸¦ ÀÌ¿ë)
	BOOL getUseAllData(int _iUseDataIdx, int *_piArrIdx, DataType **_ppData);	// ¿ÜºÎ¿¡¼­ cArray ÀÇ ¸ðµç µ¥ÀÌÅ¸¸¦ °Ë»çÇÒ¶§ »ç¿ë
	void enterCS(int _iArrIdx);									// ÇÏ³ªÀÇ µ¥ÀÌÅ¸¿¡ ´ëÇØ¼­ CS °É±â (CS ¸¦ °Ç´Ù´Â °ÍÀº µ¥ÀÌÅ¸ÀÇ ÀÎµ¦½º¸¦ ¾Ë°íÀÖ´Ù°í º»´Ù)
	void leaveCS(int _iArrIdx);									// ÇÏ³ªÀÇ µ¥ÀÌÅ¸¿¡ ´ëÇØ¼­ CS °É±â (CS ¸¦ °Ç´Ù´Â °ÍÀº µ¥ÀÌÅ¸ÀÇ ÀÎµ¦½º¸¦ ¾Ë°íÀÖ´Ù°í º»´Ù)
	void enterCS();												// ÀüÃ¼ÀÇ µ¥ÀÌÅ¸¿¡ ´ëÇØ¼­ CS °É±â
	void leaveCS();												// ÀüÃ¼ÀÇ µ¥ÀÌÅ¸¿¡ ´ëÇØ¼­ CS °É±â
	
	inline int getCurDataCnt(){return m_iCurDataCnt;}
	inline int getMaxDataCnt(){return m_iMaxDataCnt;}	
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
cArray<DataType, UniqueKey>::cArray()
{
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// cDataManager ¿¡¼­ ³»ºÎ¿¡¼­ µ¥ÀÌÅ¸¸¦ °¡Áö°í ÀÖÀ¸¹Ç·Î cArray Å¬·¡½º´Â ¼öÁ¤ÇØ¾ß ÇÑ´Ù!! (2009-09-05)
	// ÇöÀç cArray Å¬·¡½º¸¦ »ç¿ëÇÏ´Â°÷Àº ¾Æ¹«µ¥µµ ¾ø´Ù
	assert(0);
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	m_iDataDefaultSize = 0;
	m_bBinarySearch = FALSE;
	m_pcLog = NULL;
	m_vecDataPtrManager.reserve(eDATAPTRMANAGERSIZE);
	m_vecArrayManagerPtrManager.reserve(eDATAPTRMANAGERSIZE);
	m_bUseCriticalSection = FALSE;
	InitializeCriticalSection(&m_csData);
	m_vecCSPtrManager.reserve(eDATAPTRMANAGERSIZE);	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
cArray<DataType, UniqueKey>::~cArray()
{
	for(int i=0; i<(int)m_vecDataPtrManager.size(); i++){
		delete []m_vecDataPtrManager[i];
	}

	for(int i=0; i<(int)m_vecArrayManagerPtrManager.size(); i++){
		delete m_vecArrayManagerPtrManager[i];							// ÇÏ³ªÀÇ °´Ã¼¸¸ »ý¼ºÇß´Ù
	}

	if(m_bUseCriticalSection){
		DeleteCriticalSection(&m_csData);
		CRITICAL_SECTION *pCSOneData;
		for(int i=0; i<(int)m_vecCSPtrManager.size(); i++){
			pCSOneData = m_vecCSPtrManager[i];
			for(int k=0; k<m_iDataDefaultSize; k++){
				DeleteCriticalSection(&pCSOneData[k]);
			}
			delete []m_vecCSPtrManager[i];
		}
		m_vecCSPtrManager.clear();
	}

	m_vecDataPtrManager.clear();
	m_vecArrayManagerPtrManager.clear();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: cArray »ç¿ë½Ã¿¡ Ã³À½ ÇÑ¹ø È£ÃâÇØ¾ßÇÑ´Ù
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
void cArray<DataType, UniqueKey>::init(int _iReserveSize, BOOL _bBinarySearch, BOOL _bUseCriticalSection, cLog *_pcLog)
{
	if(m_iDataDefaultSize){
		TCHAR szPrint[256];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cArray::init(), Already call cArray::init()"));
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);
		if(m_pcLog){			// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return;
	}

	m_bUseCriticalSection = _bUseCriticalSection;
	m_iCurDataCnt = 0;
	memset(&m_dummyData, 0, sizeof(DataType));
	m_pcLog = _pcLog;		
	m_iDataDefaultSize = _iReserveSize;
	m_iMaxDataCnt = _iReserveSize;
	m_bBinarySearch = _bBinarySearch;
	resetDataSize();	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: È£ÃâÇÏ´Â °÷¿¡¼­ CS ¸¦ Ã³¸®ÇÑ´Ù
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
void cArray<DataType, UniqueKey>::getArrIdx(int _piVirtualArrIdx, int *_piVecIdx, int *_piArrIdx)
{
	if(_piVirtualArrIdx < m_iDataDefaultSize){				// Ã¹¹øÂ° Vector ÀÇ µ¥ÀÌÅ¸ÀÏ°æ¿ì
		*_piVecIdx = 0;
		*_piArrIdx = _piVirtualArrIdx;		
	}else{
		int iCnt = 0;
		while(1){
			_piVirtualArrIdx = _piVirtualArrIdx - m_iDataDefaultSize;
			++iCnt;
			if(_piVirtualArrIdx < m_iDataDefaultSize){
				break;
			}
		}
		*_piVecIdx = iCnt;
		*_piArrIdx = _piVirtualArrIdx;		
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: µ¥ÀÌÅ¸ÀÇ Å©±â¸¸Å­ »õ·Î µ¥ÀÌÅ¸¸¦ »ý¼ºÇÑ´Ù(±âÁ¸ÀÇ µ¥ÀÌÅ¸´Â À¯ÁöµÈ´Ù(ÁÖ¼Òµµ À¯Áö)
//       È£ÃâÇÏ´Â °÷¿¡¼­ CS ¸¦ Ã³¸®ÇÑ´Ù
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
void cArray<DataType, UniqueKey>::resetDataSize()
{
	DataType *pData;
	pData = new DataType[m_iDataDefaultSize];
	memset(pData, 0, sizeof(DataType) * m_iDataDefaultSize);
	m_vecDataPtrManager.push_back(pData);

	cDataManager<DataType, UniqueKey> *pcArrayManager;
	pcArrayManager = new cDataManager<int, UniqueKey>;					// ÇÏ³ªÀÇ °´Ã¼¸¸ »ý¼ºÇÑ´Ù
	pcArrayManager->init(m_iDataDefaultSize, m_bBinarySearch, 0, m_bUseCriticalSection, m_pcLog);
	m_vecArrayManagerPtrManager.push_back(pcArrayManager);

	if(m_bUseCriticalSection){
		CRITICAL_SECTION *pCSOneData;
		pCSOneData = new CRITICAL_SECTION[m_iDataDefaultSize];
		for(int i=0; i<m_iDataDefaultSize; i++){
			InitializeCriticalSection(&pCSOneData[i]);
		}
		m_vecCSPtrManager.push_back(pCSOneData);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: ºó°÷À» Ã£¾Æ µ¥ÀÌÅ¸¸¦ ³Ö¾îÁØ´Ù
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
int cArray<DataType, UniqueKey>::insertData(DataType *_pData, UniqueKey *_pKey)
{
	int iRestArrIdx;
	
	if(m_bUseCriticalSection){
		EnterCriticalSection(&m_csData);
	}
	
	for(int iVecIdx=0; iVecIdx<(int)m_vecArrayManagerPtrManager.size(); iVecIdx++){
		if(m_vecArrayManagerPtrManager[iVecIdx]->getUseArrCnt() < m_iDataDefaultSize){	// ¾ÆÁ÷ ºó°ø°£ÀÌ ÀÖÀ»¶§			
			iRestArrIdx = m_vecArrayManagerPtrManager[iVecIdx]->allocArrIdx(*_pKey);
			memcpy(&m_vecDataPtrManager[iVecIdx][iRestArrIdx], _pData, sizeof(DataType));
			++m_iCurDataCnt;

			if(m_bUseCriticalSection){
				LeaveCriticalSection(&m_csData);
			}
			return (iVecIdx * m_iDataDefaultSize + iRestArrIdx);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	// À§ÀÇ Á¶°ÇÀÌ ¾È¸ÂÀ»°æ¿ì´Â ºñ¾îÀÖ´Â ¹æÀÌ ¾øÀ»°æ¿ì

	m_iMaxDataCnt = m_iDataDefaultSize + m_iMaxDataCnt;	
	resetDataSize();

	// °ªÀ» ³Ö¾îÁØ´Ù
	int iVecIdx = (int)m_vecDataPtrManager.size() - 1;	// ¸ðµç º¤ÅÍÀÇ À§Ä¡´Â °°´Ù
	iRestArrIdx = m_vecArrayManagerPtrManager[iVecIdx]->allocArrIdx(*_pKey);
	memcpy(&m_vecDataPtrManager[iVecIdx][iRestArrIdx], _pData, sizeof(DataType));
	++m_iCurDataCnt;

	if(m_bUseCriticalSection){
		LeaveCriticalSection(&m_csData);
	}

	// Notice
	TCHAR szPrint[512];
	_stprintf_s(szPrint, _countof(szPrint), 
			_T("Notice!, cArray::insertData(), Call cArray::resetDataSize(), m_iMaxDataCnt: %d, m_iDataDefaultSize: %d"), 
			m_iMaxDataCnt, m_iDataDefaultSize);
	cUnicode::print(szPrint, eCONSOLECOLOR_NOTICE);	
	if(m_pcLog){		// Log
		m_pcLog->writeLog(szPrint);
	}
	
	// À§ÀÇ Á¶°ÇÀÌ ¾È¸ÂÀ»°æ¿ì´Â ºñ¾îÀÖ´Â ¹æÀÌ ¾øÀ»°æ¿ì
	////////////////////////////////////////////////////////////////////////////////////////////

	return (iVecIdx * m_iDataDefaultSize + iRestArrIdx);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
void cArray<DataType, UniqueKey>::deleteAllData()
{
	const int *piUseArrIdx;	
	int iArrMax;

	// m_iMaxDataCnt Å©±â°¡ º¯ÇÒ¼ö ÀÖÀ¸¹Ç·Î CS Ã³¸®
	if(m_bUseCriticalSection){
		EnterCriticalSection(&m_csData);		
	}
	
	for(int iVecIdx=0; iVecIdx<(int)m_vecArrayManagerPtrManager.size(); iVecIdx++){
		piUseArrIdx = m_vecArrayManagerPtrManager[iVecIdx]->getUseArrIdx();
		iArrMax = m_vecArrayManagerPtrManager[iVecIdx]->getUseArrCnt();
		for(int iDataIdx=0; iDataIdx<iArrMax; iDataIdx++){
			memset(&m_vecDataPtrManager[iVecIdx][piUseArrIdx[iDataIdx]], 0, sizeof(DataType));			
		}
	}

	for(int iVecIdx=0; iVecIdx<(int)m_vecArrayManagerPtrManager.size(); iVecIdx++){		
		m_vecArrayManagerPtrManager[iVecIdx]->deleteAllData();
	}

	m_iCurDataCnt = 0;

	if(m_bUseCriticalSection){
		LeaveCriticalSection(&m_csData);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
void cArray<DataType, UniqueKey>::deleteData(UniqueKey *_pKey)
{
	/**
	int iVecIdx;
	int iArrIdx;
	BOOL bFind = FALSE;
	
	if(m_bUseCriticalSection){
		EnterCriticalSection(&m_csData);
	}

	DataType *pData = NULL;
	for(iVecIdx=0; iVecIdx<(int)m_vecArrayManagerPtrManager.size(); iVecIdx++){
		pData = m_vecArrayManagerPtrManager[iVecIdx]->searchMap(*_pKey);
		if(!pData){
			bFind = TRUE;
			break;
		}
	}

	// µ¥ÀÌÅ¸¸¦ Ã£À»¼ö ¾ø´Ù
	if(!bFind){	
		if(m_bUseCriticalSection){
			LeaveCriticalSection(&m_csData);
		}
		TCHAR szPrint[512];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cArray::deleteData(), Not find Key: %d"), *_pKey);
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);	
		if(m_pcLog){		// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return;
	}

	memset(&m_vecDataPtrManager[iVecIdx][iArrIdx], 0, sizeof(DataType));
	m_vecArrayManagerPtrManager[iVecIdx]->releaseArrIdx(iArrIdx);
	--m_iCurDataCnt;

	if(m_bUseCriticalSection){
		LeaveCriticalSection(&m_csData);
	}
	**/
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
void cArray<DataType, UniqueKey>::deleteData(int _iArrIdx)
{
	if(_iArrIdx >= m_iMaxDataCnt){
		TCHAR szPrint[512];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cArray::deleteData(), MaxDataCnt: %d, FindArrIdx: %d"), m_iMaxDataCnt, _iArrIdx);
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);	
		if(m_pcLog){		// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return;
	}

	int iVecIdx;
	int iArrIdx;

	if(m_bUseCriticalSection){
		EnterCriticalSection(&m_csData);
	}

	getArrIdx(_iArrIdx, &iVecIdx, &iArrIdx);	

	// »ç¿ëµÈ ÀÎµ¦½º°¡ ¸Â´ÂÁö È®ÀÎÇÑ´Ù
	BOOL bFind = FALSE;
	const int *piArrIdx = m_vecArrayManagerPtrManager[iVecIdx]->getUseArrIdx();
	for(int i=0; i<m_vecArrayManagerPtrManager[iVecIdx]->getUseArrCnt(); i++){
		if(piArrIdx[i] == iArrIdx){
			bFind = TRUE;	
			break;
		}
	}

	if(bFind){
		memset(&m_vecDataPtrManager[iVecIdx][iArrIdx], 0, sizeof(DataType));
		m_vecArrayManagerPtrManager[iVecIdx]->releaseArrIdx(iArrIdx);
		--m_iCurDataCnt;

		if(m_bUseCriticalSection){
			LeaveCriticalSection(&m_csData);
		}
	}else{
		if(m_bUseCriticalSection){
			LeaveCriticalSection(&m_csData);
		}
		TCHAR szPrint[512];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cArray::deleteData(), Unused ArrIdx: %d"), _iArrIdx);
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);	
		if(m_pcLog){		// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
	}	
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
DataType *cArray<DataType, UniqueKey>::getData(UniqueKey *_pKey, int *_piArrIdx)
{
	return NULL;
	/**
	int iVecIdx;
	int iArrIdx;
	BOOL bFind = FALSE;
	
	if(m_bUseCriticalSection){
		EnterCriticalSection(&m_csData);
	}

	for(iVecIdx=0; iVecIdx<(int)m_vecArrayManagerPtrManager.size(); iVecIdx++){
		iArrIdx = m_vecArrayManagerPtrManager[iVecIdx]->getBinarySearch(*_pKey);
		if(iArrIdx != cDataManager<DataType, UniqueKey>::eNOTFINDBINARYSEARCH){
			bFind = TRUE;
			break;
		}
	}

	// µ¥ÀÌÅ¸¸¦ Ã£À»¼ö ¾ø´Ù
	if(!bFind){	
		if(m_bUseCriticalSection){
			LeaveCriticalSection(&m_csData);
		}
		// µ¥ÀÌÅ¸°¡ ÀÖ´ÂÁö È®ÀÎÇÏ±âÀ§ÇØ getData() ¸¦ È£ÃâÇÏ´Â °æ¿ì°¡ ÀÖÀ¸¹Ç·Î ·Î±×¸¦ ³²±âÁö ¾Ê´Â´Ù
		//TCHAR szPrint[512];
		//_stprintf_s(szPrint, _countof(szPrint), 
		//		_T("Error!, cArray::getData(), Not find Key: %d"), *_pKey);
		//cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);	
		//if(m_pcLog){		// Log
		//	m_pcLog->writeLog(szPrint);
		//}
		return NULL;
	}
	
	*_piArrIdx = (m_iDataDefaultSize * iVecIdx) + iArrIdx;	// ½ÇÁ¦ ¹è¿­¿¡¼­ ¾²ÀÌ´Â ÀÎµ¦½º
	
	if(m_bUseCriticalSection){
		LeaveCriticalSection(&m_csData);
	}
	
	return &m_vecDataPtrManager[iVecIdx][iArrIdx];	
	**/
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
DataType *cArray<DataType, UniqueKey>::getData(int _iArrIdx)
{
	if(_iArrIdx >= m_iMaxDataCnt){
		// µ¥ÀÌÅ¸°¡ ÀÖ´ÂÁö È®ÀÎÇÏ±âÀ§ÇØ getData() ¸¦ È£ÃâÇÏ´Â °æ¿ì°¡ ÀÖÀ¸¹Ç·Î ·Î±×¸¦ ³²±âÁö ¾Ê´Â´Ù
		//TCHAR szPrint[512];
		//_stprintf_s(szPrint, _countof(szPrint), 
		//	_T("Error!, cArray::getData(), MaxDataCnt: %d, FindArrIdx: %d"), m_iMaxDataCnt, _iArrIdx);
		//cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);	
		//if(m_pcLog){		// Log
		//	m_pcLog->writeLog(szPrint);
		//}
		return NULL;
	}

	int iVecIdx;
	int iArrIdx;

	if(m_bUseCriticalSection){
		EnterCriticalSection(&m_csData);
	}

	getArrIdx(_iArrIdx, &iVecIdx, &iArrIdx);	

	// »ç¿ëµÈ ÀÎµ¦½º°¡ ¸Â´ÂÁö È®ÀÎÇÑ´Ù
	BOOL bFind = FALSE;
	const int *piArrIdx = m_vecArrayManagerPtrManager[iVecIdx]->getUseArrIdx();
	for(int i=0; i<m_vecArrayManagerPtrManager[iVecIdx]->getUseArrCnt(); i++){
		if(piArrIdx[i] == iArrIdx){
			bFind = TRUE;	
			break;
		}
	}

	if(m_bUseCriticalSection){
		LeaveCriticalSection(&m_csData);
	}

	if(bFind){
		return &m_vecDataPtrManager[iVecIdx][iArrIdx];
	}else{		
		TCHAR szPrint[512];
		_stprintf_s(szPrint, _countof(szPrint), _T("Error!, cArray::deleteData(), Unused ArrIdx: %d"), _iArrIdx);
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);	
		if(m_pcLog){		// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return NULL;
	}	
}

	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: ¿ÜºÎ¿¡¼­ cArray ÀÇ ¸ðµç µ¥ÀÌÅ¸¸¦ °Ë»çÇÒ¶§ »ç¿ë (¸ðµç ¹è¿­À» °Ë»çÇÏ´Â°Ô ¾Æ´Ï¶ó µ¥ÀÌÅ¸°¡ µé¾îÀÖ´Â ¹è¿­¸¸ °Ë»çÇÑ´Ù)
//       Ex)
//       int iCnt = 0;
//       int iArrIdx;
//       int *piData = NULL;
//       cA.enterCS();			// getUseAllData() »ç¿ë½Ã Áß¿äÇÑ °÷ÀÏ¶§ CS Ã³¸®¸¦ ÇØÁÖ¾î¾ß ÇÑ´Ù
//       while(cA.getUseAllData(iCnt, &iArrIdx, &piData)){
//          int aa = *piData;	
//          if(aa == 5){
//              cA.deleteData(&aa);			// µ¥ÀÌÅ¸¸¦ »èÁ¦ÇÒ°æ¿ì¿¡´Â ++iCnt ¸¦ ÇÏÁö ¾Ê´Â´Ù
//          }else{
//              ++iCnt;
//          }
//       }
//       cA.leaveCS();
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
BOOL cArray<DataType, UniqueKey>::getUseAllData(int _iUseDataIdx, int *_piArrIdx, DataType **_ppData)
{
	int iVecMax = (int)m_vecArrayManagerPtrManager.size();
	for(int iVecIdx=0; iVecIdx<iVecMax; iVecIdx++){
		if(m_vecArrayManagerPtrManager[iVecIdx]->getUseArrCnt() <= _iUseDataIdx){			// ´ÙÀ½ º¤ÅÍ µ¥ÀÌÅ¸·Î ÀÌµ¿
			_iUseDataIdx -= m_vecArrayManagerPtrManager[iVecIdx]->getUseArrCnt();
			continue;
		}else{
			const int *piUseArrIdx = m_vecArrayManagerPtrManager[iVecIdx]->getUseArrIdx();
			*_piArrIdx = (m_iDataDefaultSize * iVecIdx) + piUseArrIdx[_iUseDataIdx];
			*_ppData = &m_vecDataPtrManager[iVecIdx][piUseArrIdx[_iUseDataIdx]];
			return TRUE;
		}
	}

	*_piArrIdx = 0;
	*_ppData = &m_dummyData;
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
void cArray<DataType, UniqueKey>::enterCS(int _iArrIdx)
{
	if(_iArrIdx >= m_iMaxDataCnt){
		TCHAR szPrint[512];
		_stprintf_s(szPrint, _countof(szPrint), 
			_T("Error!, cArray::enterCS(), MaxDataCnt: %d, FindArrIdx: %d"), m_iMaxDataCnt, _iArrIdx);
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);	
		if(m_pcLog){		// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return;
	}

	int iVecIdx;
	int iArrIdx;
	getArrIdx(_iArrIdx, &iVecIdx, &iArrIdx);

	EnterCriticalSection(&m_vecCSPtrManager[iVecIdx][iArrIdx]);						
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
void cArray<DataType, UniqueKey>::leaveCS(int _iArrIdx)
{
	if(_iArrIdx >= m_iMaxDataCnt){
		TCHAR szPrint[512];
		_stprintf_s(szPrint, _countof(szPrint), 
			_T("Error!, cArray::enterCS(), MaxDataCnt: %d, FindArrIdx: %d"), m_iMaxDataCnt, _iArrIdx);
		cUnicode::print(eCONSOLECOLOR_ERROR, szPrint);	
		if(m_pcLog){		// Log
			m_pcLog->writeLog(szPrint);
		}
		assert(0);
		return;
	}

	int iVecIdx;
	int iArrIdx;
	getArrIdx(_iArrIdx, &iVecIdx, &iArrIdx);

	LeaveCriticalSection(&m_vecCSPtrManager[iVecIdx][iArrIdx]);			
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
void cArray<DataType, UniqueKey>::enterCS()
{
	EnterCriticalSection(&m_csData);	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename DataType, typename UniqueKey>
void cArray<DataType, UniqueKey>::leaveCS()
{
	LeaveCriticalSection(&m_csData);				
}






// ===============================================================================================================================
// ========================================================= cCS =================================================================
// ===============================================================================================================================
//	2005-09-22 Kwon Yong Jae (amadeusj@gmail.com)
//	CRITICAL_SECTION
//	Debug ¸ðµå¿¡¼­´Â ÀÏ¹Ý CRITICAL_SECTION º¸´Ù ´À¸®Áö¸¸ Release ¸ðµå¿¡¼­´Â º°Â÷ÀÌ°¡ ¾ø´Ù
//	cCS ¸¦ »ý¼ºÇÏ¿© LeaveCriticalSection() À» ½Å°æ¾µÇÊ¿ä¾øÀÌ cCS ¼Ò¸êÀÚ¿¡¼­ Ã³¸®µÈ´Ù
//	³»ºÎÀûÀ¸·Î ½Ç¼ö¸¦ ¸·±âÀ§ÇÑ ·çÆ¾ÀÌ Àû¿ëµÈ´Ù
// ===============================================================================================================================

class cCS
{
private:
	CRITICAL_SECTION *m_pCS;	

public:	
	cCS(CRITICAL_SECTION *_pCS)
	{
		assert(_pCS);
		m_pCS = _pCS;	
		EnterCriticalSection(m_pCS);		
	}

	~cCS()
	{	
		if(m_pCS){
			leaveCS();
		}
	}	

	inline void leaveCS()
	{
		assert(m_pCS);
		LeaveCriticalSection(m_pCS);
		m_pCS = NULL;
	}
};



// ===============================================================================================================================
// ================================================== cThreadDataRef =============================================================
// ===============================================================================================================================
//	2007-03-16 Kwon Yong Jae (amadeusj@gmail.com)
//	ThreadDataReference
//	Ã³À½ »ç¿ë½Ã init() ¸¦ È£ÃâÇØ¾ßÇÑ´Ù
//	µ¥ÀÌÅ¸ ±¸Á¶¿¡¼­ »ç¿ë (¸ÖÆ¼¾²·¹µå ÀÏ¶§¸¸)
//	¸ÖÆ¼¾²·¹µåÀÏ¶§ ÇØ´ç µ¥ÀÌÅ¸ÀÇ Æ÷ÀÎÅÍ¸¦ ÂüÁ¶ÇØ¼­ »ç¿ëÇÏ´Âµ¥ ´Ù¸¥¾²·¹µå¿¡¼­ ÇØ´ç µ¥ÀÌÅ¸¸¦ »èÁ¦ÇÏ´Â °ÍÀ» ¹æÁö
//	µ¥ÀÌÅ¸¸¦ ÂüÁ¶ÇÒ¶§ ÂüÁ¶Ä«¿îÅÍ¸¦ +1 »ç¿ëÈÄ¿¡´Â -1
// ===============================================================================================================================
class cThreadDataRef
{
private:	
	CRITICAL_SECTION *m_pCS;		// ÇöÀçÀÇ °´Ã¼ ÂüÁ¶ °¹¼ö CS
	int m_iRefCnt;					// ÇöÀçÀÇ °´Ã¼ ÂüÁ¶ °¹¼ö
	BOOL m_bDelete;					// µ¥ÀÌÅ¸¸¦ Áö¿ö¾ß ÇÏ´Âµ¥ iRefCnt °¡ 0ÀÌ ¾Æ´Ò¶§ bDelete ¸¦ TRUE ¸¦ ÁÖ¾î¼­ iRefCnt °¡ 0ÀÌ µÉ¶§ µ¥ÀÌÅ¸¸¦ Áö¿î´Ù

public:
	cThreadDataRef()
	{
		m_pCS = NULL;
	}

	~cThreadDataRef()
	{
		if(m_pCS){
			DeleteCriticalSection(m_pCS);
			delete m_pCS;
			m_pCS = NULL;
		}
	}

	inline void init()
	{
		if(m_pCS){
			return;
		}
		m_pCS = new CRITICAL_SECTION;
		InitializeCriticalSection(m_pCS);
		clearData();
	}

	inline void clearData()
	{
		m_iRefCnt = 0;
		m_bDelete = FALSE;
	}

	inline BOOL addRefCnt()
	{
		if(!m_pCS){
			return FALSE;
		}
		EnterCriticalSection(m_pCS);
		++m_iRefCnt;
		LeaveCriticalSection(m_pCS);
		return TRUE;
	}

	inline BOOL subRefCnt()
	{
		if(!m_pCS){
			return FALSE;
		}
		EnterCriticalSection(m_pCS);
		--m_iRefCnt;
		m_iRefCnt = (m_iRefCnt < 0 ? 0 : m_iRefCnt);
		LeaveCriticalSection(m_pCS);
		return TRUE;
	}

	inline int getRefCnt(){return m_iRefCnt;}
	inline void setDeleteData(){m_bDelete = TRUE;}
	inline BOOL getDeleteData(){return m_bDelete;}
};








// ===============================================================================================================================
// ========================================================= cThread =============================================================
// ===============================================================================================================================
//	2006-03-26 Kwon Yong Jae (amadeusj@gmail.com)
//	Thread »ý¼º
//	cParent Å¬·¡½º¿¡¼­ cThread¸¦ »ç¿ëÇÒ¶§ cParent Å¬·¡½ºÀÇ ¸â¹öÇÔ¼ö¸¦ static void test(cParent *p) À¸·Î ¼±¾ðÇÏ°í »ç¿ëÀ» ÇØ¾ßÇÑ´Ù
//	ÀÌ¶§ test()ÇÔ¼ö´Â static ÀÌ¹Ç·Î ÀÚ½ÅÀÇ ¸â¹öº¯¼ö¸¦ »ç¿ëÇÒ¼ö ¾ø´Ù 
//	±×·¡¼­ cThread::m_pcParentPrt ¸¦ »ç¿ëÇØ¼­ test ÇÔ¼ö¿¡¼­ cParent ÀÇ µ¥ÀÌÅ¸¸¦ Ã³¸®ÇÑ´Ù
//	¾²·¹µå°¡ ½ÇÇàÁßÀÌ¶óµµ endThread() ¸¦ È£ÃâÇÏ¸é ¾²·¹µå°¡ Á×´Â¼ø°£ endThread() ´Â ¸®ÅÏµÈ´Ù (´Ù½Ã ¸»ÇØ endThread() ¸¦È£ÃâÇØµµµÈ´Ù)
//	Ex)
//	class cTest
//	{
//	public:
//		int m_iValue;
//		cThread thread;	
//		cTest(){
//			m_iValue = 0;				
//		}
//		~cTest(){}	
//		static void loop(void *_pcPtr){			// ÇÔ¼ö Æ÷ÀÎÅÍ¸¦ »ç¿ëÇÏ·Á¸é ÇÔ¼ö°¡ ÄÄÆÄÀÏ½Ã¿¡ »ý¼ºµÇ¾î¾ßÇÏ¹Ç·Î static À¸·Î ¼±¾ðµÇ¾î¾ßÇÑ´Ù
//			cTest *pThis = (cTest *)_pcPtr;	
//			while(pThis->thread.threadWhile()){
//				pThis->m_iValue ++;
//				TCHAR szTemp[256];		
//				_stprintf_s(szTemp, _countof(szTemp), _T("%d\t"), pThis->m_iValue);
//				cUnicode::print(szTemp);
//				pThis->thread.sleep();
//			}
//		}
//		void start(){
//			thread.startThread(this, 0, 1, loop, NULL, _T("cTest"));		
//		}
//		void end(){
//			thread.endThread();
//		}
//	};
// ===============================================================================================================================

class cThread
{
private:
	// ¾²·¹µå¸¦ »ý¼º½Ã¿¡ ¾²·¹µå¿¡ ÀÎµ¦½º¸¦ ºÎ¿©ÇÏ±â À§ÇØ
	typedef struct _stTHREADPARAM
	{
		void *pData;			// this
		int iIdx;				// ¾²·¹µå ÀÎµ¦½º
	}stTHREADPARAM;

	TCHAR m_szCaption[80];		// ´Ü¼øÇÑ ÀÌ¸§ (¾î¶°ÇÑ ¾²·¹µåÀÎÁöÈ®ÀÎ)	
	HANDLE *m_phThread;			// ¾²·¹µå ÇÚµé	(¸î°³¸¦ »ý¼ºÇÒÁö ¸ð¸§)
	BOOL *m_pbThreadWhile;		// ÇÏ³ª·Î Ã³¸®ÇØµµ µÇ´Âµ¥ endThread Ã³¸®½Ã¿¡ ¹®Á¦°¡ µÇ¾î¼­ µû·Î °ü¸®ÇÑ´Ù
	BOOL m_bEndThread;			// endThread() ¿¡ ÀÇÇØ¼­ ¾²·¹µå°¡ Á¾·áµÇ´ÂÁö
	int m_iSleepMillisecond;	// ¾²·¹µå°¡ ¸î¹Ð¸®ÃÊ ´ÜÀ§·Î ½ÇÇàµÉ°ÍÀÎÁö (0 ÀÌ¸é °è¼Ó)
	int m_iThreadCnt;
	int m_iCurThreadCnt;		
	void *m_pcParentPrt;		// cThread ¸¦ »ç¿ëÇÏ´Â ºÎ¸ð Å¬·¡½ºÀÇ Æ÷ÀÎÅÍ	

private:	
	// ÇÔ¼ö Æ÷ÀÎÅÍ (¿ÜºÎÀÇ ÇÔ¼ö¸¦ ½ÇÇà)
	DWORD (*pThreadMainProcPtr)(void *_pThis, int _iThreadIdx);							// ¾²·¹µå ¸ÞÀÎ		
	void (*pThreadEndProcPtr)(void *_pThis, int _iThreadIdx, DWORD _dwReturnValue);		// ¾²·¹µå Á¾·á½Ã È£Ãâ
	
	void clearMem();
	static UINT __stdcall threadMain(LPVOID _lpData);	

public:
	cThread();
	~cThread();
	BOOL startThread(void *_pThis, int _iSleepMillisecond, int _iThreadCnt,
			DWORD (*_pThreadMainProcPtr)(void *_pThis, int _iThreadIdx), 
			void (*_pThreadEndProcPtr)(void *_pThis, int _iThreadIdx, DWORD _dwReturnValue) = NULL, 
			TCHAR *_pszCaption = NULL);
	void endThread();	
	void forcedEndThread();	
	void suspendThread(int _iThreadIdx);
	void resumeThread(int _iThreadIdx);

	inline void sleep(){Sleep(m_iSleepMillisecond);}
	inline BOOL threadWhile(int _iThreadIdx){return m_pbThreadWhile[_iThreadIdx];}	
};

