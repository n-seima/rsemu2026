#ifndef _classBARMENU_H
#define _classBARMENU_H

#include "SFC.H"
#include "cIMAGE.H"

#define		dMAX_MENUBAR			100

#define		dBAR_BUTTON				0
#define		dBAR_RADIO				1
#define		dBAR_CHECK				2

class	cBarMenu;

class cBar
{
public:
	WORD				m_wNormal,m_wActive,m_wSelect,m_wCheck,m_wDisable;
	DWORD				m_dwMenu;
	int					m_iKey;		//	이 키가 눌러지면 이 녀석이 반응 한다.
	cRECT				m_rectCrash;
	cPOINT				m_pos;
	cPOINT				m_posClickImageMargin,m_posCorrect;
	BYTE				m_isChecked,m_isClicked,m_isFocused,m_isEnable,m_isShow;

						cBar();
						~cBar();

	void				reset();//	리셋
	void				init(cSPRITE *sprite,int x,int y,DWORD menu,WORD normal,WORD active=0xffff,WORD select=0xffff,WORD check=0xffff,WORD disable=0xffff);

	void				drawImageBar(cBarMenu *_lpBarMenu);

	void				setSize(int xs,int ys);
	void				setSize(int x,int y,int xs,int ys);
	BOOL				isFocus(int x,int y,BOOL _bIsIgnoreBarStatus=FALSE);
	void				move(int x,int y);
	void				move(int x,int y,int xs,int ys);
	void				resize(int xs,int ys);
	void				update(BOOL focus,BOOL click);
	void				show();	//	보이게 하기
	void				hide();	//	감추기
	BOOL				isShotKey(BYTE *_lpaKeyMap);//	단축키 눌렸냐?
};

//
//	Barmenu Text Output Method
enum
{
	eBTOM_CENTER,
	eBTOM_BAR_RIGHT,
	eBTOM_BAR_LEFT,
};

//
//	Text Barmenu Output Style
enum
{
	eTBOS_NORMAL,	
	eTBOS_TYPE1,	//	선택 안했을때는 텍스트만 출력하고 선택하면 바탕에 그림자를 출력
};

class cBarMenu
{
public:
	static	BOOL		s_isExculsiveFocus,s_iExclusiveFocusUpdate;
	int					m_iValidUpdate,m_iExclusiveFocusUpdate;
	BOOL				m_bIsActionedByHotKey;

	BOOL				m_bIsHalfblending;
	BOOL				m_isFocusOwner;
	BOOL				m_bIsDisplayMenuNumber;
	BOOL				m_bIsStrongCheck;

	WORD				m_wCount,m_wFocus,m_wMaxBar;	//	메뉴수,새로운 시리얼,현재 선택된거
	WORD				m_wCheckMode;
	DWORD				m_dwClickMenu;
	cPOINT				m_posPressed;
	cSPRITE				*m_lpSprite;
	cBar				*m_pMenu;

						cBarMenu();
						~cBarMenu();

	void				correctPos(DWORD _dwMenu,int _iX,int _iY);
	void				setCorrectPos(int _iDx,int _iDy);
	void				setCorrectPos(DWORD _dwMenu,int _iDx,int _iDy);
	void				init(cSPRITE *image,int maxbar	= 80,WORD checkmode=dBAR_BUTTON);

	void				setClickImageMargin(DWORD _dwMenu,int _iDx,int _iDy);
	void				setClickImageAllBarMargin(int _iDx,int _iDy);
	void				setClickMargin(DWORD _dwMenu,int _iDx,int _iDy);
	void				setClicktAllBarMargin(int _iDx,int _iDy);
	void				setStrongCheck()	{m_bIsStrongCheck	=	TRUE;}
	void				releaseStrongCheck(){m_bIsStrongCheck	=	FALSE;}

	BOOL				isClickedPos(int _iX,int _iY)
	{
		if (m_posPressed.x	==	_iX	&&	m_posPressed.y	==	_iY)
			return	TRUE;
		return	FALSE;
	}

	void				reset();
	void				close();

	inline	void		setCheckMode(WORD checkmode){	m_wCheckMode	=	checkmode;}
	inline	void		setHalfBlending(BOOL _bIsHalfBlending)	{m_bIsHalfblending=_bIsHalfBlending;}
	inline	BOOL		isPressed(){if (m_dwClickMenu	==	0xffffffff)	return	FALSE;return TRUE;}
	inline	BOOL		isActive(){if (m_wCount)	return	TRUE;return	FALSE;}
	BOOL				isFocusedMenu(DWORD _dwMenu);	//	포커스된 메뉴냐?
	cBar*				getFocusedMenu();//	포커스된 메뉴
	cBar*				getFocusMenu(int _iX,int _iY);//	포커스된 메뉴

	BOOL				addImageBar(DWORD menu,int x,int y,WORD normal=0xffff,WORD active=0xffff,WORD select=0xffff,WORD check=0xffff,WORD disable=0xffff);

	void				setKey(DWORD _dwMenu,int _iKey);

	BOOL				resetImageBar(DWORD destMenu,DWORD menu,int x,int y,WORD normal=0xffff,WORD active=0xffff,WORD select=0xffff,WORD check=0xffff,WORD disable=0xffff);
	BOOL				setImageBar(DWORD destMenu,WORD normal=0xffff,WORD active=0xffff,WORD select=0xffff,WORD check=0xffff,WORD disable=0xffff);
	void				setSize(DWORD menu,int xs,int ys);
	void				setSize(int xs,int ys);
	void				setSize(int x,int y,int xs,int ys);
	void				setSize(DWORD menu,int x,int y,int xs,int ys);
	void				setMenuId(int _iIndex,DWORD _dwMenu);
	DWORD				update(int x,int y,BYTE *_aKeyMap,BOOL _isLeftTouch,BOOL _isLeftPressed,BOOL checkNow = TRUE);
	DWORD				updatePressed(int x,int y,BYTE *_aKeyMap,BOOL _isLeftPressed);
	DWORD				getMenu(int x,int y);
	int					getIndex(DWORD menu);//	menu가 몇번째 인덱스 일까?
	BOOL				getRect(DWORD _dwMenu,cRECT *_lpRect);//menu항목의 둘레

	void				draw();
	BOOL				move(DWORD menu,int x,int y);
	BOOL				move(DWORD menu,int x,int y,int xs,int ys);
	BOOL				resize(DWORD menu,int x,int y);

	void				resetChecker();
	BOOL				check(DWORD menu,WORD check=0xffff);
	BOOL				checkRadio(DWORD _dwMenu,BOOL _isCheck,DWORD _dwBegin,DWORD _dwEnd);
	int					getCheckedMenu();

	BOOL				isCheck(DWORD menu);
	BOOL				isActive(DWORD menu);

	void				enable(DWORD menu);
	void				disable(DWORD menu);
	void				show(DWORD menu);	//	보이게
	void				hide(DWORD menu);	//	안 보이게
	BOOL				isShow(DWORD _dwMenu);

	BOOL				isEnable(DWORD menu);
};

#endif
