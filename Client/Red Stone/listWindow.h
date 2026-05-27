#ifndef _classlist_window_h
#define	_classlist_window_h

#include "CGamePlay.H"

const	int	c_iMaxListWindowContentsCount	=	256;

class	cListWindowContents
{
public:
	char	m_strText[128];
	int		m_iID;
	int		m_aiValue[6];
	BOOL	m_bIsSelected;
	int		m_iPlusValue;
	int		m_iLevel;
};

enum
{
	eCLW_SELECT_CH5_TITLE_BONUS			,
	eCLW_SELECT_CH5_RELEASE_REVERSION	,
	eCLW_SELECT_CH5_REVERSE_STATE_POINT	,
	eCLW_SELECT_CH5_REVERSE_SKILL_POINT	,

	eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_LEVEL_UP	,
	eCLW_SELECT_CH5_UNIQUE_ITEM_PREFIX_LEVEL_UP	,
	eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_UPGRADE	,
	eCLW_SELECT_CH5_UNIQUE_ITEM_PREFIX_UPGRADE	,
	eCLW_SELECT_ALLIGNMENT					,
	eCLW_SELECT_DISJOINTING_ITEM			,

};

class	cListWindow	:	public CGamePlay
{
public:

	int				m_iCount;
	int				m_iPoint;
	int				m_iSelectCount,m_iIncreaseCount;
	CPos			m_pos;
	int				m_iMode;
	BOOL			m_bExistBottomText;
	cRECT			m_rectFocus;

	int				m_aiValue[10];
	int				m_iFocus,m_iRealFocus;

	cListWindowContents	m_aContentsList[c_iMaxListWindowContentsCount];
	cBARMENU		m_bmPlus;
	cBARMENU		m_bmMinus;
	BOOL			m_bIsExistPlus;
	BOOL			m_bIsMultiSelect;
	BOOL			m_bIsSelectAble;

	char			m_strOkButtonText[64];
	char			m_strBottomText[256];
	char			m_strTitle[256];

					cListWindow();

	void			init();
	void			reset();
	void			close();
	int				getFocus();
	void			select(int _iIndex);
	int				getSelectedCount();
	int				getSelected();

	void			open(int _iMode,BOOL _bIsWantPlusButton=FALSE,BOOL _bIsSelectAble=FALSE);
	void			setOkButtonText(char *_lpstrText)
	{
		strcpy(m_strOkButtonText,_lpstrText);
	}
	void			setBottomText(char *_lpstrText)
	{
		strcpy(m_strBottomText,_lpstrText);
	}
	void			setTitleText(char *_lpstrText)
	{
		strcpy(m_strTitle,_lpstrText);
	}

	void			setValue(int _iIndex,int _iValue)
	{
		m_aiValue[_iIndex]	=	_iValue;
	}
	bool			addContents(char *_lpstrText,int _iID,int _iValue1=0,int _iValue2=0,int _iValue3=0,int _iValue4=0,int _iValue5=0,int _iValue6=0);
	void			resetButtons();
	void			resetButtonsPos();
					
	void			update();//스킬 인터페이스 업데이트
	void			draw();// 그리기
};

extern	cListWindow	g_gwCommonList;

#define	dBORDER_OF_BOTTOM_SKILL	1000

#endif