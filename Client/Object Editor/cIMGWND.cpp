#include "cMAIN.H"
#include "cIMGWND.H"
#include "cDRAW.H"
#include "cANM.H"
#include "cTEXT.H"

cIMGWND	_IMGWND;

cPOINT	margin;

int		CurrentBlock	=	0;

cIMGWND::cIMGWND()
        :cDIBWND()
{	YPos	=	0;
	Rate	=	100;
	moveImg	=	0xffff;
}

cIMGWND::~cIMGWND()
{
}

void
cIMGWND::CloseWND()
{	Close();
}

BOOL
cIMGWND::Init(HINSTANCE hInst,HWND hWnd,cRECT *rect)
{	lpszRegister	=	"IMGWINDOW";
	Close();

	if (!cDIBWND::Init(0,(LPTSTR)lpszRegister,(LPTSTR)lpszRegister,
					WS_CHILD|WS_VSCROLL,
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
cIMGWND::ViewEnd()
{	imgXS=(cIMG::BigXS + 5);
	imgYS=(cIMG::BigYS + 5);
	Rate = 100;

	ImgPerLine = (width()  - 8)/ imgXS;

	if (ImgPerLine < _cfg.imgMinCol	)
	{	ImgPerLine	=	_cfg.imgMinCol;
		imgXS		=	(max(width()-8-5*_cfg.imgMinCol,0))/_cfg.imgMinCol  + 5;
		Rate		=	imgXS*100/(cIMG::BigXS + 5);
		imgYS		=	imgYS*Rate/100;
	}

	ImgPerCol  = (height() - 8 )/ imgYS;

	if (ImgPerCol < _cfg.imgMinRow	)
	{	ImgPerCol	=	_cfg.imgMinRow;
		imgYS		=	(max(height()-8-5*_cfg.imgMinRow,0))/_cfg.imgMinRow + 5;
		Rate		=	imgYS*100/(cIMG::BigYS + 5);
		imgXS		=	(cIMG::BigXS + 5)*Rate/100;
		ImgPerLine	=	(max(width() -8,0))/ imgXS;
	}

	YPos	=	(cIMG::Count-1)/ImgPerLine - ImgPerCol + 1;
}

void
cIMGWND::Draw()
{	SCROLLINFO	si;
	int			i,j;

	if (cIMG::Count <= 0)
	{
		if (!IsWindow(hWND)) return;

		PAINTSTRUCT ps;
		HDC		hDC		=	GetDC(hWND);

		BeginPaint(hWND, &ps);

		RECT rect;

		client.Set(&rect);

		FillRect(hDC,&rect,(HBRUSH)(COLOR_GRAYTEXT));
		ReleaseDC(hWND,hDC);
		EndPaint(hWND, &ps);

		si.cbSize	=	sizeof(SCROLLINFO);
		si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;
		EnableScrollBar(hWND,SB_VERT,ESB_DISABLE_BOTH);
		YPos		=	0;

		return;
	}

	if (!Active()) return;

	cDRAW::Fill(_cfg.imgColor);

	if (_MAIN.workMode == dWORK_SET_CRASH)
	{	imgXS=(64 + 5);
		imgYS=(32 + 5);
		Rate = 400;

		ImgPerLine = max((width()  - 8)/ imgXS,1);
		ImgPerCol  = max((height() - 8)/ imgYS,1);

		imgXP	=	(width ()-(ImgPerLine*imgXS))/2;
		imgYP	=	(height()-(ImgPerCol *imgYS))/2;

		if (sdDATA.Count > ImgPerCol * ImgPerLine)
		{	si.cbSize	=	sizeof(SCROLLINFO);
			si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;
			si.nMin		=	0;
			si.nMax		=	(sdDATA.Count -1)/ImgPerLine;
			si.nPage	=	ImgPerCol;
			si.nPos		=	YPos;
			EnableScrollBar(hWND,SB_VERT,ESB_ENABLE_BOTH);
			SetScrollInfo(hWND,SB_VERT,&si,TRUE);

			if (YPos > si.nMax - ImgPerCol+1) YPos	=	si.nMax - ImgPerCol+1;
		}
		else 
		{	EnableScrollBar(hWND,SB_VERT,ESB_DISABLE_BOTH);
			YPos		=	0;
		}

		int		count	=	YPos*ImgPerLine,xp,yp;

		for (j=0;j<ImgPerCol ;j++)	
			for (i=0;i<ImgPerLine;i++,count++)
			{	if (count >= sdDATA.Count) break;
				xp = imgXP+i*imgXS+2;
				yp = imgYP+j*imgYS+2;

				int	bxp	=imgXP+i*imgXS,byp=imgYP+j*imgYS;

				if (count == CurrentBlock)	cDRAW::Fill	(_LTBLUE	,bxp,byp,bxp+imgXS-2,byp+imgYS-2);
				else						cDRAW::Fill	(0			,bxp,byp,bxp+imgXS-2,byp+imgYS-2);

				sdDATA.Put(xp,yp,count,Rate,Rate,dPUT_COLOR,_LTRED);
				if (count == CurrentBlock)	cDRAW::Box	(_LTGREEN	,bxp,byp,bxp+imgXS-2,byp+imgYS-2);
				else						cDRAW::Box	(0x7fff		,bxp,byp,bxp+imgXS-2,byp+imgYS-2);
			}

		cDIBWND::Draw();

		return;
		
	}

	if (_MAIN.workMode == dWORK_SET_BLOCK || _MAIN.workMode == dWORK_SET_OVERLAP)
	{	CurrentBlock	=	min(CurrentBlock,1);
		imgXS=(64 + 5);
		imgYS=(32 + 5);
		Rate = 400;

		ImgPerLine = max((width()  - 8)/ imgXS,1);
		ImgPerCol  = max((height() - 8)/ imgYS,1);

		imgXP	=	(width ()-(ImgPerLine*imgXS))/2;
		imgYP	=	(height()-(ImgPerCol *imgYS))/2;

		if (sdDATA.Count > ImgPerCol * ImgPerLine)
		{	si.cbSize	=	sizeof(SCROLLINFO);
			si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;
			si.nMin		=	0;
			si.nMax		=	(sdDATA.Count -1)/ImgPerLine;
			si.nPage	=	ImgPerCol;
			si.nPos		=	YPos;
			EnableScrollBar(hWND,SB_VERT,ESB_ENABLE_BOTH);
			SetScrollInfo(hWND,SB_VERT,&si,TRUE);

			if (YPos > si.nMax - ImgPerCol+1) YPos	=	si.nMax - ImgPerCol+1;
		}
		else 
		{	EnableScrollBar(hWND,SB_VERT,ESB_DISABLE_BOTH);
			YPos		=	0;
		}

		int		count	=	YPos*ImgPerLine,xp,yp;

		for (j=0;j<ImgPerCol ;j++)	
			for (i=0;i<ImgPerLine;i++,count++)
			{	if (count >= 2) break;
				xp = imgXP+i*imgXS+2;
				yp = imgYP+j*imgXS+2;

				int	bxp	=imgXP+i*imgXS,byp=imgYP+j*imgYS;

				if (count == CurrentBlock)	cDRAW::Fill	(_LTBLUE	,bxp,byp,bxp+imgXS-2,byp+imgYS-2);
				else						cDRAW::Fill	(0			,bxp,byp,bxp+imgXS-2,byp+imgYS-2);

//				sdDATA.Put(xp,yp,count);
				sdDATA.Put(xp,yp,count,Rate,Rate,dPUT_COLOR,_LTRED);
				if (count == CurrentBlock)	cDRAW::Box	(_LTGREEN	,bxp,byp,bxp+imgXS-2,byp+imgYS-2);
				else						cDRAW::Box	(0x7fff		,bxp,byp,bxp+imgXS-2,byp+imgYS-2);
			}

		cDIBWND::Draw();

		return;
		
	}

	imgXS=(cIMG::BigXS + 5);
	imgYS=(cIMG::BigYS + 5);
	Rate = 100;

	ImgPerLine = (width()  - 8)/ imgXS;

	if (ImgPerLine < _cfg.imgMinCol	)
	{	ImgPerLine	=	_cfg.imgMinCol;
		imgXS		=	(max(width()-8-5*_cfg.imgMinCol,0))/_cfg.imgMinCol  + 5;
		Rate		=	imgXS*100/(cIMG::BigXS + 5);
		imgYS		=	imgYS*Rate/100;
	}

	ImgPerCol  = (height() - 8 )/ imgYS;

	if (ImgPerCol < _cfg.imgMinRow	)
	{	ImgPerCol	=	_cfg.imgMinRow;
		imgYS		=	(max(height()-8-5*_cfg.imgMinRow,0))/_cfg.imgMinRow + 5;
		Rate		=	imgYS*100/(cIMG::BigYS + 5);
		imgXS		=	(cIMG::BigXS + 5)*Rate/100;
		ImgPerLine	=	(max(width() -8,0))/ imgXS;
	}

	imgXP	=	(width ()-(ImgPerLine*imgXS))/2;
	imgYP	=	(height()-(ImgPerCol *imgYS))/2;

/*
	if (_MAIN.workMode == dWORK_SET_SPRITE)
	{	if (YPos*ImgPerLine > cIMG::Current-1) YPos=(cIMG::Current-1)/ImgPerLine;
		if (YPos*ImgPerLine + ImgPerLine*ImgPerCol <= cIMG::Current )
			YPos=max(cIMG::Current/ImgPerLine-(ImgPerCol -1),0);
	}
*/
	if (cIMG::Count > ImgPerCol * ImgPerLine)
	{	si.cbSize	=	sizeof(SCROLLINFO);
		si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;
		si.nMin		=	0;
		si.nMax		=	(cIMG::Count-1)/ImgPerLine;
		si.nPage	=	ImgPerCol;
		si.nPos		=	YPos;
		EnableScrollBar(hWND,SB_VERT,ESB_ENABLE_BOTH);
		SetScrollInfo(hWND,SB_VERT,&si,TRUE);

		if (YPos > si.nMax - ImgPerCol+1) YPos	=	si.nMax - ImgPerCol+1;
	}
	else 
	{	EnableScrollBar(hWND,SB_VERT,ESB_DISABLE_BOTH);
		YPos		=	0;
	}

	int		count	=	YPos*ImgPerLine,xp,yp;
	cIMG	*img	=	cIMG::Get(count);

	if (img)
	for (j=0;j<ImgPerCol ;j++)	
		for (i=0;i<ImgPerLine;i++,count++)
		{	if (count >= cIMG::Count) break;
			xp = imgXP+i*imgXS+imgXS/2-img->xs*Rate/200+img->xp*Rate/100;
			yp = imgYP+j*imgYS+imgYS/2-img->ys*Rate/200+img->yp*Rate/100;
			int	bxp	=imgXP+i*imgXS,byp=imgYP+j*imgYS;

			if (count == cIMG::Current)						cDRAW::Fill	(_LTBLUE	,bxp,byp,bxp+imgXS-2,byp+imgYS-2);
			else
			{		 if (img->isSelect)						cDRAW::Fill	(_GREEN		,bxp,byp,bxp+imgXS-2,byp+imgYS-2);
				else if (_MAIN.workMode==dWORK_SET_SPRITE)	cDRAW::Fill	(0			,bxp,byp,bxp+imgXS-2,byp+imgYS-2);
				else										cDRAW::Fill	(_DEEPGRAY	,bxp,byp,bxp+imgXS-2,byp+imgYS-2);
			}

			if (cANM::isFlip())	xp = imgXP+i*imgXS+imgXS/2-img->xs*Rate/200 +(img->xs-img->xp)*Rate/100;
			img->Put(xp,yp,Rate,cANM::isFlip(),dPUT_NORMAL);
//			PutSpriteR[dPUT_NORMAL][cANM::isFlip()](xp,yp,img->pSPRITE,Rate,Rate,32);

//			cDRAW::Box	(0x7fff	,bxp,byp,bxp+imgXS-2,byp+imgYS-2);

			if (count == CurrentBlock)	cDRAW::Box	(_LTGREEN	,bxp,byp,bxp+imgXS-2,byp+imgYS-2);
			else						cDRAW::Box	(0x7fff		,bxp,byp,bxp+imgXS-2,byp+imgYS-2);

			if (_cfg.bPUTINDEX)
			{	int x=imgXP+i*imgXS+2,y=imgYP+j*imgYS+2;
				int size = strlen(_ms("%d",count));

				cDRAW::Fill(_GRAY,x,y,x+4+6*size,y+16);
				cTEXT::Put (x+2,y+3,RGBmix(31,63,31),_ms("%d",count),32);
			}

			img=img->pNEXT;
		}

	if (moveImg!=0xffff)
	{	cDRAW::FillHB(_LTBLUE,Mouse.x- margin.x,Mouse.y- margin.y,Mouse.x- margin.x+imgXS-2,Mouse.y+imgYS-2- margin.y);
		img=cIMG::Get(moveImg);

		if (img) 
		{	xp = Mouse.x+imgXS/2-img->xs*Rate/200+img->xp*Rate/100 - margin.x;
			yp = Mouse.y+imgYS/2-img->ys*Rate/200+img->yp*Rate/100 - margin.y;

//			PutSpriteR[dPUT_HALFBLENDING][cANM::isFlip()](xp,yp,img->pSPRITE,Rate,Rate,32);
			img->Put(xp,yp,Rate,cANM::isFlip(),dPUT_HALF_BLENDING);
		}
	}

	cDIBWND::Draw();
}

void 
cIMGWND::VScroll(WORD wScroll )
{	SCROLLINFO si;

	si.cbSize= sizeof( SCROLLINFO );
	si.fMask =SIF_ALL;
	GetScrollInfo( hWND, SB_VERT, &si );

	switch( wScroll )
	{	case SB_LINEDOWN	:
			if ( YPos <= (int)(si.nMax - si.nPage) )	YPos += 1;
			break;

		case SB_LINEUP		:
			if ( YPos > 0 ) YPos -= 1;
			break;

		case SB_THUMBTRACK	:  // 스크롤바를 잡구 움직였을때..
			GetScrollInfo( hWND, SB_VERT, &si );
			YPos = si.nTrackPos;
			break;

		case SB_PAGEUP		:
			YPos	=	max(0,YPos-ImgPerCol);
			break;

		case SB_PAGEDOWN	:
			YPos	=	min(YPos+ImgPerCol,si.nMax - ImgPerCol+1);
			break;
			
	}

	if (si.nPos != YPos )
	{	si.fMask = SIF_POS;
		si.nPos  = YPos;
		SetScrollInfo( hWND, SB_VERT, &si, TRUE );
		Draw();
	}
}

int 
cIMGWND::GetImage(BOOL mar)
{	if (_MAIN.workMode == dWORK_SET_CRASH || _MAIN.workMode == dWORK_SET_BLOCK || _MAIN.workMode == dWORK_SET_OVERLAP)
	{	if (!client.isIN(Mouse.x,Mouse.y)) return 0xffff;

		int	count	=	YPos*ImgPerLine,i,j;

		for (j=0;j<ImgPerCol ;j++)
			for (i=0;i<ImgPerLine;i++,count++)
			{	if (count >= sdDATA.Count) return 0xffff;
				cRECT rect;
				rect.Set(imgXP+i*imgXS,imgYP+j*imgYS,imgXP+(i+1)*imgXS-2,imgYP+(j+1)*imgYS-2);
				if (rect.isIN(Mouse.x,Mouse.y))
				{	CurrentBlock	=	count;
					Draw();
					return	0xffff;
				}
			}

		return 0xffff;
	}

	if (cIMG::Count <= 0 ) return 0xffff;
	if (!client.isIN(Mouse.x,Mouse.y)) return 0xffff;

	int	count	=	YPos*ImgPerLine,i,j;

	for (j=0;j<ImgPerCol ;j++)
		for (i=0;i<ImgPerLine;i++,count++)
		{	if (count >= cIMG::Count) return 0xffff;
			cRECT rect;
			rect.Set(imgXP+i*imgXS,imgYP+j*imgYS,imgXP+(i+1)*imgXS-2,imgYP+(j+1)*imgYS-2);
			if (rect.isIN(Mouse.x,Mouse.y))
			{	if (mar)
				{	margin.x	=	Mouse.x	-	rect.x1;
					margin.y	=	Mouse.y	-	rect.y1;
				}
				return	count;
			}
		}

	return 0xffff;
}

BOOL
cIMGWND::Run()
{	static	cPOINT	oldPos		=	Mouse;
	static	BOOL	pressKey	=	FALSE;
	int		pos;
	cPOINT	MMM;
	cRECT	outFrame;

	MMM.MousePos();
	Mouse.MousePos(hWND);
	client.Set(hWND);
	outFrame.OutFrame(hWND);

	if (moveImg	!= 0xffff)
	{	Mouse.x=max(Mouse.x,0)			,	Mouse.y=max(Mouse.y,0);
		Mouse.x=min(Mouse.x,width()-1 )	,	Mouse.y=min(Mouse.y,height()-1);
	}

	if (keyBuff[VK_LBUTTON] & 0x80)
	{	if (moveImg==0xffff)
		{	if (_MAIN.workMode == dWORK_SET_SPRITE)
			{	if (keyBuff[VK_CONTROL] & 0x80)
				{	pos=_IMGWND.GetImage();
					if (pos!=0xffff && !pressKey)
						if (pos!=cIMG::Current)
						{	cIMG *img=cIMG::Get(pos);
							if (img) img->isSelect=1-img->isSelect;
							_MAIN.Draw();
						}
				}
				else if (keyBuff[VK_SHIFT] & 0x80)
				{	pos=_IMGWND.GetImage();
					if (pos!=0xffff && !pressKey)
						if (pos!=cIMG::Current)
						{	cIMG::Select(pos);
							_MAIN.Draw();
						}
				}
				else
				{	pos=_IMGWND.GetImage();
					if (pos!=0xffff && !pressKey)
						if (pos!=cIMG::Current)
						{	cIMG::SetCURRENT(pos);
							_MAIN.Draw();
						}
				}
			}
			else
			{	pos=_IMGWND.GetImage();

				if (pos!=0xffff && !pressKey )
					if (pos!=cIMG::Current)
					{	cIMG::SetCURRENT(pos);
						if (_MAIN.workMode == dWORK_SET_FRAME)	cANM::SetFrameImage(pos);
						_MAIN.Draw();
					}
			}
		}
		pressKey	=	TRUE;
	}
	else
	{	pressKey	=	FALSE;
	
		if (moveImg!=0xffff)
		{	if (outFrame.isIN(MMM.x,MMM.y))
			{	//	cMSG::Put("asd","%d,%d,%d,%d-%d,%d",client.x1,client.y1,client.x2,client.y2,Mouse.x,Mouse.y);
				pos	=	GetImage();
				if (pos!=0xffff && pos!=cIMG::Current)	cIMG::Move(moveImg,pos);
				if (pos==0xffff && pos!=cIMG::Current)	cIMG::Move(moveImg,cIMG::Count);
				moveImg	=	0xffff;
				_MAIN.Draw();
			}
			else
			{	moveImg=0xffff;
				_MAIN.Draw();
			}
		}
	}

	if (oldPos.x != Mouse.x || oldPos.y != Mouse.y) // 마우스가 움직 거렸다!!
	{	if (_MAIN.workMode==dWORK_SET_SPRITE)
		{	if (keyBuff[VK_LBUTTON] & 0x80)
			{	if (moveImg == 0xffff)
				{	pos=_IMGWND.GetImage(TRUE);
					if (pos==cIMG::Current)		moveImg	=  cIMG::Current;
				}

				Draw();
			}
		}
	}

//	client.OutFrame(hWND);
	outFrame.y1-=30;
	outFrame.y2+=30;
	if (moveImg != 0xffff && !outFrame.isIN(MMM.x,MMM.y))
	{	SCROLLINFO si;

		si.cbSize= sizeof( SCROLLINFO );
		si.fMask =SIF_ALL;
		GetScrollInfo( hWND, SB_VERT, &si );
		
		if (MMM.y <=	outFrame.y1	)	if ( YPos > 0 ) YPos -= 1;
		if (MMM.y >		outFrame.y2	)	if ( YPos <= (int)(si.nMax - si.nPage) )	YPos += 1;

		if (si.nPos != YPos )
		{	si.fMask = SIF_POS;
			si.nPos  = YPos;
			SetScrollInfo( hWND, SB_VERT, &si, TRUE );
			Draw();
		}
	}

	oldPos	=	Mouse;

	return TRUE;
}

void
cIMGWND::Delete()
{	int result = cMSG::YESNO("선택된 스프라이트들을 지웁니다.","계속하시겠습니까?");

	if (result == IDNO) return;

	cIMG *img=cIMG::pHEAD->pNEXT;

	while(img!=cIMG::pTAIL)
	{	img	=	img->pNEXT;
		if (img->pPRE->isSelect)	cIMG::Del(img->pPRE);
	}

	cIMG::SetCURRENT(cIMG::Current);
	_MAIN.Draw();
}

void
cIMGWND::DeleteShadow()
{	int result = cMSG::YESNO("선택된 스프라이트의 그림자를 지웁니다.","계속하시겠습니까?");

	if (result == IDNO) return;

	cIMG *img=cIMG::pHEAD->pNEXT;

	while(img!=cIMG::pTAIL)
	{	if (img->isSelect)	img->DelShadow();
		img	=	img->pNEXT;
	}

	cIMG::SetCURRENT(cIMG::Current);
	_MAIN.Draw();
}


#define	IDM_DELETE			1000
#define	IDM_DELETE_SHADOW	1001

void
cIMGWND::PopupMenu()
{//	if (_MAIN.workMode	!=	dWORK_SET_SPRITE	&&	)	return;
	if (cIMG::Count <= 0 )	return;

	HMENU	hMenu	= CreatePopupMenu();
	cPOINT	pos;

	pos.MousePos();

	AppendMenu(hMenu,MFT_STRING,IDM_DELETE			,"선택된 이미지 지우기");
	AppendMenu(hMenu,MFT_STRING,IDM_DELETE_SHADOW	,"선택된 이미지의 그림자 지우기");

	TrackPopupMenu(hMenu,TPM_RIGHTBUTTON|TPM_TOPALIGN|TPM_LEFTALIGN,pos.x,pos.y,0,hWND,NULL);
	DestroyMenu(hMenu);
}

LRESULT CALLBACK 
cIMGWND::WNDProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{	switch(uMsg)
	{	case WM_PAINT		:
			_IMGWND.Draw();
			break;

		case WM_RBUTTONDOWN	:
			_IMGWND.PopupMenu();
			break;

		case WM_VSCROLL			:
			_IMGWND.VScroll(wParam);
			break;

		case WM_CLOSE		:
		case WM_DESTROY		:
			return (0L);

		case WM_KEYDOWN		:
			_MAIN.updateKey(wParam);
			break;

		case WM_COMMAND		:
			switch( LOWORD( wParam ) )
            {	case	IDM_DELETE			:
					_IMGWND.Delete();
					break;

				case	IDM_DELETE_SHADOW	:
					_IMGWND.DeleteShadow();
					break;
			}
			break;

		case WM_SYSCOMMAND	:
			if ((LOWORD(wParam)&0xfff0) == SC_MOVE) return FALSE;
			return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
			break;

		default :
			return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}