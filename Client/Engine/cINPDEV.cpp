#include "cINPDEV.h"
#include "cExceptionHandler.H"

#ifndef RELEASE
#define RELEASE(x)	if (x) {x->Release();x = NULL ;}
#endif


LPDIRECTINPUT8			cINPDEV::s_lpDI				=	NULL;
LPDIRECTINPUTDEVICE8	cINPDEV::s_lpDIDKeyboard	=	NULL;
LPDIRECTINPUTDEVICE8	cINPDEV::s_lpDIDMouse		=	NULL;     
HWND					cINPDEV::s_hInpdevWnd		=	NULL;
cRECT					cINPDEV::s_rectActiveArea;

DWORD					cINPDEV::s_dwIdleTime1		=	0;
DWORD					cINPDEV::s_dwIdleTime2		=	0;
BOOL					cINPDEV::s_isOldLB			=	FALSE;
BOOL					cINPDEV::s_isOldRB			=	FALSE;
int						cINPDEV::s_iExclusiveObject	=	-1;
int						cINPDEV::s_iWheelValue		=	0;
int						cINPDEV::s_iRareWheelValue	=	0;

CPos					cINPDEV::s_posOld;
CPos					cINPDEV::s_posMouse;			//	커서의 위치
CPos					cINPDEV::s_posLastForLeftDblClick;
CPos					cINPDEV::s_posLastForRightDblClick;
BYTE					cINPDEV::s_aOldKey[256];		//	키보드 눌렸냐?
BYTE					cINPDEV::s_aKey[256];			//	키보드 눌렸냐?
BYTE					cINPDEV::s_aTouched[256];		//	키보드 눌렸냐?(누르는 시점에서 반응)
BYTE					cINPDEV::s_aClicked[256];		//	키보드 눌렸냐?(눌렀다가 떼는 시점에서 반응)
BOOL					cINPDEV::s_isDoubleClickLeftButton;
BOOL					cINPDEV::s_isDoubleClickRightButton;
BOOL					cINPDEV::s_isTouchLeftButton;
BOOL					cINPDEV::s_isTouchRightButton;
BOOL					cINPDEV::s_isClickedLeftButton;
BOOL					cINPDEV::s_isClickedRightButton;
BOOL					cINPDEV::s_isLeftButtonPressed;
BOOL					cINPDEV::s_isRightButtonPressed;
BOOL					cINPDEV::s_isMovedMouse;
BOOL					cINPDEV::s_isKbhit;
BOOL					cINPDEV::s_bIsDragMouse		=	FALSE;
BOOL					cINPDEV::s_bIsReadyToDrag	=	FALSE;
BOOL					cINPDEV::s_bIsWaitForReleaseLeftButton	=	FALSE;


cINPDEV::~cINPDEV()
{	Destroy();
}

//
//	인풋 디바이스 초기화
BOOL
cINPDEV::Init(HWND hWnd,HINSTANCE hInst,cRECT *_lpRectActiveArea)
{	
	if (ERRORED) return g_eh.addStaticLog("passed cINPDEV::Init - because error count not zero");

	s_hInpdevWnd=	hWnd;

	if (_lpRectActiveArea)	memcpy(&s_rectActiveArea,_lpRectActiveArea,sizeof(cRECT));
	else
	{
		s_rectActiveArea.clientFrame(s_hInpdevWnd);
		s_rectActiveArea.x2--;
		s_rectActiveArea.y2--;
	}

    HRESULT hr;

	Destroy();

	if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION,IID_IDirectInput8, (VOID**)&s_lpDI, NULL ) ) )
		return ERRMSG(hr);

	//	키보드
	if( FAILED( hr = s_lpDI->CreateDevice( GUID_SysKeyboard, &s_lpDIDKeyboard, NULL					)))	return ERRMSG(hr);
	if( FAILED( hr = s_lpDIDKeyboard->SetDataFormat( &c_dfDIKeyboard								)))	return ERRMSG(hr);
	if( FAILED( hr = s_lpDIDKeyboard->SetCooperativeLevel(s_hInpdevWnd,DISCL_NONEXCLUSIVE|DISCL_FOREGROUND)))	return ERRMSG(hr);

	s_lpDIDKeyboard->Acquire();
	memset(s_aOldKey,0,sizeof(s_aOldKey));

	//	마우스
    if( FAILED( hr = s_lpDI->CreateDevice( GUID_SysMouse, &s_lpDIDMouse, NULL						)))	return ERRMSG(hr);
	if( FAILED( hr = s_lpDIDMouse->SetDataFormat( &c_dfDIMouse2									)))	return ERRMSG(hr);
	if( FAILED( hr = s_lpDIDMouse->SetCooperativeLevel(s_hInpdevWnd, DISCL_NONEXCLUSIVE| DISCL_FOREGROUND	)))	return ERRMSG(hr);
	s_lpDIDMouse->Acquire();

	return TRUE;
}

//
//	인풋 디바이스 날림~
void
cINPDEV::Destroy(void)
{
	if (s_lpDIDKeyboard	)
		s_lpDIDKeyboard->Unacquire();
	if (s_lpDIDMouse	)
		s_lpDIDMouse->Unacquire();

	RELEASE(s_lpDIDKeyboard);
	RELEASE(s_lpDIDMouse);
	RELEASE(s_lpDI);
}

void
cINPDEV::Reset()
{
	s_isKbhit					=	FALSE;		//	뭔가 눌렀냐?
	s_isMovedMouse				=	FALSE;		//	마우스가 움직였냐?
	s_isLeftButtonPressed		=	FALSE;		//	마우스 왼쪽 버튼이 눌러진 상태?
	s_isRightButtonPressed		=	FALSE;		//	마우스 오른쪽 버튼이 눌러진 상태?
	s_isTouchLeftButton			=	FALSE;		//	마우스 왼쪽 버튼 눌렀다가 놓았가.
	s_isTouchRightButton		=	FALSE;		//	마우스 오른쪽 버튼 눌렀다가 떼었다.
	s_isClickedLeftButton		=	FALSE;
	s_isClickedRightButton		=	FALSE;
	s_isDoubleClickLeftButton	=	FALSE;		//	마우스 왼쪽 버튼이 더블클릭?
	s_isDoubleClickRightButton	=	FALSE;		//	마우스 오른쪽 버튼이 더블클릭?
	memset(s_aTouched,0,sizeof(s_aTouched));	//	터치 버튼 초기화
	memset(s_aClicked,0,sizeof(s_aTouched));	//	터치 버튼 초기화

	ZeroMemory( s_aKey, sizeof(s_aKey) );
}

//
//	업데이트 하자
void 
cINPDEV::Update(void)
{
	BOOL	bIsNotAvtiveDI	=	FALSE;

	if (!s_lpDIDKeyboard )
		bIsNotAvtiveDI	=	TRUE;
	if (!s_lpDIDMouse )
		bIsNotAvtiveDI	=	TRUE;

	HRESULT			hr;
    DIMOUSESTATE2	dims2;      // DirectInput mouse state structure

	//	값들 초기화
	Reset();

	if (bIsNotAvtiveDI)
	{
		GetKeyboardState(s_aKey);

		for (int i=0;i<256;i++)	
		{
			if	(s_aKey[i] & 0x80)
				s_aKey[i]	=	1;
			else
				s_aKey[i]	=	0;
		}
	}
	else
	{
		//	키보드 상태 얻어옴
		if( FAILED(hr=s_lpDIDKeyboard->GetDeviceState(sizeof(s_aKey), &s_aKey)) ) 
		{
			hr = s_lpDIDKeyboard->Acquire();

			while( hr == DIERR_INPUTLOST )
				hr = s_lpDIDKeyboard->Acquire();

			return; 
		}
	}

	for (int i=0;i<256;i++)
	{	
		if (s_aKey[i])
		{
			s_isKbhit	=	TRUE;

			if	(!s_aOldKey[i])
				s_aTouched[i]	=	TRUE;
		}
		else
		{
			if (s_aOldKey[i])
				s_aClicked[i]	=	TRUE;
		}
	}

	memcpy(s_aOldKey,s_aKey,sizeof(s_aKey));

	//	마우스 상태 얻어옴
	if	(!bIsNotAvtiveDI)
	{
		ZeroMemory( &dims2, sizeof(dims2) );

		if( FAILED(hr = s_lpDIDMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 )) ) 
		{
			hr = s_lpDIDMouse->Acquire();

			while( hr == DIERR_INPUTLOST )
				hr = s_lpDIDMouse->Acquire();

			return;
		}

		if	(dims2.lX!=	0 || dims2.lY != 0 || dims2.lZ != 0)
			s_isMovedMouse	=	TRUE;
	}

	s_iWheelValue		=	s_iRareWheelValue;
	s_iRareWheelValue	=	0;

	s_posMouse.SetMousePos(s_hInpdevWnd);

	if	(s_posMouse.x < s_rectActiveArea.x1)
		s_posMouse.x = s_rectActiveArea.x1;

	if	(s_posMouse.x > s_rectActiveArea.x2)
		s_posMouse.x = s_rectActiveArea.x2;

	if	(s_posMouse.y < s_rectActiveArea.y1)
		s_posMouse.y = s_rectActiveArea.y1;

	if	(s_posMouse.y > s_rectActiveArea.y2)
		s_posMouse.y = s_rectActiveArea.y2;

	if	(bIsNotAvtiveDI)
	{
		s_isLeftButtonPressed	=	s_aKey[1];
		s_isRightButtonPressed	=	s_aKey[2];
	}
	else
	{
		if	(dims2.rgbButtons[0] & 0x80)	
			s_isLeftButtonPressed	=	TRUE,s_isKbhit	=	TRUE;
		if	(dims2.rgbButtons[1] & 0x80)
			s_isRightButtonPressed	=	TRUE,s_isKbhit	=	TRUE;
	}

	if	(s_isLeftButtonPressed)
	{
		if	(s_bIsWaitForReleaseLeftButton)
		{
			s_isOldLB				=	FALSE;
			s_isLeftButtonPressed	=	FALSE;
			return;
		}
		else
		if	(!s_isOldLB)
		{
			s_posOld.Set(s_posMouse.x,s_posMouse.y);
			s_isTouchLeftButton			=	TRUE;
			s_bIsReadyToDrag			=	TRUE;
		}

		if	(s_bIsDragMouse	==	FALSE	&&	s_bIsReadyToDrag	&&	s_isMovedMouse)
		{
			s_posMouse.x				=	s_posOld.x;
			s_posMouse.y				=	s_posOld.y;
			s_bIsDragMouse				=	TRUE;
			s_bIsReadyToDrag			=	FALSE;
		}
	}
	else
	{
		s_bIsWaitForReleaseLeftButton	=	FALSE;
		s_bIsReadyToDrag				=	FALSE;
		s_bIsDragMouse					=	FALSE;
	}

	if	(!s_isOldRB	&& s_isRightButtonPressed	)
	{
		s_posOld.Set(s_posMouse.x,s_posMouse.y);
		s_isTouchRightButton	=	TRUE;
	}
	if	(s_isOldLB	&& !s_isLeftButtonPressed	)
	{
		if	(s_posOld.Match(&s_posMouse))
			s_isClickedLeftButton	=	TRUE;
	}
	if	(s_isOldRB	&& !s_isRightButtonPressed	)
	{
		if	(s_posOld.Match(&s_posMouse))
			s_isClickedRightButton	=	TRUE;
	}

	if	(s_posLastForLeftDblClick.Match(&s_posMouse)	==	FALSE)
		s_dwIdleTime1	=	0;

	if	(s_posLastForRightDblClick.Match(&s_posMouse)==	FALSE)
		s_dwIdleTime2	=	0;

	if	(s_isLeftButtonPressed && !s_isOldLB)
	{
		if	(timeGetTime() < s_dwIdleTime1 + 600 )
		{
			s_isDoubleClickLeftButton	=	TRUE;
			s_dwIdleTime1				=	0;
			s_bIsWaitForReleaseLeftButton=	TRUE;
		}
		else
		{
			s_posLastForLeftDblClick.Set(s_posMouse.x,s_posMouse.y);
			s_dwIdleTime1				=	timeGetTime();
		}
	}

	if	(s_isRightButtonPressed && !s_isOldRB)
	{	
		if	(timeGetTime() < s_dwIdleTime2 + 600 )
		{
			s_isDoubleClickRightButton	=	TRUE;
			s_dwIdleTime2				=	0;
		}
		else
		{
			s_posLastForRightDblClick.Set(s_posMouse.x,s_posMouse.y);
			s_dwIdleTime2				=	timeGetTime();
		}
	}

	s_isOldLB	=	s_isLeftButtonPressed;
	s_isOldRB	=	s_isRightButtonPressed;
}