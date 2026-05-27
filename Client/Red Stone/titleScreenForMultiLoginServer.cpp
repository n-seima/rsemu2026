#include "cGAME_TITLE.H"

int		cGAME_TITLE::s_iLoginServerLineCount	=	0;
int		cGAME_TITLE::s_iCurrentLoginServer		=	0;
int		cGAME_TITLE::s_iFocusLoginServer		=	0;
cRECT	cGAME_TITLE::s_arectList[100];

#define	dLOGIN_SERVER_LIST_TOP_HEIGHT			6

enum
{
	eCSWI_CARROT	=	33,
	eCSWI_TITLE		=	67,
	eCSWI_FIRST_BAR	=	72,
};

void
cGAME_TITLE::DrawLoginServerList()
{
	if	(s_iLoginServerLocalCount	<=	1)
	{
		s_iLoginServerLineCount	=	0;
		return;
	}

	int		i;

	int		iLeftMargin			=	10;
	int		iWindowWidth		=	s_rectServerListBorder.getWidth()-6;
	int		iBarHeight			=	g_sprInterface2.getSpriteHeight(eCSWI_FIRST_BAR);
	int		iBarWidth			=	g_sprInterface2.getSpriteWidth(eCSWI_FIRST_BAR);
	int		iX					=	iLeftMargin+iBarWidth;
	int		iY					=	s_rectServerListBorder.y1+dLOGIN_SERVER_LIST_TOP_HEIGHT;
	int		aiMarginWidth[100];
	int		iBarPerLine			=	0;
	int		iCategoryLineCount	=	0;

	memset(aiMarginWidth,0,sizeof(aiMarginWidth));

	s_iLoginServerLineCount	=	1;

	for	(i=0;i<s_iLoginServerLocalCount;i++)
	{
		int	iWidth	=	s_text.getPixelSize(s_aLoginServerIPList[i].m_strLocalName);

		if	(iX	+	iWidth+iLeftMargin	>=	iWindowWidth)
		{
			aiMarginWidth[iCategoryLineCount]	=	(iWindowWidth-iX)/iBarPerLine;
			iX					=	iLeftMargin+iBarWidth;
			iY					+=	iBarHeight+1;
			iCategoryLineCount++;
			iBarPerLine			=	0;
		}

		iX		+=		iWidth+iBarWidth*2+1;

		iBarPerLine++;
	}

	if	(iCategoryLineCount	==	0)
		aiMarginWidth[0]	=	(iWindowWidth-iX)/iBarPerLine;


	s_iLoginServerLineCount	=	1;
	iCategoryLineCount		=	0;
	iX						=	iLeftMargin+iBarWidth;
	iY						=	s_rectServerListBorder.y1+dLOGIN_SERVER_LIST_TOP_HEIGHT;

	for	(i=0;i<s_iLoginServerLocalCount;i++)
	{
		int	iWidth	=	s_text.getPixelSize(s_aLoginServerIPList[i].m_strLocalName);

		if	(iX	+	iWidth+iLeftMargin	>=	iWindowWidth)
		{
			iX	=	iLeftMargin+iBarWidth;
			iY	+=	iBarHeight+1;
			s_iLoginServerLineCount	++;
			iCategoryLineCount++;
		}

		int		iImage	=	eCSWI_FIRST_BAR;
		DWORD	dwColor	=	0;

		if	(i	==	s_iCurrentLoginServer)
		{
			iImage	+=	3;
			dwColor	=	LTBLUE;
		}
		else
		if	(i	==	s_iFocusLoginServer)
			iImage	+=	6;

		int	iMargin	=	aiMarginWidth[iCategoryLineCount];

		s_arectList[i].set(iX+s_rectServerListBorder.x1,iY,iX+s_rectServerListBorder.x1+iWidth+iMargin,iY+iBarHeight);

		g_sprInterface2.put3PieceBar(iX+s_rectServerListBorder.x1,iY,iImage,iWidth+iMargin);
		s_text.pushShadowStatus(FALSE);
		s_text.put(iX+s_rectServerListBorder.x1+iMargin/2,iY+4,dwColor,s_aLoginServerIPList[i].m_strLocalName);
		s_text.popShadowStatus();

		iX	+=	iWidth+iBarWidth*2+iMargin+1;
	}
}

int
cGAME_TITLE::GetFocusLoginServer()
{
	for	(int i=0;i<s_iLoginServerLocalCount;i++)
	{
		if	(s_arectList[i].isIn(s_posMouse.x,s_posMouse.y))
			return	i;
	}

	return	0xffff;
}
