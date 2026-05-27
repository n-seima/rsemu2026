#include "bar_menu.h"
#include "cDRAW.H"

BOOL		cBarMenu::s_isExculsiveFocus	=	FALSE;
int			cBarMenu::s_iExclusiveFocusUpdate=	0;

cBar::cBar()
{	m_isChecked	=	FALSE;
	m_isClicked	=	FALSE;
	m_isFocused	=	FALSE;
	m_isEnable	=	FALSE;

	m_wNormal	=	0xffff;
	m_wActive	=	0xffff;
	m_wSelect	=	0xffff;
	m_wCheck	=	0xffff;
	m_iKey		=	0xffff;

	m_dwMenu	=	0xffffffff;

	m_posClickImageMargin.x	=	0;
	m_posClickImageMargin.y	=	0;
}

cBar::~cBar()
{
}

//
//	보이기
void
cBar::show()
{
	m_isShow	=	TRUE;	
}

//
//	감추기
void
cBar::hide()
{
	m_isShow	=	FALSE;	
}

//
//	단축키 눌렸냐?
BOOL
cBar::isShotKey(BYTE *_lpaKeyMap)
{
	if (!_lpaKeyMap		)	return	FALSE;
	if (m_iKey == 0xffff)	return	FALSE;
	if (!m_isEnable		)	return	FALSE;
	if (!m_isShow		)	return	FALSE;
	if (_lpaKeyMap[m_iKey])	return	TRUE;

	return	FALSE;
}

//
//	리셋
void
cBar::reset()
{
	m_isChecked	=	FALSE;
	m_isClicked	=	FALSE;
	m_isFocused	=	FALSE;
	m_isEnable	=	FALSE;
	m_isShow	=	FALSE;

	m_wNormal	=	0xffff;
	m_wActive	=	0xffff;
	m_wSelect	=	0xffff;
	m_wCheck	=	0xffff;
	m_iKey		=	0xffff;

	m_dwMenu	=	0xffffffff;
}

//
//	이미지 바
void
cBar::init(cSPRITE *sprite,int x,int y,DWORD menu,WORD normal,WORD active,WORD select,WORD check,WORD disable)
{
	int image	=	normal;
	
	m_wNormal	=	normal;
	m_wActive	=	active;
	m_wSelect	=	select;
	m_wCheck	=	check;
	m_dwMenu	=	menu;
	m_wDisable	=	disable;

	m_isChecked	=	FALSE;
	m_isClicked	=	FALSE;
	m_isFocused	=	FALSE;
	m_isEnable	=	TRUE;
	m_isShow	=	TRUE;

	if (image==0xffff)	image	=	m_wActive;
	if (image==0xffff)	image	=	m_wSelect;
	if (image==0xffff)	image	=	m_wCheck;

	if	(image!=0xffff && sprite)
		sprite->GetRect(image,&m_rectCrash);

	m_rectCrash.Move(x,y);
	m_pos.Set(x,y);
	m_posCorrect.Set(0,0);
	m_posClickImageMargin.Set(0,0);
	m_iKey			=	0xffff;
}

void
cBar::setSize(int xs,int ys)
{
	m_rectCrash.x2	=	m_rectCrash.x1	+	xs;	
	m_rectCrash.y2	=	m_rectCrash.y1	+	ys;	
}

void
cBar::setSize(int x,int y,int xs,int ys)
{
	m_rectCrash.x1	=	x;	
	m_rectCrash.y1	=	y;	
	m_rectCrash.x2	=	m_rectCrash.x1	+	xs;	
	m_rectCrash.y2	=	m_rectCrash.y1	+	ys;	
}

//
//	이미지바 그리기
void
cBar::drawImageBar(cBarMenu *_lpBarMenu)
{
	if	(!m_isShow)
		return;

	BOOL	bIsHalfBlending	=	_lpBarMenu->m_bIsHalfblending;

	if	(!_lpBarMenu->m_lpSprite)
		return;

	int		wImage	=	0xffff;
	cPOINT	posImage;

	posImage.x	=	m_posCorrect.x+m_pos.x;
	posImage.y	=	m_posCorrect.y+m_pos.y;

	if (m_isEnable)
	{
		wImage	=	m_wNormal;

		if	(m_isFocused	&&	m_wActive!=0xffff)
			wImage=m_wActive;

		if	(m_isClicked	&&	m_wSelect!=0xffff)
		{
			posImage.x	+=	m_posClickImageMargin.x;
			posImage.y	+=	m_posClickImageMargin.y;
			wImage		=	m_wSelect;
		}
	}
	else
	if	(m_wDisable!=0xffff)
		wImage	=	m_wDisable;

	if	(bIsHalfBlending)
	{
		if	(wImage		!=	0xffff				)
			_lpBarMenu->m_lpSprite->Put(posImage.x,posImage.y,wImage  ,100,100,dPUT_HALF_BLENDING);
		if	(m_isChecked&&	m_wCheck !=0xffff	)
			_lpBarMenu->m_lpSprite->Put(posImage.x,posImage.y,m_wCheck,100,100,dPUT_HALF_BLENDING);
	}
	else
	{
		if	(wImage		!=	0xffff				)
			_lpBarMenu->m_lpSprite->Put(posImage.x,posImage.y,wImage);
		if	(m_isChecked&&	m_wCheck !=0xffff	)
			_lpBarMenu->m_lpSprite->Put(posImage.x,posImage.y,m_wCheck);
	}
}


BOOL
cBar::isFocus(int x,int y,BOOL _bIsIgnoreBarStatus)
{
	if (!_bIsIgnoreBarStatus)
		if (!m_isEnable || !m_isShow)
			return FALSE;

	x	-=	m_posCorrect.x;
	y	-=	m_posCorrect.y;

	return m_rectCrash.isIN(x,y);
}

void
cBar::update(BOOL focus,BOOL click)
{
	m_isClicked	=	click;
	m_isFocused	=	focus;
}

void
cBar::move(int x,int y)
{
	if (x==0xffff) x = m_rectCrash.x1;
	if (y==0xffff) y = m_rectCrash.y1;

	m_rectCrash.x2	=	(m_rectCrash.x2	-	m_rectCrash.x1)	+	x;
	m_rectCrash.y2	=	(m_rectCrash.y2	-	m_rectCrash.y1)	+	y;
	m_rectCrash.x1	=	x;
	m_rectCrash.y1	=	y;
}

void
cBar::move(int x,int y,int xs,int ys)
{
	if (x==0xffff) x = m_rectCrash.x1;
	if (y==0xffff) y = m_rectCrash.y1;

	m_rectCrash.x2	=	x+xs;;
	m_rectCrash.y2	=	y+ys;
	m_rectCrash.x1	=	x;
	m_rectCrash.y1	=	y;
}

void
cBar::resize(int xs,int ys)
{
	m_rectCrash.x2	=	m_rectCrash.x1+xs;
	m_rectCrash.y2	=	m_rectCrash.y1+ys;
}

/****************************************************************
	Barm_dwMenu class
****************************************************************/

cBarMenu::cBarMenu()
{	
	m_wCount			=	0;
	m_wFocus			=	0xffff;
	m_wMaxBar			=	0;
	m_lpSprite			=	NULL;
	m_pMenu				=	NULL;
	m_isFocusOwner		=	FALSE;
	m_bIsStrongCheck	=	FALSE;
}

cBarMenu::~cBarMenu()
{	
	close();
}

void
cBarMenu::close()
{	
	pKILL(m_pMenu);
	reset();
}

void
cBarMenu::init(cSPRITE *image,int maxbar,WORD checkmode)
{
	pKILL(m_pMenu);

	m_lpSprite				=	image;
	m_wMaxBar				=	maxbar;
	m_wCheckMode			=	checkmode;
	m_wCount				=	0;
	m_pMenu					=	new cBar [maxbar];
	m_isFocusOwner			=	FALSE;
	m_dwClickMenu			=	0xffffffff;
}

void
cBarMenu::reset()
{
	m_wCount		=	0;
	m_wFocus		=	0xffff;
	m_isFocusOwner	=	FALSE;
	m_dwClickMenu	=	0xffffffff;
}

//
//	위치 교정
void
cBarMenu::correctPos(DWORD _dwMenu,int _iX,int _iY)
{
	if	(_dwMenu	==	0xffffffff)
	{
		for (int i=0;i<m_wMaxBar;i++)
		{
			m_pMenu[i].m_pos.x	=	_iX;
			m_pMenu[i].m_pos.y	=	_iY;
			m_pMenu[i].m_rectCrash.MoveTo(_iX,_iY);
		}

		return;
	}

	int iIndex	=	getIndex(_dwMenu);

	if (iIndex	==	0xffff)
		return;

	m_pMenu[iIndex].m_pos.x	=	_iX;
	m_pMenu[iIndex].m_pos.y	=	_iY;
	m_pMenu[iIndex].m_rectCrash.MoveTo(_iX,_iY);
}

void
cBarMenu::setCorrectPos(int _iDx,int _iDy)
{
	if	(!m_pMenu)
		return;

	for (int i=0;i<m_wMaxBar;i++)
	{
		m_pMenu[i].m_posCorrect.x	=	_iDx;
		m_pMenu[i].m_posCorrect.y	=	_iDy;
	}
}

void
cBarMenu::setCorrectPos(DWORD _dwMenu,int _iDx,int _iDy)
{
	if	(!m_pMenu)
		return;

	int iIndex	=	getIndex(_dwMenu);

	if	(iIndex	==	0xffff)
		return;

	m_pMenu[iIndex].m_posCorrect.x	=	_iDx;
	m_pMenu[iIndex].m_posCorrect.y	=	_iDy;
}

//
//	이미지바 추가
BOOL
cBarMenu::addImageBar(DWORD menu,int x,int y,WORD normal,WORD active,WORD select,WORD check,WORD disable)
{
	if	(m_wCount >= m_wMaxBar)
		return FALSE;

	m_pMenu[m_wCount].init(m_lpSprite,x,y,menu,normal,active,select,check,disable);

	m_wCount++;

	return TRUE;
}

//
//	이미지바 재설정
BOOL
cBarMenu::resetImageBar(DWORD destMenu,DWORD menu,int x,int y,WORD normal,WORD active,WORD select,WORD check,WORD disable)
{
	int	iIndex	=	getIndex(destMenu);

	if (iIndex	==	0xffff)	return	FALSE;

	m_pMenu[iIndex].init(m_lpSprite,x,y,menu,normal,active,select,check,disable);

	return	TRUE;
}

//
//	이미지바 설정
BOOL
cBarMenu::setImageBar(DWORD destMenu,WORD normal,WORD active,WORD select,WORD check,WORD disable)
{
	int	iIndex	=	getIndex(destMenu);

	if (iIndex	==	0xffff)	return	FALSE;

	m_pMenu[iIndex].m_wNormal	=	normal;
	m_pMenu[iIndex].m_wActive	=	active;
	m_pMenu[iIndex].m_wSelect	=	select;
	m_pMenu[iIndex].m_wCheck	=	check;
	m_pMenu[iIndex].m_wDisable	=	disable;

	return	TRUE;
}


void
cBarMenu::setSize(DWORD menu,int xs,int ys)
{
	int index	=	getIndex(menu);

	if (index == 0xffff)	return;

	m_pMenu[index].setSize(xs,ys);
}

void
cBarMenu::setSize(int xs,int ys)
{
	m_pMenu[m_wCount-1].setSize(xs,ys);
}

void
cBarMenu::setSize(int x,int y,int xs,int ys)
{
	m_pMenu[m_wCount-1].setSize(x,y,xs,ys);
}

void
cBarMenu::setSize(DWORD menu,int x,int y,int xs,int ys)
{
	int index	=	getIndex(menu);

	if (index == 0xffff)	return;

	m_pMenu[index].setSize(x,y,xs,ys);
}

void
cBarMenu::setMenuId(int _iIndex,DWORD _dwMenu)
{
	if (_iIndex	>=	m_wCount)	return;

	m_pMenu[_iIndex].m_dwMenu	=	_dwMenu;
}


//
//	체커 전부 지우기
void
cBarMenu::resetChecker()
{
	for (int i=0;i<m_wCount;i++)	m_pMenu[i].m_isChecked	=	FALSE;
		
}	//	cBarMenu::resetChecker()

//
//
BOOL
cBarMenu::check(DWORD menu,WORD check)
{	int i,index	=	0xffff;

	for (i=0;i<m_wCount;i++)
		if (m_pMenu[i].m_dwMenu	==	menu)
		{	
			index	=	i;

			if (m_pMenu[i].m_isEnable	==	FALSE)	return	FALSE;

			break;
		}

	if	(index == 0xffff)
		return FALSE;

	switch(m_wCheckMode)
	{	
		case	dBAR_RADIO	:
			for (i=0;i<m_wCount;i++)
				m_pMenu[i].m_isChecked	=	FALSE;

			if	(check==0xffff)
				m_pMenu[index].m_isChecked	=	TRUE;
			else
				m_pMenu[index].m_isChecked	=	(BOOL)check;
			return	TRUE;

		case	dBAR_CHECK	:
			if	(check==0xffff)	
				m_pMenu[index].m_isChecked	=	1	-	m_pMenu[index].m_isChecked;
			else
				m_pMenu[index].m_isChecked	=	(BOOL)check;

			return	TRUE;
	}

	return	FALSE;
}

//
//
BOOL
cBarMenu::checkRadio(DWORD _dwCheckMenu,BOOL _isCheck,DWORD _dwBegin,DWORD _dwEnd)
{
	DWORD	dwMenu;

	for (dwMenu=_dwBegin;dwMenu<=_dwEnd;dwMenu++)	
		check(dwMenu,FALSE);

	return	check(_dwCheckMenu,TRUE);
}

//
//	바 메뉴 업데이트
DWORD
cBarMenu::update(int x,int y,BYTE *_lpaKeyMap,BOOL _isLeftTouch,BOOL _isLeftPressed,BOOL checkNow)
{
	int		i;
	DWORD	result		=	0xffffffff;
	BOOL	isGetFocus	=	FALSE;

	m_bIsActionedByHotKey=	FALSE;

	for (i=0;i<m_wCount;i++)
		if (m_pMenu[i].isShotKey(_lpaKeyMap))
		{
			m_bIsActionedByHotKey	=	TRUE;
			return	m_pMenu[i].m_dwMenu;
		}

	if (!_isLeftTouch && _isLeftPressed && !s_isExculsiveFocus)
		return	0xffffffff;

	if (s_isExculsiveFocus)
	{
		if (!m_isFocusOwner)
		{
			if (m_iExclusiveFocusUpdate	==	s_iExclusiveFocusUpdate)
			{
				m_iValidUpdate++;

				if (m_iValidUpdate	>	2)
				{	s_isExculsiveFocus	=	FALSE;
					s_iExclusiveFocusUpdate	=	0;
					m_iValidUpdate			=	0;
					m_iExclusiveFocusUpdate	=	0;
				}
			}
			else
			{
				m_iExclusiveFocusUpdate	=	s_iExclusiveFocusUpdate;
				m_iValidUpdate			=	0;
			}

			return	0xffffffff;
		}
		else	s_iExclusiveFocusUpdate++;
	}
	else
	{
		s_iExclusiveFocusUpdate	=	0;
		m_iValidUpdate			=	0;
		m_iExclusiveFocusUpdate	=	0;
	}


	for (i=0;i<m_wCount;i++)
	{
		if (m_pMenu[i].isShotKey(_lpaKeyMap))
			return	m_pMenu[i].m_dwMenu;
			
		if (m_pMenu[i].isFocus(x,y))			//	마우스가 이 버튼 위에 있다.
		{
			isGetFocus	=	TRUE;

			if (m_dwClickMenu	==	0xffffffff)		//	누른적 없다.
			{
				m_isFocusOwner		=	TRUE;
				s_isExculsiveFocus	=	TRUE;
				m_wFocus			=	i;		//	포커스를 준다.

				m_pMenu[i].update(TRUE,_isLeftPressed);	//	버튼에 포커스를 준다.

				if (_isLeftTouch)				//	버튼이 눌러 졌다.
				{
					m_posPressed.x	=	x;
					m_posPressed.y	=	y;
					m_dwClickMenu	=	m_pMenu[i].m_dwMenu;	//	설정
				}
			}
			else								//	먼가 눌러졌다.
			{
				if (!_isLeftPressed)						//	버튼을 뗏을때만 동작
				{
					if (m_pMenu[i].m_dwMenu	==	m_dwClickMenu)	//	마우스 위치가 버튼 위이다.
					{
						BOOL	bIsOk	=	TRUE;

						if (m_bIsStrongCheck)
							if (isClickedPos(x,y))
								bIsOk	=	FALSE;

						if	(bIsOk)
						{
							result	=	m_pMenu[i].m_dwMenu;		//	결과

							if (checkNow)
								check(result);			//	체크하는거면 체크
						}
					}

					m_dwClickMenu	=	0xffffffff;

					m_pMenu[i].update(TRUE,FALSE);				//	버튼에 포커스를 준다.
				}
			}
		}
		else
		{
			if (m_pMenu[i].m_dwMenu	!=	m_dwClickMenu)	m_pMenu[i].update(FALSE,FALSE);
		}
	}

	if (!_isLeftPressed)	m_dwClickMenu	=	0xffffffff;

	if (m_isFocusOwner)
	{
		if (!isGetFocus	&&	m_dwClickMenu	==	0xffffffff)
		{
			m_isFocusOwner		=	FALSE;
			s_isExculsiveFocus	=	FALSE;
		}
	}

	return result;
}

//
//	_iX,_iY의 위치에 메뉴
cBar*
cBarMenu::getFocusMenu(int _iX,int _iY)
{
	int		i;

	for (i=0;i<m_wCount;i++)
	{
		if (m_pMenu[i].isFocus(_iX,_iY,TRUE))			//	마우스가 이 버튼 위에 있다.
			return	&m_pMenu[i];
	}

	return NULL;
}

//
//	누르기 업데이트
DWORD
cBarMenu::updatePressed(int x,int y,BYTE *_lpaKeyMap,BOOL _isLeftPressed)
{	
	int		i;
	DWORD	result	=	0xffffffff;

	for (i=0;i<m_wCount;i++)
	{
		if (m_pMenu[i].m_iKey	!=	0xffff)
			if (_lpaKeyMap[m_pMenu[i].m_iKey])	return	m_pMenu[i].m_dwMenu;

		if (m_pMenu[i].isFocus(x,y))			//	마우스가 이 버튼 위에 있다.
		{
			m_wFocus	=	i;				//	포커스를 준다.
			m_pMenu[i].update(TRUE,_isLeftPressed);	//	버튼에 포커스를 준다.

			if (_isLeftPressed)						//	버튼이 눌러 졌다.
				result	=	m_pMenu[i].m_dwMenu;	//	설정
		}
		else	m_pMenu[i].update(FALSE,FALSE);
	}

	return result;
}

//
//	x,y에 버튼 찾기
DWORD
cBarMenu::getMenu(int x,int y)
{	
	int		i;

	for (i=0;i<m_wCount;i++)
		if (m_pMenu[i].isFocus(x,y))
			return m_pMenu[i].m_dwMenu;

	return 0xffffffff;
}

//
//	menu가 몇번째 바?
int
cBarMenu::getIndex(DWORD menu)
{
	int i;

	for (i=0;i<m_wCount;i++)
		if (m_pMenu[i].m_dwMenu	==	menu)	return i;

	return 0xffff;
}

//
//menu항목의 둘레
BOOL
cBarMenu::getRect(DWORD _dwMenu,cRECT *_lpRect)
{
	int	iIndex	=	getIndex(_dwMenu);

	if (iIndex	==	0xffff)	return	FALSE;

	memcpy(_lpRect,&m_pMenu[iIndex].m_rectCrash,sizeof(cRECT));

	return	TRUE;
}

//
//	그려 주세요.. *-_-*
void
cBarMenu::draw()
{	
	if	(!m_lpSprite)
		return;

	for (int i=0;i<m_wCount;i++)
		m_pMenu[i].drawImageBar(this);
}


//
//	menu 항목이 체크된거냐?
BOOL
cBarMenu::isCheck(DWORD menu)
{	
	int i;

	for (i=0;i<m_wCount;i++)
		if (m_pMenu[i].m_dwMenu==menu)
			if (m_pMenu[i].m_isChecked)	return TRUE;

	return FALSE;
}

//
//	체크된 메뉴를 찾아서 리턴~ 한다.
int
cBarMenu::getCheckedMenu()
{
	for (int i=0;i<m_wCount;i++)
		if (m_pMenu[i].m_isEnable)
			if (m_pMenu[i].m_isChecked)
				return m_pMenu[i].m_dwMenu;

	return	-1;
}

BOOL
cBarMenu::isActive(DWORD menu)
{	
	int i;

	for (i=0;i<m_wCount;i++)
		if (m_pMenu[i].m_dwMenu==menu)
			if (m_pMenu[i].m_isFocused)	return TRUE;

	return FALSE;
}

BOOL
cBarMenu::move(DWORD menu,int x,int y)
{
	int index = getIndex(menu);

	if (index == 0xffff) return FALSE;

	m_pMenu[index].move(x,y);

	return TRUE;
}

BOOL
cBarMenu::move(DWORD menu,int x,int y,int xs,int ys)
{	
	int index = getIndex(menu);

	if (index == 0xffff) return FALSE;

	m_pMenu[index].move(x,y,xs,ys);

	return TRUE;	
}

BOOL
cBarMenu::resize(DWORD menu,int xs,int ys)
{
	int index = getIndex(menu);

	if (index == 0xffff) return FALSE;

	m_pMenu[index].resize(xs,ys);

	return TRUE;
}

void
cBarMenu::enable(DWORD menu)
{
	int index = getIndex(menu);
	if (index==0xffff) return;

	m_pMenu[index].m_isEnable	=	TRUE;
}

void
cBarMenu::disable(DWORD menu)
{
	int index = getIndex(menu);
	if (index==0xffff) return;

	m_pMenu[index].m_isEnable	=	FALSE;
}

BOOL
cBarMenu::isEnable(DWORD menu)
{
	int index = getIndex(menu);
	if (index==0xffff) return FALSE;

	return m_pMenu[index].m_isEnable;
}

//
//	보이게
void
cBarMenu::show(DWORD _dwMenu)
{
	int index = getIndex(_dwMenu);

	if (index==0xffff) return;

	m_pMenu[index].show();
}	//	cBarMenu::show(DWORD _dwMenu)

//
//	안 보이게
void
cBarMenu::hide(DWORD _dwMenu)
{
	int index = getIndex(_dwMenu);

	if (index==0xffff) return;

	m_pMenu[index].hide();
}	//	cBarMenu::hide(DWORD _dwMenu)

//
//	보이냐?
BOOL
cBarMenu::isShow(DWORD _dwMenu)
{	int index = getIndex(_dwMenu);

	if (index==0xffff) return FALSE;

	return m_pMenu[index].m_isShow;
}	//	cBarMenu::isShow(DWORD _dwMenu)

//
//특정 메뉴에 키설정
void
cBarMenu::setKey(DWORD _dwMenu,int _iKey)
{
	int	iIndex	=	getIndex(_dwMenu);

	if (iIndex	==	0xffff)	return;

	m_pMenu[iIndex].m_iKey	=	_iKey;
}

//
//	포커스된 메뉴냐?
BOOL
cBarMenu::isFocusedMenu(DWORD _dwMenu)
{
	int	iIndex	=	getIndex(_dwMenu);

	if (iIndex	==	0xffff)
		return	FALSE;

	if (m_pMenu[iIndex].m_isFocused)
		return	TRUE;

	return	FALSE;
}

//
//	포커스된 메뉴의 툴팁
cBar*
cBarMenu::getFocusedMenu()
{
	for (int i=0;i<m_wCount;i++)
		if (m_pMenu[i].m_isFocused)
			return	&m_pMenu[i];

	return	NULL;
}

//
//	특정 버튼의 클릭 마진 설정
void
cBarMenu::setClickImageMargin(DWORD _dwMenu,int _iDx,int _iDy)
{
	int	iIndex	=	getIndex(_dwMenu);

	if (iIndex	==	0xffff)	return;

	m_pMenu[iIndex].m_posClickImageMargin.Set(_iDx,_iDy);
}

//
//	모든 버튼의 클릭 마진 설정
void
cBarMenu::setClickImageAllBarMargin(int _iDx,int _iDy)
{
	for (int i=0;i<m_wCount;i++)	m_pMenu[i].m_posClickImageMargin.Set(_iDx,_iDy);
}

//
//	특정 버튼의 클릭 마진 설정
void
cBarMenu::setClickMargin(DWORD _dwMenu,int _iDx,int _iDy)
{
	setClickImageMargin(_dwMenu,_iDx,_iDy);
}

//
//	모든 버튼의 클릭 마진 설정
void
cBarMenu::setClicktAllBarMargin(int _iDx,int _iDy)
{
	setClickImageAllBarMargin(_iDx,_iDy);
}