/*****************************************************************************

  개			  요 :	Direct Input 를 기반으로 한 윈도우용 입력 라이브러리.

  제한			사항 :	본 클래스를 제작자의 허락 없이 어떤식으로도 전용하는것을 금합니다.

  주의			사항 :	싱글톤 클래스로 사용시에는 상속 받아서 사용한다.

  필요	  라이브러리 :	dinput.lib<dinput.h>

  작성			날짜 :	몰라.. 오늘은.. 98.04.10

*****************************************************************************/
#ifndef _classINPDEV_H
#define _classINPDEV_H

#include "SFC.H"

#define DIRECTINPUT_VERSION         0x0800

#include <dinput.h>


#pragma comment(lib,"dinput8.lib ")

class cINPDEV
{
public:
	static	LPDIRECTINPUT8			s_lpDI;
	static	LPDIRECTINPUTDEVICE8	s_lpDIDKeyboard;
	static	LPDIRECTINPUTDEVICE8	s_lpDIDMouse;

	static	HWND					s_hInpdevWnd;
	static	cRECT					s_rectActiveArea;
	static	int						s_iExclusiveObject;	//	어떤거한테 독점-_-되었다.

	static	DWORD					s_dwIdleTime1;
	static	DWORD					s_dwIdleTime2;
	static	BOOL					s_isOldLB;
	static	BOOL					s_isOldRB;

	static	BOOL					s_bIsReadyToDrag;
	static	BOOL					s_bIsDragMouse;
	static	CPos					s_posLastForLeftDblClick,s_posLastForRightDblClick;
	static	CPos					s_posMouse;			//	커서의 위치
	static	CPos					s_posOld;
	static	BYTE					s_aOldKey[256];
	static	BYTE					s_aKey[256];		//	키보드 눌렸냐?
	static	BYTE					s_aTouched[256];	//	키보드 눌렸냐?(누르는 시점에서 반응)
	static	BYTE					s_aClicked[256];	//	키보드 눌렸냐?(눌렀다가 떼는 시점에서 반응)
	static	BOOL					s_isDoubleClickLeftButton;	//	더블 클릭?
	static	BOOL					s_isDoubleClickRightButton;
	static	BOOL					s_isClickedLeftButton;	//	마우스 버튼 눌렀다가 떼는 시점에서 한번 설정됨
	static	BOOL					s_isClickedRightButton;
	static	BOOL					s_isTouchLeftButton;	//	마우스 버튼 누르는 시점에서 한번 설정됨
	static	BOOL					s_isTouchRightButton;
	static	BOOL					s_isLeftButtonPressed;	//	마우스 버튼 누르고 있으면 쭈욱~~
	static	BOOL					s_isRightButtonPressed;
	static	BOOL					s_isMovedMouse;			//	마우스가 움직였냐?
	static	BOOL					s_isKbhit;				//	키보드나 마우스.. 뭔가 눌려졌냐?
	static	BOOL					s_bIsWaitForReleaseLeftButton;
	static	int						s_iWheelValue,s_iRareWheelValue;

									~cINPDEV();

	static	BOOL					Init(HWND hWnd,HINSTANCE hInst,cRECT *rect=NULL);
	static	void					Destroy();
	static	void					Reset();
	static	void					Update(void);
	static	inline	void			SetMouseActiveArea(int _iX1,int _iY1,int _iX2,int _iY2)	{s_rectActiveArea.set(_iX1,_iY1,_iX2,_iY2);}//	마우스 활동 영역 설정
	static	inline	void			SetWheelValue(int _iValue)	{s_iRareWheelValue	=	_iValue;}//	마우스 활동 영역 설정

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃독점 관련 처리
	static	inline	int				GetExclusive(){return	s_iExclusiveObject;}//	독점하고 잇는넘
	static	inline	BOOL			IsExclusive(int _iObject=-1)	//	누군가 독점하고있냐? 내가 독점하고 있다면 독점 안하는걸로 나온다.
	{
		if (s_iExclusiveObject == -1		)	return	FALSE;
		if (s_iExclusiveObject == _iObject	)	return	FALSE;

		return	TRUE;
	}	
	static	void					SetExclusive(int _iObject)	{s_iExclusiveObject	=	_iObject;}	//	독점 설정
	static	void					ReleaseExclusive()	{s_iExclusiveObject	=	-1;}//	독점 해제
	static	void					ReleaseExclusive(BOOL _is,int _iObject){if (s_iExclusiveObject	==	_iObject && _is)	ReleaseExclusive();}//	독점 해제
//┃독점 관련 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
};

#define KEY_ESCAPE          0x01
#define KEY_1               0x02
#define KEY_2               0x03
#define KEY_3               0x04
#define KEY_4               0x05
#define KEY_5               0x06
#define KEY_6               0x07
#define KEY_7               0x08
#define KEY_8               0x09
#define KEY_9               0x0A
#define KEY_0               0x0B
#define KEY_MINUS           0x0C    /* - on main keyboard */
#define KEY_PLUS			0x0D
#define KEY_BACK            0x0E    /* backspace */
#define KEY_TAB             0x0F
#define KEY_Q               0x10
#define KEY_W               0x11
#define KEY_E               0x12
#define KEY_R               0x13
#define KEY_T               0x14
#define KEY_Y               0x15
#define KEY_U               0x16
#define KEY_I               0x17
#define KEY_O               0x18
#define KEY_P               0x19
#define KEY_LBRACKET        0x1A
#define KEY_RBRACKET        0x1B
#define KEY_RETURN          0x1C    /* Enter on main keyboard */
#define KEY_LCONTROL        0x1D
#define KEY_A               0x1E
#define KEY_S               0x1F
#define KEY_D               0x20
#define KEY_F               0x21
#define KEY_G               0x22
#define KEY_H               0x23
#define KEY_J               0x24
#define KEY_K               0x25
#define KEY_L               0x26
#define KEY_SEMICOLON       0x27
#define KEY_APOSTROPHE      0x28
#define KEY_GRAVE           0x29    /* accent grave */
#define KEY_LSHIFT          0x2A
#define KEY_BACKSLASH       0x2B
#define KEY_Z               0x2C
#define KEY_X               0x2D
#define KEY_C               0x2E
#define KEY_V               0x2F
#define KEY_B               0x30
#define KEY_N               0x31
#define KEY_M               0x32
#define KEY_COMMA           0x33
#define KEY_PERIOD          0x34    /* . on main keyboard */
#define KEY_SLASH           0x35    /* / on main keyboard */
#define KEY_RSHIFT          0x36
#define KEY_MULTIPLY        0x37    /* * on numeric keypad */
#define KEY_LALT			0x38    /* left Alt */
#define KEY_SPACE           0x39
#define KEY_CAPITAL         0x3A
#define KEY_F1              0x3B
#define KEY_F2              0x3C
#define KEY_F3              0x3D
#define KEY_F4              0x3E
#define KEY_F5              0x3F
#define KEY_F6              0x40
#define KEY_F7              0x41
#define KEY_F8              0x42
#define KEY_F9              0x43
#define KEY_F10             0x44
#define KEY_NUMLOCK         0x45
#define KEY_SCROLL          0x46    /* Scroll Lock */
#define KEY_NUMPAD7         0x47
#define KEY_NUMPAD8         0x48
#define KEY_NUMPAD9         0x49
#define KEY_PAD_MINUS		0x4A    /* - on numeric keypad */
#define KEY_NUMPAD4         0x4B
#define KEY_NUMPAD5         0x4C
#define KEY_NUMPAD6         0x4D
#define KEY_PAD_PLUS		0x4E    /* + on numeric keypad */
#define KEY_NUMPAD1         0x4F
#define KEY_NUMPAD2         0x50
#define KEY_NUMPAD3         0x51
#define KEY_NUMPAD0         0x52
#define KEY_DECIMAL         0x53    /* . on numeric keypad */
#define KEY_OEM_102         0x56    /* < > | on UK/Germany keyboards */
#define KEY_F11             0x57
#define KEY_F12             0x58

#define KEY_F13             0x64    /*                     (NEC PC98) */
#define KEY_F14             0x65    /*                     (NEC PC98) */
#define KEY_F15             0x66    /*                     (NEC PC98) */

#define KEY_KANA            0x70    /* (Japanese keyboard)            */
#define KEY_ABNT_C1         0x73    /* / ? on Portugese (Brazilian) keyboards */
#define KEY_CONVERT         0x79    /* (Japanese keyboard)            */
#define KEY_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#define KEY_YEN             0x7D    /* (Japanese keyboard)            */
#define KEY_ABNT_C2         0x7E    /* Numpad . on Portugese (Brazilian) keyboards */
#define KEY_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define KEY_PREVTRACK       0x90    /* Previous Track (KEY_CIRCUMFLEX on Japanese keyboard) */
#define KEY_AT              0x91    /*                     (NEC PC98) */
#define KEY_COLON           0x92    /*                     (NEC PC98) */
#define KEY_UNDERLINE       0x93    /*                     (NEC PC98) */
#define KEY_KANJI           0x94    /* (Japanese keyboard)            */
#define KEY_STOP            0x95    /*                     (NEC PC98) */
#define KEY_AX              0x96    /*                     (Japan AX) */
#define KEY_UNLABELED       0x97    /*                        (J3100) */
#define KEY_NEXTTRACK       0x99    /* Next Track */
#define KEY_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define KEY_RCONTROL        0x9D
#define KEY_MUTE            0xA0    /* Mute */
#define KEY_CALCULATOR      0xA1    /* Calculator */
#define KEY_PLAYPAUSE       0xA2    /* Play / Pause */
#define KEY_MEDIASTOP       0xA4    /* Media Stop */
#define KEY_VOLUMEDOWN      0xAE    /* Volume - */
#define KEY_VOLUMEUP        0xB0    /* Volume + */
#define KEY_WEBHOME         0xB2    /* Web home */
#define KEY_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define KEY_DIVIDE          0xB5    /* / on numeric keypad */
#define KEY_SNAPSHOT		0xB7	 //	capture	
#define KEY_RALT		 	0xB8    /* right Alt */
#define KEY_PAUSE           0xC5    /* Pause */
#define KEY_HOME            0xC7    /* Home on arrow keypad */
#define KEY_UP              0xC8    /* UpArrow on arrow keypad */
#define KEY_PGUP			0xC9    /* PgUp on arrow keypad */
#define KEY_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define KEY_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define KEY_END             0xCF    /* End on arrow keypad */
#define KEY_DOWN            0xD0    /* DownArrow on arrow keypad */
#define KEY_PGDN			0xD1    /* PgDn on arrow keypad */
#define KEY_INSERT          0xD2    /* Insert on arrow keypad */
#define KEY_DELETE          0xD3    /* Delete on arrow keypad */
#define KEY_LWIN            0xDB    /* Left Windows key */
#define KEY_RWIN            0xDC    /* Right Windows key */
#define KEY_APPS            0xDD    /* AppMenu key */
#define KEY_POWER           0xDE    /* System Power */
#define KEY_SLEEP           0xDF    /* System Sleep */
#define KEY_WAKE            0xE3    /* System Wake */
#define KEY_WEBSEARCH       0xE5    /* Web Search */
#define KEY_WEBFAVORITES    0xE6    /* Web Favorites */
#define KEY_WEBREFRESH      0xE7    /* Web Refresh */
#define KEY_WEBSTOP         0xE8    /* Web Stop */
#define KEY_WEBFORWARD      0xE9    /* Web Forward */
#define KEY_WEBBACK         0xEA    /* Web Back */
#define KEY_MYCOMPUTER      0xEB    /* My Computer */
#define KEY_MAIL            0xEC    /* Mail */
#define KEY_MEDIASELECT     0xED    /* Media Select */

#define KEY_LBUTTON			256		/* Media Select */
#define KEY_RBUTTON			257		/* Media Select */

#endif