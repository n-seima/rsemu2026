#include "SFC.H"
#include "cMAIN.H"

int PASCAL 
WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPTSTR lpCmdLine, int nCmdShow)
{	MSG msg;

	if (!MAIN.Init(hInstance))	return FALSE;

	while(1)   
	{	Sleep(1);

		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{	if(!GetMessage(&msg, NULL, 0, 0 )) return msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}

		if (!MAIN.Run())	break;

	}

	return( msg.wParam ); 
}