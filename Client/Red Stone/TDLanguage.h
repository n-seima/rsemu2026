#ifndef _TDLANGUAGE_
#define _TDLANGUAGE_
#include "TDUtil.h"
#include <imm.h>
#include <vector>
using namespace std;

//#define _PROTOTYPE_	

#ifdef _PROTOTYPE_
	struct cRECT
	{
		int x1; int y1; int x2; int y2;
	};
	extern char data[1000];

	class cINPUTBOX  { };
	typedef cRECT TDRECT;
	typedef DWORD TDCOLOR;
	#define TDRGB( x, y, z ) RGB(x,y,z)
	#define TDSET_RESULT( hWnd, inputbox, x )  strcpy ( data , x );	
#else
	typedef cRECT TDRECT;
	typedef unsigned short TDCOLOR;
	#define TDRGB( x, y, z ) RGB24To16(x,y,z)
	__forceinline void TDSET_RESULT( HWND hWnd, cINPUTBOX* lpInputBox, char* compobuf )
	{ 
		int size;
		char  editbuf[256];
		int edit_max_length;
		size = GetWindowTextLength ( hWnd );
		lpInputBox->getStr ( editbuf, size + 1 );
		strcat ( editbuf, compobuf );
		edit_max_length = ( int )SendMessage ( hWnd, EM_GETLIMITTEXT, 0, 0 );
		if ( strlen ( editbuf ) < edit_max_length )
			lpInputBox->setStr( editbuf );
	}
#endif


//! 중국어 IME 종류
enum eInputMethod
{
		ecNeiMa,
		ecQuanPin,
		ecShuangPin,
		ecEnglish,
		ecZhengMa,
		ecPinyin,
};

//! 채팅 박스 상테 
enum eLanguageBoxState { ecOpen, ecClose, ecNone };

/**
******************************************************************************
  캔디박스 출력 클래스.
******************************************************************************
*/
class TDLanguageBox   
{
private:
	enum	eUISTATE
	{
		ec_ui_none,
		ec_ui_caption_selected,
		ec_ui_mouse_on_list,
		ec_ui_button_selected,
	};

	char* Alloc ( int size );
	void Release ( );

	eUISTATE get_action_state ( int x, int y, bool is_lb_down );

	bool is_mouse_on_rect ( int x, int y, TDRECT in_rect );

	bool is_mouse_on_rect ( int x, int y );

	int get_list_number ( int x, int y );

	int is_button_selected ( int x, int y );

	void render_selected ( HDC hDC );

	void render_text ( HDC hDC, const vector< char* > &index );
	
	void render_buttons ( HDC hDC );

	void render_ui ( HDC hDC );

	bool on_imn_changecandidate ( DWORD dwParam1, DWORD dwParam2 );
	bool on_imn_opencandidate ( DWORD dwParam1, DWORD dwParam2 );
	bool on_imn_closecandidate ( DWORD dwParam1, DWORD dwParam2 );
	bool on_imn_default ( DWORD dwParam1, DWORD dwParam2 );

public:
	TDLanguageBox ( );
	~TDLanguageBox ( );
	void setRect ( int x, int y );

	// true 이면 캔디박스 선택됨
	bool on_update ( int x, int y, bool is_lb_down );

	void setVisible ( bool is_visible );

	void setEnable ( bool enable);

	void on_render ( HDC hDC );
private:

	const int			width;
	const int			height;
	int					nPos;
	TDRECT				rect;

	HFONT				hFont;
	eUISTATE			ui_state;
	int					code_page;
	int					max_code_page;
	int					message1;
	int					message2;
	bool				is_enable;
	eLanguageBoxState	state;

	int					alloc_size;
	char*				alloc_memory;
	vector < char* >	index;

public:
	TDCommandBind < TDLanguageBox > cmd_imn_changecandidate;
	TDCommandBind < TDLanguageBox > cmd_imn_opencandidate;
	TDCommandBind < TDLanguageBox > cmd_imn_closecandidate;
	TDCommandBind < TDLanguageBox > cmd_imn_default;
};


/**
******************************************************************************
 문자 조합 과정을 처리하는 IME 기본 인터페이스
******************************************************************************
*/
class ILanguageCompo
{
// member functions
public:
	enum eUISTATE
	{
		ec_ui_selected,
		ec_ui_none,
	};

	ILanguageCompo ( );
	~ILanguageCompo ( );

	void setEnable ( bool enable);

	void setVisible ( bool is_visible );


	virtual bool on_update ( int x, int y, bool is_lb_down ) =0;
	virtual void on_render ( HDC hDC ) = 0;
	void setRect ( int x, int y );

protected:

	eUISTATE get_action_state ( int x, int y, bool is_lb_down );

	bool is_mouse_on_rect ( int x, int y, TDRECT in_rect );

	bool is_mouse_on_rect ( int x, int y );


protected: // member variables
	bool	 is_enable;
	int width;
	int height;
	TDRECT rect;
	eUISTATE ui_state;
	char	compo_buf [ 256 ];
	HFONT hFont;
};


/**
******************************************************************************
 TDLanguageCompo
 ...
******************************************************************************
*/
class TDLanguageCompo : public ILanguageCompo
{
public:
	TDLanguageCompo ( );
	~TDLanguageCompo ( );

	bool on_update ( int x, int y, bool is_lb_down );

	void on_render ( HDC hDC );

	TDCommandBind < TDLanguageCompo > cmd_ime_composition;
	TDCommandBind < TDLanguageCompo > cmd_ime_endcomposition;

protected:
	bool on_ime_endcomposition ( DWORD dwParam1, DWORD dwParam2 );

	bool on_ime_composition ( DWORD dwParam1, DWORD dwParam2 );

	void render_ui ( HDC hDC );
	void render_text ( HDC hDC );

};

/**
******************************************************************************
 
******************************************************************************
*/
class TDLanguagePinyinCompo : public ILanguageCompo
{

public:
	TDLanguagePinyinCompo ( );
	~TDLanguagePinyinCompo ( );

	bool on_update ( int x, int y, bool is_lb_down );

	void on_render ( HDC hDC );
	
	TDCommandBind < TDLanguagePinyinCompo > cmd_ime_composition;
	TDCommandBind < TDLanguagePinyinCompo > cmd_ime_endcomposition;

protected:
	bool on_ime_endcomposition ( DWORD dwParam1, DWORD dwParam2 );

	bool on_ime_composition ( DWORD dwParam1, DWORD dwParam2 );

	void render_ui ( HDC hDC );
	void render_text ( HDC hDC );
	void render_text2 ( HDC hDC );

	// ori_buf 파싱.
	char* get_current_ori ( char* in_buf );

	char	compo_buf [ 256 ];
	char	*ori_buf;
	HFONT hFont;
};




/**
******************************************************************************
 tdlanguage 클래스. 중국어 입력을 처리하는 메인 클래스.
******************************************************************************
*/
/*
class TDLanguage 
{
public:
	TDLanguage ( );

	~TDLanguage ( );

	void Release ( );

	void set_language_box ( TDLanguageBox* box );

	void set_language_compo ( TDLanguageCompo* compo );

	void set_language_compo2 ( TDLanguagePinyinCompo* compo );

	// 팩토리.
	void set_input_method_state ( eInputMethod in_estate );
	void render ( HDC hDC );

	bool update ( int x, int y, bool is_lb_down );

	void setVisible ( bool is_visible );
	void setPos ( int x, int y );

	int MessageProc ( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

private:
	////////////////////////////////////////////////////////////////////////////////////
	// begin. ime event. this codes placed into window message loop.
	void on_ime_notify ( HWND hWnd, WPARAM wParam );

	void on_ime_composition ( HWND hWnd, LPARAM lParam );
	void on_ime_endcomposition ( );
	// end of ime event. this codes placed into window message loop.
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	// begin. notify event.  this codes placed into ime_notify.
	void on_imn_setconversionmode ( HIMC hIMC );
	void on_imn_openstatuswindow ( HIMC hIMC );
	void on_imn_changecandidate ( HIMC hIMC );
	void on_imn_opencandidate ( HIMC hIMC );
	void on_imn_closecandidate ( );
	void on_imn_default ( );
	// end of imn event.   this codes placed into ime_notify.
	////////////////////////////////////////////////////////////////////////////////////

	eInputMethod get_input_method_state ( const char* in_state );

	////////////////////////////////////////////
	// 컴포지션 윈도우 관련 커맨드
	TDCommand* cmd_ime_composition;
	TDCommand* cmd_ime_endcomposition;
	////////////////////////////////////////////

	////////////////////////////////////////////
	// 캔디박스 윈도우 관련 커맨드
	TDCommand* cmd_imn_changecandidate;
	TDCommand* cmd_imn_closecandidate;
	TDCommand* cmd_imn_opencandidate;
	TDCommand* cmd_imn_default;
	////////////////////////////////////////////

	eInputMethod    input_mode;
	TDLanguageBox*	language_box;
	TDLanguageCompo* compo_box;
	TDLanguagePinyinCompo* pinyin_compo_box;
};
*/


/**
******************************************************************************
 중국어 처리가 제대루 안되기때문에 만든 땜빵코드 목적은. 일반상태에서 IME 가 안뜨도록하는것.
******************************************************************************
*/
class TDLanguage 
{
public:
	TDLanguage ( );
	~TDLanguage ( );

	void update ( bool lb, bool rb );
	int MainMessageProc ( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	int TalkMessageProc ( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	bool is_ok ();
	void set_state ( bool in );
//private:
	bool ime_state;
	bool ok_state;
};

extern TDLanguage g_lng;
#endif
