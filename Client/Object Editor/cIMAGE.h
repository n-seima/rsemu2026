#ifndef _classIMAGE_H
#define	_classIMAGE_H

#include <windows.h>
#include "SFC.H"

#define	REGSDHEADER		"하늘 스프라이트 데이터"
#define	REGSADHEADER	"하늘 스프라이트 에니메이션 데이터"

//	충돌 박스 타입
#define	dCRASH_NONE			0		//	없음
#define	dCRASH_ONE			1		//	하나가 있다.
#define	dCRASH_PER_ANM		2		//	에니메이션당 하나
#define	dCRASH_PER_DIRECT	3		//	방향 하나.
#define	dCRASH_PER_IMAGE	4		//	이미지당 하나.

//	에니메이션 타입
#define	dANI_CUSTOM			0		//	일반 에니메이션 방향이 없다.
#define	dANI_DIRECT2X		1		//	2 방향 좌우만 있다.
#define	dANI_DIRECT2Y		2		//	2 방향 상하만 있다.
#define	dANI_DIRECT4		3		//	4 방향
#define	dANI_DIRECT8		4		//	8 방향
#define	dANI_DIRECT16		5		//	16 방향

#define	dCRASH_NONE			0
#define	dCRASH_ONE			1
#define	dCRASH_PER_ANM		2
#define	dCRASH_PER_DIRECT	3
#define	dCRASH_PER_IMAGE	4

//	2 방향	좌우
#define	d2DIRECT_W			0
#define	d2DIRECT_E			1

//	2 방향	상하
#define	d2DIRECT_N			0
#define	d2DIRECT_S			1

//	4방향
#define	d4DIRECT_N			0
#define	d4DIRECT_E			1
#define	d4DIRECT_S			2
#define	d4DIRECT_W			3

//	8방향
#define	d8DIRECT_N			0
#define	d8DIRECT_NE			1
#define	d8DIRECT_E			2
#define	d8DIRECT_SE			3
#define	d8DIRECT_S			4
#define	d8DIRECT_SW			5
#define	d8DIRECT_W			6
#define	d8DIRECT_NW			7

//	16방향
#define	d16DIRECT_N			0
#define	d16DIRECT_NNE		1
#define	d16DIRECT_NE		2
#define	d16DIRECT_ENE		3
#define	d16DIRECT_E			4
#define	d16DIRECT_ESE		5
#define	d16DIRECT_SE		6
#define	d16DIRECT_SSE		7
#define	d16DIRECT_S			8
#define	d16DIRECT_SSW		9
#define	d16DIRECT_SW		10
#define	d16DIRECT_WSW		11
#define	d16DIRECT_W			12
#define	d16DIRECT_WNW		13
#define	d16DIRECT_NW		14
#define	d16DIRECT_NNW		15

class cSPRITE	//	스프라이트 데이터.
{
public:
	WORD			*p16DATA,*pPLT;
	BYTE			*p8DATA,*pLAYER;
	int				*spriteOFFSET,*layerOFFSET;
	int				Count,BPP;

					cSPRITE();
					~cSPRITE();

	void			Put(int x,int y,int index,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);
	void			PutFx(int x,int y,int index,int effect=dPUT_NORMAL);
	void			GetRect(int index,cRECT *rect);
	WORD			*GetSprite(int index)
					{	if (!p16DATA) return NULL;
						if (index >= Count) return NULL;

						return p16DATA+spriteOFFSET[index];
					}

	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL);
	BOOL			Load(FILE *fp,BOOL loadpalette=TRUE);
	void			SetPalette(WORD *plt)	{	pPLT	=	plt;}
};

class cSPRITEQ	:	public cSPRITE
{
public:
	char			FName[20];
	cNUX			*pNUX;
	int				SpriteLocate,LayerLocate;

	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL);
	BOOL			Load(FILE *fp,BOOL loadpalette=TRUE);
	BOOL			Put(int x,int y,int index,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);
};

class cANMDATA
{
public:
	POINTS			**Pos;									//	프레임의 위치
	WORD			**Sprite;								//	에니메이션의 방향별 스프라이트
	BYTE			*IsEvent,**BackFrame,Alpha,bSTATIC;					//	이벤트
	int  			AnmType,FrameCount,DirectCount;			//	프레임 카운트와 에니메이션 카운트

					cANMDATA();
					~cANMDATA();

	void			Init(int directCount,int frameCount);
	int				GetSprite(int direct,int frame);
};

class cANIMATION		//	에니메이션 스프라이트 데이터.
{
public:
	//	스프라이트 데이터
	WORD			*p16DATA,*pPLT;
	BYTE			*pLAYER,*pSHADOW,*p8DATA;
	int				*spriteOFFSET,*layerOFFSET,*shadowOFFSET;
	int				SpriteCount;
	int				BPP;				//	8비트? 16비트?
	//	에니메이션 데이터
	int				AnmCount,CrashType;	//	에니메이션 카운트와 충돌 박스 타입
	cANMDATA		*pANM;
	BOOL			bFLIP;				//	플립 에니메이션 이다.

					cANIMATION();
					~cANIMATION();

	void			Reset();

	BOOL			isBack(int anm,int direct,int frame);

	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL);
	BOOL			Load(FILE *fp,BOOL loadpalette=TRUE);

	void			PutReg(int x,int y,int anm,int direct,int frame,int rate);
	void			Put(int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);
	void			PutFx(int x,int y,int anm,int direct,int frame,int effect=dPUT_NORMAL);
	void			Put(WORD *plt,int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);
	void			PutFx(WORD *plt,int x,int y,int anm,int direct,int frame,int effect=dPUT_NORMAL);
	void			GetRect(int anm,int direct,int frame,cRECT *rect);
	int				GetSprite(int anm,int direct,int frame);
	int				GetMaxFrame(int anm)	{return	pANM[anm].FrameCount;}
	int				Frame(int anm,int frame){return min(pANM[anm].FrameCount-1,frame);}

	BOOL			Put(char *fn,int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);
	BOOL			Put(char *fn,cNUX *file,int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);

	void			SetPalette(WORD *plt)	{	pPLT	=	plt;}
};

class cANMQ		:	public cANIMATION	//	에니메이션 스프라이트 데이터.
{
public:
	char			FName[20];
	cNUX			*pNUX;
	int				SpriteLocate,LayerLocate,ShadowLocate;

	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL);
	BOOL			Load(FILE *fp,BOOL loadpalette=TRUE);
	BOOL			Put(int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);
};

//--------------------------------------------------------------------------------
//	BMP 화일
//--------------------------------------------------------------------------------

class cIMAGE
{
public:
	WORD				*p16IMAGE,*pPLT;
	BYTE				*p8IMAGE;
	int					Width,Height,BPP;

						cIMAGE()
						{	p16IMAGE	=	NULL;
							pPLT		=	NULL;
							p8IMAGE		=	NULL;
						}

						~cIMAGE()
						{	pKILL(p16IMAGE);
							pKILL(pPLT);
							pKILL(p8IMAGE);
						}

	BOOL				Put(int x,int y);
	virtual	BOOL		Load(char *fn,BOOL change16bit=FALSE){return TRUE;};
};

class cBMP	:	public cIMAGE
{
public:
	int					rWidth,bytePerLine,Colors;

	UINT				fileLength,dibSize;
	LPBITMAPINFO		Info; 
	LPBITMAPINFOHEADER	InfoHd;
	BITMAPFILEHEADER	bmFileHeader;
	BYTE				*data,*ptr;
	RGBQUAD				*RGBTable;

						cBMP();
						~cBMP();

	BOOL				Load(char *fn,BOOL change16bit=FALSE);
	BOOL				Load(FILE *fp);
};

//--------------------------------------------------------------------------------
//	TGA 화일
//--------------------------------------------------------------------------------
class cTGA	:	public cIMAGE
{
public:
	BOOL			Load(char *fn,BOOL change16bit=FALSE);
	BOOL			Load(FILE *fp);

	static	BOOL	Save(char *fn,WORD *image,BOOL overWrite=TRUE);
};

//--------------------------------------------------------------------------------
//	PCX 화일
//--------------------------------------------------------------------------------
class cPCX	:	public cIMAGE
{
	void			decode(BYTE *buffer, int bpl, cFILE *file);
public:
	BOOL			Load(char *fn,BOOL change16bit=FALSE);
	BOOL			Load(FILE *fp);

	static	BOOL	Save(char *fn,WORD *image,BOOL overWrite=TRUE);
};

#endif