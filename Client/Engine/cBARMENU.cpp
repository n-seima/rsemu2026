#include "cBARMENU.H"
#include "cDRAW.H"
#include "cTRANS.H"

BOOL		cBARMENU::s_isExculsiveFocus	=	FALSE;
int			cBARMENU::s_iExclusiveFocusUpdate=	0;
CTextDC		cBAR::s_text;

cBAR::cBAR()
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
	m_pstrTooltip	=	NULL;

	m_dwTextColor	=	WHITE;
	m_bIsPutshadow	=	TRUE;
	m_posClickImageMargin.x	=	0;
	m_posClickImageMargin.y	=	0;
	m_posClickTextMargin.x	=	0;
	m_posClickTextMargin.y	=	0;
}

cBAR::~cBAR()
{
	pKILL(m_pstrTooltip);
}

//
//	보이기
void
cBAR::show()
{
	m_isShow	=	TRUE;	
}

//
//	감추기
void
cBAR::hide()
{
	m_isShow	=	FALSE;	
}

//
//	단축키 눌렸냐?
BOOL
cBAR::isShotKey(BYTE *_lpaKeyMap)
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
cBAR::reset()
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
	m_pstrTooltip	=	NULL;
}

//
//	툴팁 설정
void
cBAR::setTooltip(char *_lpstrTooltip)
{
	pKILL(m_pstrTooltip);

	m_pstrTooltip	=	new char [strlen(_lpstrTooltip)+1];

	strcpy(m_pstrTooltip,_lpstrTooltip);
}

//
//	이미지 바
void
cBAR::init(char *str,cSPRITE *sprite,int x,int y,DWORD menu,WORD normal,WORD active,WORD select,WORD check,WORD disable)
{	int image	=	normal;
	
	s_text.selectFont(dWINDOW_FONT_NORMAL);

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

	if (image!=0xffff && sprite)
		sprite->getRect(image,&m_rectCrash);

	m_rectCrash.add(x,y);
	m_pos.Set(x,y);
	m_posCorrect.Set(0,0);
	m_posClickImageMargin.Set(0,0);
	m_posClickTextMargin.Set(0,0);
	m_posTextPos.Set(0xffffffff,0xffffffff);
	m_posCorrectText.Set(0xffffffff,0xffffffff);
	m_bIsPutshadow	=	TRUE;
	m_iKey			=	0xffff;
	m_dwTextColor	=	WHITE;

	setText(str);
}

//
//	텍스트
void
cBAR::init(char *str,DWORD menu,int x,int y,int xs,int ys)
{
	s_text.selectFont(dWINDOW_FONT_NORMAL);

	m_dwMenu	=	menu;

	m_isChecked	=	FALSE;
	m_isClicked	=	FALSE;
	m_isFocused	=	FALSE;
	m_isEnable	=	TRUE;
	m_isShow	=	TRUE;
	m_iKey		=	0xffff;

	strcpy(m_strText,str);

	m_rectCrash.set(x,y,x+xs-1,y+ys-1);
	m_pos.Set(x,y);
	m_posCorrect.Set(0,0);
	m_posTextPos.Set(0xffffffff,0xffffffff);
	m_posCorrectText.Set(0xffffffff,0xffffffff);
}

void
cBAR::setSize(int xs,int ys)
{	m_rectCrash.x2	=	m_rectCrash.x1	+	xs;	
	m_rectCrash.y2	=	m_rectCrash.y1	+	ys;	
}

void
cBAR::setSize(int x,int y,int xs,int ys)
{
	m_rectCrash.x1	=	x;	
	m_rectCrash.y1	=	y;	
	m_rectCrash.x2	=	m_rectCrash.x1	+	xs;	
	m_rectCrash.y2	=	m_rectCrash.y1	+	ys;	
}

//
//	이미지바 그리기
void
cBAR::drawImageBar(cBARMENU *_lpBarMenu,BOOL _bIsDisplayMenuNumber,int _iTextGab)
{
	if (!m_isShow)	return;

	char	strText[512];

	strText[0]	=	NULL;

	if (m_strText[0])
	{
		if (_bIsDisplayMenuNumber)	sprintf(strText,"%.2d) %s",m_dwMenu,m_strText);
		else						strcpy(strText,m_strText);
	}

	DWORD	dwTextColor;
	BOOL	bIsHalfBlending	=	_lpBarMenu->m_bIsHalfblending;

	if (m_isEnable)
	{	if (m_isChecked)
		{	dwTextColor	=	_lpBarMenu->m_dwCheckText;
		}
		else
		if (m_isFocused)
		{	dwTextColor	=	_lpBarMenu->m_dwFocusText;
		}
		else
		{	dwTextColor	=	_lpBarMenu->m_dwNormalText;
		}
	}
	else	dwTextColor	=	_lpBarMenu->m_dwDisableText;

	if (_lpBarMenu->m_lpSprite)
	{	int		wImage	=	0xffff;
		CPos	posImage,posText;

		posImage.x	=	m_posCorrect.x+m_pos.x;
		posImage.y	=	m_posCorrect.y+m_pos.y;
		posText.x	=	m_posCorrect.x+m_rectCrash.x1+m_rectCrash.getWidth()/2;
		posText.y	=	m_posCorrect.y+m_rectCrash.y1+(m_rectCrash.getHeight()-s_text.getFontHeight())/2-m_bIsPutshadow;

		if (m_posTextPos.x	!=	0xffffffff)
		{
			posText.x	=	posImage.x+m_posTextPos.x;
			posText.y	=	posImage.y+m_posTextPos.y;
		}

		if (m_posCorrectText.x	!=	0xffffffff)
		{
			posText.x	=	posImage.x+m_posCorrectText.x;
			posText.y	=	posImage.y+m_posCorrectText.y;
		}

		if (m_isEnable)
		{
			wImage	=	m_wNormal;

			if (m_isFocused	&&	m_wActive!=0xffff)	wImage=m_wActive;
			if (m_isClicked	&&	m_wSelect!=0xffff)
			{
				posImage.x	+=	m_posClickImageMargin.x;
				posImage.y	+=	m_posClickImageMargin.y;
				posText.x	+=	m_posClickTextMargin.x;
				posText.y	+=	m_posClickTextMargin.y;
				wImage		=	m_wSelect;
			}
		}
		else	if (m_wDisable!=0xffff)	wImage	=	m_wDisable;

		if (bIsHalfBlending)
		{
			if (wImage		!=	0xffff				)	_lpBarMenu->m_lpSprite->Put(posImage.x,posImage.y,wImage  ,100,100,dPUT_HALF_BLENDING);
			if (m_isChecked	&&	m_wCheck !=0xffff	)	_lpBarMenu->m_lpSprite->Put(posImage.x,posImage.y,m_wCheck,100,100,dPUT_HALF_BLENDING);
		}
		else
		{
			if (wImage		!=	0xffff				)	_lpBarMenu->m_lpSprite->Put(posImage.x,posImage.y,wImage);
			if (m_isChecked	&&	m_wCheck !=0xffff	)	_lpBarMenu->m_lpSprite->Put(posImage.x,posImage.y,m_wCheck);
		}

		if (strText[0]!=	0)
		{
			if (m_posTextPos.x	!=	0xffffffff)
			{
				s_text.pushShadowStatus(m_bIsPutshadow);
				s_text.put(posText.x,posText.y,m_dwTextColor,strText);
				s_text.popShadowStatus();
			}
			else
			if (_lpBarMenu->m_wTextOutputMethod	==	eBTOM_BAR_LEFT)
			{
				s_text.pushShadowStatus(m_bIsPutshadow);
				s_text.put(posImage.x+_iTextGab,posText.y,m_dwTextColor,strText);
				s_text.popShadowStatus();
			}
			else
			if (_lpBarMenu->m_wTextOutputMethod	==	eBTOM_CENTER)
			{
				s_text.pushShadowStatus(m_bIsPutshadow);
				s_text.cPut(posText.x,posText.y,m_dwTextColor,strText);
				s_text.popShadowStatus();
			}
			else
			if (_lpBarMenu->m_wTextOutputMethod	==	eBTOM_BAR_RIGHT)
			{
				int	iDx	=	_lpBarMenu->m_lpSprite->getSpriteWidth(m_wNormal);

				s_text.pushShadowStatus(m_bIsPutshadow);
				s_text.put(posImage.x+iDx+4,posText.y,m_dwTextColor,strText);
				s_text.popShadowStatus();
			}
		}
	}
}

//
//	텍스트바 그리기
void
cBAR::drawTextBar(cBARMENU *_lpBarMenu,BOOL _bIsDrawMenuNumber)
{
	if (!m_isShow)	return;

	DWORD	TextColor;
	WORD	BoxColor,FillColor;
	CPos	addPoint;
	char	strText[256];

	if (m_isEnable)
	{	if (m_isChecked)
		{	TextColor	=	_lpBarMenu->m_dwCheckText;
			BoxColor	=	_lpBarMenu->m_wCheckBox;
			FillColor	=	_lpBarMenu->m_wCheckColor;
		}
		else
		if (m_isFocused)
		{	TextColor	=	_lpBarMenu->m_dwFocusText;
			BoxColor	=	_lpBarMenu->m_wFocusBox;
			FillColor	=	_lpBarMenu->m_wFocusColor;
		}
		else
		{	TextColor	=	_lpBarMenu->m_dwNormalText;
			BoxColor	=	_lpBarMenu->m_wNormalBox;
			FillColor	=	_lpBarMenu->m_wNormalColor;
		}
	}
	else
	{	TextColor	=	_lpBarMenu->m_dwDisableText;
		BoxColor	=	_lpBarMenu->m_wDisableBox;
		FillColor	=	_lpBarMenu->m_wDisableColor;
	}

	if(m_isClicked)	addPoint.x	=	1,addPoint.y	=	1;
	else			addPoint.x	=	0,addPoint.y	=	0;

	if (_bIsDrawMenuNumber)	sprintf(strText,"%.2d) %s",m_dwMenu,m_strText);
	else					strcpy(strText,m_strText);

	m_rectCrash.add(addPoint.x+m_posCorrect.x,addPoint.y+m_posCorrect.y);

	if (_lpBarMenu->m_wTextBarOutputStyle	==	eTBOS_NORMAL)
	{
		cDRAW::FillHB(FillColor	,m_rectCrash.x1	,m_rectCrash.y1,m_rectCrash.x2,m_rectCrash.y2);
		cDRAW::Box	 (BoxColor	,m_rectCrash.x1	,m_rectCrash.y1,m_rectCrash.x2,m_rectCrash.y2);
	}

	if (_lpBarMenu->m_wTextBarOutputStyle	==	eTBOS_TYPE1)
	{
//		if (m_isChecked)
//			cDRAW::FillHB(FillColor	,m_rectCrash.x1	,m_rectCrash.y1,m_rectCrash.x2,m_rectCrash.y2);
	}

	int	iHeight	=	s_text.getFontHeight()/2;
	CPos	pos;

	if (_lpBarMenu->m_wTextOutputMethod	==	eBTOM_BAR_LEFT)
	{
		s_text.pushShadowStatus(m_bIsPutshadow);
		pos.x	=	m_rectCrash.x1+2;
		pos.y	=	m_rectCrash.y1+m_rectCrash.getHeight()/2-iHeight	;
		s_text.put(pos.x,pos.y,TextColor	,strText);
		s_text.popShadowStatus();
	}
	else
	if (_lpBarMenu->m_wTextOutputMethod	==	eBTOM_CENTER)
	{
		s_text.pushShadowStatus(m_bIsPutshadow);
		pos.x	=	m_rectCrash.x1+m_rectCrash.getWidth()/2	;
		pos.y	=	m_rectCrash.y1+m_rectCrash.getHeight()/2-iHeight	;
		s_text.cPut(pos.x,pos.y,TextColor	,strText);
		s_text.popShadowStatus();
	}
	else
	if (_lpBarMenu->m_wTextOutputMethod	==	eBTOM_BAR_RIGHT)
	{
		int	iDx	=	_lpBarMenu->m_lpSprite->getSpriteWidth(m_wNormal);

		s_text.pushShadowStatus(m_bIsPutshadow);
		pos.x	=	m_rectCrash.x1+2;
		pos.y	=	m_rectCrash.y1+m_rectCrash.getHeight()/2-iHeight	;
		s_text.put(pos.x+iDx+4,pos.y,m_dwTextColor,strText);
		s_text.popShadowStatus();
	}

	m_rectCrash.add(-addPoint.x-m_posCorrect.x,-addPoint.y-m_posCorrect.y);
}

BOOL
cBAR::isFocus(int x,int y,BOOL _bIsIgnoreBarStatus)
{
	if (!_bIsIgnoreBarStatus)
		if (!m_isEnable || !m_isShow)
			return FALSE;

	x	-=	m_posCorrect.x;
	y	-=	m_posCorrect.y;

	return m_rectCrash.isIn(x,y);
}

void
cBAR::update(BOOL focus,BOOL click)
{
	m_isClicked	=	click;
	m_isFocused	=	focus;
}

void
cBAR::move(int x,int y)
{
	if	(x==0xffff)
		x = m_rectCrash.x1;
	if	(y==0xffff)
		y = m_rectCrash.y1;

	m_pos.x			=	x;
	m_pos.y			=	y;

	m_rectCrash.x2	=	(m_rectCrash.x2	-	m_rectCrash.x1)	+	x;
	m_rectCrash.y2	=	(m_rectCrash.y2	-	m_rectCrash.y1)	+	y;
	m_rectCrash.x1	=	x;
	m_rectCrash.y1	=	y;
}

void
cBAR::move(int x,int y,int xs,int ys)
{
	if	(x==0xffff) 
		x = m_rectCrash.x1;
	if	(y==0xffff) 
		y = m_rectCrash.y1;

	m_pos.x			=	x;
	m_pos.y			=	y;

	m_rectCrash.x2	=	x+xs;;
	m_rectCrash.y2	=	y+ys;
	m_rectCrash.x1	=	x;
	m_rectCrash.y1	=	y;
}

void
cBAR::resize(int xs,int ys)
{
	m_rectCrash.x2	=	m_rectCrash.x1+xs;
	m_rectCrash.y2	=	m_rectCrash.y1+ys;
}

//
//	출력할 텍스트의 컬러를 선택한다.
void
cBAR::setTextColor(DWORD _dwColor,BOOL _bIsPutShadow)
{
	m_dwTextColor	=	_dwColor;
	m_bIsPutshadow	=	_bIsPutShadow;
}

/****************************************************************
	Barm_dwMenu class
****************************************************************/

cBARMENU::cBARMENU()
{	
	m_wCount			=	0;
	m_wFocus			=	0xffff;
	m_wMaxBar			=	0;
	m_lpSprite			=	NULL;
	m_pMenu				=	NULL;
	m_isFocusOwner		=	FALSE;
	m_wTextOutputMethod	=	eBTOM_CENTER;
	m_wTextBarOutputStyle=	eTBOS_NORMAL;
	m_bIsStrongCheck	=	FALSE;
}

cBARMENU::~cBARMENU()
{	
	close();
}

void
cBARMENU::close()
{	
	pKILL(m_pMenu);
	reset();
}

void
cBARMENU::init(cSPRITE *image,int maxbar,WORD checkmode,int _iTextOutputMethod,BOOL _bIsDisplayMenuNumber)
{
	pKILL(m_pMenu);

	m_lpSprite				=	image;
	m_wMaxBar				=	maxbar;
	m_wCheckMode			=	checkmode;
	m_wCount				=	0;
	m_bIsDisplayMenuNumber	=	_bIsDisplayMenuNumber;
	m_pMenu					=	new cBAR [maxbar];
	m_isFocusOwner			=	FALSE;
	m_dwClickMenu			=	0xffffffff;

	m_wTextOutputMethod		=	_iTextOutputMethod;

//	         (normalText,focusText,checkText,disableText,noramlColor,focusColor,checkColor,disableColor)
	initColor(LTGRAY    ,WHITE    ,WHITE    ,GRAY		,0          ,_GREEN    ,_LTGREEN  ,_DEEPGRAY);
}

void
cBARMENU::init(int maxbar,WORD checkmode,BOOL _bIsDisplayMenuNumber)
{
	pKILL(m_pMenu);

	m_wMaxBar				=	maxbar;
	m_wCheckMode			=	checkmode;
	m_wCount				=	0;
	m_dwClickMenu			=	0xffffffff;
	m_lpSprite				=	NULL;
	m_isFocusOwner			=	FALSE;
	m_bIsStrongCheck		=	FALSE;
	m_bIsDisplayMenuNumber	=	_bIsDisplayMenuNumber;
	m_pMenu					=	new cBAR [maxbar];

//	         (normalText,focusText,checkText,disableText,noramlColor,focusColor,checkColor,disableColor)
	initColor(LTGRAY    ,WHITE    ,WHITE    ,GRAY		  ,0          ,_GREEN    ,_LTGREEN  ,_DEEPGRAY);
}

void
cBARMENU::initColor(DWORD normalText,DWORD focusText,DWORD checkText,DWORD disableText,WORD noramlColor,WORD focusColor,WORD checkColor,WORD disableColor)
{
	m_dwNormalText		=	normalText;		
	m_dwFocusText		=	focusText;
	m_dwCheckText		=	checkText;
	m_dwDisableText		=	disableText;
	m_wNormalColor		=	noramlColor;
	m_wFocusColor		=	focusColor;
	m_wCheckColor		=	checkColor;
	m_wDisableColor		=	disableColor;

	m_wNormalBox		=	RGB24To16(normalText);
	m_wFocusBox			=	RGB24To16(focusText);
	m_wCheckBox			=	RGB24To16(checkText);
	m_wDisableBox		=	RGB24To16(disableText);
}

void
cBARMENU::reset()
{
	m_wCount		=	0;
	m_wFocus		=	0xffff;
	m_isFocusOwner	=	FALSE;
	m_dwClickMenu	=	0xffffffff;
}

//
//	위치 교정
void
cBARMENU::correctPos(DWORD _dwMenu,int _iX,int _iY)
{
	if	(_dwMenu	==	0xffffffff)
	{
		for (int i=0;i<m_wMaxBar;i++)
		{
			m_pMenu[i].m_pos.x	=	_iX;
			m_pMenu[i].m_pos.y	=	_iY;
			m_pMenu[i].m_rectCrash.move(_iX,_iY);
		}

		return;
	}

	int iIndex	=	getIndex(_dwMenu);

	if (iIndex	==	0xffff)
		return;

	m_pMenu[iIndex].m_pos.x	=	_iX;
	m_pMenu[iIndex].m_pos.y	=	_iY;
	m_pMenu[iIndex].m_rectCrash.move(_iX,_iY);
}

void
cBARMENU::setCorrectPos(int _iDx,int _iDy)
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
cBARMENU::setCorrectPos(DWORD _dwMenu,int _iDx,int _iDy)
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
cBARMENU::addImageBar(char *_strText,DWORD menu,int x,int y,WORD normal,WORD active,WORD select,WORD check,WORD disable,char *_lpstrTooltip)
{
	if	(m_wCount >= m_wMaxBar)
		return FALSE;

	m_pMenu[m_wCount].init(_strText,m_lpSprite,x,y,menu,normal,active,select,check,disable);

	if (_lpstrTooltip)	m_pMenu[m_wCount].setTooltip(_lpstrTooltip);

	m_wCount++;

	return TRUE;
}

//
//	이미지바 재설정
BOOL
cBARMENU::resetImageBar(DWORD destMenu,char *_strText,DWORD menu,int x,int y,WORD normal,WORD active,WORD select,WORD check,WORD disable)
{
	int	iIndex	=	getIndex(destMenu);

	if (iIndex	==	0xffff)	return	FALSE;

	m_pMenu[iIndex].init(_strText,m_lpSprite,x,y,menu,normal,active,select,check,disable);

	return	TRUE;
}

//
//	이미지바 설정
BOOL
cBARMENU::setImageBar(DWORD destMenu,WORD normal,WORD active,WORD select,WORD check,WORD disable)
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

//
//	텍스트바 추가
BOOL
cBARMENU::addTextBar(char *str,DWORD menu,int x,int y,int xs,int ys,char *_lpstrTooltip)
{	if (m_wCount >= m_wMaxBar) return FALSE;

	m_pMenu[m_wCount].init(str,menu,x,y,xs,ys);
	if (_lpstrTooltip)	m_pMenu[m_wCount].setTooltip(_lpstrTooltip);

	m_wCount++;

	return TRUE;
}

//
//	menu에 텍스트를 변경
void
cBARMENU::setText(DWORD menu,char *_strText)
{
	int	i,iIndex	=	0xffff;

	for (i=0;i<m_wCount;i++)
		if (m_pMenu[i].m_dwMenu	==	menu)
		{	iIndex	=	i;
			break;
		}

	if (iIndex	==	0xffff)	return;

	strcpy(m_pMenu[iIndex].m_strText,_strText);
}

void
cBARMENU::setSize(DWORD menu,int xs,int ys)
{	int index	=	getIndex(menu);

	if (index == 0xffff)	return;

	m_pMenu[index].setSize(xs,ys);
}

void
cBARMENU::setSize(int xs,int ys)
{	m_pMenu[m_wCount-1].setSize(xs,ys);
}

void
cBARMENU::setSize(int x,int y,int xs,int ys)
{	m_pMenu[m_wCount-1].setSize(x,y,xs,ys);
}

void
cBARMENU::setSize(DWORD menu,int x,int y,int xs,int ys)
{
	int index	=	getIndex(menu);

	if (index == 0xffff)	return;

	m_pMenu[index].setSize(x,y,xs,ys);
}

void
cBARMENU::setMenuId(int _iIndex,DWORD _dwMenu)
{
	if (_iIndex	>=	m_wCount)	return;

	m_pMenu[_iIndex].m_dwMenu	=	_dwMenu;
}


//
//	체커 전부 지우기
void
cBARMENU::resetChecker()
{
	for (int i=0;i<m_wCount;i++)	m_pMenu[i].m_isChecked	=	FALSE;
		
}	//	cBARMENU::resetChecker()

//
//
BOOL
cBARMENU::check(DWORD menu,WORD check)
{	int i,index	=	0xffff;

	for (i=0;i<m_wCount;i++)
		if (m_pMenu[i].m_dwMenu	==	menu)
		{	
			index	=	i;

			if (m_pMenu[i].m_isEnable	==	FALSE)	return	FALSE;

			break;
		}

	if (index == 0xffff) return FALSE;

	switch(m_wCheckMode)
	{	case	dBAR_RADIO	:
			for (i=0;i<m_wCount;i++)	m_pMenu[i].m_isChecked	=	FALSE;

			if (check==0xffff)	m_pMenu[index].m_isChecked	=	TRUE;
			else				m_pMenu[index].m_isChecked	=	(BOOL)check;
			return	TRUE;

		case	dBAR_CHECK	:
			if (check==0xffff)	m_pMenu[index].m_isChecked	=	1	-	m_pMenu[index].m_isChecked;
			else				m_pMenu[index].m_isChecked	=	(BOOL)check;

			return	TRUE;
	}

	return	FALSE;
}

//
//
BOOL
cBARMENU::checkRadio(DWORD _dwCheckMenu,BOOL _isCheck,DWORD _dwBegin,DWORD _dwEnd)
{
	DWORD	dwMenu;

	for (dwMenu=_dwBegin;dwMenu<=_dwEnd;dwMenu++)	check(dwMenu,FALSE);

	return	check(_dwCheckMenu,TRUE);
}

//
//스트링을 검색해서 바에 체크한다.
BOOL
cBARMENU::checkByTitle(char *_lpString,WORD check)
{
	int i,index	=	0xffff;

	for (i=0;i<m_wCount;i++)
		if (stricmp(m_pMenu[i].m_strText,_lpString)	==	0)
		{
			if (m_pMenu[i].m_isEnable	==	FALSE)
				return	FALSE;

			index	=	i;
			break;
		}

	if (index	==	0xffff)
		return	FALSE;

	switch(m_wCheckMode)
	{	
		case	dBAR_RADIO	:
			for (i=0;i<m_wCount;i++)	
				m_pMenu[i].m_isChecked	=	FALSE;

			m_pMenu[index].m_isChecked	=	TRUE;

			return	TRUE;

		case	dBAR_CHECK	:
			if (check==0xffff)
				m_pMenu[index].m_isChecked	=	1	-	m_pMenu[index].m_isChecked;
			else
				m_pMenu[index].m_isChecked	=	(BOOL)check;

			return	TRUE;
	}

	return	FALSE;
}

//
//	바 메뉴 업데이트
DWORD
cBARMENU::update(int x,int y,BYTE *_lpaKeyMap,BOOL _isLeftTouch,BOOL _isLeftPressed,BOOL checkNow)
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
cBAR*
cBARMENU::getFocusMenu(int _iX,int _iY)
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
cBARMENU::updatePressed(int x,int y,BYTE *_lpaKeyMap,BOOL _isLeftPressed)
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
cBARMENU::getMenu(int x,int y)
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
cBARMENU::getIndex(DWORD menu)
{
	int i;

	for (i=0;i<m_wCount;i++)
		if (m_pMenu[i].m_dwMenu	==	menu)	return i;

	return 0xffff;
}

//
//menu항목의 둘레
BOOL
cBARMENU::getRect(DWORD _dwMenu,cRECT *_lpRect)
{
	int	iIndex	=	getIndex(_dwMenu);

	if (iIndex	==	0xffff)	return	FALSE;

	memcpy(_lpRect,&m_pMenu[iIndex].m_rectCrash,sizeof(cRECT));

	return	TRUE;
}

//
//	그려 주세요.. *-_-*
void
cBARMENU::draw()
{	
	if (m_lpSprite)
		for (int i=0;i<m_wCount;i++)
			m_pMenu[i].drawImageBar(this,m_bIsDisplayMenuNumber,m_iTextLeftGab);
	else
	{
		for (int i=0;i<m_wCount;i++)	m_pMenu[i].drawTextBar(this,m_bIsDisplayMenuNumber);
	}
}


//
//	menu 항목이 체크된거냐?
BOOL
cBARMENU::isCheck(DWORD menu)
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
cBARMENU::getCheckedMenu()
{
	for (int i=0;i<m_wCount;i++)
		if (m_pMenu[i].m_isEnable)
			if (m_pMenu[i].m_isChecked)
				return m_pMenu[i].m_dwMenu;

	return	-1;
}

//
//	체크된 메뉴의 스트링을 리턴
BOOL
cBARMENU::getCheckedMenuString(char *_lpstrString)
{
	for (int i=0;i<m_wCount;i++)
		if (m_pMenu[i].m_isEnable)
			if (m_pMenu[i].m_isChecked)
			{
				strcpy(_lpstrString,m_pMenu[i].m_strText);
				
				return	TRUE;
			}

	return	FALSE;
}

BOOL
cBARMENU::isActive(DWORD menu)
{	
	int i;

	for (i=0;i<m_wCount;i++)
		if (m_pMenu[i].m_dwMenu==menu)
			if (m_pMenu[i].m_isFocused)	return TRUE;

	return FALSE;
}

BOOL
cBARMENU::move(DWORD menu,int x,int y)
{
	int index = getIndex(menu);

	if	(index == 0xffff)
		return FALSE;

	m_pMenu[index].move(x,y);

	return TRUE;
}

BOOL
cBARMENU::move(DWORD menu,int x,int y,int xs,int ys)
{	
	int index = getIndex(menu);

	if (index == 0xffff) return FALSE;

	m_pMenu[index].move(x,y,xs,ys);

	return TRUE;	
}

BOOL
cBARMENU::resize(DWORD menu,int xs,int ys)
{
	int index = getIndex(menu);

	if (index == 0xffff) return FALSE;

	m_pMenu[index].resize(xs,ys);

	return TRUE;
}

void
cBARMENU::enable(DWORD menu)
{
	int index = getIndex(menu);
	if (index==0xffff) return;

	m_pMenu[index].m_isEnable	=	TRUE;
}

void
cBARMENU::disable(DWORD menu)
{
	int index = getIndex(menu);
	if (index==0xffff) return;

	m_pMenu[index].m_isEnable	=	FALSE;
}

BOOL
cBARMENU::isEnable(DWORD menu)
{
	int index = getIndex(menu);
	if (index==0xffff) return FALSE;

	return m_pMenu[index].m_isEnable;
}

//
//	보이게
void
cBARMENU::show(DWORD _dwMenu)
{
	int index = getIndex(_dwMenu);

	if (index==0xffff) return;

	m_pMenu[index].show();
}	//	cBARMENU::show(DWORD _dwMenu)

//
//	안 보이게
void
cBARMENU::hide(DWORD _dwMenu)
{
	int index = getIndex(_dwMenu);

	if (index==0xffff) return;

	m_pMenu[index].hide();
}	//	cBARMENU::hide(DWORD _dwMenu)

//
//	보이냐?
BOOL
cBARMENU::isShow(DWORD _dwMenu)
{	int index = getIndex(_dwMenu);

	if (index==0xffff) return FALSE;

	return m_pMenu[index].m_isShow;
}	//	cBARMENU::isShow(DWORD _dwMenu)

//
//특정 메뉴에 키설정
void
cBARMENU::setKey(DWORD _dwMenu,int _iKey)
{
	int	iIndex	=	getIndex(_dwMenu);

	if (iIndex	==	0xffff)	return;

	m_pMenu[iIndex].m_iKey	=	_iKey;
}

//
//	포커스된 메뉴냐?
BOOL
cBARMENU::isFocusedMenu(DWORD _dwMenu)
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
char*
cBARMENU::getFocusedMenuTooltop()
{
	for (int i=0;i<m_wCount;i++)
		if (m_pMenu[i].m_isFocused)
			return	m_pMenu[i].getTooltip();

	return	NULL;
}

//
//	포커스된 메뉴의 툴팁
cBAR*
cBARMENU::getFocusedMenu()
{
	for (int i=0;i<m_wCount;i++)
		if (m_pMenu[i].m_isFocused)
			return	&m_pMenu[i];

	return	NULL;
}

//
//	포커스된 메뉴냐?
char*
cBARMENU::getTooltop(DWORD _dwMenu)
{
	int	iIndex	=	getIndex(_dwMenu);

	if (iIndex	==	0xffff)	return	FALSE;

	return	m_pMenu[iIndex].getTooltip();
}

//
//	포커스된 메뉴의 툴팁
void
cBARMENU::setTooltop(DWORD _dwMenu,char *_lpstrTooltip)
{
	int	iIndex	=	getIndex(_dwMenu);

	if (iIndex	==	0xffff)	return;

	m_pMenu[iIndex].setTooltip(_lpstrTooltip);
}

//
//	출력할 텍스트의 컬러를 선택한다.
void
cBARMENU::setTextColor(DWORD _dwMenu,DWORD _dwColor,BOOL _bIsPutShadow)
{
	int	iIndex	=	getIndex(_dwMenu);

	if (iIndex	==	0xffff)	return;

	m_pMenu[iIndex].setTextColor(_dwColor,_bIsPutShadow);
}

//
//	출력할 텍스트의 컬러를 선택한다.
void
cBARMENU::setAllBarTextColor(DWORD _dwColor,BOOL _bIsPutShadow)
{
	for (int i=0;i<m_wCount;i++)	m_pMenu[i].setTextColor(_dwColor,_bIsPutShadow);
}

//
//	특정 버튼의 클릭 마진 설정
void
cBARMENU::setClickImageMargin(DWORD _dwMenu,int _iDx,int _iDy)
{
	int	iIndex	=	getIndex(_dwMenu);

	if (iIndex	==	0xffff)	return;

	m_pMenu[iIndex].m_posClickImageMargin.Set(_iDx,_iDy);
}

//
//	모든 버튼의 클릭 마진 설정
void
cBARMENU::setClickImageAllBarMargin(int _iDx,int _iDy)
{
	for (int i=0;i<m_wCount;i++)	m_pMenu[i].m_posClickImageMargin.Set(_iDx,_iDy);
}

//
//	특정 버튼의 클릭 마진 설정
void
cBARMENU::setClickTextMargin(DWORD _dwMenu,int _iDx,int _iDy)
{
	int	iIndex	=	getIndex(_dwMenu);

	if (iIndex	==	0xffff)	return;

	m_pMenu[iIndex].m_posClickTextMargin.Set(_iDx,_iDy);
}

//
//	텍스트 출력위치 설정
void
cBARMENU::setTextPos(DWORD _dwMenu,int _iX,int _iY)
{
	int	iIndex	=	getIndex(_dwMenu);

	if (iIndex	==	0xffff)	return;

	m_pMenu[iIndex].m_posTextPos.Set(_iX,_iY);
}

//
//	텍스트 출력위치 설정
void
cBARMENU::correctTextPos(DWORD _dwMenu,int _iX,int _iY)
{
	int	iIndex	=	getIndex(_dwMenu);

	if (iIndex	==	0xffff)
		return;

	m_pMenu[iIndex].m_posCorrectText.Set(_iX,_iY);
}

//
//	모든 버튼의 클릭 마진 설정
void
cBARMENU::setClickTextAllBarMargin(int _iDx,int _iDy)
{
	for (int i=0;i<m_wCount;i++)	m_pMenu[i].m_posClickTextMargin.Set(_iDx,_iDy);
}


//
//	특정 버튼의 클릭 마진 설정
void
cBARMENU::setClickMargin(DWORD _dwMenu,int _iDx,int _iDy)
{
	setClickImageMargin(_dwMenu,_iDx,_iDy);
	setClickTextMargin(_dwMenu,_iDx,_iDy);
}

//
//	모든 버튼의 클릭 마진 설정
void
cBARMENU::setClicktAllBarMargin(int _iDx,int _iDy)
{
	setClickImageAllBarMargin(_iDx,_iDy);
	setClickTextAllBarMargin(_iDx,_iDy);
}