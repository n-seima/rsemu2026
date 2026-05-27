#include "cMAIN.H"
#include "cFRMWND.H"
#include "cDRAW.H"
#include "cANM.H"
#include "cTEXT.H"


#define	IDM_MINUS_FRAME_5	0
#define	IDM_MINUS_FRAME_1	1
#define	IDM_PLUS_FRAME_1	2
#define	IDM_PLUS_FRAME_5	3

#define	dFRAME_BAR_COUNT	4

cBARMENU	frameMenu[dFRAME_BAR_COUNT];
BOOL		frameStatus[dFRAME_BAR_COUNT];

void
cFRMWND::SetBarMenu()
{	frameMenu[0 ].Set("FRM -5",WIDTH - 48*4,1	,WIDTH - 48*3-2	,19,IDM_MINUS_FRAME_5);
	frameMenu[1 ].Set("FRM -1",WIDTH - 48*3,1	,WIDTH - 48*2-2	,19,IDM_MINUS_FRAME_1);
	frameMenu[2 ].Set("FRM +1",WIDTH - 48*2,1	,WIDTH - 48*1-2	,19,IDM_PLUS_FRAME_1);
	frameMenu[3 ].Set("FRM +5",WIDTH - 48*1,1	,WIDTH - 2		,19,IDM_PLUS_FRAME_5);
}

void
cFRMWND::UpdateBarMenu()
{	int		current=0xffff;
	for (int i=0;i<dFRAME_BAR_COUNT;i++)	if (frameMenu[i].rect.isIN(Mouse.x,Mouse.y)) current=i;

	static	BOOL	ldown	=	FALSE;

	if (keyBuff[VK_LBUTTON	] & 0x80	)
	{	if (!ldown && current != 0xffff)
		{	switch(current)
			{	case	IDM_MINUS_FRAME_5	:	cANM::SetMaxFrame(cANM::GetMaxFrame(cANM::CurAnm)-5);break;
				case	IDM_MINUS_FRAME_1	:	cANM::SetMaxFrame(cANM::GetMaxFrame(cANM::CurAnm)-1);break;
				case	IDM_PLUS_FRAME_1	:	cANM::SetMaxFrame(cANM::GetMaxFrame(cANM::CurAnm)+1);break;
				case	IDM_PLUS_FRAME_5	:	cANM::SetMaxFrame(cANM::GetMaxFrame(cANM::CurAnm)+5);break;
			}
			Draw();
		}
		ldown	=	TRUE;
	}
	else	ldown	=	FALSE;
}

void
cFRMWND::DrawBarMenu()
{	SetBarMenu();

	for(int i=0;i<dFRAME_BAR_COUNT;i++)
	{	WORD color	=	0;
		cDRAW::Box	 (0x7fff,frameMenu[i].rect.x1  ,frameMenu[i].rect.y1  ,frameMenu[i].rect.x2  ,frameMenu[i].rect.y2  );
		cDRAW::FillHB(color,frameMenu[i].rect.x1+1,frameMenu[i].rect.y1+1,frameMenu[i].rect.x2-1,frameMenu[i].rect.y2-1);
		cTEXT::Put	 (frameMenu[i].rect.x1+4,frameMenu[i].rect.y1+4,0x7fff,frameMenu[i].string);
	}
}

cFRMWND	_FRMWND;

cFRMWND::cFRMWND()
        :cDIBWND()
{	Rate	=	100;
	XPos	=	0;
}

cFRMWND::~cFRMWND()
{
}

void
cFRMWND::CloseWND()
{	Close();
}

BOOL
cFRMWND::Init(HINSTANCE hInst,HWND hWnd,cRECT *rect)
{	lpszRegister	=	"FRMWINDOW";
	Close();

	if (!cDIBWND::Init(	0,(LPTSTR)lpszRegister,(LPTSTR)lpszRegister,
						WS_CHILD|WS_HSCROLL,
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
cFRMWND::Draw()
{
	SCROLLINFO si;

	if (cIMG::Count <= 0 )
	{	PAINTSTRUCT ps;
		HDC		hDC		=	GetDC(hWND);

		BeginPaint(hWND, &ps);

		RECT rect;
		client.Set(&rect);

		FillRect(hDC,&rect,(HBRUSH)(COLOR_GRAYTEXT));
		ReleaseDC(hWND,hDC);
		EndPaint(hWND, &ps);

		si.cbSize	=	sizeof(SCROLLINFO);
		si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;
		EnableScrollBar(hWND,SB_HORZ,ESB_DISABLE_BOTH);
		XPos		=	0;
		return;
	}

	if (!Active()) return;

	if	(_MAIN.workMode!=dWORK_SET_FRAME)
	{	cDRAW::Fill(_cfg.frmColor);
		cTEXT::OutLine(TRUE,0);
		cTEXT::Put(4,4,0x7fff,"에니메이션 편집중이 아닙니다.",32);
		cTEXT::OutLine(TRUE,0);
		cDIBWND::Draw();
		return;
	}

	imgXS=(cIMG::BigXS + 5);
	imgYS=(cIMG::BigYS + 5);
	Rate = 100;

	ImgPerLine = (width()  - 8)/ imgXS;

	if (ImgPerLine < _cfg.frmMinCol	)
	{	ImgPerLine	=	_cfg.frmMinCol;
		imgXS		=	(max(width()-8-5*_cfg.frmMinCol,0))/_cfg.frmMinCol  + 5;
		Rate		=	imgXS*100/(cIMG::BigXS + 5);
		imgYS		=	imgYS*Rate/100;
	}

	if (imgYS > height()-26	)
	{	imgYS		=	max(height()-26,1);
		Rate		=	imgYS*100/(cIMG::BigYS + 5);
		imgXS		=	max((cIMG::BigXS + 5)*Rate/100,1);
		ImgPerLine	=	(max(width() -8,0))/ imgXS;
	}

	imgXP	=	(width()-(ImgPerLine*imgXS))/2;
	imgYP	=	height()-imgYS-4;

	if (cANM::GetMaxFrame() > ImgPerLine)
	{	si.cbSize	=	sizeof(SCROLLINFO);
		si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;
		si.nMin		=	0;
		si.nMax		=	cANM::GetMaxFrame()-1;
		si.nPage	=	ImgPerLine;
		si.nPos		=	XPos;
		EnableScrollBar(hWND,SB_HORZ,ESB_ENABLE_BOTH);
		SetScrollInfo(hWND,SB_HORZ,&si,TRUE);

		XPos	=	min(XPos,si.nMax	-	ImgPerLine+1);
	}
	else 
	{	EnableScrollBar(hWND,SB_HORZ,ESB_DISABLE_BOTH);
		XPos		=	0;
	}

	cDRAW::Fill(_cfg.frmColor);

	cDRAW::Fill(_GREEN,2,2, width()-2,18);

	int image	=	cANM::GetFrameImage(cANM::CurFrame);

	cTEXT::Put(8,4,_WHITE,   _ms("에니메이션 %s[%s]    프레임 [%d/%d] - 스프라이트 [%d]",
							ANM[cANM::CurAnm].Name,strDirect[cANM::GetAnmType()][cANM::CurDirect],
							cANM::CurFrame,cANM::GetMaxFrame(),image));

	cTEXT::Put(8,4,_LTYELLOW,_ms("           %s %s             %d %d  -             %d  ",
							ANM[cANM::CurAnm].Name,strDirect[cANM::GetAnmType()][cANM::CurDirect],
							cANM::CurFrame,cANM::GetMaxFrame(),image));

	int i,xp,yp;

	cIMG	*img;

	for (i=0;i<cANM::GetMaxFrame()-XPos;i++)
	{	if (i>=ImgPerLine) break;
		xp	=	imgXP + i*imgXS;
		yp	=	imgYP;

			 if	(_MAIN.workMode!=dWORK_SET_FRAME)	cDRAW::Fill	(_DEEPGRAY	,xp,yp,xp+imgXS-2,yp+imgYS-2);
		else if (i+XPos == cANM::CurFrame)			cDRAW::Fill	(_LTBLUE	,xp,yp,xp+imgXS-2,yp+imgYS-2);
		else										cDRAW::Fill	(0			,xp,yp,xp+imgXS-2,yp+imgYS-2);

		cDRAW::Box	(0x7fff	,xp,yp,xp+imgXS-2,yp+imgYS-2);

		img	=	cIMG::Get(cANM::GetFrameImage(i+XPos));
		if (img)
		{	xp	=	imgXP+i*imgXS+imgXS/2-img->xs*Rate/200+img->xp*Rate/100;
			yp	=	imgYP+imgYS/2-img->ys*Rate/200+img->yp*Rate/100;

			if (cANM::isFlip())	xp = imgXP+i*imgXS+imgXS/2-img->xs*Rate/200 +(img->xs-img->xp)*Rate/100;

			img->Put(xp,yp,Rate,cANM::isFlip(),dPUT_NORMAL);
		}

		xp	=	imgXP + i*imgXS;
		yp	=	imgYP;

		cDRAW::Box	(0x7fff	,xp,yp,xp+imgXS-2,yp+imgYS-2);

		if (_cfg.bPUTINDEX)
		{	cTEXT::OutLine(TRUE,0);
			cTEXT::Put(imgXP+i*imgXS,imgYP,0x7fff,_ms("%d",i+XPos),32);

			if (ANM[cANM::CurAnm].isBack(cANM::CurDirect,XPos+i))
					cTEXT::Put(imgXP+i*imgXS,imgYP+imgYS-14,RGBmix(31,63,0),"B",32);
			else	cTEXT::Put(imgXP+i*imgXS,imgYP+imgYS-14,RGBmix(31,63,0),"F",32);

			cTEXT::OutLine(TRUE,0);
		}
	}
	DrawBarMenu();

	cDIBWND::Draw();

	return;
}

void 
cFRMWND::HScroll(WORD wScroll )
{	SCROLLINFO si;

	si.cbSize	=	sizeof( SCROLLINFO );
	si.fMask	=	SIF_ALL;
	GetScrollInfo( hWND, SB_HORZ, &si );

	switch( wScroll )
	{	case SB_LINERIGHT	:
			if ( XPos <= (int)(si.nMax - si.nPage) )	XPos += 1;
			break;

		case SB_LINELEFT	:
			if ( XPos > 0 ) XPos -= 1;
			break;

		case SB_THUMBTRACK :  // 스크롤바를 잡구 움직였을때..
			GetScrollInfo( hWND, SB_HORZ, &si );
			XPos = si.nTrackPos;
			break;

		case SB_PAGELEFT	:
			XPos	=	max(0,XPos-ImgPerLine);
			break;

		case SB_PAGERIGHT	:
			XPos	=	min(XPos+ImgPerLine,si.nMax - ImgPerLine+1);
			break;
	}

	if (si.nPos != XPos )
	{	si.fMask=	SIF_POS;
		si.nPos	=	XPos;
		SetScrollInfo( hWND, SB_HORZ, &si, TRUE );
		Draw();
	}
}

int
cFRMWND::GetFrame()
{	if (cIMG::Count <= 0 ) return 0xffff;

	for (int i=0;i<cANM::GetMaxFrame()-XPos;i++)
	{	if (i>=ImgPerLine) return 0xffff;

		cRECT rect;
		int xp	=	imgXP + i*imgXS,yp	=	imgYP;

		rect.Set(xp,yp,xp+imgXS-2,yp+imgYS-2);

		if (rect.isIN(Mouse.x,Mouse.y)) return i+XPos;
	}

	return 0xffff;
}

BOOL
cFRMWND::Run()
{	if (cIMG::Count <= 0 ) return FALSE;
	if (_MAIN.workMode	!= dWORK_SET_FRAME) return FALSE;

	Mouse.MousePos(hWND);
	int	pos;
	UpdateBarMenu();

	if (keyBuff[VK_LBUTTON	]	& 0x80)
	{	pos	=	GetFrame();
		if (pos!=0xffff)
		{	cANM::SetCurrentFrame(pos);
			_MAIN.Draw();
		}
	}

	return TRUE;
}

void
cFRMWND::IncFrameIndex()
{	cIMG *img=cIMG::Get(cANM::GetFrameImage(curFrame));
	int image=cIMG::Get(img);

	for (int i=1;i<cANM::GetMaxFrame()-curFrame;i++)
		cANM::SetFrameImage(min(image+i,cIMG::Count-1),curFrame+i);

	cANM::SetCurrentFrame(cANM::CurFrame);
	_MAIN.Draw();
}

void
cFRMWND::AddFrameIndex()
{	cIMG *img=cIMG::Get(cANM::GetFrameImage(curFrame-1));
	int image=cIMG::Get(img);

	image=min(image+1,cIMG::Count-1);

	cANM::SetFrameImage(image,curFrame);

	if (curFrame == cANM::CurFrame)
	{	cANM::SetCurrentFrame(cANM::CurFrame);
		_MAIN.Draw();
	}
	else	Draw();
}

#define	IDM_INC_INDEX		1000
#define	IDM_ADD_INDEX		1001
#define	IDM_SET_END			1002
#define	IDM_PLUS_10FRAME	1003
#define	IDM_SET_FRONT		1004
#define	IDM_SET_BACK		1005
#define	IDM_0FRAME			1006

void
cFRMWND::PopupMenu()
{	if (_MAIN.workMode	!=	dWORK_SET_FRAME)	return;
	if (cIMG::Count <= 0 )	return;

	curFrame	=	GetFrame();


	HMENU	hMenu	= CreatePopupMenu();
	cPOINT	pos;

	pos.MousePos();

	if (curFrame!=0xffff)
	{	AppendMenu(hMenu,MFT_STRING,IDM_INC_INDEX,"이후 프레임 인덱스 증가");

		if (cANM::GetMaxFrame() > 1 && curFrame < cANM::GetMaxFrame()-1)
			AppendMenu(hMenu,MFT_STRING,IDM_SET_END,"이후 프레임 제거");

		AppendMenu(hMenu,MFT_SEPARATOR,0,NULL);
	}

	AppendMenu(hMenu,MFT_STRING,IDM_PLUS_10FRAME,"최대 프레임 +10");
	AppendMenu(hMenu,MFT_STRING,IDM_0FRAME,"프레임 0 으로 설정");

	if (curFrame!=0xffff)
	{	AppendMenu(hMenu,MFT_SEPARATOR,0,NULL);
		AppendMenu(hMenu,MFT_STRING,IDM_SET_FRONT,"이미지 앞에 배치");
		AppendMenu(hMenu,MFT_STRING,IDM_SET_BACK,"이미지 뒤에 배치");
	}
	
	TrackPopupMenu(hMenu,TPM_RIGHTBUTTON|TPM_TOPALIGN|TPM_LEFTALIGN,pos.x,pos.y,0,hWND,NULL);
	DestroyMenu(hMenu);
}

LRESULT CALLBACK 
cFRMWND::WNDProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{	switch(uMsg)
	{	case	WM_PAINT		:
			_FRMWND.Draw();
			break;

		case	WM_CLOSE		:
		case	WM_DESTROY		:
			return (0L);

		case	WM_KEYDOWN		:
			_MAIN.updateKey(wParam);
			break;

		case	WM_RBUTTONDOWN	:
			_FRMWND.PopupMenu();
			break;

		case	WM_HSCROLL		:
			_FRMWND.HScroll(wParam);
			break;

		case WM_COMMAND		:
			switch( LOWORD( wParam ) )
            {	case	IDM_INC_INDEX		:
					_FRMWND.IncFrameIndex();
					break;

				case	IDM_ADD_INDEX		:
					_FRMWND.AddFrameIndex();
					break;

				case	IDM_SET_END			:
					cANM::SetMaxFrame(_FRMWND.curFrame+1);
					_MAIN.Draw();
					break;

				case	IDM_PLUS_10FRAME	:
					cANM::SetMaxFrame(cANM::GetMaxFrame(cANM::CurAnm)+10);
					_MAIN.Draw();
					break;

				case	IDM_0FRAME			:
					cANM::SetMaxFrame(0);
					_MAIN.Draw();
					break;

				case	IDM_SET_FRONT		:
					cANM::SetFront(_FRMWND.curFrame);
					_MAIN.Draw();
					break;

				case	IDM_SET_BACK		:
					cANM::SetBack(_FRMWND.curFrame);
					_MAIN.Draw();
					break;

			}
			break;

		case	WM_SYSCOMMAND	:
			if ((LOWORD(wParam)&0xfff0) == SC_MOVE) return FALSE;
			return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
			break;

		default :
			return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}