
//	일  자 : 2008년 06월 29일
//	손댄자 : 이영찬
//	수정 사항 요약 : 기존 클래스의 인터페이스는 수정하지 않고 이런 저런 기능 추가
//	수정 & 추가 사항 :
//	1. 상황에 따라 콘솔창가 OutputDebugString을 사용 할 수 있도록 수정
//	2. 텍스트 출력시 색상을 같이 입력 할 수 있는 함수 추가
//	3. TCHAR가 아닌 char로 입력 가능한 print 추가
//	4. 클래스 관리하기 귀찮은 사람을 위해 더미 클래스를 추가 하고 연계해서 OpenColsole,dPrint(debug print) 함수 시리즈 제작
//	
#pragma once

// MWSOnline 프로젝트에서만 적용
#include <tchar.h>
#include <windows.h>

class cConsole
{
private:

	HANDLE	m_hConsole;
	WORD	m_wCurrentTextColor;
	BOOL	m_bIsWantOutputDebug;	//	텍스트를 OutputDebugStr을 통해 출력한다.
	int		m_iRFLength;

public:	
	cConsole();
	~cConsole();

	BOOL	create(const TCHAR *_pszTitle, BOOL _bNoClose = TRUE);	// create the console	
	void	setColor(WORD _wColor = NULL,BOOL _bIsInstance=FALSE);							// set color for output
	void	setCurrentColor();		// set color for output	
	void	changeOutputDevice(BOOL _bIsDebugString)
	{
		m_bIsWantOutputDebug	=	_bIsDebugString;
	}

	void	_printRF();	//	줄바꾸기
	void	print(const TCHAR *_pszOutput = NULL, ...);	//	일반적인 출력
	void	print(WORD _wColor,const TCHAR *_pszOutput = NULL, ...);	//	텍스트 컬러값이 있는 출력
	void	_print(const TCHAR *_pszOutput);
	void	_print(WORD _wColor,const TCHAR *_pszOutput);
#ifdef	_UNICODE
	//	메시지 출력시 _T(.. 를 붙이기 귀찮다면 아래를 사용
	void	print(const char *_pszOutput = NULL, ...);
	void	print(WORD _wColor,const char *_pszOutput = NULL, ...);
	void	_print(WORD _wColor,const char *_pszOutput);
	void	_print(const char *_pszOutput);
#endif

	void	setTitle(const TCHAR *_pszTitle);	// set and get title of console
	TCHAR	*getTitle();
	HWND	getHWND();							// get HWND of console
	HANDLE	getHandle();						// get HANDLE of console
	void	showConsole(BOOL _bShow = TRUE);	// show/hide the console
	void	disableClose();						// disable the [x] button of the console
	void	clear();							// clear all output
	void	close();							// close the console and delete it

	inline	BOOL	isValidHandle()	//	핸들이 유효하냐?
	{
		if	(m_bIsWantOutputDebug == FALSE || m_hConsole)
			return	TRUE;

		return	FALSE;
	}
};

//	CFC Console Foreground Color
//	CBC Console Background Color
#define CFC_BLUE			0x0001 // text color contains blue.
#define CFC_GREEN			0x0002 // text color contains green.
#define CFC_RED				0x0004 // text color contains red.
#define CFC_INTENSITY		0x0008 // text color is intensified.
#define CBC_BLUE			0x0010 // CBC color contains blue.
#define CBC_GREEN			0x0020 // CBC color contains green.
#define CBC_RED				0x0040 // CBC color contains red.
#define CBC_INTENSITY		0x0080 // CBC color is intensified.

//	debug print
extern	void	OpenColsole(const TCHAR *_lpstrName);	//	게으름뱅이용 콘솔 열기

extern	void	dprint(const TCHAR *_pszOutput = NULL, ...);
extern	void	dprint(WORD _wColor,const TCHAR *_pszOutput = NULL, ...);
#ifdef	_UNICODE
extern	void	dprint(const char *_pszOutput, ...);
extern	void	dprint(WORD _wColor,const char *_pszOutput = NULL, ...);
#endif