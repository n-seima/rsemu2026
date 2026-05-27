#include "cMAIN.H"
#include "crtdbg.H"

int PASCAL 
WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPTSTR lpCmdLine, int nCmdShow)
{	
	const	int	c_iCheckLeakBlock	=	-1;

	if	(c_iCheckLeakBlock	!=	-1)
	{
		cMSG::Put("체크!!","메모리 릭 체크 변수가 설정되어 있습니다");
//		assert(0 && "메모리 릭 체크 변수가 세팅되어 있습니다!!");

		_CrtSetBreakAlloc( c_iCheckLeakBlock );
	}

  	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 

	MSG	msg;
	HACCEL hAccel;				// 09.07.10임시 ..추가
	CoInitialize(NULL);

	if	(!MAIN.Init(hInstance))
		return FALSE;

	hAccel	=	LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MAIN_ACCEL));

	while(1)   
	{
		Sleep(1);

		if	(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if	(!GetMessage(&msg, NULL, 0, 0 )) 
				return msg.wParam;

			if(!TranslateAccelerator(MAIN.hWND, hAccel, &msg)){		//// 09.07.10임시 ..추가.
				
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				
			}

		}

		if (!MAIN.Run())	break;

	}

	CoUninitialize();

	return( msg.wParam ); 
}