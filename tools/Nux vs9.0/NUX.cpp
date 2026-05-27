#include <windows.h>

#include "cMAIN.H"

int PASCAL 
WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPTSTR lpCmdLine, int nCmdShow)
{	MSG msg;

	if (!cMAIN::Init(hInstance))	return FALSE;

	while(1)
	{	Sleep(1);

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{	if (IsWindow(cMAIN::hPROGRESS))
				if (IsDialogMessage(cMAIN::hPROGRESS,&msg)) continue;

			if( msg.message == WM_QUIT) break;
			else
			{	TranslateMessage(&msg); 
				DispatchMessage(&msg);
				continue;
			}
		}

		if (!cMAIN::Run()) break;
	}

	return( msg.wParam ); 
}