#include <stdio.h>
#include "stdafx.h"

#include "cMAIN.H"
#include "cMESSAGE.H"

#ifdef	_FOR_INTERNATIONAL
LPCTSTR			lpszRegistry				=	"software\\L&K Logic Korea\\Red Stone for International";
LPCTSTR			lpszTestRegistry			=	"software\\L&K Logic Korea\\Red Stone(test) for International";
LPCTSTR			lpszBalanceRegistry			=	"software\\L&K Logic Korea\\Red Stone(balance) for International";
LPCTSTR			lpszUninstallRegistry		=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone for International";
LPCTSTR			lpszUninstallTestRegistry	=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(test) for International";
LPCTSTR			lpszUninstallBalanceRegistry=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(balance) for International";
#elif	_FOR_USA
LPCTSTR			lpszRegistry				=	"software\\L&K Logic Korea\\Red Stone for USA";
LPCTSTR			lpszTestRegistry			=	"software\\L&K Logic Korea\\Red Stone(test) for USA";
LPCTSTR			lpszBalanceRegistry			=	"software\\L&K Logic Korea\\Red Stone(balance) for USA";
LPCTSTR			lpszUninstallRegistry		=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone for USA";
LPCTSTR			lpszUninstallTestRegistry	=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(test) for USA";
LPCTSTR			lpszUninstallBalanceRegistry=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(balance) for USA";
#else
LPCTSTR			lpszRegistry				=	"software\\L&K Logic Korea\\Red Stone";
LPCTSTR			lpszTestRegistry			=	"software\\L&K Logic Korea\\Red Stone(test)";
LPCTSTR			lpszBalanceRegistry			=	"software\\L&K Logic Korea\\Red Stone(balance)";
LPCTSTR			lpszUninstallRegistry		=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone";
LPCTSTR			lpszUninstallTestRegistry	=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(test)";
LPCTSTR			lpszUninstallBalanceRegistry=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(balance)";
#endif

static char	*lpszAppName			=	"SERVERINFO";
static char	*lpszTitle				=	"AccuseTool";
char				cMAIN::s_strDataFolder[1024]	=	"";

cWND				*cMAIN::pWND			=	NULL;
cDIBWND				*cMAIN::pDIBWND			=	NULL;
HWND				cMAIN::hWND				=	NULL;
HINSTANCE			cMAIN::hINST;
char				cMAIN::s_strId[dID_LENGTH];
char				cMAIN::s_strName[dNAME_LENGTH];
char				cMAIN::s_strLastServerName[dNAME_LENGTH];
char				cMAIN::s_strFname[512];

int					cMAIN::s_iLogCount		=	0;
CChatMessage		cMAIN::s_aLog[dMAX_LOG_COUNT];

BOOL				bRUNAPPLICATION	=	TRUE,bACTIVEAPP	=	FALSE;

DWORD
CChatMessage::decode()
{
	int		i;
	DWORD	dwCheckSum	=	0;

	for (i=0;i<dNAME_LENGTH;i++)
	{
		dwCheckSum	+=	m_strName[i];
		m_strName[i]-=	random(100);
	}

	for (i=0;i<dCHAT_LENGTH;i++)
	{
		dwCheckSum		+=	m_strMessage[i];
		m_strMessage[i]	-=	random(100);
	}

	return	dwCheckSum;
}	//	CChatMessage::mix()

BOOL
cMAIN::Init(HINSTANCE hInst)
{
	CRegKey		reg;

	int	iResult	=	reg.Open( HKEY_CURRENT_USER, lpszRegistry);
	
	if	(iResult	!= ERROR_SUCCESS)
		iResult	=	reg.Open( HKEY_LOCAL_MACHINE, lpszRegistry);

	if	(iResult == ERROR_SUCCESS)
	{	
		DWORD	count	=	sizeof(_logFolder);
		if (reg.QueryValue((LPTSTR )_logFolder,"Path",&count) == ERROR_SUCCESS)
		{
			if (count	<=	0)
				return cMSG::Error("Error in cMAIN::Init","can not find registry data of Red Stone");
		}
		else
			return cMSG::Error("Error in cMAIN::Init","can not find registry data of Red Stone");
	}
	else
		return cMSG::Error("Error in cMAIN::Init","can not find registry data of Red Stone");

	hINST	=	hInst;

	ReadData();
#ifdef	_FOR_KOREA
	DialogBox(hInst,"MAIN",NULL,(DLGPROC)DlgProc);
#else
	DialogBox(hInst,"MAIN_E",NULL,(DLGPROC)DlgProc);
#endif

	return FALSE;
}

//
//	창에 데이터를 채워준다.
void
cMAIN::FillData(HWND _hWnd)
{
}	//	cMAIN::FillData(HWND _hWnd)

BOOL
cMAIN::InitDlg(HWND _hWnd)
{

	return	TRUE;
}

BOOL
cMAIN::ReadData()
{
	GetCurrentDirectory(sizeof(_logFolder),_logFolder);

	cFILE	file;

	if	(!file.Open("logdecoderconfig.cfg","rb"))
	{
		GetCurrentDirectory(sizeof(s_strDataFolder),s_strDataFolder);
		return FALSE;
	}

	file.Read(s_strDataFolder,sizeof(s_strDataFolder));

	file.Close();

	return TRUE;
}

BOOL
cMAIN::SaveData()
{
	SetCurrentDirectory(_logFolder);
	
	cFILE	file;

	file.Open("logdecoderconfig.cfg","wb");

	file.Write(s_strDataFolder,sizeof(s_strDataFolder));

	file.Close();

	return TRUE;
}	//	cMAIN::SaveData()

//
//	종료
BOOL
cMAIN::Exit()
{	
	if	(!bRUNAPPLICATION)
		return FALSE;

	SaveData();
	
	bRUNAPPLICATION	=	FALSE;

	PostQuitMessage(0);

	return TRUE;
}	//	cMAIN::Exit()

BOOL
cMAIN::Run()			//	윈도우가 활성화 중일때 계속 업데이트 시켜준다.
{	
	if	(!bRUNAPPLICATION)
		return FALSE;

	return TRUE;
}

/*****************************************************************************
								풀어주기
*****************************************************************************/

//
//	리사이즈
void
cMAIN::Load(HWND _hDlg)
{
	strcpy(s_strFname,"");

	if (!cFILE::LoadBox(s_strFname,"log file {*.log}\0*.log\0\0","log",s_strDataFolder))	return;

	CChatMessage	message;
	DWORD			dwCheckSum	=	0,dwSaveCheckSum,dwSeedValue;
	cFILE			file;
	int				iInfoSize	=	dID_LENGTH+dNAME_LENGTH+dNAME_LENGTH+8+4*6;

	if (!file.Open(s_strFname,"rb"))	return;

	int				iCount		=	(file.Length-iInfoSize)/sizeof(CChatMessage),i;
	BOOL			bIsExistProcessFile	=	FALSE;
	int				iYear,iMonth,iDay,iHour,iMin,iSec;
	char			saveFileName[512];

	strcpy(saveFileName,s_strFname);

	_setExt(saveFileName,"txt");

	if (IsFile(saveFileName))	bIsExistProcessFile	=	TRUE;

	DWORD	dwCheckCode,dwFilePos;
	int		iFileVersion	=	0;

	file.Read(&dwCheckCode,4);

	if (dwCheckCode	==	0x12345678)
	{
		file.Read(&iFileVersion,4);
		file.Read(&iYear,4);
	}
	else	iYear	=	dwCheckCode;

	if (iFileVersion	<=	0)
	{
		iCount		=	(file.Length-iInfoSize)/(sizeof(CChatMessage)-2);
	}

	file.Read(&iMonth,4);
	file.Read(&iDay,4);
	file.Read(&iHour,4);
	file.Read(&iMin,4);
	file.Read(&iSec,4);

	dwFilePos	=	file.GetPos();

	file.Seek(-(dID_LENGTH+dNAME_LENGTH+dNAME_LENGTH+8),SEEK_END);
	file.Read(s_strId,dID_LENGTH);
	file.Read(s_strName,dNAME_LENGTH);
	file.Read(s_strLastServerName,dNAME_LENGTH);
	file.Read(&dwSaveCheckSum,4);
	file.Read(&dwSeedValue,4);
	file.Seek(dwFilePos,SEEK_SET);

	srand(dwSeedValue);
	for (i=0;i<iSec+iMin+iHour+iMonth+iDay;i++)	random(100);

	s_iLogCount		=	iCount;

	cSTRING		string;

	string.Add("   Server : [%s]%c%c",s_strLastServerName,0x0d,0x0a);
	string.Add("       ID : [%s]%c%c",s_strId,0x0d,0x0a);
	string.Add("Character : [%s]%c%c",s_strName,0x0d,0x0a);
	string.Add("     Time : [%.4d/%.2d/%.2d %.2d:%.2d:%.2d]%c%c",iYear,iMonth,iDay,iHour,iMin,iSec,0x0d,0x0a);

	string.Add("---------------------------------------------------------------------------------------------------------------%c%c%c%c",0x0d,0x0a,0x0d,0x0a);

	for (i=0;i<iCount;i++)
	{
		if (iFileVersion	<=	0)
		{
			file.Read(&message,sizeof(CChatMessageOld20031025));
			message.m_wChatType	=	eLCMT_CHAT;
		}
		else	file.Read(&message,sizeof(message));

		dwCheckSum	+=	message.decode();

		strcpy(s_aLog[i].m_strName		,message.m_strName);
		strcpy(s_aLog[i].m_strMessage	,message.m_strMessage);

		//
//	Loged Chat Message Type
#ifdef	_FOR_KOREA
		switch(message.m_wChatType)
		{
			case	eLCMT_CHAT				:
				if (stricmp(message.m_strName,s_strName)==0)
						string.Add(   "%20s (◆ ) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				else	string.Add(   "%20s (   ) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				break;
			case	eLCMT_SHOUT				:
				if (stricmp(message.m_strName,s_strName)==0)
						string.Add("%20s (◆!) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				else	string.Add("%20s (  !) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				break;
			case	eLCMT_SEND_WHISPER		:
				string.Add("%20s (◆W) to %s> %s%c%c",s_strName,message.m_strName,message.m_strMessage,0x0d,0x0a);
				break;
			case	eLCMT_RECEIVE_WHISPER	:
				string.Add("%20s (  W) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				break;
			case	eLCMT_GUILD				:
				if (stricmp(message.m_strName,s_strName)==0)
						string.Add("%20s (◆G) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				else	string.Add("%20s (  G) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				break;
			case	eLCMT_PARTY				:
				if (stricmp(message.m_strName,s_strName)==0)
						string.Add("%20s (◆P) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				else	string.Add("%20s (  P) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				break;
		}
#else
		switch(message.m_wChatType)
		{
			case	eLCMT_CHAT				:
				if (stricmp(message.m_strName,s_strName)==0)
						string.Add(   "%20s (* ) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				else	string.Add(   "%20s (  ) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				break;
			case	eLCMT_SHOUT				:
				if (stricmp(message.m_strName,s_strName)==0)
						string.Add("%20s (*!) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				else	string.Add("%20s ( !) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				break;
			case	eLCMT_SEND_WHISPER		:
				string.Add("%20s (*W) to %s> %s%c%c",s_strName,message.m_strName,message.m_strMessage,0x0d,0x0a);
				break;
			case	eLCMT_RECEIVE_WHISPER	:
				string.Add("%20s ( W) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				break;
			case	eLCMT_GUILD				:
				if (stricmp(message.m_strName,s_strName)==0)
						string.Add("%20s (*G) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				else	string.Add("%20s ( G) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				break;
			case	eLCMT_PARTY				:
				if (stricmp(message.m_strName,s_strName)==0)
						string.Add("%20s (*P) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				else	string.Add("%20s ( P) %s%c%c",message.m_strName,message.m_strMessage,0x0d,0x0a);
				break;
		}
#endif
	}

	for (i=0;i<dID_LENGTH;i++)	dwCheckSum	+=	s_strId[i];
	for (i=0;i<dNAME_LENGTH;i++)dwCheckSum	+=	s_strName[i];
	for (i=0;i<dNAME_LENGTH;i++)dwCheckSum	+=	s_strLastServerName[i];

	if (dwCheckSum	!=	dwSaveCheckSum)
		cMSG::Error("error!!","it's damaged file!",s_strFname);

	SetDlgItemText(_hDlg,IDC_EDIT,string.String);
	SetDlgItemText(_hDlg,IDC_EDIT2,"");

	if (bIsExistProcessFile)	ShellExecute(NULL,"open",saveFileName,NULL,NULL,SW_SHOWNORMAL);

	SetWindowText(_hDlg,s_strFname);

	GetCurrentDirectory(sizeof(s_strDataFolder),s_strDataFolder);
}	//	cMAIN::Resize()

void
cMAIN::Save(HWND _hDlg)
{
	char	strText[1024*100];
	GetDlgItemText(_hDlg,IDC_EDIT2,strText,1024*100);

	if (stricmp(strText,"")==0)
	{
		cMSG::Error("error!!","valid 'result'");
		return;
	}

	cFILE	file;
	char	saveFileName[512];

	strcpy(saveFileName,s_strFname);

	_setExt(saveFileName,"txt");

	file.Open(saveFileName,"wb");

	fprintf(file.fp,">>>>>>>>>> result <<<<<<<<<<%c%c",0x0d,0x0a);
	file.Write(strText,strlen(strText));
	fprintf(file.fp,"%c%c%c%c",0x0d,0x0a,0x0d,0x0a);
	fprintf(file.fp,">>>>>>>>>> content <<<<<<<<<<%c%c",0x0d,0x0a);
	GetDlgItemText(_hDlg,IDC_EDIT,strText,1024*100);
	file.Write(strText,strlen(strText));
	file.Close();

	cMSG::Put("complete","save result data follow file\n\n%s",saveFileName);
}	//	cMAIN::Save(HWND _hDlg)

LRESULT CALLBACK
cMAIN::DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
	{	case	WM_INITDIALOG	: 
			InitDlg(hwnd);
			return (TRUE);

		case	WM_CLOSE	:
			Exit();
			break;

		case WM_COMMAND		:
			switch(LOWORD(wParam))
			{
				case	IDOK					:
				{
					Exit();
					break;
				}

				case	IDC_OPEN				:
				{
					Load(hwnd);
					break;
				}

				case	IDC_SAVE				:
				{
					Save(hwnd);
					break;
				}

				default :
					return FALSE;
			}
	}

	return( FALSE );
}