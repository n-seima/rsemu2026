//	http://topzero.wo.to 
#include "cLISTCONTROL.H"
#include "cDRAW.H"
#include "cTEXT.H"


int			cLISTCONTROL::s_iCount;

//
//	리스트 컨트롤 아이템 설정
void
cLISTCONTROL_COLUMN::Set(char *name,int left,int width,int _iOutputMethod)
{
	int	size		=	width/6-1;

	ustrncpy(m_strTitle,name,min(sizeof(m_strTitle),size));
	m_iWidth		=	width;
	m_iLeft			=	left;
	m_wOutputMethod	=	_iOutputMethod;
}

//
//	index를 삭제한다.
BOOL
cLISTCONTROL_COLUMN::RemoveItem(int index)
{
	return	m_Strings.Remove(index);
}

//
//	리스트에 아이템들을 초기화한다.
void
cLISTCONTROL_COLUMN::ResetItem()
{
	m_Strings.Reset();
}

//
//	리스트 컨트롤 초기화
BOOL
cLISTCONTROL::Init(HWND hwnd,int count,BOOL bMulti,LRESULT (CALLBACK *lpProc)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam))
{
	Reset();

	m_hWnd						=	hwnd;
	m_pColumns					=	new cLISTCONTROL_COLUMN [count];
	m_iMaxColumn				=	count;
	m_isMultiSelect				=	bMulti;

	m_ScrollBar.Init(hwnd);
	m_ScrollBar.InitHeight();

	m_Dib.lpszRegister			=	_ms("List control No %d",s_iCount);
	m_Dib.Init		(hwnd		,(WNDPROC)s_Proc);
	SetWindowLong	(m_Dib.hWND	,GWL_USERDATA,(LONG)this);

	m_iListHeight				=	15;

	fnProc						=	lpProc;
	s_iCount++;

	return	TRUE;
}


//
//	리스트 간의 높이를 설정한다.
void
cLISTCONTROL::SetItemHeight(int _iHeight)
{
	m_iListHeight				=	_iHeight;
}

//
//	리스트 초기화
void
cLISTCONTROL::Reset()
{
	m_iCount		=	0;
	m_iMaxColumn	=	0;
	m_iLeft			=	2;
	m_iSelect		=	0xffff;
	m_iScrollPos	=	0;
	m_iLineCounter	=	0;
	m_iTitleHeight	=	24;
	m_iSortColumn	=	0xffff;
	memset(m_aSelected,0,sizeof(m_aSelected));

	pKILL(m_pColumns);
}

//
//	리스트 초기화
void
cLISTCONTROL::ResetItems()
{
	m_iSelect		=	0xffff;
	m_iScrollPos	=	0;
	m_iLineCounter	=	0;

	memset(m_aSelected,0,sizeof(m_aSelected));
	m_ScrollBar.SetHeightPos(0);

	for (int i=0;i<m_iCount;i++)
		m_pColumns[i].ResetItem();
}

//
//	칼럼 추가
BOOL
cLISTCONTROL::AddColumn(char *name,int width,int _iOutputMethod)
{
	if (m_iCount	>=	m_iMaxColumn	)
		return	FALSE;

	m_pColumns[m_iCount++].Set(name,m_iLeft,width,_iOutputMethod);
	m_iLeft	+=	width;

	return	TRUE;
}

//
//	아이템 추가
BOOL
cLISTCONTROL::AddItem(int index,char *str)
{
	if (index >= m_iCount)	return FALSE;

	m_pColumns[index].Add(str);

	if (m_iSortColumn	!=	0xffff)	Sort(m_iSortColumn);

	return	TRUE;
}

//
//	아이템 추가
BOOL
cLISTCONTROL::AddItem(int index,char *str,int data)
{
	if (index >= m_iCount)	return FALSE;

	m_pColumns[index].Add(str,data);

	if (m_iSortColumn	!=	0xffff)	
		Sort(m_iSortColumn);

	return	TRUE;
}

//
//	라인 카운터 증가
void
cLISTCONTROL::IncreaseLineCounter()
{
	m_iLineCounter++;
}


//
//	세로 스크롤
BOOL
cLISTCONTROL::VScroll(WORD scroll)
{
	return	m_ScrollBar.VScroll(scroll);
}

//
//	선택
BOOL
cLISTCONTROL::Select(int x,int y)
{
	cINPDEV::Update();
	int	ypos,j,i;

	int	iTitleHeight	=	6;

	if (m_isViewColumn)	
		iTitleHeight	=	m_iTitleHeight;
	
	ypos	=	iTitleHeight;

	if (m_isViewColumn)
		if (y > 0 && y < m_iTitleHeight)
			for (j	=	0;j<m_iCount;j++)
			{
				if (x < m_pColumns[j].m_iLeft +m_pColumns[j].m_iWidth)	//	컬럼 타이틀을 선택했다.
				{
					if (m_isSort)
					{	Sort(j);		//	해당 컬럼을 이용해 소트하자.

						return	2;
					}

					return	FALSE;
				}
			}


	for (j	=	m_ScrollBar.GetHeightPos();j<m_iLineCounter;j++,ypos+=m_iListHeight)
	{
		if (ypos+m_iListHeight >= m_Dib.height())	break;

		if (y >= ypos  && y <= ypos +m_iListHeight)
		{
			if (m_isMultiSelect)
			{
				if (KEY[dKEY_SHIFT] && m_iSelect	!=	0xffff)
				{
					if (!KEY[dKEY_CONTROL])	memset(m_aSelected,0,sizeof(m_aSelected));

					int	bl		=	min(j,m_iSelect);
					int	el		=	max(j,m_iSelect);

					for (i=bl;i<=el;i++)	m_aSelected[i]	=	TRUE;
				}
				else
				if (KEY[dKEY_CONTROL])
				{
					m_aSelected[j]	=	1-m_aSelected[j];
					m_iSelect		=	j;
				}
				else
				{
					memset(m_aSelected,0,sizeof(m_aSelected));
					m_iSelect		=	j;
					m_aSelected[j]	=	TRUE;
				}
			}
			else
			{
				memset(m_aSelected,0,sizeof(m_aSelected));
				m_iSelect		=	j;
				m_aSelected[j]	=	TRUE;
			}
			

			return	TRUE;
		}
	}

	return	FALSE;
}

//
//	선택
BOOL
cLISTCONTROL::Select(int _iIndex)
{
	if (_iIndex	>=	m_iLineCounter	||	_iIndex	<	0)	return	FALSE;

	memset(m_aSelected,0,sizeof(m_aSelected));
	m_iSelect			=	_iIndex;
	m_aSelected[_iIndex]=	TRUE;

	return	TRUE;
}

//
//	특정 컬럼에 _strString를 선택한다.
BOOL
cLISTCONTROL::select(int _iColumn,char *_strString)
{
	cSTRINGS	*lpStrings	=	&m_pColumns[_iColumn].m_Strings;
	cSTRING		*lpString	=	lpStrings->Pick();

	int	iIndex	=	lpStrings->GetIndex(_strString);

	if (iIndex	!=	-1)
	{
		memset(m_aSelected,0,sizeof(m_aSelected));
		m_iSelect				=	iIndex;
		m_aSelected[m_iSelect]	=	TRUE;

		return	TRUE;
	}

	return	FALSE;
}

//
//	_iColumn의 데이터를 비교해서 선택한다.
BOOL
cLISTCONTROL::selectByData(int _iColumn,int _iData)
{
	cSTRINGS	*lpStrings	=	&m_pColumns[_iColumn].m_Strings;
	cSTRING		*lpString	=	lpStrings->Pick();

	for (int i=0;i<m_iLineCounter;i++)
	{
		if (lpString->m_iValue	==	_iData)
		{
			memset(m_aSelected,0,sizeof(m_aSelected));
			m_iSelect				=	i;
			m_aSelected[m_iSelect]	=	TRUE;

			return	TRUE;
		}

		lpString	=	lpString->pNEXT;
	}

	return	FALSE;
}

//
//	column에 index번째 항목의 이름을 구함
char*
cLISTCONTROL::GetStr(int column,int line)
{
	if (line		>=	m_iLineCounter	)	return	NULL;
	if (column		>=	m_iCount		)	return	NULL;

	return	m_pColumns[column].m_Strings.GetStr(line);
}

//
//	column에 index번째 항목의 데이터 값을 구함
//
int
cLISTCONTROL::GetData(int column,int line)
{
	if (line		>=	m_iLineCounter	)	return	0x7fffffff;
	if (column		>=	m_iCount		)	return	0x7fffffff;

	return	m_pColumns[column].m_Strings.GetData(line);
}

int
cLISTCONTROL::GetData(int line)
{
	if (line		>=	m_iLineCounter	)	return	NULL;

	return	m_pColumns[0].m_Strings.GetData(line);
}

//
//	column에 선택한 항목의 이름을 구함
//
char*
cLISTCONTROL::GetSelectStr(int column)
{
	if (m_iSelect	>=	m_iLineCounter	)
		return	NULL;
	if (column		>=	m_iCount		)
		return	NULL;

	return	m_pColumns[column].m_Strings.GetStr(m_iSelect);
}

//
//	column에 선택한 항목의 data를 구함
int
cLISTCONTROL::GetSelectData(int column)
{
	if (m_iSelect	>=	m_iLineCounter	)
		return	0x7fffffff;
	if (column		>=	m_iCount		)
		return	0x7fffffff;

	return	m_pColumns[column].m_Strings.GetData(m_iSelect);
}

//
//	선택된 라인
int
cLISTCONTROL::GetSelectLine(int column)
{
	if (m_iSelect	>=	m_iLineCounter	)	return	0x7fffffff;
	if (column		>=	m_iCount		)	return	0x7fffffff;

	return	m_iSelect;
}

//
//	그려주기
void
cLISTCONTROL::Draw()
{
	if (!m_Dib.Active())
		return;

	cRECT	Rect;

	Rect.Set(m_hWnd);
	int	iWidth	=	Rect.width();

	cDRAW::Fill(RGB24To16(165,203,247));

	int	iTitleHeight	=	6;
	int	y;

	if (m_isViewColumn)		//	헤드를 그려준다.
	{
		cDRAW::Fill(RGB24To16(214,211,206)	,1			,1					,iWidth		,m_iTitleHeight-7);

		cDRAW::Fill(_WHITE					,0			,0					,iWidth		,0);
		cDRAW::Fill(_WHITE					,0			,0					,0			,m_iTitleHeight-7);

		cDRAW::Fill(_BLACK					,0			,m_iTitleHeight-7	,iWidth		,m_iTitleHeight-7);
		cDRAW::Fill(RGB24To16(123,125,123)	,1			,m_iTitleHeight-8	,iWidth-3	,m_iTitleHeight-8);
		cDRAW::Fill(RGB24To16(123,125,123)	,iWidth-3	,1					,iWidth-3	,m_iTitleHeight-8);

		iTitleHeight	=	m_iTitleHeight;
	}

	m_ScrollBar.SetHeight(m_iLineCounter,(cDRAW::HEIGHT-iTitleHeight)/m_iListHeight);

	cDRAW::Box(0,-2,-2,iWidth-2,cDRAW::HEIGHT+1);

	cTEXT::PushShadow(FALSE);

	y	=	iTitleHeight;

	//	선택된 넘들 표시
	for (int j=m_ScrollBar.GetHeightPos();j<m_iLineCounter;j++,y+=m_iListHeight)
	{
		if	(y+m_iListHeight > cDRAW::HEIGHT)
			break;

		if (m_aSelected[j])
		{
			cDRAW::Fill	(_BLACK		,4  ,y-2,iWidth-4  ,y+m_iListHeight-2);
			cDRAW::Fill	(_BLUE	,2  ,y-4,iWidth-6  ,y+m_iListHeight-4);
		}
	}

	if (m_iLineCounter	==	0)
		cTEXT::Put(6,iTitleHeight	,0	,"Valid list");

	for (int i	=	0;i<m_iCount;i++)
	{
		int	x	=	m_pColumns[i].m_iLeft;

		int	y	=	iTitleHeight;

		cSTRINGS	*lpStrings	=	&m_pColumns[i].m_Strings;

		for (int j=m_ScrollBar.GetHeightPos();j<lpStrings->Count;j++,y+=m_iListHeight)
		{
			if	(y+m_iListHeight > cDRAW::HEIGHT)
				break;

			if	(m_aSelected[j])
			{
				if	(m_aSpecialLine[j])
				{
					cTEXT::SetBoldFont();

					if (m_pColumns[i].m_wOutputMethod	==	eLCOM_CENTER)
						cTEXT::cPut(x+m_pColumns[i].m_iWidth/2,y-1		,LTYELLOW	,lpStrings->GetStr(j));
					else
						cTEXT::Put(x+5,y-1		,LTYELLOW	,lpStrings->GetStr(j));

					cTEXT::SetNormalFont();
				}
				else
				{
					if (m_pColumns[i].m_wOutputMethod	==	eLCOM_CENTER)
						cTEXT::cPut(x+m_pColumns[i].m_iWidth/2,y-1		,LTYELLOW	,lpStrings->GetStr(j));
					else
						cTEXT::Put(x+5,y-1	,LTYELLOW	,lpStrings->GetStr(j));
				}
			}
			else
			{
				if	(m_aSpecialLine[j])
				{
					cTEXT::SetBoldFont();

					if (m_pColumns[i].m_wOutputMethod	==	eLCOM_CENTER)
						cTEXT::cPut(x+m_pColumns[i].m_iWidth/2,y		,0			,lpStrings->GetStr(j));
					else
						cTEXT::Put(x+6,y		,0			,lpStrings->GetStr(j));

					cTEXT::SetNormalFont();
				}
				else
				{
					if	(m_pColumns[i].m_wOutputMethod	==	eLCOM_CENTER)
						cTEXT::cPut(x+m_pColumns[i].m_iWidth/2,y	,0			,lpStrings->GetStr(j));
					else
						cTEXT::Put(x+6,y	,0			,lpStrings->GetStr(j));
				}

			}
		}

		if (m_isViewColumn)		//	헤드
		{
			if (m_pColumns[i].m_wOutputMethod	==	eLCOM_CENTER)
				cTEXT::cPut(x+m_pColumns[i].m_iWidth/2,3,0,m_pColumns[i].m_strTitle);
			else
				cTEXT::Put(x+6,3,0,m_pColumns[i].m_strTitle);

			x		+=	m_pColumns[i].m_iWidth;

			cDRAW::Fill(RGB24To16(123,125,123)	,x-1,1,x-1,m_iTitleHeight - 8);
			cDRAW::Fill(_BLACK					,x  ,0,x  ,m_iTitleHeight - 7);
			cDRAW::Fill(_WHITE					,x+1,1,x+1,m_iTitleHeight - 8);
		}
		else
			x	+=	m_pColumns[i].m_iWidth;
	}

	cTEXT::PopShadow();

	m_Dib.Draw();
}

//
//	인덱스 한줄을 지워버린다.
//
BOOL
cLISTCONTROL::RemoveItem(int index)
{
	if (index >= m_iLineCounter)	return	FALSE;

	for (int i	=	0;i<m_iCount;i++)	m_pColumns[i].RemoveItem(index);

	m_iLineCounter--;

	if (m_iSelect	>=	m_iLineCounter)	m_iSelect	=	m_iLineCounter	-	1;

	
	m_ScrollBar.SetHeight(m_iLineCounter,(m_Dib.height()	-	m_iTitleHeight)/m_iListHeight);

	return	TRUE;
}

BOOL
cLISTCONTROL::RemoveSelectItem()
{
	return	RemoveItem(m_iSelect);
}

//
//	column을 기준으로 소트한다.
//
void
cLISTCONTROL::Sort(int column)					//	column 항목을 기본으로 소트한다.
{
	if (column	>=	m_iCount)	return;

	if	(m_iLineCounter	<=	0)
		return;

	int	i,j;

	cSTRINGS	*lpSortStrings	=	&m_pColumns[column].m_Strings;

	lpSortStrings->Sort();

	for (i=0;i<m_iCount;i++)
	{
		if	(i	==	column)
			continue;

		cSTRINGS	Strings;
		cSTRINGS	*lpStrings	=	&m_pColumns[i].m_Strings;

		cSTRING		*str		=	lpStrings->Pick(0);

		for (j=0;j<lpStrings->Count;j++)
		{
			Strings.Add(str);

			str	=	str->pNEXT;
		}

		lpStrings->Reset();

		str		=	lpSortStrings->Pick(0);

		for (j=0;j<lpSortStrings->Count;j++)
		{
			cSTRING	*lpStr		=	Strings.Pick(str->m_iSerial);

			lpStrings->Add(lpStr);

			if	(j	==	105)
				j	=	j;

			str	=	str->pNEXT;
		}
	}
}

//
//	column의 선택한 인덱스의 문자열을 strName으로 변경한다.
//
void
cLISTCONTROL::SetSelectItemStr(int column,char *strName)
{
	if (m_iSelect	>=	m_iLineCounter)	return;

	cSTRING	*str	=	m_pColumns[column].m_Strings.Pick(m_iSelect);

	str->Set(strName);
}

//
//	column의 index의 문자열을 strName으로 변경한다.
//
void
cLISTCONTROL::SetItemStr(int column,int index,char *strName)
{
	cSTRING	*str	=	m_pColumns[column].m_Strings.Pick(index);

	str->Set(strName);
}


LRESULT CALLBACK
cLISTCONTROL::Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			m_Dib.Active();
			m_Dib.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		case WM_LBUTTONDOWN		:
		{
			if(Select(LOWORD(lParam),HIWORD(lParam)))	Draw();

			break;
		}

		case WM_VSCROLL			:
		{
			if (VScroll(wParam))	Draw();

			break;
		}

		default					:
		{
			if (fnProc)	( fnProc( hwnd, uMsg, wParam, lParam ) );

			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
		}
	}

	if (fnProc)	fnProc( hwnd, uMsg, wParam, lParam);

	return (0L);
}

LRESULT		CALLBACK
cLISTCONTROL::s_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	cWND_BASE	*lpWndBase	=	(cWND_BASE *)GetWindowLong(hwnd,GWL_USERDATA);

	return	lpWndBase->Proc(hwnd,uMsg,wParam,lParam);
}