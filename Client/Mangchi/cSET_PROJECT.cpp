#include "cSET.H"
#include "cPROJECT.H"
#include "resource.H"

//
//	초기화
void
cSET_PROJECT::Init(HWND _hWnd)
{
//	DialogBox( s_hInst, "SET_PROJECT",_hWnd,(DLGPROC)s_procDlg);

}	//	cSET_PROJECT::Init(HWND _hWnd)

//
//	데이터를 세팅 한다.
void
cSET_PROJECT::SetData(HWND _hWnd)
{
//	int		i;
}	//	cSET_PROJECT::SetData(HWND _hWnd)

//
//	기본 직업 리스트 얻어 오기
BOOL
cSET_PROJECT::GetData(HWND _hWnd)
{

	return	TRUE;
}	//	cSET_PROJECT::GetData(HWND _hWnd)

//
//	직업 추가
void
cSET_PROJECT::AddJob(HWND _hWnd)
{
}	//	cSET_PROJECT::AddJob(HWND _hWnd)

//
//직업 제거
void
cSET_PROJECT::RemoveJob(HWND _hWnd)
{
}	//	cSET_PROJECT::RemoveJob(HWND _hWnd)


LRESULT CALLBACK
cSET_PROJECT::s_procDlg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{	case WM_INITDIALOG	:
			SetData(hwnd);
			break;

		case WM_CLOSE		:
			EndDialog(hwnd,IDCANCEL);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {
				case	IDOK				:
				{
					if (GetData(hwnd))	EndDialog(hwnd,IDOK);
					break;
				}

				case	IDCANCEL			:
				{
					EndDialog(hwnd,IDCANCEL);
					break;
				}

				case	IDC_ADD				:
				{
					AddJob(hwnd);
					break;
				}

				case	IDC_REMOVE			:
				{
					RemoveJob(hwnd);
					break;
				}

			}
			break;
	}

	return (0L);
}