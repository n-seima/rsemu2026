#include "cMAIN.H"
#include "CGamePlay.H"
#include "devCode.H"

#ifdef _USE_XTRAP
#include "XTrap/Xtrap_C_Interface.h"
#endif

#ifdef	_DEBUG
#define _CRTDBG_MAP_ALLOC
#endif
#include	<stdlib.h>
#include	<crtdbg.h>

class	cCheckMemoryLeak
{
public:
	cCheckMemoryLeak(int _iIndex=-1)
	{
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 

		if	(_iIndex	!=	-1)
			_CrtSetBreakAlloc(_iIndex);
	}
};

WINAPI
WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, int nCmdShow)
{
	const	int	c_iCheckPoint	=	-1;

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 

	if	(c_iCheckPoint	!=	-1)
	{
		cMSG::Put("경고!","메모리 릭 체크 포인트가 존재합니다");
		_CrtSetBreakAlloc(c_iCheckPoint);
	}

	MSG msg;

	if	(!cMAIN::Init(hInst))
	{
		return FALSE;
	}

//VIRTUALIZER_START
	while(1)
	{
		if	(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if	(!GetMessage(&msg, NULL, 0, 0 )) 
				return msg.wParam;

			TranslateMessage(&msg); 
			DispatchMessage(&msg);

			continue;
		}

		if	(!cMAIN::Run()) 
		{
			break;
		}

	}

//VIRTUALIZER_END	

	cMAIN::Exit();

	return( msg.wParam ); 
}
