#include "cMAIN.H"
#include "cWORKWND.H"
#include "cDRAW.H"
#include "cANM.H"
#include "cTEXT.H"
#include "cANGLE.H"

cPOINT		revisedPos[100]	;
char		revisedCount=	0;
cWORKWND	_WORKWND;

#define	IDM_SET_OUTLINE			0
#define	IDM_SET_SHADOW			1
#define	IDM_SET_BACK			2
#define	IDM_SET_SPRITEINFO		3
#define	IDM_SET_CRASHBOX		4
#define	IDM_PUT_INDEX			5
#define	IDM_LOCK				6

#define	IDM_COMPARE_DATA		7

#define	IDM_ALPHA_SOFTDODGE		8
#define	IDM_ALPHA_DIFFEREMT		9
#define	IDM_ALPHA_DODGEBURN		10
#define	IDM_ALPHA_SOFT			11
#define	IDM_ALPHA_DODGE			12
#define	IDM_ALPHA_LIGHT			13
#define	IDM_ALPHA_EXLIGHT		14
#define	IDM_ALPHA_NORMAL		15
#define	IDM_ALPHA_HALF			16

#define	IDM_IMAGE_8BIT			17
#define	IDM_IMAGE_16BIT			18

#define	IDM_PLAY_ANM			19

#define	IDM_INCREASE_FPS		20
#define	IDM_FPS					21
#define	IDM_DECREASE_FPS		22

#define	IDM_PPS_M10				29
#define	IDM_PPS_M2				30
#define	IDM_PPS					31
#define	IDM_PPS_P2				32
#define	IDM_PPS_P10				33
#define	IDM_MOVE_OVAL			34
#define	IDM_MOVE_CIRCLE			35

#define	IDM_ANM_KIND			36
#define	IDM_DATA_CHARACTER		37
#define	IDM_DATA_WEAPON			38
#define	IDM_DATA_WEAPONEFFECT	39
#define	IDM_DATA_SHIELD			40
#define	IDM_DATA_SHIELDEFFECT	41
#define	IDM_DATA_EFFECT			42

#define	IDM_RELEASE_POINT		43

#define	IDM_ANM_FIRST			44
#define	IDM_ANM_LAST			74

#define	IDM_LINK_FIRST			75
#define	IDM_LINK_LAST			105
#define	IDM_ATTACK_ACTION		106
#define	IDM_MAGIC_ACTION		107

#define	dBAR_COUNT				108

//	에니메이션 이동을 위한..
cPOINT	MovePoint;


cRECT	imgRect;

cBARMENU	barMenu[dBAR_COUNT];
BOOL		barStatus[dBAR_COUNT],moveSprite=FALSE,moveCross=FALSE,drawCrash=FALSE,bPRESSBAR=FALSE;
int			curCursor	=	0;

cWORKWND::cWORKWND()
		 :cDIBWND()
{	Zoom	=	100;
	Alpha	=	dPUT_NORMAL;
	Zero.x	=	0xffff;
	Zero.y	=	0xffff;
	Repaint	=	FALSE;

	imgRect.Set(0,0,0,0);
}






void
DrawCircle(int x,int y,WORD color,int size)
{
	int	i,j;

	for (i=0;i<=size/2+1;i++)
	{
		for (j=0;j<=size;j++)
		{	
			int im	=	i-1;
			int	jm	=	j-1;

			int	old	=	(im*2*im*2) + jm*jm;
			int	cur	=	(i*2*i*2) + j*j;

			if (old < size*size && cur >= size*size)
			{
				cDRAW::PutPixel(color,x+j,y+i);
				cDRAW::PutPixel(color,x-j,y+i);
				cDRAW::PutPixel(color,x+j,y-i);
				cDRAW::PutPixel(color,x-j,y-i);
			}
		}
	}
}	//	DrawCircle(int x,int y,WORD color,int size)

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
	barMenu[2 ].Set("배경그림"	,5			,HEIGHT-22	,71		,HEIGHT- 6	,IDM_SET_BACK);
	barMenu[3 ].Set("그림정보"	,5			,HEIGHT-42	,71		,HEIGHT-24	,IDM_SET_SPRITEINFO);
	barMenu[5 ].Set("인덱스"	,5			,HEIGHT-62	,71		,HEIGHT-44	,IDM_PUT_INDEX);

	barMenu[IDM_PLAY_ANM].SetActive(FALSE);
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
{	int i;

	memset(barStatus,0,sizeof(barStatus));

	barStatus[IDM_SET_BACK			]	=	_cfg.bDRAWBACK;
	barStatus[IDM_SET_SPRITEINFO	]	=	_cfg.bDRAWSPRITEINFO;
	barStatus[IDM_PUT_INDEX			]	=	_cfg.bPUTINDEX;

	if (!_cfg.bOPTIONBAR)	for (i=IDM_SET_OUTLINE		;i<=IDM_LOCK		;i++)	barMenu[i].SetActive(FALSE);	//	옵션 바
	if (!_cfg.bEFFECTBAR)	for (i=IDM_ALPHA_SOFTDODGE	;i<=IDM_ALPHA_HALF	;i++)	barMenu[i].SetActive(FALSE);	//	효과 바

	for(i=0;i<dBAR_COUNT;i++)
	{	if (!barMenu[i].Active		) continue;

		WORD color	=	0;
		if (barStatus[i])	color	=	_LTCYAN;
		cDRAW::Box	 (0	   ,barMenu[i].rect.x1  ,barMenu[i].rect.y1  ,barMenu[i].rect.x2  ,barMenu[i].rect.y2  );
		cDRAW::FillHB(color,barMenu[i].rect.x1+1,barMenu[i].rect.y1+1,barMenu[i].rect.x2-1,barMenu[i].rect.y2-1);
		cTEXT::Put	 (barMenu[i].rect.x1+4,barMenu[i].rect.y1+4,0x7fff,barMenu[i].string);
	}
}


void
cWORKWND::Draw()
{
	if (!Active()) return;

	Mouse.MousePos(hWND);
	SetBarMenu();

	if (_cfg.bDRAWBACK && wallPaper)
	{	int i,j;
		for (i=0;i<height()/wallPaper->Height+1;i++)	for (j=0;j<width()/wallPaper->Width+1;j++)
			wallPaper->Put(j*wallPaper->Width,i*wallPaper->Height);
	}
	else	cDRAW::Fill(_cfg.workColor);

	int	tx=8,ty=8,line=1;
	int work=_MAIN.workMode;

	if (!cIMG::pCURRENT)	if (cIMG::Count)	cIMG::SetCURRENT(0);

	if (cIMG::pCURRENT)
	{
//		cDRAW::PutImage(0,0,cIMG::pCURRENT->m_p16Image);
		if (cANM::s_iBPP==	16)
			cDRAW::PutImageScale(0,0,max(Zoom/100,1),cIMG::pCURRENT->m_p16Image);
		else
			cDRAW::PutImageScale(0,0,cIMG::pCURRENT->m_p8Image,cANM::s_awPlt,max(Zoom/100,1));

		if (_cfg.bDRAWSPRITEINFO)
		{
			cDRAW::FillHB(0,4,4,200,2*14+8);
		
			cTEXT::Put(tx,ty    ,0x7fff,_ms ("이미지   :     /"));
			cTEXT::Put(tx,ty    ,_LTCYAN,_ms("           %4d  %4d",cIMG::Current,cIMG::Count));
			cTEXT::Put(tx,ty+=15,0x7fff,_ms ("사이즈   :     *"));
			cTEXT::Put(tx,ty    ,_LTCYAN,_ms("           %4d  %4d",cIMG::pCURRENT->xs,cIMG::pCURRENT->ys));
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
cWORKWND::UpdateBarMenu()
{	int		current=0xffff;
	for (int i=0;i<dBAR_COUNT;i++)	if (barMenu[i].rect.isIN(Mouse.x,Mouse.y) && barMenu[i].Active) current=i;

	if (keyBuff[VK_LBUTTON	] & 0x80)
	{	if (current != 0xffff)	bPRESSBAR	=	TRUE;
		if (!lbDown && current != 0xffff)
		{	lbDown	=	TRUE;
			
			switch(current)
			{	case	IDM_SET_OUTLINE		:	_cfg.bDRAWOUTLINE		=	1-_cfg.bDRAWOUTLINE		;break;
				case	IDM_SET_BACK		:	_cfg.bDRAWBACK			=	1-_cfg.bDRAWBACK		;break;
				case	IDM_SET_SPRITEINFO	:	_cfg.bDRAWSPRITEINFO	=	1-_cfg.bDRAWSPRITEINFO	;break;
				case	IDM_PUT_INDEX		:	
					_cfg.bPUTINDEX		=1-_cfg.bPUTINDEX		;
					_MAIN.Draw();
					break;
			}
			Repaint	=	TRUE;
		}
		lbDown	=	TRUE;
	}
	else	bPRESSBAR=	FALSE;

}

/*
POINT *
TraceLine(int x1,int y1,int x2,int y2,int &count)
{	int	slant,address,len=0,x=0;
	int	index = 0;

	POINT	*values;

	if (x2==x1)
	{	if (y1>y2) swap(y1,y2);

		count	=	y2-y1+1;
		values	=	new POINT [count];
		for (int i=0;i<count;i++)
		{	values[i].x	=	x1;
			values[i].y	=	y1+i;
		}

		return values;
	}

	if (y2==y1)
	{	if (x1>x2) swap(x1,x2);

		count	= x2-x1+1;
		values	=	new POINT [count];
		for (int i=0;i<count;i++)
		{	values[i].x	=	x1+i;
			values[i].y	=	y1;
		}

		return values;
	}

	slant	=	(x2-x1)*(y2-y1);

	int width	=	max(x2,x1)-min(x2,x1)+1;
	int height	=	max(y2,y1)-min(y2,y1)+1;

	count		=	max(width,height);
	values		=	new POINT [count];

	if (x1>x2)
	{	swap(x1,x2);
		swap(y1,y2);
	}

	address	=	x1+y1*WIDTH;

	if (slant > 0)
	{	if (width< height)
			for (int i=0;i<height;i++)
			{	if (x1 >= WIDTH )
				{	count = index;
					return values;
				}
				if (y1 >= HEIGHT)
				{	count = index;
					return values;
				}

				if (x1 >= 0 && y1 >= 0)
				{	values[index].x	=	x;
					values[index].y	=	x;
					index	++;
				}

				len					+=	width;
				y1++;
				if (len >= height)	len-=height,x++,x1++;
			}
		else
			for (int i=0;i<width;i++)
			{	if (x1 >= WIDTH )
				{	count = index;
					return values;
				}
				if (y1 >= HEIGHT)
				{	count = index;
					return values;
				}

				values[index].x	=	x1;
				values[index].y	=	y1;
				index++;

				len					+=	height;
				x1++;
				if (len >= width ) len-=width,address+=WIDTH,y1--;
			}
	}
	else
	{	if (width< height)
			for (int i=0;i<height;i++)
			{	if (x1 >= WIDTH )
				{	count = index;
					return values;
				}
				if (y1 <  0		)
				{	count = index;
					return values;
				}

				if (x1 >= 0 && y1 < HEIGHT)
				{	values[index].x	=	x1;
					values[index].y	=	y1;
					index++;
				}

				len					+=	width;
				address				-=	WIDTH;
				y1--;
				if (len >= height)	len-=height,x++,x1++;
			}
		else
			for (int i=0;i<width;i++)
			{	if (x1 >= WIDTH )
				{	count = index;
					return values;
				}
				if (y1 >= HEIGHT)
				{	count = index;
					return values;
				}

				values[index].x	=	x1;
				values[index].y	=	y1;
				index++;

				len					+=	height;
				x1++;
				if (len >= width ) len-=width,address-=WIDTH,y1++;
			}
	}

	return values;
}

*/

//int	revisedAngle[8]	=	{90,30,0,330,270,210,180,150};
int	revisedAngle[6][16]	=
{	{0},
	{0,180},
	{90,270},
	{90,0,270,180},
	{90,45,0,315,270,225,180,135},
	{90,67,45,22,0,337,315,292,270,247,225,203,180,157,135,112}
};

BOOL
cWORKWND::Run()
{	static	BOOL	isIn	=	FALSE;
	Repaint	=	FALSE;

	Mouse.MousePos(hWND);
	if ((keyBuff[VK_LBUTTON	] & 0x80) == 0 ) lbDown	=	FALSE;
	if ((keyBuff[VK_RBUTTON	] & 0x80) == 0 ) rbDown	=	FALSE;


	UpdateBarMenu();

	MovePoint.x	=	0;
	MovePoint.y	=	0;
	revisedCount=	0;

	curCursor	=	0;


	oldMouse	=	Mouse;

	if(Repaint) Draw();
//	if (cIMG::Count <= 0 ) return TRUE;

	return TRUE;
}

#define	IDM_CROSS_BACK		1000
#define	IDM_CROSS_FRONT		1001
#define	IDM_CROSS_HALF		1002
#define	IDM_CROSS_VALID		1003

void
cWORKWND::PopupMenu()
{
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
					_cfg.putCROSSBAR	=	LOWORD( wParam )-IDM_CROSS_BACK;
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