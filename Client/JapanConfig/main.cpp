#include "stdafx.H"
#include "main.H"
#include <stdio.h>
#include <SHLOBJ.h>
#include <atlbase.h>
#include "message.H"
LPCTSTR			lpszAppName		=	"INSTALLDDAKZI";
char			*g_strLogFile	=	"uninstall.dat";

cMain			MAIN;

char			g_strInstallFolder[1024];
char			g_strExcuteFileName[1024];
int				g_iSerial;
char			g_strCDKey[17];
BOOL			g_bLaunch	=	FALSE;

int				l_iRequireMinVersion=	211;
//int				l_iRequireMinVersion=	242;
int				g_iVersion			=	252;
BOOL			l_bIsTemplatePatch	=	FALSE;

LPCTSTR			lpszRegistry	=	"software\\L&K Logic Korea\\Red Stone";

////////////////////////////////////////////////////////////////////////////////
//									초기화 클래스..
////////////////////////////////////////////////////////////////////////////////

cMain::cMain()
{
}

cMain::~cMain()
{
}


BOOL
cMain::init(HINSTANCE hInst)
{
	if	(IsFile("data/japanHangame.dat"))
		lpszRegistry							=	"software\\L&K Logic Korea\\Red Stone for japan hangame";

	DialogBox(hInst,"CONFIG",NULL,(DLGPROC)s_procDialog);

	return	FALSE;
}

LRESULT CALLBACK
cMain::s_wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch( uMsg )
	{
		case WM_CREATE		:
			break;

		default :
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return( 0L );
}

BOOL
cMain::initDlg(HWND hDlg)
{
/*	CRegKey	reg;

	if (reg.Open( HKEY_LOCAL_MACHINE, "software\\L&K Logic Korea\\Red Stone") == ERROR_SUCCESS)
	{	DWORD	count	=	sizeof(g_strLogFolder);
		if (reg.QueryValue((LPTSTR )g_strLogFolder,"Path",&count) == ERROR_SUCCESS)
		{	if (count	<=	0)
				return ERRMSG(dM_ERROR,dM_CAN_NOT_FIND_REGISTRY);
		}
		else	return ERRMSG(dM_ERROR,dM_CAN_NOT_FIND_REGISTRY);
	}
	else	return ERRMSG(dM_ERROR,dM_CAN_NOT_FIND_REGISTRY);

	if (!SetLogFolder())
		return ERRMSG(dM_ERROR,dMF_CAN_NOT_FIND_FOLLOW_FOLDER,g_strLogFolder);
*/
	m_pFileData	=	NULL;

	cFILE	file;

	if (file.Open("config.cfg","rb"))
	{
		m_dwFileSize	=	file.Length;
		m_pFileData		=	new char [m_dwFileSize];

		file.Read(m_pFileData,m_dwFileSize);
		memcpy(&m_option,m_pFileData,sizeof(m_option));

		if (m_option.m_iOutputDevice > eCD_DIB || m_option.m_iOutputDevice < 0)
			m_option.m_iOutputDevice	=	eCD_DIRECTDRAW;

		if	(m_option.m_dwConfigVersion	<	eCFV_ADD_WINDOW_MODE)
			m_option.m_bf1IsWindowMode	=	FALSE;

		file.Close();
	}
	else
	{
		m_dwFileSize	=	sizeof(m_option);
		m_pFileData		=	new char [sizeof(m_option)];
		memcpy(m_pFileData,&m_option,sizeof(m_option));
	}


	m_bIsVirgin	=	TRUE;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog

	switch(m_option.m_iOutputDevice)
	{
		case	eCD_DIRECTDRAW	:
			CheckDlgButton(hDlg,IDC_USE_DIRECT_DRAW,TRUE);
			break;
		case	eCD_AGP			:
			CheckDlgButton(hDlg,IDC_USE_AGP,TRUE);
			break;
		case	eCD_DIB			:
			CheckDlgButton(hDlg,IDC_USE_DIB,TRUE);
			break;
	}

	if (m_option.m_bIsMusicOn		)
		CheckDlgButton(hDlg,IDC_MUSIC_ON,TRUE);
	if (m_option.m_bIsSoundOn		)
		CheckDlgButton(hDlg,IDC_SOUND_ON,TRUE);
	if (m_option.m_wIsExclusiveMode	)
		CheckDlgButton(hDlg,IDC_EXCLUSIVE_MODE,TRUE);
	if	(m_option.m_bf1IsWindowMode)
		CheckDlgButton(hDlg,IDC_WINDOW_MODE,TRUE);
	if	(m_option.m_bf1IsUse1024X768	)
		CheckDlgButton(hDlg,IDC_1024X768,TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void
cMain::complete(HWND _hDlg)
{
	if (IsDlgButtonChecked(_hDlg,IDC_USE_DIRECT_DRAW))
		m_option.m_iOutputDevice	=	eCD_DIRECTDRAW;
	else
	if (IsDlgButtonChecked(_hDlg,IDC_USE_AGP))
		m_option.m_iOutputDevice	=	eCD_AGP;
	else
	if (IsDlgButtonChecked(_hDlg,IDC_USE_DIB))
		m_option.m_iOutputDevice	=	eCD_DIB;

	if	(IsDlgButtonChecked(_hDlg,IDC_WINDOW_MODE))
		m_option.m_bf1IsWindowMode	=	TRUE;
	else
		m_option.m_bf1IsWindowMode	=	FALSE;

	m_option.m_bIsMusicOn		=	IsDlgButtonChecked(_hDlg,IDC_MUSIC_ON);
	m_option.m_bIsSoundOn		=	IsDlgButtonChecked(_hDlg,IDC_SOUND_ON);
	m_option.m_wIsExclusiveMode	=	IsDlgButtonChecked(_hDlg,IDC_EXCLUSIVE_MODE);
	m_option.m_dwConfigVersion	=	eCFV_CURRENT_VERSION-1;
	m_option.m_bf1IsUse1024X768	=	IsDlgButtonChecked(_hDlg,IDC_1024X768);

	memcpy(m_pFileData,&m_option,sizeof(m_option));

	SetLogFolder();

	cFILE	file;

	file.Open("config.cfg","wb");

	file.Write(m_pFileData,m_dwFileSize);

	file.Close();

}

LRESULT CALLBACK
cMain::s_procDialog(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case	WM_INITDIALOG	:
			MAIN.initDlg(hDlg);
			break;

		case	WM_COMMAND		:
			if ( HIWORD( wParam ) == EN_CHANGE ) break;
			switch( LOWORD( wParam ) )
            {
				case	IDOK				:
				{
					MAIN.complete(hDlg);
					EndDialog(hDlg,IDOK);
					break;
				}
				case	IDCANCEL				:
				{
					EndDialog(hDlg,IDCANCEL);
					break;
				}
			}
			break;
	}

	return (0L);
}
