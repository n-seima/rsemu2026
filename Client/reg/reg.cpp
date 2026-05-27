// reg.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "atlbase.h"

//#define	dBALANCE	1
//#define	dTEST		1
// #define	_FOR_USA
// #define	_FOR_JAPAN_HANGAME
// #define	_FOR_JAPAN
// #define	_FOR_INTER
// #define	_FOR_THAI

#ifdef	_FOR_JAPAN_HANGAME
LPCTSTR			lpszRegistry				=	"software\\L&K Logic Korea\\Red Stone for japan hangame";
LPCTSTR			lpszTestRegistry			=	"software\\L&K Logic Korea\\Red Stone for japan hangame(test)";
LPCTSTR			lpszBalanceRegistry			=	"software\\L&K Logic Korea\\Red Stone for japan hangame(balance)";
LPCTSTR			lpszUninstallRegistry		=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone for japan hangame";
LPCTSTR			lpszUninstallTestRegistry	=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone for japan hangame(test)";
LPCTSTR			lpszUninstallBalanceRegistry=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone for japan hangame(balance)";
#elif	_FOR_JAPAN
LPCTSTR			lpszRegistry				=	"software\\L&K Logic Korea\\Red Stone";
LPCTSTR			lpszTestRegistry			=	"software\\L&K Logic Korea\\Red Stone(test)";
LPCTSTR			lpszBalanceRegistry			=	"software\\L&K Logic Korea\\Red Stone(balance)";
LPCTSTR			lpszUninstallRegistry		=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone";
LPCTSTR			lpszUninstallTestRegistry	=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(test)";
LPCTSTR			lpszUninstallBalanceRegistry=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(balance)";
#elif	_FOR_INTER
LPCTSTR			lpszRegistry				=	"software\\L&K Logic Korea\\Red Stone for Inter";
LPCTSTR			lpszTestRegistry			=	"software\\L&K Logic Korea\\Red Stone(test) for Inter";
LPCTSTR			lpszBalanceRegistry			=	"software\\L&K Logic Korea\\Red Stone(balance) for Inter";
LPCTSTR			lpszUninstallRegistry		=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone for Inter";
LPCTSTR			lpszUninstallTestRegistry	=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(test) for Inter";
LPCTSTR			lpszUninstallBalanceRegistry=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(balance) for Inter";
#elif	_FOR_USA
#define	dENG
LPCTSTR			lpszRegistry				=	"software\\L&K Logic Korea\\Red Stone for USA";
LPCTSTR			lpszTestRegistry			=	"software\\L&K Logic Korea\\Red Stone(test) for USA";
LPCTSTR			lpszBalanceRegistry			=	"software\\L&K Logic Korea\\Red Stone(balance) for USA";
LPCTSTR			lpszUninstallRegistry		=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone for USA";
LPCTSTR			lpszUninstallTestRegistry	=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(test) for USA";
LPCTSTR			lpszUninstallBalanceRegistry=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(balance) for USA";
#elif	_FOR_WCG
LPCTSTR			lpszRegistry				=	"software\\L&K Logic Korea\\Red Stone for WCG";
LPCTSTR			lpszTestRegistry			=	"software\\L&K Logic Korea\\Red Stone(test) for WCG";
LPCTSTR			lpszBalanceRegistry			=	"software\\L&K Logic Korea\\Red Stone(balance) for WCG";
LPCTSTR			lpszUninstallRegistry		=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone for WCG";
LPCTSTR			lpszUninstallTestRegistry	=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(test) for WCG";
LPCTSTR			lpszUninstallBalanceRegistry=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(balance) for WCG";
#elif	_FOR_THAI
LPCTSTR			lpszRegistry				=	"software\\L&K Logic Korea\\Red Stone for Thai";
LPCTSTR			lpszTestRegistry			=	"software\\L&K Logic Korea\\Red Stone(test) for Thai";
LPCTSTR			lpszBalanceRegistry			=	"software\\L&K Logic Korea\\Red Stone(balance) for Thai";
LPCTSTR			lpszUninstallRegistry		=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone for Thai";
LPCTSTR			lpszUninstallTestRegistry	=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(test) for Thai";
LPCTSTR			lpszUninstallBalanceRegistry=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(balance) for Thai";
#else
LPCTSTR			lpszRegistry				=	"software\\L&K Logic Korea\\Red Stone";
LPCTSTR			lpszTestRegistry			=	"software\\L&K Logic Korea\\Red Stone(test)";
LPCTSTR			lpszBalanceRegistry			=	"software\\L&K Logic Korea\\Red Stone(balance)";
LPCTSTR			lpszUninstallRegistry		=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone";
LPCTSTR			lpszUninstallTestRegistry	=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(test)";
LPCTSTR			lpszUninstallBalanceRegistry=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(balance)";
#endif

#ifdef	dENG
char	*lpstrTitle			=	"Change Red Stone Client Folder";
char	*lpstrTitleTest		=	"Change Red Stone Test Client Folder";
char	*lpstrChangeConfirm	=	"Check this folder\n\n%s"
char	*lpstrComplete		=	"Work Complete";
#else
char	*lpstrTitle			=	"붉은 보석 실행 폴더 변경";
char	*lpstrTitleTest		=	"테스트용 붉은 보석 실행 폴더 변경";
char	*lpstrChangeConfirm	=	"현재 폴더를 붉은 보석 레지스트리에 등록 하시겠습니까?\n\n%s";
char	*lpstrComplete		=	"현재 폴더를 붉은 보석 레지스트리에 등록 했습니다.";
char	*lpstrDownVersion	=	"현재 폴더를 붉은 보석 레지스트리에 등록 했습니다.";
#endif

int main(int argc, char* argv[])
{

#ifdef	dTEST
	lpszRegistry	=	lpszTestRegistry;
	lpstrTitle		=	lpstrTitleTest;
#elif	dBALANCE
	lpszRegistry	=	lpszBalanceRegistry;
	lpstrTitle		=	"기획팀 테스트용 붉은 보석 실행 폴더 변경";
#endif

	int	iVersion	=	-1;

	char		strFolder[MAX_PATH],strMsg[256];

	GetCurrentDirectory(MAX_PATH,strFolder);

	sprintf(strMsg,lpstrChangeConfirm,strFolder);

	if	(MessageBox(NULL,strMsg,lpstrTitle,MB_YESNO)	==	IDNO)
		return	0;

	CRegKey		reg;

	if	(reg.Create(HKEY_CURRENT_USER,lpszRegistry)==ERROR_SUCCESS)
	{
		reg.SetValue(strFolder,"path");
	}

	MessageBox(NULL,strFolder,lpstrComplete,MB_OK);

	return 0;
}