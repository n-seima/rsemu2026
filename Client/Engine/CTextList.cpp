#include "CText.H"


//
//	생성자
CTextList::CTextList()
{
	m_pstrText			=	NULL;
	m_pColumnPos		=	NULL;
	m_iMaxColumnCount	=	0;
	m_iExpandValueCount	=	0;
	m_iMaxLineCount		=	0;
	m_iVisibleLineCount	=	0;	//	화면에 보일펯E있는 라인 펯E	
	m_iLineGap			=	0;
	m_rectViewPort.set(0,0,0,0);//	가시 화툈E
	reset();
}

//
//	파괴자
CTextList::~CTextList()
{
	close();
}

//
//	닫콅E
void
CTextList::close()
{
	reset();

	m_iMaxColumnCount	=	0;
	m_iExpandValueCount	=	0;
	m_iLineGap			=	0;
	m_iMaxLineCount		=	0;
	m_iVisibleLineCount	=	0;	//	화면에 보일펯E있는 라인 펯E	
	m_iFocusLine		=	-1;
	m_rectViewPort.set(0,0,0,0);//	가시 화툈E
	pKILL(m_pstrText);
	pKILL(m_pColumnPos);
}

//
//	초기화
void
CTextList::reset()
{
	m_iLineCounter		=	0;
	m_iHeadLine			=	0;
	m_iEndLine			=	0;	//	현픸E맨위에 라인/마지막 라인
	m_iFirstLine		=	0;	//	현픸E뷰포트 가픸E위에 보이는 라인
	m_iDisplayLineCount	=	0;
}

//
//	초기화
void
CTextList::init(cRECT *_lpViewPort,CTextDC *_lpText,int _iLineCount,int _iTextLength)
{
	init(_iLineCount,1,&_iTextLength);

	m_lpText			=	_lpText;

	memcpy(&m_rectViewPort,_lpViewPort,sizeof(m_rectViewPort));

	m_iVisibleLineCount	=	m_rectViewPort.getHeight()/(m_lpText->getFontHeight()+m_iLineGap);
}

//
//	초기화
void
CTextList::initForMultiColumn(cRECT *_lpViewPort,CTextDC *_lpText,int _iLineCount,int _iColumnCount,int *_lpiColumnSize,int _iExpandValueCount)
{
	init(_iLineCount,_iColumnCount,_lpiColumnSize,_iExpandValueCount);

	m_lpText			=	_lpText;

	memcpy(&m_rectViewPort,_lpViewPort,sizeof(m_rectViewPort));

	m_iVisibleLineCount	=	m_rectViewPort.getHeight()/(m_lpText->getFontHeight()+m_iLineGap);
}

//
//	초기화
void
CTextList::init(int _iLineCount,int _iColumnCount,int *_lpiColumnSize,int _iExpandValueCount)
{
	int	i;

	close();

	m_iExpandValueCount	=	_iExpandValueCount;
	m_iMaxColumnCount	=	_iColumnCount;
	m_iMaxLineCount		=	_iLineCount;
	m_pColumnPos		=	new int [_iColumnCount];
	m_iLineLength		=	m_iExpandValueCount*4+4+4;

	for (i=0;i<_iColumnCount;i++)	
	{
		m_pColumnPos[i]	=	m_iLineLength;
		m_iLineLength	+=	_lpiColumnSize[i];
	}

	m_pstrText			=	new char [m_iMaxLineCount*m_iLineLength];

	memset(m_pstrText,0,m_iMaxLineCount*m_iLineLength);
}

//
//	프레임 픸E설정
void			
CTextList::resetFrame(cRECT *_lpRect,int _iCheckColumn)
{
	memcpy(&m_rectViewPort,_lpRect,sizeof(m_rectViewPort));

	m_iVisibleLineCount	=	m_rectViewPort.getHeight()/(m_lpText->getFontHeight()+m_iLineGap);
	m_iDisplayLineCount	=	0;

	if	(_iCheckColumn	!=	-1)
	{
		int	i;
		int	iLine	=	m_iHeadLine;

		for (i=0;i<m_iLineCounter;i++)
		{
			char	*lpString	=	getTextByLine(iLine,_iCheckColumn);
			int		iDLCount	=	m_lpText->getLineCount(m_rectViewPort.getWidth(),lpString);
			m_iDisplayLineCount	+=	iDLCount;

			setDisplayLineCount(iLine,iDLCount);

			iLine++;

			if	(iLine	>=	m_iMaxLineCount)
				iLine	=	0;
		}
	}

	m_iFirstLine	=	m_iDisplayLineCount-m_iVisibleLineCount;
}

//
//	라인카웝訴 증가
void
CTextList::increaseLineCount()
{
	m_iEndLine++;
	m_iLineCounter++;

	if	(m_iEndLine		>=	m_iMaxLineCount)
		m_iEndLine		=	0;

	if	(m_iHeadLine	==	m_iEndLine)
	{
		m_iDisplayLineCount	-=	getDisplayLineCount(m_iHeadLine);
		m_iLineCounter--;

		m_iHeadLine++;

		if	(m_iHeadLine	>=	m_iMaxLineCount)
			m_iHeadLine	=	0;
	}
}

//
//	텍스트 추가
int
CTextList::addColorText(DWORD _dwColor,char *_lpString,BOOL _bIsIncreaseLineCounter)
{
	int	iGab	=	0;

	if	(m_iLineCounter	>	m_iVisibleLineCount)
		iGab	=	m_iDisplayLineCount-m_iVisibleLineCount-m_iFirstLine;

	addText(_lpString);

	int		iDLCount	=	m_lpText->getLineCount(m_rectViewPort.getWidth(),_lpString);

	setDisplayLineCount(m_iLastestLine,iDLCount);
	setColor(m_iLastestLine,_dwColor);

	m_iDisplayLineCount	+=	iDLCount;

	if	(_bIsIncreaseLineCounter)
		increaseLineCount();

	if	(m_iLineCounter	>	m_iVisibleLineCount)
	{
		if	(iGab	<=	0)
			m_iFirstLine	=	m_iDisplayLineCount-m_iVisibleLineCount-iGab;
	}
	else
		m_iFirstLine	=	0;

	return	m_iLastestLine;
}

//
//	텍스트 추가
int
CTextList::addText(char *_lpString,BOOL _bIsIncreaseLineCounter)
{
	int	iGab	=	0;

	if	(m_iLineCounter	>	m_iVisibleLineCount)
		iGab	=	m_iDisplayLineCount-m_iVisibleLineCount-m_iFirstLine;

	m_iLastestLine		=	m_iEndLine;

	char	*lpString	=	getTextByLine(m_iEndLine);

	strcpy(lpString,_lpString);

	if	(_bIsIncreaseLineCounter)
		increaseLineCount();

	if	(m_iLineCounter	>	m_iVisibleLineCount)
	{
		if	(iGab	<=	0)
			m_iFirstLine	=	m_iDisplayLineCount-m_iVisibleLineCount-iGab;
	}
	else
		m_iFirstLine	=	0;

	return	m_iLastestLine;
}

//
//	텍스트 추가
int
CTextList::addText(int _iColumn,char *_lpString,BOOL _bIsIncreaseLineCounter)
{
	int	iGab	=	0;

	m_iLastestLine		=	m_iEndLine;

	char	*lpString	=	getTextByLine(m_iEndLine,_iColumn);

	strcpy(lpString,_lpString);

	if	(m_iLineCounter	>	m_iVisibleLineCount)
		iGab	=	m_iDisplayLineCount-m_iVisibleLineCount-m_iFirstLine;

	if	(_bIsIncreaseLineCounter)
		increaseLineCount();

	if	(m_iLineCounter	>	m_iVisibleLineCount)
	{
		if	(iGab	<=	0)
			m_iFirstLine	=	m_iDisplayLineCount-m_iVisibleLineCount-iGab;
	}
	else
		m_iFirstLine	=	0;

	return	m_iLastestLine;
}

void
CTextList::setRefitPos(int _iDX,int _iDY)
{
	m_iDX	=	_iDX;
	m_iDY	=	_iDY;
}

//
//	텍스트 그려주콅E
void
CTextList::drawText()
{
	int	i;
	int	iLine		=	m_iHeadLine;
	int	iDLCounter	=	0;
	int	iSkipLine	=	0;

	for (i=0;i<m_iLineCounter;i++)
	{
		int	iCurDLCount	=	getDisplayLineCount(iLine);

		if	(m_iFirstLine	>=	iDLCounter	&&	m_iFirstLine	<	iDLCounter+iCurDLCount)
		{
			iSkipLine	=	abs(iDLCounter-m_iFirstLine);
			break;
		}

		iDLCounter		+=	iCurDLCount;
		iLine++;

		if	(iLine	>=	m_iMaxLineCount)
			iLine	=	0;
	}

	int	x				=	m_rectViewPort.x1+m_iDX;
	int	y				=	m_rectViewPort.y1+m_iDY;
	int	iPutLineCounter	=	0;

	for (i=0;i<m_iVisibleLineCount	&&	i<m_iDisplayLineCount;)
	{
		char	*lpString		=	getTextByLine(iLine);
		char	strOutputText[256];
		DWORD	dwColor			=	getColor(iLine);
		int		iCurLineCount	=	getDisplayLineCount(iLine),iTextPoint=0;

		if	(iCurLineCount	>	1)
		{
			while(lpString[iTextPoint] && i<m_iVisibleLineCount)
			{
				int		iTextWidth	=	0;

				iTextPoint			=	m_lpText->getOneLine(m_rectViewPort.getWidth(),lpString,iTextPoint,strOutputText,TRUE,&iTextWidth);

				if	(iSkipLine)
				{
					iSkipLine--;
					continue;
				}

				i++;
				if	(iLine	==	m_iFocusLine)
					m_lpText->putReverse(x,y,dwColor,strOutputText);
				else
					m_lpText->put(x,y,dwColor,strOutputText);
				y	+=	m_lpText->m_iFontHeight+m_iLineGap;
			}
		}
		else
		{
			if	(iLine	==	m_iFocusLine)
				m_lpText->putReverse(x,y,dwColor,lpString);
			else
				m_lpText->put(x,y,dwColor,lpString);

			y	+=	m_lpText->m_iFontHeight+m_iLineGap;
			i	++;
		}

		iLine++;

		if	(iLine	>=	m_iMaxLineCount)
			iLine	=	0;
	}
}

//
//	줄간격 설정
void
CTextList::setLineGap(int _iLineGap)
{
	m_iLineGap			=	_iLineGap;
	m_iVisibleLineCount	=	m_rectViewPort.getHeight()/(m_lpText->getFontHeight()+_iLineGap);
}

//	높이 구하콅E
int	
CTextList::getHeight()
{
	int	iLineCount	=	m_iVisibleLineCount;

	iLineCount		=	min(iLineCount,m_iDisplayLineCount);

	return	iLineCount*(m_lpText->getFontHeight()+m_iLineGap);
}

//
//	라인 카웝獸를 줄인다.
void
CTextList::decreaseLineCount()
{
	if	(m_iLineCounter	<=	0)
		return;

	m_iDisplayLineCount-=	getDisplayLineCount(m_iHeadLine);
	m_iLineCounter--;

	if	(m_iFirstLine)
		m_iFirstLine--;

	m_iHeadLine++;

	if	(m_iHeadLine	>=	m_iMaxLineCount)
		m_iHeadLine	=	0;
}

//
//	보이는 텍스트중 가픸E콅E문장의 사이햨E
int	
CTextList::getMaxLengthInViewText()
{
	int	i;
	int	iLine		=	m_iHeadLine;
	int	iDLCounter	=	0;
	int	iSkipLine	=	0,iMaxSize	=	0;

	for (i=0;i<m_iLineCounter;i++)
	{
		int	iCurDLCount	=	getDisplayLineCount(iLine);

		if	(m_iFirstLine	>=	iDLCounter	&&	m_iFirstLine	<	iDLCounter+iCurDLCount)
		{
			iSkipLine	=	abs(iDLCounter-m_iFirstLine);
			break;
		}

		iDLCounter		+=	iCurDLCount;
		iLine++;

		if	(iLine	>=	m_iMaxLineCount)
			iLine	=	0;
	}

	int	iPutLineCounter	=	0;

	for (i=0;i<m_iVisibleLineCount	&&	i<m_iDisplayLineCount;)
	{
		char	*lpString		=	getTextByLine(iLine);
		char	strOutputText[256];
		int		iCurLineCount	=	getDisplayLineCount(iLine),iTextPoint=0;

		if	(iCurLineCount	>	1)
		{
			while(lpString[iTextPoint] && i<m_iVisibleLineCount)
			{
				int		iTextWidth	=	0;

				iTextPoint	=	m_lpText->getOneLine(m_rectViewPort.getWidth(),lpString,iTextPoint,strOutputText,TRUE,&iTextWidth);
				iMaxSize	=	max(m_lpText->getPixelSize(strOutputText),iMaxSize);

				if	(iSkipLine)
				{
					iSkipLine--;
					continue;
				}

				i++;
			}
		}
		else
		{
			i	++;
			iMaxSize	=	max(m_lpText->getPixelSize(lpString),iMaxSize);
		}

		iLine++;

		if	(iLine	>=	m_iMaxLineCount)
			iLine	=	0;
	}

	return	iMaxSize;
}

//
//	한줄 리턴~
char*
CTextList::getText(int _iIndex,int _iColumn)
{
	if	(_iIndex >= m_iLineCounter)
		return	NULL;

	int	iLine	=	_iIndex+m_iHeadLine;

	if	(iLine	>=	m_iMaxLineCount)
		iLine	-=	m_iMaxLineCount;

	return	getTextByLine(iLine,_iColumn);
}

char*
CTextList::getText(char *_lpstrText,int _iColumn)
{
	return	_lpstrText	+m_pColumnPos[_iColumn];
}

char*
CTextList::getLastestText(int _iColumn)	//	마지막 문픸E리턴
{
	return	getText(m_iLastestLine,_iColumn);
}

int
CTextList::getValue(char *_lpstrText,int _iColumn)
{
	return	*(int *)(_lpstrText+8+_iColumn*4);
}

DWORD
CTextList::getColor(char *_lpstrText)
{
	return	*(DWORD *)(_lpstrText);
}

void
CTextList::setFirstLine(int _iLine)	//	첫번째 라인 설정
{
	if	(m_iDisplayLineCount	<=	m_iVisibleLineCount)
		m_iFirstLine	=	0;
	else
	{
		if	(_iLine	>	m_iDisplayLineCount-m_iVisibleLineCount)
			_iLine	=	m_iDisplayLineCount-m_iVisibleLineCount;

		if	(_iLine	<	0)
			_iLine	=	0;

		m_iFirstLine	=	_iLine;
	}
}

int
CTextList::getFocusLine(int _iX,int _iY)
{
	if	(!m_rectViewPort.isIn(_iX-m_iDX,_iY-m_iDY))
		return	-1;

	int	i;
	int	iLine		=	m_iHeadLine;
	int	iDLCounter	=	0;
	int	iSkipLine	=	0;

	for (i=0;i<m_iLineCounter;i++)
	{
		int	iCurDLCount	=	getDisplayLineCount(iLine);

		if	(m_iFirstLine	>=	iDLCounter	&&	m_iFirstLine	<	iDLCounter+iCurDLCount)
		{
			iSkipLine	=	abs(iDLCounter-m_iFirstLine);
			break;
		}

		iDLCounter		+=	iCurDLCount;
		iLine++;

		if	(iLine	>=	m_iMaxLineCount)
			iLine	=	0;
	}

	int	x				=	m_rectViewPort.x1+m_iDX;
	int	y				=	m_rectViewPort.y1+m_iDY;
	int	iPutLineCounter	=	0;
	int	iLineHeight		=	max(1,m_lpText->m_iFontHeight+m_iLineGap);

	for (i=0;i<m_iVisibleLineCount	&&	i<m_iDisplayLineCount;)
	{
		int		iCurLineCount	=	getDisplayLineCount(iLine),iTextPoint=0;

		if	(iCurLineCount	>	1)
		{
			iCurLineCount	-=	iSkipLine;
			iSkipLine		=	0;

			if	(_iY		>=	y	&&	_iY	<=	y+iCurLineCount*iLineHeight)
				return	iLine;

			i	+=	iCurLineCount;
			y	+=	iCurLineCount*iLineHeight;
		}
		else
		{
			if	(_iY	>=	y	&&	_iY	<=	y+iLineHeight)
				return	iLine;

			i	++;
			y	+=	iLineHeight;
		}

		iLine++;

		if	(iLine	>=	m_iMaxLineCount)
			iLine	=	0;
	}

	return	-1;
}

int
CTextList::getFocusedLine()
{
	return	m_iFocusLine;
}

void
CTextList::setFocusLine(int _iLine)
{
	m_iFocusLine	=	_iLine;
}

char*
CTextList::getNextText(int _iColumn)
{
	int	iLine	=	m_iCurrentText;

	if	(m_iCurrentText	!=	m_iEndLine)
		m_iCurrentText++;

	if	(m_iCurrentText	>=	m_iMaxLineCount)
		m_iCurrentText	=	0;

	return	getTextByLine(iLine,_iColumn);
}

char*
CTextList::getPreviousText(int _iColumn)
{
	int	iLine	=	m_iCurrentText;

	if	(m_iCurrentText	!=	m_iHeadLine)
		m_iCurrentText--;

	if	(m_iCurrentText	<	0)
		m_iCurrentText	=	m_iMaxLineCount-1;

	return	getTextByLine(iLine,_iColumn);
}

void
CTextList::setCurrentTextLineToLastestLine()
{
	m_iCurrentText	=	m_iLastestLine;
}