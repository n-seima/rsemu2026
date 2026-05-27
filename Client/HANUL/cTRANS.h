#ifndef _classTRANS_H_
#define _classTRANS_H_

#include <windows.h>

#include "SFC.H"

class cTRANS
{
public:

	static	int		GetSize(int xs,int ys,WORD TransColor,WORD *data);
	static	int		GetSize(int xs,int ys,BYTE TransColor,BYTE *data);
	static	int		GetShadowSize(int xs,int ys,WORD TransColor,WORD *data);
	static	int		GetShadowSize(int xs,int ys,BYTE TransColor,BYTE *data);
	static	int		GetFontSize(WORD *data);
	static	int		GetFontSize(BYTE *data);
	static	int		GetSize(WORD *sprite);
	static	int		GetSize(BYTE *sprite);
	static	WORD	*ImageToSprite(int xs,int ys,int xp,int yp,WORD TransColor,WORD *data,int &TotalSize);
	static	BYTE	*ImageToSprite(int xs,int ys,int xp,int yp,BYTE TransColor,BYTE *data,int &TotalSize);
	static	BYTE	*ImageToShadow(int xs,int ys,int xp,int yp,WORD TransColor,WORD *data,int &size);
	static	BYTE	*ImageToShadow(int xs,int ys,int xp,int yp,BYTE TransColor,BYTE *data,int &size);
	static	BYTE	*FontToSprite(BYTE *font);
	static	BYTE	*FontToSprite(WORD *font);

	static	int		GetSTFSize(WORD *sprite);
	static	BYTE	*SpriteToFont(WORD *sprite);				//	압축 스프라이트를 압축 폰트로 변환..
	static	BYTE	*LayerToFontLayer(WORD *sprite,int size);


	static	WORD	*SpriteToImage(WORD *data,WORD TransColor);
	static	BYTE	*SpriteToImage(BYTE *data,BYTE TransColor);

	static	BYTE	*BPP15To8(WORD *src,WORD *plt);
	static	WORD	*BPP8To15(BYTE *src,WORD *plt);

	static	WORD	*RGB565ToRGB555(WORD *src);
	static	WORD	*RGB555ToRGB565(WORD *src);
	static	WORD	*RGB565ToBGR565(WORD *src);
	static	WORD	*BGR565ToRGB565(WORD *src);

	static	WORD	RGB565ToRGB555(WORD color);
	static	WORD	RGB555ToRGB565(WORD color);
	static	WORD	RGB565ToBGR565(WORD color);
	static	WORD	BGR565ToRGB565(WORD color);

	static	BOOL	FontToSprite(char *in_file,char *out_file);
};

extern	WORD*	PALETTE24TO15(BYTE *plt);
inline	WORD	RGB24TO15(int r,int g,int b)
{	r	=	(r>>3)<<10;
	g	=	(g>>3)<<5;
	b	=	(b>>3);

	return (WORD) (r+g+b);
}


inline	WORD	RGB24ToRGB565(int r,int g,int b);				//	픽셸변환 24 -> RGB565
inline	WORD	RGB24ToRGB555(int r,int g,int b);				//	픽셸변환 24 -> RGB555
inline	WORD	RGB24ToBGR565(int r,int g,int b);				//	픽셸변환 24 -> BGR555
inline	WORD	RGB24To16(int r,int g,int b);					//	픽셸변환 24 -> 16
inline	void	RGB16To24(WORD color,BYTE &r,BYTE &g,BYTE &b);	//	픽셸변환 16 -> 24
inline	WORD	RGB565ToBGR565(WORD color);						//	픽셸변환 RGB565 -> BGR565
inline	WORD	RGB565ToRGB555(WORD color);						//	픽셸변환 RGB565 -> RGB555
inline	WORD	ToRGB565(WORD color);							//	픽셸변환 RGB555 -> RGB565
inline	WORD	ToBGR565(WORD color);							//	픽셸변환 RGB555 -> BGR565
inline	WORD	CONVERTPIXEL(WORD color);						//	픽셸변환 RGB555 -> 현재 그래픽 카드
BOOL	CONVERTSPRITE(WORD *sprite);					//	스프라이트 변환 RGB555 -> 현재 그래픽 카드
BOOL	CONVERTIMAGE(WORD *image);						//	이미지 변환 RGB555 -> 현재 그래픽 카드
BOOL	CONVERTPALETTE(WORD *plt,int range=256);		//	팔레트 변환 RGB555 -> 현재 그래픽 카드
WORD*	CONVERTPALETTE(BYTE *plt);						//	팔레트 변환 24bit -> 16bit
WORD*	CONVERT24TO16(BYTE *buffer,int xs,int ys);			//	이미지 변환 24bit -> 16bit
WORD*	PCX24TO16(BYTE *buffer,int xs,int ys);			//	이미지 변환 24bit -> 16bit
WORD*	CONVERT8TO16(BYTE *buffer,BYTE *plt,int xs,int ys);	//	이미지 변환 8bit -> 16bit
BYTE*	CONVERT16TO24(WORD *buffer);					//	이미지 변환 16bit -> 24bit

#endif