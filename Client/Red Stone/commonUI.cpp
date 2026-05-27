#include	"commonUI.h"
#include	"cIMAGE.h"
#include	"cTEXT.h"
#include	"CGamePlay.h"

cCommonUI	g_commonUI;

void
cUI_TextNode::draw(cCommonUI *_lpCommonUI)
{
	CPos	pos;

	pos		=	m_pos;

	pos.x	+=	_lpCommonUI->m_rectBox.x1;
	pos.y	+=	_lpCommonUI->m_rectBox.y1;

	switch(m_iAlignMethod)
	{
		case	eFT_LEFT			:
			if	(m_size.cx)
				m_lpTextDC->put(pos.x,pos.y,m_size.cx,m_size.cy,0,m_uiTextColor,m_pstrText);
			else
				m_lpTextDC->put(pos.x,pos.y,m_uiTextColor,m_pstrText);
			break;
		case	eFT_CENTER			:
			if	(m_size.cx)
				m_lpTextDC->cPut(pos.x,pos.y,m_size.cx,m_size.cy,0,m_uiTextColor,m_pstrText);
			else
				m_lpTextDC->cPut(pos.x,pos.y,m_uiTextColor,m_pstrText);
			break;
		case	eFT_RIGHT			:
			m_lpTextDC->putRF(pos.x,pos.y,m_uiTextColor,m_pstrText);
			break;
	}
}

void
cCommonUI::init(cSPRITE *_lpSprite)
{
	m_lpSprite	=	_lpSprite;
}

void
cCommonUI::open(int _iID,char *_lpstrTitleText,int _iX,int _iY,int _iXS,int _iYS)
{
	if	(CGamePlay::s_iPopupInterface	==	ePIW_COMMON_UI)
	{
		CGamePlay::ClosePopupWindow();

		return;
	}

	CGamePlay::ClosePopupWindow();

	CGamePlay::s_iPopupInterface	=	ePIW_COMMON_UI;

	setBoxSize(_iX,_iY,_iXS,_iYS);

	CGamePlay::s_rectInterfaceSize.x1 = g_iScreenWidth-_iX;
	CGamePlay::s_rectInterfaceSize.x2 = g_iScreenWidth;

	m_iID	=	_iID;

	m_bIsActive	=	TRUE;

	strcpy(m_strTitleText,_lpstrTitleText);

	switch(_iID)
	{
		case	eCUI_DUEL_RANKING	:
			IDuelRanking()->init();
			break;
	}
}

void
cCommonUI::close()
{
	for (int i=c_iCommonUI_NodeCount;i;)
	{
		--i;
		m_aNodeList[i].close();
	}

	m_bIsActive	=	FALSE;
}

bool
cCommonUI::addText(CTextDC *_lpTextDC,int _iX,int _iY,int _iXS,int _iYS,UINT _uiColor,char *_lpstrText,int _iAlignMethod)
{
	if	(m_wNodeCount	>=	c_iCommonUI_NodeCount)
		return	false;

	cUI_TextNode	*lpNode	=	(cUI_TextNode	*)&m_aNodeList[m_wNodeCount];

	lpNode->m_uiTextColor	=	_uiColor;
	lpNode->m_pos.x			=	_iX;
	lpNode->m_pos.y			=	_iY;
	lpNode->m_size.cx		=	_iXS;
	lpNode->m_size.cy		=	_iYS;
	lpNode->m_iAlignMethod	=	_iAlignMethod;
	lpNode->m_lpTextDC		=	_lpTextDC;

	lpNode->setText(_lpstrText);

	m_wNodeCount++;

	return	true;
}

bool
cCommonUI::addText(CTextDC *_lpTextDC,int _iX,int _iY,UINT _uiColor,char *_lpstrText,int _iAlignMethod)
{
	if	(m_wNodeCount	>=	c_iCommonUI_NodeCount)
		return	false;

	cUI_TextNode	*lpNode	=	(cUI_TextNode	*)&m_aNodeList[m_wNodeCount];

	lpNode->m_uiTextColor	=	_uiColor;
	lpNode->m_pos.x			=	_iX;
	lpNode->m_pos.y			=	_iY;
	lpNode->m_size.cx		=	0;
	lpNode->m_size.cy		=	0;
	lpNode->m_iAlignMethod	=	_iAlignMethod;
	lpNode->m_lpTextDC		=	_lpTextDC;

	lpNode->setText(_lpstrText);

	m_wNodeCount++;

	return	true;
}

void
cCommonUI::setBoxSize(int _iX,int _iY,int _iXS,int _iYS)
{
	m_rectBox.set(_iX,_iY,_iX+_iXS,_iY+_iYS);
}

void
cCommonUI::draw()
{
	if	(m_bIsActive	==	FALSE)
		return;

	cRECT	rect	=	m_rectBox;

	g_msgBox.drawFrame(&rect,TRUE,m_strTitleText);

	switch(m_iID)
	{
		case	eCUI_DUEL_RANKING	:
			IDuelRanking()->draw();
			break;
	}

	for (int i=0;i<m_wNodeCount;i++)
	{
		cUI_Node	*lpNode	=	&m_aNodeList[i];

		switch(lpNode->m_wWork)
		{
			case	eUIW_DRAW_TEXT	:			//	텍스트 그리기
			{
				((cUI_TextNode *)lpNode)->draw(this);

				break;
			}
		}
	}

	m_buttonMenu.draw();
}

void
cCommonUI::update()
{
	if	(m_bIsActive	==	FALSE)
		return;

	switch(m_iID)
	{
		case	eCUI_DUEL_RANKING	:
			IDuelRanking()->update();
			break;
	}
}
