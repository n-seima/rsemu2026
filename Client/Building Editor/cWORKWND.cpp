#include "cMAIN.H"
#include "cWORKWND.H"
#include "cDRAW.H"
#include "cANM.H"
#include "cTEXT.H"

int	l_iGradeY	=	32;
int	l_iGradeX	=	64;
int	l_iGradeY2	=	16;
int	l_iGradeX2	=	32;

cWORKWND	_WORKWND;

#define	IDM_SET_OUTLINE		0
#define	IDM_SET_SHADOW		1
#define	IDM_SET_BACK		2
#define	IDM_SET_SPRITEINFO	3

#define	IDM_ALPHA_SOFTDODGE	4
#define	IDM_ALPHA_DIFFEREMT	5
#define	IDM_ALPHA_DODGEBURN	6
#define	IDM_ALPHA_SOFT		7
#define	IDM_ALPHA_DODGE		8
#define	IDM_ALPHA_LIGHT		9
#define	IDM_ALPHA_EXLIGHT	10
#define	IDM_ALPHA_NORMAL	12
#define	IDM_ALPHA_HALF		11

#define	IDM_BLOCK_1			13
#define	IDM_BLOCK_2			14
#define	IDM_BLOCK_3			15
#define	IDM_BLOCK_4			16
#define	IDM_BLOCK_5			17

#define	dBAR_COUNT			18


cRECT	imgRect;

cBARMENU	barMenu[dBAR_COUNT];
BOOL		barStatus[dBAR_COUNT],moveSprite=FALSE,moveCross=FALSE;
int			curCursor	=	0;

cWORKWND::cWORKWND()
		 :cDIBWND()
{	Zoom	=	100;
	Alpha	=	dPUT_NORMAL;
	Zero.x	=	0xffff;
	Zero.y	=	0xffff;

	imgRect.Set(0,0,0,0);
}

cWORKWND::~cWORKWND()
{
}

void
cWORKWND::CloseWND()
{	Close();
}

void
cWORKWND::SetBarMenu()
{
	barMenu[0 ].Set("외곽선"	,5			,HEIGHT-22	,71		,HEIGHT-4	,IDM_SET_OUTLINE);
	barMenu[1 ].Set("그림자"	,5			,HEIGHT-42	,71		,HEIGHT-24	,IDM_SET_SHADOW);
	barMenu[2 ].Set("배경그림"	,5			,HEIGHT-62	,71		,HEIGHT-44	,IDM_SET_BACK);
	barMenu[3 ].Set("그림정보"	,5			,HEIGHT-82	,71		,HEIGHT-64	,IDM_SET_SPRITEINFO);
	barMenu[13].Set("Block 1"	,5			,HEIGHT-102	,71		,HEIGHT-84	,IDM_BLOCK_1);
	barMenu[14].Set("Block 2"	,5			,HEIGHT-122	,71		,HEIGHT-104	,IDM_BLOCK_2);
	barMenu[15].Set("Block 3"	,5			,HEIGHT-142	,71		,HEIGHT-124	,IDM_BLOCK_3);
	barMenu[16].Set("Block 4"	,5			,HEIGHT-162	,71		,HEIGHT-144	,IDM_BLOCK_4);
	barMenu[17].Set("Block 5"	,5			,HEIGHT-182	,71		,HEIGHT-164	,IDM_BLOCK_5);

	barMenu[4 ].Set("불꽃"		,WIDTH-71	,HEIGHT-22	,WIDTH-5,HEIGHT-4	,IDM_ALPHA_SOFTDODGE);
	barMenu[5 ].Set("중독"		,WIDTH-71	,HEIGHT-42	,WIDTH-5,HEIGHT-24	,IDM_ALPHA_DIFFEREMT);
	barMenu[6 ].Set("그을림"	,WIDTH-71	,HEIGHT-62	,WIDTH-5,HEIGHT-44	,IDM_ALPHA_DODGEBURN);
	barMenu[7 ].Set("연기"		,WIDTH-71	,HEIGHT-82	,WIDTH-5,HEIGHT-64	,IDM_ALPHA_SOFT);
	barMenu[8 ].Set("빛마법"	,WIDTH-71	,HEIGHT-102	,WIDTH-5,HEIGHT-84	,IDM_ALPHA_DODGE);
	barMenu[9 ].Set("라이트"	,WIDTH-71	,HEIGHT-122	,WIDTH-5,HEIGHT-104	,IDM_ALPHA_LIGHT);
	barMenu[10].Set("라이트EX"	,WIDTH-71	,HEIGHT-142	,WIDTH-5,HEIGHT-124	,IDM_ALPHA_EXLIGHT);
	barMenu[11].Set("반투명"	,WIDTH-71	,HEIGHT-162	,WIDTH-5,HEIGHT-144	,IDM_ALPHA_HALF);
	barMenu[12].Set("보통"		,WIDTH-71	,HEIGHT-182	,WIDTH-5,HEIGHT-164	,IDM_ALPHA_NORMAL);

}

BOOL
cWORKWND::Init(HINSTANCE hInst,HWND hWnd,cRECT *rect)
{	lpszRegister	=	"WORKWINDOW";
	Close();

	if (!cDIBWND::Init(	0,(LPTSTR)lpszRegister,(LPTSTR)lpszRegister,
						WS_CHILD,
						rect->x1,rect->y1,rect->x2,rect->y2,
						hWnd,NULL,hInst, 0L,
						SW_SHOW,(WNDPROC)WNDProc
						)
		)
		return FALSE;

	Draw();

	return TRUE;
}

void
cWORKWND::ResetZero()
{	Zero.Set(width() /2,height()*19/32);
}

void
cWORKWND::PointedCenter()
{
}

void
cWORKWND::DrawBarMenu()
{	int i,alpha;
	
	memset(barStatus,0,sizeof(barStatus));

	barStatus[IDM_SET_OUTLINE		]	=	_cfg.bDRAWOUTLINE;
	barStatus[IDM_SET_SHADOW		]	=	_cfg.bDRAWSHADOW;
	barStatus[IDM_SET_BACK			]	=	_cfg.bDRAWBACK;
	barStatus[IDM_SET_SPRITEINFO	]	=	_cfg.bDRAWSPRITEINFO;

	barStatus[IDM_BLOCK_1+g_iBlockSize-1]	=	1;


	if (_MAIN.workMode	==	dWORK_SET_FRAME)alpha	=	ANM[cANM::CurAnm].ALPHA;
	else									alpha	=	Alpha;

	switch(alpha)
	{	case	dPUT_SOFT_DODGE		:	barStatus[IDM_ALPHA_SOFTDODGE	]	=	TRUE;break;
		case	dPUT_DIFFERENT		:	barStatus[IDM_ALPHA_DIFFEREMT	]	=	TRUE;break;
		case	dPUT_DODGE_BURN		:	barStatus[IDM_ALPHA_DODGEBURN	]	=	TRUE;break;
		case	dPUT_SOFT			:	barStatus[IDM_ALPHA_SOFT		]	=	TRUE;break;
		case	dPUT_DODGE			:	barStatus[IDM_ALPHA_DODGE		]	=	TRUE;break;
		case	dPUT_LIGHT			:	barStatus[IDM_ALPHA_LIGHT		]	=	TRUE;break;
		case	dPUT_EXTREAM_LIGHT	:	barStatus[IDM_ALPHA_EXLIGHT		]	=	TRUE;break;
		case	dPUT_HALF_BLENDING	:	barStatus[IDM_ALPHA_HALF		]	=	TRUE;break;
		case	dPUT_NORMAL			:	barStatus[IDM_ALPHA_NORMAL		]	=	TRUE;break;
	}

	for(i=0;i<dBAR_COUNT;i++)
	{	if (i< 6 && !_cfg.bOPTIONBAR) continue;
		if (i>=6 && !_cfg.bEFFECTBAR) continue;

		WORD color	=	0;
		if (barStatus[i])	color	=	_LTCYAN;
		cDRAW::Box	 (0	   ,barMenu[i].rect.x1  ,barMenu[i].rect.y1  ,barMenu[i].rect.x2  ,barMenu[i].rect.y2  );
		cDRAW::FillHB(color,barMenu[i].rect.x1+1,barMenu[i].rect.y1+1,barMenu[i].rect.x2-1,barMenu[i].rect.y2-1);
		cTEXT::Put	 (barMenu[i].rect.x1+4,barMenu[i].rect.y1+4,0x7fff,barMenu[i].string);
	}
}

void
cWORKWND::Draw()
{	if (!Active()) return;

	Mouse.MousePos(hWND);
	SetBarMenu();

	int	xp,yp;

	if (Zero.x	==	0xffff)	ResetZero();

	Zero.x	=	max(10,Zero.x);
	Zero.y	=	max(10,Zero.y);
	Zero.x	=	min(Zero.x,width()-10);
	Zero.y	=	min(Zero.y,height()-10);

	xp		=	Zero.x;
	yp		=	Zero.y;

	if (_cfg.bDRAWBACK && wallPaper)
	{	int i,j; 
		for (i=0;i<height()/wallPaper->Height+1;i++)	for (j=0;j<width()/wallPaper->Width+1;j++)
			wallPaper->Put(j*wallPaper->Width,i*wallPaper->Height);
	}
	else	cDRAW::Fill(_cfg.workColor);

	int	tx=8,ty=8,line=1,i;
	int work=_MAIN.workMode;

	if (cIMG::pCURRENT)
	{	int alpha	=	Alpha;

		if (_cfg.putCROSSBAR == dPUT_BACK|| _cfg.putCROSSBAR == dPUT_HALF)
		{	for (i=Zero.y-l_iGradeY2*Zoom/100-(l_iGradeY*Zoom/100)*128;i<Zero.y+l_iGradeY*128;i+=(l_iGradeY*Zoom/100))	cDRAW::XLine(_GRAY,0,cDRAW::WIDTH-1 ,i,max(Zoom/100,1));
			for (i=Zero.x-l_iGradeX2*Zoom/100-(l_iGradeX*Zoom/100)*128;i<Zero.x+l_iGradeX*128;i+=(l_iGradeX*Zoom/100))	cDRAW::YLine(_GRAY,0,cDRAW::HEIGHT-1,i,max(Zoom/100,1));

			cDRAW::XLine(_cfg.crossColor,0,cDRAW::WIDTH-1 ,Zero.y,max(Zoom/100,1));
			cDRAW::YLine(_cfg.crossColor,0,cDRAW::HEIGHT-1,Zero.x,max(Zoom/100,1));

			int x,y;

			if (_MAIN.workMode == dWORK_SET_CRASH)
			{	for (y=-128;y<128;y++)
					for (x=-128;x<128;x++)
					{	BYTE block	=	CRASHES.Get(x,y);

						sdDATA.Put(Zero.x+x*l_iGradeX*Zoom/100-l_iGradeX2*Zoom/100,Zero.y+y*l_iGradeY*Zoom/100-l_iGradeY2*Zoom/100,block,Zoom*4,Zoom*4,dPUT_COLOR,_LTRED);
					}
			}

			if (_MAIN.workMode == dWORK_SET_BLOCK)
			{	for (y=-128;y<128;y++)
					for (x=-128;x<128;x++)
					{	BYTE block	=	min(BLOCKS.Get(x,y),1);

						sdDATA.Put(Zero.x+x*l_iGradeX*Zoom/100-l_iGradeX2*Zoom/100,Zero.y+y*l_iGradeY*Zoom/100-l_iGradeY2*Zoom/100,block,Zoom*4,Zoom*4,dPUT_COLOR,_LTRED);
					}
			}

			if (_MAIN.workMode == dWORK_SET_OVERLAP)
			{	for (y=-128;y<128;y++)
					for (x=-128;x<128;x++)
					{	BYTE block	=	min(OVERLAPBLOCKS.Get(x,y),1);

						sdDATA.Put(Zero.x+x*l_iGradeX*Zoom/100-l_iGradeX2*Zoom/100,Zero.y+y*l_iGradeY*Zoom/100-l_iGradeY2*Zoom/100,block,Zoom*4,Zoom*4,dPUT_COLOR,_LTRED);
					}
			}
		}

		if (_cfg.bDRAWSHADOW && cIMG::pCURRENT->pSHADOW )
		{	
			if (_MAIN.workMode	==	dWORK_SET_SPRITE)	cIMG::pCURRENT->PutShadow(xp,yp,Zoom,TRUE);
		}

		if (_MAIN.workMode	==	dWORK_SET_SPRITE)
		{	if (_cfg.putCROSSBAR == dPUT_HALF)	cIMG::pCURRENT->Put(xp,yp,Zoom,FALSE,dPUT_HALF_BLENDING);
			else								cIMG::pCURRENT->Put(xp,yp,Zoom,FALSE,alpha);
		}
		else
		{
			cIMG	*lpImg	=	cIMG::Get(0);

			lpImg->PutShadow(xp,yp,Zoom,TRUE);

			if (_cfg.putCROSSBAR == dPUT_HALF)	lpImg->Put(xp,yp,Zoom,FALSE,dPUT_HALF_BLENDING);
			else								lpImg->Put(xp,yp,Zoom,FALSE,dPUT_NORMAL);

			lpImg	=	lpImg->pNEXT;

			for (int i=1;i<cIMG::Count;i++)
			{
				if(g_aBuildBlock[i])
				{
					lpImg->PutShadow(xp,yp,Zoom,TRUE);

					if (_cfg.putCROSSBAR == dPUT_HALF)	lpImg->Put(xp,yp,Zoom,FALSE,dPUT_HALF_BLENDING);
					else								lpImg->Put(xp,yp,Zoom,FALSE,dPUT_NORMAL);
				}

				lpImg	=	lpImg->pNEXT;
			}
		}

		if (_MAIN.BGAnm != 0xffff && _MAIN.workMode	==	dWORK_SET_FRAME)
			ANM[_MAIN.BGAnm].Put(xp,yp,Zoom,FALSE);

		if (_cfg.bDRAWOUTLINE)
		{	if (_MAIN.workMode	==	dWORK_SET_FRAME)
					cIMG::pCURRENT->PutLayer(xp-cANM::ReformPosX()*Zoom/100,yp-cANM::ReformPosY()*Zoom/100,_cfg.outColor,Zoom,FALSE);
			if (_MAIN.workMode	==	dWORK_SET_SPRITE)
					cIMG::pCURRENT->PutLayer(xp,yp,_cfg.outColor,Zoom,FALSE);
		}

		line		=	1+_cfg.bDRAWSPRITEINFO*2;
		if (_MAIN.workMode == dWORK_SET_FRAME && _cfg.bDRAWSPRITEINFO)	line++;

		cDRAW::FillHB (0,4,4,180,4+line*15+4);

		cTEXT::Put(tx,ty    ,0x7fff,_ms ("이미지     :     /"));
		cTEXT::Put(tx,ty    ,_LTCYAN,_ms("             %4d  %4d",cIMG::Current,cIMG::Count));

		if (_cfg.bDRAWSPRITEINFO)
		{	cTEXT::Put(tx,ty+=15,0x7fff,_ms ("사이즈     :     *"));
			cTEXT::Put(tx,ty    ,_LTCYAN,_ms("             %4d  %4d",cIMG::pCURRENT->xs,cIMG::pCURRENT->ys));
			cTEXT::Put(tx,ty+=15,0x7fff,_ms ("교정       :     ,"));
			cTEXT::Put(tx,ty    ,_LTCYAN,_ms("             %4d  %4d",-cIMG::pCURRENT->xp,-cIMG::pCURRENT->yp));

			if (_MAIN.workMode == dWORK_SET_FRAME)
			{	cTEXT::Put(tx,ty+=15,0x7fff,_ms ("프레임교정 :     ,"));
				cTEXT::Put(tx,ty    ,_LTCYAN,_ms("             %4d  %4d",-cANM::ReformPosX(),-cANM::ReformPosY()));
			}
		}

		if (_cfg.putCROSSBAR == dPUT_FRONT)
		{	for (i=Zero.y-(l_iGradeY*Zoom/100)*128-l_iGradeY2*Zoom/100;i<Zero.y+l_iGradeY*128;i+=(l_iGradeY*Zoom/100))	cDRAW::XLine(_GRAY,0,cDRAW::WIDTH-1 ,i,max(Zoom/100,1));
			for (i=Zero.x-(l_iGradeX*Zoom/100)*128-l_iGradeX2*Zoom/100;i<Zero.x+l_iGradeX*128;i+=(l_iGradeX*Zoom/100))	cDRAW::YLine(_GRAY,0,cDRAW::HEIGHT-1,i,max(Zoom/100,1));

			cDRAW::XLine(_cfg.crossColor,0,cDRAW::WIDTH-1 ,Zero.y,max(Zoom/100,1));
			cDRAW::YLine(_cfg.crossColor,0,cDRAW::HEIGHT-1,Zero.x,max(Zoom/100,1));

			int x,y;


			if (_MAIN.workMode == dWORK_SET_CRASH)
			{	for (y=-128;y<128;y++)
					for (x=-128;x<128;x++)
					{	BYTE block	=	CRASHES.Get(x,y);

						sdDATA.Put(Zero.x+x*l_iGradeX*Zoom/100-l_iGradeX2*Zoom/100,Zero.y+y*l_iGradeY*Zoom/100-l_iGradeY2*Zoom/100,block,Zoom*4,Zoom*4,dPUT_COLOR,_LTRED);
					}
			}

			if (_MAIN.workMode == dWORK_SET_BLOCK)
			{	for (y=-128;y<128;y++)
					for (x=-128;x<128;x++)
					{	BYTE block	=	min(BLOCKS.Get(x,y),1);

						sdDATA.Put(Zero.x+x*l_iGradeX*Zoom/100-l_iGradeX2*Zoom/100,Zero.y+y*l_iGradeY*Zoom/100-l_iGradeY2*Zoom/100,block,Zoom*4,Zoom*4,dPUT_COLOR,_LTRED);
					}
			}

			if (_MAIN.workMode == dWORK_SET_OVERLAP)
			{	for (y=-128;y<128;y++)
					for (x=-128;x<128;x++)
					{	BYTE block	=	min(OVERLAPBLOCKS.Get(x,y),1);

						sdDATA.Put(Zero.x+x*l_iGradeX*Zoom/100-l_iGradeX2*Zoom/100,Zero.y+y*l_iGradeY*Zoom/100-l_iGradeY2*Zoom/100,block,Zoom*4,Zoom*4,dPUT_COLOR,_LTRED);
					}
			}
		}

		DrawBarMenu();
	}
	else
	{	cDRAW::FillHB(0,4,4,200,line*14+8);
		cTEXT::Put(tx,ty,0x7fff,"이미지 데이터 없음");
		DrawBarMenu();
	}
	
	cDIBWND::Draw();

}

void
cWORKWND::ZoomIn()	
{	Zoom	=	min(Zoom+100,500);

	if (Zoom == 150) Zoom = 100;

	Draw();
	if (Zoom>=500) _MAIN.TOOLBAR.Disable(IDM_ZOOMIN);
	_MAIN.TOOLBAR.Enable(IDM_ZOOMOUT);
}

void
cWORKWND::ZoomOut()	
{	Zoom	=	max(Zoom-100,50);
	Draw();	
	if (Zoom==50) _MAIN.TOOLBAR.Disable(IDM_ZOOMOUT);
	_MAIN.TOOLBAR.Enable(IDM_ZOOMIN);
}

BOOL
cWORKWND::Run()
{	int		current=0xffff;
	static	BOOL	isIn	=	FALSE;

	BOOL	lb	=	FALSE,rb	=	FALSE;

	Mouse.MousePos(hWND);
	if ((keyBuff[VK_LBUTTON	] & 0x80) == 0 )	lbDown	=	FALSE;
	else										lb		=	TRUE;
	if ((keyBuff[VK_RBUTTON	] & 0x80) == 0 )	rbDown	=	FALSE;
	else										rb		=	TRUE;

	for (int i=0;i<dBAR_COUNT;i++)	if (barMenu[i].rect.isIN(Mouse.x,Mouse.y)) current=i;

	if (keyBuff[VK_LBUTTON	] & 0x80	)
	{	if (!lbDown && current != 0xffff)
		{	lbDown	=	TRUE;
			switch(current)
			{	case	IDM_SET_OUTLINE		:	_cfg.bDRAWOUTLINE	=1-_cfg.bDRAWOUTLINE	;break;
				case	IDM_SET_SHADOW		:	_cfg.bDRAWSHADOW	=1-_cfg.bDRAWSHADOW		;break;
				case	IDM_SET_BACK		:	_cfg.bDRAWBACK		=1-_cfg.bDRAWBACK		;break;
				case	IDM_SET_SPRITEINFO	:	_cfg.bDRAWSPRITEINFO=1-_cfg.bDRAWSPRITEINFO	;break;

				case	IDM_BLOCK_1			:
				case	IDM_BLOCK_2			:
				case	IDM_BLOCK_3			:
				case	IDM_BLOCK_4			:
				case	IDM_BLOCK_5			:
					g_iBlockSize	=	current	-	IDM_BLOCK_1+1;
					break;

				case	IDM_ALPHA_SOFTDODGE	:
				case	IDM_ALPHA_DIFFEREMT	:
				case	IDM_ALPHA_DODGEBURN	:
				case	IDM_ALPHA_SOFT		:
				case	IDM_ALPHA_DODGE		:
				case	IDM_ALPHA_LIGHT		:
				case	IDM_ALPHA_EXLIGHT	:
					if (_MAIN.workMode==dWORK_SET_FRAME)
					{	ANM[cANM::CurAnm].ALPHA	=dPUT_SOFT_DODGE+current-IDM_ALPHA_SOFTDODGE;
						bVIRGIN			=	FALSE;
					}
					else	Alpha					=dPUT_SOFT_DODGE+current-IDM_ALPHA_SOFTDODGE;
					
					break;

				case	IDM_ALPHA_HALF		:
					if (_MAIN.workMode==dWORK_SET_FRAME)
					{	ANM[cANM::CurAnm].ALPHA	=dPUT_HALF_BLENDING;
						bVIRGIN			=	FALSE;
					}
					else	Alpha					=dPUT_HALF_BLENDING;
					break;

				case	IDM_ALPHA_NORMAL	:
					if (_MAIN.workMode==dWORK_SET_FRAME)
					{	ANM[cANM::CurAnm].ALPHA	=dPUT_NORMAL;
						bVIRGIN			=	FALSE;
					}
					else	Alpha					=dPUT_NORMAL;
					break;

			}
			Draw();
		}
	}


	curCursor	=	0;

	if (cIMG::pCURRENT && client.isIN(Mouse.x,Mouse.y))
	{	int xp=cIMG::pCURRENT->xp,yp=cIMG::pCURRENT->yp,xs=cIMG::pCURRENT->xs,ys=cIMG::pCURRENT->ys;
		imgRect.Set(Zero.x-xp*Zoom/100,Zero.y-yp*Zoom/100,Zero.x-xp*Zoom/100+xs*Zoom/100,Zero.y-yp*Zoom/100+ys*Zoom/100);

		if (keyBuff[VK_SPACE	] & 0x80)
		{	curCursor	=	1;
			if(lb)
			{	lbDown		=	TRUE;
				if (!moveCross)
				{	moveCross	=	TRUE;
					SetCursor(_MAIN.handCursor);
				}
			}
			else
			{	if (GetCursor()!=_MAIN.defaultCursor)	SetCursor(_MAIN.defaultCursor);
				moveCross	=	FALSE;
			}
		}
		else
		{	if (GetCursor()!=_MAIN.defaultCursor && !moveSprite)	SetCursor(_MAIN.defaultCursor);

			moveCross	=	FALSE;

			if (client.isIN(Mouse.x,Mouse.y))
			{	if(lb)
				{	if (!moveSprite && (_MAIN.workMode == dWORK_SET_SPRITE || _MAIN.workMode == dWORK_SET_FRAME))
					{	int addX=0,addY=0;

						if (_MAIN.workMode == dWORK_SET_FRAME)
						{	addX	=	cANM::ReformPosX();
							addY	=	cANM::ReformPosY();
						}
						
						if (cIMG::pCURRENT->isIN((Mouse.x-imgRect.x1)*100/Zoom+addX*100/Zoom,(Mouse.y-imgRect.y1)*100/Zoom+addY*100/Zoom))
						{	moveSprite	=	TRUE;
							movePoll	=	Mouse;

							if (_MAIN.workMode	==	dWORK_SET_FRAME)
							{	oldPos.x	=	cANM::ReformPosX();
								oldPos.y	=	cANM::ReformPosY();
							}
							else
							{	oldPos.x	=	cIMG::pCURRENT->xp;
								oldPos.y	=	cIMG::pCURRENT->yp;
							}
							SetCursor(_MAIN.handCursor);
						}
					}
				}
				else	moveSprite	=	FALSE;
			}
			else
			{	if (GetCursor()!=_MAIN.defaultCursor)	SetCursor(_MAIN.defaultCursor);
				moveSprite	=	FALSE;
			}

			if (_MAIN.workMode	==	dWORK_SET_CRASH)
			{	if (lb && !lbDown)
				{	int xs = Mouse.x-Zero.x+l_iGradeX2*Zoom/100;
					int ys = Mouse.y-Zero.y+l_iGradeY2*Zoom/100;
					if (xs < 0) xs	=	xs/(l_iGradeX*Zoom/100)-1;
					else		xs	=	xs/(l_iGradeX*Zoom/100);
					if (ys < 0) ys	=	ys/(l_iGradeY*Zoom/100)-1;
					else		ys	=	ys/(l_iGradeY*Zoom/100);

					if (xs < 128 && xs >= -128 && ys < 128 && ys >= -128)
					{//	CRASHES.Set(xs,ys,CurrentBlock);
						for (int i=-(g_iBlockSize-1);i<=(g_iBlockSize-1);i++)
							for (int j=-(g_iBlockSize-1);j<=(g_iBlockSize-1);j++)
							{	CRASHES.Set(xs+i,ys+j,CurrentBlock);
							}

						bVIRGIN	=	FALSE;
						Draw();
					}
				}
			}

			if (_MAIN.workMode	==	dWORK_SET_BLOCK)
			{	if (lb && !lbDown)
				{	int xs = Mouse.x-Zero.x+l_iGradeX2*Zoom/100;
					int ys = Mouse.y-Zero.y+l_iGradeY2*Zoom/100;
					if (xs < 0) xs	=	xs/(l_iGradeX*Zoom/100)-1;
					else		xs	=	xs/(l_iGradeX*Zoom/100);
					if (ys < 0) ys	=	ys/(l_iGradeY*Zoom/100)-1;
					else		ys	=	ys/(l_iGradeY*Zoom/100);

					if (xs < 128 && xs >= -128 && ys < 128 && ys >= -128)
					{	
						for (int i=-(g_iBlockSize-1);i<=(g_iBlockSize-1);i++)
							for (int j=-(g_iBlockSize-1);j<=(g_iBlockSize-1);j++)
							{	BLOCKS.Set(xs+i,ys+j,CurrentBlock);
							}

						bVIRGIN	=	FALSE;
						Draw();
					}
				}
			}

			if (_MAIN.workMode	==	dWORK_SET_OVERLAP)
			{	if (lb && !lbDown)
				{	int xs = Mouse.x-Zero.x+l_iGradeX2*Zoom/100;
					int ys = Mouse.y-Zero.y+l_iGradeY2*Zoom/100;
					if (xs < 0) xs	=	xs/(l_iGradeX*Zoom/100)-1;
					else		xs	=	xs/(l_iGradeX*Zoom/100);
					if (ys < 0) ys	=	ys/(l_iGradeY*Zoom/100)-1;
					else		ys	=	ys/(l_iGradeY*Zoom/100);

					if (xs < 128 && xs >= -128 && ys < 128 && ys >= -128)
					{	
						for (int i=-(g_iBlockSize-1);i<=(g_iBlockSize-1);i++)
							for (int j=-(g_iBlockSize-1);j<=(g_iBlockSize-1);j++)
							{	OVERLAPBLOCKS.Set(xs+i,ys+j,CurrentBlock);
							}

						bVIRGIN	=	FALSE;
						Draw();
					}
				}
			}

		}
	}
	else
	{	moveCross	=	FALSE;
		moveSprite	=	FALSE;
	}

	if (client.isIN(Mouse.x,Mouse.y))
	{	if (!oldMouse.match(Mouse))
		{	if (moveCross)
			{	Zero.x	-=	(oldMouse.x-Mouse.x);
				Zero.y	-=	(oldMouse.y-Mouse.y);
				Draw();
			}
			else
			if (moveSprite)
			{	int x,y;
				static	int	dx	=	0,dy	=	0;
				x	=	(movePoll.x	-	Mouse.x)*100/Zoom;
				y	=	(movePoll.y	-	Mouse.y)*100/Zoom;

				if (dx!=x || dy!=y)
				{	if (_MAIN.workMode == dWORK_SET_SPRITE)
					{	cIMG::pCURRENT->rePos(oldPos.x+x,oldPos.y+y);
						dx	=	x	,	dy	=	y;
					}

					if (_MAIN.workMode == dWORK_SET_FRAME)
					{	cANM::ReformPos(oldPos.x+x,oldPos.y+y);
						dx	=	x	,	dy	=	y;
					}

					Draw();
				}
			}
		}
		isIn	=	TRUE;
	}
	else
	{	if (isIn)	Draw();
		isIn	=	FALSE;
	}

	oldMouse	=	Mouse;


	return TRUE;
}

#define	IDM_CROSS_FRONT		1000
#define	IDM_CROSS_BACK		1001
#define	IDM_CROSS_HALF		1002
#define	IDM_CROSS_VALID		1003

void
cWORKWND::PopupMenu()
{	if (cIMG::Count		<=	0				)	return;

	HMENU	hMenu	= CreatePopupMenu();
	cPOINT	pos;

	pos.MousePos();

  	AppendMenu(hMenu,MFT_STRING,IDM_CROSS_FRONT	,"십자선을 이미지 앞으로");
  	AppendMenu(hMenu,MFT_STRING,IDM_CROSS_BACK	,"십자선을 이미지 뒤로");
  	AppendMenu(hMenu,MFT_STRING,IDM_CROSS_HALF	,"십자선을 이미지와 반투명으로");
  	AppendMenu(hMenu,MFT_STRING,IDM_CROSS_VALID	,"십자선을 없애기");

	CheckMenuItem(hMenu,IDM_CROSS_FRONT	+_cfg.putCROSSBAR	,MF_BYCOMMAND|MF_CHECKED);

	TrackPopupMenu(hMenu,TPM_RIGHTBUTTON|TPM_TOPALIGN|TPM_LEFTALIGN,pos.x,pos.y,0,hWND,NULL);
	DestroyMenu(hMenu);
}

void
cWORKWND::SelectAnm()
{
}

LRESULT CALLBACK
cWORKWND::WNDProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{	switch(uMsg)
	{	case WM_PAINT			:
			_WORKWND.Draw();
			break;

		case WM_KEYDOWN			:
			_MAIN.updateKey(wParam);
			break;

		case WM_RBUTTONDOWN		:
			_WORKWND.PopupMenu();
			break;

		case WM_SETCURSOR		:
			if (moveSprite || moveCross)
					SetCursor(_MAIN.handCursor);
			else	SetCursor(_MAIN.defaultCursor);
			break;

		case WM_COMMAND		:
			switch( LOWORD( wParam ) )
			{	case	IDM_CROSS_BACK		:
				case	IDM_CROSS_FRONT		:
				case	IDM_CROSS_HALF		:
				case	IDM_CROSS_VALID		:
					_cfg.putCROSSBAR	=	LOWORD( wParam )-IDM_CROSS_FRONT;
					_WORKWND.Draw();
					break;

			}


		case WM_CLOSE			:
		case WM_DESTROY			:
			return (0L);

		case WM_SYSCOMMAND	:
			if ((LOWORD(wParam)&0xfff0) == SC_MOVE) return FALSE;
			return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
			break;

		default					:
			return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}