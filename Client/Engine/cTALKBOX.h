#ifndef _classTALK_H
#define _classTALK_H

#include "SFC.H"
#include "CText.H"
#include <vector>

using namespace std;

class	cEditBoxClippingInfo
{
public:
	cRECT	m_rect;
	BOOL	m_bIsEnable;
};

extern vector< cRECT > g_EditControlExcludeList;
extern vector< cEditBoxClippingInfo >	g_rectEditControlList;
extern COLORREF					g_EditControlTextColor;
extern COLORREF					g_EditControlBkColor;

class cEDITBOX
{
public:
	HWND			m_hWnd;
	BOOL			m_isPassword,m_bIsOnlyNumber;
	short			m_sBeginSel,m_sEndSel;
	char			m_strMessage[128];
	BOOL			m_bIsCompositionIme;
	BOOL			m_bIsHideCtrl;

					cEDITBOX();
					~cEDITBOX();


	BOOL			init(HWND hwnd,HINSTANCE hinst,DWORD menu,char *str,int x,int y,int xs,int ys,DWORD ws=0);
	void			close();

	void			getStr(char *str,int size);
	void			setStr(char *str);
	int				getCarret();
	void			reset();

	void			setSelect(int begin,int end);
	void			select();
	void			deselect();

	void			setLimit(int limit);
	void			setFocus(BOOL select=FALSE);
	BOOL			isFocus();
	void			disable();
	void			enable();

	int				getCaretPos();

};

class cINPUTBOX	:	public cEDITBOX
{
public:
	cRECT			m_rectBox;
	CPos			m_pos,m_posCorrect;

	WNDPROC			m_procOld;

	void			setPos(int _iX,int _iY);

	inline	void	set(int posx,int posy,int x1,int y1,int xs,int ys)
					{
						m_pos.x	=	posx;
						m_pos.y	=	posy;
						m_posCorrect.x	=	0;
						m_posCorrect.y	=	0;

						m_rectBox.set(x1,y1,x1+xs,y1+ys);
					}
	inline	void	setRect(int x1,int y1,int x2,int y2)
					{	
						m_rectBox.set(x1,y1,x2,y2);
					}
	inline	BOOL	isIn(int posx,int posy)
					{	
						return m_rectBox.isIn(posx+m_posCorrect.x-m_pos.x,posy+m_posCorrect.y-m_pos.y);
					}
};


#define	dMAX_EDITBOX		10

class cTALK
{
public:
	BOOL				m_isComposition;
	cINPUTBOX			*m_pEditBox;
	int					m_iCount,m_iActiveBox,m_iActivateBox;
	HWND				s_hWnd;
	HINSTANCE			s_hInst;
	BOOL				m_isActive;
	BOOL				m_isDrawCursor;
	DWORD				m_dwTextColor;

	CTextDC				*m_lpText;


						cTALK();
						~cTALK();

	BOOL				init	(HWND hwnd,HINSTANCE hinst,CTextDC *_lpText,int controlCount=1);

	inline	BOOL		isHideCtrl(int _iIndex)
	{
		if	(_iIndex >= m_iCount)
			return	TRUE;

		return m_pEditBox[_iIndex].m_bIsHideCtrl;
	}

	inline	void		setHideCtrl(int _iIndex,BOOL _bIsHide)
	{
		if	(_iIndex >= m_iCount)
			return;

		m_pEditBox[_iIndex].m_bIsHideCtrl	=	_bIsHide;
	}

	inline	int			getActivateBox(){return m_iActivateBox;}
	inline	int			getActiveBox()	{return m_iActiveBox;}
	inline	HWND		getWindowHandle(int _iIndex)
	{
		if (_iIndex >= m_iCount)	return	NULL;

		return m_pEditBox[_iIndex].m_hWnd;
	}
	void				setSelect(int _iIndex,int _iBegin,int _iEnd);
	void				setCorrectPos(int _iDx,int _iDy)
	{
		for (int i=0;i<m_iCount;i++)
			setCorrectPos(i,_iDx,_iDy);
	}
	void				setCorrectPos(int _iIndex,int _iDx,int _iDy);
	void				setPos(int _iIndex,int _iDx,int _iDy);
	void				close();
	void				reset();

	//	putX,putY 박스가 찍히는 위치 x,y 박스 안에서 텍스트가 찍히는 위치 xs,ys 너비 limit 텍스트 입력 한계
	void				set		(int putX,int puty,int index,int x,int y,int xs,int ys,int limit,char *str="",DWORD ws=0);
	void				active	(int index,BOOL select=FALSE);
	void				getStr	(int index,char *str,int strsize);
	void				setStr	(int index,char *str);

	void				draw	(DWORD color,DWORD reverseColor=0);
	inline	void		draw	()	{draw(m_dwTextColor);}
//
//	각종 컬러 설정
	void				setColor(COLORREF text, COLORREF bk);


	void				update	(int x,int y,BOOL lbutton,BOOL tab,BOOL select=TRUE);
	void				setLimit(int _iIndex,int _iSize);

	void				operateEnterKey(int _iIndex=0xffff);//엔터키 처리
	void				disable	(BOOL _isResetText=FALSE);
	void				enable	(BOOL _isResetText=FALSE);
	void				toggle	(BOOL _isResetText=FALSE);
	inline	BOOL		isEnable(){return	m_isActive;}

	static	CPos		s_posCaret;
	static	int			s_iCaretSize;
	static	BOOL		s_isInputText;

	BOOL				getCompositionState (int _iIndex );

	static	LRESULT CALLBACK	WNDProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
};

#endif