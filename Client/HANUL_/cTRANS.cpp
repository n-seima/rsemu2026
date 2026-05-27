#include <stdio.h>
#include "cTRANS.H"
#include "cTEXT.H"
#include "cDRAW.H"

WORD*
PALETTE24TO15(BYTE *plt)
{	WORD *result;
	result	=	new WORD [256];

	for (int i=0;i<256;i++)	result[i]	=	RGB24TO15(plt[i*3],plt[i*3+1],plt[i*3+2]);

	return result;
}

int
cTRANS::GetSize(int xs,int ys,WORD TransColor,WORD *data)
{	int size=0,y,x,m,dp=0;

	size+=2+2;	//	스프라이트 x,y사이즈(2 byte)와 이 스프라이트의 위치(2 byte)

	for	(y=0; y<ys; y++)
	{
		size++;			//	데이터 블럭 수..

		for (x=0;x<xs;)
		{
			while (data[dp] == TransColor && x < xs )
			{
				dp++;
				x++;
			}

			m	=	0;

			while (data[dp] != TransColor && x < xs ) 
			{	
				m++;
				x++;
				dp++;
			}

			if (m!=0)
				size += 2 + m;			// 투명칼라가 아닌 데이타 (m byte)
		}
	}

	return size;
}

int
cTRANS::GetSize(int xs,int ys,BYTE TransColor,BYTE *data)
{	
	int size=0,y,x,dp=0;

	size+=4+4;	//	스프라이트 x,y사이즈(2 byte)와 이 스프라이트의 위치(2 byte)

	for(y=0; y<ys; y++)
	{	
		size++;			//	데이터 블럭 수..
		for (x=0;x<xs;)
		{
			int zeroCount	=	0,pixelCount	=	0;
			int	iOx			=	x;
			int	iODP		=	dp;

			while (data[dp] == TransColor && x < xs )
			{
				dp++;
				x++;
				zeroCount++;
			}

			if	(zeroCount	==	xs)
				goto LABEL_ZERO_SETTING;

			x			=	iOx;
			dp			=	iODP;
			zeroCount	=	0;

			while (data[dp] == TransColor && x < xs )
			{	dp++;
				x++;
				zeroCount++;

				if (zeroCount >= 255)
					goto LABEL_SETTING;
			}
			while (data[dp] != TransColor && x < xs ) 
			{	dp++;
				x++;
				pixelCount++;
				if (pixelCount >= 255) goto LABEL_SETTING;
			}

			if (pixelCount!=0)
			{
LABEL_SETTING:
				size += 2 + pixelCount;			// 투명칼라가 아닌 데이타 (m byte)
			}
		}
LABEL_ZERO_SETTING:
		size	=	size;
	}

	return size;
}

int
cTRANS::GetShadowSize(int xs,int ys,WORD TransColor,WORD *data)
{	int size	=	8,i,j,dp=0;

	for(i=0; i<ys; i++)
	{	size++;						//	데이터 블럭 수..
		for (j=0;j<xs;)
		{	int	zeroCount	=	0,pixelCount	=	0;

			while (data[dp] == TransColor && j < xs )
			{	dp++;
				j++;
				zeroCount++;
				if (zeroCount>=255)	goto LABEL_SETTING;

			}
			while (data[dp] != TransColor && j < xs )
			{	j++;
				dp++;
				pixelCount++;

				if (pixelCount>=255)goto LABEL_SETTING;
			}

			if (pixelCount!=0)
			{
LABEL_SETTING:
				size += 2;	// 투명칼라가 아닌 데이타 (m byte)
			}
		}
	}

	return size;
}

int
cTRANS::GetShadowSize(int xs,int ys,BYTE TransColor,BYTE *data)
{	int size	=	8,i,j,dp=0;

	for(i=0; i<ys; i++)
	{	size++;						//	데이터 블럭 수..
		for (j=0;j<xs;)
		{	int	zeroCount	=	0,pixelCount	=	0;

			while (data[dp] == TransColor && j < xs )
			{	dp++;
				j++;
				zeroCount++;
				if (zeroCount>=255)	goto LABEL_SETTING;

			}
			while (data[dp] != TransColor && j < xs )
			{	j++;
				dp++;
				pixelCount++;

				if (pixelCount>=255)goto LABEL_SETTING;
			}

			if (pixelCount!=0)
			{
LABEL_SETTING:
				size += 2;	// 투명칼라가 아닌 데이타 (m byte)
			}
		}
	}

	return size;
}

int 
cTRANS::GetFontSize(BYTE *font)
{	int     i,j,addr=2,size=0;

	for(i=0; i<16; i++)
	{	addr++;		//	이미지 카운트(공백과 이미지를 한묶움으로..)가 들어갈 자리
		for (j=0;j<8;)
		{	size	=	0;
			while (!(font[i] & (0x80>>j)) && j < 8 ) j++;
			while ( (font[i] & (0x80>>j)) && j < 8 ) {size++,j++;}

			if (size != 0) 	addr += 2;			// 투명칼라가 아닌 데이타 (m byte)
		}
	}

	return addr;
}

int 
cTRANS::GetFontSize(WORD *font)
{	int     i,j,addr=2,size=0;

	for(i=0; i<16; i++)
	{	addr++;		//	이미지 카운트(공백과 이미지를 한묶움으로..)가 들어갈 자리
		for (j=0;j<16;)
		{	size	=	0;
			while (!(font[i]& (0x8000>>j)) && j < 16 ) j++;
			while ( (font[i]& (0x8000>>j)) && j < 16 ) {size++,j++;}

			if (size != 0) 	addr += 2;			// 투명칼라가 아닌 데이타 (m byte)
		}
	}

	return addr;
}

int
cTRANS::GetSize(WORD *sprite)
{
#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! in cTRANS::GetSize[code0007] 'sprite' is valid");
		return	NULL;
	}
#endif

	int   i,j,ys,count,BufferPoint=4;
	WORD  size;

	ys=(int )sprite[1];

	for(j=0;j<ys;j++)
	{	count=sprite[BufferPoint++];
   		for(i=0;i<count;i++)
		{	size		=	sprite[BufferPoint+1];
			BufferPoint	+=	2 + size;
		}
	}

	return BufferPoint;
}

int
cTRANS::GetSize(BYTE *sprite)
{
#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! in cTRANS::GetSize[code0008] 'sprite' is valid");
		return	NULL;
	}

#endif

	int   i,j,ys,count,BufferPoint=8;
	WORD  size;

	ys=*(short *)(sprite+2);

	for(j=0;j<ys;j++)
	{	count=sprite[BufferPoint++];
   		for(i=0;i<count;i++)
		{	size		=	sprite[BufferPoint+1];
			BufferPoint	+=	2 + size;
		}
	}

	return BufferPoint;
}

//#define	dMIRROR_WAR_TYPE

#ifdef dMIRROR_WAR_TYPE
WORD* 
cTRANS::ImageToSprite    (int xs,int ys,int xp,int yp,WORD TransColor,WORD *data,int &TotalSize)
{
	int     i, j, n, m,addr=4,dp=0,count=0,ad=0,size=0,offset;
	WORD	*spr;

	TotalSize=	GetSize(xs,ys,TransColor,data);

	spr		=	new WORD [TotalSize];
	spr[0]	=	(WORD ) xs;
	spr[1]	=	(WORD ) ys;
	spr[2]	=	(short) xp;
	spr[3]	=	(short) yp;

	for(i=0; i<ys; i++)
	{
		ad=addr++;		//	이미지 카운트(공백과 이미지를 한묶움으로..)가 들어갈 자리
		n=0;count=0;
		for (j=0;j<xs;)
		{
			m=0;
			while (data[dp] == TransColor && j < xs ) {n++,dp++,j++;}
			offset	=	n;
			while (data[dp] != TransColor && j < xs ) {n++,m++,j++,dp++;}
			if (m != 0)
			{
				spr[addr  ] = offset;	//	옵셋..
				spr[addr+1] = m  ;		//	투명칼라가 아닌 데이터의 수.. (byte..
				memcpy(spr+addr+2,data+(dp-m), m<<1);
				addr += 2 + m;			// 투명칼라가 아닌 데이타 (m byte)
				count++;
			}
		}
		spr[ad]=count;
	}

	return spr;
}

#else
WORD* 
cTRANS::ImageToSprite(int xs,int ys,int xp,int yp,WORD TransColor,WORD *data,int &TotalSize)
{
#ifdef dDEBUG_MODE
	if (!data)
	{	cMSG::Error("Error!! in cTRANS::ImageToSprite[code0009] pointer data is valid");
		return	NULL;
	}
#endif

	int     x, y,addr=4,dataPoint=0,budden=0,buddenPoint=0,size=0;
	WORD	*spr;

	TotalSize=	GetSize(xs,ys,TransColor,data);

	spr		=	new WORD [TotalSize];
	spr[0]	=	(WORD ) xs;
	spr[1]	=	(WORD ) ys;
	spr[2]	=	(short) xp;
	spr[3]	=	(short) yp;

	for(y=0; y<ys; y++)
	{	
		buddenPoint	=	addr++;		//	이미지 카운트(공백과 이미지를 한묶움으로..)가 들어갈 자리
		budden		=	0;
		for (x=0;x<xs;)
		{
			int	zeroCount	=	0,pixelCount	=	0;

			while (data[dataPoint] == TransColor && x < xs )
			{
				zeroCount++;
				dataPoint++,
				x++;
			}

			while (data[dataPoint] != TransColor && x < xs )
			{
				pixelCount++,
				dataPoint++;
				x++;
			}

			if (pixelCount != 0)
			{
				spr[addr  ] = zeroCount;	//	옵셋..
				spr[addr+1] = pixelCount;	//	투명칼라가 아닌 데이터의 수.. (byte..
				memcpy(spr+addr+2,data+(dataPoint-pixelCount), pixelCount<<1);
				addr		+= 2 + pixelCount;			// 투명칼라가 아닌 데이타 (m byte)
				budden++;
			}
		}

		spr[buddenPoint]	=	budden;
	}

	return spr;
}

#endif
BYTE*
cTRANS::ImageToSprite(int xs,int ys,int xp,int yp,BYTE TransColor,BYTE *data,int &TotalSize)
{
#ifdef dDEBUG_MODE
	if (!data)
	{	cMSG::Error("Error!! in cTRANS::ImageToSprite[code0010] 8bit pointer data is valid");
		return	NULL;
	}
#endif

	int     y, x, addr=8,dataPoint=0,budden=0,buddenAddress=0,size=0;
	BYTE	*spr;

	TotalSize	=	GetSize(xs,ys,TransColor,data);
	spr			=	new BYTE [TotalSize];

	*(short *)(spr  )= (short ) xs;
	*(short *)(spr+2)= (short ) ys;
	*(short *)(spr+4)= (short ) xp;
	*(short *)(spr+6)= (short ) yp;

	for(y=0; y<ys; y++)
	{	
		buddenAddress	=	addr++;		//	이미지 카운트(공백과 이미지를 한묶움으로..)가 들어갈 자리
		budden			=	0;

		for (x=0;x<xs;)
		{
			int pixelCount		=	0,zeroCount	=	0;
			int	iBeginDataPoint	=	dataPoint;
			int	iOx				=	x;

			while (data[dataPoint] == TransColor && x < xs )
			{
				dataPoint++;
				x++;
				zeroCount++;
			}

			if	(zeroCount	==	xs)
				goto LABEL_ZERO_SETTING;

			zeroCount	=	0;
			x			=	iOx;
			dataPoint	=	iBeginDataPoint;

			while (data[dataPoint] == TransColor && x < xs )
			{
				dataPoint++;
				x++;
				zeroCount++;

				if	(zeroCount >=255)
					goto LABEL_SETTING;
			}
			while (data[dataPoint] != TransColor && x < xs )
			{	
				dataPoint++;
				x++;
				pixelCount++;
				if (pixelCount >=255)
					goto LABEL_SETTING;
			}

			if	(pixelCount	!= 0)
			{
LABEL_SETTING:
				spr[addr  ]	=	zeroCount;		//	옵셋..
				spr[addr+1]	=	pixelCount;		//	투명칼라가 아닌 데이터의 수.. (byte..
				memcpy(spr+addr+2,data+(dataPoint-pixelCount), pixelCount);
				addr		+=	2+pixelCount;	// 투명칼라가 아닌 데이타 (m byte)
				budden++;
			}
		}
LABEL_ZERO_SETTING:
		spr[buddenAddress]	=	budden;
	}

	return spr;
}

BYTE*
cTRANS::ImageToShadow(int xs,int ys,int xp,int yp,WORD TransColor,WORD *data,int &FullSize)
{
#ifdef dDEBUG_MODE
	if (!data)
	{	cMSG::Error("Error!! in cTRANS::ImageToSprite[code0009] pointer data is valid");
		return	NULL;
	}
#endif

	int     y, x,addr=8,dataPoint=0,budden=0,buddenAddr=0;
	BYTE	*spr;

	FullSize=	GetShadowSize(xs,ys,TransColor,data);
	spr		=	new BYTE [FullSize];
	*(WORD *)(spr  )=	(WORD ) xs;
	*(WORD *)(spr+2)=	(WORD ) ys;
	*(WORD *)(spr+4)=	(short) xp;
	*(WORD *)(spr+6)=	(short) yp;

	for(y=0; y<ys; y++)
	{	buddenAddr	=	addr++;		//	이미지 카운트(공백과 이미지를 한묶움으로..)가 들어갈 자리
		budden		=	0;
		
		for (x=0;x<xs;)
		{	BYTE	zeroCount	=	0;
			BYTE	pixelCount	=	0;

			while (data[dataPoint] == TransColor && x < xs )
			{	zeroCount++;		//	빈거 카운트
				dataPoint++;
				x++;

				if (zeroCount >= 255)	goto LABEL_SETTING;	//	빈공간이 255를 넘으면 설정해 버린다.
			}

			while (data[dataPoint] != TransColor && x < xs )
			{	pixelCount++;
				dataPoint++;
				x++;

				if (pixelCount >= 255)	goto LABEL_SETTING;	//	픽셸이 255를 넘으면 설정해 버린다.
			}

			if (pixelCount != 0)
			{
LABEL_SETTING:
				spr[addr  ] = zeroCount;	//	0의 수
				spr[addr+1] = pixelCount;	//	투명칼라가 아닌 데이타 (pixelCount byte)
				addr		+= 2 ;
				budden++;					//	뭉탱이 수
			}
		}

		spr[buddenAddr]	=	budden;
	}

	return spr;
}

BYTE*
cTRANS::ImageToShadow(int xs,int ys,int xp,int yp,BYTE TransColor,BYTE *data,int &FullSize)
{
#ifdef dDEBUG_MODE
	if (!data)
	{	cMSG::Error("Error!! in cTRANS::ImageToSprite[code0009] pointer data is valid");
		return	NULL;
	}
#endif

	int     y, x,addr=8,dataPoint=0,budden=0,buddenAddr=0;
	BYTE	*spr;

	FullSize=	GetShadowSize(xs,ys,TransColor,data);
	spr		=	new BYTE [FullSize];
	*(WORD *)(spr  )=	(WORD ) xs;
	*(WORD *)(spr+2)=	(WORD ) ys;
	*(WORD *)(spr+4)=	(short) xp;
	*(WORD *)(spr+6)=	(short) yp;

	for(y=0; y<ys; y++)
	{	buddenAddr	=	addr++;		//	이미지 카운트(공백과 이미지를 한묶움으로..)가 들어갈 자리
		budden		=	0;
		
		for (x=0;x<xs;)
		{	BYTE	zeroCount	=	0;
			BYTE	pixelCount	=	0;

			while (data[dataPoint] == TransColor && x < xs )
			{	zeroCount++;		//	빈거 카운트
				dataPoint++;
				x++;

				if (zeroCount >= 255)	goto LABEL_SETTING;	//	빈공간이 255를 넘으면 설정해 버린다.
			}

			while (data[dataPoint] != TransColor && x < xs )
			{	pixelCount++;
				dataPoint++;
				x++;

				if (pixelCount >= 255)	goto LABEL_SETTING;	//	픽셸이 255를 넘으면 설정해 버린다.
			}

			if (pixelCount != 0)
			{
LABEL_SETTING:
				spr[addr  ] = zeroCount;	//	0의 수
				spr[addr+1] = pixelCount;	//	투명칼라가 아닌 데이타 (pixelCount byte)
				addr		+= 2 ;
				budden++;					//	뭉탱이 수
			}
		}

		spr[buddenAddr]	=	budden;
	}

	return spr;
}


BYTE *
cTRANS::BPP15To8(WORD *src,WORD *plt)
{
#ifdef dDEBUG_MODE
	if (!src)
	{	cMSG::Error("Error!! in cTRANS::BPP15To8[code0011] pointer 'src' is valid");
		return	NULL;
	}
	if (!plt)
	{	cMSG::Error("Error!! in cTRANS::BPP15To8[code0012] pointer 'plt' is valid");
		return	NULL;
	}
#endif

	int   i,j,k, xs,ys,count,BufferPoint=8,SrcPoint=4,XPoint,sprsize;
	BYTE  *spr;
	WORD  size;

	sprsize	=	GetSize(src);

	spr		=	new BYTE [sprsize+4];

	*(short *)(spr  )=*(short *)(src  );
	*(short *)(spr+2)=*(short *)(src+1);
	*(short *)(spr+4)=*(short *)(src+2);
	*(short *)(spr+6)=*(short *)(src+3);

	xs=(int )src[0];
	ys=(int )src[1];

	for(j=0;j<ys;j++)
	{	count				=src[SrcPoint++];
		spr[BufferPoint++]	=(BYTE)count;

   		for(i=0;i<count;i++)
		{	XPoint				=	src[SrcPoint++];
			size				=	src[SrcPoint++];
			spr[BufferPoint++]	=	XPoint;
			spr[BufferPoint++]	=	(BYTE)size;

			for (k=0;k<size;k++)
			{	WORD color=src[SrcPoint+k];
				for (int index=0;index<256;index++)
					if (plt[index] == color) spr[BufferPoint+k]	= (BYTE) index;
			}

			BufferPoint	+=	size;
			SrcPoint	+=	size;
		}
	}

	return spr;
}

WORD *
cTRANS::BPP8To15(BYTE *src,WORD *plt)
{
#ifdef dDEBUG_MODE
	if (!src)
	{	cMSG::Error("Error!! in cTRANS::BPP8To15[code0013] pointer 'src' is valid");
		return	NULL;
	}
	if (!plt)
	{	cMSG::Error("Error!! in cTRANS::BPP8To15[code0014] pointer 'plt' is valid");
		return	NULL;
	}
#endif

	int   i,j,k, xs,ys,count,BufferPoint=4,SrcPoint=8,XPoint,sprsize;
	WORD  *spr;
	BYTE  size;

	sprsize	=	GetSize(src);

	spr=new WORD [sprsize-4];
	*(short *)(spr  )=*(short *)(src  );
	*(short *)(spr+1)=*(short *)(src+2);
	*(short *)(spr+2)=*(short *)(src+4);
	*(short *)(spr+3)=*(short *)(src+6);

	xs=spr[0];
	ys=spr[1];

	for(j=0;j<ys;j++)
	{	count				=src[SrcPoint++];
		spr[BufferPoint++]	=(WORD)count;

		for(i=0;i<count;i++)
		{	XPoint				=	src[SrcPoint++];
			size				=	src[SrcPoint++];
			spr[BufferPoint++]	=	XPoint;
			spr[BufferPoint++]	=	size;

			for (k=0;k<size;k++)
				spr[BufferPoint+k]	=	plt[src[SrcPoint+k]];

			BufferPoint			+=	size;
			SrcPoint			+=	size;
		}
	}

	return spr;
}


WORD
cTRANS::RGB565ToBGR565(WORD color)
{	WORD r,g,b;
	r	=	(BYTE)(color>>11)&31;
	g	=	(BYTE)(color>>5 )&63;
	b	=	(BYTE)color&31;

	return (WORD) (  (((WORD)(b<<11))) | (((WORD)(g<<5))) | ((WORD)r)  );
}

WORD
cTRANS::BGR565ToRGB565(WORD color)
{	WORD r,g,b;
	r	=	(BYTE)(color>>11)&31;
	g	=	(BYTE)(color>>5 )&63;
	b	=	(BYTE)color&31;

	return (WORD) (  (((WORD)(b<<11))) | (((WORD)(g<<5))) | ((WORD)r)  );
}

WORD
cTRANS::RGB555ToRGB565(WORD color)
{	WORD r,g,b;
	r	=	(BYTE)(color>>10)&31;
	g	=	(BYTE)(color>>5 )&31;
	b	=	(BYTE)color&31;
	g	<<=	1;

	return (WORD) (  (((WORD)(r<<11))) | (((WORD)(g<<5))) | ((WORD)b)  );
}

WORD
cTRANS::RGB565ToRGB555(WORD color)
{	WORD r,g,b;
	r	=	(BYTE)(color>>11)&31;
	g	=	(BYTE)(color>>5 )&63;
	b	=	(BYTE)color&31;
	g	>>=	1;

	return 	(WORD) (  (((WORD)(r<<10))) | (((WORD)(g<<5))) | ((WORD)b)  );
}

WORD *
cTRANS::SpriteToImage(WORD *Spr,WORD TransColor)
{
#ifdef dDEBUG_MODE
	if (!Spr)
	{	cMSG::Error("Error!! in cTRANS::SpriteToImage[code0020] 8bit pointer 'Spr' is valid");
		return	NULL;
	}
#endif

	int   i,j;
	int   xs,ys,count,bp=4,ip=2,xp=0,size;
	WORD  *image;

	xs		=	Spr[0];
	ys		=	Spr[1];

	image	=	new WORD [xs*ys+2];

	image[0]=	(WORD)xs;
	image[1]=	(WORD)ys;

	memset(image+4,TransColor,xs*ys);
	
	for(j=0;j<ys;j++,ip+=xs)
	{	count	=	Spr[bp++];
		xp		=	0;

		for(i=0;i<count;i++)
		{	xp		+=	Spr[bp++];
			size	=	Spr[bp++];

			memcpy(image+ip+xp,Spr+bp,size*2);

			xp		+=	size;
			bp		+=	size;
		}
	}

	return image;
}

BYTE *
cTRANS::SpriteToImage(BYTE *Spr,BYTE TransColor)
{
#ifdef dDEBUG_MODE
	if (!Spr)
	{	cMSG::Error("Error!! in cTRANS::SpriteToImage[code0020] 8bit pointer 'Spr' is valid");
		return	NULL;
	}
#endif

	int   i,j;
	int   xs,ys,count,bp=8,ip=4,xp=0,size;
	BYTE  *image;

	xs		=	*(WORD *)(Spr  );
	ys		=	*(WORD *)(Spr+2);

	image	=	new BYTE [xs*ys+4];

	*(WORD *)(image		)	=	(WORD)xs;
	*(WORD *)(image+2	)	=	(WORD)ys;

	memset(image+4,TransColor,xs*ys);
	
	for(j=0;j<ys;j++,ip+=xs)
	{	count	=	Spr[bp++];
		xp		=	0;

		for(i=0;i<count;i++)
		{	xp		+=	Spr[bp++];
			size	=	Spr[bp++];

			memcpy(image+ip+xp,Spr+bp,size);

			xp		+=	size;
			bp		+=	size;
		}
	}

	return image;
}


BYTE *
cTRANS::FontToSprite(BYTE *font)
{
#ifdef dDEBUG_MODE
	if (!font)
	{	cMSG::Error("Error!! in cTRANS::FontToSprite[code0003] pointer data is valid");
		return	NULL;
	}
#endif

	int     i, j,addr=2,count=0,ad=0,size=0,offset;
	BYTE	*spr;

	spr=new BYTE [GetFontSize(font)];
	spr[0]= 8;
	spr[1]= 16;

	for(i=0; i<16; i++)
	{	ad=addr++;		//	이미지 카운트(공백과 이미지를 한묶움으로..)가 들어갈 자리
		offset=0;count=0;
		for (j=0;j<8;)
		{	int size=0;
			while (!(font[i]&(0x80>>j)) && j < 8 ) {offset++,j++;}
			while ( (font[i]&(0x80>>j)) && j < 8 ) {offset++,size++,j++;}
			if (size != 0)
			{	spr[addr  ] = offset-size;	//	옵셋..
				spr[addr+1] = size  ;		//	투명칼라가 아닌 데이터의 수.. (byte..
				addr += 2;
				count++;
			}
		}
		spr[ad]=count;
	}

	return spr;
}

BYTE *
cTRANS::FontToSprite(WORD *font)
{
#ifdef dDEBUG_MODE
	if (!font)
	{
		cMSG::Error("Error!! in cTRANS::FontToSprite[code0004] pointer data is valid");
		return	NULL;
	}
#endif

	int     i, j,addr=2,count=0,ad=0,size=0,offset;
	BYTE	*spr;

	spr=new BYTE [GetFontSize(font)];
	spr[0]= 16;
	spr[1]= 16;

	for(i=0; i<16; i++)
	{	ad=addr++;		//	이미지 카운트(공백과 이미지를 한묶움으로..)가 들어갈 자리
		offset=0;count=0;
		for (j=0;j<16;)
		{	WORD rfont=(font[i]<<8)|(font[i]>>8);
			int size=0;
			while (!(rfont&(0x8000>>j)) && j < 16 ) {offset++,j++;}
			while ( (rfont&(0x8000>>j)) && j < 16 ) {offset++,size++,j++;}
			if (size != 0)
			{	spr[addr  ] = offset-size;	//	옵셋..
				spr[addr+1] = size  ;		//	투명칼라가 아닌 데이터의 수.. (byte..
				addr += 2;					
				count++;
			}
		}
		spr[ad]=count;
	}

	return spr;
}

BOOL
cTRANS::FontToSprite(char *in_file,char *out_file)
{	int		file_size,i;
	FILE	*fp;
	BYTE	*data;
	int 	type;

	fp=fopen(in_file,"rb");
	if (fp==NULL) return cMSG::Put("ERROR!! in cTRANS :: FonToSprite[code0005]","'%s', file not Found",in_file);
	fseek(fp,0L,SEEK_END);
	file_size=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	if (file_size==4096	) type=1;
	else
	if (file_size==11520) type=2;
	else
	if (file_size==75200) type=3;
	else
	{	fclose(fp);
		return cMSG::Put("ERROR!! in cTRANS :: FonToSprite[code0006]","'%s' file의 형식을 알 수 없습니다.",in_file);
	}

	data=new BYTE [file_size];
	fread(data,file_size,1,fp);
	fclose(fp);

	if (type > 1)
	{	WORD *han_data=(WORD *)data;
		int font_number=file_size/32,Size=0;

		fp=fopen(out_file,"wb");
		fwrite(&Size,4,1,fp);			//	화일 사이즈 넣을 자리..
		if (type==2) fwrite(lpszJFONTReg,40,1,fp);	//	인식자..
		else		 fwrite(lpszWFONTReg,40,1,fp);	//	인식자..
		fwrite(&font_number,4,1,fp);	//	폰트 수..
		fwrite(&Size,4,1,fp);			//	첫번째 오프셋.. 0이다.

		for (i=0;i<font_number;i++)
		{	Size		+=	GetFontSize(han_data+i*16);
			fwrite(&Size,4,1,fp);		//	마지막 오프셋은 데이터 사이즈가 된다.
		}

		for (i=0;i<font_number;i++)
		{	BYTE *font;
			font=FontToSprite(han_data+i*16);
			Size=GetFontSize(han_data+i*16);
			fwrite(font,Size,1,fp);
			pKILL(font);
		}
		file_size=ftell(fp);
		fseek(fp,0L,SEEK_SET);
		fwrite(&file_size,4,1,fp);
		fclose(fp);
	}
	else
	{	int font_number=file_size/32,Size=0;

		fp=fopen(out_file,"wb");
		fwrite(&Size,4,1,fp);			//	화일 사이즈 넣을 자리..
		fwrite(lpszEFONTReg,40,1,fp);	//	인식자..
		fwrite(&font_number,4,1,fp);	//	폰트 수..
		fwrite(&Size,4,1,fp);			//	첫번째 오프셋.. 0이다.

		for (i=0;i<font_number;i++)
		{	Size		+=	GetFontSize(data+i*16);
			fwrite(&Size,4,1,fp);		//	마지막 오프셋은 데이터 사이즈가 된다.
		}

		for (i=0;i<font_number;i++)
		{	BYTE *font;
			font=FontToSprite(data+i*16);
			Size=GetFontSize(data+i*16);
			fwrite(font,Size,1,fp);
			pKILL(font);
		}
		file_size=ftell(fp);
		fseek(fp,0L,SEEK_SET);
		fwrite(&file_size,4,1,fp);
		fclose(fp);
	}
	pKILL(data);

	return TRUE;
}

int
cTRANS::GetSTFSize(WORD *sprite)
{	int xs,ys,SrcPoint=4,BufferPoint=2,count,XPoint,i,j,size;

	xs			=	sprite[0];
	ys			=	sprite[1];

	for(j=0;j<ys;j++)
	{	count				=sprite[SrcPoint++];
		BufferPoint++;

   		for(i=0;i<count;i++)
		{	XPoint				=	sprite[SrcPoint++];
			size				=	sprite[SrcPoint++];
			SrcPoint			+=	size;
			BufferPoint			+=	2;
		}
	}

	return BufferPoint;
}

BYTE *
cTRANS::SpriteToFont(WORD *sprite)
{	BYTE *font;

	int xs,ys,SrcPoint=4,BufferPoint=2,count,XPoint,i,j,size;

	font=new BYTE [GetSTFSize(sprite)];

	BufferPoint	=	2;
	SrcPoint	=	4;
	font[0]		=	(BYTE)sprite[0];
	font[1]		=	(BYTE)sprite[1];
	xs			=	sprite[0];
	ys			=	sprite[1];

	for(j=0;j<ys;j++)
	{	count				=sprite[SrcPoint++];
		font[BufferPoint++]	=(BYTE)count;

   		for(i=0;i<count;i++)
		{	XPoint				=	sprite[SrcPoint++];
			size				=	sprite[SrcPoint++];
			font[BufferPoint++]	=	XPoint;
			font[BufferPoint++]	=	(BYTE)size;
			SrcPoint			+=	size;
		}
	}

	return font;
}

BYTE *
cTRANS::LayerToFontLayer(WORD *sprite,int size)
{	BYTE *font;

	int xs,ys,SrcPoint=4,BufferPoint=2,count,XPoint,i,j;

	font	=	new BYTE [size-2];

	font[0]	=	(BYTE)sprite[0];
	font[1]	=	(BYTE)sprite[1];
	xs		=	sprite[0];
	ys		=	sprite[1];

	for(j=0;j<ys;j++)
	{	count				=sprite[SrcPoint++];
		font[BufferPoint++]	=(BYTE)count;

   		for(i=0;i<count;i++)
		{	XPoint				=	sprite[SrcPoint++];
			size				=	sprite[SrcPoint++];
			font[BufferPoint++]	=	XPoint;
			font[BufferPoint++]	=	(BYTE)size;
		}
	}

	return font;
}



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
{	WORD	r	=	color>>11;
	WORD	g	=	color&0x7e0;
	WORD	b	=	(color&31)<<11;

	return (r+g+b);
}

inline WORD
RGB565ToRGB555(WORD color)
{	WORD	r	=	(color&0xf800)>>1;
	WORD	g	=	(color&0x7c0)>>1;
	WORD	b	=	color&31;

	return (r+g+b);
}

inline WORD
ToRGB565(WORD color)
{	WORD	r	=	(color&0x7c00)<<1;
	WORD	g	=	(color&0x3e0)<<1;
	WORD	b	=	color&31;

	return (r+g+b);
}

inline WORD
ToBGR565(WORD color)
{	WORD	r	=	(color&0x7c00)>>10;
	WORD	g	=	(color&0x3e0)<<1;
	WORD	b	=	(color&31)<<11;

	return (r+g+b);
}

inline WORD
CONVERTPIXEL(WORD color)
{	if (_PixelFormat	==	dRGB565)	return	ToRGB565(color);
	if (_PixelFormat	==	dBGR565)	return	ToBGR565(color);

	return	color;
}

BOOL
CONVERTSPRITE(WORD *sprite)
{	if (_PixelFormat	==	dRGB555)	return TRUE;
	if (!sprite)	return	MSGOUT("Error!! in CONVERTSPRITE - pointer 'sprite' is valid");

	int   i,j,k,xs ,ys,count,BufferPoint=4,size;

	xs	=	sprite[0];
	ys	=	sprite[1];

	for(j=0;j<ys;j++)
	{	count	=	sprite[BufferPoint];
		BufferPoint++;

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

	if (_PixelFormat	==	dRGB565)
	{	for (i=0;i<size;i++)	image[i+2]	=	ToRGB565(image[i+2]);
	}
	else
	if (_PixelFormat	==	dBGR565)
	{	for (i=0;i<size;i++)	image[i+2]	=	ToBGR565(image[i+2]);
	}

	return TRUE;
}

BOOL
CONVERTPALETTE(WORD *plt,int range)
{	if (!plt)	return	MSGOUT("Error!! in CONVERTIMAGE - pointer 'plt' is valid");
	int	i;

	if (_PixelFormat	==	dRGB565)
	{	for (i=0;i<range;i++)	plt[i]	=	ToRGB565(plt[i]);
	}
	else
	if (_PixelFormat	==	dBGR565)
	{	for (i=0;i<range;i++)	plt[i]	=	ToBGR565(plt[i]);
	}

	return TRUE;
}

WORD*
CONVERTPALETTE(BYTE *plt)
{	WORD	*result;
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

WORD*
CONVERT8TO16(BYTE *buffer,BYTE *plt,int xs,int ys)
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