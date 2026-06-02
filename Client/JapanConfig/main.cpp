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

struct	cResolutionOption
{
	int			m_iMode;
	const char	*m_lpstrText;
};

cResolutionOption	l_aResolutionOption[] =
{
	{	eGAME_RESOLUTION_800X600,	"800 x 600"		},
	{	eGAME_RESOLUTION_1024X768,	"1024 x 768"	},
	{	eGAME_RESOLUTION_1280X720,	"1280 x 720"	},
	{	eGAME_RESOLUTION_1280X768,	"1280 x 768"	},
	{	eGAME_RESOLUTION_1366X768,	"1366 x 768"	},
	{	eGAME_RESOLUTION_1200X1005,	"1200 x 1005"	},
};

int				l_iResolutionOptionCount	=	sizeof(l_aResolutionOption)/sizeof(l_aResolutionOption[0]);

int
GetResolutionOptionIndex(int _iResolutionMode)
{
	for	(int i=0;i<l_iResolutionOptionCount;i++)
	{
		if	(l_aResolutionOption[i].m_iMode	==	_iResolutionMode)
			return	i;
	}

	return	0;
}

BOOL
GetResolutionSizeFromText(char *_lpstrText,int &_iWidth,int &_iHeight)
{
	int	aiValue[2]	=	{0,0};
	int	iIndex		=	0;
	char	*lpText	=	_lpstrText;

	while(*lpText && iIndex < 2)
	{
		while(*lpText && (*lpText < '0' || *lpText > '9'))
			lpText++;

		if	(!*lpText)
			break;

		while(*lpText >= '0' && *lpText <= '9')
		{
			aiValue[iIndex]	=	aiValue[iIndex]*10+(*lpText-'0');
			lpText++;
		}

		iIndex++;
	}

	if	(iIndex	<	2)
		return	FALSE;

	_iWidth		=	aiValue[0];
	_iHeight	=	aiValue[1];

	return	TRUE;
}

void
SetResolutionComboText(HWND _hResolution,const CGameOption &_option)
{
	int		iWidth,iHeight;
	char	strResolution[64];

	GetGameResolutionSize(_option,iWidth,iHeight);
	wsprintf(strResolution,"%d x %d",iWidth,iHeight);

	SendMessage(_hResolution,CB_SETCURSEL,GetResolutionOptionIndex(GetGameResolutionMode(_option)),0);
	SetWindowText(_hResolution,strResolution);
}

////////////////////////////////////////////////////////////////////////////////
//									초기화 클래스..
////////////////////////////////////////////////////////////////////////////////

cMain::cMain()
{
	m_pFileData		=	NULL;
	m_dwFileSize	=	0;
}

cMain::~cMain()
{
	if	(m_pFileData)
		delete [] m_pFileData;
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

	{
		CGameOption	defaultOption;
		m_option	=	defaultOption;
	}

	if (file.Open("config.cfg","rb"))
	{
		m_dwFileSize	=	file.Length;
		m_pFileData		=	new char [m_dwFileSize];

		file.Read(m_pFileData,m_dwFileSize);
		{
			DWORD	dwCopySize	=	m_dwFileSize < sizeof(m_option) ? m_dwFileSize : sizeof(m_option);
			if	(dwCopySize	>	0)
				memcpy(&m_option,m_pFileData,dwCopySize);
		}

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

	if	(m_option.m_dwConfigVersion < eCFV_ADD_RESOLUTION_MODE)
		SetGameResolutionMode(m_option,m_option.m_bf1IsUse1024X768 ? eGAME_RESOLUTION_1024X768 : eGAME_RESOLUTION_800X600);
	else
	if	(m_option.m_dwConfigVersion < eCFV_ADD_VARIABLE_RESOLUTION)
		SetGameResolutionMode(m_option,GetGameResolutionMode(m_option));
	else
		SetGameResolutionSize(m_option,m_option.m_wScreenWidth,m_option.m_wScreenHeight);


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

	HWND	hResolution	=	GetDlgItem(hDlg,IDC_RESOLUTION);
	for	(int i=0;i<l_iResolutionOptionCount;i++)
		SendMessage(hResolution,CB_ADDSTRING,0,(LPARAM)l_aResolutionOption[i].m_lpstrText);
	SetResolutionComboText(hResolution,m_option);

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
	{
		char	strResolution[64];
		int		iWidth,iHeight;

		GetDlgItemText(_hDlg,IDC_RESOLUTION,strResolution,sizeof(strResolution));

		if	(!GetResolutionSizeFromText(strResolution,iWidth,iHeight))
			GetGameResolutionSize(m_option,iWidth,iHeight);

		SetGameResolutionSize(m_option,iWidth,iHeight);
	}

	if	(m_pFileData)
		delete [] m_pFileData;

	m_dwFileSize	=	sizeof(m_option);
	m_pFileData		=	new char [m_dwFileSize];
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
