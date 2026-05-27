/*
*/
#ifndef _classMAIN_H
#define _classMAIN_H

#include <windows.h>
#include <commctrl.h>

#include "SFC.H"
#include "cWINDOW.H"
#include "cWORKWND.H"
#include "cFRMWND.H"
#include "cIMGWND.H"
#include "cBMPWND.H"
#include "cANM.H"
#include "cIMAGE.H"


#include "resource.h"

#define	IDM_ZOOMIN			1000
#define	IDM_ZOOMOUT			1001
#define	IDM_SET_SPRITE		1002
#define	IDM_SET_FRAME		1003
#define	IDM_SET_CRASH		1004
#define	IDM_GET_IMAGE		1005
#define	IDM_MULTI_IMG		1006
#define	IDM_SPOID			1011

#define	IDM_PRE_ANM			1007
#define	IDM_NEXT_ANM		1008
#define	IDM_PRE_DIRECT		1009
#define	IDM_NEXT_DIRECT		1010

#define	IDM_SETANM			1012
#define	IDM_SETTOOL			1013
#define	IDM_SET_BLOCK		1014
#define	IDM_SET_OVERLAP		1015

#define	dWORK_SET_SPRITE	0
#define	dWORK_SET_FRAME		1
#define	dWORK_SET_CRASH		2
#define	dWORK_SET_BLOCK		3
#define	dWORK_SET_OVERLAP	4
#define	dWORK_GET_IMAGE		5


#define	dDATA_SAD			0
#define	dDATA_SD			1
#define	dDATA_ID			2
#define	dDATA_PHF			3
#define	dDATA_PEF			4

#define	dPUT_FRONT			0
#define	dPUT_BACK			1
#define	dPUT_HALF			2
#define	dPUT_VALID			3

//extern	LPCTSTR		workStr[];

#define	dOBJECT_STATIC		0
#define	dOBJECT_RANDOM		1

class cfgData
{
public:
	char				reg[40];
	char				BMPDir[256],dataDir[10][256];
	int					imgMinRow,imgMinCol,frmMinCol;
	BOOL				bOPTIONBAR,bEFFECTBAR;
	BOOL				bDRAWOUTLINE,bDRAWSHADOW,bDRAWBACK,bPUTINDEX;
	BOOL				bDRAWSPRITEINFO,bDRAWCRASHBOX,bDRAWPREFRAME;
	BOOL				bMULTI,bINCLUDESUB;
	int					putCROSSBAR;
	WORD				workColor,imgColor,frmColor,outColor,crossColor;
	DWORD				dwCustColors[16];

	cPOINT				border;

	WORD				WIDTH,HEIGHT;
	BYTE				objectType;

						cfgData()	{	setDefault();	}

	BOOL				setDefault()
						{	bMULTI			=	TRUE	,bINCLUDESUB	=	FALSE;
							imgMinRow		=	5		,imgMinCol		=	5		,frmMinCol		=	10;
							bOPTIONBAR		=	TRUE	,bEFFECTBAR		=	TRUE	,bDRAWOUTLINE	=	TRUE;
							bDRAWSHADOW		=	FALSE	,bDRAWBACK		=	FALSE	,bDRAWSPRITEINFO=	TRUE;
							bDRAWCRASHBOX	=	FALSE	,bDRAWPREFRAME	=	FALSE	,bMULTI			=	TRUE;
							bINCLUDESUB		=	FALSE	,bPUTINDEX		=	TRUE	,putCROSSBAR	=	dPUT_BACK;

							workColor		=	21		,imgColor		=	0		,frmColor		=	0;
							outColor		=	0x7fff	,crossColor		=	0x7fff	;

							border.x		=	0xffff	,border.y		=	0xffff	;
							WIDTH			=	0xffff	,HEIGHT			=	0xffff	;

							objectType		=	dOBJECT_STATIC;

							memset(dwCustColors,0,sizeof(dwCustColors));

							return FALSE;
						}

};

class cMAIN : public cWND
{
public:

	class	cEDIT_OBJECT
	{
	public:
		char				m_strFolder[512];
		char				m_strFiles[1000][64];
		int					m_iCount,m_iEditFile;

		cEDIT_OBJECT()		{reset();}
		inline	void		reset()
		{
			m_iCount	=	0;
			m_iEditFile	=	0;	
		}

		inline	BOOL		isActive()
		{
			if (m_iCount	>	0)	return	TRUE;

			return	FALSE;
		}

		inline	BOOL		next()
		{
			if (m_iEditFile>=	m_iCount-1)	return	FALSE;

			m_iEditFile++;

			return	TRUE;
		}
		inline	BOOL		previous()
		{
			if (m_iEditFile<=	0		)	return	FALSE;

			m_iEditFile--;
			return	TRUE;
		}

		inline	char*		getEditFileName()
		{
			if (m_iCount	<=	0)	return	NULL;

			return	m_strFiles[m_iEditFile];
		}
		inline	void		setFolder()
		{
			SetCurrentDirectory(m_strFolder);
		}
	};

	cEDIT_OBJECT		m_editInfo;

	HMENU				hMENU;
	cTOOLBAR			TOOLBAR;									//	툴바..
	cSPLITBAR			VERT,HORZ;
	cCOMBOBOX			comboANM,comboDIRECT;

	cRECT				workWnd,frmWnd,imgWnd,bmpWnd,mainWnd;
	int					workMode,currentExt,WIDTH,HEIGHT,BGAnm;
	char				FName[256],curDir[256];
	BOOL				Lock;


	HCURSOR				handCursor,defaultCursor,spoidCursor,nullCursor;
						cMAIN();
						~cMAIN();

	BOOL				Init(HINSTANCE hInst);
	BOOL				Init(HWND hwnd);
	void				InitWnds();
	void				InitWndRect();
	void				SetMenu();

	void				updateKey(WPARAM wParam);
	void				keyThread();

	BOOL				getEditObjectInfo();
	BOOL				editPreviousFile();	//	이전 화일 편집하기
	BOOL				editNextFile();		//	이후 화일 편집하기

	BOOL				New();
	BOOL				Save();
	BOOL				SaveAs();
	BOOL				LoadWeapon(BOOL weapon=TRUE);

	BOOL				SaveLayer();
	BOOL				Load();


	BOOL				Run();
	BOOL				Exit();	

	void				Window();
	void				Draw();
	void				movableWindow();
	void				setToolbar(HWND hwnd);
	void				ResizeWnds(WPARAM wParam,LPARAM lParam);		//	윈도우들의 크기 재설정..
	void				Move(int dx,int dy,BOOL shift,BOOL control);	//	스프라이트 위치 교정..

	void				changeMode(int mode);
	void				resetToolbar();
	void				reformFname(HWND hwnd);

	void				Update();
	void				resetBorder();

	BOOL				SaveEnviroment();
	BOOL				LoadEnviroment();

	static	BOOL		bACTIVEAPP;
	static	LRESULT CALLBACK	WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	static	UINT APIENTRY		hookSave(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam);

};

extern	BYTE		keyBuff[256];
extern	BOOL		lbDown;
extern	BOOL		rbDown;

extern	cMAIN		_MAIN;
extern	BOOL		bVIRGIN;
extern	cfgData		_cfg;
extern	cSPRITE		sdDATA;
extern	cBMP		*wallPaper;
extern	cANIMATION	WEAPON,BASEIMAGE;


class cBLOCKINFO
{
public:
	BYTE	DATA[256][256];

			cBLOCKINFO()
			{	Reset();
				
			}

	void	Reset()
			{	memset(DATA,0,256*256);
			}

	void	Set(int x,int y,BYTE set)
			{	y	+=	128;
				x	+=	128;

				DATA[y][x]	=	set;
			}

	BYTE	Get(int x,int y)
			{	y	+=	128;
				x	+=	128;

				return DATA[y][x];
			}
};

class cBARMENU
{
public:
	char	string[20];
	cRECT	rect;
	DWORD	id;
	BOOL	Active;

	void	Set(char *str,int x1,int y1,int x2,int y2,DWORD num)
			{	strcpy(string,str);
				rect.Set(x1,y1,x2,y2);
				id		=	num;
				Active	=	TRUE;
			}

	void	Set(char *str,int x1,int y1,int x2,int y2)
			{	strcpy(string,str);
				rect.Set(x1,y1,x2,y2);
				Active	=	TRUE;
			}

	void	SetActive(BOOL active)
			{	Active	=	active;
			}
};

extern	cBLOCKINFO	CRASHES;
extern	cBLOCKINFO	BLOCKS;
extern	cBLOCKINFO	OVERLAPBLOCKS;
extern	BOOL		g_isDrawGrid;
extern	int			g_iBlockSize;

#endif