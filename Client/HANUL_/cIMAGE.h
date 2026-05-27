#ifndef _classIMAGE_H
#define	_classIMAGE_H

#include <windows.h>
#include "SFC.H"

#define	REGSDHEADER		"하늘 스프라이트 데이터"
#define	REGSADHEADER	"하늘 스프라이트 에니메이션 데이터"
#define	REGRBDHEADER	"붉은 보석 건물 데이터"
#define	REGRODHEADER	"붉은 보석 오브젝트 데이터"

#define	REGSDHEADER2	"하늘 스프라이트 데이터 2"
#define	REGSADHEADER2	"하늘 스프라이트 에니메이션 데이터 2"
#define	REGRBDHEADER2	"붉은 보석 건물 데이터 2"
#define	REGRODHEADER2	"붉은 보석 오브젝트 데이터 2"

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

class cSPRITE
{
public:
	WORD			*p16DATA,*pPLT;
	BYTE			*p8DATA,*pLAYER,*pSHADOW;
	int				*spriteOFFSET,*layerOFFSET,*shadowOFFSET;
	int				Count,BPP;
	BYTE			bSHADOW,bLAYER,bLOADEDPLT;

					cSPRITE();
					~cSPRITE();

	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL);
	BOOL			Load(cFILE *file,BOOL loadpalette=TRUE);

	void			Reset();
	void			GetRect(int index,cRECT *rect);
	void			SetPalette(WORD *plt);

	void			Put(int x,int y,int index,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32,int flip=0);
	void			PutLayer(int x,int y,int index,WORD color,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32,int flip=0);
	void			PutShadow(int x,int y,int index,int xrate=100,int yrate=100,int flip=0);
	WORD*			Get16(int index);
	BYTE*			Get8(int index);
};

class cSPRITEQ	:	public cSPRITE
{
public:
	char			FName[20];
	cFILE			*pFILE;
	int				SpriteLocate,LayerLocate,ShadowLocate;

					cSPRITEQ();
					~cSPRITEQ();

	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL);
	BOOL			Load(cFILE *fp,BOOL loadpalette=TRUE);

	BOOL			Put(int x,int y,int index,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32,int flip=0);
	BOOL			PutLayer(int x,int y,int index,WORD color,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32,int flip=0);
	BOOL			PutShadow(int x,int y,int index,int xrate=100,int yrate=100,int flip=0);
};
/*
class cANMDATA
{
public:
	POINTS			*Pos;									//	프레임의 위치
	WORD			*Sprite;								//	에니메이션의 방향별 스프라이트
	BYTE			*IsEvent,*BackFrame,Alpha,bSTATIC;		//	이벤트
	int  			AnmType,FrameCount,DirectCount;			//	프레임 카운트와 에니메이션 카운트

					cANMDATA();
					~cANMDATA();

	BOOL			Init(cFILE *file);
	int				GetSprite(int direct,int frame,BOOL bflip);
	BOOL			IsBack(int direct,int frame,BOOL bflip);
	void			Reset();
};
*/

class cANM_INFO
{
public:
	int  			AnmType,DirectCount;			//	에니메이션 타입,방향 수
	int				ALPHA,FPS,PPS;					//	알파 효과,Frame per Sec,Pixel per Sec
	int				bRELEASE,bREFITFRAME;			//	릴리즈 포인트,프레임 보정
	int				LinkAnm;						//	연계된 에니메이션
};

class cANMDATA	:	public cANM_INFO
{
	BOOL			bMCU;
public:
	POINTS			*Pos;							//	프레임의 위치
	POINTS			*ReleasePos;					//	릴리즈 포인트
	cPOINT			RefitPos;						//	방향별 보정 위치
	WORD			*Sprite;						//	에니메이션의 방향별 스프라이트
	BYTE			*IsEvent,*BackFrame;			//	이벤트

	int				FrameCount;						//	프레임 카운트

					cANMDATA();
					~cANMDATA();

	BOOL			Init(cFILE *file);
	int				GetSprite(int direct,int frame,BOOL bflip);
	BOOL			IsBack(int direct,int frame,BOOL bflip);
	void			Reset();
};

class cANIMATION		:	public cSPRITE
{
public:
	//	에니메이션 데이터
	int				AnmCount,CrashType;	//	에니메이션 카운트와 충돌 박스 타입
	cANMDATA		*pANM;
	BOOL			bFLIP;				//	플립 에니메이션 이다.
	int				MoveOval;
	cRECT			CrashBox;
	cRECT			SelectBox;
	cPOINT			RefitPos;
	WORD			Kind;
	WORD			m_wParts;
	int				m_iDefaultAttack;
	int				m_iDefaultMagic;
	WORD			m_wPlayerJob;
	WORD			m_wCrashSize;

	char			Name[512];

					cANIMATION();
					~cANIMATION();

	void			Reset();
	void			Close();
	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL);
	BOOL			Load(cFILE *fp,BOOL loadpalette=TRUE);

	BOOL			isBack(int anm,int direct,int frame);
	BOOL			isLoaded(){if (Count>0)return	TRUE;return FALSE;}

	void			Put(int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);
	void			PutShadow(int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100);
	void			PutLayer(int x,int y,int anm,int direct,int frame,WORD color,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);
	void			PutReg(int x,int y,int anm,int direct,int frame,int xrate,int yrate);
	void			GetRect(int anm,int direct,int frame,cRECT *rect);
	BOOL			IsEnable()
					{	if (pANM)	return TRUE;
						else		return FALSE;
					}
};

class cANMQ		:	public cSPRITEQ
{
public:
	//	에니메이션 데이터
	int				AnmCount,CrashType;	//	에니메이션 카운트와 충돌 박스 타입
	cANMDATA		*pANM;

					cANMQ();
					~cANMQ();

	void			Reset();

	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL);
	BOOL			Load(cFILE *fp,BOOL loadpalette=TRUE);
	BOOL			Put(int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);
	void			PutReg(int x,int y,int anm,int direct,int frame,int xrate,int yrate);
};

#define	dOBJECT_RAPE	0
#define	dOBJECT_NORMAL	1
#define	dOBJECT_FLOAT	2

class cOBJ_ANM_INFO
{
public:
	POINTS			*Pos;				//	프레임의 위치
	WORD			*Sprite;			//	에니메이션의 방향별 스프라이트
	BYTE			Alpha;				//	이벤트
	WORD			FrameCount;			//	프레임 카운트와 에니메이션 카운트

					cOBJ_ANM_INFO();
					~cOBJ_ANM_INFO();

	BOOL			Init(cFILE *file);
	void			Init(int frameCount);
	int				GetSprite(int frame);
	void			Reset();

};

class cOBJ_ANM			:	public cSPRITE	//	건물/오브젝트 데이터 클래스
{
public:
	WORD			AnmCount;		//	에니메이션 카운트
	int				Attr;			//	오브젝트 성질
	cOBJ_ANM_INFO	*pANM;			//	에니메이션 정보
	BYTE			*pBLOCK,*pCRASH;//	블럭,충돌정보

//	에디터에서만 필요한 정보
	BYTE			bSTATIC;
	cRECT			Crash;			//	충돌 박스
	char			Name[40];

					cOBJ_ANM();
					~cOBJ_ANM();

	void			Reset();

	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL);
	BOOL			Load(cFILE *fp,BOOL loadpalette);
	void			Put(int x,int y);
	void			PutLayer(int x,int y);
	void			PutShadow(int x,int y);
	void			FitPut(int x,int y,int xs,int ys);
};

//--------------------------------------------------------------------------------
//	IMAGE 클래스
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

//--------------------------------------------------------------------------------
//	BMP 화일
//--------------------------------------------------------------------------------
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
	void			decode(BYTE *buffer, int fullsize,int bpl,int width, cFILE *file);
public:

	BOOL			Load(char *fn,BOOL change16bit=FALSE);
	BOOL			Load(FILE *fp);
	

	static	BOOL	Save(char *fn,WORD *image,BOOL overWrite=TRUE);
	static	BOOL	Save(char * fname,BYTE *image,int xs,int ys,BYTE *lpPlt);

};

#endif