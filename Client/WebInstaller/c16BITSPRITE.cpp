#include "cDRAW.H"

/*******************************************************************************************

	16bit 압축 스프라이트 출력 루틴들..
	압축 스프라이트 형식

		16bit 압축 스프라이트..	----------------+
			가로	사이즈 2 byte				|
			세로	사이즈 2 byte				|
			가로 위치 교정 2 byte				|
			세로 위치 교정 2 byte				|
			데이터								|
				카운트(한 라인에 이미지 수)		|
					공백 크기					|
					이미지 크기					|
					이미지	--------------------+ -> 2 byte 괜히 16bit냐.. ==;
void	PutSpriteXX(int x,int y,WORD *sprite,...)
int	x,y
	:	이미지의 좌표
WORD *sprite
	:	이미지 데이터
...
	:	알파 블렌딩시 알파값



void	PutSprite(int x,int y,WORD *sprite,int alpha)
		그냥 출력해준다.
void	PutSpriteHB(int x,int y,WORD *sprite,int alpha)
		화면과 반투명처리해준다.
void	PutSpriteAB(int x,int y,WORD *sprite,int alpha)
		알파블렌딩 출력
void	PutSpriteAB(int x,int y,WORD *sprite,int alpha)
		주어진 알파값으로 스프라이트와 배경을 섞어 준다.
void	PutSpriteFX(int x,int y,WORD *sprite,int effect)
		주어진 이펙트로 스프라이트와 배경을 섞어 준다.

void	PutSpriteR(int x,int y,WORD *sprite,int alpha)
void	PutSpriteRHB(int x,int y,WORD *sprite,int alpha)
void	PutSpriteRAB(int x,int y,WORD *sprite,int alpha)
void	PutSpriteRFX(int x,int y,WORD *sprite,int effect)

		확대/축소 출력..

		위에서 알파값은 32 가  Max이며 이때 이미지가 원본 상태로 나타난다.
		0일때는 아무것도 출력 되지 않는다.

*******************************************************************************************/

void
c16BITSPRITE::PutSprite(int x,int y,WORD *sprite,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=4;
	int	  size;
#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code c16BITSPRITE_0001");
		return;
	}
#endif

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(short)sprite[2];	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

	if (x >= WIDTH) return;

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{	count=sprite[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=sprite[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (x<0)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];

		xPoint	=	x;

		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			if (size > beginX )
				memcpy(p2SCREEN+addr+xPoint+beginX,sprite+bufferPoint+beginX,(size-beginX)<<1);

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x;
		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..
			if (endX > 0 )
				memcpy(p2SCREEN+addr+xPoint,sprite+bufferPoint,endX<<1);

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x;

		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			memcpy(p2SCREEN+addr+xPoint,sprite+bufferPoint,(size<<1));
			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
}

void
c16BITSPRITE::PutSpriteColor(int x,int y,WORD *sprite,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=4;
	int	  size;
#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code c16BITSPRITE_0001");
		return;
	}
#endif

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(short)sprite[2];	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

	if (x >= WIDTH) return;

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{	count=sprite[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=sprite[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (x<0)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		xPoint	=	x;

		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint	< 0		)	beginX= -xPoint;			//	왼쪽 클리핑..
			if (size	> beginX)	memset16(p2SCREEN+addr+xPoint+beginX,(WORD)alpha,size-beginX);

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x;
		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size>  WIDTH)	endX	= WIDTH-xPoint;		//	오른쪽 클리핑..
			if (endX		>	0	)	memset16(p2SCREEN+addr+xPoint,(WORD)alpha,endX);

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x;

		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];
			memset16(p2SCREEN+addr+xPoint,(WORD)alpha,size);
			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
}


void
c16BITSPRITE::PutSpriteHB(int x,int y,WORD *sprite,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=4;
	int   size;
#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code c16BITSPRITE_0002");
		return;
	}
#endif

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(short)sprite[2];	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

	if (x >= WIDTH) return;

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{	count=sprite[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size		=	sprite[bufferPoint++];
			bufferPoint	+=	size;
		}
	}

	if (x<0)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x;
		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint+beginX];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+beginX];
			width		=size-beginX;

			for (j=0;j<width/2;j++)
				p4_scr[j]	=	(((p4_scr[j]&d4HM_MASK)>>1)+((p4_spr[j]&d4HM_MASK)>>1));

			if ((width&1) && width > 0)
				p2SCREEN[addr+xPoint+size-1]
					=((p2SCREEN[addr+xPoint+size-1]&d2HM_MASK)+(sprite[bufferPoint+size-1]&d2HM_MASK))>>1;

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];
		xPoint	=	x;

		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint];

			for (j=0;j<endX/2;j++)
				p4_scr[j]=(((p4_scr[j]&d4HM_MASK)>>1)+((p4_spr[j]&d4HM_MASK)>>1));
			if ((endX&1) && endX > 0)
				p2SCREEN[addr+xPoint+endX-1]
				=((p2SCREEN[addr+xPoint+endX-1]&d2HM_MASK)+(sprite[bufferPoint+endX-1]&d2HM_MASK))>>1;

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];
		xPoint	=	x;

		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint];

			for (j=0;j<size/2;j++)
				p4_scr[j]=(((p4_scr[j]&d4HM_MASK)>>1)+((p4_spr[j]&d4HM_MASK)>>1));
			if ( size&1 )
				p2SCREEN[addr+xPoint+size-1]
				=((p2SCREEN[addr+xPoint+size-1]&d2HM_MASK)+(sprite[bufferPoint+size-1]&d2HM_MASK))>>1;
			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
}

void
c16BITSPRITE::PutSpriteAB(int x,int y,WORD *sprite,int alpha)
{	if (alpha<=0 ) return;
	if (alpha>=32)
	{	PutSprite(x,y,sprite,alpha);
		return;
	}

	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=4;
	int	  size;
	int	  dest_alpha=32-alpha;
#ifdef dDEBUG_MODE
	if (!sprite )
	{	cMSG::Error("Error!! code c16BITSPRITE_0003");
		return;
	}
#endif
	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(short)sprite[2];	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

	if (x >= WIDTH) return;

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{	count=sprite[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=sprite[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (x<0)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x;

		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];	
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint+beginX];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+beginX];

			width		=size-beginX;

			for (j=0;j<width/2;j++)
				p4_scr[j]	=	MIX4BYTE(p4_spr[j],p4_scr[j],alpha,dest_alpha);

			if ((width&1) && width > 0)
				p2SCREEN[addr+xPoint+size-1]	=	
					MIX2BYTE(sprite[bufferPoint+size-1],p2SCREEN[addr+xPoint+size-1],alpha,dest_alpha);

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];
		xPoint	=	x;

		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];	
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint];

			for (j=0;j<endX/2;j++)
				p4_scr[j]	=	MIX4BYTE(p4_spr[j],p4_scr[j],alpha,dest_alpha);

			if ((endX&1)&& endX > 0)
				p2SCREEN[addr+xPoint+endX-1]	=	MIX2BYTE(sprite[bufferPoint+endX-1],p2SCREEN[addr+xPoint+endX-1],alpha,dest_alpha);

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];
		xPoint	=	x;

		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];	
			size	=	sprite[bufferPoint++];

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint];

			for (j=0;j<size/2;j++)
				p4_scr[j]	=	MIX4BYTE(p4_spr[j],p4_scr[j],alpha,dest_alpha);

			if (size&1)
				p2SCREEN[addr+xPoint+size-1]=	MIX2BYTE(sprite[bufferPoint+size-1],p2SCREEN[addr+xPoint+size-1],alpha,dest_alpha);

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
}

void
c16BITSPRITE::PutSpriteFX(int x,int y,WORD *sprite,int effect)
{
#ifdef dDEBUG_MODE
	if (!sprite || effect > dPUT_EXTREAM_LIGHT)
	{	cMSG::Error("Error!! code c16BITSPRITE_0004");
		return;
	}
#endif

	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=4;
	int   size;

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(short)sprite[2];	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

	if (x >= WIDTH) return;

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{	count=sprite[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=sprite[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (x<0)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];

		xPoint	=	x;
		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..

			for (j=beginX;j<size;j++)
				p2SCREEN[addr+xPoint+j]	=	FXMIX(sprite[bufferPoint+j],p2SCREEN[addr+xPoint+j],effect);

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		xPoint	=	x;
		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			for (j=0;j<endX;j++)
				p2SCREEN[addr+xPoint+j]	=	FXMIX(sprite[bufferPoint+j],p2SCREEN[addr+xPoint+j],effect);

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		xPoint	=	x;
		for(i=0;i<count;i++)
		{	xPoint	+=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			for (j=0;j<size;j++)
				p2SCREEN[addr+xPoint+j]	=	FXMIX(sprite[bufferPoint+j],p2SCREEN[addr+xPoint+j],effect);

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
}

/*******************************************************************************************

	압축 스프라이트 출력 루틴들.. 플립

	알고리즘.. 걍 거꾸로 찍어 준다..
	단지.. 더블워드 역전송을 위해..
	UINT color;
	*(video+address)=(color<<16)+(color>>16);
	과 같은 방법을 사용했다.

*******************************************************************************************/
void
c16BITSPRITE::fPutSprite(int x,int y,WORD *sprite,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=4;
	int   size;
#ifdef dDEBUG_MODE
	if (!sprite )
	{	cMSG::Error("Error!! code c16BITSPRITE_0005");
		return;
	}
#endif

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(xs-(short)sprite[2]);	//	위치 교정..
	y-=(short)sprite[3];		//	위치 교정..

	if (x >= WIDTH) return;

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{	count=sprite[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=sprite[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (x<0)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x+xs;

		for(i=0;i<count;i++)
		{	xPoint	-=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint	< size	) beginX= size-xPoint;			//	왼쪽 클리핑..

			width	=	size-beginX;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-width];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+(width&1)];

			for (j=0;j<width/2;j++)	p4_scr[j]=(p4_spr[width/2-1-j]<<16)+(p4_spr[width/2-1-j]>>16);
			if  ((width&1) && width > 0  ) p2SCREEN[addr+xPoint-1]=sprite[bufferPoint];

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x+xs;

		for(i=0;i<count;i++)
		{	xPoint	-=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	size;
			width	=	xPoint;	//	tail

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-size];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+size%2];

			if (xPoint	>  WIDTH)
			{	endX	= WIDTH-(xPoint-size);		//	오른쪽 클리핑..
				width	= WIDTH;
			}

			for (j=0;j<endX/2;j++		)	p4_scr[j]=(p4_spr[size/2-1-j]<<16)+(p4_spr[size/2-1-j]>>16);
			if  ((endX&1) && endX > 0	)	p2SCREEN[addr+width-1]=sprite[bufferPoint+size-endX];

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr	+=	WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x+xs;

		for(i=0;i<count;i++)
		{	xPoint	-=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-size+1];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+(size&1)];

			for (j=size/2-1,x1=0;j>=0;j--,x1++	)	p4_scr[x1]=(p4_spr[j]<<16)+(p4_spr[j]>>16);
			if  (size&1							)	p2SCREEN[addr+xPoint]=sprite[bufferPoint];

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr+=WIDTH;
	}
}

void
c16BITSPRITE::fPutSpriteColor(int x,int y,WORD *sprite,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=4;
	int   size;
	UINT  color=(alpha<<16)+alpha;
#ifdef dDEBUG_MODE
	if (!sprite )
	{	cMSG::Error("Error!! code c16BITSPRITE_0005");
		return;
	}
#endif

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(xs-(short)sprite[2]);	//	위치 교정..
	y-=(short)sprite[3];		//	위치 교정..

	if (x >= WIDTH) return;

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{	count=sprite[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=sprite[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (x<0)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x+xs;

		for(i=0;i<count;i++)
		{	xPoint	-=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint	< size	) beginX= size-xPoint;			//	왼쪽 클리핑..

			width	=size-beginX;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-width];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+(width&1)];

			for (j=0;j<width/2;j++		)	p4_scr[j]				=color;
			if  ((width&1)&& width > 0  )	p2SCREEN[addr+xPoint-1]	=alpha;

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x+xs;

		for(i=0;i<count;i++)
		{	xPoint	-=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	size;
			width	=	xPoint;	//	tail

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-size];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+size%2];

			if (xPoint	>  WIDTH)
			{	endX	= WIDTH-(xPoint-size);		//	오른쪽 클리핑..
				width	= WIDTH;
			}

			for (j=0;j<endX/2;j++	)	p4_scr[j]				=	color;
			if  ((endX&1) && endX>0	)	p2SCREEN[addr+width-1]	=	alpha;

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x+xs;

		for(i=0;i<count;i++)
		{	xPoint	-=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-size+1];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+(size&1)];

			for (j=size/2-1,x1=0;j>=0;j--,x1++	)	p4_scr[x1]				=	color;
			if  (size & 1						)	p2SCREEN[addr+xPoint]	=	alpha;

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr+=WIDTH;
	}
}

void
c16BITSPRITE::fPutSpriteHB(int x,int y,WORD *sprite,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=4;
	int  size;
#ifdef dDEBUG_MODE
	if (!sprite )
	{	cMSG::Error("Error!! code c16BITSPRITE_0006");
		return;
	}
#endif

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(xs-(short)sprite[2]);	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

	if (x >= WIDTH) return;

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{	count=sprite[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=sprite[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (x<0)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x+xs;

		for(i=0;i<count;i++)
		{	xPoint	-=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint	< size	) beginX= size-xPoint;			//	왼쪽 클리핑..

			width	=	size-beginX;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-width];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+(width&1)];

			for (j=0;j<width/2;j++)
				p4_scr[j]	=	(((p4_scr[j]&d4HM_MASK)>>1)+((((p4_spr[width/2-1-j]<<16)+(p4_spr[width/2-1-j]>>16))&d4HM_MASK)>>1));
			if  ((width&1) && width > 0 )
				p2SCREEN[addr+xPoint-1]=
					((p2SCREEN[addr+xPoint-1]&d2HM_MASK)+(sprite[bufferPoint]&d2HM_MASK))>>1;

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr	+=	WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x+xs;

		for(i=0;i<count;i++)
		{	xPoint	-=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	size;
			width	=	xPoint;		//tail

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-size];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+size%2];

			if (xPoint	>  WIDTH)
			{	endX	= WIDTH-(xPoint-size);		//	오른쪽 클리핑..	
				width	= WIDTH;
			}

			for (j=0;j<endX/2;j++)
				p4_scr[j]	=
				(	((p4_scr[j]&d4HM_MASK)>>1)	+
					((((p4_spr[size/2-1-j]<<16)+(p4_spr[size/2-1-j]>>16))&d4HM_MASK)>>1)
				);

			if  ((endX&1) && endX > 0)
				p2SCREEN[addr+width-1]
					=	((p2SCREEN[addr+width-1]&d2HM_MASK)+(sprite[bufferPoint+size-endX]&d2HM_MASK))>>1;

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr	+=	WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x+xs;

		for(i=0;i<count;i++)
		{	xPoint	-=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-size+1];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+(size&1)];

			for (j=size/2-1,x1=0;j>=0;j--,x1++)
				p4_scr[x1]	=	(((p4_scr[x1]&d4HM_MASK)>>1)+((((p4_spr[j]<<16)+(p4_spr[j]>>16))&d4HM_MASK)>>1));

			if  (size&1)
				p2SCREEN[addr+xPoint]
					=((p2SCREEN[addr+xPoint]&d2HM_MASK)+(sprite[bufferPoint]&d2HM_MASK))>>1;

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr	+=	WIDTH;
	}
}

void
c16BITSPRITE::fPutSpriteAB(int x,int y,WORD *sprite,int alpha)
{	if (alpha<=0 ) return;
	if (alpha>=32) fPutSprite(x,y,sprite,alpha);
#ifdef dDEBUG_MODE
	if (!sprite )
	{	cMSG::Error("Error!! code c16BITSPRITE_0007");
		return;
	}
#endif

	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=4;
	int   size;
	int	  dest_alpha=32-alpha;

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(xs-(short)sprite[2]);	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

	if (x >= WIDTH) return;

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{	count=sprite[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=sprite[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (x<0)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x+xs;

		for(i=0;i<count;i++)
		{	xPoint	-=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint	< size	) beginX= size-xPoint;			//	왼쪽 클리핑..

			width	=	size-beginX;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-width];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+(width&1)];

			for (j=0;j<width/2;j++)
				p4_scr[j]	=	fMIX4BYTE(p4_spr[width/2-1-j],p4_scr[j],alpha,dest_alpha);

			if  ((width&1) && width > 0 )
				p2SCREEN[addr+xPoint-1]	=	MIX2BYTE(sprite[bufferPoint],p2SCREEN[addr+xPoint-1],alpha,dest_alpha);

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x+xs;

		for(i=0;i<count;i++)
		{	xPoint	-=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			width	=	xPoint;		//	tail
			endX	=	size;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-size];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+size%2];

			if (xPoint	>  WIDTH)
			{	endX	= WIDTH-(xPoint-size);		//	오른쪽 클리핑..
				width	= WIDTH;
			}

			for (j=0;j<endX/2;j++)
				p4_scr[j]	=	fMIX4BYTE(p4_spr[size/2-1-j],p4_scr[j],alpha,dest_alpha);

			if  ((endX&1) && endX > 0 )
				p2SCREEN[addr+width-1]	=	MIX2BYTE(sprite[bufferPoint+size-endX],p2SCREEN[addr+width-1],alpha,dest_alpha);

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x+xs;

		for(i=0;i<count;i++)
		{	xPoint	-=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-size+1];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+(size&1)];

			for (j=size/2-1,x1=0;j>=0;j--,x1++)
				p4_scr[x1]	=	fMIX4BYTE(p4_spr[j],p4_scr[j],alpha,dest_alpha);

			if  ((size&1) && size > 0 )
				p2SCREEN[addr+xPoint]	=	MIX2BYTE(sprite[bufferPoint],p2SCREEN[addr+xPoint-1],alpha,dest_alpha);

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr+=WIDTH;
	}
}

void
c16BITSPRITE::fPutSpriteFX(int x,int y,WORD *sprite,int effect)
{
#ifdef dDEBUG_MODE
	if (!sprite || effect > dPUT_EXTREAM_LIGHT)
	{	cMSG::Error("Error!! code c16BITSPRITE_0008");
		return;
	}
#endif

	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=4;
	int   size;

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(xs-(short)sprite[2]);	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

	if (x >= WIDTH) return;

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{	count=sprite[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=sprite[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (x<0)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x+xs;

		for(i=0;i<count;i++)
		{	xPoint	-=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	size;
			if (xPoint - size	< 0		) endX	= xPoint;		//	왼쪽 클리핑..

			for (j=0;j<endX;j++)
				p2SCREEN[addr+xPoint-j]	=	FXMIX(sprite[bufferPoint+j],p2SCREEN[addr+xPoint-j],effect);

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x+xs;

		for(i=0;i<count;i++)
		{	xPoint	-=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;
			if (xPoint	> WIDTH	) beginX	= xPoint-WIDTH;	//	오른쪽 클리핑..

			for (j=beginX;j<size;j++)
				p2SCREEN[addr+xPoint-j]	=	FXMIX(sprite[bufferPoint+j],p2SCREEN[addr+xPoint-j],effect);

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		xPoint	=	x+xs;

		for(i=0;i<count;i++)
		{	xPoint	-=	sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			for (j=0;j<size;j++)
				p2SCREEN[addr+xPoint-j]	=	FXMIX(sprite[bufferPoint+j],p2SCREEN[addr+xPoint-j],effect);

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr+=WIDTH;
	}
}


/*********************************************************************************************************
	확대/축소 출력
*********************************************************************************************************/

//	확대/축소 테이블 생성	
//	실이미지의 x,y 각각의 좌표가 확대된 이미지의 주소에 대응한다.

void
c16BITSPRITE::PutSpriteR(int x,int y,WORD *sprite,int xRate,int yRate,int alpha)
{	if (xRate==100 && yRate==100) {PutSprite(x,y,sprite,0);	return;}
	if (xRate		< 0			) {fPutSpriteR(x,y,sprite,-xRate,yRate,alpha);return;}

	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=4,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code c16BITSPRITE_0009");
		return;
	}
#endif

	xs	=(short)sprite[0]			,ys		=(short)sprite[1]			;	//	스프라이트 사이즈
	xLen=xs*xRate/100				,yLen	=ys*yRate/100				;	//	확대/축소된 이미즈 사이즈
	x	-=(short)sprite[2]*xRate/100,y		-=(short)sprite[3]*yRate/100;	//	위치 교정..
	if (x + xLen < 0 || x >= WIDTH || y + yLen < 0 || y > HEIGHT )  return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(x,y,xs,ys,xLen,yLen);

	addr=max(y,0)*WIDTH+x;			//	스크린 주소..

	for (y=0;y<ys;y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (yp=0;yp<zoomYTable[y].size;yp++)
		{	count	=	sprite[bufferPoint++];	//	이미지 카운트
			xp		=	0;
			for(i=0;i<count;i++)
			{	xp	+=	sprite[bufferPoint++];
				size=	sprite[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+xp].size;k++)
						*(p2SCREEN+addr+zoomXTable[j+xp].pos+k)=*(sprite+bufferPoint+j);
				bufferPoint	+=	size;
				xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	sprite[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	sprite[bufferPoint+1] + 2;
	}
}

void
c16BITSPRITE::PutSpriteRColor(int x,int y,WORD *sprite,int xRate,int yRate,int alpha)
{	if (xRate==100 && yRate==100) {PutSpriteColor(x,y,sprite,alpha);	return;}
	if (xRate		< 0			) {fPutSpriteRColor(x,y,sprite,-xRate,yRate,alpha);return;}

	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=4,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code c16BITSPRITE_0009");
		return;
	}
#endif

	xs	=(short)sprite[0]			,ys		=(short)sprite[1]			;	//	스프라이트 사이즈
	xLen=xs*xRate/100				,yLen	=ys*yRate/100				;	//	확대/축소된 이미즈 사이즈
	x	-=(short)sprite[2]*xRate/100,y		-=(short)sprite[3]*yRate/100;	//	위치 교정..
	if (x + xLen < 0 || x >= WIDTH || y + yLen < 0 || y > HEIGHT )  return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(x,y,xs,ys,xLen,yLen);

	addr=max(y,0)*WIDTH+x;			//	스크린 주소..

	for (y=0;y<ys;y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (yp=0;yp<zoomYTable[y].size;yp++)
		{	count	=	sprite[bufferPoint++];	//	이미지 카운트
			xp		=	0;
			for(i=0;i<count;i++)
			{	xp	=	sprite[bufferPoint++];
				size=	sprite[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+xp].size;k++)
						*(p2SCREEN+addr+zoomXTable[j+xp].pos+k)=alpha;
				bufferPoint	+=	size;
				xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	sprite[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	sprite[bufferPoint+1] + 2;
	}
}

void
c16BITSPRITE::PutSpriteRHB(int x,int y,WORD *sprite,int xRate,int yRate,int alpha)
{	if (xRate==100 && yRate==100) {PutSpriteHB(x,y,sprite,0);	return;}
	if (xRate		< 0			) {fPutSpriteRHB(x,y,sprite,-xRate,yRate,alpha);return;}

	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=4,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code c16BITSPRITE_0010");
		return;
	}
#endif

	xs	=(short)sprite[0]			,ys		=(short)sprite[1]			;	//	스프라이트 사이즈
	xLen=xs*xRate/100				,yLen	=ys*yRate/100				;	//	확대/축소된 이미즈 사이즈
	x	-=(short)sprite[2]*xRate/100,y		-=(short)sprite[3]*yRate/100;	//	위치 교정..
	if (x + xLen < 0 || x >= WIDTH || y + yLen < 0 || y > HEIGHT )  return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(x,y,xs,ys,xLen,yLen);

	addr=max(y,0)*WIDTH+x;			//	스크린 주소..

	for (y=0;y<ys;y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (yp=0;yp<zoomYTable[y].size;yp++)
		{	count	=	sprite[bufferPoint++];	//	이미지 카운트
			xp		=	0;
			for(i=0;i<count;i++)
			{	xp	+=	sprite[bufferPoint++];
				size=	sprite[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+xp].size;k++)
						*(p2SCREEN+addr+zoomXTable[j+xp].pos+k)
						=((p2SCREEN[addr+zoomXTable[j+xp].pos+k]&d2HM_MASK)+(sprite[bufferPoint+j]&d2HM_MASK))>>1;

				bufferPoint	+=	size;
				xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	sprite[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	sprite[bufferPoint+1] + 2;
	}
}

void
c16BITSPRITE::PutSpriteRAB(int x,int y,WORD *sprite,int xRate,int yRate,int alpha)
{	if (xRate==100 && yRate==100) {PutSpriteAB(x,y,sprite,alpha);	return;}
	if (xRate		< 0			) {fPutSpriteRAB(x,y,sprite,-xRate,yRate,alpha);return;}
	if (alpha<=0 ) return;
	if (alpha>=32)
	{	PutSpriteR(x,y,sprite,xRate,yRate,alpha);
		return;
	}

	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=4,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..
	int	dest_alpha=32-alpha;

#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code c16BITSPRITE_0011");
		return;
	}
#endif

	xs	=(short)sprite[0]			,ys		=(short)sprite[1]			;	//	스프라이트 사이즈
	xLen=xs*xRate/100				,yLen	=ys*yRate/100				;	//	확대/축소된 이미즈 사이즈
	x	-=(short)sprite[2]*xRate/100,y		-=(short)sprite[3]*yRate/100;	//	위치 교정..
	if (x + xLen < 0 || x >= WIDTH || y + yLen < 0 || y > HEIGHT )  return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(x,y,xs,ys,xLen,yLen);

	addr=max(y,0)*WIDTH+x;			//	스크린 주소..

	for (y=0;y<ys;y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (yp=0;yp<zoomYTable[y].size;yp++)
		{	count	=	sprite[bufferPoint++];	//	이미지 카운트
			xp		=	0;
			for(i=0;i<count;i++)
			{	xp	+=	sprite[bufferPoint++];
				size=	sprite[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+xp].size;k++)
						p2SCREEN[addr+zoomXTable[j+xp].pos+k]	
							=	MIX2BYTE(sprite[bufferPoint+j],p2SCREEN[addr+zoomXTable[j+xp].pos+k],alpha,dest_alpha);

				bufferPoint	+=	size;
				xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	sprite[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	sprite[bufferPoint+1] + 2;
	}
}

void
c16BITSPRITE::PutSpriteRFX(int x,int y,WORD *sprite,int xRate,int yRate,int effect)
{	if (xRate==100 && yRate==100) {PutSpriteFX(x,y,sprite,effect);	return;}
	if (xRate		< 0			) {fPutSpriteRFX(x,y,sprite,-xRate,yRate,effect);return;}

	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=4,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code c16BITSPRITE_0012");
		return;
	}
#endif

	xs	=(short)sprite[0]			,ys		=(short)sprite[1]			;	//	스프라이트 사이즈
	xLen=xs*xRate/100				,yLen	=ys*yRate/100				;	//	확대/축소된 이미즈 사이즈
	x	-=(short)sprite[2]*xRate/100,y		-=(short)sprite[3]*yRate/100;	//	위치 교정..
	if (x + xLen < 0 || x >= WIDTH || y + yLen < 0 || y > HEIGHT )  return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(x,y,xs,ys,xLen,yLen);

	addr=max(y,0)*WIDTH+x;			//	스크린 주소..

	for (y=0;y<ys;y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (yp=0;yp<zoomYTable[y].size;yp++)
		{	count	=	sprite[bufferPoint++];	//	이미지 카운트
			xp		=	0;
			for(i=0;i<count;i++)
			{	xp	+=	sprite[bufferPoint++];
				size=	sprite[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+xp].size;k++)
						p2SCREEN[addr+zoomXTable[j+xp].pos+k]	
							=	FXMIX(sprite[bufferPoint+j],p2SCREEN[addr+zoomXTable[j+xp].pos+k],effect);

				bufferPoint	+=	size;
				xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	sprite[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	sprite[bufferPoint+1] + 2;
	}
}

void
c16BITSPRITE::fPutSpriteR(int x,int y,WORD *sprite,int xRate,int yRate,int alpha)
{	if (xRate==100 && yRate==100) {fPutSprite(x,y,sprite,0);	return;}
	if (xRate		< 0			) {PutSpriteR(x,y,sprite,-xRate,yRate,alpha);return;}

	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=4,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code c16BITSPRITE_0013");
		return;
	}
#endif

	xs	=(short)sprite[0]				,ys		=(short)sprite[1]			;	//	스프라이트 사이즈
	xLen=xs*xRate/100					,yLen	=ys*yRate/100				;	//	확대/축소된 이미즈 사이즈
	x	-=(xs-(short)sprite[2])*xRate/100,y		-=(short)sprite[3]*yRate/100;	//	위치 교정..
	if (x + xLen < 0 || x >= WIDTH || y + yLen < 0 || y > HEIGHT )  return;	//	이미지가 화면에 없다.

	fMakeSpriteRTable(x,y,xs,ys,xLen,yLen);

	addr=max(y,0)*WIDTH+x;			//	스크린 주소..

	for (y=0;y<ys;y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (yp=0;yp<zoomYTable[y].size;yp++)
		{	count	=	sprite[bufferPoint++];	//	이미지 카운트
			xp		=	0;
			for(i=0;i<count;i++)
			{	xp	+=	sprite[bufferPoint++];
				size=	sprite[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+xp].size;k++)
						*(p2SCREEN+addr+zoomXTable[xp+j].pos-k-1)=*(sprite+bufferPoint+j);
				bufferPoint	+=	size;
				xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	sprite[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	sprite[bufferPoint+1] + 2;
	}
}

void
c16BITSPRITE::fPutSpriteRColor(int x,int y,WORD *sprite,int xRate,int yRate,int alpha)
{	if (xRate==100 && yRate==100) {fPutSpriteColor(x,y,sprite,alpha);	return;}
	if (xRate		< 0			) {PutSpriteRColor(x,y,sprite,-xRate,yRate,alpha);return;}

	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=4,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code c16BITSPRITE_0013");
		return;
	}
#endif

	xs	=(short)sprite[0]				,ys		=(short)sprite[1]			;	//	스프라이트 사이즈
	xLen=xs*xRate/100					,yLen	=ys*yRate/100				;	//	확대/축소된 이미즈 사이즈
	x	-=(xs-(short)sprite[2])*xRate/100,y		-=(short)sprite[3]*yRate/100;	//	위치 교정..
	if (x + xLen < 0 || x >= WIDTH || y + yLen < 0 || y > HEIGHT )  return;	//	이미지가 화면에 없다.

	fMakeSpriteRTable(x,y,xs,ys,xLen,yLen);

	addr=max(y,0)*WIDTH+x;			//	스크린 주소..

	for (y=0;y<ys;y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (yp=0;yp<zoomYTable[y].size;yp++)
		{	count	=	sprite[bufferPoint++];	//	이미지 카운트
			xp		=	0;
			for(i=0;i<count;i++)
			{	xp	+=	sprite[bufferPoint++];
				size=	sprite[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+xp].size;k++)
						*(p2SCREEN+addr+zoomXTable[xp+j].pos-k-1)=alpha;
				bufferPoint	+=	size;
				xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	sprite[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	sprite[bufferPoint+1] + 2;
	}
}

void
c16BITSPRITE::fPutSpriteRHB(int x,int y,WORD *sprite,int xRate,int yRate,int alpha)
{	if (xRate==100 && yRate==100) {fPutSpriteHB(x,y,sprite,0);	return;}
	if (xRate		< 0			) {PutSpriteRHB(x,y,sprite,-xRate,yRate,alpha);return;}

	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=4,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code c16BITSPRITE_0014");
		return;
	}
#endif

	xs	=(short)sprite[0]			,ys		=(short)sprite[1]			;	//	스프라이트 사이즈
	xLen=xs*xRate/100				,yLen	=ys*yRate/100				;	//	확대/축소된 이미즈 사이즈
	x	-=(xs-(short)sprite[2])*xRate/100,y		-=(short)sprite[3]*yRate/100;	//	위치 교정..
	if (x + xLen < 0 || x >= WIDTH || y + yLen < 0 || y > HEIGHT )  return;	//	이미지가 화면에 없다.

	fMakeSpriteRTable(x,y,xs,ys,xLen,yLen);

	addr=max(y,0)*WIDTH+x;			//	스크린 주소..

	for (y=0;y<ys;y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (yp=0;yp<zoomYTable[y].size;yp++)
		{	count	=	sprite[bufferPoint++];	//	이미지 카운트
			xp		=	0;
			for(i=0;i<count;i++)
			{	xp	+=	sprite[bufferPoint++];
				size=	sprite[bufferPoint++];

				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+xp].size;k++)
						*(p2SCREEN+addr+zoomXTable[j+xp].pos-k-1)
							=((p2SCREEN[addr+zoomXTable[j+xp].pos-k-1]&d2HM_MASK)+(sprite[bufferPoint+j]&d2HM_MASK))>>1;

				bufferPoint	+=	size;
				xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	sprite[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	sprite[bufferPoint+1] + 2;
	}
}

void
c16BITSPRITE::fPutSpriteRAB(int x,int y,WORD *sprite,int xRate,int yRate,int alpha)
{	if (xRate==100	&& yRate==100	) {fPutSpriteAB(x,y,sprite,alpha);	return;}
	if (xRate		< 0				) {PutSpriteRAB(x,y,sprite,-xRate,yRate,alpha);return;}

	if (alpha<=0 ) return;
	if (alpha>=32)
	{	PutSpriteR(x,y,sprite,xRate,yRate,alpha);
		return;
	}

	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=4,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..
	int	dest_alpha=32-alpha;

#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code c16BITSPRITE_0015");
		return;
	}
#endif

	xs	=(short)sprite[0]			,ys		=(short)sprite[1]			;	//	스프라이트 사이즈
	xLen=xs*xRate/100				,yLen	=ys*yRate/100				;	//	확대/축소된 이미즈 사이즈
	x	-=(xs-(short)sprite[2])*xRate/100,y		-=(short)sprite[3]*yRate/100;	//	위치 교정..
	if (x + xLen < 0 || x >= WIDTH || y + yLen < 0 || y > HEIGHT )  return;	//	이미지가 화면에 없다.

	fMakeSpriteRTable(x,y,xs,ys,xLen,yLen);

	addr=max(y,0)*WIDTH+x;			//	스크린 주소..

	for (y=0;y<ys;y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (yp=0;yp<zoomYTable[y].size;yp++)
		{	count	=	sprite[bufferPoint++];	//	이미지 카운트
			xp		=	0;

			for(i=0;i<count;i++)
			{	xp	+=	sprite[bufferPoint++];
				size=	sprite[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+xp].size;k++)
						p2SCREEN[addr+zoomXTable[j+xp].pos-1-k]	
							=	MIX2BYTE(sprite[bufferPoint+j],p2SCREEN[addr+zoomXTable[j+xp].pos-1-k],alpha,dest_alpha);
				bufferPoint	+=	size;
				xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	sprite[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	sprite[bufferPoint+1] + 2;
	}
}

void
c16BITSPRITE::fPutSpriteRFX(int x,int y,WORD *sprite,int xRate,int yRate,int effect)
{	if (xRate==100	&& yRate==100	) {fPutSpriteFX(x,y,sprite,effect);	return;}
	if (xRate		<	0			) {PutSpriteRFX(x,y,sprite,-xRate,yRate,effect);return;}

	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=4,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! code c16BITSPRITE_0016");
		return;
	}
#endif

	xs	=(short)sprite[0]			,ys		=(short)sprite[1]			;	//	스프라이트 사이즈
	xLen=xs*xRate/100				,yLen	=ys*yRate/100				;	//	확대/축소된 이미즈 사이즈
	x	-=(xs-(short)sprite[2])*xRate/100,y		-=(short)sprite[3]*yRate/100;	//	위치 교정..
	if (x + xLen < 0 || x >= WIDTH || y + yLen < 0 || y > HEIGHT )  return;	//	이미지가 화면에 없다.

	fMakeSpriteRTable(x,y,xs,ys,xLen,yLen);

	addr=max(y,0)*WIDTH+x;			//	스크린 주소..

	for (y=0;y<ys;y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (yp=0;yp<zoomYTable[y].size;yp++)
		{	count	=	sprite[bufferPoint++];	//	이미지 카운트
			xp		=	0;
			for(i=0;i<count;i++)
			{	xp	+=	sprite[bufferPoint++];
				size=	sprite[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+xp].size;k++)
						p2SCREEN[addr+zoomXTable[j+xp].pos-k-1]	
							=	FXMIX(sprite[bufferPoint+j],p2SCREEN[addr+zoomXTable[j+xp].pos-k-1],effect);
				bufferPoint	+=	size;
				xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	sprite[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	sprite[bufferPoint+1] + 2;
	}
}