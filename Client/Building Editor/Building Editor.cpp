#include <windows.h>

#include "cMAIN.H"

int PASCAL 
WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPTSTR lpCmdLine, int nCmdShow)
{	MSG msg;

	if (!_MAIN.Init(hInstance))	return FALSE;

	while(1)   
	{	if (cMAIN::bACTIVEAPP) 	if (!_MAIN.Run()) break;

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{	if( msg.message == WM_QUIT) break;
			else
			{	TranslateMessage(&msg); 
				DispatchMessage(&msg);
			}



		}
	}

	return( msg.wParam ); 
}