/*************************************************************************************
  개			  요 :  윈도우 컨트롤을 이용하지 않고 레포트 타입의 리스트 컨트롤 구현

  제한			사항 :	알아서 써.
						
  필요	  라이브러리 :	몰라.

  작성			날짜 :	2001.06.09

  기              타 :  귀찮아서 완전히 독립적으로 만들지 못했다. 완전히 독립적으로 하
						려면 cDIBWND클래스까지 붙여야야 한다.
						하지만... 완전히 독립적으로 해 버리면 다른면에서 제한적이 되어
						버린다.... 알아서 하자. --

*************************************************************************************/

#ifndef _classLISTCONTROL_H
#define	_classLISTCONTROL_H

#include "SFC.H"
#include "cWINDOW.H"

#define	dMAX_LC_LINE		1024		//	리스트 콘트롤 최대 라인수
#define	dMAX_LIST_CONTROL	20

enum
{
	eLCOM_NORMAL,
	eLCOM_CENTER,
};

class	cLISTCONTROL_COLUMN
{
public:
	int			m_iWidth,m_iLeft;
	WORD		m_wOutputMethod;
	char		m_strTitle[32];
	cSTRINGS	m_Strings;

	void		Set(char *name,int left,int width,int _iOutputMethod);
	void		Add(char *str)
				{
					m_Strings.Add(str);
				}

	void		Add(char *str,int data)
				{
					m_Strings.AddAndData(str,data);
				}

	BOOL		RemoveItem(int index);
	void		ResetItem();
};

//	리스트 컨트롤
class	cLISTCONTROL	:	cWND_BASE
{
public:

	HWND				m_hWnd;		//	컨트롤의 윈도우 핸들
	int					m_iMaxColumn,m_iCount,m_iLeft,m_iSelect,m_iScrollPos,m_iLineCounter;
	int					m_iTitleHeight,m_iSortColumn;
	BOOL				m_isSort,			//	정렬
						m_isViewColumn,		//	컬럼이 보이냐?
						m_isMultiSelect,		//	여러개 선택 할 수 있다.
						m_isImpactLine;		//	특정 조건이 만족하는 라인을 강조 시킨다.

	BYTE				m_aSelected[dMAX_LC_LINE];
	BYTE				m_aSpecialLine[dMAX_LC_LINE];
	int					m_iListHeight;

	cLISTCONTROL_COLUMN	*m_pColumns;
	cSCROLL_BAR			m_ScrollBar;
	cDIBWND				m_Dib;

						cLISTCONTROL()
						{
							m_iListHeight	=	16;
							m_pColumns		=	NULL;
							m_iMaxColumn	=	0;
							m_isSort		=	FALSE;
							m_isViewColumn	=	TRUE;
							m_iSortColumn	=	0xffff;
							m_isMultiSelect	=	FALSE;
							memset(m_aSelected,0,sizeof(m_aSelected));
							memset(m_aSpecialLine,0,sizeof(m_aSpecialLine));
							
						}

						~cLISTCONTROL()
						{
							pKILL(m_pColumns);
							m_iMaxColumn	=	0;
						}

	BOOL				Init(HWND hwnd,int count,BOOL bMulti=FALSE,LRESULT (CALLBACK *lpProc)(HWND, UINT, WPARAM, LPARAM)=NULL);
	inline	void		setSpecialLine(int _iLine,BOOL _isSet){m_aSpecialLine[_iLine]	=	_isSet;}	//	스페셜라인 표시

	BOOL				AddColumn(char *name,int width,int _iOutputMethod=eLCOM_NORMAL);
	BOOL				AddItem(int index,char *str);
	BOOL				AddItem(int index,char *str,int data);
	void				IncreaseLineCounter();
	BOOL				VScroll(WORD scroll);
	BOOL				Select(int x,int y);
	BOOL				Select(int _iIndex);
	BOOL				select(int _iColumn,char *_strString);
	BOOL				selectByData(int _iColumn,int _iData);
	void				SetItemHeight(int _iHeight);

	char				*GetStr(int column,int line);
	int					GetData(int column,int line);
	int					GetData(int line);
	char				*GetSelectStr(int column=0);
	int					GetSelectData(int column=0);		//	잘못 되면 0x7fffffff
	int					GetSelectLine(int column=0);		//	잘못 되면 0x7fffffff

	void				SetSelectItemStr(int clolumn,char *strName);
	void				SetItemStr(int clolumn,int line,char *strName);

	BOOL				RemoveItem(int line);				//	인덱스 한줄을 지워버린다.
	void				ResetItems();
	BOOL				RemoveSelectItem();
	void				Sort(int column=0);					//	column 항목을 기본으로 소트한다.
	void				SetSort(BOOL sort=TRUE)	{m_isSort=sort;}
	void				SetSortColumn(int column = 0xffff)
						{
							if (column	==	0xffff)	return;

							if (column	>=	m_iCount)	column	=	0xffff;

							m_iSortColumn	=	column;

						}

	void				HideColumn()	{m_isViewColumn	=	FALSE;}
	void				ShowColumn()	{m_isViewColumn	=	TRUE;}

	inline	void		setHeightPos(int _iPos)	{m_ScrollBar.SetHeightPos(_iPos);}
	inline	int			getHeightPos()			{return	m_ScrollBar.GetHeightPos();}


	void				Reset();
	void				Draw();
	LRESULT				(CALLBACK *fnProc)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT CALLBACK	Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static	int			s_iCount;

	static	LRESULT		CALLBACK s_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//	static	LRESULT CALLBACK	s_aProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif