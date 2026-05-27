#include "cDRAW.H"

/*******************************************************************************************

	8bit 압축 스프라이트 출력 루틴들..
	압축 스프라이트 형식

		8bit 압축 스프라이트..
			가로	사이즈 2 b_yte  ---+
			세로	사이즈 2 b_yte     |
			가로 위치 교정 2 b_yte     |
			세로 위치 교정 2 b_yte  ---+ -> *(short *)
			데이터						--------+
				카운트(한 라인에 이미지 수)		|
					공백 크기					|
					이미지 크기					|
					이미지				--------+ -> 1 b_yte 이러려면..

void	PutSpriteXX(int _x,int _y,BYTE *_p8SPRITE,WORD *_pPLT,int _Alpha)
int	_x,_y
	:	이미지의 좌표
BYTE *_p8SPRITE
	:	이미지 데이터
WORD *_pPLT
	:	팔레트
int _Alpha
	:	알파 블렌딩시 알파값

*******************************************************************************************/

//	Error code c8BITSPRITE565

void
c8BITSPRITE565::PutSpriteHB()
{	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;

	Xs=*(short *)(_p8SPRITE  );
	Ys=*(short *)(_p8SPRITE+2);
	_x-=*(short *)(_p8SPRITE+4);	//	위치 교정..
	_y-=*(short *)(_p8SPRITE+6);	//	위치 교정..

	if (_x >= WIDTH) return;

	X1= 0;if ( _x > 0 ) X1 = _x;
	Y1= 0;if ( _y > 0 ) Y1 = _y;

	X2 = Xs+_x;
	Y2 = Ys+_y;
	if (HEIGHT < _y+Ys) Y2=HEIGHT;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-_y;j++)		//	_y축 상단 클리핑..
	{	count=_p8SPRITE[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=_p8SPRITE[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (_x<0 || _x+Xs > WIDTH)
	for(;Y1<Y2;Y1++)
	{	count	=	_p8SPRITE[bufferPoint++];
		XPoint	=	_x;
		
		for(i=0;i<count;i++)
		{	XPoint	+=	_p8SPRITE[bufferPoint++];
			size	=	_p8SPRITE[bufferPoint++];

			beginX	=	0;
			endX	=	(int) size;

			if (XPoint			< 0		) beginX= -XPoint;			//	왼쪽 클리핑..
			if (XPoint+ size	>  WIDTH) endX	= WIDTH-XPoint;		//	오른쪽 클리핑..

			for (j=beginX;j<endX;j++)
				p2SCREEN[addr+XPoint+j]
					=((p2SCREEN[addr+XPoint+j]&d2HM_MASK_565)+(_pPLT[_p8SPRITE[bufferPoint+j]]&d2HM_MASK_565))>>1;

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count	=	_p8SPRITE[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{	XPoint	+=	_p8SPRITE[bufferPoint++];
			size	=	_p8SPRITE[bufferPoint++];

			for (j=0;j<size;j++)
				p2SCREEN[addr+XPoint+j]
					=((p2SCREEN[addr+XPoint+j]&d2HM_MASK_565)+(_pPLT[_p8SPRITE[bufferPoint+j]]&d2HM_MASK_565))>>1;
			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
}

void
c8BITSPRITE565::PutSpriteAB()
{	if (_Alpha<=0 ) return;
	if (_Alpha>=32)
	{	c8BITSPRITE555::PutSprite();
		return;
	}

	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;

	Xs=*(short *)(_p8SPRITE  );
	Ys=*(short *)(_p8SPRITE+2);
	_x-=*(short *)(_p8SPRITE+4);	//	위치 교정..
	_y-=*(short *)(_p8SPRITE+6);	//	위치 교정..

	if (_x >= WIDTH) return;

	X1= 0;if ( _x > 0 ) X1 = _x;
	Y1= 0;if ( _y > 0 ) Y1 = _y;

	X2 = Xs+_x;
	Y2 = Ys+_y;
	if (HEIGHT < _y+Ys) Y2=HEIGHT;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-_y;j++)		//	_y축 상단 클리핑..
	{	count=_p8SPRITE[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=_p8SPRITE[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (_x<0 || _x+Xs > WIDTH)
	for(;Y1<Y2;Y1++)
	{	count	=	_p8SPRITE[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{	XPoint	+=	_p8SPRITE[bufferPoint++];
			size	=	_p8SPRITE[bufferPoint++];

			beginX	=	0;
			endX	=	(int) size;

			if (XPoint			< 0		) beginX= -XPoint;			//	왼쪽 클리핑..
			if (XPoint+ size	>  WIDTH) endX	= WIDTH-XPoint;		//	오른쪽 클리핑..

			for (j=beginX;j<endX;j++)
				p2SCREEN[addr+XPoint+j]	=	MIX2BYTE565(_pPLT[_p8SPRITE[bufferPoint+j]],p2SCREEN[addr+XPoint+j],_Alpha,_DestAlpha);

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count	=	_p8SPRITE[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{	XPoint	+=	_p8SPRITE[bufferPoint++];
			size	=	_p8SPRITE[bufferPoint++];

			for (j=0;j<size;j++)
				p2SCREEN[addr+XPoint+j]	=	MIX2BYTE565(_pPLT[_p8SPRITE[bufferPoint+j]],p2SCREEN[addr+XPoint+j],_Alpha,_DestAlpha);

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
}

void
c8BITSPRITE565::PutSpriteFX()
{
	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;

	Xs=*(short *)(_p8SPRITE  );
	Ys=*(short *)(_p8SPRITE+2);
	_x-=*(short *)(_p8SPRITE+4);	//	위치 교정..
	_y-=*(short *)(_p8SPRITE+6);	//	위치 교정..

	if (_x >= WIDTH) return;

	X1= 0;if ( _x > 0 ) X1 = _x;
	Y1= 0;if ( _y > 0 ) Y1 = _y;

	X2 = Xs+_x;
	Y2 = Ys+_y;
	if (HEIGHT < _y+Ys) Y2=HEIGHT;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-_y;j++)		//	_y축 상단 클리핑..
	{	count=_p8SPRITE[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=_p8SPRITE[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (_x<0 || _x+Xs > WIDTH)
	for(;Y1<Y2;Y1++)
	{	count	=	_p8SPRITE[bufferPoint++];
		XPoint	=	_x;
		for(i=0;i<count;i++)
		{	XPoint	+=	_p8SPRITE[bufferPoint++];
			size	=	_p8SPRITE[bufferPoint++];

			beginX	=	0;
			endX	=	(int) size;

			if (XPoint			< 0		) beginX= -XPoint;			//	왼쪽 클리핑..
			if (XPoint+ size	>  WIDTH) endX	= WIDTH-XPoint;		//	오른쪽 클리핑..

			for (j=beginX;j<endX;j++)
				p2SCREEN[addr+XPoint+j]	=	FXMIX565(_pPLT[_p8SPRITE[bufferPoint+j]],p2SCREEN[addr+XPoint+j],_Effect);

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count	=	_p8SPRITE[bufferPoint++];
		XPoint	=	_x;

		for(i=0;i<count;i++)
		{	XPoint	+=	_p8SPRITE[bufferPoint++];
			size	=	_p8SPRITE[bufferPoint++];

			for (j=0;j<size;j++)
				p2SCREEN[addr+XPoint+j]	=	FXMIX565(_pPLT[_p8SPRITE[bufferPoint+j]],p2SCREEN[addr+XPoint+j],_Effect);

			bufferPoint	+=	size;
			XPoint		+=	size;
		}
		addr+=WIDTH;
	}
}

/*** 압축 스프라이트 출력 루틴들.. 플립**/

void
c8BITSPRITE565::fPutSpriteHB()
{	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;

	Xs=*(short *)(_p8SPRITE  );
	Ys=*(short *)(_p8SPRITE+2);
	_x-=(Xs-*(short *)(_p8SPRITE+4));	//	위치 교정..
	_y-=*(short *)(_p8SPRITE+6);	//	위치 교정..

	if (_x >= WIDTH) return;

	X1= 0;if ( _x > 0 ) X1 = _x;
	Y1= 0;if ( _y > 0 ) Y1 = _y;

	X2 = Xs+_x;
	Y2 = Ys+_y;
	if (HEIGHT < _y+Ys) Y2=HEIGHT;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-_y;j++)		//	_y축 상단 클리핑..
	{	count=_p8SPRITE[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=_p8SPRITE[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (_x<0 || _x+Xs > WIDTH)
	for(;Y1<Y2;Y1++)
	{	count	=	_p8SPRITE[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_p8SPRITE[bufferPoint++];
			size	=	_p8SPRITE[bufferPoint++];

			endX	=	size;
			if (XPoint- size	<	0		)	endX	= XPoint;		//	왼쪽 클리핑..
			beginX	=	0;
			if (XPoint			>	WIDTH	)	beginX	= XPoint-WIDTH;	//	오른쪽 클리핑..
			
			for (j=beginX;j<endX;j++)
				p2SCREEN[addr+XPoint-j]
					=((p2SCREEN[addr+XPoint-j]&d2HM_MASK_565)+(_pPLT[_p8SPRITE[bufferPoint+j]]&d2HM_MASK_565))>>1;

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count	=	_p8SPRITE[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_p8SPRITE[bufferPoint++];
			size	=	_p8SPRITE[bufferPoint++];

			for (j=0;j<size;j++)
				p2SCREEN[addr+XPoint-j]
					=((p2SCREEN[addr+XPoint-j]&d2HM_MASK_565)+(_pPLT[_p8SPRITE[bufferPoint+j]]&d2HM_MASK_565))>>1;

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
}

void
c8BITSPRITE565::fPutSpriteAB()
{	if (_Alpha<=0 ) return;
	if (_Alpha>=32) c8BITSPRITE555::fPutSprite();

	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;

	Xs=*(short *)(_p8SPRITE  );
	Ys=*(short *)(_p8SPRITE+2);
	_x-=(Xs-*(short *)(_p8SPRITE+4));	//	위치 교정..
	_y-=*(short *)(_p8SPRITE+6);	//	위치 교정..

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
		count=_p8SPRITE[bufferPoint++];
		for(i=0;i<count;i++)
		{
			bufferPoint++;
			size=_p8SPRITE[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (_x<0 || _x+Xs > WIDTH)
	for(;Y1<Y2;Y1++)
	{	count	=	_p8SPRITE[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_p8SPRITE[bufferPoint++];
			size	=	_p8SPRITE[bufferPoint++];

			endX	=	size;
			if (XPoint- size	<	0		)	endX	= XPoint;		//	왼쪽 클리핑..
			beginX	=	0;
			if (XPoint			>	WIDTH	)	beginX	= XPoint-WIDTH;	//	오른쪽 클리핑..
			
			for (j=beginX;j<endX;j++)
				p2SCREEN[addr+XPoint-j]	
					=	MIX2BYTE565(_pPLT[_p8SPRITE[bufferPoint+j]],p2SCREEN[addr+XPoint-j],_Alpha,_DestAlpha);

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count	=	_p8SPRITE[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_p8SPRITE[bufferPoint++];
			size	=	_p8SPRITE[bufferPoint++];

			for (j=0;j<size;j++)
				p2SCREEN[addr+XPoint-j]	
					=	MIX2BYTE565(_pPLT[_p8SPRITE[bufferPoint+j]],p2SCREEN[addr+XPoint-j],_Alpha,_DestAlpha);

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
}

void
c8BITSPRITE565::fPutSpriteFX()
{
	int   X1, Y1, X2, Y2 ,Xs ,Ys;
	int   XPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;

	Xs=*(short *)(_p8SPRITE  );
	Ys=*(short *)(_p8SPRITE+2);
	_x-=(Xs-*(short *)(_p8SPRITE+4));	//	위치 교정..
	_y-=*(short *)(_p8SPRITE+6);	//	위치 교정..

	if	(_x >= WIDTH)
		return;

	X1	= 0;if ( _x > 0 ) X1 = _x;
	Y1	= 0;if ( _y > 0 ) Y1 = _y;

	X2 = Xs+_x;
	Y2 = Ys+_y;
	if (HEIGHT < _y+Ys) Y2=HEIGHT;	//	_y축 하단 클리핑..

	if (X2 <= X1 || Y2 <= Y1 ) return;	//	이미지가 화면에 없다.
	int addr=Y1*WIDTH;

	for(j=0;j<-_y;j++)		//	_y축 상단 클리핑..
	{	count=_p8SPRITE[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=_p8SPRITE[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (_x<0 || _x+Xs > WIDTH)
	for(;Y1<Y2;Y1++)
	{	count	=	_p8SPRITE[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_p8SPRITE[bufferPoint++];
			size	=	_p8SPRITE[bufferPoint++];

			endX	=	size;
			if (XPoint- size	<	0		)	endX	= XPoint;		//	왼쪽 클리핑..
			beginX	=	0;
			if (XPoint			>	WIDTH	)	beginX	= XPoint-WIDTH;	//	오른쪽 클리핑..
			
			for (j=beginX;j<endX;j++)
				p2SCREEN[addr+XPoint-j]	=	FXMIX565(_pPLT[_p8SPRITE[bufferPoint+j]],p2SCREEN[addr+XPoint-j],_Effect);

			bufferPoint	+=	size;
			XPoint		-=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;Y1<Y2;Y1++)
	{	count	=	_p8SPRITE[bufferPoint++];
		XPoint	=	_x+Xs;

		for(i=0;i<count;i++)
		{	XPoint	-=	_p8SPRITE[bufferPoint++];
			size	=	_p8SPRITE[bufferPoint++];

			for (j=0;j<size;j++)
				p2SCREEN[addr+XPoint-j]	=	FXMIX565(_pPLT[_p8SPRITE[bufferPoint+j]],p2SCREEN[addr+XPoint-j],_Effect);

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
c8BITSPRITE565::PutSpriteRHB()
{
	if	(_XRate < 0	)
	{
		_XRate	=	-_XRate;
		fPutSpriteRHB();
		return;
	}

	int		i,j;
	int		XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int		count,bufferPoint=8,size;		//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int		length=0,tempBP,Xp,Yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	int		iXG		=	0;
	int		iYG		=	0;
	int		iGY		=	0;
	int		Xs		=	*(short *)(_p8SPRITE	);				//	스프라이트 사이즈
	int		Ys		=	*(short *)(_p8SPRITE+2	);
	int		iDX		=	*(short *)(_p8SPRITE+4	);
	int		iDY		=	*(short *)(_p8SPRITE+6	);

	if	(s_iMaxWidth)
	{
		iXG		=	s_iMaxWidth/2-iDX;
		iYG		=	s_iMaxHeight/2-iDY;
		iGY		=	max(s_iMaxHeight-Ys-iYG,0);

		Xs		=	s_iMaxWidth;
		Ys		=	s_iMaxHeight;
		iDX		=	s_iMaxWidth/2;
		iDY		=	s_iMaxHeight/2;
	}

	_x		=	_x-(iDX*_XRate/100);
	_y		=	_y-(iDY*_YRate/100);

	XLen	=	Xs*_XRate/100;							//	확대/축소된 이미즈 사이즈
	YLen	=	Ys*_YRate/100;

	if (_x + XLen < 0 || _x >= WIDTH || _y + YLen < 0 || _y > HEIGHT )
		return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	addr	=	max(_y,0)*WIDTH+_x;			//	스크린 주소..

	Ys		-=	iGY;

	for (_y=0;_y<Ys;_y++)
	{
		tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 

		for (Yp=0;Yp<zoomYTable[_y].size;Yp++)
		{
			if	(iYG	>	0)
			{
				addr		+=	WIDTH;
				continue;
			}

			count	=	_p8SPRITE[bufferPoint++];	//	이미지 카운트
			Xp		=	iXG;

			for(i=0;i<count;i++)
			{
				Xp	+=	_p8SPRITE[bufferPoint++];
				size=	_p8SPRITE[bufferPoint++];

				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						*(p2SCREEN+addr+zoomXTable[j+Xp].pos+k)
							=((p2SCREEN[addr+zoomXTable[j+Xp].pos+k]&d2HM_MASK_565)+(_pPLT[*(_p8SPRITE+bufferPoint+j)]&d2HM_MASK_565))>>1;

				bufferPoint	+=	size;
				Xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}

		if	(iYG	>	0)
		{
			--iYG;
			continue;
		}

		count	=	_p8SPRITE[bufferPoint++];

		for	(i=0;i<count;i++)
			bufferPoint	+=	_p8SPRITE[bufferPoint+1] + 2;
	}
}

void
c8BITSPRITE565::PutSpriteRAB()
{	
	if	(_Alpha<=0 )
		return;
	
	if	(_Alpha>=32)
	{	
		c8BITSPRITE555::PutSpriteR();
		return;
	}

	int		XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int		count,bufferPoint=8,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int		length=0,tempBP,Xp,Yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	int		iXG		=	0;
	int		iYG		=	0;
	int		iGY		=	0;
	int		Xs		=	*(short *)(_p8SPRITE	);				//	스프라이트 사이즈
	int		Ys		=	*(short *)(_p8SPRITE+2	);
	int		iDX		=	*(short *)(_p8SPRITE+4	);
	int		iDY		=	*(short *)(_p8SPRITE+6	);

	if	(s_iMaxWidth)
	{
		iXG		=	s_iMaxWidth/2-iDX;
		iYG		=	s_iMaxHeight/2-iDY;
		iGY		=	max(s_iMaxHeight-Ys-iYG,0);

		Xs		=	s_iMaxWidth;
		Ys		=	s_iMaxHeight;
		iDX		=	s_iMaxWidth/2;
		iDY		=	s_iMaxHeight/2;
	}

	_x		=	_x-(iDX*_XRate/100);
	_y		=	_y-(iDY*_YRate/100);

	XLen	=	Xs*_XRate/100;							//	확대/축소된 이미즈 사이즈
	YLen	=	Ys*_YRate/100;

	MakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	Ys		-=	iGY;

	addr	=	max(_y,0)*WIDTH+_x;			//	스크린 주소..

	for (_y=0;_y<Ys;_y++)
	{
		tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (Yp=0;Yp<zoomYTable[_y].size;Yp++)
		{
			if	(iYG	>	0)
			{
				addr		+=	WIDTH;
				continue;
			}

			count	=	_p8SPRITE[bufferPoint++];	//	이미지 카운트
			Xp		=	iXG;
			for(i=0;i<count;i++)
			{
				Xp	+=	_p8SPRITE[bufferPoint++];
				size=	_p8SPRITE[bufferPoint++];

				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						p2SCREEN[addr+zoomXTable[j+Xp].pos+k]
							=	MIX2BYTE565(_pPLT[_p8SPRITE[bufferPoint+j]],p2SCREEN[addr+zoomXTable[j+Xp].pos+k],_Alpha,_DestAlpha);

				bufferPoint	+=	size;
				Xp			+=	size;
			}

			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}

		if	(iYG	>	0)
		{
			--iYG;
			continue;
		}

		count	=	_p8SPRITE[bufferPoint++];

		for	(i=0;i<count;i++)
			bufferPoint	+=	_p8SPRITE[bufferPoint+1] + 2;
	}
}

void
c8BITSPRITE565::PutSpriteRFX()
{
	int		i,j;
	int		XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int		count,bufferPoint=8,size;		//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int		length=0,tempBP,Xp,Yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	int		iXG		=	0;
	int		iYG		=	0;
	int		iGY		=	0;
	int		Xs		=	*(short *)(_p8SPRITE	);				//	스프라이트 사이즈
	int		Ys		=	*(short *)(_p8SPRITE+2	);
	int		iDX		=	*(short *)(_p8SPRITE+4	);
	int		iDY		=	*(short *)(_p8SPRITE+6	);

	if	(s_iMaxWidth)
	{
		iXG		=	s_iMaxWidth/2-iDX;
		iYG		=	s_iMaxHeight/2-iDY;
		iGY		=	max(s_iMaxHeight-Ys-iYG,0);

		Xs		=	s_iMaxWidth;
		Ys		=	s_iMaxHeight;
		iDX		=	s_iMaxWidth/2;
		iDY		=	s_iMaxHeight/2;
	}

	_x		=	_x-(iDX*_XRate/100);
	_y		=	_y-(iDY*_YRate/100);

	XLen	=	Xs*_XRate/100;							//	확대/축소된 이미즈 사이즈
	YLen	=	Ys*_YRate/100;

	if	(_x + XLen < 0 || _x >= WIDTH || _y + YLen < 0 || _y > HEIGHT )
		return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	Ys		-=	iGY;
	addr	=	max(_y,0)*WIDTH+_x;			//	스크린 주소..

	for (_y=0;_y<Ys;_y++)
	{
		tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (Yp=0;Yp<zoomYTable[_y].size;Yp++)
		{
			if	(iYG	>	0)
			{
				addr		+=	WIDTH;
				continue;
			}

			count	=	_p8SPRITE[bufferPoint++];	//	이미지 카운트
			Xp		=	iXG;

			for	(i=0;i<count;i++)
			{
				Xp	+=	_p8SPRITE[bufferPoint++];
				size=	_p8SPRITE[bufferPoint++];

				int	iPreColor	=	-1;

				for (j=0;j<size;j++)
				{
					WORD	wColor;

					if	(zoomXTable[j+Xp].size	==	0)
					{
						iPreColor	=	_p8SPRITE[bufferPoint+j];
						continue;
					}
					else
					{
						wColor		=	_pPLT[_p8SPRITE[bufferPoint+j]];

						if	(iPreColor	!=	-1)
						{
							WORD	wColor2	=	_pPLT[iPreColor];

							iPreColor		=	-1;

							wColor			=	GetMixColor(wColor,wColor2);
						}
					}

					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						p2SCREEN[addr+zoomXTable[j+Xp].pos+k]
							=	FXMIX565(wColor,p2SCREEN[addr+zoomXTable[j+Xp].pos+k],_Effect);
				}

				bufferPoint	+=	size;
				Xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}

		if	(iYG	>	0)
		{
			--iYG;
			continue;
		}

		count	=	_p8SPRITE[bufferPoint++];
		for	(i=0;i<count;i++)
			bufferPoint	+=	_p8SPRITE[bufferPoint+1] + 2;
	}
}

void
c8BITSPRITE565::fPutSpriteRHB()
{	int	i,j;
	int Xs,Ys,XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int count,bufferPoint=8,size;		//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,Xp,Yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	Xs	=*(short *)(_p8SPRITE		);				//	스프라이트 사이즈
	Ys	=*(short *)(_p8SPRITE+2	);
	_x	-=(Xs-*(short *)(_p8SPRITE + 4))*_XRate/100;	//	위치 교정..
	_y	-=(*(short *)(_p8SPRITE + 6))*_YRate/100;

	XLen=Xs*_XRate/100;							//	확대/축소된 이미즈 사이즈
	YLen=Ys*_YRate/100;


	if (_x + XLen < 0 || _x >= WIDTH || _y + YLen < 0 || _y > HEIGHT )  return;	//	이미지가 화면에 없다.

	MakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	addr=max(_y,0)*WIDTH+_x;			//	스크린 주소..

	for (_y=0;_y<Ys;_y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (Yp=0;Yp<zoomYTable[_y].size;Yp++)
		{	count	=	_p8SPRITE[bufferPoint++];	//	이미지 카운트
			Xp		=	0;
			for(i=0;i<count;i++)
			{	Xp	+=	_p8SPRITE[bufferPoint++];
				size=	_p8SPRITE[bufferPoint++];
				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						*(p2SCREEN+addr+zoomXTable[Xp+j].pos-k-1)
						=((*(p2SCREEN+addr+zoomXTable[Xp+j].pos-k-1)&d2HM_MASK_565)+(_pPLT[*(_p8SPRITE+bufferPoint+j)]&d2HM_MASK_565))>>1;

				bufferPoint	+=	size;
				Xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	_p8SPRITE[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	_p8SPRITE[bufferPoint+1] + 2;
	}
}

void
c8BITSPRITE565::fPutSpriteRAB()
{	if (_Alpha<=0 ) return;
	if (_Alpha>=32)
	{	c8BITSPRITE555::fPutSpriteR();
		return;
	}

	int Xs ,Ys,XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int count,bufferPoint=8,size,i,j;	//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,Xp,Yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	Xs	=*(short *)(_p8SPRITE		);				//	스프라이트 사이즈
	Ys	=*(short *)(_p8SPRITE+2	);
	_x	-=(Xs-*(short *)(_p8SPRITE + 4))*_XRate/100;	//	위치 교정..
	_y	-=(*(short *)(_p8SPRITE + 6))*_YRate/100;

	XLen=Xs*_XRate/100;							//	확대/축소된 이미즈 사이즈
	YLen=Ys*_YRate/100;

	fMakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	addr=max(_y,0)*WIDTH+_x;			//	스크린 주소..

	for (_y=0;_y<Ys;_y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (Yp=0;Yp<zoomYTable[_y].size;Yp++)
		{	count	=	_p8SPRITE[bufferPoint++];	//	이미지 카운트
			Xp		=	0;
			for(i=0;i<count;i++)
			{	Xp	+=	_p8SPRITE[bufferPoint++];
				size=	_p8SPRITE[bufferPoint++];

				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						p2SCREEN[addr+zoomXTable[Xp+j].pos-k-1]	
							=	MIX2BYTE565(_pPLT[_p8SPRITE[bufferPoint+j]],p2SCREEN[addr+zoomXTable[Xp+j].pos-k-1]	,_Alpha,_DestAlpha);

				bufferPoint	+=	size;
				Xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	_p8SPRITE[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	_p8SPRITE[bufferPoint+1] + 2;
	}
}

void
c8BITSPRITE565::fPutSpriteRFX()
{	int	i,j;
	int Xs,Ys,XLen,YLen;				//	이미지 사이즈(Xs,Ys)  확대된 이미지의 사이즈(XLen,YLen)
	int count,bufferPoint=8,size;		//	디코딩.. count 이미지 수 bufferPoint 버퍼 포인트 size 이미지 사이즈
	int length=0,tempBP,Xp,Yp,addr;		//	length 확대축소를 위한 변수, tempBP 템플릿 버퍼 포인트,addr 화면 주소..

	Xs	=*(short *)(_p8SPRITE		);				//	스프라이트 사이즈
	Ys	=*(short *)(_p8SPRITE+2	);
	_x	-=(Xs-*(short *)(_p8SPRITE + 4))*_XRate/100;	//	위치 교정..
	_y	-=(*(short *)(_p8SPRITE + 6))*_YRate/100;

	XLen=Xs*_XRate/100;							//	확대/축소된 이미즈 사이즈
	YLen=Ys*_YRate/100;


	if (_x + XLen < 0 || _x >= WIDTH || _y + YLen < 0 || _y > HEIGHT )  return;	//	이미지가 화면에 없다.

	fMakeSpriteRTable(_x,_y,Xs,Ys,XLen,YLen);

	addr=max(_y,0)*WIDTH+_x;			//	스크린 주소..

	for (_y=0;_y<Ys;_y++)
	{	tempBP	=	bufferPoint;	//	버퍼 포인트 백업.. 
		for (Yp=0;Yp<zoomYTable[_y].size;Yp++)
		{	count	=	_p8SPRITE[bufferPoint++];	//	이미지 카운트
			Xp		=	0;
			for(i=0;i<count;i++)
			{	Xp	+=	_p8SPRITE[bufferPoint++];
				size=	_p8SPRITE[bufferPoint++];

				for (j=0;j<size;j++)
					for (int k=0;k<zoomXTable[j+Xp].size;k++)
						p2SCREEN[addr+zoomXTable[Xp+j].pos-k-1]
							=	FXMIX565(_pPLT[_p8SPRITE[bufferPoint+j]],p2SCREEN[addr+zoomXTable[Xp+j].pos-k-1],_Effect);

				bufferPoint	+=	size;
				Xp			+=	size;
			}
			addr		+=	WIDTH,	bufferPoint	=	tempBP;
		}
		count	=	_p8SPRITE[bufferPoint++];
		for(i=0;i<count;i++)	bufferPoint	+=	_p8SPRITE[bufferPoint+1] + 2;
	}
}