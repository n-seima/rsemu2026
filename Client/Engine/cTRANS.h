#ifndef _classTRANS_H
#define _classTRANS_H

#include "SFC.H"

extern	inline	WORD	RGB24ToRGB565(int r,int g,int b);				//	픽셸변환 24 -> RGB565
extern	inline	WORD	RGB24ToRGB555(int r,int g,int b);				//	픽셸변환 24 -> RGB555
extern	inline	WORD	RGB24ToBGR565(int r,int g,int b);				//	픽셸변환 24 -> BGR555
extern	inline	WORD	RGB24To16(int r,int g,int b);					//	픽셸변환 24 -> 16
extern	inline	WORD	RGB24To16(DWORD color);
extern	inline	void	RGB16To24(WORD color,BYTE &r,BYTE &g,BYTE &b);	//	픽셸변환 16 -> 24
extern	inline	DWORD	RGB16To24(WORD color);
extern	inline	DWORD	RGB565To24(WORD color);
extern	inline	WORD	RGB565ToBGR565(WORD color);						//	픽셸변환 RGB565 -> BGR565
extern	inline	WORD	RGB565ToRGB555(WORD color);						//	픽셸변환 RGB565 -> RGB555
extern	inline	WORD	BGR565ToRGB555(WORD color);						//	픽셸변환 RGB565 -> RGB555
extern	inline	WORD	ToRGB565(WORD color);							//	픽셸변환 RGB555 -> RGB565
extern	inline	WORD	ToBGR565(WORD color);							//	픽셸변환 RGB555 -> BGR565
extern	inline	WORD	CONVERTPIXEL(WORD color);						//	픽셸변환 RGB555 -> 현재 그래픽 카드
extern	BOOL	CONVERTSPRITE(WORD *sprite);							//	스프라이트 변환 RGB555 -> 현재 그래픽 카드
extern	BOOL	CONVERTIMAGE(WORD *image);								//	이미지 변환 RGB555 -> 현재 그래픽 카드
extern	BOOL	CONVERTIMAGETo555(WORD *image);							//	현재 그래픽 카드 -> RGB555
extern	BOOL	CONVERTPALETTE(WORD *plt,int range=256);		//	팔레트 변환 RGB555 -> 현재 그래픽 카드
extern	WORD*	CONVERTPALETTE(BYTE *plt);						//	팔레트 변환 24bit -> 16bit
extern	WORD*	CONVERT24TO16(BYTE *buffer,int xs,int ys);			//	이미지 변환 24bit -> 16bit
extern	WORD*	PCX24TO16(BYTE *buffer,int xs,int ys);			//	이미지 변환 24bit -> 16bit
extern	WORD*	CONVERT8TO16(BYTE *buffer,BYTE *plt,int xs,int ys);	//	이미지 변환 8bit -> 16bit
extern	BYTE*	CONVERT16TO24(WORD *buffer);					//	이미지 변환 16bit -> 24bit
extern	WORD	GetBrightColor(WORD _wColor,int _iGrade);

extern	inline	WORD	BRIGHTPIXEL555(WORD srcColor,int grade);
extern	inline	WORD	BRIGHTPIXEL565(WORD srcColor,int grade);

#define	dPLT_EFFECT_NORMAL			0
#define	dPLT_EFFECT_BRIGHT			1
#define	dPLT_EFFECT_COLORIZE		2
#define	dPLT_EFFECT_RED				3
#define	dPLT_EFFECT_GREEN			4
#define	dPLT_EFFECT_BLUE			5
#define	dPLT_EFFECT_YELLOW			6
#define	dPLT_EFFECT_PURPLE			7
#define	dPLT_EFFECT_GRAY			8
#define	dPLT_EFFECT_REVERSE			9
#define dPLT_EFFECT_BLACK			10
#define dPLT_EFFECT_MAX				(dPLT_EFFECT_BLACK + 1)

extern	void	(*fnCONVERTPLT[dPLT_EFFECT_MAX])(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor);
inline	void	EffectPalette(int _iEffect,WORD *_lpwSourcePalette,WORD *_lpwDestPalette,int _iGrade,WORD _wDestColor)
{
	fnCONVERTPLT[_iEffect](_lpwSourcePalette,_lpwDestPalette,_iGrade,_wDestColor);
}

#endif