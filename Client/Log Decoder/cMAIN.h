#ifndef _classMAIN_H
#define _classMAIN_H

#include "SFC.H"
#include "cWINDOW.H"
#include "resource.h"

#define	dMAX_MAP_COUNT_IN_SERVER	100
#define	dMAX_SERVER_COUNT			100

#define	dNAME_LENGTH				18
#define	dID_LENGTH					20
#define	dCHAT_LENGTH				128
#define	dMAX_LOG_COUNT				1024

class	CChatMessageOld20031025
{
public:
	char		m_strName[dNAME_LENGTH];
	char		m_strMessage[dCHAT_LENGTH];
};
class	CChatMessage : public CChatMessageOld20031025
{
public:
	WORD		m_wChatType;

	DWORD		mix();
	DWORD		decode();
};

class cMAIN
{
public:
	static	char				s_strDataFolder[1024];
	static	cWND				*pWND;
	static	cDIBWND				*pDIBWND;
	static	HWND				hWND;
	static	HINSTANCE			hINST;
	static	char				s_strId[dID_LENGTH],s_strName[dNAME_LENGTH],s_strLastServerName[dNAME_LENGTH];
	static	int					s_iLogCount;
	static	CChatMessage		s_aLog[dMAX_LOG_COUNT];
	static	char				s_strFname[512];

	static	BOOL				Init(HINSTANCE hInst);
	static	BOOL				InitDlg(HWND _hWnd);
	static	BOOL				ReadData();
	static	BOOL				SaveData();
	static	void				FillData(HWND _hDlg);
	static	BOOL				Exit();
	static	BOOL				Run();
	static	void				Load(HWND _hDlg);
	static	void				Save(HWND _hDlg);

	static	LRESULT CALLBACK	DlgProc(HWND,UINT,WPARAM,LPARAM);

};

extern	BOOL	bRUNAPPLICATION,bACTIVEAPP;

//
//	Loged Chat Message Type
enum
{
	eLCMT_CHAT,
	eLCMT_SHOUT,
	eLCMT_SEND_WHISPER,
	eLCMT_RECEIVE_WHISPER,
	eLCMT_GUILD,
	eLCMT_PARTY,
};

#endif