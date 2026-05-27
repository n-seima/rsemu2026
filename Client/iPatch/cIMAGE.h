#ifndef _classIMAGE_H
#define	_classIMAGE_H

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
	char			FName[256];

					cSPRITE();
					~cSPRITE();

	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL);
	BOOL			Load(cFILE *file,BOOL loadpalette=TRUE);

	void			Reset();
	void			GetRect(int index,cRECT *rect);
	void			SetPalette(WORD *plt);

	int				FitPut(int x,int y,int dx,int dy,int index,BOOL putshadow=FALSE);
	int				FitPutColor(int x,int y,int dx,int dy,int index,WORD color);
	void			Put(int x,int y,int index,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32,int flip=0);
	void			PutColor(int x,int y,int index,WORD color,int xrate=100,int yrate=100);
	void			PutLayer(int x,int y,int index,WORD color,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32,int flip=0);
	void			PutShadow(int x,int y,int index,int xrate=100,int yrate=100,int flip=0);
	WORD*			Get16(int index);
	BYTE*			Get8(int index);

	void			uploadImage(int _iCount,WORD *_lpSpriteData,int *_lpiOffset);
};

class cSPRITEQ	:	public cSPRITE
{
public:
	char			FName[64];
	cFILE			*pFILE;
	int				SpriteLocate,LayerLocate,ShadowLocate;
	int				m_iDataSize;


					cSPRITEQ();
					~cSPRITEQ();

	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL);
	BOOL			Load(cFILE *fp,BOOL loadpalette=TRUE);

	BOOL			Put(int x,int y,int index,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32,int flip=0);
	BOOL			PutLayer(int x,int y,int index,WORD color,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32,int flip=0);
	BOOL			PutShadow(int x,int y,int index,int xrate=100,int yrate=100,int flip=0);
};

class cANMDATA 
{
	BOOL			m_isMCU;
public:
	POINTS			*m_pPos;							//	프레임의 위치
	POINTS			*m_pReleasePos;					//	릴리즈 포인트
	WORD			*m_pSprite;						//	에니메이션의 방향별 스프라이트
	BYTE			*m_pIsEvent,*m_pBackFrame;		//	이벤트

	int				m_iFrameCount;					//	프레임 카운트
	int  			m_iAnmType,m_iDirectCount;		//	에니메이션 타입,방향 수
	int				m_iALPHA,m_iFPS,m_iPPS;			//	알파 효과,Frame per Sec,Pixel per Sec
	int				m_isRELEASE,m_isREFITFRAME;		//	릴리즈 포인트,프레임 보정
	int				m_iLinkAnm;						//	연계된 에니메이션

					cANMDATA();
					~cANMDATA();

	BOOL			Init(cFILE *file);
	int				GetSprite(int direct,int frame,BOOL bflip);
	BOOL			IsBack(int direct,int frame,BOOL bflip);
	void			Reset();
};

class	cANM_INDEX
{
public:
	int		m_iAnm;
	int		m_iDirect;
	int		m_iFrame;
};

class cANM		:	public cSPRITE
{
public:
	//	에니메이션 데이터
	int				m_iAnmCount;			//	에니메이션 카운트와 충돌 박스 타입
	cANMDATA		*m_pANM;
	BOOL			m_isFLIP;				//	플립 에니메이션 이다.
	int				m_iMoveOval;
	int				m_iAlpha;
	cRECT			m_CrashBox;
	cRECT			m_SelectBox;
	cPOINT			m_RefitPos;
	int				m_iCrashSize;
	int				m_iKind;
	int				m_iDefaultAttack;
	int				m_iDefaultMagic;

					cANM();
					~cANM();

	void			Reset();
	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL);
	BOOL			Load(cFILE *fp,BOOL loadpalette);

	BOOL			IsBack(int anm,int direct,int frame);
	BOOL			IsFront(int anm,int direct,int frame);
	int				GetLinkAnm(int anm);

	void			Put(int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);
	void			PutShadow(int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100);
	void			ShadowMask(int x,int y,int anm,int direct,int frame);
	void			PutLayer(int x,int y,int anm,int direct,int frame,WORD color,int xrate=100,int yrate=100,int effect=dPUT_NORMAL,int alpha=32);
	void			PutReg(int x,int y,int anm,int direct,int frame,int xrate=100,int yrate=100);
	void			GetRect(int anm,int direct,int frame,cRECT *rect);

	int				GetMaxFrame(int anm)	{return m_pANM[anm].m_iFrameCount;}
	int				GetLinkFrame(int anm)	{return m_pANM[anm].m_iLinkAnm;}
	int				GetFPS(int anm)			{return m_pANM[anm].m_iFPS;}
	int				GetPPS(int anm)			{return m_pANM[anm].m_iPPS;}
	int				GetDirectCount(int anm)	{return m_pANM[anm].m_iDirectCount;}
};


class cANMQ		:	public cSPRITEQ
{
public:
	//	에니메이션 데이터
	int				m_iAnmCount;			//	에니메이션 카운트와 충돌 박스 타입
	int				m_iAlpha;
	cANMDATA		*m_pANM;
	BOOL			m_isFLIP;				//	플립 에니메이션 이다.
	int				m_iMoveOval;
	cRECT			m_CrashBox;
	cRECT			m_SelectBox;
	cPOINT			m_RefitPos;
	int				m_iCrashSize;
	int				m_iKind;
	int				m_iDefaultAttack;
	int				m_iDefaultMagic;

					cANMQ();
					~cANMQ();

	BOOL			isGetTriggerFrame();

	void			Reset();
	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL);
	BOOL			Load(cFILE *fp,BOOL loadpalette);

	BOOL			Read(cANM_INDEX *lpAnmInfo,int iReadCount);

	void			Put(int x,int y,int index,int _sx=100,int _sy=100);
	void			PutHalf(int x,int y,int index,int _sx=100,int _sy=100);
	void			PutRed(int x,int y,int index,int _sx=100,int _sy=100);
	void			PutLayer(int x,int y,int index,WORD color,int _sx=100,int _sy=100);

	int				FitPut(int x,int y,int dx,int dy,int index,BOOL putshadow=FALSE);

	int				GetMaxFrame(int anm)					{return m_pANM[anm].m_iFrameCount;}
	int				GetLinkFrame(int anm)					{return m_pANM[anm].m_iLinkAnm;}
	int				GetFPS(int anm)							{return m_pANM[anm].m_iFPS;}
	int				GetPPS(int anm)							{return m_pANM[anm].m_iPPS;}
	int				GetDirectCount(int anm)					{return m_pANM[anm].m_iDirectCount;}
	int				GetSprite(int anm,int direct,int frame)	{return m_pANM[anm].GetSprite(direct,frame,FALSE);}
};

#define	dOBJECT_RAPE					0
#define	dOBJECT_NORMAL					1
#define	dOBJECT_FLOAT					2
#define	dOBJECT_FIXED					4
#define	dOBJECT_CAN_NOT_PASS_OBJECT		8

#define	dOBJECT_TYPE_NORMAL				0
#define	dOBJECT_TYPE_ANM				1
#define	dOBJECT_TYPE_DOOR				2
#define	dOBJECT_TYPE_BRIDGE				3

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
	int				m_iAlpha;
	int				Attr;			//	오브젝트 성질
	cOBJ_ANM_INFO	*pANM;			//	에니메이션 정보
	BYTE			*pBLOCK,*pCRASH,*pOVERLAPBLOCK;//	블럭,충돌정보

//	에디터에서만 필요한 정보
	BYTE			bSTATIC;
	cRECT			Crash;			//	충돌 박스
	int				m_iObjectType;
	char			Name[40];

					cOBJ_ANM();
					~cOBJ_ANM();

	void			Reset();

	BOOL			isDoor()
	{
		if (m_iObjectType	==	dOBJECT_TYPE_DOOR)
			return	TRUE;

		return	FALSE;
	}

	BOOL			Load(char *fn,BOOL loadpalette=TRUE,cNUX *file=NULL);
	BOOL			Load(cFILE *fp,BOOL loadpalette);

	void			Put(int index,int x,int y);
	void			PutLayer(int index,int x,int y);
	void			PutLayerScale(int index,int x,int y,int _iScale);
	void			PutLayer(int index,int x,int y,WORD color);
	void			PutRed(int index,int x,int y);
	int				FitPut(int index,int x,int y,int xs,int ys);
	void			PutShadow(int index,int x,int y);

	void			Put(int x,int y);
	void			PutLayer(int x,int y);
	void			PutLayerScale(int x,int y,int _iScale);
	void			PutRed(int x,int y);
	void			PutShadow(int x,int y);

	void			PutShadowScale(int x,int y,int _iScale);
	void			PutScale(int x,int y,int _iScale);
	
	void			PutShadowScale(int _iIndex,int x,int y,int _iScale);
	void			PutScale(int _iIndex,int x,int y,int _iScale);


	int				FitPut(int x,int y,int xs,int ys);
	int				GetAttr()
					{
						return Attr&(0xffffffff-dOBJECT_FIXED-dOBJECT_CAN_NOT_PASS_OBJECT);
					}

	int				IsFixed()
					{
						return Attr&dOBJECT_FIXED;
					}

	BOOL			IsMissileBlockObject()
					{
						if (Attr&dOBJECT_CAN_NOT_PASS_OBJECT)	return	TRUE;

						return	FALSE;
					}

	int				IsBridge()
					{
						if (m_iObjectType	==	dOBJECT_TYPE_BRIDGE)	return	TRUE;

						return	FALSE;
					}
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
						{	Reset();
						}

	BOOL				Put(int x,int y);
	void				Reset()
						{	pKILL(p16IMAGE);
							pKILL(pPLT);
							pKILL(p8IMAGE);
						}

	static	cIMAGE*		LOAD(char *fn,BOOL change16bit=FALSE);
	virtual	BOOL		Load(char *fn,BOOL change16bit=FALSE){return TRUE;};

};

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃이미지 클래스... 그런데... 왜 SMI지.. -_-;;
class cSMI
{
public:
	int					*m_pOffset,m_iCount,m_iImageLocate;	//	오프셋,이미지 수
	WORD				*m_pImage;	//	이미지
	cFILE				*m_pFile;
	cMCU				*m_lpMcu;//	Mcu

						cSMI();
						~cSMI();

	void				close();//닫기
	BOOL				load(char *fn);

	void				put(int _iIndex,int _iX=0,int _iY=0,int _iAlpha=32);
	WORD*				getImage(int _iIndex);
};
//┃이미지 클래스...
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

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
	WORD				*m_pPLT;
	int					m_iBPP;
	BYTE				m_aPalette[256*3];

						cBMP();
						~cBMP();

	BOOL				Load(char *fn,BOOL change16bit=FALSE);
	BOOL				Load(FILE *fp);
	BOOL				loadPalette(char *_lpstrFileName,BOOL _bIsWantRGB565=TRUE);
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
};

#endif