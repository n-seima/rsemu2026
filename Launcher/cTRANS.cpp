#include <stdio.h>
#include "cDRAW.H"

inline WORD
RGB24ToRGB565(int r,int g,int b)
{	r	=	(r>>3)<<11;
	g	=	(g>>2)<<5;
	b	=	(b>>3);

	return (WORD) (r+g+b);
}

inline WORD
RGB24ToRGB555(int r,int g,int b)
{	r	=	(r>>3)<<10;
	g	=	(g>>3)<<5;
	b	=	(b>>3);

	return (WORD) (r+g+b);
}

inline WORD
RGB24ToBGR565(int r,int g,int b)
{	r	=	(r>>3);
	g	=	(g>>2)<<5;
	b	=	(b>>3)<<11;

	return (WORD) (r+g+b);
}

inline WORD
RGB24To16(int r,int g,int b)
{	if (_PixelFormat	==	dRGB565)	return	RGB24ToRGB565(r,g,b);
	if (_PixelFormat	==	dRGB555)	return	RGB24ToRGB555(r,g,b);
	return	RGB24ToBGR565(r,g,b);
}

inline WORD
RGB24To16(DWORD color)
{	int	r	=	GetRValue(color);
	int	g	=	GetGValue(color);
	int	b	=	GetBValue(color);

	return RGB24To16(r,g,b);
}

inline void
RGB16To24(WORD color,BYTE &r,BYTE &g,BYTE &b)
{	if (_PixelFormat	==	dRGB565)
	{	r	=	(color>>11    )<<3;
		g	=	((color>>5)&63)<<2;
		b	=	(color&31	  )<<3;
		return;
	}
	if (_PixelFormat	==	dRGB555)
	{	r	=	(color>>10    )<<3;
		g	=	((color>>5)&31)<<3;
		b	=	(color&31	  )<<3;
		return;
	}

	r	=	(color&31	  )<<3;
	g	=	((color>>5)&63)<<2;
	b	=	(color>>11    )<<3;
}

inline WORD
RGB565ToBGR565(WORD color)
{	int	r	=	color>>11;
	int	g	=	(color>>5)&63;
	int	b	=	color&31;

	return (WORD) (  (b<<11) + (g<<5) + r  );
}

inline WORD
RGB565ToRGB555(WORD color)
{	int	r	=	color>>11;
	int	g	=	(color>>6)&31;
	int	b	=	color&31;

	return (WORD) (  (r<<10) + (g<<5) + b  );
}

inline WORD
CONVERTPIXEL(WORD color)
{	if (_PixelFormat	==	dRGB555)	return	RGB565ToRGB555(color);

	return	RGB565ToBGR565(color);
}

BOOL
CONVERTSPRITE(WORD *sprite)
{	if (_PixelFormat	==	dRGB565)	return TRUE;
	if (!sprite)	return	MSGOUT("Error!! in cTRANS::RGB565ToRGB555 - pointer 'sprite' is valid");

	int   i,j,k,xs ,ys,count,BufferPoint=4,size;

	xs	=	sprite[0];
	ys	=	sprite[1];

	for(j=0;j<ys;j++)
	{	count	=	sprite[BufferPoint];

		for(i=0;i<count;i++)
		{	size				=	sprite[BufferPoint+1];
			BufferPoint			+=	2;

			for (k=0;k<size;k++)
			{	WORD color	=	sprite[BufferPoint+k];
				sprite[BufferPoint+k]	=	CONVERTPIXEL(color);
			}
			BufferPoint	+=	size;
		}
	}

	return TRUE;
}

BOOL
CONVERTIMAGE(WORD *image)
{	if (!image)	return	MSGOUT("Error!! in CONVERTIMAGE - pointer 'image' is valid");

	int	i;
	int	size=image[0]*image[1];

	if (_PixelFormat	==	dRGB555)
	{	for (i=2;i<size+2;i++)
			image[i]	=	RGB565ToRGB555(image[i]);
	}
	else
	if (_PixelFormat	==	dBGR565)
	{	for (i=2;i<size+2;i++)
			image[i]	=	RGB565ToBGR565(image[i]);
	}

	return TRUE;
}

BOOL
CONVERTPALETTE(WORD *plt,int range)
{	if (!plt)	return	MSGOUT("Error!! in CONVERTIMAGE - pointer 'plt' is valid");
	int	i;

	if (_PixelFormat	==	dRGB555)
	{	for (i=0;i<range;i++)
			plt[i]	=	RGB565ToRGB555(plt[i]);
	}
	else
	if (_PixelFormat	==	dBGR565)
	{	for (i=0;i<range;i++)
			plt[i]	=	RGB565ToBGR565(plt[i]);
	}

	return TRUE;
}

WORD*
CONVERTPALETTE(BYTE *plt)
{	WORD *result;
	result	=	new WORD [256];

	for (int i=0;i<256;i++)	result[i]	=	RGB24To16(plt[i*3],plt[i*3+1],plt[i*3+2]);

	return result;
}

WORD*
CONVERT24TO16(BYTE *buffer,int xs,int ys)
{	WORD	*result;

	result		=	new WORD [xs*ys+2];
	result[0]	=	xs;
	result[1]	=	ys;

	for (int i=0;i<xs*ys;i++)
	{	result[i+2]	=	RGB24To16(buffer[i*3],buffer[i*3+1],buffer[i*3+2]);
	}

	return result;
}

WORD*
CONVERT24TO16(BYTE *buffer,BYTE *plt,int xs,int ys)
{	WORD	*result;
	WORD	*plt2	=	CONVERTPALETTE(plt);

	result		=	new WORD [xs*ys+2];
	result[0]	=	xs;
	result[1]	=	ys;

	for (int i=0;i<xs*ys;i++)
	{	int index	=	buffer[i];
		result[i+2]	=	plt2[index];
	}

	pKILL(plt);

	return result;
}


BYTE*
CONVERT16TO24(WORD *buffer)
{	BYTE	*result;
	int		xs	=	buffer[0],ys =	buffer[1];

	result	=	new BYTE [xs*ys*3];

	for (int i=0;i<xs*ys;i++)
		RGB16To24(buffer[i+2],result[i*3],result[i*3+1],result[i*3+2]);

	return result;
}

WORD*
PCX24TO16(BYTE *buffer,int xs,int ys)
{	WORD	*result;

	result		=	new WORD [xs*ys+2];
	result[0]	=	xs;
	result[1]	=	ys;

	for(int j = 0; j < ys; j++)
	{	int	src	= j * xs * 3;
		int	dest= j * xs+2;

		for(int i = 0; i < xs; i++,dest++,src++)
			result[dest]	=	RGB24To16(buffer[src],buffer[src + xs],buffer[src + xs* 2]);
	}


	return result;
}
