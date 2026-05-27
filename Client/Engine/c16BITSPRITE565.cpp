#include "cDRAW.H"

/*******************************************************************************************

	16bit 압축 스프라이트 출력 루틴들..
	압축 스프라이트 형식

		16bit 압축 스프라이트..	----------------+
			가로	사이즈 2 b_yte				|
			세로	사이즈 2 b_yte				|
			가로 위치 교정 2 b_yte				|
			세로 위치 교정 2 b_yte				|
			데이터								|
				카운트(한 라인에 이미지 수)		|
					공백 크기					|
					이미지 크기					|
					이미지	--------------------+ -> 2 b_yte 괜히 16bit냐.. ==;
void	PutSpriteXX(int _x,int _y,WORD *_p16SPRITE,...)
int	_x,_y
	:	이미지의 좌표
WORD *_p16SPRITE
	:	이미지 데이터
...
	:	알파 블렌딩시 알파값



void	PutSprite(int _x,int _y,WORD *_p16SPRITE,int _Alpha)
		그냥 출력해준다.
void	PutSpriteHB(int _x,int _y,WORD *_p16SPRITE,int _Alpha)
		화면과 반투명처리해준다.
void	PutSpriteAB(int _x,int _y,WORD *_p16SPRITE,int _Alpha)
		알파블렌딩 출력
void	PutSpriteAB(int _x,int _y,WORD *_p16SPRITE,int _Alpha)
		주어진 알파값으로 스프라이트와 배경을 섞어 준다.
void	PutSpriteFX(int _x,int _y,WORD *_p16SPRITE,int _Effect)
		주어진 이펙트로 스프라이트와 배경을 섞어 준다.

void	PutSpriteR(int _x,int _y,WORD *_p16SPRITE,int _Alpha)
void	PutSpriteRHB(int _x,int _y,WORD *_p16SPRITE,int _Alpha)
void	PutSpriteRAB(int _x,int _y,WORD *_p16SPRITE,int _Alpha)
void	PutSpriteRFX(int _x,int _y,WORD *_p16SPRITE,int _Effect)

		확대/축소 출력..

		위에서 알파값은 32 가  Ma_x이며 이때 이미지가 원본 상태로 나타난다.
		0일때는 아무것도 출력 되지 않는다.

*******************************************************************************************/

void
c16BITSPRITE565::PutSpriteHB()
{
	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=4;
	int   size;

	Xs=(short)_p16SPRITE[0];
	Ys=(short)_p16SPRITE[1];
	_x-=(short)_p16SPRITE[2];	//	위치 교정..
	_y-=(short)_p16SPRITE[3];	//	위치 교정..

	if (_x >= s_rectClippingArea.x2)
		return;

	X1= s_rectClippingArea.x1;if ( _x > s_rectClippingArea.x1 ) X1 = _x;
	Y1= s_rectClippingArea.y1;if ( _y > s_rectClippingArea.y1 ) Y1 = _y;

	X2 = Xs+_x;
	Y2 = Ys+_y;

	if (s_rectClippingArea.y2 < _y+Ys)
		Y2=s_rectClippingArea.y2;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 )
		return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-(_y-s_rectClippingArea.y1);j++)		//	_y축 상단 클리핑..
	{	count=_p16SPRITE[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size		=	_p16SPRITE[bufferPoint++];
			bufferPoint	+=	size;
		}
	}

	if (_x<s_rectClippingArea.x1)
	for(;Y1<Y2;Y1++)
	{	count	=	_p16SPRITE[bufferPoint++];
		XPoint	=	_x;
		for(i=0;i<count;i++)
		{	XPoint	+=	_p16SPRITE[bufferPoint++];
			size	=	_p16SPRITE[bufferPoint++];

			beginX	=	0;
			endX	=	size;

			if (XPoint			< s_rectClippingArea.x1	) beginX= s_rectClippingArea.x1-XPoint;			//	왼쪽 클리핑..
			if (XPoint+ size	> s_rectClippingArea.x2	) endX	= s_rectClippingArea.x2-XPoint;	//	오른쪽 클리핑..

			if (endX	> beginX )
			{
				UINT *p4_scr=(UINT *)&p2SCREEN[addr+XPoint+beginX];
				UINT *p4_spr=(UINT *)&_p16SPRITE[bufferPoint+beginX];
				width		=endX-beginX;

				for (j=0;j<width/2;j++)
					p4_scr[j]	=	(((p4_scr[j]&d4HM_MASK_565)>>1)+((p4_spr[j]&d4HM_MASK_565)>>1));

				if ((width&1) && width > 0)
					p2SCREEN[addr+XPoint+endX-1]
						=((p2SCREEN[addr+XPoint+endX-1]&d2HM_MASK_565)+(_p16SPRITE[bufferPoint+endX-1]&d2HM_MASK_565))>>1;
			}

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (_x+Xs > s_rectClippingArea.x2)
	for(;Y1<Y2;Y1++)
	{	count=_p16SPRITE[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{	XPoint	+=	_p16SPRITE[bufferPoint++];
			size	=	_p16SPRITE[bufferPoint++];

			endX	=	(int) size;

			if (XPoint+ size	>  s_rectClippingArea.x2) endX	= s_rectClippingArea.x2-XPoint;		//	오른쪽 클리핑..

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+XPoint];
			UINT *p4_spr=(UINT *)&_p16SPRITE[bufferPoint];

			for (j=0;j<endX/2;j++)
				p4_scr[j]=(((p4_scr[j]&d4HM_MASK_565)>>1)+((p4_spr[j]&d4HM_MASK_565)>>1));
			if ((endX&1) && endX > 0)
				p2SCREEN[addr+XPoint+endX-1]
				=((p2SCREEN[addr+XPoint+endX-1]&d2HM_MASK_565)+(_p16SPRITE[bufferPoint+endX-1]&d2HM_MASK_565))>>1;

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count=_p16SPRITE[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{	XPoint	+=	_p16SPRITE[bufferPoint++];
			size	=	_p16SPRITE[bufferPoint++];

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+XPoint];
			UINT *p4_spr=(UINT *)&_p16SPRITE[bufferPoint];

			for (j=0;j<size/2;j++)
				p4_scr[j]=(((p4_scr[j]&d4HM_MASK_565)>>1)+((p4_spr[j]&d4HM_MASK_565)>>1));
			if ( size&1 )
				p2SCREEN[addr+XPoint+size-1]
				=((p2SCREEN[addr+XPoint+size-1]&d2HM_MASK_565)+(_p16SPRITE[bufferPoint+size-1]&d2HM_MASK_565))>>1;
			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
}

void
c16BITSPRITE565::PutSpriteAB()
{
	if (_Alpha<=0 )
		return;
	if (_Alpha>=32)
	{	
		c16BITSPRITE555::PutSprite();
		return;
	}

	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=4;
	int	  size;
	int	  _DestAlpha=32-_Alpha;

	Xs=(short)_p16SPRITE[0];
	Ys=(short)_p16SPRITE[1];
	_x-=(short)_p16SPRITE[2];	//	위치 교정..
	_y-=(short)_p16SPRITE[3];	//	위치 교정..

	if (_x >= s_rectClippingArea.x2)
		return;

	X1= s_rectClippingArea.x1;if ( _x > s_rectClippingArea.x1 ) X1 = _x;
	Y1= s_rectClippingArea.y1;if ( _y > s_rectClippingArea.y1 ) Y1 = _y;

	X2 = Xs+_x;
	Y2 = Ys+_y;
	if (s_rectClippingArea.y2 < _y+Ys) Y2=s_rectClippingArea.y2;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 )
		return;	//	이미지가 화면에 없다.

	int addr=Y1*WIDTH;

	for(j=0;j<-(_y-s_rectClippingArea.y1);j++)		//	_y축 상단 클리핑..
	{
		count=_p16SPRITE[bufferPoint++];
		for(i=0;i<count;i++)
		{
			bufferPoint++;
			size=_p16SPRITE[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (_x<s_rectClippingArea.x1)
	for(;Y1<Y2;Y1++)
	{	count	=	_p16SPRITE[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{	XPoint	+=	_p16SPRITE[bufferPoint++];	
			size	=	_p16SPRITE[bufferPoint++];

			beginX	=	0;
			endX	=	size;

			if (XPoint			< s_rectClippingArea.x1	) beginX= -XPoint;			//	왼쪽 클리핑..
			if (XPoint+ size	> s_rectClippingArea.x2	) endX	= s_rectClippingArea.x2-XPoint;	//	오른쪽 클리핑..

			if (endX	> beginX )
			{
				UINT *p4_scr=(UINT *)&p2SCREEN[addr+XPoint+beginX];
				UINT *p4_spr=(UINT *)&_p16SPRITE[bufferPoint+beginX];
				width		=size-beginX;

				for (j=0;j<width/2;j++)
					p4_scr[j]	=	MIX4BYTE565(p4_spr[j],p4_scr[j],_Alpha,_DestAlpha);

				if ((width&1) && width > 0)
					p2SCREEN[addr+XPoint+endX-1]	=	
						MIX2BYTE565(_p16SPRITE[bufferPoint+endX-1],p2SCREEN[addr+XPoint+endX-1],_Alpha,_DestAlpha);

			}

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (_x+Xs > s_rectClippingArea.x2)
	for(;Y1<Y2;Y1++)
	{	count=_p16SPRITE[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{	XPoint	+=	_p16SPRITE[bufferPoint++];	
			size	=	_p16SPRITE[bufferPoint++];

			endX	=	(int) size;

			if (XPoint+ size	>  s_rectClippingArea.x2) endX	= s_rectClippingArea.x2-XPoint;		//	오른쪽 클리핑..

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+XPoint];
			UINT *p4_spr=(UINT *)&_p16SPRITE[bufferPoint];

			for (j=0;j<endX/2;j++)
				p4_scr[j]	=	MIX4BYTE565(p4_spr[j],p4_scr[j],_Alpha,_DestAlpha);

			if ((endX&1)&& endX > 0)
				p2SCREEN[addr+XPoint+endX-1]	=	MIX2BYTE565(_p16SPRITE[bufferPoint+endX-1],p2SCREEN[addr+XPoint+endX-1],_Alpha,_DestAlpha);

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count=_p16SPRITE[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{	XPoint	+=	_p16SPRITE[bufferPoint++];	
			size	=	_p16SPRITE[bufferPoint++];

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+XPoint];
			UINT *p4_spr=(UINT *)&_p16SPRITE[bufferPoint];

			for (j=0;j<size/2;j++)
				p4_scr[j]	=	MIX4BYTE565(p4_spr[j],p4_scr[j],_Alpha,_DestAlpha);

			if (size&1)
				p2SCREEN[addr+XPoint+size-1]=	MIX2BYTE565(_p16SPRITE[bufferPoint+size-1],p2SCREEN[addr+XPoint+size-1],_Alpha,_DestAlpha);

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
}

void
c16BITSPRITE565::PutSpriteFX()
{
	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=4;
	int   size;

	Xs=(short)_p16SPRITE[0];
	Ys=(short)_p16SPRITE[1];
	_x-=(short)_p16SPRITE[2];	//	위치 교정..
	_y-=(short)_p16SPRITE[3];	//	위치 교정..

	if	(_x >= s_rectClippingArea.x2)
		return;

	X1	=	s_rectClippingArea.x1;

	if ( _x > s_rectClippingArea.x1 )
		X1 = _x;

	Y1	= s_rectClippingArea.y1;

	if	( _y > s_rectClippingArea.y1 )
		Y1 = _y;

	X2	=	Xs+_x;
	Y2	=	Ys+_y;

	if	(s_rectClippingArea.y2 < _y+Ys)
		Y2=s_rectClippingArea.y2;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-(_y-s_rectClippingArea.y1);j++)		//	_y축 상단 클리핑..
	{	
		count=_p16SPRITE[bufferPoint++];

		for(i=0;i<count;i++)
		{
			bufferPoint++;
			size		=	_p16SPRITE[bufferPoint++];
			bufferPoint	+=	size;
		}
	}

	if	(_x<s_rectClippingArea.x1)
	for	(;Y1<Y2;Y1++)
	{
		count	=	_p16SPRITE[bufferPoint++];

		XPoint	=	_x;

		for(i=0;i<count;i++)
		{
			XPoint	+=	_p16SPRITE[bufferPoint++];
			size	=	_p16SPRITE[bufferPoint++];

			beginX	=	0;

			if	(XPoint			< 0		)
				beginX= -XPoint;			//	왼쪽 클리핑..

			for (j=beginX;j<size;j++)
				p2SCREEN[addr+XPoint+j]	=	FXMIX565(_p16SPRITE[bufferPoint+j],p2SCREEN[addr+XPoint+j],_Effect);

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	if	(_x+Xs > s_rectClippingArea.x2)
	for	(;Y1<Y2;Y1++)
	{	
		count=_p16SPRITE[bufferPoint++];

		XPoint	=	_x;
		for(i=0;i<count;i++)
		{	
			XPoint	+=	_p16SPRITE[bufferPoint++];
			size	=	_p16SPRITE[bufferPoint++];

			endX	=	(int) size;

			if	(XPoint+ size	>  s_rectClippingArea.x2)
				endX	=	s_rectClippingArea.x2-XPoint;		//	오른쪽 클리핑..

			for (j=0;j<endX;j++)
				p2SCREEN[addr+XPoint+j]	=	FXMIX565(_p16SPRITE[bufferPoint+j],p2SCREEN[addr+XPoint+j],_Effect);

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count=_p16SPRITE[bufferPoint++];

		XPoint	=	_x;
		for(i=0;i<count;i++)
		{	XPoint	+=	_p16SPRITE[bufferPoint++];
			size	=	_p16SPRITE[bufferPoint++];

			for (j=0;j<size;j++)
				p2SCREEN[addr+XPoint+j]	=	FXMIX565(_p16SPRITE[bufferPoint+j],p2SCREEN[addr+XPoint+j],_Effect);

			bufferPoint	+=	size;
			XPoint		+=	size;
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
c16BITSPRITE565::fPutSpriteHB()
{	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=4;
	int  size;

	Xs=(short)_p16SPRITE[0];
	Ys=(short)_p16SPRITE[1];
	_x-=(Xs-(short)_p16SPRITE[2]);	//	위치 교정..
	_y-=(short)_p16SPRITE[3];	//	위치 교정..

	if (_x >= WIDTH) return;

	X1= 0;if ( _x > 0 ) X1 = _x;
	Y1= 0;if ( _y > 0 ) Y1 = _y;

	X2 = Xs+_x;
	Y2 = Ys+_y;
	if (HEIGHT < _y+Ys) Y2=HEIGHT;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-_y;j++)		//	_y축 상단 클리핑..
	{
		count=_p16SPRITE[bufferPoint++];

		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=_p16SPRITE[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (_x<0)
	for(;Y1<Y2;Y1++)
	{	count	=	_p16SPRITE[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_p16SPRITE[bufferPoint++];
			size	=	_p16SPRITE[bufferPoint++];

			beginX	=	0;

			if (XPoint	< size	) beginX= size-XPoint;			//	왼쪽 클리핑..

			width	=	size-beginX;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+XPoint-width];
			UINT *p4_spr=(UINT *)&_p16SPRITE[bufferPoint+(width&1)];

			for (j=0;j<width/2;j++)
				p4_scr[j]	=	(((p4_scr[j]&d4HM_MASK_565)>>1)+((((p4_spr[width/2-1-j]<<16)+(p4_spr[width/2-1-j]>>16))&d4HM_MASK_565)>>1));
			if  ((width&1) && width > 0 )
				p2SCREEN[addr+XPoint-1]=
					((p2SCREEN[addr+XPoint-1]&d2HM_MASK_565)+(_p16SPRITE[bufferPoint]&d2HM_MASK_565))>>1;

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr	+=	WIDTH;
	}
	else
	if (_x+Xs > WIDTH)
	for(;Y1<Y2;Y1++)
	{	count	=	_p16SPRITE[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_p16SPRITE[bufferPoint++];
			size	=	_p16SPRITE[bufferPoint++];

			endX	=	size;
			width	=	XPoint;		//tail

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+XPoint-size];
			UINT *p4_spr=(UINT *)&_p16SPRITE[bufferPoint+size%2];

			if (XPoint	>  WIDTH)
			{	endX	= WIDTH-(XPoint-size);		//	오른쪽 클리핑..	
				width	= WIDTH;
			}

			for (j=0;j<endX/2;j++)
				p4_scr[j]	=
				(	((p4_scr[j]&d4HM_MASK_565)>>1)	+
					((((p4_spr[size/2-1-j]<<16)+(p4_spr[size/2-1-j]>>16))&d4HM_MASK_565)>>1)
				);

			if  ((endX&1) && endX > 0)
				p2SCREEN[addr+width-1]
					=	((p2SCREEN[addr+width-1]&d2HM_MASK_565)+(_p16SPRITE[bufferPoint+size-endX]&d2HM_MASK_565))>>1;

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr	+=	WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count	=	_p16SPRITE[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_p16SPRITE[bufferPoint++];
			size	=	_p16SPRITE[bufferPoint++];

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+XPoint-size+1];
			UINT *p4_spr=(UINT *)&_p16SPRITE[bufferPoint+(size&1)];

			for (j=size/2-1,X1=0;j>=0;j--,X1++)
				p4_scr[X1]	=	(((p4_scr[X1]&d4HM_MASK_565)>>1)+((((p4_spr[j]<<16)+(p4_spr[j]>>16))&d4HM_MASK_565)>>1));

			if  (size&1)
				p2SCREEN[addr+XPoint]
					=((p2SCREEN[addr+XPoint]&d2HM_MASK_565)+(_p16SPRITE[bufferPoint]&d2HM_MASK_565))>>1;

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr	+=	WIDTH;
	}
}

void
c16BITSPRITE565::fPutSpriteAB()
{	
	if (_Alpha<=0 )
		return;
	if (_Alpha>=32)
	{
		c16BITSPRITE555::fPutSprite();
		return;
	}

	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=4;
	int   size;
	int	  _DestAlpha=32-_Alpha;

	Xs=(short)_p16SPRITE[0];
	Ys=(short)_p16SPRITE[1];
	_x-=(Xs-(short)_p16SPRITE[2]);	//	위치 교정..
	_y-=(short)_p16SPRITE[3];	//	위치 교정..

	if (_x >= WIDTH) return;

	X1= 0;if ( _x > 0 ) X1 = _x;
	Y1= 0;if ( _y > 0 ) Y1 = _y;

	X2 = Xs+_x;
	Y2 = Ys+_y;
	if (HEIGHT < _y+Ys) Y2=HEIGHT;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-_y;j++)		//	_y축 상단 클리핑..
	{	count=_p16SPRITE[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=_p16SPRITE[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (_x<0)
	for(;Y1<Y2;Y1++)
	{	count	=	_p16SPRITE[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_p16SPRITE[bufferPoint++];
			size	=	_p16SPRITE[bufferPoint++];

			beginX	=	0;

			if (XPoint	< size	) beginX= size-XPoint;			//	왼쪽 클리핑..

			width	=	size-beginX;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+XPoint-width];
			UINT *p4_spr=(UINT *)&_p16SPRITE[bufferPoint+(width&1)];

			for (j=0;j<width/2;j++)
				p4_scr[j]	=	fMIX4BYTE565(p4_spr[width/2-1-j],p4_scr[j],_Alpha,_DestAlpha);

			if  ((width&1) && width > 0 )
				p2SCREEN[addr+XPoint-1]	=	MIX2BYTE565(_p16SPRITE[bufferPoint],p2SCREEN[addr+XPoint-1],_Alpha,_DestAlpha);

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	if (_x+Xs > WIDTH)
	for(;Y1<Y2;Y1++)
	{	count	=	_p16SPRITE[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_p16SPRITE[bufferPoint++];
			size	=	_p16SPRITE[bufferPoint++];

			width	=	XPoint;		//	tail
			endX	=	size;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+XPoint-size];
			UINT *p4_spr=(UINT *)&_p16SPRITE[bufferPoint+size%2];

			if (XPoint	>  WIDTH)
			{	endX	= WIDTH-(XPoint-size);		//	오른쪽 클리핑..
				width	= WIDTH;
			}

			for (j=0;j<endX/2;j++)
				p4_scr[j]	=	fMIX4BYTE565(p4_spr[size/2-1-j],p4_scr[j],_Alpha,_DestAlpha);

			if  ((endX&1) && endX > 0 )
				p2SCREEN[addr+width-1]	=	MIX2BYTE565(_p16SPRITE[bufferPoint+size-endX],p2SCREEN[addr+width-1],_Alpha,_DestAlpha);

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count	=	_p16SPRITE[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_p16SPRITE[bufferPoint++];
			size	=	_p16SPRITE[bufferPoint++];

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+XPoint-size+1];
			UINT *p4_spr=(UINT *)&_p16SPRITE[bufferPoint+(size&1)];

			for (j=size/2-1,X1=0;j>=0;j--,X1++)
				p4_scr[X1]	=	fMIX4BYTE565(p4_spr[j],p4_scr[j],_Alpha,_DestAlpha);

			if  ((size&1) && size > 0 )
				p2SCREEN[addr+XPoint]	=	MIX2BYTE565(_p16SPRITE[bufferPoint],p2SCREEN[addr+XPoint-1],_Alpha,_DestAlpha);

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
}

void
c16BITSPRITE565::fPutSpriteFX()
{
	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=4;
	int   size;

	Xs=(short)_p16SPRITE[0];
	Ys=(short)_p16SPRITE[1];
	_x-=(Xs-(short)_p16SPRITE[2]);	//	위치 교정..
	_y-=(short)_p16SPRITE[3];	//	위치 교정..

	if (_x >= WIDTH) return;

	X1= 0;if ( _x > 0 ) X1 = _x;
	Y1= 0;if ( _y > 0 ) Y1 = _y;

	X2 = Xs+_x;
	Y2 = Ys+_y;
	if (HEIGHT < _y+Ys) Y2=HEIGHT;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-_y;j++)		//	_y축 상단 클리핑..
	{	count=_p16SPRITE[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=_p16SPRITE[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (_x<0)
	for(;Y1<Y2;Y1++)
	{	count	=	_p16SPRITE[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_p16SPRITE[bufferPoint++];
			size	=	_p16SPRITE[bufferPoint++];

			endX	=	size;
			if (XPoint - size	< 0		) endX	= XPoint;		//	왼쪽 클리핑..

			for (j=0;j<endX;j++)
				p2SCREEN[addr+XPoint-j]	=	FXMIX565(_p16SPRITE[bufferPoint+j],p2SCREEN[addr+XPoint-j],_Effect);

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	if (_x+Xs > WIDTH)
	for(;Y1<Y2;Y1++)
	{	count	=	_p16SPRITE[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_p16SPRITE[bufferPoint++];
			size	=	_p16SPRITE[bufferPoint++];

			beginX	=	0;
			if (XPoint	> WIDTH	) beginX	= XPoint-WIDTH;	//	오른쪽 클리핑..

			for (j=beginX;j<size;j++)
				p2SCREEN[addr+XPoint-j]	=	FXMIX565(_p16SPRITE[bufferPoint+j],p2SCREEN[addr+XPoint-j],_Effect);

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count	=	_p16SPRITE[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_p16SPRITE[bufferPoint++];
			size	=	_p16SPRITE[bufferPoint++];

			for (j=0;j<size;j++)
				p2SCREEN[addr+XPoint-j]	=	FXMIX565(_p16SPRITE[bufferPoint+j],p2SCREEN[addr+XPoint-j],_Effect);

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
}


/*********************************************************************************************************
	확대/축소 출력
*********************************************************************************************************/

//	확대/축소 테이블 생성	
//	실이미지의 _x,_y 각각의 좌표가 확대된 이미지의 주소에 대응한다.

void
c16BITSPRITE565::PutSpriteRHB()
{
	int Xs ,Ys,XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int count,bufferPoint=4,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,Xp,_yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	Xs	=(short)_p16SPRITE[0]			,Ys		=(short)_p16SPRITE[1]			;	//	스프라이트 사이즈
	XLen=Xs*_XRate/100				,YLen	=Ys*_YRate/100				;	//	확대/축소된 이미즈 사이즈
	_x	-=(short)_p16SPRITE[2]*_XRate/100,_y		-=(short)_p16SPRITE[3]*_YRate/100;	//	위치 교정..
	if (_x + XLen < 0 || _x >= WIDTH || _y + YLen < 0 || _y > HEIGHT )  return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	addr=max(_y,0)*WIDTH+_x;			//	스크린 주소..

	for (_y=0;_y<Ys;_y++)
	{
		tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 

		for (_yp=0;_yp<zoomYTable[_y].size;_yp++)
		{
			count	=	_p16SPRITE[bufferPoint++];	//	이미지 카운트
			Xp		=	0;

			for(i=0;i<count;i++)
			{
				Xp	+=	_p16SPRITE[bufferPoint++];
				size=	_p16SPRITE[bufferPoint++];

				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						*(p2SCREEN+addr+zoomXTable[j+Xp].pos+k)
						=((p2SCREEN[addr+zoomXTable[j+Xp].pos+k]&d2HM_MASK_565)+(_p16SPRITE[bufferPoint+j]&d2HM_MASK_565))>>1;

				bufferPoint	+=	size;
				Xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}

		count	=	_p16SPRITE[bufferPoint++];

		for(i=0;i<count;i++)
			bufferPoint	+=	_p16SPRITE[bufferPoint+1] + 2;
	}
}

void
c16BITSPRITE565::PutSpriteRAB()
{	
	if (_Alpha<=0 )
		return;
	if (_Alpha>=32)
	{	
		c16BITSPRITE555::PutSpriteR();
		return;
	}

	int Xs ,Ys,XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int count,bufferPoint=4,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,Xp,_yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..
	int	_DestAlpha=32-_Alpha;

	Xs	=(short)_p16SPRITE[0]			,Ys		=(short)_p16SPRITE[1]			;	//	스프라이트 사이즈
	XLen=Xs*_XRate/100				,YLen	=Ys*_YRate/100				;	//	확대/축소된 이미즈 사이즈
	_x	-=(short)_p16SPRITE[2]*_XRate/100,_y		-=(short)_p16SPRITE[3]*_YRate/100;	//	위치 교정..
	if (_x + XLen < 0 || _x >= WIDTH || _y + YLen < 0 || _y > HEIGHT )  return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	addr=max(_y,0)*WIDTH+_x;			//	스크린 주소..

	for (_y=0;_y<Ys;_y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (_yp=0;_yp<zoomYTable[_y].size;_yp++)
		{	count	=	_p16SPRITE[bufferPoint++];	//	이미지 카운트
			Xp		=	0;
			for(i=0;i<count;i++)
			{	Xp	+=	_p16SPRITE[bufferPoint++];
				size=	_p16SPRITE[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						p2SCREEN[addr+zoomXTable[j+Xp].pos+k]	
							=	MIX2BYTE565(_p16SPRITE[bufferPoint+j],p2SCREEN[addr+zoomXTable[j+Xp].pos+k],_Alpha,_DestAlpha);

				bufferPoint	+=	size;
				Xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	_p16SPRITE[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	_p16SPRITE[bufferPoint+1] + 2;
	}
}

void
c16BITSPRITE565::PutSpriteRFX()
{
	int Xs ,Ys,XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int count,bufferPoint=4,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,Xp,_yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	Xs	=(short)_p16SPRITE[0]			,Ys		=(short)_p16SPRITE[1]			;	//	스프라이트 사이즈
	XLen=Xs*_XRate/100				,YLen	=Ys*_YRate/100				;	//	확대/축소된 이미즈 사이즈
	_x	-=(short)_p16SPRITE[2]*_XRate/100,_y		-=(short)_p16SPRITE[3]*_YRate/100;	//	위치 교정..
	if (_x + XLen < 0 || _x >= WIDTH || _y + YLen < 0 || _y > HEIGHT )  return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	addr=max(_y,0)*WIDTH+_x;			//	스크린 주소..

	for (_y=0;_y<Ys;_y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (_yp=0;_yp<zoomYTable[_y].size;_yp++)
		{	count	=	_p16SPRITE[bufferPoint++];	//	이미지 카운트
			Xp		=	0;
			for(i=0;i<count;i++)
			{	Xp	+=	_p16SPRITE[bufferPoint++];
				size=	_p16SPRITE[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						p2SCREEN[addr+zoomXTable[j+Xp].pos+k]	
							=	FXMIX565(_p16SPRITE[bufferPoint+j],p2SCREEN[addr+zoomXTable[j+Xp].pos+k],_Effect);

				bufferPoint	+=	size;
				Xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	_p16SPRITE[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	_p16SPRITE[bufferPoint+1] + 2;
	}
}

void
c16BITSPRITE565::fPutSpriteRHB()
{	int Xs ,Ys,XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int count,bufferPoint=4,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,Xp,_yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	Xs	=(short)_p16SPRITE[0]			,Ys		=(short)_p16SPRITE[1]			;	//	스프라이트 사이즈
	XLen=Xs*_XRate/100				,YLen	=Ys*_YRate/100				;	//	확대/축소된 이미즈 사이즈
	_x	-=(Xs-(short)_p16SPRITE[2])*_XRate/100,_y		-=(short)_p16SPRITE[3]*_YRate/100;	//	위치 교정..
	if (_x + XLen < 0 || _x >= WIDTH || _y + YLen < 0 || _y > HEIGHT )  return;	//	이미지가 화면에 없다.

	fMakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	addr=max(_y,0)*WIDTH+_x;			//	스크린 주소..

	for (_y=0;_y<Ys;_y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (_yp=0;_yp<zoomYTable[_y].size;_yp++)
		{	count	=	_p16SPRITE[bufferPoint++];	//	이미지 카운트
			Xp		=	0;
			for(i=0;i<count;i++)
			{	Xp	+=	_p16SPRITE[bufferPoint++];
				size=	_p16SPRITE[bufferPoint++];

				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						*(p2SCREEN+addr+zoomXTable[j+Xp].pos-k-1)
							=((p2SCREEN[addr+zoomXTable[j+Xp].pos-k-1]&d2HM_MASK_565)+(_p16SPRITE[bufferPoint+j]&d2HM_MASK_565))>>1;

				bufferPoint	+=	size;
				Xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	_p16SPRITE[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	_p16SPRITE[bufferPoint+1] + 2;
	}
}

void
c16BITSPRITE565::fPutSpriteRAB()
{	
	if (_Alpha<=0 )
		return;
	if (_Alpha>=32)
	{
		c16BITSPRITE555::fPutSpriteR();
		return;
	}

	int Xs ,Ys,XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int count,bufferPoint=4,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,Xp,_yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..
	int	_DestAlpha=32-_Alpha;

	Xs	=(short)_p16SPRITE[0]			,Ys		=(short)_p16SPRITE[1]			;	//	스프라이트 사이즈
	XLen=Xs*_XRate/100				,YLen	=Ys*_YRate/100				;	//	확대/축소된 이미즈 사이즈
	_x	-=(Xs-(short)_p16SPRITE[2])*_XRate/100,_y		-=(short)_p16SPRITE[3]*_YRate/100;	//	위치 교정..
	if (_x + XLen < 0 || _x >= WIDTH || _y + YLen < 0 || _y > HEIGHT )  return;	//	이미지가 화면에 없다.

	fMakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	addr=max(_y,0)*WIDTH+_x;			//	스크린 주소..

	for (_y=0;_y<Ys;_y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (_yp=0;_yp<zoomYTable[_y].size;_yp++)
		{	count	=	_p16SPRITE[bufferPoint++];	//	이미지 카운트
			Xp		=	0;

			for(i=0;i<count;i++)
			{	Xp	+=	_p16SPRITE[bufferPoint++];
				size=	_p16SPRITE[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						p2SCREEN[addr+zoomXTable[j+Xp].pos-1-k]	
							=	MIX2BYTE565(_p16SPRITE[bufferPoint+j],p2SCREEN[addr+zoomXTable[j+Xp].pos-1-k],_Alpha,_DestAlpha);
				bufferPoint	+=	size;
				Xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	_p16SPRITE[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	_p16SPRITE[bufferPoint+1] + 2;
	}
}

void
c16BITSPRITE565::fPutSpriteRFX()
{	int Xs ,Ys,XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int count,bufferPoint=4,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,Xp,_yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	Xs	=(short)_p16SPRITE[0]			,Ys		=(short)_p16SPRITE[1]			;	//	스프라이트 사이즈
	XLen=Xs*_XRate/100				,YLen	=Ys*_YRate/100				;	//	확대/축소된 이미즈 사이즈
	_x	-=(Xs-(short)_p16SPRITE[2])*_XRate/100,_y		-=(short)_p16SPRITE[3]*_YRate/100;	//	위치 교정..
	if (_x + XLen < 0 || _x >= WIDTH || _y + YLen < 0 || _y > HEIGHT )  return;	//	이미지가 화면에 없다.

	fMakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	addr=max(_y,0)*WIDTH+_x;			//	스크린 주소..

	for (_y=0;_y<Ys;_y++)
	{
		tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		
		for (_yp=0;_yp<zoomYTable[_y].size;_yp++)
		{
			count	=	_p16SPRITE[bufferPoint++];	//	이미지 카운트
			Xp		=	0;

			for(i=0;i<count;i++)
			{
				Xp	+=	_p16SPRITE[bufferPoint++];
				size=	_p16SPRITE[bufferPoint++];

				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						p2SCREEN[addr+zoomXTable[j+Xp].pos-k-1]	
							=	FXMIX565(_p16SPRITE[bufferPoint+j],p2SCREEN[addr+zoomXTable[j+Xp].pos-k-1],_Effect);
				bufferPoint	+=	size;
				Xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}

		count	=	_p16SPRITE[bufferPoint++];
		for(i=0;i<count;i++)
			bufferPoint	+=	_p16SPRITE[bufferPoint+1] + 2;
	}
}