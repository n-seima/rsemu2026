#include "cDRAW.H"

/*******************************************************************************************

	8bit 압축 스프라이트 출력 루틴들..
	압축 스프라이트 형식

		8bit 압축 스프라이트..
			가로	사이즈 2 byte  ---+
			세로	사이즈 2 byte     |
			가로 위치 교정 2 byte     |
			세로 위치 교정 2 byte  ---+ -> *(short *)
			데이터						--------+
				카운트(한 라인에 이미지 수)		|
					공백 크기					|
					이미지 크기					|
					이미지				--------+ -> 1 byte 이러려면..

void	PutSpriteXX(int x,int y,BYTE *sprite,WORD *plt,int alpha)
int	x,y
	:	이미지의 좌표
BYTE *sprite
	:	이미지 데이터
WORD *plt
	:	팔레트
int alpha
	:	알파 블렌딩시 알파값

*******************************************************************************************/

//	Error code c8BITSPRITE

void
c8BITSPRITE::PutSprite(int x,int y,BYTE *sprite,WORD *plt,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;

#ifdef dDEBUG_MODE
	if (!sprite || !plt)
	{	cMSG::Error("Error!! code c8BITSPRITE_0001");
		return;
	}
#endif

	xs=*(short *)(sprite  );
	ys=*(short *)(sprite+2);
	x-=*(short *)(sprite+4);	//	위치 교정..
	y-=*(short *)(sprite+6);	//	위치 교정..

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
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];

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
			for (j=0;j<endX;j++)
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];

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

			for (j=0;j<size;j++)
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];
			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
}

void
c8BITSPRITE::PutSpriteColor(int x,int y,BYTE *sprite,WORD *plt,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;
	WORD  Color	=	(WORD)alpha;

#ifdef dDEBUG_MODE
	if (!sprite || !plt)
	{	cMSG::Error("Error!! code c8BITSPRITE_0001");
		return;
	}
#endif

	xs=*(short *)(sprite  );
	ys=*(short *)(sprite+2);
	x-=*(short *)(sprite+4);	//	위치 교정..
	y-=*(short *)(sprite+6);	//	위치 교정..

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

			if (xPoint	< 0		)	beginX	= -xPoint;			//	왼쪽 클리핑..
			if (size	> beginX)	memset(p2SCREEN+addr+xPoint+j,Color,size-beginX);

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

			if (xPoint+ size	>  WIDTH	) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..
			if (endX			>	0		)	memset(p2SCREEN+addr+xPoint+j,Color,endX);

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

			memset(p2SCREEN+addr+xPoint+j,Color,size);
			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
}


void
c8BITSPRITE::PutSpriteHB(int x,int y,BYTE *sprite,WORD *plt,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;
#ifdef dDEBUG_MODE
	if (!sprite  || !plt)
	{	cMSG::Error("Error!! code c8BITSPRITE_0002");
		return;
	}
#endif

	xs=*(short *)(sprite  );
	ys=*(short *)(sprite+2);
	x-=*(short *)(sprite+4);	//	위치 교정..
	y-=*(short *)(sprite+6);	//	위치 교정..

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
				p2SCREEN[addr+xPoint+j]
					=((p2SCREEN[addr+xPoint+j]&d2HM_MASK)+(plt[sprite[bufferPoint+j]]&d2HM_MASK))>>1;

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
			for (j=0;j<endX;j++)
				p2SCREEN[addr+xPoint+j]
					=((p2SCREEN[addr+xPoint+j]&d2HM_MASK)+(plt[sprite[bufferPoint+j]]&d2HM_MASK))>>1;

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

			for (j=0;j<size;j++)
				p2SCREEN[addr+xPoint+j]
					=((p2SCREEN[addr+xPoint+j]&d2HM_MASK)+(plt[sprite[bufferPoint+j]]&d2HM_MASK))>>1;
			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
}

void
c8BITSPRITE::PutSpriteAB(int x,int y,BYTE *sprite,WORD *plt,int alpha)
{	if (alpha<=0 ) return;
	if (alpha>=32)
	{	PutSprite(x,y,sprite,plt,alpha);
		return;
	}

#ifdef dDEBUG_MODE
	if (!sprite  || !plt)
	{	cMSG::Error("Error!! code c8BITSPRITE_0003");
		return;
	}
#endif

	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;
	int	  dest_alpha=32-alpha;

	xs=*(short *)(sprite  );
	ys=*(short *)(sprite+2);
	x-=*(short *)(sprite+4);	//	위치 교정..
	y-=*(short *)(sprite+6);	//	위치 교정..

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
				p2SCREEN[addr+xPoint+j]	=	MIX2BYTE(plt[sprite[bufferPoint+j]],p2SCREEN[addr+xPoint+j],alpha,dest_alpha);

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

			for (j=0;j<endX;j++)
				p2SCREEN[addr+xPoint+j]	=	MIX2BYTE(plt[sprite[bufferPoint+j]],p2SCREEN[addr+xPoint+j],alpha,dest_alpha);

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

			for (j=0;j<size;j++)
				p2SCREEN[addr+xPoint+j]	=	MIX2BYTE(plt[sprite[bufferPoint+j]],p2SCREEN[addr+xPoint+j],alpha,dest_alpha);

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
}

void
c8BITSPRITE::PutSpriteFX(int x,int y,BYTE *sprite,WORD *plt,int effect)
{
#ifdef dDEBUG_MODE
	if (!sprite  || !plt || effect > dPUT_EXTREAM_LIGHT)
	{	cMSG::Error("Error!! code c8BITSPRITE_0004");
		return;
	}
#endif

	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;

	xs=*(short *)(sprite  );
	ys=*(short *)(sprite+2);
	x-=*(short *)(sprite+4);	//	위치 교정..
	y-=*(short *)(sprite+6);	//	위치 교정..

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
				p2SCREEN[addr+xPoint+j]	=	FXMIX(plt[sprite[bufferPoint+j]],p2SCREEN[addr+xPoint+j],effect);

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

			for (j=0;j<endX;j++)
				p2SCREEN[addr+xPoint+j]	=	FXMIX(plt[sprite[bufferPoint+j]],p2SCREEN[addr+xPoint+j],effect);

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

			for (j=0;j<size;j++)
				p2SCREEN[addr+xPoint+j]	=	FXMIX(plt[sprite[bufferPoint+j]],p2SCREEN[addr+xPoint+j],effect);

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
		addr+=WIDTH;
	}
}

/*** 압축 스프라이트 출력 루틴들.. 플립**/

void
c8BITSPRITE::fPutSprite(int x,int y,BYTE *sprite,WORD *plt,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;
#ifdef dDEBUG_MODE
	if (!sprite  || !plt)
	{	cMSG::Error("Error!! code c8BITSPRITE_0005");
		return;
	}
#endif

	xs=*(short *)(sprite  );
	ys=*(short *)(sprite+2);
	x-=(xs-*(short *)(sprite+4));	//	위치 교정..
	y-=*(short *)(sprite+6);	//	위치 교정..

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
			if (xPoint- size	< 0		) endX	= xPoint;		//	왼쪽 클리핑..
			for (j=0;j<endX;j++) *(p2SCREEN+addr+xPoint-j)=plt[*(sprite+bufferPoint+j)];

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
			if (xPoint	> WIDTH	) beginX= xPoint-WIDTH;	//	오른쪽 클리핑..

			for (j=beginX;j<size;j++) *(p2SCREEN+addr+xPoint-j)=plt[*(sprite+bufferPoint+j)];

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

			for (j=0;j<size;j++) *(p2SCREEN+addr+xPoint-j)=plt[*(sprite+bufferPoint+j)];
			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr+=WIDTH;
	}
}

void
c8BITSPRITE::fPutSpriteColor(int x,int y,BYTE *sprite,WORD *plt,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;
	WORD  Color	=	(WORD)alpha;

#ifdef dDEBUG_MODE
	if (!sprite  || !plt)
	{	cMSG::Error("Error!! code c8BITSPRITE_0005");
		return;
	}
#endif

	xs=*(short *)(sprite  );
	ys=*(short *)(sprite+2);
	x-=(xs-*(short *)(sprite+4));	//	위치 교정..
	y-=*(short *)(sprite+6);	//	위치 교정..

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
			if (xPoint- size	<	0)	endX	= xPoint;		//	왼쪽 클리핑..
			if (endX			>	0)	memset(p2SCREEN+addr+xPoint-j,Color,endX);

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
			if (xPoint	> WIDTH	)	beginX= xPoint-WIDTH;	//	오른쪽 클리핑..
			if (size	> beginX)	memset(p2SCREEN+addr+xPoint-j,Color,size - beginX);

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

			memset(p2SCREEN+addr+xPoint-j,Color,size);

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr+=WIDTH;
	}
}

void
c8BITSPRITE::fPutSpriteHB(int x,int y,BYTE *sprite,WORD *plt,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;
#ifdef dDEBUG_MODE
	if (!sprite  || !plt)
	{	cMSG::Error("Error!! code c8BITSPRITE_0006");
		return;
	}
#endif

	xs=*(short *)(sprite  );
	ys=*(short *)(sprite+2);
	x-=(xs-*(short *)(sprite+4));	//	위치 교정..
	y-=*(short *)(sprite+6);	//	위치 교정..

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
			if (xPoint- size	< 0		) endX	= xPoint;		//	왼쪽 클리핑..

			for (j=0;j<endX;j++)
				p2SCREEN[addr+xPoint-j]
					=((p2SCREEN[addr+xPoint-j]&d2HM_MASK)+(plt[sprite[bufferPoint+j]]&d2HM_MASK))>>1;

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
			if (xPoint	> WIDTH	) beginX= xPoint-WIDTH;	//	오른쪽 클리핑..

			for (j=beginX;j<size;j++)
				p2SCREEN[addr+xPoint-j]
					=((p2SCREEN[addr+xPoint-j]&d2HM_MASK)+(plt[sprite[bufferPoint+j]]&d2HM_MASK))>>1;

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
				p2SCREEN[addr+xPoint-j]
					=((p2SCREEN[addr+xPoint-j]&d2HM_MASK)+(plt[sprite[bufferPoint+j]]&d2HM_MASK))>>1;

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr+=WIDTH;
	}
}

void
c8BITSPRITE::fPutSpriteAB(int x,int y,BYTE *sprite,WORD *plt,int alpha)
{	if (alpha<=0 ) return;
	if (alpha>=32) fPutSprite(x,y,sprite,plt,alpha);
#ifdef dDEBUG_MODE
	if (!sprite  || !plt )
	{	cMSG::Error("Error!! code c8BITSPRITE_0007");
		return;
	}
#endif

	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;
	int	  dest_alpha=32-alpha;

	xs=*(short *)(sprite  );
	ys=*(short *)(sprite+2);
	x-=(xs-*(short *)(sprite+4));	//	위치 교정..
	y-=*(short *)(sprite+6);	//	위치 교정..

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{
		count=sprite[bufferPoint++];
		for(i=0;i<count;i++)
		{
			bufferPoint++;
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
				p2SCREEN[addr+xPoint-j]	
					=	MIX2BYTE(plt[sprite[bufferPoint+j]],p2SCREEN[addr+xPoint-j],alpha,dest_alpha);

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
			if (xPoint	> WIDTH	) beginX= xPoint-WIDTH;	//	오른쪽 클리핑..

			for (j=beginX;j<size;j++)
				p2SCREEN[addr+xPoint-j]	
					=	MIX2BYTE(plt[sprite[bufferPoint+j]],p2SCREEN[addr+xPoint-j],alpha,dest_alpha);


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
				p2SCREEN[addr+xPoint-j]	
					=	MIX2BYTE(plt[sprite[bufferPoint+j]],p2SCREEN[addr+xPoint-j],alpha,dest_alpha);

			bufferPoint	+=	size;
			xPoint		-=	size;
		}
		addr+=WIDTH;
	}
}

void
c8BITSPRITE::fPutSpriteFX(int x,int y,BYTE *sprite,WORD *plt,int effect)
{
#ifdef dDEBUG_MODE
	if (!sprite  || !plt || effect > dPUT_EXTREAM_LIGHT)
	{	cMSG::Error("Error!! code c8BITSPRITE_0008");
		return;
	}
#endif

	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;

	xs=*(short *)(sprite  );
	ys=*(short *)(sprite+2);
	x-=(xs-*(short *)(sprite+4));	//	위치 교정..
	y-=*(short *)(sprite+6);	//	위치 교정..

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
				p2SCREEN[addr+xPoint-j]	=	FXMIX(plt[sprite[bufferPoint+j]],p2SCREEN[addr+xPoint-j],effect);

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
			if (xPoint	> WIDTH	) beginX= xPoint-WIDTH;	//	오른쪽 클리핑..

			for (j=beginX;j<size;j++)
				p2SCREEN[addr+xPoint-j]	=	FXMIX(plt[sprite[bufferPoint+j]],p2SCREEN[addr+xPoint-j],effect);

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
				p2SCREEN[addr+xPoint-j]	=	FXMIX(plt[sprite[bufferPoint+j]],p2SCREEN[addr+xPoint-j],effect);

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
c8BITSPRITE::PutSpriteR(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int alpha)
{	if (xRate==100	&& yRate==100	){PutSprite(x,y,sprite,plt,0);	return;}
	if (xRate		< 0				){fPutSpriteR(x,y,sprite,plt,-xRate,yRate,alpha);return;}

	int	i,j;
	int xs,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=8,size;		//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite && !plt)
	{	cMSG::Error("Error!! code c8BITSPRITE_0009");
		return;
	}
#endif

	xs	=*(short *)(sprite		);				//	스프라이트 사이즈
	ys	=*(short *)(sprite+2	);
	x	-=(*(short *)(sprite + 4))*xRate/100;	//	위치 교정..
	y	-=(*(short *)(sprite + 6))*yRate/100;

	xLen=xs*xRate/100;							//	확대/축소된 이미즈 사이즈
	yLen=ys*yRate/100;


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
						*(p2SCREEN+addr+zoomXTable[j+xp].pos+k)=plt[*(sprite+bufferPoint+j)];

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
c8BITSPRITE::PutSpriteRColor(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int alpha)
{	if (xRate==100	&& yRate==100	){PutSpriteColor(x,y,sprite,plt,0);	return;}
	if (xRate		< 0				){fPutSpriteRColor(x,y,sprite,plt,-xRate,yRate,alpha);return;}

	int	i,j;
	int xs,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=8,size;		//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite && !plt)
	{	cMSG::Error("Error!! code c8BITSPRITE_0009");
		return;
	}
#endif

	xs	=*(short *)(sprite		);				//	스프라이트 사이즈
	ys	=*(short *)(sprite+2	);
	x	-=(*(short *)(sprite + 4))*xRate/100;	//	위치 교정..
	y	-=(*(short *)(sprite + 6))*yRate/100;

	xLen=xs*xRate/100;							//	확대/축소된 이미즈 사이즈
	yLen=ys*yRate/100;


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
c8BITSPRITE::PutSpriteRHB(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int alpha)
{	if (xRate==100 && yRate==100)	{PutSpriteHB(x,y,sprite,plt,0);	return;}
	if (xRate < 0				)	{fPutSpriteRHB(x,y,sprite,plt,-xRate,yRate,alpha);return;}

	int	i,j;
	int xs,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=8,size;		//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite && !plt)
	{	cMSG::Error("Error!! code c8BITSPRITE_0010");
		return;
	}
#endif

	xs	=*(short *)(sprite		);				//	스프라이트 사이즈
	ys	=*(short *)(sprite+2	);
	x	-=(*(short *)(sprite + 4))*xRate/100;	//	위치 교정..
	y	-=(*(short *)(sprite + 6))*yRate/100;

	xLen=xs*xRate/100;							//	확대/축소된 이미즈 사이즈
	yLen=ys*yRate/100;


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
							=((p2SCREEN[addr+zoomXTable[j+xp].pos+k]&d2HM_MASK)+(plt[*(sprite+bufferPoint+j)]&d2HM_MASK))>>1;

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
c8BITSPRITE::PutSpriteRAB(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int alpha)
{	if (xRate==100 && yRate==100)	{PutSpriteAB(x,y,sprite,plt,alpha);	return;}
	if (xRate < 0)					{fPutSpriteRAB(x,y,sprite,plt,-xRate,yRate,alpha);return;}

	if (alpha<=0 ) return;
	if (alpha>=32){	PutSpriteR(x,y,sprite,plt,xRate,yRate,alpha);return;}

	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=8,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..
	int	dest_alpha=32-alpha;

#ifdef dDEBUG_MODE
	if (!sprite && !plt)
	{	cMSG::Error("Error!! code c8BITSPRITE_0011");
		return;
	}
#endif

	xs	=*(short *)(sprite		);				//	스프라이트 사이즈
	ys	=*(short *)(sprite+2	);
	x	-=(*(short *)(sprite + 4))*xRate/100;	//	위치 교정..
	y	-=(*(short *)(sprite + 6))*yRate/100;

	xLen=xs*xRate/100;							//	확대/축소된 이미즈 사이즈
	yLen=ys*yRate/100;

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
							=	MIX2BYTE(plt[sprite[bufferPoint+j]],p2SCREEN[addr+zoomXTable[j+xp].pos+k],alpha,dest_alpha);

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
c8BITSPRITE::PutSpriteRFX(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int effect)
{	if (xRate==100 && yRate==100)	{PutSpriteFX(x,y,sprite,plt,effect);	return;}
	if (xRate < 0)					{fPutSpriteRFX(x,y,sprite,plt,-xRate,yRate,effect);return;}

	int	i,j;
	int xs,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=8,size;		//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite || !plt)
	{	cMSG::Error("Error!! code c8BITSPRITE_0012");
		return;
	}
#endif

	xs	=*(short *)(sprite		);				//	스프라이트 사이즈
	ys	=*(short *)(sprite+2	);
	x	-=(*(short *)(sprite + 4))*xRate/100;	//	위치 교정..
	y	-=(*(short *)(sprite + 6))*yRate/100;

	xLen=xs*xRate/100;							//	확대/축소된 이미즈 사이즈
	yLen=ys*yRate/100;

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
							=	FXMIX(plt[sprite[bufferPoint+j]],p2SCREEN[addr+zoomXTable[j+xp].pos+k],effect);

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
c8BITSPRITE::fPutSpriteR(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int alpha)
{	if (xRate==100 && yRate==100)	{fPutSprite(x,y,sprite,plt,0);	return;}
	if (xRate < 0)					{PutSpriteR(x,y,sprite,plt,-xRate,yRate,alpha);return;}

	int	i,j;
	int xs,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=8,size;		//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite && !plt)
	{	cMSG::Error("Error!! code c8BITSPRITE_0013");
		return;
	}
#endif

	xs	=*(short *)(sprite		);				//	스프라이트 사이즈
	ys	=*(short *)(sprite+2	);
	x	-=(xs-*(short *)(sprite + 4))*xRate/100;	//	위치 교정..
	y	-=(*(short *)(sprite + 6))*yRate/100;

	xLen=xs*xRate/100;							//	확대/축소된 이미즈 사이즈
	yLen=ys*yRate/100;


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
				size	=	sprite[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+xp].size;k++)
						*(p2SCREEN+addr+zoomXTable[xp+j].pos-k-1)=plt[*(sprite+bufferPoint+j)];

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
c8BITSPRITE::fPutSpriteRColor(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int alpha)
{	if (xRate==100 && yRate==100)	{fPutSpriteColor(x,y,sprite,plt,0);	return;}
	if (xRate < 0)					{PutSpriteRColor(x,y,sprite,plt,-xRate,yRate,alpha);return;}

	int	i,j;
	int xs,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=8,size;		//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite && !plt)
	{	cMSG::Error("Error!! code c8BITSPRITE_0013");
		return;
	}
#endif

	xs	=*(short *)(sprite		);				//	스프라이트 사이즈
	ys	=*(short *)(sprite+2	);
	x	-=(xs-*(short *)(sprite + 4))*xRate/100;	//	위치 교정..
	y	-=(*(short *)(sprite + 6))*yRate/100;

	xLen=xs*xRate/100;							//	확대/축소된 이미즈 사이즈
	yLen=ys*yRate/100;


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
				size	=	sprite[bufferPoint++];
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
c8BITSPRITE::fPutSpriteRHB(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int alpha)
{	if (xRate==100 && yRate==100)	{fPutSpriteHB(x,y,sprite,plt,0);	return;}
	if (xRate < 0)					{PutSpriteRHB(x,y,sprite,plt,-xRate,yRate,alpha);return;}

	int	i,j;
	int xs,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=8,size;		//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite && !plt)
	{	cMSG::Error("Error!! code c8BITSPRITE_0014");
		return;
	}
#endif

	xs	=*(short *)(sprite		);				//	스프라이트 사이즈
	ys	=*(short *)(sprite+2	);
	x	-=(xs-*(short *)(sprite + 4))*xRate/100;	//	위치 교정..
	y	-=(*(short *)(sprite + 6))*yRate/100;

	xLen=xs*xRate/100;							//	확대/축소된 이미즈 사이즈
	yLen=ys*yRate/100;


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
						*(p2SCREEN+addr+zoomXTable[xp+j].pos-k-1)
						=((*(p2SCREEN+addr+zoomXTable[xp+j].pos-k-1)&d2HM_MASK)+(plt[*(sprite+bufferPoint+j)]&d2HM_MASK))>>1;

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
c8BITSPRITE::fPutSpriteRAB(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int alpha)
{	if (xRate==100 && yRate==100)	{fPutSpriteAB(x,y,sprite,plt,alpha);	return;}
	if (xRate < 0)					{PutSpriteRAB(x,y,sprite,plt,-xRate,yRate,alpha);return;}

	if (alpha<=0 ) return;
	if (alpha>=32){	PutSpriteR(x,y,sprite,plt,xRate,yRate,alpha);return;}

	int xs ,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=8,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..
	int	dest_alpha=32-alpha;

#ifdef dDEBUG_MODE
	if (!sprite && !plt)
	{	cMSG::Error("Error!! code c8BITSPRITE_0015");
		return;
	}
#endif

	xs	=*(short *)(sprite		);				//	스프라이트 사이즈
	ys	=*(short *)(sprite+2	);
	x	-=(xs-*(short *)(sprite + 4))*xRate/100;	//	위치 교정..
	y	-=(*(short *)(sprite + 6))*yRate/100;

	xLen=xs*xRate/100;							//	확대/축소된 이미즈 사이즈
	yLen=ys*yRate/100;

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
						p2SCREEN[addr+zoomXTable[xp+j].pos-k-1]	
							=	MIX2BYTE(plt[sprite[bufferPoint+j]],p2SCREEN[addr+zoomXTable[xp+j].pos-k-1]	,alpha,dest_alpha);

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
c8BITSPRITE::fPutSpriteRFX(int x,int y,BYTE *sprite,WORD *plt,int xRate,int yRate,int effect)
{	if (xRate==100 && yRate==100)	{fPutSprite(x,y,sprite,plt,0);	return;}
	if (xRate < 0)					{PutSpriteR(x,y,sprite,plt,-xRate,yRate,effect);return;}

	int	i,j;
	int xs,ys,xLen,yLen;				//	이미지 사이즈(xs,ys)  확대된 이미지의 사이즈(xLen,yLen)
	int count,bufferPoint=8,size;		//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,xp,yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

#ifdef dDEBUG_MODE
	if (!sprite && !plt)
	{	cMSG::Error("Error!! code c8BITSPRITE_0016");
		return;
	}
#endif

	xs	=*(short *)(sprite		);				//	스프라이트 사이즈
	ys	=*(short *)(sprite+2	);
	x	-=(xs-*(short *)(sprite + 4))*xRate/100;	//	위치 교정..
	y	-=(*(short *)(sprite + 6))*yRate/100;

	xLen=xs*xRate/100;							//	확대/축소된 이미즈 사이즈
	yLen=ys*yRate/100;


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
						p2SCREEN[addr+zoomXTable[xp+j].pos-k-1]
							=	FXMIX(plt[sprite[bufferPoint+j]],p2SCREEN[addr+zoomXTable[xp+j].pos-k-1],effect);

				bufferPoint	+=	size;
				xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	sprite[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	sprite[bufferPoint+1] + 2;
	}
}