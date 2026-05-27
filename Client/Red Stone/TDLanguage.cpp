#include "../engine/SFC.h"
#include "../engine/ctalkbox.h"
#include "../engine/ctrans.h"
#include "../engine/cdraw.h"

#include "tdlanguage.h"

TDLanguage g_lng;


#ifdef _PROTOTYPE_
	char data[1000];
#endif

//! predefine values..
namespace TDLanguageBox_Defines
{
	const int startx = 10;
	const int starty = 10;
	const int caption_height = 25;
	const int list_max_count = 10;
	const int list_start_ypos = 25;
	const int list_col_margin = 1;
	const int list_row_margin = 0;
	const int font_height = 14;
	const int screen_width = 800;
	const int screen_height = 600;
};

//! predefine values..
namespace TDLanguageCompo_Defines
{
	const int font_height = 14;
	const int screen_width = 800;
	const int screen_height = 600;
};


/**
******************************************************************************
  캔디박스 출력 클래스.
******************************************************************************
*/
char* TDLanguageBox::Alloc ( int size )
{
	if ( alloc_memory != NULL )
	{
		Release ( );
	}
	if ( size < 0 )
	{
		throw "alloc failed!!";
		return 0;
	}
	alloc_memory = new char [ size ];
	if ( alloc_memory == NULL )
	{
		throw "alloc failed!!";
		return 0;
	}
	alloc_size = size;

	return alloc_memory;
}

void TDLanguageBox::Release ( )
{
	if ( alloc_size > 0 && alloc_memory != NULL)
	{
		index.clear( ); 
		delete alloc_memory;
		alloc_memory = NULL;
		alloc_size = 0;
	}
}

TDLanguageBox::eUISTATE TDLanguageBox::get_action_state ( int x, int y, bool is_lb_down )
{		
	using namespace TDLanguageBox_Defines;
	const TDRECT  caption_rect = { rect.x1, rect.y1, rect.x2, rect.y1 + caption_height };
	const TDRECT  list_rect = { rect.x1, caption_height , rect.x2, rect.y2 };

	int state = is_button_selected ( x, y );
	if ( state >= 0 )
	{
		message1 = state;
		return ec_ui_button_selected;
	}
	if ( is_mouse_on_rect ( x, y, caption_rect ) )
	{
		return ec_ui_caption_selected;
	}
	else if ( is_mouse_on_rect ( x, y, list_rect ) )
	{
		return ec_ui_mouse_on_list;
	}
	else
	{
		return ec_ui_none;
	}
}

bool TDLanguageBox::is_mouse_on_rect ( int x, int y, TDRECT in_rect )
{
	if ( x > in_rect.x1 )
	{
		if ( x < in_rect.x2 )
		{
			if ( y > in_rect.y1 )
			{
				if ( y < in_rect.y2 )
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool TDLanguageBox::is_mouse_on_rect ( int x, int y )
{
	return is_mouse_on_rect ( x, y, rect );
}

int TDLanguageBox::get_list_number ( int x, int y )
{
	using namespace TDLanguageBox_Defines;

	int i , cnt;
	for ( i = 1, cnt = 0; i < list_max_count + 1 ; i++, cnt++ )
	{
		const int yPos = ( font_height + list_row_margin ) * cnt + list_start_ypos + rect.y1 ;
		const TDRECT select_rect = { rect.x1 + list_col_margin, yPos, rect.x2 - list_col_margin, yPos + font_height	};

		if ( is_mouse_on_rect ( x, y, select_rect ) == true ) return cnt;
	}

	return -1;
}

int TDLanguageBox::is_button_selected ( int x, int y )
{
	const int button_x_margin = 4;
	const int button_width  = 10;
	const int button_height = 10;
	const int button_ypos   = rect.y1 + 2;
	const int button_xpos   = rect.x2 -( ( button_width  + button_x_margin ) * 4 );

	const int most_lb_pos		= button_xpos;
	const int lb_pos			= most_lb_pos + button_width + button_x_margin;
	const int rb_pos			= lb_pos + button_width + button_x_margin;
	const int most_rb_pos		= rb_pos + button_width + button_x_margin;

	const TDRECT most_lb_rect = { most_lb_pos, button_ypos, most_lb_pos + button_width, button_ypos + button_height };
	const TDRECT lb_rect = { lb_pos, button_ypos, lb_pos + button_width, button_ypos + button_height };
	const TDRECT rb_rect = { rb_pos, button_ypos, rb_pos + button_width, button_ypos + button_height };
	const TDRECT most_rb_rect = { most_rb_pos, button_ypos, most_rb_pos + button_width, button_ypos + button_height };

	if ( is_mouse_on_rect ( x, y, most_lb_rect ) )
		return 0;
	if ( is_mouse_on_rect ( x, y, lb_rect ) )
		return 1;
	if ( is_mouse_on_rect ( x, y, rb_rect ) )
		return 2;
	if ( is_mouse_on_rect ( x, y, most_rb_rect ) )
		return 3;

	return -1;
}

void TDLanguageBox::render_selected ( HDC hDC )
{
	using namespace TDLanguageBox_Defines;
	const TDCOLOR selected_line_color = TDRGB ( 110, 55, 110 );
	const TDCOLOR selected_color = TDRGB ( 110, 55, 255 );

	if ( message1 == -1 ) return;
	if ( ui_state != ec_ui_mouse_on_list ) return;

	const int yPos = ( font_height + list_row_margin ) * message1 + list_start_ypos + rect.y1 ;
	const TDRECT select_rect = { rect.x1 + list_col_margin, yPos, rect.x2 - list_col_margin, yPos + font_height	};
	cDRAW::Fill ( selected_color, select_rect.x1, select_rect.y1, select_rect.x2, select_rect.y2 );
	cDRAW::Box  ( selected_line_color, select_rect.x1, select_rect.y1, select_rect.x2, select_rect.y2, 2 );
}

void TDLanguageBox::render_text ( HDC hDC, const vector< char* > &index )
{
	using namespace TDLanguageBox_Defines;

	const int start_xpos = 5;
	render_selected ( hDC );

	int i,cnt;
	for ( i = 1, cnt = code_page ; i < list_max_count + 1 ; i ++, cnt ++ )
	{
		char* element;
		int xPos = start_xpos + rect.x1;
		int yPos = ( font_height + list_row_margin ) * cnt + list_start_ypos + rect.y1 ;

		if ( cnt >= index.size ( ) ) break;
		element = index [ cnt ];

		{
			int num;
			const TDCOLOR line_color = TDRGB ( 110, 55, 110 );
			const TDCOLOR text_color = TDRGB ( 110, 55, 55 );
			TDCOLOR oldcol;
			int oldbk;
			char buf [ 255 ];

			oldbk = SetBkMode ( hDC, TRANSPARENT );
			oldcol = SetTextColor ( hDC, text_color );

			num = i;
			if ( num == 10 ) num = 0;

			wsprintf ( buf, "%d: %s", num, element );
			cDRAW::Line( line_color, rect.x1, yPos + font_height, rect.x2, yPos+ font_height );
			TextOut ( hDC, xPos, yPos, buf, strlen ( buf ) );		
			SetTextColor ( hDC, oldcol );
			SetBkMode ( hDC, oldbk );
		}
	}
}

void TDLanguageBox::render_buttons ( HDC hDC )
{
	const TDCOLOR back_selected_color = TDRGB ( 0, 0, 0 );
	const TDCOLOR box_line_color = TDRGB ( 127, 63, 63 );
	const TDCOLOR back_ground_color = TDRGB ( 255, 127, 127 );

	const int button_x_margin = 4;
	const int button_width  = 10;
	const int button_height = 10;
	const int button_ypos   = rect.y1 + 2;
	const int button_xpos   = rect.x2 -( ( button_width  + button_x_margin ) * 4 );

	const int most_lb_pos		= button_xpos;
	const int lb_pos			= most_lb_pos + button_width + button_x_margin;
	const int rb_pos			= lb_pos + button_width + button_x_margin;
	const int most_rb_pos		= rb_pos + button_width + button_x_margin;

	const TDRECT most_lb_rect = { most_lb_pos, button_ypos, most_lb_pos + button_width, button_ypos + button_height };
	const TDRECT lb_rect = { lb_pos, button_ypos, lb_pos + button_width, button_ypos + button_height };
	const TDRECT rb_rect = { rb_pos, button_ypos, rb_pos + button_width, button_ypos + button_height };
	const TDRECT most_rb_rect = { most_rb_pos, button_ypos, most_rb_pos + button_width, button_ypos + button_height };


	int oldbk;
	oldbk = SetBkMode ( hDC, TRANSPARENT );

	cDRAW::Fill( back_ground_color, most_lb_rect.x1, most_lb_rect.y1, most_lb_rect.x2, most_lb_rect.y2 );
	cDRAW::Box ( box_line_color, most_lb_rect.x1, most_lb_rect.y1, most_lb_rect.x2, most_lb_rect.y2, 2 );
	if ( message1 == 0 && message2 == 1 )
	{
		cDRAW::Fill( back_selected_color, most_lb_rect.x1, most_lb_rect.y1, most_lb_rect.x2, most_lb_rect.y2 );
	}

//		TextOut ( hDC, most_lb_rect.x1, most_lb_rect.y1, "<<", 2 );

	cDRAW::Fill( back_ground_color, lb_rect.x1, lb_rect.y1, lb_rect.x2, lb_rect.y2 );
	cDRAW::Box ( box_line_color, lb_rect.x1, lb_rect.y1, lb_rect.x2, lb_rect.y2, 2 );
	if ( message1 == 1 && message2 == 1 )
	{
		cDRAW::Fill( back_selected_color, lb_rect.x1, lb_rect.y1, lb_rect.x2, lb_rect.y2 );
	}
//		TextOut ( hDC, lb_rect.x1, lb_rect.y1, "<", 1 );

	cDRAW::Fill( back_ground_color, rb_rect.x1, rb_rect.y1, rb_rect.x2, rb_rect.y2 );
	cDRAW::Box ( box_line_color, rb_rect.x1, rb_rect.y1, rb_rect.x2, rb_rect.y2, 2 );
//		TextOut ( hDC, rb_rect.x1, rb_rect.y1, ">", 1 );
	if ( message1 == 2 && message2 == 1 )
	{
		cDRAW::Fill( back_selected_color, rb_rect.x1, rb_rect.y1, rb_rect.x2, rb_rect.y2 );
	}
	
	cDRAW::Fill( back_ground_color, most_rb_rect.x1, most_rb_rect.y1, most_rb_rect.x2, most_rb_rect.y2 );
	cDRAW::Box ( box_line_color, most_rb_rect.x1, most_rb_rect.y1, most_rb_rect.x2, most_rb_rect.y2, 2 );
//		TextOut ( hDC, most_rb_rect.x1, most_rb_rect.y1, ">>", 2 );
	if ( message1 == 3 && message2 == 1 )
	{
		cDRAW::Fill( back_selected_color, most_rb_rect.x1, most_rb_rect.y1, most_rb_rect.x2, most_rb_rect.y2 );
	}

	SetBkMode ( hDC, oldbk );


}

void TDLanguageBox::render_ui ( HDC hDC )
{
	using namespace TDLanguageBox_Defines;

	const TDCOLOR box_line_color = TDRGB ( 127, 63, 63 );
	const TDCOLOR back_ground_color = TDRGB ( 255, 127, 127 );
	
	const TDCOLOR caption_line_color = TDRGB ( 127, 63, 63 ); 
	const TDCOLOR caption_back_color = TDRGB ( 200, 100, 100 ); 
	const TDRECT  caption_rect = { rect.x1, rect.y1, rect.x2, rect.y1 + caption_height };

	const TDCOLOR shadow_color = TDRGB ( 80, 40, 40 );
	const int   shadow_thick = 3;
	const TDRECT	shadow_linex = { rect.x1, rect.y2 + shadow_thick / 2, rect.x2, rect.y2 + shadow_thick / 2 };
	const TDRECT	shadow_liney = { rect.x2 + shadow_thick / 2, rect.y1, rect.x2 + shadow_thick / 2, rect.y2 };

//		cDRAW::Line( shadow_color, shadow_linex.x1, shadow_linex.y1, shadow_linex.x2, shadow_linex.y2, shadow_thick );
//		cDRAW::Line( shadow_color, shadow_liney.x1, shadow_liney.y1, shadow_liney.x2, shadow_liney.y2, shadow_thick );
	cDRAW::Fill( back_ground_color, rect.x1, rect.y1, rect.x2, rect.y2 );
	cDRAW::Box ( box_line_color, rect.x1, rect.y1, rect.x2, rect.y2, 2 );

	cDRAW::Fill( caption_back_color, caption_rect.x1, caption_rect.y1, caption_rect.x2, caption_rect.y2 );
	cDRAW::Box ( caption_line_color, caption_rect.x1, caption_rect.y1, caption_rect.x2, caption_rect.y2, 2 );

	render_buttons ( hDC );
}

bool TDLanguageBox::on_imn_changecandidate ( DWORD dwParam1, DWORD dwParam2 )
{
	on_imn_opencandidate ( dwParam1, dwParam2 );
	/*
	candilist  = ( LPCANDIDATELIST )alloc_memory ;
	hIMC = ImmGetContext ( hWnd );	
	size = ImmGetCandidateList ( hIMC, 0, candilist, alloc_size );
	candilist->dwPageStart = 2;
	for ( i = 1, off = 0; i < index.size() + 1; off = i, i ++ )
	{
		index [ off ] = alloc_memory + candilist->dwOffset [ off ];
	}
	ImmReleaseContext ( hWnd, hIMC );
	break;
	*/
	return true;
}
bool TDLanguageBox::on_imn_opencandidate ( DWORD dwParam1, DWORD dwParam2 )
{
	int i, off, size;
	HIMC hIMC = (HIMC)dwParam1;
	LPCANDIDATELIST candilist;

	state = ecOpen;
	size = ImmGetCandidateList ( hIMC, 0, NULL, 0 );
	if ( size > 0 )
	{
		candilist = ( LPCANDIDATELIST ) Alloc ( size );
		ImmGetCandidateList ( hIMC, 0, candilist, alloc_size );
		index.resize( candilist->dwCount );
		setEnable ( true );
	}
	else
	{
		Release ( );
	}

	for ( i = 1, off = 0; i < index.size ( ) + 1; off = i, i ++ )
	{
		index [ off ] = alloc_memory + candilist->dwOffset [ off ];
	}
	return true;
}
bool TDLanguageBox::on_imn_closecandidate ( DWORD dwParam1, DWORD dwParam2 )
{
	state = ecClose;
	setEnable ( false );
	return true;
}
bool TDLanguageBox::on_imn_default ( DWORD dwParam1, DWORD dwParam2 )
{
	state = ecNone;
	return true;
}

TDLanguageBox::TDLanguageBox ( )
	: width ( 150 ), height ( 166 ), alloc_size ( 0 ), alloc_memory ( 0 )
{
	using namespace TDLanguageBox_Defines;

	cmd_imn_changecandidate.bind( this, &TDLanguageBox::on_imn_changecandidate );
	cmd_imn_opencandidate.bind( this, &TDLanguageBox::on_imn_opencandidate );
	cmd_imn_closecandidate.bind( this, &TDLanguageBox::on_imn_closecandidate );
	cmd_imn_default.bind( this, &TDLanguageBox::on_imn_default );

	state = ecNone;
	ui_state = ec_ui_none;
	message1 = - 1;
	message2 = - 1;
	nPos = 0;
	code_page = 0;
	max_code_page = 0;

	is_enable = false;
	setRect ( startx, starty );

	hFont = CreateFont ( font_height, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, NULL );
}
TDLanguageBox::~TDLanguageBox ( )
{
	Release ( );
	DeleteObject ( hFont );
}
void TDLanguageBox::setRect ( int x, int y )
{
	using namespace TDLanguageBox_Defines;
	if ( x < 0 ) x = 0;
	if ( y < 0 ) y = 0;
	if ( x + width > screen_width ) x = screen_width - width;
	if ( y + height > screen_height ) y = screen_height - height;

	rect.x1 = x;
	rect.y1  = y;
	rect.x2  = x + width;
	rect.y2 = y + height;
}

// true 이면 캔디박스 선택됨
bool TDLanguageBox::on_update ( int x, int y, bool is_lb_down )
{
	bool focus_state;
	static bool button_locked = false ;
	static int dx, dy;
	
	if ( is_enable == false ) return button_locked;

	focus_state = is_mouse_on_rect ( x, y );

	if ( focus_state == false && button_locked == false ) return button_locked;

	if ( button_locked == false )
		ui_state = get_action_state ( x, y, is_lb_down );


	switch ( ui_state )
	{
		case ec_ui_caption_selected:
		if ( button_locked == false  )
		{
			if ( is_lb_down == true )
			{
				button_locked = true;
				dx = x - rect.x1  ;
				dy = y - rect.y1 ;
			}
		}
		else 
		{
			if ( is_lb_down == false )
			{
				button_locked = false;
				dx = 0;
				dy = 0;
			}
		}

		if ( button_locked  )
		{
			setRect ( x - dx, y - dy );
		}			
		break;
	case ec_ui_mouse_on_list:
		message1 = get_list_number ( x, y );
		break;

	case ec_ui_button_selected:
		{
			static bool button_clicked = false;

			if ( message1 == -1 ) break;
			if ( button_locked == false  )
			{
				if ( is_lb_down == true )
				{
					button_locked = true;
					message2 = 1;
					button_clicked = true;
				}
			}
			else if ( button_locked == true ) 
			{
				if ( is_lb_down == false )
				{
					button_locked = false;
					message2 = 0;
				}
			}
			


			if ( button_clicked == false ) break;
			switch ( message1 )
			{
			case 0:
				{
				
/*
				HIMC hIMC;
				DWORD len = sizeof (  CANDIDATEFORM );


				hIMC = ImmGetContext ( hWnd );
				if ( 0 == ImmNotifyIME( hIMC, NI_CHANGECANDIDATELIST, 1, 0) )
				_asm int 3;
				if ( 0 == ImmNotifyIME( hIMC, NI_SETCANDIDATE_PAGESTART, 0, 0) )
				_asm int 3;
			//	if ( 0 == ImmNotifyIME ( hIMC, NI_SELECTCANDIDATESTR, 0, 0 ) )
			//		_asm int 3;
				ImmReleaseContext ( hWnd, hIMC ); 
				*/
				break;
				}
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			}

			button_clicked = false;


		}

		break;
		
	case ec_ui_none:
		break;
	}
	return button_locked;
}

void TDLanguageBox::setVisible ( bool is_visible )
{
	state = ecNone;
}

void TDLanguageBox::setEnable ( bool enable)
{
	is_enable = enable;
}

void TDLanguageBox::on_render ( HDC hDC )
{
	HFONT h_oldfont;

	if ( is_enable == false ) return ;
	render_ui ( hDC );
	h_oldfont = ( HFONT )SelectObject ( hDC, hFont );
	render_text ( hDC, index );
	SelectObject ( hDC, h_oldfont );
}




/**
******************************************************************************
 문자 조합 과정을 처리하는 IME 기본 인터페이스
******************************************************************************
*/
ILanguageCompo::ILanguageCompo ( )
	: width ( 70 ), height ( 20 )
{
	using namespace TDLanguageCompo_Defines;

	is_enable = false;
	setRect ( 0, 0 );
	hFont = CreateFont ( font_height, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, NULL );
}
ILanguageCompo::~ILanguageCompo ( )
{
	DeleteObject ( hFont );
}

void ILanguageCompo::setEnable ( bool enable)
{
	is_enable = enable;
}

void ILanguageCompo::setVisible ( bool is_visible )
{
	ui_state = ec_ui_none;
}

void ILanguageCompo::setRect ( int x, int y )
{
	using namespace TDLanguageCompo_Defines;

	if ( x < 0 ) x = 0;
	if ( y < 0 ) y = 0;
	if ( x + width > screen_width ) x = screen_width - width;
	if ( y + height > screen_height ) y = screen_height - height;

	rect.x1 = x;
	rect.y1  = y;
	rect.x2  = x + width;
	rect.y2 = y + height;
}


ILanguageCompo::eUISTATE ILanguageCompo::get_action_state ( int x, int y, bool is_lb_down )
{		
	if ( is_mouse_on_rect ( x, y, rect ) )
	{
		return ec_ui_selected;
	}
	else
	{
		return ec_ui_none;
	}
}

bool ILanguageCompo::is_mouse_on_rect ( int x, int y, TDRECT in_rect )
{
	if ( x > in_rect.x1 )
	{
		if ( x < in_rect.x2 )
		{
			if ( y > in_rect.y1 )
			{
				if ( y < in_rect.y2 )
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool ILanguageCompo::is_mouse_on_rect ( int x, int y )
{
	return is_mouse_on_rect ( x, y, rect );
}


/**
******************************************************************************
 
******************************************************************************
*/

TDLanguageCompo::TDLanguageCompo ( )
{
	using namespace TDLanguageCompo_Defines;

	cmd_ime_composition.bind( this, &TDLanguageCompo::on_ime_composition );
	cmd_ime_endcomposition.bind ( this, &TDLanguageCompo::on_ime_endcomposition );
	is_enable = false;
	setRect ( 50, 300 );
}
TDLanguageCompo::~TDLanguageCompo ( )
{
}


bool TDLanguageCompo::on_update ( int x, int y, bool is_lb_down )
{
	bool focus_state;
	static bool button_locked = false ;
	static int dx, dy;

	if ( is_enable == false ) return button_locked;
	focus_state = is_mouse_on_rect ( x, y );

	if ( focus_state == false && button_locked == false ) return button_locked;

	if ( button_locked == false )
		ui_state = get_action_state ( x, y, is_lb_down );


	switch ( ui_state )
	{
	case ec_ui_selected:
		if ( button_locked == false  )
		{
			if ( is_lb_down == true )
			{
				button_locked = true;
				dx = x - rect.x1  ;
				dy = y - rect.y1 ;
			}
		}
		else 
		{
			if ( is_lb_down == false )
			{
				button_locked = false;
				dx = 0;
				dy = 0;
			}
		}
		if ( button_locked  )
		{
			setRect ( x - dx, y - dy );
		}
		break;
	}

	return button_locked;
}

void TDLanguageCompo::on_render ( HDC hDC )
{
	HFONT h_oldfont;

	if ( is_enable == false ) return ;
	render_ui ( hDC );
	h_oldfont = ( HFONT )SelectObject ( hDC, hFont );
	render_text ( hDC );
	SelectObject ( hDC, h_oldfont );
}

bool TDLanguageCompo::on_ime_endcomposition ( DWORD dwParam1, DWORD dwParam2 )
{
	setEnable( false );
	return true;
}

bool TDLanguageCompo::on_ime_composition ( DWORD dwParam1, DWORD dwParam2 )
{
	int size;
	HIMC hIMC;
	HWND hWnd = ( HWND )dwParam1;
	LPARAM lParam = ( LPARAM )dwParam2;



	ZeroMemory ( compo_buf, sizeof ( compo_buf ) );
	hIMC = ImmGetContext ( hWnd );
	if( lParam & GCS_RESULTSTR )
	{
		// 조합한 문자 리턴
		size = ImmGetCompositionString ( hIMC, GCS_RESULTSTR, NULL, 0 );
		if ( size > 0 )
		{
			ImmGetCompositionString ( hIMC, GCS_RESULTSTR, compo_buf, size );
			setEnable( false );
#ifdef _PROTOTYPE_
			TDSET_RESULT ( hWnd, NULL, compo_buf );
#else
			cINPUTBOX	*lpInputBox	=	(cINPUTBOX	*)GetWindowLong(hWnd,GWL_USERDATA);
			TDSET_RESULT ( hWnd, lpInputBox,compo_buf );
#endif
		}
	}
	else if ( lParam & GCS_COMPSTR )
	{
		size = ImmGetCompositionString ( hIMC, GCS_COMPSTR, NULL, 0 );
		if ( size > 0 )
		{
			setEnable( true );
			ImmGetCompositionString ( hIMC, GCS_COMPSTR, compo_buf, size );
		}
/*		
		if( size + m_imeBox->GetLength() > m_imeBox->GetMaxLength())	// 최대길이보다 길면.
		{
			ImmNotifyIME(m_hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0);		// 조합중인 문자열을 날려주자.

			m_imeBox->SetComText("",0);
			return true;
		}
		*/
	}
	ImmReleaseContext ( hWnd, hIMC );

	return true;
}

void TDLanguageCompo::render_ui ( HDC hDC )
{
	const TDCOLOR back_ground_color = TDRGB ( 255, 127, 127 );
	const TDCOLOR box_line_color = TDRGB ( 127, 63, 63 );

	cDRAW::Fill( back_ground_color, rect.x1, rect.y1, rect.x2, rect.y2 );
	cDRAW::Box( box_line_color, rect.x1, rect.y1, rect.x2, rect.y2, 2 );
}
void TDLanguageCompo::render_text ( HDC hDC )
{
	TDCOLOR oldcol;
	int oldbk;
    const TDCOLOR text_color = TDRGB ( 110, 55, 55 );
	const int xaxis_margin = 5;
	const int yaxis_margin = 5;
	const int xpos = rect.x1 + xaxis_margin;
	const int ypos = rect.y1 + yaxis_margin;		
	oldbk = SetBkMode ( hDC, TRANSPARENT );
	oldcol = SetTextColor ( hDC, text_color );
	TextOut ( hDC, xpos, ypos, compo_buf, strlen ( compo_buf ) );
	SetTextColor ( hDC, oldcol );
	SetBkMode ( hDC, oldbk );		
}


/**
******************************************************************************
 
******************************************************************************
*/
TDLanguagePinyinCompo::TDLanguagePinyinCompo ( )
{
	using namespace TDLanguageCompo_Defines;
	ori_buf = 0;
	cmd_ime_composition.bind( this, &TDLanguagePinyinCompo::on_ime_composition );
	cmd_ime_endcomposition.bind ( this, &TDLanguagePinyinCompo::on_ime_endcomposition );
	is_enable = false;
	setRect ( 50, 300 );
}
TDLanguagePinyinCompo::~TDLanguagePinyinCompo ( )
{
	if ( ori_buf != NULL )
		delete ori_buf;
}

bool TDLanguagePinyinCompo::on_update ( int x, int y, bool is_lb_down )
{
	bool focus_state;
	static bool button_locked = false ;
	static int dx, dy;

	if ( is_enable == false ) return button_locked;
	focus_state = is_mouse_on_rect ( x, y );

	if ( focus_state == false && button_locked == false ) return button_locked;

	if ( button_locked == false )
		ui_state = get_action_state ( x, y, is_lb_down );


	switch ( ui_state )
	{
	case ec_ui_selected:
		if ( button_locked == false  )
		{
			if ( is_lb_down == true )
			{
				button_locked = true;
				dx = x - rect.x1  ;
				dy = y - rect.y1 ;
			}
		}
		else 
		{
			if ( is_lb_down == false )
			{
				button_locked = false;
				dx = 0;
				dy = 0;
			}
		}
		if ( button_locked  )
		{
			setRect ( x - dx, y - dy );
		}
		break;
	}

	return button_locked;
}

void TDLanguagePinyinCompo::on_render ( HDC hDC )
{
	HFONT h_oldfont;

	if ( is_enable == false ) return ;
	render_ui ( hDC );
	h_oldfont = ( HFONT )SelectObject ( hDC, hFont );
	render_text ( hDC );
	render_text2 ( hDC );
	SelectObject ( hDC, h_oldfont );
}

bool TDLanguagePinyinCompo::on_ime_endcomposition ( DWORD dwParam1, DWORD dwParam2 )
{
	setEnable( false );
	return true;
}

bool TDLanguagePinyinCompo::on_ime_composition ( DWORD dwParam1, DWORD dwParam2 )
{
	int size;
	HIMC hIMC;
	HWND hWnd = ( HWND )dwParam1;
	LPARAM lParam = ( LPARAM )dwParam2;
	cINPUTBOX	*lpInputBox	=	(cINPUTBOX	*)GetWindowLong(hWnd,GWL_USERDATA);

	ZeroMemory ( compo_buf, sizeof ( compo_buf ) );

	hIMC = ImmGetContext ( hWnd );
	if( lParam & GCS_RESULTSTR )
	{
		size = ImmGetCompositionString ( hIMC, GCS_RESULTSTR, NULL, 0 );
		// 조합한 문자 리턴
		if ( size > 0 )
		{
			ImmGetCompositionString ( hIMC, GCS_RESULTSTR, compo_buf, size );
			setEnable( false );
			TDSET_RESULT ( hWnd, lpInputBox, compo_buf );
		}
		size = ImmGetCompositionString ( hIMC, GCS_RESULTREADSTR, NULL, 0 );
		if ( size > 0 )
		{
			setEnable( true );

			if ( ori_buf != NULL )
				delete ori_buf;
			ori_buf = new char [ size ];
			ZeroMemory ( ori_buf, size );
			ImmGetCompositionString ( hIMC, GCS_RESULTREADSTR, ori_buf, size - 1);
		}
	}
	else if ( lParam & GCS_COMPSTR )
	{
		size = ImmGetCompositionString ( hIMC, GCS_COMPSTR, NULL, 0 );
		if ( size > 0 )
		{
			setEnable( true );
			ImmGetCompositionString ( hIMC, GCS_COMPSTR, compo_buf, size );
		}
		size = ImmGetCompositionString ( hIMC, GCS_COMPREADSTR, NULL, 0 );
		if ( size > 0 )
		{
			setEnable( true );
			if ( ori_buf != NULL )
				delete ori_buf;
			ori_buf = new char [ size ];
			ZeroMemory ( ori_buf, size );
			ImmGetCompositionString ( hIMC, GCS_COMPREADSTR, ori_buf, size - 1 );
		}
	}
	ImmReleaseContext ( hWnd, hIMC );

	return true;
}

void TDLanguagePinyinCompo::render_ui ( HDC hDC )
{
	const TDCOLOR back_ground_color = TDRGB ( 255, 127, 127 );
	const TDCOLOR box_line_color = TDRGB ( 127, 63, 63 );

	cDRAW::Fill( back_ground_color, rect.x1, rect.y1, rect.x2, rect.y2 );
	cDRAW::Box( box_line_color, rect.x1, rect.y1, rect.x2, rect.y2, 2 );
}
void TDLanguagePinyinCompo::render_text ( HDC hDC )
{
	TDCOLOR oldcol;
	int oldbk;
    const TDCOLOR text_color = TDRGB ( 110, 55, 55 );
	const int xaxis_margin = 5;
	const int yaxis_margin = 5;
	int xpos = rect.x1 + xaxis_margin;
	int ypos = rect.y1 + yaxis_margin;		
	int compo_len = strlen ( compo_buf );
//		GetCharWidth32 ( hDC, 1, compo_len - 1, &xpos_offset );

	oldbk = SetBkMode ( hDC, TRANSPARENT );
	oldcol = SetTextColor ( hDC, text_color );
	TextOut ( hDC, xpos, ypos, compo_buf, compo_len );
	SetTextColor ( hDC, oldcol );
	SetBkMode ( hDC, oldbk );		
}
void TDLanguagePinyinCompo::render_text2 ( HDC hDC )
{/*
	TDCOLOR oldcol;
	int oldbk;
    const TDCOLOR text_color = TDRGB ( 110, 55, 55 );
	const int xaxis_margin = 5;
	const int yaxis_margin = 5;
	const int xpos = rect.x1 + xaxis_margin + 8;
	const int ypos = rect.y1 + yaxis_margin ;
	char  word [256 ];
	oldbk = SetBkMode ( hDC, TRANSPARENT );
	oldcol = SetTextColor ( hDC, text_color );
	if ( ori_buf != NULL )

	get_current_ori ( word );
	TextOut ( hDC, xpos, ypos, word , strlen ( word ) );
	SetTextColor ( hDC, oldcol );
	SetBkMode ( hDC, oldbk );	
	*/
}

// ori_buf 파싱.
char* TDLanguagePinyinCompo::get_current_ori ( char* in_buf )
{
	char*		word = in_buf ;
	int			wordsize;
	char* buf = ori_buf;
	char  ch;
	int	  offset;
	bool  is_blank = false ;

	offset = 0;
	wordsize = 0;
	word [ 0 ] = '\0';
	while ( 1 )
	{
		ch = buf [ offset ];
		if ( ch == '\0' )
			break;

		offset++;

		// 공백문자일때.
		if ( ch == 0x20 )
		{
			wordsize = 0;
		}
		else
		{
			word [ wordsize ] = ch;
			word [ wordsize + 1 ] = '\0';
			wordsize ++;
		}
	}
	return word;
}




/**
******************************************************************************
 tdlanguage 클래스. 중국어 입력을 처리하는 메인 클래스.
******************************************************************************
*/
/*
TDLanguage::TDLanguage ( )
	: language_box ( 0 ), pinyin_compo_box ( 0 ), compo_box ( 0 )
{
	set_input_method_state ( ecQuanPin );
}

TDLanguage::~TDLanguage ( ) 
{
	Release ( );
}

void TDLanguage::Release ( )
{
	if ( language_box != NULL )
	{
		delete language_box;
		language_box = NULL;
	}

	if ( compo_box != NULL )
	{
		delete compo_box;
		compo_box = NULL;
	}

	if ( pinyin_compo_box != NULL )
	{
		delete pinyin_compo_box;
		pinyin_compo_box = NULL;
	}
}

void TDLanguage::set_language_box ( TDLanguageBox* box )
{
	language_box = box;
	cmd_imn_changecandidate = &box->cmd_imn_changecandidate;
	cmd_imn_opencandidate = &box->cmd_imn_opencandidate;
	cmd_imn_closecandidate = &box->cmd_imn_closecandidate;
	cmd_imn_default = &box->cmd_imn_default;
}

void TDLanguage::set_language_compo ( TDLanguageCompo* compo )
{
	compo_box = compo;
	cmd_ime_composition = &compo->cmd_ime_composition;
	cmd_ime_endcomposition = &compo->cmd_ime_endcomposition;
}

void TDLanguage::set_language_compo2 ( TDLanguagePinyinCompo* compo )
{
	if ( compo == NULL )
		return;
	pinyin_compo_box = compo;
	cmd_ime_composition = &compo->cmd_ime_composition;
	cmd_ime_endcomposition = &compo->cmd_ime_endcomposition;
}

// 팩토리.
void TDLanguage::set_input_method_state ( eInputMethod in_estate )
{
	TDLanguagePinyinCompo* pinyin_compo;
	TDLanguageCompo* compo;
	TDLanguageBox* box; 

	Release ( );
	switch ( in_estate )
	{
	case ecNeiMa:
		box = new TDLanguageBox ( );
		compo = new TDLanguageCompo ( );
		pinyin_compo = NULL;
		break;
	case ecQuanPin:
		box = new TDLanguageBox ( );
		compo = new TDLanguageCompo ( );
		pinyin_compo = NULL;
		break;
	case ecShuangPin:
		box = new TDLanguageBox ( );
		compo = new TDLanguageCompo ( );
		pinyin_compo = NULL;
		break;
	case ecEnglish:
		box = new TDLanguageBox ( );
		compo = new TDLanguageCompo ( );
		pinyin_compo = NULL;
		break;
	case ecZhengMa:
		box = new TDLanguageBox ( );
		compo = new TDLanguageCompo ( );
		pinyin_compo = NULL;
		break;
	case ecPinyin:
		box = new TDLanguageBox ( );
		compo = new TDLanguageCompo ( );
		pinyin_compo = new TDLanguagePinyinCompo ( );
		break;	
	}

	set_language_box ( box );
	set_language_compo ( compo );
	set_language_compo2 ( pinyin_compo );
}

void TDLanguage::render ( HDC hDC )
{
	if ( language_box )
		language_box->on_render( hDC );
	if ( compo_box )
		compo_box->on_render ( hDC );
	if ( pinyin_compo_box )
		pinyin_compo_box->on_render ( hDC );
}

bool TDLanguage::update ( int x, int y, bool is_lb_down )
{
	bool state;
	if ( language_box )
	state = language_box->on_update( x, y, is_lb_down );
	if ( state == true ) return true;

	if ( compo_box )
	state = compo_box->on_update( x, y, is_lb_down );
	if ( state == true ) return true;

	if ( pinyin_compo_box )
	state = pinyin_compo_box->on_update ( x, y, is_lb_down );
	if ( state == true ) return true ;

	return false;
}

void TDLanguage::setVisible ( bool is_visible )
{
	if ( language_box )
		language_box->setVisible ( is_visible );
	if ( compo_box )
		compo_box->setVisible ( is_visible );
	if ( pinyin_compo_box )
		pinyin_compo_box->setVisible ( is_visible );
}

void TDLanguage::setPos ( int x, int y )
{
	language_box->setRect ( x+70, y );
	compo_box->setRect ( x, y );
}

int TDLanguage::MessageProc ( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if ( language_box == NULL )
		goto _def;
	if ( compo_box == NULL )
		goto _def;
	switch ( message )
	{
		case WM_IME_NOTIFY:
			on_ime_notify ( hWnd, wParam );
			break;
		case WM_IME_COMPOSITION:
			on_ime_composition ( hWnd, lParam );
			break;
		case WM_IME_ENDCOMPOSITION:
			on_ime_endcomposition ( );
			break;
		default:
			goto _def;
	}
	return 0;
_def:
#ifdef _PROTOTYPE_
	return DefWindowProc ( hWnd, message, wParam, lParam );
#else
	cINPUTBOX	*lpInputBox	=	(cINPUTBOX	*)GetWindowLong(hWnd,GWL_USERDATA);
	if ( lpInputBox == NULL ) 	
		return DefWindowProc ( hWnd, message, wParam, lParam );

	if ( lpInputBox->isFocus ( ) == FALSE )
	{
		setVisible ( false );
	}
	return CallWindowProc((WNDPROC)lpInputBox->m_procOld,hWnd, message, wParam, lParam);
#endif
}

////////////////////////////////////////////////////////////////////////////////////
// begin. ime event. this codes placed into window message loop.
void TDLanguage::on_ime_notify ( HWND hWnd, WPARAM wParam )
{
	HIMC hIMC;

	try 
	{
		hIMC = ImmGetContext ( hWnd );	
		switch ( (DWORD)wParam )
		{
		case IMN_SETCONVERSIONMODE:
			on_imn_setconversionmode ( hIMC );
			break;
		case IMN_OPENSTATUSWINDOW:
			on_imn_openstatuswindow ( hIMC );
			break;
		case IMN_CHANGECANDIDATE :
			on_imn_changecandidate ( hIMC );
			break;
		case IMN_OPENCANDIDATE:
			on_imn_opencandidate ( hIMC );
			break;
		case IMN_CLOSECANDIDATE:
			on_imn_closecandidate ( );
			break;
		default:
			on_imn_default ( );
			break;
		}
		ImmReleaseContext ( hWnd, hIMC );
	}
	catch ( char *str )
	{
		MessageBox ( hWnd, str, NULL, NULL );
		// 오류 있으니 제발좀 잡으쇼!! 
		_asm int 3;
	}
}

void TDLanguage::on_ime_composition ( HWND hWnd, LPARAM lParam )
{
	cmd_ime_composition->execute ( ( DWORD )hWnd, ( DWORD )lParam );
}
void TDLanguage::on_ime_endcomposition ( )
{
	cmd_ime_endcomposition->execute ( );
}
// end of ime event. this codes placed into window message loop.
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
// begin. notify event.  this codes placed into ime_notify.
void TDLanguage::on_imn_setconversionmode ( HIMC hIMC )
{
}
void TDLanguage::on_imn_openstatuswindow ( HIMC hIMC )
{
	HKL hKL;
	char str[256];
	int nresult;
	hKL = GetKeyboardLayout(0);
	nresult = ImmGetDescription( hKL, str, sizeof(str ) );
	if ( nresult == 0 )	
		str[0] = NULL;
	input_mode = get_input_method_state ( str );
	set_input_method_state ( input_mode );
}
void TDLanguage::on_imn_changecandidate ( HIMC hIMC )
{
	cmd_imn_changecandidate->execute( (DWORD) hIMC );
}
void TDLanguage::on_imn_opencandidate ( HIMC hIMC )
{
	cmd_imn_opencandidate->execute( (DWORD) hIMC );
}
void TDLanguage::on_imn_closecandidate ( )
{
	cmd_imn_closecandidate->execute( );
}
void TDLanguage::on_imn_default ( )
{
	cmd_imn_default->execute ( );
}
// end of imn event.   this codes placed into ime_notify.
////////////////////////////////////////////////////////////////////////////////////

eInputMethod TDLanguage::get_input_method_state ( const char* in_state )
{
	// 문제가 있는데.. ZhengMa와 ShuangPin을 구별 할수가 없다는것이다.
	const char* szNeiMa     = "?位?入法 5.0 版";
	const char* szQuanPin   = "全??入法 5.0 版";
	const char* szShuangPin = "???入法 5.0 版";
	const char* szEnglish   = "";
	const char* szZhengMa   = "???入法 5.0 版";
	const char* szPinyin    = "微??音?入法 3.0";

	int nresult;

	nresult = strcmp ( in_state,  szNeiMa );
	if ( nresult == 0 )
		return ecNeiMa;
	nresult = strcmp ( in_state,  szQuanPin );
	if ( nresult == 0 )
		return ecQuanPin;
	nresult = strcmp ( in_state,  szShuangPin );
	if ( nresult == 0 )
		return ecShuangPin;
	nresult = strcmp ( in_state,  szEnglish );
	if ( nresult == 0 )
		return ecEnglish;
	nresult = strcmp ( in_state,  szZhengMa );
	if ( nresult == 0 )
		return ecZhengMa;
	nresult = strcmp ( in_state,  szPinyin );
	if ( nresult == 0 )
		return ecPinyin;
	return ecEnglish;
}
*/

//////////////////////////////////////////////////////////////////////////////////////
// 땜빵용 코드

TDLanguage::TDLanguage ( )
{
	ime_state = true;
	ok_state = true;
}
TDLanguage::~TDLanguage()
{
}
void TDLanguage::set_state ( bool in )
{
	ok_state =in;
}
bool TDLanguage::is_ok()
{
	return ime_state && ok_state;
}
void TDLanguage::update(bool lb, bool rb)
{
	if ( ok_state )
	{
		if ( !lb )
			ime_state = true;
	}
}
int TDLanguage::MainMessageProc ( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	HIMC hIMC;
	switch ( message )
	{
		case WM_IME_NOTIFY:
		case WM_IME_COMPOSITION:
		hIMC = ImmGetContext ( hWnd );
		ImmNotifyIME(hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0);		// 조합중인 문자열을 날려주자.
		ImmNotifyIME ( hIMC, NI_CLOSECANDIDATE, 0, 0 );
		ImmDestroyContext ( hIMC );
		ImmReleaseContext ( hWnd, hIMC );
		return 0;
	}
	return 1;
}

int TDLanguage::TalkMessageProc ( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	HIMC hIMC;
//	if ( ImmIsUIMessage ( hWnd, message, wParam, lParam ) == false )
//		return 1;

	switch ( message )
	{
	case WM_IME_NOTIFY:
		hIMC = ImmGetContext ( hWnd );	
		switch ( (DWORD)wParam )
		{
		case IMN_OPENCANDIDATE:
			ok_state = false;
			break;
		case IMN_CLOSECANDIDATE :
			ok_state = true;
			ime_state = false;
			break;
		}
		ImmReleaseContext ( hWnd, hIMC );
		break;
	case WM_IME_STARTCOMPOSITION:
		ok_state = false;
		break;
	case WM_IME_ENDCOMPOSITION:
		ok_state = true;
		ime_state = false;
		break;
	}
	return 0;
}
