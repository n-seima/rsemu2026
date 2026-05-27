#ifndef _classTEXT_H
#define _classTEXT_H

#include <windows.h>
#include "SFC.H"
#include "cDRAW.H"

#define dJOHAP		0
#define dWANSUNG	1
#define dENGLISH	2

//            段失

const  WORD   C_G   = 2048;         // ぁ
const  WORD   C_GG  = 3072;         // あ
const  WORD   C_N   = 4096;         // い
const  WORD   C_D   = 5120;         // ぇ
const  WORD   C_DD  = 6144;         // え
const  WORD   C_R   = 7168;         // ぉ
const  WORD   C_M   = 8192;         // け
const  WORD   C_B   = 9216;         // げ
const  WORD   C_BB  = 10240;        // こ
const  WORD   C_S   = 11264;        // さ
const  WORD   C_SS  = 12288;        // ざ
const  WORD   C_O   = 13312;        // し
const  WORD   C_J   = 14336;        // じ
const  WORD   C_JJ  = 15360;        // す
const  WORD   C_CH  = 16384;        // ず
const  WORD   C_K   = 17408;        // せ
const  WORD   C_T   = 18432;        // ぜ
const  WORD   C_P   = 19456;        // そ
const  WORD   C_H   = 20480;        // ぞ

// 掻失

const  WORD   J_A   = 96;            // た
const  WORD   J_AE  = 128;           // だ
const  WORD   J_YA  = 160;           // ち
const  WORD   J_YAE = 192;           // ぢ
const  WORD   J_EO  = 224;           // っ
const  WORD   J_E   = 320;           // つ
const  WORD   J_YEO = 352;           // づ
const  WORD   J_YE  = 384;           // て
const  WORD   J_OH  = 416;           // で
const  WORD   J_WA  = 448;           // と
const  WORD   J_WAE = 480;           // ど
const  WORD   J_OE  = 576;           // な
const  WORD   J_YO  = 608;           // に
const  WORD   J_U   = 640;           // ぬ
const  WORD   J_WU  = 672;           // ね
const  WORD   J_WE  = 704;           // の
const  WORD   J_WI  = 736;           // は
const  WORD   J_YU  = 832;           // ば
const  WORD   J_EU  = 864;           // ぱ
const  WORD   J_EUI = 896;           // ひ
const  WORD   J_I   = 928;           // び

// 曽失

const  WORD   G_    = 2;             // ぁ
const  WORD   GG_   = 3;             // あ
const  WORD   GS_   = 4;             // ぃ
const  WORD   N_    = 5;             // い
const  WORD   NJ_   = 6;             // ぅ
const  WORD   NH_   = 7;             // う
const  WORD   D_    = 8;             // ぇ
const  WORD   R_    = 9;             // ぉ
const  WORD   RG_   = 10;            // お
const  WORD   RM_   = 11;            // か
const  WORD   RB_   = 12;            // が
const  WORD   RS_   = 13;            // き
const  WORD   RT_   = 14;            // ぎ
const  WORD   RP_   = 15;            // く
const  WORD   RH_   = 16;            // ぐ
const  WORD   M_    = 17;            // け
const  WORD   B_    = 19;            // げ
const  WORD   BS_   = 20;            // ご
const  WORD   S_    = 21;            // さ
const  WORD   SS_   = 22;            // ざ
const  WORD   O_    = 23;            // し
const  WORD   J_    = 24;            // じ
const  WORD   CH_   = 25;            // ず
const  WORD   K_    = 26;            // せ
const  WORD   T_    = 27;            // ぜ
const  WORD   P_    = 28;            // そ
const  WORD   H_    = 29;            // ぞ

typedef struct {
	BYTE sndchar;
	BYTE fstchar;
} hanchar;

typedef struct {
	WORD jongsung : 5;
	WORD jungsung : 5;
	WORD chosung  : 5;
	WORD det_han_or_eng : 1;
} hancode;

typedef union {
	 hanchar hchar;
	 hancode hcode;
} hangul;

const int  ENGLISH_INPUT_MODE  = 0;
const int  HANGLE_INPUT_MODE   = 1;

class cTEXT 
{
public:
	static	WORD		colShadow,colOutline;
	static	BOOL		bMULTI,bSHADOW,bOUTLINE;
	static	BYTE		*pHFONT,*pEFONT,*pHLAYER,*pELAYER;
	static	int			*pHOFFSET,*pEOFFSET,*pHLOFFSET,*pELOFFSET;
	static	int			engWidth,hanWidth,spaceWidth,fitSize;

	static	BOOL		bINPUT,bHANGUL;
	static	BYTE		pGETPARA[256];
	static	char		*tempStr;
	static	int			curPos,getLimit;
	static	int			getXPos,getYPos,curLen;
	static	BOOL		bVIRGIN,binHANGUL;
	static	WORD		getCOLOR;
	static	BOOL		bJOHAP;
	static	int			PutMode,Alpha;

public:
						~cTEXT();
	static	void		SetShadow(BOOL shadow,WORD color=0){bSHADOW=shadow,colShadow=color;}
	static	void		setMulti(BOOL multi){bMULTI=multi;}
	static	void		SetFontWidth(int h,int e,int s)		{hanWidth=h;engWidth	= e;spaceWidth  = s;}
	static	void		SetEngWidth(int e)					{engWidth	= e;}
	static	void		SetKSWidth(int h)					{hanWidth	= h;}
	static	void		SetSpaceWidth(int s)				{spaceWidth = s;}
	static	void		SetFitSize(int size)				{fitSize	= size;}
	static	void		OutLine(BOOL set,int color=0xfffff)	
						{	bOUTLINE	= set;
							if (color!=0xfffff)	colOutline=color;
						}

	static	BOOL		Init(char *han,char *eng);
	static	void		Destroy();
	
	static	BOOL		LoadFont(char *pFile);

	static	void		PutHan(int x,int y,WORD color,BYTE fstByte,BYTE sndByte);
	static	void		PutJohap(int xPos,int yPos,WORD color,char *pStr);

	static	void		Put	  (int xPos,int yPos,WORD color,char *pStr,int alpha=32); 
	static	void		Put   (int yPos         ,WORD color,char *pStr,int alpha=32);
	static	void		FitPut(int xPos,int yPos,WORD color,char *pStr,int alpha=32);

	static	BOOL		GetString();
	static	void		GetString(int x,int y,int len,char *destChar,WORD color,BOOL virgin=FALSE);
	static	void		PutString();

	static	void		ToggleHan();	//	廃/慎 穿発..

	static	void		Johap2Wansung(BYTE *String);
	static	void		Wansung2Johap(BYTE *String);

};

extern LPCTSTR	lpszWFONTReg;
extern LPCTSTR	lpszJFONTReg;
extern LPCTSTR	lpszEFONTReg;

#endif