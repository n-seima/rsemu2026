#include <windows.h>
#include <commctrl.h>
#include "cDIB.H"
#include "cMESSAGE.H"
#include "cDRAW.H"
#include "cTEXT.H"


WORD		_WHITE		,_BLACK;
WORD		_LTGRAY		,_GRAY		,_DEEPGRAY;
WORD		_LTCYAN		,_CYAN		,_DEEPCYAN;
WORD		_LTBLUE		,_BLUE		,_DEEPBLUE;
WORD		_LTRED		,_RED		,_DEEPRED;
WORD		_LTGREEN	,_GREEN		,_DEEPGREEN;
WORD		_LTYELLOW	,_YELLOW	,_DEEPYELLOW;
WORD		_LTPURPLE	,_PURPLE	,_DEEPPURPLE;
int			_PixelFormat;

WORD		_gCOL[7][16];
WORD		_ALPHATABLE[0x7fff+1];
BYTE		_ALPHAVALUE[0x7fff+1];

WORD		redEFFECT[7][32][32];
WORD		blueEFFECT[7][32][32];
WORD		greenEFFECT[7][64][64];

cDIB		dib;

//			rgb 565로 받아 들였을때를 기준으로 한다.
WORD		
RGBmix(BYTE r,BYTE g,BYTE b)
{
	g	/=	2;
	return 	(WORD) (  (((WORD)(r<<10))) | (((WORD)(g<<5))) | ((WORD)b)  );
}

class templete
{
public:
	templete();
};


void initColors()
{	int i;
/***************************************************************************
		7가지 그라데이션 데이터.. 완죤히 노가다로 뽑아온.. ^^
		이런 노가다를 스스럼없이 하는 나는 넘 위대한거 같아..
***************************************************************************/

BYTE yellow[8][3] = {{10,21, 0},{15,31, 2},{17,35, 4},{20,48, 6},{22,45, 9},{25,50,11},{27,55,14},{29,59,16}};
BYTE cyan  [8][3] = {{ 0,27,13},{ 4,36,18},{ 6,40,20},{ 9,45,22},{11,50,25},{14,55,27},{16,60,30},{18,63,31}};
BYTE red   [8][3] = {{31, 8, 0},{31,15, 0},{31,23, 0},{31,31, 0},{31,39, 0},{31,47, 0},{31,55, 0},{31,63, 0}};
BYTE green [8][3] = {{ 0,35, 8},{ 1,41,12},{ 3,46,15},{ 5,51,17},{ 8,56,20},{10,60,22},{14,63,24},{20,63,27}};
BYTE gray  [8][3] = {{ 9,18, 9},{14,28,14},{16,32,16},{18,37,18},{21,42,21},{23,46,23},{25,51,25},{27,55,27}};
BYTE blue  [8][3] = {{ 6,13,31},{11,23,31},{14,28,31},{17,34,31},{19,39,31},{22,44,31},{25,50,30},{29,54,31}};
BYTE purple[8][3] = {{17,13,17},{22,17,22},{24,21,24},{26,26,26},{29,31,29},{31,35,31},{31,43,31},{31,50,31}};

	// 그라데이션 칼라 셋팅!!! 그래픽 카드의 픽셸 포맷에 관계 없음.. 발진 준비!!! ^^
	for (i=0;i<8;i++) 
	{	_gCOL[0][i	 ]=RGBmix(yellow[i][0],yellow[i][1],yellow[i][2]);
		_gCOL[0][15-i]=RGBmix(yellow[i][0],yellow[i][1],yellow[i][2]);
		_gCOL[1][i	 ]=RGBmix(cyan  [i][0],cyan  [i][1],cyan  [i][2]);
		_gCOL[1][15-i]=RGBmix(cyan  [i][0],cyan  [i][1],cyan  [i][2]);
		_gCOL[2][i	 ]=RGBmix(red   [i][0],red   [i][1],red   [i][2]);
		_gCOL[2][15-i]=RGBmix(red   [i][0],red   [i][1],red   [i][2]);
		_gCOL[3][i	 ]=RGBmix(green [i][0],green [i][1],green [i][2]);
		_gCOL[3][15-i]=RGBmix(green [i][0],green [i][1],green [i][2]);
		_gCOL[4][i	 ]=RGBmix(gray  [i][0],gray  [i][1],gray  [i][2]);
		_gCOL[4][15-i]=RGBmix(gray  [i][0],gray  [i][1],gray  [i][2]);
		_gCOL[5][i	 ]=RGBmix(blue  [i][0],blue  [i][1],blue  [i][2]);
		_gCOL[5][15-i]=RGBmix(blue  [i][0],blue  [i][1],blue  [i][2]);
		_gCOL[6][i	 ]=RGBmix(purple[i][0],purple[i][1],purple[i][2]);
		_gCOL[6][15-i]=RGBmix(purple[i][0],purple[i][1],purple[i][2]);
	}

	_WHITE		=RGBmix(31,63,31);
	_BLACK		=RGBmix( 0, 0, 0);

	_LTGRAY		=RGBmix(24,48,24);
	_GRAY 		=RGBmix(16,32,16);
	_DEEPGRAY	=RGBmix( 8,16, 8);

	_LTCYAN 	=RGBmix( 0,63,31);
	_CYAN		=RGBmix( 0,32,16);
	_DEEPCYAN	=RGBmix( 0,16, 8);

	_LTBLUE 	=RGBmix( 0, 0,31);
	_BLUE		=RGBmix( 0, 0,16);
	_DEEPBLUE	=RGBmix( 0, 0, 8);

	_LTRED  	=RGBmix(31, 0, 0);
	_RED	 	=RGBmix(16, 0, 0);
	_DEEPRED 	=RGBmix( 8, 0, 0);

	_LTGREEN	=RGBmix( 0,63, 0);
	_GREEN		=RGBmix( 0,32, 0);
	_DEEPGREEN	=RGBmix( 0,16, 0);

	_LTYELLOW	=RGBmix(31,63, 0);
	_YELLOW		=RGBmix(16,32, 0);
	_DEEPYELLOW	=RGBmix( 8,16, 0);

	_LTPURPLE	=RGBmix(31, 0,31);
	_PURPLE		=RGBmix(16, 0,16);
	_DEEPPURPLE	=RGBmix( 8, 0, 8);

	//	알파 블렌딩 효과를 위한 테이블을 만든다..
	for (i=0;i<=0x7fff;i++)
	{	BYTE	r,g,b,alpha;
		UINT	first,second;

		r=(i>>10)&31;
		g=(i>>5 )&31;
		b= i     &31;
		alpha			=	(r+g+b)*32/93;

		_ALPHAVALUE[i]	=	alpha;

		first			=	i&d4AM_RGB555_1;
		first			>>=	5;
		first			*=	alpha;
		first			&=	d4AM_RGB555_1;

		second			=	i&d4AM_RGB555_2;
		second			*=	alpha;
		second			>>=	5;
		second			&=	d4AM_RGB555_2;
		_ALPHATABLE[i]	=(WORD)(first|second);
	}

	int spr,bg;

	for (spr=0;spr<32;spr++)
		for (bg=0;bg<32;bg++)
		{	int soft=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));

			redEFFECT[dEFFECT_DIFFERENT]		[spr][bg]=(max(bg,spr)-min(bg,spr))<<10;
			redEFFECT[dEFFECT_DODGE_BURN]		[spr][bg]=(min(((bg*(32-spr)))/32 , 31))<<10;
			redEFFECT[dEFFECT_SOFT]				[spr][bg]=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31))<<10;
			redEFFECT[dEFFECT_DODGE]			[spr][bg]=(min((bg*32)/(32-spr),31))<<10;
			redEFFECT[dEFFECT_EXTREAM_LIGHT]	[spr][bg]=(min(spr+bg,31))<<10;
			redEFFECT[dEFFECT_LIGHT]			[spr][bg]=(max(bg,spr))<<10;
			redEFFECT[dEFFECT_SOFT_DODGE]		[spr][bg]=(min((soft*32)/(32-spr),31))<<10;
		}

	for (spr=0;spr<32;spr++)
		for (bg=0;bg<32;bg++)
		{	int soft=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));

			greenEFFECT[dEFFECT_DIFFERENT]		[spr][bg]=(max(bg,spr)-min(bg,spr))<<5;
			greenEFFECT[dEFFECT_DODGE_BURN]		[spr][bg]=(min(((bg*(32-spr)))/32 , 31))<<5;
			greenEFFECT[dEFFECT_SOFT]			[spr][bg]=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31))<<5;
			greenEFFECT[dEFFECT_DODGE]			[spr][bg]=(min((bg*32)/(32-spr),31))<<5;
			greenEFFECT[dEFFECT_EXTREAM_LIGHT]	[spr][bg]=(min(spr+bg,31))<<5;
			greenEFFECT[dEFFECT_LIGHT]			[spr][bg]=(max(bg,spr))<<5;
			greenEFFECT[dEFFECT_SOFT_DODGE]		[spr][bg]=(min((soft*32)/(32-spr),31))<<5;
		}

	for (spr=0;spr<32;spr++)
		for (bg=0;bg<32;bg++)
		{	int soft=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));

			blueEFFECT[dEFFECT_DIFFERENT]		[spr][bg]=(max(bg,spr)-min(bg,spr));
			blueEFFECT[dEFFECT_DODGE_BURN]		[spr][bg]=(min(((bg*(32-spr)))/32 , 31));
			blueEFFECT[dEFFECT_SOFT]			[spr][bg]=(min(max(bg,spr)+((32-max(bg,spr))*min(bg,spr)/32),31));
			blueEFFECT[dEFFECT_DODGE]			[spr][bg]=(min((bg*32)/(32-spr),31));
			blueEFFECT[dEFFECT_EXTREAM_LIGHT]	[spr][bg]=(min(spr+bg,31));
			blueEFFECT[dEFFECT_LIGHT]			[spr][bg]=(max(bg,spr));
			blueEFFECT[dEFFECT_SOFT_DODGE]		[spr][bg]=(min((soft*32)/(32-spr),31));
		}
}

templete	tmp;

templete::templete()
{	initColors();
}

cDIB::cDIB()
{	hDIBWND		=	NULL;
	hBM			=	NULL;
	isACTIVE	=	FALSE;

	hFONT		=	NULL;
	bkMODE		=	TRANSPARENT;
	textColor	=	RGB(255,255,255);
}

cDIB::cDIB(HWND hwnd)
{	hBM		=	NULL;
	initColors();
	Init(hwnd);
}

BOOL
cDIB::Init(HWND hwnd)
{	Close();

	if (hBM)
	{	DeleteObject(hBM);
		hBM	=	NULL;
	}

	hDIBWND	=	hwnd;

	if (!hDIBWND) return FALSE;

	BITMAPINFO BMInfo;

	cRECT_SIZE	view;

	view.ClientFrame(hDIBWND);

	WIDTH	=	view.xs;
	HEIGHT	=	view.ys;

	if (WIDTH%4!=0) WIDTH=WIDTH/4*4+4;

	hDC		=	GetDC(hDIBWND);

	BMInfo.bmiHeader.biSize				= sizeof (BITMAPINFOHEADER);
	BMInfo.bmiHeader.biWidth			= WIDTH;
	BMInfo.bmiHeader.biHeight			= -HEIGHT;
	BMInfo.bmiHeader.biPlanes			= 1;
	BMInfo.bmiHeader.biBitCount			= 16;
	BMInfo.bmiHeader.biCompression		= BI_RGB;
	BMInfo.bmiHeader.biSizeImage		= 0;
	BMInfo.bmiHeader.biXPelsPerMeter	= 0;
	BMInfo.bmiHeader.biYPelsPerMeter	= 0;
	BMInfo.bmiHeader.biClrUsed			= 0;
	BMInfo.bmiHeader.biClrImportant		= 0;

	hBM = CreateDIBSection (hDC , (BITMAPINFO *)&BMInfo, DIB_RGB_COLORS, (VOID **)&p2SCREEN, NULL, 0);

	if (!hBM) cMSG::Test();

	ReleaseDC(hDIBWND , hDC );

	return TRUE;
}

BOOL
cDIB::Flip(cRECT *rect)
{	if (!IsWindow(hDIBWND)	) return FALSE;
	if (!isACTIVE			) return FALSE;

	RECT		Dest;
	PAINTSTRUCT ps;

	BeginPaint(hDIBWND, &ps);

	HDC	destDC	= GetDC(hDIBWND);
	GetClientRect(hDIBWND, &Dest);

	BitBlt(destDC, 0, 0, Dest.right - Dest.left, Dest.bottom - Dest.top, hDC, 0, 0, SRCCOPY);

	ReleaseDC(hDIBWND, destDC);
	Active(FALSE);

	EndPaint(hDIBWND, &ps);

	return TRUE;
}

BOOL
cDIB::Active(BOOL active)
{	if (!hDIBWND) return FALSE;

	if (active)
	{	cDRAW::Init(WIDTH,HEIGHT,p2SCREEN);

		hDC				= CreateCompatibleDC(0);
		DefaultBitmap	= (HBITMAP)SelectObject(hDC, hBM);

		if (hFONT) SelectObject( hDC, hFONT);
		SetBkMode	( hDC, bkMODE);
		SetTextColor( hDC, textColor);

		isACTIVE		= TRUE;
		cTEXT::hDC		= hDC;

		return TRUE;
	}

	if (!isACTIVE) return FALSE;

	SelectObject(hDC, DefaultBitmap);
	DeleteObject(DefaultBitmap);
	DeleteDC(hDC);

	isACTIVE	=	FALSE;
	return TRUE;
}

BOOL
cDIB::TextSetting(HFONT hfont,int mode,int r,int g,int b)
{	if (!hDIBWND) return FALSE;

	hFONT		=	hfont;
	bkMODE		=	mode;
	textColor	=	RGB( r, g, b);

	return TRUE;
}

void
cDIB::Close()
{	Active(FALSE);
//	if (hBM)
//	{	DeleteObject(hBM);
//		hBM	=	NULL;
//	}
}