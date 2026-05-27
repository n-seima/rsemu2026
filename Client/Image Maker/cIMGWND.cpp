#include "cMAIN.H"
#include "cIMGWND.H"
#include "cDRAW.H"
#include "cANM.H"
#include "cTEXT.H"

cIMGWND	_IMGWND;

cPOINT	margin;

cIMGWND::cIMGWND()
        :cDIBWND()
{	YPos				=	0;
	Rate				=	100;
	moveImg				=	0xffff;
	m_pClipImage		=	NULL;
	m_iClipImageCount	=	0;
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
{
	FirstImg	=	ImageFolder[CurrentImageFolder];
	LastImg		=	ImageFolder[CurrentImageFolder+1];
	ImgCount	=	LastImg	-	FirstImg;

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

	ImgPerCol  = (height() - (8+24) )/ imgYS;

	if (ImgPerCol < _cfg.imgMinRow	)
	{	ImgPerCol	=	_cfg.imgMinRow;
		imgYS		=	(max(height()-(8+24)-5*_cfg.imgMinRow,0))/_cfg.imgMinRow + 5;
		Rate		=	imgYS*100/(cIMG::BigYS + 5);
		imgXS		=	(cIMG::BigXS + 5)*Rate/100;
		ImgPerLine	=	(max(width() -8,0))/ imgXS;
	}

	imgXP	=	(width ()-(ImgPerLine*imgXS))/2;
	imgYP	=	(height()-(ImgPerCol *imgYS))/2+12;

	YPos	=	(ImgCount-1)/ImgPerLine - ImgPerCol + 1;
}

void
cIMGWND::Draw()
{	SCROLLINFO si;

	ImageFolder[0]		=	0;
	ImageFolder[1]		=	cIMG::Count;
	ImageFolderCount	=	1;

	FirstImg	=	ImageFolder[CurrentImageFolder];
	LastImg		=	ImageFolder[CurrentImageFolder+1];
	ImgCount	=	LastImg	-	FirstImg;

	if (ImgCount <= 0 || width() < 30) 
	{
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

	int	i,j;

	imgXS=width()-8;
	imgYS=20;
	Rate =100;

	ImgPerLine = (width()  - 8)/ imgXS;
	ImgPerCol  = (height() - (8+24) )/ imgYS;

	imgXP	=	(width ()-(ImgPerLine*imgXS))/2;
	imgYP	=	(height()-(ImgPerCol *imgYS))/2+12;

/*
	if (_MAIN.workMode == dWORK_SET_SPRITE)
	{	if (YPos*ImgPerLine > cIMG::Current-1) YPos=(cIMG::Current-1)/ImgPerLine;
		if (YPos*ImgPerLine + ImgPerLine*ImgPerCol <= cIMG::Current )
			YPos=max(cIMG::Current/ImgPerLine-(ImgPerCol -1),0);
	}
*/
	if (ImgCount > ImgPerCol * ImgPerLine)
	{	si.cbSize	=	sizeof(SCROLLINFO);
		si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;
		si.nMin		=	0;
		si.nMax		=	(ImgCount-1)/ImgPerLine;
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

	int		count		=	YPos*ImgPerLine+FirstImg,xp,yp;
	cIMG	*img		=	cIMG::Get(count);
	cIMG	*current	=	cIMG::Get(cIMG::Current);

	if (img)
	{
		cDRAW::Fill(_GREEN,2,2, width()-2,18);

		cTEXT::Put(8,4,_WHITE,current->m_strName);

		for (j=0;j<ImgPerCol ;j++)	
			for (i=0;i<ImgPerLine;i++,count++)
			{	if (count >= cIMG::Count) break;
				if (count >= LastImg) break;

				xp = imgXP+i*imgXS+imgXS/2-img->xs*Rate/200+img->xp*Rate/100;
				yp = imgYP+j*imgYS+imgYS/2-img->ys*Rate/200+img->yp*Rate/100;
				int	bxp	=imgXP+i*imgXS,byp=imgYP+j*imgYS;

				if (count == cIMG::Current)						cDRAW::Fill	(_LTBLUE	,bxp,byp,bxp+imgXS-2,byp+imgYS-2);
				else
				{		 if (img->isSelect)						cDRAW::Fill	(_GREEN		,bxp,byp,bxp+imgXS-2,byp+imgYS-2);
					else if (_MAIN.workMode==dWORK_SET_SPRITE)	cDRAW::Fill	(0			,bxp,byp,bxp+imgXS-2,byp+imgYS-2);
					else										cDRAW::Fill	(_DEEPGRAY	,bxp,byp,bxp+imgXS-2,byp+imgYS-2);
				}

				cTEXT::Put(bxp+32,byp+2,_WHITE,img->m_strName);

				if (count == cIMG::Current)	cDRAW::Box	(_LTGREEN,bxp,byp,bxp+imgXS-2,byp+imgYS-2);
				else						cDRAW::Box	(0x7fff	,bxp,byp,bxp+imgXS-2,byp+imgYS-2);

				if (_cfg.bPUTINDEX)
				{	int x=imgXP+i*imgXS+2,y=imgYP+j*imgYS+2;
					int size = strlen(_ms("%d",count));

					cDRAW::Fill(_GRAY,x,y,x+4+6*size,y+16);
					cTEXT::Put (x+2,y+3,RGBmix(31,63,31),_ms("%d",count),32);
				}

				img=img->pNEXT;
			}
	}

	if (moveImg!=0xffff)
	{	cDRAW::FillHB(_LTBLUE,Mouse.x- margin.x,Mouse.y- margin.y,Mouse.x- margin.x+imgXS-2,Mouse.y+imgYS-2- margin.y);
		img=cIMG::Get(moveImg);

		if (img) 
		{	xp = Mouse.x+imgXS/2-img->xs*Rate/200+img->xp*Rate/100 - margin.x;
			yp = Mouse.y+imgYS/2-img->ys*Rate/200+img->yp*Rate/100 - margin.y;

//			PutSpriteR[dPUT_HALFBLENDING][cANM::isFlip()](xp,yp,img->pSPRITE,Rate,Rate,32);
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
{	if (ImgCount <= 0 ) return 0xffff;
	if (!client.isIN(Mouse.x,Mouse.y)) return 0xffff;

	int	count	=	YPos*ImgPerLine+FirstImg,i,j;

	for (j=0;j<ImgPerCol ;j++)
		for (i=0;i<ImgPerLine;i++,count++)
		{	if (count >= cIMG::Count) return 0xffff;
			if (count >= LastImg) return 0xffff;
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
		{	if (_MAIN.workMode==dWORK_SET_SPRITE)
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
				if (pos==0xffff && pos!=cIMG::Current)	cIMG::Move(moveImg,LastImg);
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
cIMGWND::Cut()
{
	int		iCount	=	0;
	cIMG	*lpImg	=	cIMG::pHEAD->pNEXT;

	while(lpImg		!=	cIMG::pTAIL)
	{	
		if (lpImg->isSelect)	iCount++;
		lpImg		=	lpImg->pNEXT;
	}

	pKILL(m_pClipImage);

	m_pClipImage	=	new	cIMG	[iCount];

	lpImg			=	cIMG::pHEAD->pNEXT;
	iCount			=	0;

	while(lpImg	!=	cIMG::pTAIL)
	{
		if (lpImg->isSelect)
		{
			m_pClipImage[iCount].Copy(lpImg);

			iCount++;
		}

		lpImg	=	lpImg->pNEXT;
	}

	m_iClipImageCount	=	iCount;

	Delete(TRUE);
}

void
cIMGWND::Paste()
{
	if (m_iClipImageCount	==	0)	return;

	if (m_iPopPos	==	0xffff)	return;

	cIMG	*lpImage	=	cIMG::Get(m_iPopPos);

	for (int i=0;i<m_iClipImageCount;i++)
	{
		cIMG	*pImg;

		pImg	=	new cIMG();

		pImg->Copy(&m_pClipImage[i]);

		pImg->pPRE				=	lpImage->pPRE;
		pImg->pNEXT				=	lpImage;
		lpImage->pPRE->pNEXT	=	pImg;
		lpImage->pPRE			=	pImg;
	}

	cIMG::Count			+=	m_iClipImageCount;

	pKILL(m_pClipImage);
	m_iClipImageCount	=	0;

	Draw();
}

void
cIMGWND::Delete(BOOL quick)
{
	int		result;

	if (!quick)
	{	result = cMSG::YESNO("선택된 스프라이트들을 지웁니다.","계속하시겠습니까?");
		if (result == IDNO) return;
	}

	if (ImageFolderCount > 1)
	{
		if (!quick)
		{
			result = cMSG::YESNO("선택된 스프라이트들을 지웁니다.","스프라이트를 지우게 되면 폴더정렬이 사라지게 됩니다.\n\n계속하시겠습니까?");

			if (result == IDNO) return;	
		}

		ImageFolder[0]		=	0;
		ImageFolder[1]		=	cIMG::Count;
		ImageFolderCount	=	1;
		CurrentImageFolder	=	0;
		cSTRING	*str		=	imageFOLDERS->Pick(0);
		str->Set("폴더 없음");
	}

	cIMG *img=cIMG::pHEAD->pNEXT;

	while(img!=cIMG::pTAIL)
	{	img	=	img->pNEXT;
		if (img->pPRE->isSelect)	cIMG::Del(img->pPRE);
	}

	cIMG::SetCURRENT(cIMG::Current);
	_MAIN.Draw();
}

void
cIMGWND::DeleteShadow(BOOL all)
{
}


#define	IDM_DELETE				1000
#define	IDM_DELETE_SHADOW		1001
#define	IDM_DELETE_ALL_SHADOW	1002
#define	IDM_IMAGE_MERGE			1003
#define	IDM_CUT					1004
#define	IDM_PASTE				1005
#define	IDM_IMAGE_FOLDER		1006

void
cIMGWND::PopupMenu()
{//	if (_MAIN.workMode	!=	dWORK_SET_SPRITE	&&	)	return;
	if (cIMG::Count <= 0 )	return;

	HMENU	hMenu	= CreatePopupMenu();
	cPOINT	pos;

	pos.MousePos();

	Mouse.MousePos(hWND);
	m_iPopPos	=	GetImage();

	AppendMenu(hMenu,MFT_STRING,IDM_DELETE				,"선택된 이미지 지우기");
	AppendMenu(hMenu,MFT_STRING,IDM_CUT					,"선택된 이미지 잘라내기");

	if (m_iClipImageCount	>	0)	AppendMenu(hMenu,MFT_STRING				,IDM_PASTE				,"붙여넣기");
	else							AppendMenu(hMenu,MFT_STRING|MFS_GRAYED	,IDM_PASTE				,"붙여넣기");

	AppendMenu(hMenu,MFT_SEPARATOR,0,NULL);

	AppendMenu(hMenu,MFT_STRING,IDM_DELETE_SHADOW		,"선택된 이미지의 그림자 지우기");
	AppendMenu(hMenu,MFT_STRING,IDM_DELETE_ALL_SHADOW	,"모든 그림자 지우기");

	if (ImageFolderCount > 1)
	{
		AppendMenu(hMenu,MFT_SEPARATOR,0,NULL);

		for (int i=0;i<ImageFolderCount;i++)
			AppendMenu(hMenu,MFT_STRING,IDM_IMAGE_FOLDER+i,imageFOLDERS->GetStr(i));

		AppendMenu(hMenu,MFT_SEPARATOR,0,NULL);
		AppendMenu(hMenu,MFT_STRING,IDM_IMAGE_MERGE,"모든 이미지");
	}

	TrackPopupMenu(hMenu,TPM_RIGHTBUTTON|TPM_TOPALIGN|TPM_LEFTALIGN,pos.x,pos.y,0,hWND,NULL);
	DestroyMenu(hMenu);
}

LRESULT CALLBACK 
cIMGWND::WNDProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{	switch(uMsg)
	{
	case WM_PAINT		:
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

				case	IDM_CUT				:
					_IMGWND.Cut();
					break;

				case	IDM_PASTE			:
					_IMGWND.Paste();
					break;

				case	IDM_DELETE_ALL_SHADOW	:
					_IMGWND.DeleteShadow(TRUE);
					break;

				case	IDM_IMAGE_MERGE		:
				{	CurrentImageFolder	=	ImageFolderCount+1;
					ImageFolder[ImageFolderCount+1]	=	0;
					ImageFolder[ImageFolderCount+2]	=	cIMG::Count;
					_IMGWND.Draw();
					break;
				}

				default						:
				{	int index	=	LOWORD( wParam )-IDM_IMAGE_FOLDER;
					
					if (index >= 0 && index < ImageFolderCount)
					{	CurrentImageFolder	=	index;
						_IMGWND.Draw();
					}
					break;
				}
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