#include "cDRAW.H"
#include "cMESSAGE.H"

WORD	*cDRAW::p2SCREEN		=NULL;
UINT	*cDRAW::p4SCREEN		=NULL;
int		cDRAW::WIDTH			=0	;
int		cDRAW::HEIGHT			=0	;
int		cDRAW::halfWidth		=0	;
int		cDRAW::DotPerScreen		=0	;
int		cDRAW::BytePerScreen	=0	;
int		cDRAW::HalfScreen		=0	;

/*		함수 포인터			*/

void (*cDRAW::pPutSprite16[6][2])(int x,int y,WORD *sprite,int alpha) =
{	{PutSprite16	,fPutSprite16	},
	{PutSpriteHM16	,fPutSpriteHM16	},
	{PutSpriteAM16	,fPutSpriteAM16	},
	{PutSpriteHC16	,fPutSpriteHC16	},
	{PutSpriteAC16	,fPutSpriteAC16	},
	{PutSpriteAB16	,fPutSpriteAB16	}
};

void (*cDRAW::pPutSprite8[6][2] )(int x,int y,BYTE *sprite,WORD *plt,int alpha) =
{	{PutSprite8		,fPutSprite8	},
	{PutSpriteHM8	,fPutSpriteHM8	},
	{PutSpriteAM8	,fPutSpriteAM8	},
	{PutSpriteHM8	,fPutSpriteHM8	},
	{PutSpriteAM8	,fPutSpriteAM8	},
	{PutSpriteAB8	,fPutSpriteAB8	}
};

BOOL
cDRAW::Init(cDIB *dib)
{	if (dib==NULL) return FALSE;

	WIDTH			=	dib->WIDTH;
	HEIGHT			=	dib->HEIGHT;
	halfWidth		=	WIDTH/2;
	p2SCREEN		=	dib->p2SCREEN;
	p4SCREEN		=	(UINT *)dib->p2SCREEN;
	DotPerScreen	=	WIDTH*HEIGHT;
	BytePerScreen	=	WIDTH*HEIGHT*2;
	HalfScreen		=	WIDTH*HEIGHT/2;

	return TRUE;
}

BOOL
cDRAW::Init(int width,int height,WORD *screen)
{	if (!screen) return FALSE;

	WIDTH			=	width;
	HEIGHT			=	height;
	halfWidth		=	WIDTH/2;
	p2SCREEN		=	screen;
	p4SCREEN		=	(UINT *)screen;
	DotPerScreen	=	WIDTH*HEIGHT;
	BytePerScreen	=	WIDTH*HEIGHT*2;
	HalfScreen		=	WIDTH*HEIGHT/2;

	return TRUE;
}

void
cDRAW::Clear()
{	UINT *p4	=p4SCREEN;
	int	 dsize	=HalfScreen;

	__asm
	{	mov  edi, dword ptr [p4]
		mov  eax, 0x00000000
		mov  ecx, dsize
		rep	 stosd                       // 4바이트 단위로 전송한다.
	}
}

void
cDRAW::Line(WORD color,int x1,int y1,int x2,int y2)
{	int	slant,address,len=0,x=0;

	if (x2==x1)	YLine(color,y1,y2,x1);return;
	if (y2==y1)	YLine(color,x1,x2,y1);return;

	slant	=	(x2-x1)*100/(y2-y1);

	int width	=	max(x2,x1)-min(x2,x1);
	int height	=	max(y2,y1)-min(y2,y1);

	if (slant > 0)
	{	if (x1>x2)
		{	swap(x1,x2);
			swap(y1,y2);
		}

		address=x1+y1*WIDTH;

		if (width< height)
			for (int i=0;i<height;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 <  0		) return;

				if (x1 >= 0 && y1 < HEIGHT) p2SCREEN[address+x]	=	color;

				len					+=	width;
				address				-=	WIDTH;
				y1--;
				if (len >= height)	len-=height,x++,x1++;
			}
		else
			for (int i=0;i<width;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 <  0		) return;

				p2SCREEN[address+i]	=	color;
				len					+=	height;
				x1++;
				if (len >= width ) len-=width,address-=WIDTH,y1--;
			}
	}
	else
	{	if (x1>x2)
		{	swap(x1,x2);
			swap(y1,y2);
		}

		address=x1+y1*WIDTH;

		if (width< height)
			for (int i=0;i<height;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 >= HEIGHT) return;

				if (x1 >= 0 && y1 < HEIGHT) p2SCREEN[address+x]	=	color;

				len					+=	width;
				address				+=	WIDTH;
				y1++;
				if (len >= height)	len-=height,x++,x1++;
			}
		else
			for (int i=0;i<width;i++)
			{	if (x1 >= WIDTH ) return;
				if (y1 >= HEIGHT) return;

				p2SCREEN[address+i]	=	color;
				len					+=	height;
				x1++;
				if (len >= width ) len-=width,address+=WIDTH,y1++;
			}
	}
}

void
cDRAW::XLine(WORD color,int x1,int x2,int y,int thick)
{	int	xs,i;

	x1=max(x1,0);
	x2=min(x2,WIDTH-1);

	if (y+thick > HEIGHT) thick = HEIGHT-y;
	if (y < 0 )
	{	if (y+thick < 0) return;
		thick=thick+y;
		y=0;
	}

	xs=x2-x1+1;
	if (xs <= 0 ) return;

	UINT	fc,*p4_scr,addr=0,addr2=0;		

	fc=(UINT) (color|(color<<16));			//	4바이트로 만듦..

	addr=x1+y*WIDTH;
	for (i=0;i<thick;i++,addr+=WIDTH)
	{	p4_scr=(UINT *)&p2SCREEN[addr];		//	4바이트 주소..
		__asm
		{
			mov  edi, dword ptr [ p4_scr]
			mov  ax,  color
			ror  eax, 16
			mov  ax,  color
			mov  ecx, xs

			shr  ecx, 1
			rep stosd                       // 4바이트 단위로 전송한다.
						
			mov  ecx, xs
			test ecx, 1
			jz   end_xline
			stosw                       // 한점이 남을때 다시 찍는다.
			end_xline:
		}
	}
}

void
cDRAW::YLine(WORD color,int y1,int y2,int x,int thick)
{	int	ys,i,j;

	y1=max(y1,0);
	y2=min(y2,HEIGHT-1);

	if (x+thick > WIDTH ) thick = WIDTH-x;
	if (x < 0 )
	{	if (x+thick < 0) return;
		thick=thick+x;
		x=0;
	}

	ys=y2-y1+1;
	if (ys <= 0 ) return;

	int addr=x+y1*WIDTH;

	for (i=0;i<ys;i++,addr+=WIDTH)
		for (j=0;j<thick;j++)
			p2SCREEN[addr+j]=color;
}

void
cDRAW::XLineAM(WORD color,int alpha,int x1,int x2,int y,int thick)
{	FillAM(color,x1,y,x2,y+thick-1,alpha);
}

void
cDRAW::YLineAM(WORD color,int alpha,int y1,int y2,int x,int thick)
{	FillAM(color,x,y1,x+thick-1,y2,alpha);
}

void
cDRAW::XLineHM(WORD color,int x1,int x2,int y,int thick)
{	FillHM(color,x1,y,x2,y+thick-1);
}

void
cDRAW::YLineHM(WORD color,int y1,int y2,int x,int thick)
{	FillHM(color,x,y1,x+thick-1,y2);
}

void
cDRAW::Box(WORD color,int x1,int y1,int x2,int y2,int thick)
{	if (x1 > x2) swap(x1,x2);
	if (y1 > y2) swap(y1,y2);
	XLine(color,x1,x2,y1		,thick);
	XLine(color,x1,x2,y2-thick+1,thick);
	YLine(color,y1,y2,x1		,thick);
	YLine(color,y1,y2,x2-thick+1,thick);
}

void
cDRAW::BoxHM(WORD color,int x1,int y1,int x2,int y2,int thick)
{	if (x1 > x2) swap(x1,x2);
	if (y1 > y2) swap(y1,y2);
	XLineHM(color,x1,x2,y1		  ,thick);
	XLineHM(color,x1,x2,y2-thick+1,thick);
	YLineHM(color,y1,y2,x1		  ,thick);
	YLineHM(color,y1,y2,x2-thick+1,thick);
}

void
cDRAW::BoxAM(WORD color,int alpha,int x1,int y1,int x2,int y2,int thick)
{	if (x1 > x2) swap(x1,x2);
	if (y1 > y2) swap(y1,y2);
	XLineAM(color,alpha,x1,x2,y1		,thick);
	XLineAM(color,alpha,x1,x2,y2-thick+1,thick);
	YLineAM(color,alpha,y1,y2,x1		,thick);
	YLineAM(color,alpha,y1,y2,x2-thick+1,thick);
}


/****************************************************************************
void	Fillxx(WORD color)

void	Fill(WORD color)					normal	
void	FillHM(WORD color)					half blending
void	FillHC(WORD color)					half converting
void	FillAM(WORD color,int alpha)		alpha blending
void	FillAC(WORD color,int alpha)		alpha converting

	화면 전체를 한가지 컬러로 채운다.
*****************************************************************************/

void
cDRAW::PutPixel(WORD color,int x,int y)
{	if (x<0||y<0||x>=WIDTH||y>=HEIGHT)  return;
	p2SCREEN[x+y*WIDTH]=color;
}

void
cDRAW::PutPixel(WORD color,int x,int y,int size)
{	Fill(color,x,y,x+size-1,y+size-1);
}

void
cDRAW::Fill(WORD color)
{	UINT *p4	=p4SCREEN;
	int	 dsize	=HalfScreen;

	__asm
	{
		mov  edi, dword ptr [p4]
		mov  ax,  color
		ror  eax, 16
		mov  ax,  color
		mov  ecx, dsize
		rep	 stosd                       // 4바이트 단위로 전송한다.
	}
}

void
cDRAW::FillHM(WORD color)
{	UINT	fc4,hc4;

	fc4=(UINT) (color|(color<<16));
	hc4=(fc4&d4HM_RGB555)>>1;
	for (int i=0;i<HalfScreen;i++)	p4SCREEN[i]=((p4SCREEN[i]&d4HM_RGB555)>>1)+hc4;
}

void
cDRAW::FillHC(WORD color)
{	UINT	hc4,fc4;

	fc4=(UINT) (color|(color<<16));			//	4바이트로 만듦..
	hc4=(fc4&d4HM_RGB555)>>1;
	for (int i=0;i<HalfScreen;i++)	p4SCREEN[i]=hc4;
}

void
cDRAW::FillAM(WORD color,int alpha)
{	int		dest_alpha=32-alpha;
	UINT	ac4,sc4,fc4,first,second;

	fc4=(UINT) (color|(color<<16));			//	4바이트로 만듦..

	first	=	fc4&d4AM_RGB555_1;
	first	>>=	5;
	first	*=	alpha;
	first	&=	d4AM_RGB555_1;

	second	=	fc4&d4AM_RGB555_2;
	second	*=	alpha;
	second	>>=	5;
	second	&=	d4AM_RGB555_2;

	ac4		=	first | second;

	for (int i=0;i<HalfScreen;i++)
	{	sc4			=	p4SCREEN[i];

		first		=	sc4&d4AM_RGB555_1;
		first		>>=	5;
		first		*=	dest_alpha;
		first		&=	d4AM_RGB555_1;

		second		=	sc4&d4AM_RGB555_2;
		second		*=	dest_alpha;
		second		>>=	5;
		second		&=	d4AM_RGB555_2;

		p4SCREEN[i]	=	ac4 + (first | second);
	}
}

void
cDRAW::FillAC(WORD color,int alpha)
{	UINT	ac4,fc4,first,second;

	fc4=(UINT) (color|(color<<16));			//	4바이트로 만듦..

	first	=	fc4&d4AM_RGB555_1;
	first	>>=	5;
	first	*=	alpha;
	first	&=	d4AM_RGB555_1;

	second	=	fc4&d4AM_RGB555_2;
	second	*=	alpha;
	second	>>=	5;
	second	&=	d4AM_RGB555_2;

	ac4		=	first | second;

	for (int i=0;i<HalfScreen;i++) p4SCREEN[i]	=	ac4;
}

/****************************************************************************
void	Fillxx(WORD color,int x1,int y1,int x2,int y2)

void	Fill(WORD color,int x1,int y1,int x2,int y2)					normal
void	FillHM(WORD color,int x1,int y1,int x2,int y2)					half blending
void	FillHC(WORD color,int x1,int y1,int x2,int y2)					half converting
void	FillAM(WORD color,int x1,int y1,int x2,int y2,int alpha)		alpha blending
void	FillAC(WORD color,int x1,int y1,int x2,int y2,int alpha)		alpha converting
	화면 일부를 한가지 컬러로 채운다.
*****************************************************************************/

void
cDRAW::Fill(WORD color,int x1,int y1,int x2,int y2)
{	int	xs,ys,i,j;

	x1=max(x1,0);
	y1=max(y1,0);
	x2=min(x2,WIDTH-1);
	y2=min(y2,HEIGHT-1);

	xs=x2-x1+1;
	ys=y2-y1+1;

	if (xs <= 0  || ys <= 0) return;

	UINT	fc4,*p4_scr,addr=0;

	fc4=(UINT) (color|(color<<16));			//	4바이트로 만듦..
	p4_scr=(UINT *)&p2SCREEN[x1+y1*WIDTH];	//	4바이트 주소..

	if (xs%2)							//	4바이트로 나누어지는 경우..
	{	int mica=x2+y1*WIDTH;

		for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH)
		{	for (j=0;j<xs/2;j++) 	p4_scr[addr+j]=fc4;
			p2SCREEN[mica]=color;			//	안 나눠 지니까.. 쪼가리가 남는다..
		}
	}
	else
	{	for (i=0;i<ys;i++,addr+=halfWidth)
			for (j=0;j<xs/2;j++)	p4_scr[addr+j]=fc4;
	}
}

void
cDRAW::FillHM(WORD color,int x1,int y1,int x2,int y2)
{	int	xs,ys,i,j;

	x1=max(x1,0);
	y1=max(y1,0);
	x2=min(x2,WIDTH-1);
	y2=min(y2,HEIGHT-1);

	xs=x2-x1+1;
	ys=y2-y1+1;

	if (xs <= 0  || ys <= 0) return;

	UINT	fc4,*p4_scr,addr=0,hc4;
	WORD	hc2;
	fc4=(UINT) (color|(color<<16));			//	4바이트로 만듦..
	hc4=(fc4&d4HM_RGB555)>>1;
	hc2=(color&d2HM_RGB555)>>1;
	p4_scr=(UINT *)&p2SCREEN[x1+y1*WIDTH];	//	4바이트 주소..

	if (xs%2)							//	4바이트로 나누어지는 경우..
	{	int	mica=x2+y1*WIDTH;

		for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH)
		{	for (j=0;j<xs/2;j++) p4_scr[addr+j]=((p4_scr[addr+j]&d4HM_RGB555)>>1)+hc4;
			p2SCREEN[mica]=((p2SCREEN[mica]&d2HM_RGB555)>>1)+hc2;			//	안 나눠 지니까.. 쪼가리가 남는다..
		}
	}
	else	
	{	for (i=0;i<ys;i++,addr+=halfWidth)
			for (j=0;j<xs/2;j++)
				p4_scr[addr+j]=((p4_scr[addr+j]&d4HM_RGB555)>>1)+hc4;
	}

}

void
cDRAW::FillHC(WORD color,int x1,int y1,int x2,int y2)
{	int	xs,ys,i,j;

	x1=max(x1,0);
	y1=max(y1,0);
	x2=min(x2,WIDTH-1);
	y2=min(y2,HEIGHT-1);

	xs=x2-x1+1;
	ys=y2-y1+1;

	if (xs <= 0  || ys <= 0) return;

	UINT	fc4,*p4_scr,addr=0,hc4;
	WORD	hc2;
	
	fc4=(UINT) (color|(color<<16));			//	4바이트로 만듦..
	hc4=(fc4&d4HM_RGB555)>>1;
	hc2=(color&d2HM_RGB555)>>1;
	p4_scr=(UINT *)&p2SCREEN[x1+y1*WIDTH];	//	4바이트 주소..

	if (xs%2)							//	4바이트로 나누어지는 경우..
	{	int mica=x2+y1*WIDTH;
		for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH)
		{	for (j=0;j<xs/2;j++) p4_scr[addr+j]=hc4;
			p2SCREEN[mica]	=	hc2;				//	안 나눠 지니까.. 쪼가리가 남는다..
		}
	}
	else	
	{	for (i=0;i<ys;i++,addr+=halfWidth)
			for (j=0;j<xs/2;j++)	p4_scr[addr+j]=hc4;
	}
	
}

void
cDRAW::FillAM(WORD color,int x1,int y1,int x2,int y2,int alpha)
{	int	xs,ys,i,j;

	x1=max(x1,0);
	y1=max(y1,0);
	x2=min(x2,WIDTH-1);
	y2=min(y2,HEIGHT-1);

	xs=x2-x1+1;
	ys=y2-y1+1;

	if (xs <= 0  || ys <= 0) return;

	int		dest_alpha=32-alpha;
	UINT	*p4_scr,addr=0,ac4,sc4,fc4,first,second;
	WORD	ac2,sc2;

	fc4=(UINT) (color|(color<<16));			//	4바이트로 만듦..

	first	=	fc4&d4AM_RGB555_1;
	first	>>=	5;
	first	*=	alpha;
	first	&=	d4AM_RGB555_1;

	second	=	fc4&d4AM_RGB555_2;
	second	*=	alpha;
	second	>>=	5;
	second	&=	d4AM_RGB555_2;

	ac4		=	first | second;


	first	=	color&d4AM_RGB555_1;
	first	>>=	5;
	first	*=	alpha;
	first	&=	d4AM_RGB555_1;

	second	=	color&d4AM_RGB555_2;
	second	*=	alpha;
	second	>>=	5;
	second	&=	d4AM_RGB555_2;

	ac2		=	(WORD)(first | second);


	p4_scr=(UINT *)&p2SCREEN[x1+y1*WIDTH];	//	4바이트 주소..

	if (xs%2==0)							//	4바이트로 나누어지는 경우..
	{	for (i=0;i<ys;i++,addr+=halfWidth)
			for (j=0;j<xs/2;j++)
			{	sc4				=	p4_scr[addr+j];

				first			=	sc4&d4AM_RGB555_1;
				first			>>=	5;
				first			*=	dest_alpha;
				first			&=	d4AM_RGB555_1;

				second			=	sc4&d4AM_RGB555_2;
				second			*=	dest_alpha;
				second			>>=	5;
				second			&=	d4AM_RGB555_2;

				p4_scr[addr+j]	=	ac4 + (first | second);
			}
	}
	else									//	안나눠 지는 경우.. ==;
	{	int mica=x2+y1*WIDTH;
		for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH)
		{	for (j=0;j<xs/2;j++)
			{	sc4				=	p4_scr[addr+j];

				first			=	sc4&d4AM_RGB555_1;
				first			>>=	5;
				first			*=	dest_alpha;
				first			&=	d4AM_RGB555_1;

				second			=	sc4&d4AM_RGB555_2;
				second			*=	dest_alpha;
				second			>>=	5;
				second			&=	d4AM_RGB555_2;

				p4_scr[addr+j]	=	ac4 + (first | second);
			}
			sc2				=	p2SCREEN[mica];

			first			=	sc2&d4AM_RGB555_1;
			first			>>=	5;
			first			*=	dest_alpha;
			first			&=	d4AM_RGB555_1;

			second			=	sc2&d4AM_RGB555_2;
			second			*=	dest_alpha;
			second			>>=	5;
			second			&=	d4AM_RGB555_2;

			p2SCREEN[mica]	=	ac2 + (WORD)(first | second);
		}
	}
}

void
cDRAW::FillAC(WORD color,int x1,int y1,int x2,int y2,int alpha)
{	int	xs,ys,i,j;

	x1=max(x1,0);
	y1=max(y1,0);
	x2=min(x2,WIDTH-1);
	y2=min(y2,HEIGHT-1);

	xs=x2-x1+1;
	ys=y2-y1+1;

	if (xs <= 0  || ys <= 0) return;

	int		dest_alpha=32-alpha;
	UINT	*p4_scr,addr=0,ac4,fc4,first,second;
	WORD	ac2;

	fc4=(UINT) (color|(color<<16));			//	4바이트로 만듦..

	first	=	fc4&d4AM_RGB555_1;
	first	>>=	5;
	first	*=	alpha;
	first	&=	d4AM_RGB555_1;

	second	=	fc4&d4AM_RGB555_2;
	second	*=	alpha;
	second	>>=	5;
	second	&=	d4AM_RGB555_2;

	ac4		=	first | second;

	first	=	color&d4AM_RGB555_1;
	first	>>=	5;
	first	*=	alpha;
	first	&=	d4AM_RGB555_1;

	second	=	color&d4AM_RGB555_2;
	second	*=	alpha;
	second	>>=	5;
	second	&=	d4AM_RGB555_2;

	ac2		=	(WORD)(first | second);


	p4_scr=(UINT *)&p2SCREEN[x1+y1*WIDTH];	//	4바이트 주소..

	if (xs%2)							//	4바이트로 나누어지는 경우..
	{	int mica=x2+y1*WIDTH;

		for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH)
		{	for (j=0;j<xs/2;j++)p4_scr[addr+j]	=	ac4;
			p2SCREEN[mica]	=	ac2;
		}
	}
	else
	{	xs/=2;
		for (i=0;i<ys;i++,addr+=halfWidth)
			for (j=0;j<xs;j++)	p4_scr[addr+j]	=	ac4;
	}

}


void
cDRAW::Grade(WORD color1,WORD color2,int x1,int y1,int x2,int y2,int xs)		//	그라데이션
{	WORD	color;

	WORD	r1,g1,b1,r2,g2,b2,r,g,b;

	r1	=	(color1>>10)&31;
	g1	=	(color1>>5 )&31;
	b1	=	 color1     &31;

	r2	=	(color2>>10)&31;
	g2	=	(color2>>5 )&31;
	b2	=	 color2     &31;

	for (int i=x1;i<=x2;i++)
	{	int rate=	(i-x1)*100/xs;
		r		=	r1*(100-rate)/100+r2*rate/100;
		g		=	g1*(100-rate)/100+g2*rate/100;
		b		=	b1*(100-rate)/100+b2*rate/100;
		color	=	(r<<10) + (g<<5) + b;

		YLine(color,y1,y2,i);
	}
}

/****************************************************************************
void	GetImage(int x,int y,int xs,int ys,WORD *img)
WORD*	GetImage(int x,int y,int xs,int ys)
	임의의 지점의 이미지를 읽어 온다.
****************************************************************************/

void
cDRAW::GetImage(int x,int y,int xs,int ys,WORD *img)
{
#ifdef dDEBUG_MODE
	if (!img)
	{	cMSG::Error("Error!! in cDRAW::GetImage[code0001]","Valid Image Buffer");
		return;
	}
#endif

	int width,offset=0,addr=0,i;

	width	=	xs;

	if (x	< 0			) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0			) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs>= WIDTH	) xs=WIDTH	-x-1;
	if (y+ys>= HEIGHT	) ys=HEIGHT	-y-1;

	if (xs <= 0 || ys <= 0) return;

	addr	=	x+y*WIDTH;

	xs*=2;
	for (i=0;i<ys;i++,addr+=WIDTH,offset+=width) fastcpy(img+offset,p2SCREEN+addr,xs);
}

WORD *
cDRAW::GetImage(int x,int y,int xs,int ys)
{	WORD *img=new WORD [xs*ys];
#ifdef dDEBUG_MODE
	if (!img) 
	{	cMSG::Error("Error!! in cDRAW::GetImage[code0000]","Not Enough Memory!!");
		return NULL;
	}
#endif

	int width,offset=0,addr=0,i;

	width	=	xs;

	if (x	< 0		) offset+=-x		,xs+=x,x=0;
	if (y	< 0		) offset+=-y*width	,ys+=y,y=0;
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return img;

	addr	=	x+y*WIDTH;

	xs*=2;
	for (i=0;i<ys;i++,addr+=WIDTH,offset+=width) fastcpy(img+offset,p2SCREEN+addr,xs);

	return img;
}

/****************************************************************************
void	PutImagexx(int x,int y,int xs,int ys,WORD *img)

void	PutImage(int x,int y,int xs,int ys,WORD *img)					normal
void	PutImageHM(int x,int y,int xs,int ys,WORD *img)					half blending
void	PutImageHC(int x,int y,int xs,int ys,WORD *img)					half converting
void	PutImageAM(int x,int y,int xs,int ys,WORD *img,int alpha)		alpha blending
void	PutImageAC(int x,int y,int xs,int ys,WORD *img,int alpha)		alpha converting
	임의의 지점에 이미지를 찍어준다.
*****************************************************************************/

void
cDRAW::PutImage(int x,int y,int xs,int ys,WORD *img)
{
#ifdef dDEBUG_MODE
	if (!img)
	{	cMSG::Error("Error!! in cDRAW::PutImage[code0032]  Valid Image data");
		return;
	}
#endif

int width,offset=0,addr=0,i;

	width	=	xs;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return;

	addr	=	x+y*WIDTH;

	xs*=2;
	for (i=0;i<ys;i++,addr+=WIDTH,offset+=width) fastcpy(p2SCREEN+addr,img+offset,xs);

}

void
cDRAW::PutImageHalf(int x,int y,int xs,int ys,WORD *img)
{
#ifdef dDEBUG_MODE
	if (!img)
	{	cMSG::Error("Error!! in cDRAW::PutImage[code0032]  Valid Image data");
		return;
	}
#endif

int width,offset=0,addr=0,i,j;

	width	=	xs;

	if (x	< 0			) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0			) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs/2> WIDTH	) xs=WIDTH	-x;
	if (y+ys/2> HEIGHT	) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return;

	addr	=	x+y*WIDTH;

	for (i=0;i<ys;i+=2,addr+=WIDTH,offset+=width*2)
	{	for (j=0;j<xs;j+=2)
			*(p2SCREEN+addr+j/2)=*(img+offset+j);
	}
}

void
cDRAW::PutImageHM(int x,int y,int xs,int ys,WORD *img)
{
#ifdef dDEBUG_MODE
	if (!img)
	{
		cMSG::Error("Error!! in cDRAW::PutImageHM[code0033]  Valid Image data");
		return;
	}
#endif

int width,offset=0,addr=0,i,j;

	width	=	xs;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return;

	if (xs%2)
	{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH];
		int mica=x+xs-1+y*WIDTH;
		for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH,offset+=width)
		{
			UINT *p4_img=(UINT *)&img[offset];
			for (j=0;j<xs/2;j++) 
				p4_scr[addr+j]=(((p4_scr[addr+j]&d4HM_RGB555)>>1)+((p4_img[j]&d4HM_RGB555)>>1));
			p2SCREEN[mica]=((p2SCREEN[mica]&d2HM_RGB555)+(img[offset+xs]&d2HM_RGB555))>>1;
		}
	}
	else
	{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH];
		for (i=0;i<ys;i++,addr+=halfWidth,offset+=width)
		{	UINT *p4_img=(UINT *)&img[offset];
			for (j=0;j<xs/2;j++)
				p4_scr[addr+j]=(((p4_scr[addr+j]&d4HM_RGB555)>>1)+((p4_img[j]&d4HM_RGB555)>>1));
		}
	}
}


void
cDRAW::PutImageHC(int x,int y,int xs,int ys,WORD *img)
{
#ifdef dDEBUG_MODE
	if (!img)
	{	cMSG::Error("Error!! in cDRAW::PutImageHC[code0034]  Valid Image data");
		return;
	}
#endif

	int width=xs,offset=0,addr=0,i,j;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return;

	if (xs%2)
	{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH];
		int mica=x+xs-1+y*WIDTH;
		for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH,offset+=width)
		{	UINT *p4_img=(UINT *)&img[offset];
			for (j=0;j<xs/2;j++)	p4_scr[addr+j]=(p4_img[j]&d4HM_RGB555)>>1;
			p2SCREEN[mica]=(img[offset+xs]&d2HM_RGB555)>>1;
		}
	}
	else
	{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH];
		for (i=0;i<ys;i++,addr+=halfWidth,offset+=width)
		{	UINT *p4_img=(UINT *)&img[offset];
			for (j=0;j<xs/2;j++)	p4_scr[addr+j]=(p4_img[j]&d4HM_RGB555)>>1;
		}
	}
}

void
cDRAW::PutImageAM(int x,int y,int xs,int ys,WORD *img,int alpha)
{
#ifdef dDEBUG_MODE
	if (!img || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::PutImageAM[code0035]  Valid Image data");
		return;
	}
#endif

int width=xs,offset=0,addr=0,i,j;
int dest_alpha=32-alpha;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return;

	if (xs%2)
	{
		UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH],first,second;
		int mica=x+xs-1+y*WIDTH;
		for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH,offset+=width)
		{	UINT *p4_img=(UINT *)&img[offset];
			for (j=0;j<xs/2;j++)
			{	first	=	p4_scr[addr+j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_scr[addr+j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				UINT sc4=	first | second;

				first	=	p4_img[j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_img[j]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				UINT ic4=	first | second;

				p4_scr[addr+j]=sc4+ic4;
			}

			first	=	p2SCREEN[mica]&d4AM_RGB555_1;
			first	>>=	5;
			first	*=	dest_alpha;
			first	&=	d4AM_RGB555_1;

			second	=	p2SCREEN[mica]&d4AM_RGB555_2;
			second	*=	dest_alpha;
			second	>>=	5;
			second	&=	d4AM_RGB555_2;

			WORD sc2=	(WORD)(first | second);

			first	=	img[offset+xs]&d4AM_RGB555_1;
			first	>>=	5;
			first	*=	alpha;
			first	&=	d4AM_RGB555_1;

			second	=	img[offset+xs]&d4AM_RGB555_2;
			second	*=	alpha;
			second	>>=	5;
			second	&=	d4AM_RGB555_2;

			WORD ic2=	(WORD)(first | second);

			p2SCREEN[mica]=(WORD)sc2+ic2;
		}
	}
	else
	{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH],first,second;
		for (i=0;i<ys;i++,addr+=halfWidth,offset+=width)
		{	UINT *p4_img=(UINT *)&img[offset];
			for (j=0;j<xs/2;j++)
			{	UINT	sc4,ic4;

				first	=	p4_scr[addr+j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_scr[addr+j]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc4		=	first | second;

				first	=	p4_img[j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_img[j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic4		=	first | second;

				p4_scr[addr+j]=sc4+ic4;
			}
		}
	}
}

void
cDRAW::PutImageAC(int x,int y,int xs,int ys,WORD *img,int alpha)
{
#ifdef dDEBUG_MODE
	if (!img || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::PutImageAC[code0036]  Valid Image data");
		return;
	}
#endif

int width=xs,offset=0,addr=0,i,j;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return;

	if (xs%2)
	{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH],first,second;
		int mica=x+xs-1+y*WIDTH;
		for (i=0;i<ys;i++,addr+=halfWidth,mica+=WIDTH,offset+=width)
		{	UINT *p4_img=(UINT *)&img[offset];
			for (j=0;j<xs/2;j++)
			{	first	=	p4_img[j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_img[j]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;
				p4_scr[addr+j]=first | second;
			}
			first	=	img[offset+xs]&d4AM_RGB555_1;
			first	>>=	5;
			first	*=	alpha;
			first	&=	d4AM_RGB555_1;

			second	=	img[offset+xs]&d4AM_RGB555_2;
			second	*=	alpha;
			second	>>=	5;
			second	&=	d4AM_RGB555_2;

			p2SCREEN[mica]=(WORD)(first | second);
		}
	}
	else
	{	UINT *p4_scr=(UINT *)&p2SCREEN[x+y*WIDTH],first,second;
		for (i=0;i<ys;i++,addr+=halfWidth,offset+=width)
		{	UINT *p4_img=(UINT *)&img[offset];
			for (j=0;j<xs/2;j++)
			{	first	=	p4_img[j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_img[j]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				p4_scr[addr+j]=first | second;
			}
		}
	}
}

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
void	PutSpriteHM(int x,int y,WORD *sprite,int alpha)
		화면과 반투명처리해준다.
void	PutSpriteHC(int x,int y,WORD *sprite,int alpha)
		화면과 관계 없이 컬러를 반으로 다운시킨다.
void	PutSpriteAB(int x,int y,WORD *sprite,int alpha)
		알파블렌딩 출력
void	PutSpriteAM(int x,int y,WORD *sprite,int alpha)
		주어진 알파값으로 스프라이트와 배경을 섞어 준다.
void	PutSpriteAC(int x,int y,WORD *sprite,int alpha)
		주어진 알파값으로 스프라이트를 나타낸다.

		위에서 알파값은 32 가  Max이며 이때 이미지가 원본 상태로 나타난다.
		0일때는 아무것도 출력 되지 않는다.

*******************************************************************************************/

void
cDRAW::PutSpriteColor(int x,int y,WORD *sprite,WORD color)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,endX;
	int   count,bufferPoint=4;
	int	  size;
#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! in cDRAW::PutSprite16[code0040]");
		return;
	}
#endif

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(short)sprite[2];	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

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
	{
		count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{
			xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			if (xPoint	< 0		)
			{	if (size+xPoint	>= 0)
					memset(p2SCREEN+addr,color,size+xPoint);
			}
			else memset(p2SCREEN+addr+xPoint,color,size);
			
			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{
		count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{
			xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size>  WIDTH)	endX	= WIDTH-xPoint;		//	오른쪽 클리핑..
			if (endX		>	0	)	memset(p2SCREEN+addr+xPoint	,color	,endX);

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{
		count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{
			xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			memset(p2SCREEN+addr+xPoint	,	color	,	size);
			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutSprite16(int x,int y,WORD *sprite,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=4;
	int	  size;
#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! in cDRAW::PutSprite16[code0040]");
		return;
	}
#endif

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(short)sprite[2];	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

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
	{
		count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{
			xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			if (size > beginX )
				fastcpy(p2SCREEN+addr+xPoint+beginX,sprite+bufferPoint+beginX,(size-beginX)<<1);

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{
		count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{
			xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..
			if (endX > 0 )
				fastcpy(p2SCREEN+addr+xPoint,sprite+bufferPoint,endX<<1);

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{
		count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{
			xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			fastcpy(p2SCREEN+addr+xPoint,sprite+bufferPoint,(size<<1));
			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutSpriteHalf(int x,int y,WORD *sprite,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=4;
	int	  size;
#ifdef dDEBUG_MODE
	if (!sprite || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::PutSprite16[code0040]");
		return;
	}
#endif

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(short)sprite[2]/2;	//	위치 교정..
	y-=(short)sprite[3]/2;	//	위치 교정..

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys/2) y2=HEIGHT;	//	y축 하단 클리핑..

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

	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++]/2;
			size	=	sprite[bufferPoint++];
			if (y1%2)
			{	beginX	=	0;
				endX	=	(int) size;

				if (xPoint+ size/2	>  WIDTH) endX	= WIDTH-xPoint;	//	오른쪽 클리핑..
				if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..

				for (j=beginX;j<endX;j+=2)
					*(p2SCREEN+addr+xPoint+j/2)=*(sprite+bufferPoint+j);
			}
			bufferPoint	+=	size;
		}
		if (y1%2)  addr+=WIDTH;
	}
}

void
cDRAW::PutSpriteHM16(int x,int y,WORD *sprite,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=4;
	int   size;
#ifdef dDEBUG_MODE
	if (!sprite || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::PutSpriteHM16[code0041]");
		return;
	}
#endif

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(short)sprite[2];	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

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

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint+beginX];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+beginX];
			width		=size-beginX;

			for (j=0;j<width/2;j++)
				p4_scr[j]=(((p4_scr[j]&d4HM_RGB555)>>1)+((p4_spr[j]&d4HM_RGB555)>>1));
			if (width%2 && width > 0)
				p2SCREEN[addr+xPoint+size-1]
				=((p2SCREEN[addr+xPoint+size-1]&d2HM_RGB555)+(sprite[bufferPoint+size-1]&d2HM_RGB555))>>1;

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint];
			width		=endX;

			for (j=0;j<width/2;j++)
				p4_scr[j]=(((p4_scr[j]&d4HM_RGB555)>>1)+((p4_spr[j]&d4HM_RGB555)>>1));
			if (width%2 && endX > 0)
				p2SCREEN[addr+xPoint+endX-1]
				=((p2SCREEN[addr+xPoint+endX-1]&d2HM_RGB555)+(sprite[bufferPoint+endX-1]&d2HM_RGB555))>>1;

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint];

			for (j=0;j<size/2;j++)
				p4_scr[j]=(((p4_scr[j]&d4HM_RGB555)>>1)+((p4_spr[j]&d4HM_RGB555)>>1));
			if ( size%2 )
				p2SCREEN[addr+xPoint+size-1]
				=((p2SCREEN[addr+xPoint+size-1]&d2HM_RGB555)+(sprite[bufferPoint+size-1]&d2HM_RGB555))>>1;
			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutSpriteHC16(int x,int y,WORD *sprite,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX,width,i_size;
	int   count,bufferPoint=4;
#ifdef dDEBUG_MODE
	if (!sprite || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::PutSpriteHC16[code0042]");
		return;
	}
#endif

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(short)sprite[2];	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

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
			i_size=sprite[bufferPoint++];
			bufferPoint+=i_size;
		}
	}

	if (x<0)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			i_size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..

			UINT *p4_spr=(UINT *)&sprite[bufferPoint+beginX];
			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint+beginX];
			width		=i_size-beginX;

			if (width > 0)
			__asm
			{	mov		ecx,width
				shr		ecx,1
				mov		esi, dword ptr [p4_spr]
				mov		edi, dword ptr [p4_scr]
__makedata21:
				lodsd
				and		eax, d4HM_RGB555
				shr		eax, 1
				stosd

				loop	__makedata21

				mov		edx, width
				test	edx, 1
				jne     __makeworddata21
				jmp		__endmakedata21
__makeworddata21:
				lodsw
				and		ax, d2HM_RGB555
				shr		ax, 1
				stosw
__endmakedata21:
			}

			bufferPoint	+=	i_size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			i_size	=	sprite[bufferPoint++];

			endX	=	(int) i_size;

			if (xPoint+ i_size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint];
			width		=endX;

			if (width > 0)
			__asm
			{	mov		ecx,width
				shr		ecx,1
				mov		esi, dword ptr [p4_spr]
				mov		edi, dword ptr [p4_scr]
__makedata22:
				lodsd
				and		eax, d4HM_RGB555
				shr		eax, 1
				stosd

				loop	__makedata22

				mov		edx, width
				test	edx, 1
				jne     __makeworddata22
				jmp		__endmakedata22
__makeworddata22:
				lodsw
				and		ax, d2HM_RGB555
				shr		ax, 1
				stosw
__endmakedata22:
			}
			
			bufferPoint	+=	i_size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			i_size	=	sprite[bufferPoint++];

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint];

			__asm
			{	mov		ecx,i_size
				shr		ecx,1
				mov		esi, dword ptr [p4_spr]
				mov		edi, dword ptr [p4_scr]
__makedata23:
				lodsd
				and		eax, d4HM_RGB555
				shr		eax, 1
				stosd

				loop	__makedata23

				mov		edx, i_size
				test	edx, 1
				jne     __makeworddata23
				jmp		__endmakedata23
__makeworddata23:
				lodsw
				and		ax, d2HM_RGB555
				shr		ax, 1
				stosw
__endmakedata23:
			}

			bufferPoint	+=	i_size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutSpriteAB16(int x,int y,WORD *sprite,int alpha)
{	if (alpha<=0) return;
	if (alpha>=32)
	{	PutSprite16(x,y,sprite,alpha);
		return;
	}
	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size,dest_alpha;
#ifdef dDEBUG_MODE
	if (!sprite || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::PutSpriteAB16[code0043]");
		return;
	}
#endif
	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(short)sprite[2];	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

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

	UINT	first,second;
	WORD	sc2;

	if (x<0)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..

			for (j=beginX;j<size;j++)
			{	dest_alpha	=	32-_ALPHAVALUE[sprite[bufferPoint+j]];

				first	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint+j]	=(WORD)(sc2+_ALPHATABLE[sprite[bufferPoint+j]]);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			for (j=0;j<endX;j++)
			{	dest_alpha	=	32-_ALPHAVALUE[sprite[bufferPoint+j]];

				first	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint+j]	=(WORD)(sc2+_ALPHATABLE[sprite[bufferPoint+j]]);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			for (j=0;j<size;j++)
			{	dest_alpha	=	32-_ALPHAVALUE[sprite[bufferPoint+j]];

				first	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint+j]	=(WORD)(sc2+_ALPHATABLE[sprite[bufferPoint+j]]);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutSpriteAM16(int x,int y,WORD *sprite,int alpha)
{	if (alpha<=0 ) return;
	if (alpha>=32)
	{	PutSprite16(x,y,sprite,alpha);
		return;
	}

	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=4;
	int	  size;
	int	  dest_alpha=32-alpha;
#ifdef dDEBUG_MODE
	if (!sprite || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::PutSpriteAM16[code0044]");
		return;
	}
#endif
	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(short)sprite[2];	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

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

	UINT	first,second,sc4,ic4;
	WORD	sc2,ic2;

	if (x<0)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];	
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint+beginX];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+beginX];
			width		=size-beginX;
			for (j=0;j<width/2;j++)
			{	first	=	p4_scr[j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_scr[j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc4		=	first | second;

				first	=	p4_spr[j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_spr[j]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic4		=	first | second;

				p4_scr[j]=	sc4+ic4;
			}
			if (width%2 && width > 0)
			{	first	=	p2SCREEN[addr+xPoint+size-1]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint+size-1]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				first	=	sprite[bufferPoint+size-1]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	sprite[bufferPoint+size-1]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint+size-1]
						=	(WORD)(sc2+ic2);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint];
			width		=endX;
			for (j=0;j<width/2;j++)
			{	first	=	p4_scr[j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_scr[j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc4		=	first | second;

				first	=	p4_spr[j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_spr[j]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic4		=	first | second;

				p4_scr[j]=	sc4+ic4;
			}
			if (width%2 && width > 0)
			{	first	=	p2SCREEN[addr+xPoint+endX-1]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint+endX-1]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				first	=	sprite[bufferPoint+endX-1]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	sprite[bufferPoint+endX-1]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint+endX-1]
						=	(WORD)(sc2+ic2);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint];

			for (j=0;j<size/2;j++)
			{	first	=	p4_scr[j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_scr[j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc4		=	first | second;

				first	=	p4_spr[j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_spr[j]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic4		=	first | second;

				p4_scr[j]=	sc4+ic4;
			}
			if (size%2)
			{	first	=	p2SCREEN[addr+xPoint+size-1]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint+size-1]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				first	=	sprite[bufferPoint+size-1]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	sprite[bufferPoint+size-1]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint+size-1]
						=	(WORD)(sc2+ic2);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutSpriteAC16(int x,int y,WORD *sprite,int alpha)
{	if (alpha>=32)
	{	PutSprite16(x,y,sprite,alpha);
		return;
	}

	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=4;
	int   size;
	int	  dest_alpha=32-alpha;
#ifdef dDEBUG_MODE
	if (!sprite || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::PutSpriteAC16[code0045]");
		return;
	}
#endif
	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(short)sprite[2];	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

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

	UINT	first,second;

	if (x<0)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint+beginX];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+beginX];
			width		=size-beginX;
			for (j=0;j<width/2;j++)
			{	first	=	p4_spr[j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_spr[j]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				p4_scr[j]=	first | second;
			}
			if (width%2 && width > 0)
			{	first	=	sprite[bufferPoint+size-1]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	sprite[bufferPoint+size-1]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				p2SCREEN[addr+xPoint+size-1]	=	(WORD) (first | second);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint];
			width		=endX;
			for (j=0;j<width/2;j++)
			{	first	=	p4_spr[j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_spr[j]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				p4_scr[j]=	first | second;
			}
			if (width%2 && width > 0)
			{	first	=	sprite[bufferPoint+endX-1]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	sprite[bufferPoint+endX-1]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				p2SCREEN[addr+xPoint+endX-1]
						=	(WORD) (first | second);
			}
			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint];

			for (j=0;j<size/2;j++)
			{	first	=	p4_spr[j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_spr[j]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				p4_scr[j]=	first | second;
			}
			if (size%2)
			{	first	=	sprite[bufferPoint+size-1]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	sprite[bufferPoint+size-1]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				p2SCREEN[addr+xPoint+size-1]
						=	(WORD) (first | second);
			}
			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

/*******************************************************************************************

	압축 스프라이트 출력 루틴들.. 플립

	알고리즘.. 걍 거꾸로 찍어 준다..
	단지.. 더블워드 역전송을 위해..
	UINT color;
	*(video+address)=(color<<16)|(color>>16);
	과 같은 방법을 사용했다.

*******************************************************************************************/

void
cDRAW::fPutSprite16(int x,int y,WORD *sprite,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=4,dx;
	int   size;
#ifdef dDEBUG_MODE
	if (!sprite || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::fPutSprite16[code0046]");
		return;
	}
#endif

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(xs-(short)sprite[2]);	//	위치 교정..
	y-=(short)sprite[3];		//	위치 교정..

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

		for(i=0;i<count;i++)
		{	xPoint	=	x+(xs-sprite[bufferPoint++]);
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint	< size	) beginX= size-xPoint;			//	왼쪽 클리핑..

			width	=size-beginX;
			dx		=	width%2;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-width];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+dx];

			for (j=0;j<width/2;j++)	p4_scr[j]=(p4_spr[width/2-1-j]<<16)|(p4_spr[width/2-1-j]>>16);
			if  (dx && width > 0  ) p2SCREEN[addr+xPoint-1]=sprite[bufferPoint];

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+(xs-sprite[bufferPoint++]);
			size	=	sprite[bufferPoint++];

			endX	=	size;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-size];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+size%2];

			if (xPoint	>  WIDTH)
			{	endX	= WIDTH-(xPoint-size);		//	오른쪽 클리핑..
				xPoint	= WIDTH;
			}

			width	=	endX;
			dx		=	width%2;

			for (j=0;j<width/2;j++)	p4_scr[j]=(p4_spr[size/2-1-j]<<16)|(p4_spr[size/2-1-j]>>16);
			if  (dx>0) p2SCREEN[addr+xPoint-1]=sprite[bufferPoint+size-endX];

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+(xs-sprite[bufferPoint++]);
			size	=	sprite[bufferPoint++];

			dx		=	size%2;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-size+1];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+dx];

			for (j=size/2-1,x1=0;j>=0;j--,x1++)	p4_scr[x1]=(p4_spr[j]<<16)|(p4_spr[j]>>16);
			if  (dx) p2SCREEN[addr+xPoint]=sprite[bufferPoint];

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::fPutSpriteHM16(int x,int y,WORD *sprite,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=4,dx=0;
	int  size;
#ifdef dDEBUG_MODE
	if (!sprite || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::fPutSpriteHM16[code0047]");
		return;
	}
#endif

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(xs-(short)sprite[2]);	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

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

		for(i=0;i<count;i++)
		{	xPoint	=	x+(xs-sprite[bufferPoint++]);
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint	< size	) beginX= size-xPoint;			//	왼쪽 클리핑..

			width	=size-beginX;
			dx		=	width%2;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-width];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+dx];

			for (j=0;j<width/2;j++)
				p4_scr[j]=(((p4_scr[j]&d4HM_RGB555)>>1)+((((p4_spr[width/2-1-j]<<16)|(p4_spr[width/2-1-j]>>16))&d4HM_RGB555)>>1));
			if  (dx > 0 )
				p2SCREEN[addr+xPoint-1]=
				((p2SCREEN[addr+xPoint-1]&d2HM_RGB555)+(sprite[bufferPoint]&d2HM_RGB555))>>1;

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+(xs-sprite[bufferPoint++]);
			size	=	sprite[bufferPoint++];

			endX	=	size;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-size];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+size%2];

			if (xPoint	>  WIDTH)
			{	endX	= WIDTH-(xPoint-size);		//	오른쪽 클리핑..	
				xPoint	= WIDTH;
			}

			width	=	endX;
			dx		=	width%2;

			for (j=0;j<width/2;j++)
				p4_scr[j]=
				(	
					((p4_scr[j]&d4HM_RGB555)>>1)	+
					((((p4_spr[size/2-1-j]<<16)|(p4_spr[size/2-1-j]>>16))&d4HM_RGB555)>>1)
				);

			if  (dx>0)	p2SCREEN[addr+xPoint-1]
				=((p2SCREEN[addr+xPoint-1]&d2HM_RGB555)+(sprite[bufferPoint+size-endX]&d2HM_RGB555))>>1;

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+(xs-sprite[bufferPoint++]);
			size	=	sprite[bufferPoint++];

			dx		=	size%2;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-size+1];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+dx];

			for (j=size/2-1,x1=0;j>=0;j--,x1++)
				p4_scr[x1]=(((p4_scr[x1]&d4HM_RGB555)>>1)+((((p4_spr[j]<<16)|(p4_spr[j]>>16))&d4HM_RGB555)>>1));
			if  (dx)
			{	p2SCREEN[addr+xPoint]
				=((p2SCREEN[addr+xPoint]&d2HM_RGB555)+(sprite[bufferPoint]&d2HM_RGB555))>>1;
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}

}

void
cDRAW::fPutSpriteHC16(int x,int y,WORD *sprite,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX,width,i_size;
	int   count,bufferPoint=4;
#ifdef dDEBUG_MODE
	if (!sprite || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::fPutSpriteHC16[code0048]");
		return;
	}
#endif

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(xs-(short)sprite[2]);	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

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
		{
			bufferPoint++;
			i_size=sprite[bufferPoint++];
			bufferPoint+=i_size;
		}
	}

	if (x<0)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			i_size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..

			UINT *p4_spr=(UINT *)&sprite[bufferPoint+beginX];
			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint+beginX];
			width		=i_size-beginX;

			if (width > 0)
			__asm
			{	mov		ecx,width
				shr		ecx,1
				mov		esi, dword ptr [p4_spr]
				mov		edi, dword ptr [p4_scr]
__makedata21:
				lodsd
				and		eax, d4HM_RGB555
				shr		eax, 1
				stosd

				loop	__makedata21

				mov		edx, width
				test	edx, 1
				jne     __makeworddata21
				jmp		__endmakedata21
__makeworddata21:
				lodsw
				and		ax, d2HM_RGB555
				shr		ax, 1
				stosw
__endmakedata21:
			}

			bufferPoint	+=	i_size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			i_size	=	sprite[bufferPoint++];

			endX	=	(int) i_size;

			if (xPoint+ i_size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint];
			width		=endX;

			if (width > 0)
			__asm
			{	mov		ecx,width
				shr		ecx,1
				mov		esi, dword ptr [p4_spr]
				mov		edi, dword ptr [p4_scr]
__makedata22:
				lodsd
				and		eax, d4HM_RGB555
				shr		eax, 1
				stosd

				loop	__makedata22

				mov		edx, width
				test	edx, 1
				jne     __makeworddata22
				jmp		__endmakedata22
__makeworddata22:
				lodsw
				and		ax, d2HM_RGB555
				shr		ax, 1
				stosw
__endmakedata22:
			}
			
			bufferPoint	+=	i_size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			i_size	=	sprite[bufferPoint++];

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint];

			__asm
			{	mov		ecx,i_size
				shr		ecx,1
				mov		esi, dword ptr [p4_spr]
				mov		edi, dword ptr [p4_scr]
__makedata23:
				lodsd
				and		eax, d4HM_RGB555
				shr		eax, 1
				stosd

				loop	__makedata23

				mov		edx, i_size
				test	edx, 1
				jne     __makeworddata23
				jmp		__endmakedata23
__makeworddata23:
				lodsw
				and		ax, d2HM_RGB555
				shr		ax, 1
				stosw
__endmakedata23:
			}

			bufferPoint	+=	i_size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::fPutSpriteAB16(int x,int y,WORD *sprite,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;
	int	  dest_alpha;
#ifdef dDEBUG_MODE
	if (!sprite || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::fPutSpriteAB16[code0049]");
		return;
	}
#endif

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(xs-(short)sprite[2]);	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

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

	UINT	first,second;
	WORD	sc2;

	if (x<0)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+xs-sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	size;
			if (xPoint - size	< 0		) endX	= xPoint;		//	왼쪽 클리핑..

			for (j=0;j<endX;j++)
			{	dest_alpha	=	32-_ALPHAVALUE[sprite[bufferPoint+j]];

				first	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint-j]	=(WORD)(sc2+_ALPHATABLE[sprite[bufferPoint+j]]);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+xs-sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;
			if (xPoint	> WIDTH	) beginX= xPoint-WIDTH;	//	오른쪽 클리핑..

			for (j=beginX;j<size;j++)
			{	dest_alpha	=	32-_ALPHAVALUE[sprite[bufferPoint+j]];

				first	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint-j]	=(WORD)(sc2+_ALPHATABLE[sprite[bufferPoint+j]]);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+xs-sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			for (j=0;j<size;j++)
			{	dest_alpha	=	32-_ALPHAVALUE[sprite[bufferPoint+j]];

				first	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint-j]	=(WORD)(sc2+_ALPHATABLE[sprite[bufferPoint+j]]);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::fPutSpriteAM16(int x,int y,WORD *sprite,int alpha)
{	if (alpha==0 ) return;
	if (alpha==32) fPutSprite16(x,y,sprite,alpha);
#ifdef dDEBUG_MODE
	if (!sprite || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::fPutSpriteAM16[code0050]");
		return;
	}
#endif

	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=4;
	int   size;
	int	  dest_alpha=32-alpha,dx;

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(xs-(short)sprite[2]);	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

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

	UINT	first,second,sc4,ic4;
	WORD	sc2,ic2;

	if (x<0)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+(xs-sprite[bufferPoint++]);
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint	< size	) beginX= size-xPoint;			//	왼쪽 클리핑..

			width	=size-beginX;
			dx		=	width%2;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-width];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+dx];

			for (j=0;j<width/2;j++)
			{	first	=	p4_scr[j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_scr[j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc4		=	first | second;

				first	=	((p4_spr[width/2-1-j]<<16)|(p4_spr[width/2-1-j]>>16))&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	((p4_spr[width/2-1-j]<<16)|(p4_spr[width/2-1-j]>>16))&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic4		=	first | second;

				p4_scr[j]=	sc4+ic4;

			}
			if  (dx > 0 )
			{	first	=	p2SCREEN[addr+xPoint-1]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint-1]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				first	=	sprite[bufferPoint]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	sprite[bufferPoint]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint-1]
						=	(WORD)(sc2+ic2);

			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+(xs-sprite[bufferPoint++]);
			size	=	sprite[bufferPoint++];

			endX	=	size;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-size];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+size%2];

			if (xPoint	>  WIDTH)
			{	endX	= WIDTH-(xPoint-size);		//	오른쪽 클리핑..
				xPoint	= WIDTH;
			}

			width	=	endX;
			dx		=	width%2;

			for (j=0;j<width/2;j++)
			{	first	=	p4_scr[j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_scr[j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc4		=	first | second;

				first	=	((p4_spr[size/2-1-j]<<16)|(p4_spr[size/2-1-j]>>16))&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	((p4_spr[size/2-1-j]<<16)|(p4_spr[size/2-1-j]>>16))&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic4		=	first | second;

				p4_scr[j]=	sc4+ic4;

			}
			if  (dx>0)
			{	first	=	p2SCREEN[addr+xPoint-1]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint-1]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				first	=	sprite[bufferPoint+size-endX]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	sprite[bufferPoint+size-endX]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint-1]
						=	(WORD)(sc2+ic2);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+(xs-sprite[bufferPoint++]);
			size	=	sprite[bufferPoint++];

			dx		=	size%2;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-size+1];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+dx];

			for (j=size/2-1,x1=0;j>=0;j--,x1++)
			{	first	=	p4_scr[x1]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p4_scr[x1]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc4		=	first | second;

				first	=	((p4_spr[j]<<16)|(p4_spr[j]>>16))&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	((p4_spr[j]<<16)|(p4_spr[j]>>16))&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic4		=	first | second;

				p4_scr[x1]=	sc4+ic4;
			}
			if  (dx)
			{	first	=	p2SCREEN[addr+xPoint]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				first	=	sprite[bufferPoint]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	sprite[bufferPoint]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint]	=	(WORD)(sc2+ic2);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}

}

void
cDRAW::fPutSpriteAC16(int x,int y,WORD *sprite,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=4;
	int   size;
	int	  dest_alpha=32-alpha,dx;
#ifdef dDEBUG_MODE
	if (!sprite || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::fPutSpriteAC16[code0051]");
		return;
	}
#endif

	xs=(short)sprite[0];
	ys=(short)sprite[1];
	x-=(xs-(short)sprite[2]);	//	위치 교정..
	y-=(short)sprite[3];	//	위치 교정..

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

	UINT	first,second;

	if (x<0)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+(xs-sprite[bufferPoint++]);
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint	< size	) beginX= size-xPoint;			//	왼쪽 클리핑..

			width	=size-beginX;
			dx		=	width%2;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-width];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+dx];

			for (j=0;j<width/2;j++)
			{	first	=	((p4_spr[width/2-1-j]<<16)|(p4_spr[width/2-1-j]>>16))&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	((p4_spr[width/2-1-j]<<16)|(p4_spr[width/2-1-j]>>16))&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				p4_scr[j]=	first | second;
			}
			if  (dx > 0 )
			{	first	=	sprite[bufferPoint]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	sprite[bufferPoint]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				p2SCREEN[addr+xPoint-1]
						=	(WORD) (first | second);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+(xs-sprite[bufferPoint++]);
			size	=	sprite[bufferPoint++];

			endX	=	size;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-size];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+size%2];

			if (xPoint	>  WIDTH)
			{	endX	= WIDTH-(xPoint-size);		//	오른쪽 클리핑..
				xPoint	= WIDTH;
			}

			width	=	endX;
			dx		=	width%2;

			for (j=0;j<width/2;j++)
			{	first	=	((p4_spr[size/2-1-j]<<16)|(p4_spr[size/2-1-j]>>16))&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	((p4_spr[size/2-1-j]<<16)|(p4_spr[size/2-1-j]>>16))&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				p4_scr[j]=	first | second;
			}
			if  (dx>0)
			{	first	=	sprite[bufferPoint+size-endX]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	sprite[bufferPoint+size-endX]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				p2SCREEN[addr+xPoint-1]
						=	(WORD) (first | second);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+(xs-sprite[bufferPoint++]);
			size	=	sprite[bufferPoint++];

			dx		=	size%2;

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint-size+1];
			UINT *p4_spr=(UINT *)&sprite[bufferPoint+dx];

			for (j=size/2-1,x1=0;j>=0;j--,x1++)
			{	first	=	((p4_spr[j]<<16)|(p4_spr[j]>>16))&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	((p4_spr[j]<<16)|(p4_spr[j]>>16))&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				p4_scr[x1]=	first | second;
			}
			if  (dx)
			{	first	=	sprite[bufferPoint]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	sprite[bufferPoint]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				p2SCREEN[addr+xPoint]
						=	(WORD) (first | second);
			}
			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}

}

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

void	PutSpriteXX(int x,int y,BYTE *sprite,WORD *plt,...)
int	x,y
	:	이미지의 좌표
BYTE *sprite
	:	이미지 데이터
WORD *plt
	:	팔레트
...
	:	알파 블렌딩시 알파값



void	PutSprite(int x,int y,BYTE *sprite,WORD *plt,int alpha)
		그냥 출력해준다.
void	PutSpriteHM(int x,int y,BYTE *sprite,WORD *plt,int alpha)
		화면과 반투명처리해준다.
void	PutSpriteAB(int x,int y,BYTE *sprite,WORD *plt,int alpha)
		알파블렌딩 출력
void	PutSpriteAM(int x,int y,BYTE *sprite,WORD *plt,int alpha)
		주어진 알파값으로 스프라이트와 배경을 섞어 준다.

		위에서 알파값은 32 가  Max이며 이때 이미지가 원본 상태로 나타난다.
		0일때는 아무것도 출력 되지 않는다.

*******************************************************************************************/

void
cDRAW::redSprite8(int x,int y,BYTE *sprite,WORD *pplt)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=6;
	int   size;
	WORD  plt[256];
	fastcpy(plt,pplt,256*2);

	BYTE *plt8=(BYTE *)plt;
	for (i=1;i<512;i+=2) plt8[i]|=0x7c;

#ifdef dDEBUG_MODE
	if (!sprite || !plt )
	{	cMSG::Error("Error!! in cDRAW::PutSprite8[code0052]");
		return;
	}
#endif

	xs=sprite[0];
	ys=sprite[1];
	x-=*(short *)(sprite+2);	//	위치 교정..
	y-=*(short *)(sprite+4);	//	위치 교정..

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

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			for (j=beginX;j<size;j++)
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..
			for (j=0;j<endX;j++)
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			for (j=0;j<size;j++)
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];
			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutSprite8(int x,int y,BYTE *sprite,WORD *plt,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=6;
	int   size;
#ifdef dDEBUG_MODE
	if (!sprite || !plt )
	{	cMSG::Error("Error!! in cDRAW::PutSprite8[code0052]");
		return;
	}
#endif

	xs=sprite[0];
	ys=sprite[1];
	x-=*(short *)(sprite+2);	//	위치 교정..
	y-=*(short *)(sprite+4);	//	위치 교정..

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..
	if (x >= WIDTH) return;

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

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			for (j=beginX;j<size;j++)
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..
			for (j=0;j<endX;j++)
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			for (j=0;j<size;j++)
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];
			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutSprite8Anti(int x,int y,BYTE *sprite,WORD *plt,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=6;
	int   size;
#ifdef dDEBUG_MODE
	if (!sprite || !plt )
	{	cMSG::Error("Error!! in cDRAW::PutSprite8[code0052]");
		return;
	}
#endif

	xs=sprite[0];
	ys=sprite[1];
	x-=*(short *)(sprite+2);	//	위치 교정..
	y-=*(short *)(sprite+4);	//	위치 교정..

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	if (x >= WIDTH) return;
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{	count=sprite[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=sprite[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (x<1)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..

			for (j=beginX;j<size;j++)
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];

			if (xPoint			> 1		) 
				*(p2SCREEN+addr+xPoint-1)	
					=	((p2SCREEN[addr+xPoint-1]&d2HM_RGB555)+(plt[sprite[bufferPoint	]]&d2HM_RGB555))>>1;

			if (xPoint	+size	<	WIDTH) 
				*(p2SCREEN+addr+xPoint+size)	
					=	((p2SCREEN[addr+xPoint+size]&d2HM_RGB555)+(plt[sprite[bufferPoint+size-1]]&d2HM_RGB555))>>1;

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs+1 > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..
			for (j=0;j<endX;j++)
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];

			if (xPoint			> 1		) 
				*(p2SCREEN+addr+xPoint-1)	
					=	((p2SCREEN[addr+xPoint-1]&d2HM_RGB555)+(plt[sprite[bufferPoint	]]&d2HM_RGB555))>>1;

			if (xPoint	+size	<	WIDTH) 
				*(p2SCREEN+addr+xPoint+size)	
					=	((p2SCREEN[addr+xPoint+size]&d2HM_RGB555)+(plt[sprite[bufferPoint+size-1]]&d2HM_RGB555))>>1;


			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			*(p2SCREEN+addr+xPoint-1)	
					=	((p2SCREEN[addr+xPoint-1]&d2HM_RGB555)+(plt[sprite[bufferPoint	]]&d2HM_RGB555))>>1;

			*(p2SCREEN+addr+xPoint+size)	
					=	((p2SCREEN[addr+xPoint+size]&d2HM_RGB555)+(plt[sprite[bufferPoint+size-1]]&d2HM_RGB555))>>1;


			for (j=0;j<size;j++)
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];
			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutBuilding(int x,int y,BYTE *sprite,WORD *plt)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;
#ifdef dDEBUG_MODE
	if (!sprite || !plt )
	{	cMSG::Error("Error!! in cDRAW::PutSprite8[code0052]");
		return;
	}
#endif

	xs=*(WORD  *)(sprite+0);
	ys=*(WORD  *)(sprite+2);
	x-=*(short *)(sprite+4);	//	위치 교정..
	y-=*(short *)(sprite+6);	//	위치 교정..

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..
	if (x >= WIDTH) return;

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

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			for (j=beginX;j<size;j++)
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..
			for (j=0;j<endX;j++)
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			for (j=0;j<size;j++)
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];
			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutBuildingLayer(int x,int y,BYTE *sprite,WORD color)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;
#ifdef dDEBUG_MODE
	if (!sprite)
	{	cMSG::Error("Error!! in cDRAW::PutSprite8[code0052]");
		return;
	}
#endif

	xs=*(WORD  *)(sprite+0);
	ys=*(WORD  *)(sprite+2);
	x-=*(short *)(sprite+4);	//	위치 교정..
	y-=*(short *)(sprite+6);	//	위치 교정..

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..
	if (x >= WIDTH) return;

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

	if (x<1)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			if (xPoint		>= 1	)	*(p2SCREEN+addr+xPoint-1)=color;
			if (xPoint+ size>= 0	)	*(p2SCREEN+addr+xPoint+size)=color;

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs+1 > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			if (xPoint		<=	WIDTH	)	*(p2SCREEN+addr+xPoint-1)=color;
			if (xPoint+ size<	WIDTH	)	*(p2SCREEN+addr+xPoint+size)=color;

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count	=	sprite[bufferPoint++];
		beginX	=	0xffff;
		endX	=	0;

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			*(p2SCREEN+addr+xPoint-1)=color;
			*(p2SCREEN+addr+xPoint+size)=color;

			bufferPoint	+=	size;

			if (i==0) beginX=xPoint;
		}
		addr+=WIDTH;
	}
}


void
cDRAW::redPutBuilding(int x,int y,BYTE *sprite,WORD *pplt)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;
	WORD  plt[256];
	fastcpy(plt,pplt,256*2);

	BYTE *plt8=(BYTE *)plt;
	for (i=1;i<512;i+=2) plt8[i]|=0x7c;

#ifdef dDEBUG_MODE
	if (!sprite || !plt )
	{	cMSG::Error("Error!! in cDRAW::PutSprite8[code0052]");
		return;
	}
#endif

	xs=*(WORD  *)(sprite+0);
	ys=*(WORD  *)(sprite+2);
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

//	cMSG::Put("asd","%d,%d,%d,%d",xs,ys,*(short *)(sprite+4),*(short *)(sprite+6));
//	cMSG::Put("asd","%d,%d,%d,%d",sprite[8],sprite[9],sprite[10],sprite[11]);
	if (x<0)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			for (j=beginX;j<size;j++)
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..
			for (j=0;j<endX;j++)
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			for (j=0;j<size;j++)
				*(p2SCREEN+addr+xPoint+j)=plt[*(sprite+bufferPoint+j)];
			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutSpriteHM8(int x,int y,BYTE *sprite,WORD *plt,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=6;
	int   size;
#ifdef dDEBUG_MODE
	if (!sprite  || !plt )
	{	cMSG::Error("Error!! in cDRAW::PutSpriteHM8[code0053]");
		return;
	}
#endif

	xs=sprite[0];
	ys=sprite[1];
	x-=*(short *)(sprite+2);	//	위치 교정..
	y-=*(short *)(sprite+4);	//	위치 교정..

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

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..

			for (j=beginX;j<size;j++)
				p2SCREEN[addr+xPoint+j]
					=((p2SCREEN[addr+xPoint+j]&d2HM_RGB555)+(plt[sprite[bufferPoint+j]]&d2HM_RGB555))>>1;

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..
			for (j=0;j<endX;j++)
				p2SCREEN[addr+xPoint+j]
					=((p2SCREEN[addr+xPoint+j]&d2HM_RGB555)+(plt[sprite[bufferPoint+j]]&d2HM_RGB555))>>1;

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			for (j=0;j<size;j++)
				p2SCREEN[addr+xPoint+j]
					=((p2SCREEN[addr+xPoint+j]&d2HM_RGB555)+(plt[sprite[bufferPoint+j]]&d2HM_RGB555))>>1;
			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}

}

void
cDRAW::PutSpriteAB8(int x,int y,BYTE *sprite,WORD *plt,int alpha)
{	if (alpha<=0 ) return;
	if (alpha>=32)
	{	PutSprite8(x,y,sprite,plt,alpha);
		return;
	}
	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size,dest_alpha;
#ifdef dDEBUG_MODE
	if (!sprite  || !plt || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::PutSpriteAB8[code0054]");
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

	UINT	first,second;
	WORD	sc2;

	if (x<0)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..

			for (j=beginX;j<size;j++)
			{	dest_alpha	=	32-_ALPHAVALUE[plt[sprite[bufferPoint+j]]];

				first	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint+j]	=(WORD)(sc2+_ALPHATABLE[plt[sprite[bufferPoint+j]]]);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			for (j=0;j<endX;j++)
			{	dest_alpha	=	32-_ALPHAVALUE[plt[sprite[bufferPoint+j]]];

				first	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint+j]	=(WORD)(sc2+_ALPHATABLE[plt[sprite[bufferPoint+j]]]);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			for (j=0;j<size;j++)
			{	dest_alpha	=	32-_ALPHAVALUE[plt[sprite[bufferPoint+j]]];

				first	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint+j]	=(WORD)(sc2+_ALPHATABLE[plt[sprite[bufferPoint+j]]]);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutSpriteAM8(int x,int y,BYTE *sprite,WORD *plt,int alpha)
{	if (alpha<=0 ) return;
	if (alpha>=32)
	{	PutSprite8(x,y,sprite,plt,alpha);
		return;
	}

#ifdef dDEBUG_MODE
	if (!sprite  || !plt || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::PutSpriteAM8[code0055]");
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

	UINT	first,second;
	WORD	sc2,ic2;

	if (x<0)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..

			for (j=beginX;j<size;j++)
			{	first	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				first	=	plt[sprite[bufferPoint+j]]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	plt[sprite[bufferPoint+j]]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint+j]	=(WORD)(sc2+ic2);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			for (j=0;j<endX;j++)
			{	first	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				first	=	plt[sprite[bufferPoint+j]]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	plt[sprite[bufferPoint+j]]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint+j]	=	(WORD)(sc2+ic2);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			for (j=0;j<size;j++)
			{	first	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint+j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				first	=	plt[sprite[bufferPoint+j]]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	plt[sprite[bufferPoint+j]]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint+j]	=	(WORD)(sc2+ic2);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

/*******************************************************************************************

	압축 스프라이트 출력 루틴들.. 플립

	알고리즘.. 걍 거꾸로 찍어 준다..
	속도가 떨어지거나 하지는 않는다..
	어차피 워드 전송인것을..

*******************************************************************************************/

void
cDRAW::fPutSprite8(int x,int y,BYTE *sprite,WORD *plt,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=6;
	int   size;
#ifdef dDEBUG_MODE
	if (!sprite  || !plt || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::fPutSprite8[code0056]");
		return;
	}
#endif

	xs=sprite[0];
	ys=sprite[1];
	x-=(xs-*(short *)(sprite+2));	//	위치 교정..
	y-=*(short *)(sprite+4);	//	위치 교정..

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

		for(i=0;i<count;i++)
		{	xPoint	=	x+xs-sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	size;
			if (xPoint- size	< 0		) endX	= xPoint;		//	왼쪽 클리핑..
			for (j=0;j<endX;j++) *(p2SCREEN+addr+xPoint-j)=plt[*(sprite+bufferPoint+j)];

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+xs-sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;
			if (xPoint	> WIDTH	) beginX= xPoint-WIDTH;	//	오른쪽 클리핑..

			for (j=beginX;j<size;j++) *(p2SCREEN+addr+xPoint-j)=plt[*(sprite+bufferPoint+j)];

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+xs-sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			for (j=0;j<size;j++) *(p2SCREEN+addr+xPoint-j)=plt[*(sprite+bufferPoint+j)];
			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::fPutSpriteHM8(int x,int y,BYTE *sprite,WORD *plt,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=6;
	int   size;
#ifdef dDEBUG_MODE
	if (!sprite  || !plt || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::fPutSpriteHM8[code0057]");
		return;
	}
#endif

	xs=sprite[0];
	ys=sprite[1];
	x-=(xs-*(short *)(sprite+2));	//	위치 교정..
	y-=*(short *)(sprite+4);	//	위치 교정..

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

		for(i=0;i<count;i++)
		{	xPoint	=	x+xs-sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	size;
			if (xPoint- size	< 0		) endX	= xPoint;		//	왼쪽 클리핑..

			for (j=0;j<endX;j++)
				p2SCREEN[addr+xPoint-j]
					=((p2SCREEN[addr+xPoint-j]&d2HM_RGB555)+(plt[sprite[bufferPoint+j]]&d2HM_RGB555))>>1;

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+xs-sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;
			if (xPoint	> WIDTH	) beginX= xPoint-WIDTH;	//	오른쪽 클리핑..

			for (j=beginX;j<size;j++)
				p2SCREEN[addr+xPoint-j]
					=((p2SCREEN[addr+xPoint-j]&d2HM_RGB555)+(plt[sprite[bufferPoint+j]]&d2HM_RGB555))>>1;

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+xs-sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			for (j=0;j<size;j++) 
				*(p2SCREEN+addr+xPoint-j)
				=((p2SCREEN[addr+xPoint-j]&d2HM_RGB555)+(plt[*(sprite+bufferPoint+j)]&d2HM_RGB555))>>1;
			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::fPutSpriteAB8(int x,int y,BYTE *sprite,WORD *plt,int alpha)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=8;
	int   size;
	int	  dest_alpha;
#ifdef dDEBUG_MODE
	if (!sprite  || !plt || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::fPutSpriteAB8[code0058]");
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
	{
		count=sprite[bufferPoint++];
		for(i=0;i<count;i++)
		{
			bufferPoint++;
			size=sprite[bufferPoint++];
			bufferPoint+=size;
		}
	}

	UINT	first,second;
	WORD	sc2;

	if (x<0)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+xs-sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	size;
			if (xPoint - size	< 0		) endX	= xPoint;		//	왼쪽 클리핑..

			for (j=0;j<endX;j++)
			{	dest_alpha	=	32-_ALPHAVALUE[plt[sprite[bufferPoint+j]]];

				first	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint-j]	=(WORD)(sc2+_ALPHATABLE[plt[sprite[bufferPoint+j]]]);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+xs-sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;
			if (xPoint	> WIDTH	) beginX= xPoint-WIDTH;	//	오른쪽 클리핑..

			for (j=beginX;j<size;j++)
			{	dest_alpha	=	32-_ALPHAVALUE[plt[sprite[bufferPoint+j]]];

				first	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint-j]	=(WORD)(sc2+_ALPHATABLE[plt[sprite[bufferPoint+j]]]);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+xs-sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			for (j=0;j<size;j++)
			{	dest_alpha	=	32-_ALPHAVALUE[plt[sprite[bufferPoint+j]]];

				first	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint-j]	=(WORD)(sc2+_ALPHATABLE[plt[sprite[bufferPoint+j]]]);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::fPutSpriteAM8(int x,int y,BYTE *sprite,WORD *plt,int alpha)
{	if (alpha==0 ) return;
	if (alpha==32) fPutSprite8(x,y,sprite,plt,alpha);
#ifdef dDEBUG_MODE
	if (!sprite  || !plt || alpha < 0 || alpha > 32 )
	{	cMSG::Error("Error!! in cDRAW::fPutSpriteAM8[code0059]");
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

	UINT	first,second;
	WORD	sc2,ic2;

	if (x<0)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+xs-sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			endX	=	size;
			if (xPoint - size	< 0		) endX	= xPoint;		//	왼쪽 클리핑..

			for (j=0;j<endX;j++)
			{	first	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				first	=	plt[sprite[bufferPoint+j]]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	plt[sprite[bufferPoint+j]]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint-j]	=(WORD)(sc2+ic2);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+xs-sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			beginX	=	0;
			if (xPoint	> WIDTH	) beginX= xPoint-WIDTH;	//	오른쪽 클리핑..

			for (j=beginX;j<size;j++)
			{	first	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				first	=	plt[sprite[bufferPoint+j]]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	plt[sprite[bufferPoint+j]]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint-j]	=	(WORD)(sc2+ic2);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=sprite[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+xs-sprite[bufferPoint++];
			size	=	sprite[bufferPoint++];

			for (j=0;j<size;j++)
			{	first	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	dest_alpha;
				first	&=	d4AM_RGB555_1;

				second	=	p2SCREEN[addr+xPoint-j]&d4AM_RGB555_2;
				second	*=	dest_alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				sc2		=	(WORD) (first | second);

				first	=	plt[sprite[bufferPoint+j]]&d4AM_RGB555_1;
				first	>>=	5;
				first	*=	alpha;
				first	&=	d4AM_RGB555_1;

				second	=	plt[sprite[bufferPoint+j]]&d4AM_RGB555_2;
				second	*=	alpha;
				second	>>=	5;
				second	&=	d4AM_RGB555_2;

				ic2		=	(WORD) (first | second);

				p2SCREEN[addr+xPoint-j]	=	(WORD)(sc2+ic2);
			}

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

/*****************************************************************************************

	그림자 출력.. 이미지를 4바이트 단위로 잘라 버린다.. 넘치면?? 무시해버림.. ==;

*****************************************************************************************/

void
cDRAW::PutShadow(int x,int y,BYTE *shadow)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX,width;
	int   count,bufferPoint=6;
	int   size;
#ifdef dDEBUG_MODE
	if (!shadow)
	{	cMSG::Error("Error!! in cDRAW::PutShadow[code0060]");
		return;
	}
#endif

	xs=shadow[0];
	ys=shadow[1];
	x-=*(short *)(shadow+2);	//	위치 교정..
	y-=*(short *)(shadow+4);	//	위치 교정..

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{	count=shadow[bufferPoint++];
		for(i=0;i<count;i++) bufferPoint+=2;				//	옵셋
	}

	if (x<0)
	for(;y1<y2;y1++)
	{	count=shadow[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+shadow[bufferPoint++];
			size	=	shadow[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint+beginX];
			width		=size-beginX;

			for (j=0;j<width/2;j++				)	p4_scr[j]=(p4_scr[j]&d4HM_RGB555)>>1;
			if ( width%2 && xPoint+size-1 >= 0	)	p2SCREEN[addr+xPoint+size-1]=(p2SCREEN[addr+xPoint+size-1]&d2HM_RGB555)>>1;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=shadow[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+shadow[bufferPoint++];
			size	=	shadow[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint];
			width		=endX;

			for (j=0;j<width/2;j++)	p4_scr[j]=(p4_scr[j]&d4HM_RGB555)>>1;
			if ( width%2 && xPoint+size-1 < WIDTH )
				p2SCREEN[addr+xPoint+size-1]=(p2SCREEN[addr+xPoint+size-1]&d2HM_RGB555)>>1;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=shadow[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+shadow[bufferPoint++];
			size	=	shadow[bufferPoint++];

			UINT *p4_scr=(UINT *)&p2SCREEN[addr+xPoint];

			for (j=0;j<size/2;j++)	p4_scr[j]=(p4_scr[j]&d4HM_RGB555)>>1;
			if ( size%2 )	p2SCREEN[addr+xPoint+size-1]=(p2SCREEN[addr+xPoint+size-1]&d2HM_RGB555)>>1;
		}
		addr+=WIDTH;
	}
}
/*****************************************************************************************

	폰트 출력 (폰트라고는 하지만 공백 압축 방식이다... 이미지만 없음..)

*****************************************************************************************/

void
cDRAW::PutFontNormal(int x,int y,WORD color,BYTE *font,BOOL bMC)
{	int   x1, y1, x2, y2 ,xs ,ys,yp=0;
	int   xPoint=0,i,beginX,endX;
	int   count,bufferPoint=2;
	int   size;
	WORD  put_color=color;	

	if (!font)
	{
#ifdef dDEBUG_MODE
		cMSG::Error("Error!! in cDRAW::PutFont[code0061]");
#endif
		return;
	}

	xs=font[0];
	ys=font[1];

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(yp=0;yp<-y;yp++)		//	y축 상단 클리핑..
	{	count=font[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=font[bufferPoint++];
		}
	}

	if (x<0 || x+xs > WIDTH)
	for(;y1<y2;y1++,yp++)
	{	count=font[bufferPoint++];
		if (bMC) put_color=_gCOL[color][yp];

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];

			beginX	=	0;
			endX	=	size;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			if (xPoint+ size	> WIDTH	) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..
			if (endX			> beginX)	memset(p2SCREEN+addr+beginX+xPoint,put_color,endX - beginX);
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++,yp++)
	{	count=font[bufferPoint++];
		if (bMC) put_color=_gCOL[color][yp];

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];
			memset(p2SCREEN+addr+xPoint,put_color,size);
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutFontHB(int x,int y,WORD color,BYTE *font,BOOL bMC)
{	int   x1, y1, x2, y2 ,xs ,ys,yp=0;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=2;
	int   size;

	if (!font)
	{
#ifdef dDEBUG_MODE
		cMSG::Error("Error!! in cDRAW::PutFontHM2[code0084]");
#endif
		return;
	}

	xs=font[0];
	ys=font[1];

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(yp=0;yp<-y;yp++)		//	y축 상단 클리핑..
	{	count=font[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=font[bufferPoint++];
		}
	}

	WORD  put_color=(color&d2HM_RGB555)>>1;

	if (x<0 || x+xs > WIDTH)
	for(;y1<y2;y1++,yp++)
	{	count=font[bufferPoint++];
		if (bMC) put_color=(_gCOL[color][yp]&d2HM_RGB555)>>1;

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];

			beginX	=	0;
			endX	=	size;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			if (xPoint+ size	> WIDTH	) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			for (j=beginX;j<endX;j++)
				p2SCREEN[+addr+xPoint+j]=((p2SCREEN[+addr+xPoint+j]&d2HM_RGB555)>>1)+put_color;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++,yp++)
	{	count=font[bufferPoint++];
		if (bMC) put_color=(_gCOL[color][yp]&d2HM_RGB555)>>1;

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];
			for (j=0;j<size;j++)
				p2SCREEN[+addr+xPoint+j]=((p2SCREEN[+addr+xPoint+j]&d2HM_RGB555)>>1)+put_color;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutFont(int x,int y,WORD color,BYTE *font,int alpha,BOOL bMC)
{	if (alpha<=0 ) return;
	if (alpha>=32)
	{	PutFontNormal(x,y,color,font,bMC);
		return;
	}
	if (alpha==16)
	{	PutFontHB(x,y,color,font,bMC);
		return;
	}

	int   x1, y1, x2, y2 ,xs ,ys,yp=0;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=2;
	int   size,dest_alpha=32-alpha;
	WORD  put_color;
	UINT  first,second;

	if (!font)
	{
#ifdef dDEBUG_MODE
		cMSG::Error("Error!! in cDRAW::PutFontAM2[code0083]");
#endif
		return;
	}

	xs=font[0];
	ys=font[1];

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(yp=0;yp<-y;yp++)		//	y축 상단 클리핑..
	{	count=font[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=font[bufferPoint++];
		}
	}

	first	=	color&d4AM_RGB555_1;
	first	>>=	5;
	first	*=	alpha;
	first	&=	d4AM_RGB555_1;

	second	=	color&d4AM_RGB555_2;
	second	*=	alpha;
	second	>>=	5;
	second	&=	d4AM_RGB555_2;

	put_color=	(WORD)(first | second);

	if (x<0 || x+xs > WIDTH)
	for(;y1<y2;y1++,yp++)
	{	count=font[bufferPoint++];

		if (bMC)
		{	first	=	_gCOL[color][yp]&d4AM_RGB555_1;
			first	>>=	5;
			first	*=	alpha;
			first	&=	d4AM_RGB555_1;

			second	=	_gCOL[color][yp]&d4AM_RGB555_2;
			second	*=	alpha;
			second	>>=	5;
			second	&=	d4AM_RGB555_2;

			put_color=	(WORD)(first | second);
		}

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];

			beginX	=	0;
			endX	=	(int) size;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			if (xPoint+ size	> WIDTH	) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			for (j=beginX;j<endX;j++)
			{	WORD	sc2		=	p2SCREEN[addr+xPoint+j];

				first			=	sc2&d4AM_RGB555_1;
				first			>>=	5;
				first			*=	dest_alpha;
				first			&=	d4AM_RGB555_1;

				second			=	sc2&d4AM_RGB555_2;
				second			*=	dest_alpha;
				second			>>=	5;
				second			&=	d4AM_RGB555_2;

				p2SCREEN[addr+xPoint+j]=	put_color + (WORD)(first | second);
			}
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++,yp++)
	{	count=font[bufferPoint++];
		if (bMC)
		{	first	=	_gCOL[color][yp]&d4AM_RGB555_1;
			first	>>=	5;
			first	*=	alpha;
			first	&=	d4AM_RGB555_1;

			second	=	_gCOL[color][yp]&d4AM_RGB555_2;
			second	*=	alpha;
			second	>>=	5;
			second	&=	d4AM_RGB555_2;

			put_color=	(WORD)(first | second);

		}

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];
			for (j=0;j<size;j++)
			{	WORD	sc2		=	p2SCREEN[addr+xPoint+j];

				first			=	sc2&d4AM_RGB555_1;
				first			>>=	5;
				first			*=	dest_alpha;
				first			&=	d4AM_RGB555_1;

				second			=	sc2&d4AM_RGB555_2;
				second			*=	dest_alpha;
				second			>>=	5;
				second			&=	d4AM_RGB555_2;

				p2SCREEN[addr+xPoint+j]=	put_color + (WORD)(first | second);
			}
		}
		addr+=WIDTH;
	}
}


void
cDRAW::PutFontAnti(int x,int y,WORD color,BYTE *font,int effect)
{	int   x1, y1, x2, y2 ,xs ,ys,yp=0;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=2;
	int   size,r,g,b;

	if (!font)
	{
#ifdef dDEBUG_MODE
		cMSG::Error("Error!! in cDRAW::PutFontAM2[code0083]");
#endif
		return;
	}

	xs=font[0];
	ys=font[1];

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(yp=0;yp<-y;yp++)		//	y축 상단 클리핑..
	{	count=font[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=font[bufferPoint++];
		}
	}

	if (x<0 || x+xs > WIDTH)
	for(;y1<y2;y1++,yp++)
	{	count=font[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];

			beginX	=	0;
			endX	=	(int) size;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			if (xPoint+ size	> WIDTH	) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..

			for (j=beginX;j<endX;j++)
			{	
				int spr	=	color;
				int bg	=	p2SCREEN[addr+xPoint+j];
				r=redEFFECT		[effect][spr>>11	][bg>>11	];
				g=greenEFFECT	[effect][(spr>>5)&63][(bg>>5)&63];
				b=blueEFFECT	[effect][spr&31		][bg&31		];
				p2SCREEN[addr+xPoint+j]	=	WORD(r+g+b);
			}
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++,yp++)
	{	count=font[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];
			for (j=0;j<size;j++)
			{	int spr	=	color;

				int bg	=	p2SCREEN[addr+xPoint+j];
				r=redEFFECT		[effect][spr>>11	][bg>>11	];
				g=greenEFFECT	[effect][(spr>>5)&63][(bg>>5)&63];
				b=blueEFFECT	[effect][spr&31		][bg&31		];
				p2SCREEN[addr+xPoint+j]	=	WORD(r+g+b);
			}
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutFontBuffer(int x,int y,int bXSize,int bYSize,BYTE color,BYTE *buffer,BYTE *font)
{	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,beginX,endX;
	int   count,bufferPoint=2;
	int   size;

#ifdef dDEBUG_MODE
	if (!font || !buffer)
	{	cMSG::Error("Error!! in cDRAW::PutFontBuffer[code0086]");
		return;
	}
#endif

	xs=font[0];
	ys=font[1];

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (bYSize < y+ys) y2=bYSize;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*bXSize;

	for(i=0;i<-y;i++)		//	y축 상단 클리핑..
	{	count=font[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=font[bufferPoint++];
		}
	}

	if (x<0)
	for(;y1<y2;y1++)
	{	count=font[bufferPoint++];
		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];

			beginX	=	0;

			if (xPoint	< 0  ) beginX= -xPoint;
			if (size > beginX)	memset(buffer+addr+beginX+xPoint,color,size - beginX);
		}
		addr+=bXSize;
	}
	else
	if (x+xs > bXSize)
	for(;y1<y2;y1++)
	{	count=font[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  bXSize) endX	= bXSize-xPoint;		//	오른쪽 클리핑..
			if (endX > 0)	memset(buffer+addr+xPoint,color,endX);
		}
		addr+=bXSize;
	}
	else
	for(;y1<y2;y1++)
	{	count=font[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+font[bufferPoint++];
			size	=	font[bufferPoint++];
			memset(buffer+addr+xPoint,color,size);
		}
		addr+=bXSize;
	}
}

void
cDRAW::PutFlame(int x,int y,int xs,int ys,BYTE *img,WORD *plt)
{
#ifdef dDEBUG_MODE
	if (!img  || !plt)
	{	cMSG::Error("Error!! in cDRAW::PutFlame[code0037]  Valid Image data");
		return;
	}
#endif

int width,offset=0,addr=0,i,j;

	width	=	xs;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return;

	addr	=	x+y*WIDTH;


	for (i=0;i<ys;i++,addr+=WIDTH,offset+=width)
		for (j=0;j<xs;j++)
		{	if (plt[img[offset+j]]==0) continue;
			p2SCREEN[addr+j]=plt[img[offset+j]];
		}

}
void 
cDRAW::PutFlame(int x,int y,int width,int height,BYTE *img,WORD *plt,WORD *compare)
{
#ifdef dDEBUG_MODE
	if (!img  || !plt || !compare)
	{	cMSG::Error("Error!! in cDRAW::PutFlame[code0038]  Valid Image data");
		return;
	}
#endif
	int   x1, y1, x2, y2 ,xs ,ys;
	int   xPoint=0,i,j,beginX,endX;
	int   count,bufferPoint=4;
	int	  size;

	xs=(short)compare[0];
	ys=(short)compare[1];
	x-=(short)compare[2];	//	위치 교정..
	y-=(short)compare[3];	//	위치 교정..

	x1= 0;if ( x > 0 ) x1 = x;
	y1= 0;if ( y > 0 ) y1 = y;

	x2 = xs+x;
	y2 = ys+y;
	if (HEIGHT < y+ys) y2=HEIGHT;	//	y축 하단 클리핑..

	if (x2 <= x1 || y2 <= y1 ) return;	//	이미지가 화면에 없다.
	int addr=y1*WIDTH;

	for(j=0;j<-y;j++)		//	y축 상단 클리핑..
	{	count=compare[bufferPoint++];
		for(i=0;i<count;i++)
		{	bufferPoint++;
			size=compare[bufferPoint++];
			bufferPoint+=size;
		}
	}

	if (x<0)
	for(;y1<y2;y1++)
	{	count=compare[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+compare[bufferPoint++];
			size	=	compare[bufferPoint++];

			beginX	=	0;

			if (xPoint			< 0		) beginX= -xPoint;			//	왼쪽 클리핑..
			for (j=beginX;j<size;j++	) p2SCREEN[addr+xPoint+j]=plt[img[(y1-y)*width+xPoint-x+j]];

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	if (x+xs > WIDTH)
	for(;y1<y2;y1++)
	{	count=compare[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+compare[bufferPoint++];
			size	=	compare[bufferPoint++];

			endX	=	(int) size;

			if (xPoint+ size	>  WIDTH) endX	= WIDTH-xPoint;		//	오른쪽 클리핑..
			for (j=0;j<endX;j++			) p2SCREEN[addr+xPoint+j]=plt[img[(y1-y)*width+xPoint-x+j]];

			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
	else
	for(;y1<y2;y1++)
	{	count=compare[bufferPoint++];

		for(i=0;i<count;i++)
		{	xPoint	=	x+compare[bufferPoint++];
			size	=	compare[bufferPoint++];

			for (j=0;j<size;j++)	p2SCREEN[addr+xPoint+j]=plt[img[(y1-y)*width+xPoint-x+j]];
			bufferPoint	+=	size;
		}
		addr+=WIDTH;
	}
}

void
cDRAW::PutFlameAB(int x,int y,int xs,int ys,BYTE *img,WORD *plt)
{
#ifdef dDEBUG_MODE
	if (!img  || !plt)
	{	cMSG::Error("Error!! in cDRAW::PutFlameAB[code0039]  Valid Image data");
		return;
	}
#endif

int width,offset=0,addr=0,i,j;

	width	=	xs;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return;

	addr	=	x+y*WIDTH;

	for (i=0;i<ys;i++,addr+=WIDTH,offset+=width)
		for (j=0;j<xs;j++)
		{	UINT first,second;

			if (plt[img[offset+j]]==0) continue;
			int dest_alpha	=	32-_ALPHAVALUE[plt[img[offset+j]]];

			first	=	p2SCREEN[addr+j]&d4AM_RGB555_1;
			first	>>=	5;
			first	*=	dest_alpha;
			first	&=	d4AM_RGB555_1;

			second	=	p2SCREEN[addr+j]&d4AM_RGB555_2;
			second	*=	dest_alpha;
			second	>>=	5;
			second	&=	d4AM_RGB555_2;

			WORD  sc2		=	(WORD) (first | second);

			p2SCREEN[addr+j] =(WORD)(sc2+_ALPHATABLE[plt[img[offset+j]]]);
		}
}

void
cDRAW::PutFlameFX(int x,int y,int xs,int ys,BYTE *img,WORD *plt,int effect)
{
#ifdef dDEBUG_MODE
	if (!img  || !plt)
	{	cMSG::Error("Error!! in cDRAW::PutFlame[code0037]  Valid Image data");
		return;
	}
#endif

int width,offset=0,addr=0,i,j,r,g,b;

	width	=	xs;

	if (x	< 0		) {offset+=-x		;xs+=x;x=0;}
	if (y	< 0		) {offset+=-y*width	;ys+=y;y=0;}
	if (x+xs> WIDTH	) xs=WIDTH	-x;
	if (y+ys> HEIGHT) ys=HEIGHT	-y;

	if (xs <= 0 || ys <= 0) return;

	addr	=	x+y*WIDTH;


	for (i=0;i<ys;i++,addr+=WIDTH,offset+=width)
		for (j=0;j<xs;j++)
		{	if (plt[img[offset+j]]==0) continue;
			int spr=plt[img[offset+j]];
			int bg=p2SCREEN[addr+j];

			r=redEFFECT[effect][spr>>10][bg>>10];
			g=greenEFFECT[effect][(spr>>5)&30][(bg>>5)&30];
			b=blueEFFECT[effect][spr&30][bg&30];

			p2SCREEN[addr+j]	=	WORD(r+g+b);
		}
}

void 
cDRAW::PutSprite(int x,int y,BYTE *sprite,WORD *plt,int put,int alpha,BOOL flip)
{	pPutSprite8[put][flip](x,y,sprite,plt,alpha);
}

void 
cDRAW::PutSprite(int x,int y,WORD *sprite,int put,int alpha,BOOL flip)
{	pPutSprite16[put][flip](x,y,sprite,alpha);
}