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
#define	IDM_SET_SELECT		1014

#define	IDM_PRE_ANM			1007
#define	IDM_NEXT_ANM		1008
#define	IDM_PRE_DIRECT		1009
#define	IDM_NEXT_DIRECT		1010

#define	IDM_SETANM			1012
#define	IDM_SETTOOL			1013

#define	dWORK_SET_SPRITE	0
#define	dWORK_SET_FRAME		1
#define	dWORK_SET_CRASH		2
#define	dWORK_GET_IMAGE		3
#define	dWORK_SET_SELECT	4


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

							memset(dwCustColors,0,sizeof(dwCustColors));

							return FALSE;
						}

};

#define	dDATA_CHARACTER		0
#define	dDATA_WEAPON		1
#define	dDATA_WEAPONEFFECT	2
#define	dDATA_SHIELD		3
#define	dDATA_SHIELDEFFECT	4
#define	dDATA_EFFECT		5

enum
{
	eBodyType_none,
	eBodyType_headB,
	eBodyType_headF,
	eBodyType_bodyB,
	eBodyType_bodyF,
	eBodyType_armL,
	eBodyType_armR,
	eBodyType_legL,
	eBodyType_legR,
	eBodyType_weapon,
	eBodyType_shield,
	eBodyType_count,
};

const	int	c_iBodyLayerCount	=	eBodyType_count-1;

class cMAIN : public cWND
{
public:
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

	BOOL				getSlideShowInfo();
	BOOL				updateSlideShow(BOOL _bIsPrevious);

	BOOL				Init(HINSTANCE hInst);
	BOOL				Init(HWND hwnd);
	void				InitWnds();
	void				InitWndRect();
	void				SetMenu();

	void				updateKey(WPARAM wParam);
	void				keyThread();

	BOOL				New();
	BOOL				Save(BOOL _bIsSaveCodeSprite=FALSE);
	BOOL				SaveAs();
	BOOL				LoadData(int what,BOOL _bIsParts=FALSE);
	BOOL				Import(BOOL _isOperateSitDown=FALSE);
	BOOL				exportImages();

	BOOL				SaveLayer();
	void				changePlayerBodyLayer(int _iDirect);
	void				loadPlayerLayerData(BOOL _bByMenu=FALSE);
	void				resetPlayerLayerData();
	void				savePlayerLayerData();


	BOOL				Load(BOOL _isAddSitdown=FALSE);

	BOOL				Run();
	BOOL				Exit();	

	void				Window();
	void				Draw();
	void				movableWindow();
	void				setToolbar(HWND hwnd);
	void				ResizeWnds(WPARAM wParam,LPARAM lParam);		//	윈도우들의 크기 재설정..
	void				Move(int dx,int dy,BOOL shift,BOOL control,BOOL _bIsPressedAlt);	//	스프라이트 위치 교정..
	void				ResaveANMFiles();

	void				changeMode(int mode);
	void				resetToolbar();
	void				resetSpritePos();

	void				reformFname(HWND hwnd);

	void				Update();
	void				resetBorder();

	BOOL				SaveEnviroment();
	BOOL				LoadEnviroment();

	static	BOOL		bACTIVEAPP;
	static	LRESULT CALLBACK	WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	static	UINT APIENTRY		hookSave(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam);

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

	void	SetText(char *_lpstrText)
	{
		strcpy(string,_lpstrText);
	}
};

extern	BYTE		keyBuff[256];
extern	BOOL		lbDown;
extern	BOOL		rbDown;
extern	BOOL		bPLAYANM;			//	현재 에니메이션중이다.
extern	BOOL		bPUTRELEASEPOINT;

extern	cMAIN		_MAIN;
extern	BOOL		bVIRGIN;
extern	BOOL		g_bIsVirginLayerData;
extern	cfgData		_cfg;
extern	cSPRITE		sdDATA,sdCIRCLE;
extern	cBMP		*wallPaper;
extern	cANIMATION	addDATA[6];

extern	cANIMATION	g_aPartsData[eBodyType_count];

extern	cSTRINGS	*imageFOLDERS;

extern	int			ImageFolder[1024];
extern	int			ImageFolderCount;
extern	int			CurrentImageFolder;
extern	WORD		g_wAnmDataType;
extern	WORD		g_wPartsType;
extern	WORD		g_wPlayerJob;

extern	BYTE		g_abDrawLayerOrder[32][8][256][eBodyType_count];
extern	int			g_iSelectParts;

extern	int			g_iLoadPart;

enum
{
	ePart_none,
	ePart_head,
	ePart_body,
	ePart_hand,
	ePart_foot,
	ePart_weapon,
	ePart_shield,
	ePart_shadow,
};

extern	char	*g_strJobName[];

#endif
