#pragma	once

#include	"cGAME.h"
#include	"cBARMENU.h"
#include	"cMESSAGE.h"

enum
{
	eCUI_DUEL_RANKING,			//	텍스트 그리기
};

enum
{
	eUIW_DRAW_TEXT,			//	텍스트 그리기
	eUIW_ADD_BUTTON,		//	버튼 추가
};

class	CTextDC;
class	cCommonUI;

class	cUI_NodeCommon
{
public:
	CPos	m_pos;
	WORD	m_wWork;
	WORD	m_wTextLength;

	char	*m_pstrText;
};

class	cUI_TextNode : public cUI_NodeCommon
{
public:
	UINT	m_uiTextColor;
	int		m_iAlignMethod;
	CTextDC *m_lpTextDC;
	SIZE	m_size;

	void	setText(char *_lpstrText)
	{
		pKILL(m_pstrText);

		int	iLen		=	strlen(_lpstrText);

		m_pstrText		=	new char [iLen+1];

		strcpy(m_pstrText,_lpstrText);
	}

	void	draw(cCommonUI *_lpCommonUI);
};

class	cUI_Node : public cUI_NodeCommon
{
public:
	int		m_aiValue[10];

	cUI_Node()
	{
		m_pstrText	=	NULL;
	}
	~cUI_Node()
	{
		close();
	}

	void	close()
	{
		pKILL(m_pstrText);
	}

	void	copy(cUI_Node *_lpNode)
	{
		close();

		m_pos			=	_lpNode->m_pos;
		m_wWork			=	_lpNode->m_wWork;

		m_wTextLength	=	_lpNode->m_wTextLength;

		if	(m_wTextLength)
		{
			m_pstrText		=	new char [m_wTextLength];
			strcpy(m_pstrText,_lpNode->m_pstrText);
		}

		memcpy(m_aiValue,_lpNode->m_aiValue,sizeof(m_aiValue));
	}
};

const	int	c_iCommonUI_NodeCount	=	256;

enum
{
	eFT_LEFT	=	0,
	eFT_WIDTH_CENTER,
	eFT_HEIGHT_CENTER,
	eFT_CENTER,
	eFT_RIGHT,
};

class	cSPRITE;

class	cCommonUI_common
{
public:
	cRECT		m_rectBox;
	BOOL		m_bIsActive;
	cBARMENU	m_buttonMenu;
	cSPRITE		*m_lpSprite;
	CTooltip	m_ttFrame;
	int			m_iID;

	WORD		m_wNodeCount;
	cUI_Node	m_aNodeList[c_iCommonUI_NodeCount];
	char		m_strTitleText[64];
};

class	cCommonUI_base : public cCommonUI_common
{
public:
	char		m_abBuffer[512];
};

class	cCommonUI_duel_ranking : public cCommonUI_base
{
public:
	int			m_iRankingType;

	void		init();
	void		draw();
	void		update();
};

class	cCommonUI : public cCommonUI_base
{
public:
	cCommonUI_duel_ranking*	IDuelRanking()
	{
		return	(cCommonUI_duel_ranking*)this;
	}

	void		init(cSPRITE *_lpSprite);

	void		open(int _iID,char *_lpstrTitleText,int _iX,int _iY,int _iXS,int _iYS);
	void		close();
	bool		addText(CTextDC *_lpTextDC,int _iX,int _iY,UINT _uiColor,char *_lpstrText,int _iAlignMethod=eFT_LEFT);
	bool		addText(CTextDC *_lpTextDC,int _iX,int _iY,int _iXS,int _iYS,UINT _uiColor,char *_lpstrText,int _iAlignMethod=eFT_LEFT);
	bool		addButton(int _iX,int _iY,UINT _uiMenu,char *_lpstrText);

	void		setBoxSize(int _iX,int _iY,int _iXS,int _iYS);

	void		draw();
	void		drawFrame(cRECT *_lpRect,BOOL _bIsDrawShadow);

	void		update();
};

extern	cCommonUI	g_commonUI;