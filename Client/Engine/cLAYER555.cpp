#include "cDRAW.H"

void
cLAYER555::PutLayer()
{
	int   X1, Y1, X2, Y2 ,XS ,YS;
	int   XPoint=0,i,j,beginX,endX;
	int   count,bufferPoint	=8;
	int	  size;

	XS=*(WORD *)(_lpLAYER);
	YS=*(WORD *)(_lpLAYER+2);
	_x-=*(short *)(_lpLAYER+4);	//	위치 교정..
	_y-=*(short *)(_lpLAYER+6);	//	위치 교정..

	if (_x >= WIDTH) return;

	X1= 0;if ( _x > 0 ) X1 = _x;
	Y1= 0;if ( _y > 0 ) Y1 = _y;

	X2 = XS+_x;
	Y2 = YS+_y;
	if (HEIGHT < _y+YS) Y2=HEIGHT;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-_y;j++)		//	_y축 상단 클리핑..
	{	count	=	_lpLAYER[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size	=	_lpLAYER[bufferPoint++];
		}
	}

	if (_x < 0 || _x+XS > WIDTH)
	for(;Y1<Y2;Y1++)
	{
		count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{	XPoint	+=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			beginX	=	0;
			endX	=	size;

			if (XPoint			< 0		) beginX= -XPoint;			//	왼쪽 클리핑..
			if (XPoint+ size	> WIDTH	) endX	= WIDTH-XPoint;		//	오른쪽 클리핑..
			if (endX			> beginX) memset16(p2SCREEN+addr+beginX+XPoint,_Color,endX - beginX);

			XPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	
		count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{
			XPoint	+=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			memset16(p2SCREEN+addr+XPoint,_Color,size);

			XPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cLAYER555::PutLayerHB()
{	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;

	Xs=*(short *)(_lpLAYER  );
	Ys=*(short *)(_lpLAYER+2);
	_x-=*(short *)(_lpLAYER+4);	//	위치 교정..
	_y-=*(short *)(_lpLAYER+6);	//	위치 교정..

	X1= 0;if ( _x > 0 ) X1 = _x;
	Y1= 0;if ( _y > 0 ) Y1 = _y;

	X2 = Xs+_x;
	Y2 = Ys+_y;
	if (HEIGHT < _y+Ys) Y2=HEIGHT;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	_Color=(_Color&d2HM_MASK_555)>>1;

	for(j=0;j<-_y;j++)		//	_y축 상단 클리핑..
	{	count=_lpLAYER[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=_lpLAYER[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (_x<0)
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x;
		
		for(i=0;i<count;i++)
		{	XPoint	+=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			beginX	=	0;

			if (XPoint			< 0		) beginX= -XPoint;			//	왼쪽 클리핑..

			for (j=beginX;j<size;j++)
				p2SCREEN[addr+XPoint+j]
					=((p2SCREEN[addr+XPoint+j]&d2HM_MASK_555)>>1)+_Color;

			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (_x+Xs > WIDTH)
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{	XPoint	+=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			endX	=	(int) size;

			if (XPoint+ size	>  WIDTH) endX	= WIDTH-XPoint;		//	오른쪽 클리핑..
			for (j=0;j<endX;j++)
				p2SCREEN[addr+XPoint+j]
					=((p2SCREEN[addr+XPoint+j]&d2HM_MASK_555)>>1)+_Color;

			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{	XPoint	+=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			for (j=0;j<size;j++)
				p2SCREEN[addr+XPoint+j]
					=((p2SCREEN[addr+XPoint+j]&d2HM_MASK_555)>>1)+_Color;

			XPoint		+=	size;
		}

		addr+=WIDTH;
	}
}

void
cLAYER555::PutLayerAB()
{	if (_Alpha<=0 ) return;

	if (_Alpha>=32)
	{	PutLayer();
		return;
	}

	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;

	Xs=*(short *)(_lpLAYER  );
	Ys=*(short *)(_lpLAYER+2);
	_x-=*(short *)(_lpLAYER+4);	//	위치 교정..
	_y-=*(short *)(_lpLAYER+6);	//	위치 교정..

	X1= 0;if ( _x > 0 ) X1 = _x;
	Y1= 0;if ( _y > 0 ) Y1 = _y;

	X2 = Xs+_x;
	Y2 = Ys+_y;
	if (HEIGHT < _y+Ys) Y2=HEIGHT;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-_y;j++)		//	_y축 상단 클리핑..
	{	count=_lpLAYER[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=_lpLAYER[bufferPoint++];
			bufferPoint+=size;
		}
	}

	_Color	=	GET2BYTEALPHAVALUE555(_Color,_Alpha);

	if (_x<0)
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{	XPoint	+=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			beginX	=	0;

			if (XPoint			< 0		) beginX= -XPoint;			//	왼쪽 클리핑..

			for (j=beginX;j<size;j++)
				p2SCREEN[addr+XPoint+j]	=	_Color+GET2BYTEALPHAVALUE555(p2SCREEN[addr+XPoint+j],_DestAlpha);

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (_x+Xs > WIDTH)
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{	XPoint	+=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			endX	=	(int) size;

			if (XPoint+ size	>  WIDTH) endX	= WIDTH-XPoint;		//	오른쪽 클리핑..

			for (j=0;j<endX;j++)
				p2SCREEN[addr+XPoint+j]	=	_Color+GET2BYTEALPHAVALUE555(p2SCREEN[addr+XPoint+j],_DestAlpha);

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{	XPoint	+=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			for (j=0;j<size;j++)
				p2SCREEN[addr+XPoint+j]	=	_Color+GET2BYTEALPHAVALUE555(p2SCREEN[addr+XPoint+j],_DestAlpha);

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
}

void
cLAYER555::PutLayerFX()
{

	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;

	Xs=*(short *)(_lpLAYER  );
	Ys=*(short *)(_lpLAYER+2);
	_x-=*(short *)(_lpLAYER+4);	//	위치 교정..
	_y-=*(short *)(_lpLAYER+6);	//	위치 교정..

	X1= 0;if ( _x > 0 ) X1 = _x;
	Y1= 0;if ( _y > 0 ) Y1 = _y;

	X2 = Xs+_x;
	Y2 = Ys+_y;
	if (HEIGHT < _y+Ys) Y2=HEIGHT;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-_y;j++)		//	_y축 상단 클리핑..
	{	count=_lpLAYER[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=_lpLAYER[bufferPoint++];
			bufferPoint+=size;
		}
	}

	int red,green,blue;
	red		=	_Color>>dGET_RED_SHIFT555;
	green	=	(_Color>>5)&dGET_GREEN_MASK555;
	blue	=	_Color&31;

	if (_x<0)
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x;
		for(i=0;i<count;i++)
		{	XPoint	+=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			beginX	=	0;

			if (XPoint			< 0		) beginX= -XPoint;			//	왼쪽 클리핑..

			for (j=beginX;j<size;j++)
				p2SCREEN[addr+XPoint+j]	=	FXMIX555(red,green,blue,p2SCREEN[addr+XPoint+j],_Effect);

			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (_x+Xs > WIDTH)
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{	XPoint	+=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			endX	=	(int) size;

			if (XPoint+ size	>  WIDTH) endX	= WIDTH-XPoint;		//	오른쪽 클리핑..

			for (j=0;j<endX;j++)
				p2SCREEN[addr+XPoint+j]	=	FXMIX555(red,green,blue,p2SCREEN[addr+XPoint+j],_Effect);

			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{	XPoint	+=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			for (j=0;j<size;j++)
				p2SCREEN[addr+XPoint+j]	=	FXMIX555(red,green,blue,p2SCREEN[addr+XPoint+j],_Effect);

			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
}

void
cLAYER555::fPutLayer()
{	int   X1, Y1, X2, Y2 ,XS ,YS;
	int   XPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;

	XS=*(WORD *)(_lpLAYER);
	YS=*(WORD *)(_lpLAYER+2);
	_x-=(XS - *(short *)(_lpLAYER+4));	//	위치 교정..
	_y-=*(short *)(_lpLAYER+6);	//	위치 교정..

	if (_x >= WIDTH) return;

	X1= 0;if ( _x > 0 ) X1 = _x;
	Y1= 0;if ( _y > 0 ) Y1 = _y;

	X2 = XS+_x;
	Y2 = YS+_y;
	if (HEIGHT < _y+YS) Y2=HEIGHT;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-_y;j++)		//	_y축 상단 클리핑..
	{	count	=	_lpLAYER[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint+=2;
	}

	if (_x < 0 || _x+XS > WIDTH)
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x+XS;

		for(i=0;i<count;i++)
		{	XPoint	-=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			beginX	=	0;
			endX	=	size;

			int	tail	=	XPoint;

			if (XPoint	< size	) beginX= size-XPoint;			//	왼쪽 클리핑..
			if (XPoint	> WIDTH)
			{	endX	= WIDTH-(XPoint-size);		//	오른쪽 클리핑..
				tail	= WIDTH;
			}
			memset16(p2SCREEN+addr+tail-size+1+beginX,_Color,endX-beginX);

			XPoint	-=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x+XS;

		for(i=0;i<count;i++)
		{	XPoint	-=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			memset16(p2SCREEN+addr+XPoint-size+1,_Color,size);

			XPoint	-=	size;
		}
		addr+=WIDTH;
	}
}

void
cLAYER555::fPutLayerHB()
{	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;

	Xs=*(short *)(_lpLAYER  );
	Ys=*(short *)(_lpLAYER+2);
	_x-=(Xs-*(short *)(_lpLAYER+4));	//	위치 교정..
	_y-=*(short *)(_lpLAYER+6);	//	위치 교정..

	X1= 0;if ( _x > 0 ) X1 = _x;
	Y1= 0;if ( _y > 0 ) Y1 = _y;

	X2 = Xs+_x;
	Y2 = Ys+_y;
	if (HEIGHT < _y+Ys) Y2=HEIGHT;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-_y;j++)		//	_y축 상단 클리핑..
	{	count=_lpLAYER[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=_lpLAYER[bufferPoint++];
			bufferPoint+=size;
		}
	}

	_Color	=	(_Color&d2HM_MASK_555)>>1;

	if (_x<0)
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			endX	=	size;
			if (XPoint- size	< 0		) endX	= XPoint;		//	왼쪽 클리핑..

			for (j=0;j<endX;j++)
				p2SCREEN[addr+XPoint-j]
					=((p2SCREEN[addr+XPoint-j]&d2HM_MASK_555)>>1)+_Color;

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	if (_x+Xs > WIDTH)
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			beginX	=	0;
			if (XPoint	> WIDTH	) beginX= XPoint-WIDTH;	//	오른쪽 클리핑..

			for (j=beginX;j<size;j++)
				p2SCREEN[addr+XPoint-j]
					=((p2SCREEN[addr+XPoint-j]&d2HM_MASK_555)>>1)+_Color;

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			for (j=0;j<size;j++)
				p2SCREEN[addr+XPoint-j]
					=((p2SCREEN[addr+XPoint-j]&d2HM_MASK_555)>>1)+_Color;

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
}

void
cLAYER555::fPutLayerAB()
{	if (_Alpha<=0 ) return;
	if (_Alpha>=32) fPutLayer();

	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;

	Xs=*(short *)(_lpLAYER  );
	Ys=*(short *)(_lpLAYER+2);
	_x-=(Xs-*(short *)(_lpLAYER+4));	//	위치 교정..
	_y-=*(short *)(_lpLAYER+6);	//	위치 교정..

	X1= 0;if ( _x > 0 ) X1 = _x;
	Y1= 0;if ( _y > 0 ) Y1 = _y;

	X2 = Xs+_x;
	Y2 = Ys+_y;
	if (HEIGHT < _y+Ys) Y2=HEIGHT;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-_y;j++)		//	_y축 상단 클리핑..
	{	count=_lpLAYER[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=_lpLAYER[bufferPoint++];
			bufferPoint+=size;
		}
	}

	_Color	=	GET2BYTEALPHAVALUE555(_Color,_Alpha);

	if (_x<0)
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			endX	=	size;
			if (XPoint - size	< 0		) endX	= XPoint;		//	왼쪽 클리핑..

			for (j=0;j<endX;j++)
				p2SCREEN[addr+XPoint-j]	
					=	_Color+GET2BYTEALPHAVALUE555(p2SCREEN[addr+XPoint-j],_DestAlpha);

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	if (_x+Xs > WIDTH)
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			beginX	=	0;
			if (XPoint	> WIDTH	) beginX= XPoint-WIDTH;	//	오른쪽 클리핑..

			for (j=beginX;j<size;j++)
				p2SCREEN[addr+XPoint-j]	
					=	_Color+GET2BYTEALPHAVALUE555(p2SCREEN[addr+XPoint-j],_DestAlpha);


			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			for (j=0;j<size;j++)
				p2SCREEN[addr+XPoint-j]	
					=	_Color+GET2BYTEALPHAVALUE555(p2SCREEN[addr+XPoint-j],_DestAlpha);

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
}

void
cLAYER555::fPutLayerFX()
{

	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;

	Xs=*(short *)(_lpLAYER  );
	Ys=*(short *)(_lpLAYER+2);
	_x-=(Xs-*(short *)(_lpLAYER+4));	//	위치 교정..
	_y-=*(short *)(_lpLAYER+6);	//	위치 교정..

	X1= 0;if ( _x > 0 ) X1 = _x;
	Y1= 0;if ( _y > 0 ) Y1 = _y;

	X2 = Xs+_x;
	Y2 = Ys+_y;
	if (HEIGHT < _y+Ys) Y2=HEIGHT;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-_y;j++)		//	_y축 상단 클리핑..
	{	count=_lpLAYER[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=_lpLAYER[bufferPoint++];
			bufferPoint+=size;
		}
	}

	int red,green,blue;
	red		=	_Color>>dGET_RED_SHIFT555;
	green	=	(_Color>>5)&dGET_GREEN_MASK555;
	blue	=	_Color&31;

	if (_x<0)
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			endX	=	size;
			if (XPoint - size	< 0		) endX	= XPoint;		//	왼쪽 클리핑..

			for (j=0;j<endX;j++)
				p2SCREEN[addr+XPoint-j]	=	FXMIX555(red,green,blue,p2SCREEN[addr+XPoint-j],_Effect);

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	if (_x+Xs > WIDTH)
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			beginX	=	0;
			if (XPoint	> WIDTH	) beginX= XPoint-WIDTH;	//	오른쪽 클리핑..

			for (j=beginX;j<size;j++)
				p2SCREEN[addr+XPoint-j]	=	FXMIX555(red,green,blue,p2SCREEN[addr+XPoint-j],_Effect);

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count	=	_lpLAYER[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_lpLAYER[bufferPoint++];
			size	=	_lpLAYER[bufferPoint++];

			for (j=0;j<size;j++)
				p2SCREEN[addr+XPoint-j]	=	FXMIX555(red,green,blue,p2SCREEN[addr+XPoint-j],_Effect);

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
}


/*********************************************************************************************************
	확대/축소 출력
*********************************************************************************************************/


void
cLAYER555::PutLayerR()
{	int XS ,YS,XLen,YLen;				//	이미지 사이즈(XS,YS)  확대된 이미지의 사이즈(XLen,YLen)
	int count,bufferPoint=8,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,Xp,Yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	XS=*(WORD *)(_lpLAYER);
	YS=*(WORD *)(_lpLAYER+2);
	Xp=*(short *)(_lpLAYER+4);	//	위치 교정..
	Yp=*(short *)(_lpLAYER+6);	//	위치 교정..

	XLen=XS*_XRate/100	;
	YLen=YS*_YRate/100	;	//	확대/축소된 이미즈 사이즈
	_x	-=Xp*_XRate/100	;
	_y	-=Yp*_YRate/100	;	//	위치 교정..
	if (_x + XLen < 0 || _x >= WIDTH || _y + YLen < 0 || _y > HEIGHT )  return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(_x,_y,XS,YS,XLen,YLen);

	addr=max(_y,0)*WIDTH+_x;			//	스크린 주소..

	for (_y=0;_y<YS;_y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (Yp=0;Yp<zoomYTable[_y].size;Yp++)
		{	count	=	_lpLAYER[bufferPoint++];	//	이미지 카운트
			Xp		=	0;
			for(i=0;i<count;i++)
			{	Xp	+=	_lpLAYER[bufferPoint++];
				size=	_lpLAYER[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						*(p2SCREEN+addr+zoomXTable[j+Xp].pos+k)=_Color;

				Xp	+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	_lpLAYER[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	2;
	}
}

void
cLAYER555::PutLayerRHB()
{	int	i,j;
	int Xs,Ys,XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int count,bufferPoint=8,size;		//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,Xp,Yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	Xs	=*(short *)(_lpLAYER		);				//	스프라이트 사이즈
	Ys	=*(short *)(_lpLAYER+2	);
	_x	-=(*(short *)(_lpLAYER + 4))*_XRate/100;	//	위치 교정..
	_y	-=(*(short *)(_lpLAYER + 6))*_YRate/100;

	XLen=Xs*_XRate/100;							//	확대/축소된 이미즈 사이즈
	YLen=Ys*_YRate/100;

	if (_x + XLen < 0 || _x >= WIDTH || _y + YLen < 0 || _y > HEIGHT )  return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	addr=max(_y,0)*WIDTH+_x;			//	스크린 주소..

	_Color	=	(_Color&d2HM_MASK_555)>>1;

	for (_y=0;_y<Ys;_y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (Yp=0;Yp<zoomYTable[_y].size;Yp++)
		{	count	=	_lpLAYER[bufferPoint++];	//	이미지 카운트
			Xp		=	0;
			for(i=0;i<count;i++)
			{	Xp	+=	_lpLAYER[bufferPoint++];
				size=	_lpLAYER[bufferPoint++];

				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						*(p2SCREEN+addr+zoomXTable[j+Xp].pos+k)
							=((p2SCREEN[addr+zoomXTable[j+Xp].pos+k]&d2HM_MASK_555)>>1)+_Color;

				Xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	_lpLAYER[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	2;
	}
}

void
cLAYER555::PutLayerRAB()
{	if (_Alpha<=0 ) return;
	if (_Alpha>=32)
	{	PutLayerR();
		return;
	}

	int Xs ,Ys,XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int count,bufferPoint=8,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,Xp,Yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	Xs	=*(short *)(_lpLAYER		);				//	스프라이트 사이즈
	Ys	=*(short *)(_lpLAYER+2	);
	_x	-=(*(short *)(_lpLAYER + 4))*_XRate/100;	//	위치 교정..
	_y	-=(*(short *)(_lpLAYER + 6))*_YRate/100;

	XLen=Xs*_XRate/100;							//	확대/축소된 이미즈 사이즈
	YLen=Ys*_YRate/100;

	MakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	addr	=	max(_y,0)*WIDTH+_x;			//	스크린 주소..

	_Color	=	GET2BYTEALPHAVALUE555(_Color,_Alpha);

	for (_y=0;_y<Ys;_y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (Yp=0;Yp<zoomYTable[_y].size;Yp++)
		{	count	=	_lpLAYER[bufferPoint++];	//	이미지 카운트
			Xp		=	0;
			for(i=0;i<count;i++)
			{	Xp	+=	_lpLAYER[bufferPoint++];
				size=	_lpLAYER[bufferPoint++];

				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						p2SCREEN[addr+zoomXTable[j+Xp].pos+k]
							=	_Color+GET2BYTEALPHAVALUE555(p2SCREEN[addr+zoomXTable[j+Xp].pos+k],_DestAlpha);

				Xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	_lpLAYER[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	2;
	}
}

void
cLAYER555::PutLayerRFX()
{	int	i,j;
	int Xs,Ys,XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int count,bufferPoint=8,size;		//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,Xp,Yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	Xs	=*(short *)(_lpLAYER		);				//	스프라이트 사이즈
	Ys	=*(short *)(_lpLAYER+2	);
	_x	-=(*(short *)(_lpLAYER + 4))*_XRate/100;	//	위치 교정..
	_y	-=(*(short *)(_lpLAYER + 6))*_YRate/100;

	XLen=Xs*_XRate/100;							//	확대/축소된 이미즈 사이즈
	YLen=Ys*_YRate/100;

	if (_x + XLen < 0 || _x >= WIDTH || _y + YLen < 0 || _y > HEIGHT )  return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	addr=max(_y,0)*WIDTH+_x;			//	스크린 주소..

	int red,green,blue;
	red		=	_Color>>dGET_RED_SHIFT555;
	green	=	(_Color>>5)&dGET_GREEN_MASK555;
	blue	=	_Color&31;

	for (_y=0;_y<Ys;_y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (Yp=0;Yp<zoomYTable[_y].size;Yp++)
		{	count	=	_lpLAYER[bufferPoint++];	//	이미지 카운트
			Xp		=	0;
			for(i=0;i<count;i++)
			{	Xp	+=	_lpLAYER[bufferPoint++];
				size=	_lpLAYER[bufferPoint++];

				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						p2SCREEN[addr+zoomXTable[j+Xp].pos+k]
							=	FXMIX555(red,green,blue,p2SCREEN[addr+zoomXTable[j+Xp].pos+k],_Effect);

				Xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	_lpLAYER[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	2;
	}
}

void
cLAYER555::fPutLayerR()
{	int XS ,YS,XLen,YLen;				//	이미지 사이즈(XS,YS)  확대된 이미지의 사이즈(XLen,YLen)
	int count,bufferPoint=8,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,Xp,Yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	XS=*(WORD *)(_lpLAYER)	;
	YS=*(WORD *)(_lpLAYER+2)	;
	Xp=*(short *)(_lpLAYER+4)	;	//	위치 교정..
	Yp=*(short *)(_lpLAYER+6)	;	//	위치 교정..
	XLen=XS*_XRate/100		;
	YLen=YS*_YRate/100		;	//	확대/축소된 이미즈 사이즈
	_x	-=(XS-Xp)*_XRate/100;
	_y	-=Yp*_YRate/100		;	//	위치 교정..
	if (_x + XLen < 0 || _x >= WIDTH || _y + YLen < 0 || _y > HEIGHT )  return;	//	이미지가 화면에 없다.

	fMakeSpriteRTable(_x,_y,XS,YS,XLen,YLen);

	addr	=	max(_y,0)*WIDTH+_x;			//	스크린 주소..

	for (_y=0;_y<YS;_y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (Yp=0;Yp<zoomYTable[_y].size;Yp++)
		{	count	=	_lpLAYER[bufferPoint++];	//	이미지 카운트
			Xp		=	0;
			for(i=0;i<count;i++)
			{	Xp	+=	_lpLAYER[bufferPoint++];
				size=	_lpLAYER[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						*(p2SCREEN+addr+zoomXTable[Xp+j].pos-k-1)=_Color;

				Xp	+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	_lpLAYER[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	2;
	}
}

void
cLAYER555::fPutLayerRHB()
{	int	i,j;
	int Xs,Ys,XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int count,bufferPoint=8,size;		//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,Xp,Yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	Xs	=*(short *)(_lpLAYER		);				//	스프라이트 사이즈
	Ys	=*(short *)(_lpLAYER+2	);
	_x	-=(Xs-*(short *)(_lpLAYER + 4))*_XRate/100;	//	위치 교정..
	_y	-=(*(short *)(_lpLAYER + 6))*_YRate/100;

	XLen=Xs*_XRate/100;							//	확대/축소된 이미즈 사이즈
	YLen=Ys*_YRate/100;


	if (_x + XLen < 0 || _x >= WIDTH || _y + YLen < 0 || _y > HEIGHT )  return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	addr	=	max(_y,0)*WIDTH+_x;			//	스크린 주소..
	_Color	=	(_Color&d2HM_MASK_555)>>1;

	for (_y=0;_y<Ys;_y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (Yp=0;Yp<zoomYTable[_y].size;Yp++)
		{	count	=	_lpLAYER[bufferPoint++];	//	이미지 카운트
			Xp		=	0;
			for(i=0;i<count;i++)
			{	Xp	+=	_lpLAYER[bufferPoint++];
				size=	_lpLAYER[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						p2SCREEN[addr+zoomXTable[Xp+j].pos-k-1]
							=	((p2SCREEN[addr+zoomXTable[Xp+j].pos-k-1]&d2HM_MASK_555)>>1)+_Color;

				Xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	_lpLAYER[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	2;
	}
}

void
cLAYER555::fPutLayerRAB()
{	if (_Alpha<=0 ) return;
	if (_Alpha>=32)
	{	fPutLayerR();
		return;
	}

	int Xs ,Ys,XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int count,bufferPoint=8,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,Xp,Yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	Xs	=*(short *)(_lpLAYER		);				//	스프라이트 사이즈
	Ys	=*(short *)(_lpLAYER+2	);
	_x	-=(Xs-*(short *)(_lpLAYER + 4))*_XRate/100;	//	위치 교정..
	_y	-=(*(short *)(_lpLAYER + 6))*_YRate/100;

	XLen=Xs*_XRate/100;							//	확대/축소된 이미즈 사이즈
	YLen=Ys*_YRate/100;

	fMakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	addr	=	max(_y,0)*WIDTH+_x;			//	스크린 주소..
	_Color	=	GET2BYTEALPHAVALUE555(_Color,_Alpha);

	for (_y=0;_y<Ys;_y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (Yp=0;Yp<zoomYTable[_y].size;Yp++)
		{	count	=	_lpLAYER[bufferPoint++];	//	이미지 카운트
			Xp		=	0;
			for(i=0;i<count;i++)
			{	Xp	+=	_lpLAYER[bufferPoint++];
				size=	_lpLAYER[bufferPoint++];

				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						p2SCREEN[addr+zoomXTable[Xp+j].pos-k-1]
							=	_Color+GET2BYTEALPHAVALUE555(p2SCREEN[addr+zoomXTable[Xp+j].pos-k-1],_DestAlpha);

				Xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	_lpLAYER[bufferPoint++];

		for(i=0;i<count;i++)	bufferPoint	+=	2;
	}
}

void
cLAYER555::fPutLayerRFX()
{	int	i,j;
	int Xs,Ys,XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int count,bufferPoint=8,size;		//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,Xp,Yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	Xs	=*(short *)(_lpLAYER		);				//	스프라이트 사이즈
	Ys	=*(short *)(_lpLAYER+2	);
	_x	-=(Xs-*(short *)(_lpLAYER + 4))*_XRate/100;	//	위치 교정..
	_y	-=(*(short *)(_lpLAYER + 6))*_YRate/100;

	XLen=Xs*_XRate/100;							//	확대/축소된 이미즈 사이즈
	YLen=Ys*_YRate/100;


	if (_x + XLen < 0 || _x >= WIDTH || _y + YLen < 0 || _y > HEIGHT )  return;	//	이미지가 화면에 없다.

	fMakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	addr	=	max(_y,0)*WIDTH+_x;			//	스크린 주소..

	int red,green,blue;
	red		=	_Color>>dGET_RED_SHIFT555;
	green	=	(_Color>>5)&dGET_GREEN_MASK555;
	blue	=	_Color&31;

	for (_y=0;_y<Ys;_y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (Yp=0;Yp<zoomYTable[_y].size;Yp++)
		{	count	=	_lpLAYER[bufferPoint++];	//	이미지 카운트
			Xp		=	0;
			for(i=0;i<count;i++)
			{	Xp	+=	_lpLAYER[bufferPoint++];
				size=	_lpLAYER[bufferPoint++];

				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						p2SCREEN[addr+zoomXTable[Xp+j].pos-k-1]
							=	FXMIX555(red,green,blue,p2SCREEN[addr+zoomXTable[Xp+j].pos-k-1],_Effect);

				Xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	_lpLAYER[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	2;
	}
}