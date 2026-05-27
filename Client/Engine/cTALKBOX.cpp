#include "cTALKBOX.H"
#include "cDRAW.H"
#include "cTEXT.H"
#include "cExceptionHandler.H"

vector< cRECT > g_EditControlExcludeList;
vector< cEditBoxClippingInfo > g_rectEditControlList;
COLORREF			g_EditControlTextColor;
COLORREF			g_EditControlBkColor;

/********************     Sonaki Foundation Class	************************************
	class cEDITBOX
****************************************************************************************/

cEDITBOX::cEDITBOX()
{
	m_sBeginSel			=	0;
	m_sEndSel			=	0;
	m_hWnd				=	NULL;
	m_isPassword		=	FALSE;
	m_bIsOnlyNumber		=	FALSE;
	m_bIsCompositionIme	=	FALSE;
	m_bIsHideCtrl		=	FALSE;

	strcpy(m_strMessage,"");
}

void
cEDITBOX::reset()
{
	strcpy(m_strMessage,"");
	setStr(m_strMessage);

}	//	cEDITBOX::reset()

//
//
cEDITBOX::~cEDITBOX()
{	
	if (m_hWnd)	if (IsWindow(m_hWnd)) DestroyWindow(m_hWnd);
}

void
cEDITBOX::close()
{
	if (m_hWnd)	
		if (IsWindow(m_hWnd))
			DestroyWindow(m_hWnd);

	m_hWnd	=	NULL;
}

BOOL
cEDITBOX::init(HWND hwnd,HINSTANCE hinst,DWORD menu,char *str,int x,int y,int xs,int ys,DWORD ws)
{	
	if (m_hWnd)
		if (!IsWindow(m_hWnd))
			DestroyWindow(m_hWnd);

	reset();

	if (ws & ES_PASSWORD)
		m_isPassword	=	TRUE;
	else
		m_isPassword	=	FALSE;

	if (ws & ES_NUMBER)	
	{
		m_bIsOnlyNumber	=	TRUE;
		ws				-=	ES_NUMBER;
	}
	else m_bIsOnlyNumber=	FALSE;

	m_hWnd	=	CreateWindowEx(0, "EDIT",str, WS_CLIPCHILDREN|WS_CHILD | WS_VISIBLE|ws, x, y, xs, ys, hwnd,NULL,hinst,NULL);

	if (!m_hWnd)
		return g_eh.addStaticLog("Error in cEDITBOX::Init - edit box creat failed");

	return TRUE;
}

void
cEDITBOX::getStr(char *str,int size)
{
	if (!m_hWnd || !IsWindow(m_hWnd)) return;

	SendMessage(m_hWnd,WM_GETTEXT,size,(LPARAM)str);

	DWORD	sel	=	SendMessage(m_hWnd,EM_GETSEL,NULL,NULL);

	m_sBeginSel	=	LOWORD(sel);
	m_sEndSel	=	HIWORD(sel);
}

void
cEDITBOX::setStr(char *str)
{	if (!m_hWnd || !IsWindow(m_hWnd)) return;

	strcpy(m_strMessage,str);

	SendMessage(m_hWnd,WM_SETTEXT,0,(LPARAM)str);
	SendMessage(m_hWnd,WM_KEYDOWN,VK_END,0);
}

//
//	사용 못하게 함
void
cEDITBOX::disable()
{
	if (!m_hWnd || !IsWindow(m_hWnd)) return;

	ShowWindow(m_hWnd,SW_HIDE);
}	//	cEDITBOX::disable()

//
//	사용 할 수 있게 함
void
cEDITBOX::enable()
{
	if (!m_hWnd || !IsWindow(m_hWnd)) return;

	ShowWindow(m_hWnd,SW_SHOW);
}	//	cEDITBOX::disable()

void
cEDITBOX::setSelect(int begin,int end)
{	if (!IsWindow(m_hWnd)) return;

	SendMessage(m_hWnd,EM_SETSEL,begin,end);
}

void
cEDITBOX::select()
{	if (!IsWindow(m_hWnd)) return;

	SendMessage(m_hWnd,EM_SETSEL,0,-1);
}

void
cEDITBOX::deselect()
{	if (!IsWindow(m_hWnd)) return;

	SendMessage(m_hWnd,EM_SETSEL,-1,0);
}

void
cEDITBOX::setLimit(int limit)
{	if (!IsWindow(m_hWnd)) return;

	SendMessage(m_hWnd,EM_LIMITTEXT,limit,0);
}

void
cEDITBOX::setFocus(BOOL _isSelect)
{	
	if (!IsWindow(m_hWnd)) return;

	::SetFocus(m_hWnd);

	if (_isSelect)	select();
}

BOOL
cEDITBOX::isFocus()
{
	if (!IsWindow(m_hWnd)) return FALSE;

	if (GetFocus()==m_hWnd) return TRUE;

	return FALSE;
}

int
cEDITBOX::getCaretPos()
{	if (!IsWindow(m_hWnd)) return 0;

	POINT	pos;
	GetCaretPos(&pos);

	DWORD index = SendMessage(m_hWnd,EM_CHARFROMPOS,0,(LPARAM)MAKELONG(pos.x+1,pos.y));

	return LOWORD(index);
}

void
cINPUTBOX::setPos(int _iX,int _iY)
{
	MoveWindow(m_hWnd,_iX,_iY,m_rectBox.getWidth(),m_rectBox.getHeight(),FALSE);
}

CPos		cTALK::s_posCaret;
int			cTALK::s_iCaretSize;
BOOL		cTALK::s_isInputText	=	FALSE;

cTALK::cTALK()
{
	m_pEditBox		=	NULL;
	m_iCount		=	0;
	g_rectEditControlList.clear();
	setColor ( RGB(255,255,255), RGB(0,0,0) );
}

cTALK::~cTALK()
{	close();
}

void
cTALK::close()
{
	if	(m_iCount	==	0)
		return;

	g_rectEditControlList.clear();

	for (int i=0;i<m_iCount;i++)
		m_pEditBox[i].close();
	
	pKILL(m_pEditBox);

	s_isInputText	=	FALSE;
	m_iCount		=	0;
	m_iActiveBox	=	0;
	m_iActivateBox	=	0xffff;
}

void
cTALK::reset()
{
	m_iActivateBox	=	0xffff;

	for (int i=0;i<m_iCount;i++)	m_pEditBox[i].reset();

}	//	cTALK::reset()

BOOL
cTALK::init(HWND hwnd,HINSTANCE hinst,CTextDC *_lpText,int count)
{
	m_lpText		=	_lpText;

	close();

	m_iCount		=	count;
	s_hWnd			=	hwnd;
	s_hInst			=	hinst;

	m_pEditBox		=	new  cINPUTBOX [count];
	m_isActive		=	TRUE;
	s_isInputText	=	TRUE;
	m_iActivateBox	=	0xffff;

	return TRUE;
}

void
cTALK::set(int putX,int putY,int index,int x,int y,int xs,int ys,int limit,char *str,DWORD ws)
{
	if	(index >= m_iCount)
		return;

	cEditBoxClippingInfo	info;

	info.m_rect.set(x,y,x+xs,y+ys);
	info.m_bIsEnable	=	TRUE;

	g_rectEditControlList.push_back(info);

	m_pEditBox[index].set(putX,putY,x,y,xs,ys);
	m_pEditBox[index].init(s_hWnd,s_hInst,index+1000,str,x,y,xs,ys,ws);
	m_pEditBox[index].setLimit(limit);

	m_pEditBox[index].m_procOld	=	(WNDPROC)SetWindowLong( m_pEditBox[index].m_hWnd, GWL_WNDPROC,(LONG)WNDProc );
	SetWindowLong( m_pEditBox[index].m_hWnd, GWL_USERDATA,(LONG)&m_pEditBox[index]);

	SendMessage(m_pEditBox[index].m_hWnd,WM_SETFONT,(WPARAM)m_lpText->getFont(),0);
}

void
cTALK::setSelect(int _iIndex,int _iBegin,int _iEnd)
{
	m_pEditBox[_iIndex].setSelect(_iBegin,_iEnd);
}

void
cTALK::active(int index,BOOL select)
{
	if	(index >= m_iCount)
		return;

	if	(m_iActiveBox	==	index)
		if	(GetFocus()	==	m_pEditBox[index].m_hWnd)
			return;

	m_pEditBox[index].setFocus(select);

	m_iActiveBox	=	index;
	m_iActivateBox	=	index;
}

void
cTALK::update(int x,int y,BOOL lbutton,BOOL tab,BOOL select)
{
	if	(m_iCount	<= 0		)
		return;

	if	(m_iActiveBox>=m_iCount	)
		m_iActiveBox	=	0;

	if	(!m_isActive			)
		return;

	m_isDrawCursor	=	TRUE;
	m_iActivateBox	=	0xffff;

	if ( lbutton )
	{
		for ( int i = 0; i < m_iCount; i ++ )
		{
			if ( m_pEditBox[i].isFocus() )
			{
				int focus	=	i;

				if ( m_iActiveBox == focus )
					m_pEditBox[focus].deselect();
				else
					active(focus,select);

				enable ( );
				break;
			}
		}
	}

	if	(tab)
	{
		int focus		=	m_iActiveBox+1;
		int	iBeginFocus	=	m_iActiveBox;

		while(m_pEditBox[focus].m_bIsHideCtrl && focus != iBeginFocus)
		{
			focus++;

			if	(focus	>=	m_iCount)
				focus	=	0;
		}

		if	(m_iActiveBox	==	focus)
			m_pEditBox[focus].deselect();
		else
		{
			active(focus,select);
		}
	}
}

void
cTALK::getStr(int index,char *str,int strsize)
{
	if (index >= m_iCount)
		return;

	m_pEditBox[index].getStr(str,strsize);
}

void
cTALK::setStr(int index,char *str)
{	
	if (index >= m_iCount)
		return;

	m_pEditBox[index].setStr(str);
}

//
//	각종 컬러 설정
void
cTALK::setColor(COLORREF text, COLORREF bk )
{
	g_EditControlTextColor = text;
	g_EditControlBkColor = bk;
}

//
//	
void
cTALK::disable(BOOL _isResetText)
{	
	if (m_iActiveBox >= m_iCount || m_iActiveBox < 0)
		return;

	if (!m_isActive)
		return;

	m_isActive		=	FALSE;

	m_pEditBox[m_iActiveBox].deselect();

	if (_isResetText)
		reset();

	m_pEditBox[m_iActiveBox].disable();

	g_rectEditControlList[m_iActiveBox].m_bIsEnable	=	FALSE;
}

//
//엔터키 처리
void
cTALK::operateEnterKey(int _iIndex)
{
	if (_iIndex	==	0xffff)
		_iIndex	=	m_iActiveBox;

	m_pEditBox[_iIndex].reset();
}

//
//
void
cTALK::enable(BOOL _isResetText)
{
	if	(m_iActiveBox >= m_iCount || m_iActiveBox < 0)
		return;

	m_isActive		=	TRUE;

	active(m_iActiveBox);

	if	(_isResetText)
		reset();

	m_pEditBox[m_iActiveBox].enable();

	g_rectEditControlList[m_iActiveBox].m_bIsEnable	=	TRUE;
}

void
cTALK::setLimit(int _iIndex,int _iSize)
{
	if (_iIndex < 0 || _iIndex >= m_iCount)
		return;

	m_pEditBox[_iIndex].setLimit(_iSize);
}

void
cTALK::toggle(BOOL _isResetText)
{
	if (m_isActive)	
		disable(_isResetText);
	else			
		enable(_isResetText);
}

LRESULT CALLBACK
cTALK::WNDProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	cINPUTBOX	*lpInputBox	=	(cINPUTBOX	*)GetWindowLong(hWnd,GWL_USERDATA);

	switch( uMsg )
	{
		case	WM_CHAR		:
		{
			if (lpInputBox && IsWindow(lpInputBox->m_hWnd))
				if (lpInputBox->m_bIsOnlyNumber && wParam != 8)
					if (wParam < '0' || wParam > '9')
						return 0;

			if (wParam < 32 && wParam != 8	)
				return 0;

			break;
		}
		case	WM_RBUTTONUP:
			return 0;

		case    WM_IME_STARTCOMPOSITION:
			if (lpInputBox)
				lpInputBox->m_bIsCompositionIme	=	true;
			break;

		case    WM_IME_ENDCOMPOSITION:
			if (lpInputBox)
				lpInputBox->m_bIsCompositionIme	=	false;
			break;
	}

	if	(IsWindow(hWnd) && lpInputBox && IsWindow(lpInputBox->m_hWnd))
		return CallWindowProc((WNDPROC)lpInputBox->m_procOld,hWnd,uMsg,wParam,lParam);

	return	0;
}

void
cTALK::setPos(int _iIndex,int _iX,int _iY)
{

	m_pEditBox[_iIndex].setPos(_iX,_iY);
}

void
cTALK::setCorrectPos(int _iIndex,int _iDx,int _iDy)
{
	int	iX	=	m_pEditBox[_iIndex].m_pos.x	+_iDx;
	int	iY	=	m_pEditBox[_iIndex].m_pos.y	+_iDy;

	m_pEditBox[_iIndex].setPos(iX,iY);
}

void
cTALK::draw(DWORD color,DWORD reverseColor)
{	
	return;
/*
	if (m_iCount	<= 0)	return;
	if (!m_isActive		)	return;

	int		i;
	char	strInputString[256];

	if (m_isPutShadow)	m_lpText->pushShadowStatus(TRUE);
	else				m_lpText->pushShadowStatus(FALSE);

	m_lpText->scriptPutOff();

	for (i=0;i<m_iCount; i++)
	{
		m_pEditBox[i].getStr(strInputString,256);

		int	iDx	=	m_pEditBox[i].m_posCorrect.x;
		int	iDy	=	m_pEditBox[i].m_posCorrect.y;

		if (m_pEditBox[i].m_isPassword)
			for (int j=0;j<(int)strlen(strInputString);j++)		strInputString[j]	=	'*';

		if (i==m_iActiveBox)
			 m_lpText->putReverse(m_pEditBox[i].m_pos.x+iDx,m_pEditBox[i].m_pos.y+iDy,m_pEditBox[i].m_sBeginSel,m_pEditBox[i].m_sEndSel-1,color,strInputString);
		else m_lpText->put(m_pEditBox[i].m_pos.x+iDx,m_pEditBox[i].m_pos.y+iDy,color,strInputString);
	}

	int pos =	m_pEditBox[m_iActiveBox].getCaretPos();

	if (pos < 255)
	{
		m_pEditBox[m_iActiveBox].getStr(strInputString,256);

		strInputString[pos]	=	NULL;

		int	iDx	=	m_pEditBox[m_iActiveBox].m_posCorrect.x;
		int	iDy	=	m_pEditBox[m_iActiveBox].m_posCorrect.y;
		s_posCaret.x	=	m_pEditBox[m_iActiveBox].m_pos.x+iDx + m_lpText->getPixelSize(strInputString);;
		s_posCaret.y	=	m_pEditBox[m_iActiveBox].m_pos.y+iDy + m_lpText->m_iFontFixHeight;

		if (strInputString[pos]&0x80)
		{
			s_posCaret.x	+=	1;
			s_iCaretSize	=	m_lpText->m_iFontFixWidth*2-2;
		}
		else	s_iCaretSize	=	m_lpText->m_iFontFixWidth;
	}

	static	DWORD	dwLastBlinkTime	=	timeGetTime();
	static	BOOL	iShowCarret		=	0;

	if (m_isDrawCursor)
	{
		if (iShowCarret	>	0)	cDRAW::Fill(m_wCursorColor,s_posCaret.x,s_posCaret.y,s_posCaret.x+s_iCaretSize,s_posCaret.y+1);

		if (dwLastBlinkTime	+	500	<	timeGetTime())
		{
			iShowCarret++;
			if (iShowCarret	>=	2)	iShowCarret	=	0;

			dwLastBlinkTime	=	timeGetTime();
		}
	}

	m_isDrawCursor	=	FALSE;

	m_lpText->popShadowStatus();

	m_lpText->scriptPutOn();
	*/
}

BOOL	cTALK::getCompositionState (int _iIndex )
{
	return m_pEditBox[_iIndex].m_bIsCompositionIme;
}
