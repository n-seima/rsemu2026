#include <stdio.h>
#include "cTRANS.H"
#include "cDRAW.H"
#include "cExceptionHandler.H"

/////////////////////////////////////////////////////////////////////////////////////////
//	팔레트 변환
/////////////////////////////////////////////////////////////////////////////////////////

//int		l_iRed,l_iGreen,l_iBlue;
//int		l_iDestRed,l_iDestGreen,l_iDestBlue;

void	NORMALPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor);
void	BRIGHTPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor);
void	COLOREDTPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor);
void	REDPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor);
void	GREENPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor);
void	BLUEPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor);
void	YELLOWPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor);
void	PURPLEPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor);
void	GRAYPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor);
void	REVERSEPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor);
void	BLACKPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor);	// 검은 색...

void	(*fnCONVERTPLT[dPLT_EFFECT_MAX])(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor)	=
{
	NORMALPLT,
	BRIGHTPLT,
	COLOREDTPLT,
	REDPLT,
	GREENPLT,
	BLUEPLT,
	YELLOWPLT,
	PURPLEPLT,
	GRAYPLT,
	REVERSEPLT,
	BLACKPLT,
};

void
NORMALPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor)
{
	memcpy(_lpDestPlt,_lpSrcPlt,512);
}

WORD
GetBrightColor(WORD _wColor,int _iGrade)
{
	int iRed,iGreen,iBlue;

	if (g_iPixelFormat == dRGB555)
	{
		iRed		=	_wColor>>10;
		iGreen		=	(_wColor>>5)&31;
		iBlue		=	_wColor&31;

		iRed		=	min(iRed	*_iGrade/100,31);
		iGreen		=	min(iGreen	*_iGrade/100,31);
		iBlue		=	min(iBlue	*_iGrade/100,31);

		return	(iRed << 10) + (iGreen << 5) + iBlue;
	}

	iRed		=	_wColor>>11;
	iGreen		=	(_wColor>>5)&63;
	iBlue		=	_wColor&31;

	iRed		=	min(iRed	*_iGrade/100,31);
	iGreen		=	min(iGreen	*_iGrade/100,63);
	iBlue		=	min(iBlue	*_iGrade/100,31);

	return	(iRed << 11) + (iGreen << 5) + iBlue;
}

void
BRIGHTPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor)
{
	int l_iRed,l_iGreen,l_iBlue,i;

	if (g_iPixelFormat == dRGB555)
	{
		for (i=0;i<256;i++)
		{
			l_iRed		=	_lpSrcPlt[i]>>10;
			l_iGreen	=	(_lpSrcPlt[i]>>5)&31;
			l_iBlue		=	_lpSrcPlt[i]&31;

			l_iRed		=	min(l_iRed	*_iGrade/100,31);
			l_iGreen	=	min(l_iGreen*_iGrade/100,31);
			l_iBlue		=	min(l_iBlue	*_iGrade/100,31);

			_lpDestPlt[i]	=	(l_iRed << 10) + (l_iGreen << 5) + l_iBlue;
		}
		return;
	}

	for (i=0;i<256;i++)
	{
		l_iRed		=	_lpSrcPlt[i]>>11;
		l_iGreen	=	(_lpSrcPlt[i]>>5)&63;
		l_iBlue		=	_lpSrcPlt[i]&31;

		l_iRed		=	min(l_iRed	*_iGrade/100,31);
		l_iGreen	=	min(l_iGreen*_iGrade/100,63);
		l_iBlue		=	min(l_iBlue	*_iGrade/100,31);

		_lpDestPlt[i]	=	(l_iRed << 11) + (l_iGreen << 5) + l_iBlue;
	}
}

void
COLOREDTPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor)
{
	int i;
	int l_iRed,l_iGreen,l_iBlue;
	int l_iDestRed,l_iDestGreen,l_iDestBlue;
	
	_iGrade	=	min(_iGrade,100);

	if (g_iPixelFormat == dRGB555)
	{
		l_iDestRed		=	_wDestColor>>10;
		l_iDestGreen	=	(_wDestColor>>5)&31;
		l_iDestBlue		=	_wDestColor&31;

		for (i=0;i<256;i++)
		{
			l_iRed		=	_lpSrcPlt[i]>>10;
			l_iGreen	=	(_lpSrcPlt[i]>>5)&31;
			l_iBlue		=	_lpSrcPlt[i]&31;

			l_iRed		+=	(l_iDestRed		- l_iRed	)*_iGrade/100;
			l_iGreen	+=	(l_iDestGreen	- l_iGreen	)*_iGrade/100;
			l_iBlue		+=	(l_iDestBlue	- l_iBlue	)*_iGrade/100;

			_lpDestPlt[i]	=	(l_iRed << 10) + (l_iGreen << 5) + l_iBlue;
		}
		return;
	}

	l_iDestRed		=	_wDestColor>>11;
	l_iDestGreen	=	(_wDestColor>>5)&63;
	l_iDestBlue		=	_wDestColor&31;

	for (i=0;i<256;i++)
	{
		l_iRed		=	_lpSrcPlt[i]>>11;
		l_iGreen	=	(_lpSrcPlt[i]>>5)&63;
		l_iBlue		=	_lpSrcPlt[i]&31;

		l_iRed		+=	(l_iDestRed		- l_iRed	)*_iGrade/100;
		l_iGreen	+=	(l_iDestGreen	- l_iGreen	)*_iGrade/100;
		l_iBlue		+=	(l_iDestBlue	- l_iBlue	)*_iGrade/100;

		_lpDestPlt[i]	=	(l_iRed << 11) + (l_iGreen << 5) + l_iBlue;
	}
}
//	노랑은 RED  ,GREEN/2,BLUE 0		-	30%
//	보라는 RED/2,0        ,BLUE		-	25%
//	레드는 RED  ,GREEN-1/2 ,BLUE	-	35%
//	블루는 RED 0,GREEN-1/2 ,BLUE+12	-	30%
//	그린은 RED 0,GREEN-1/2 ,BLUE+12	-	30%


void
REDPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor)
{
	int l_iRed,l_iGreen,l_iBlue,i;

	if (g_iPixelFormat == dRGB555)
	{
		for (i=0;i<256;i++)
		{
			l_iRed			=	_lpSrcPlt[i]>>10;
			l_iGreen		=	(_lpSrcPlt[i]>>5)&31;
			l_iBlue			=	_lpSrcPlt[i]&31;

			int iGray		=	((l_iRed * 30) + (l_iGreen * 59) + (l_iBlue * 11)) / 100;

			l_iRed			=	min(iGray*_iGrade/100,31);
			_lpDestPlt[i]	=	(l_iRed << 10);
		}
		return;
	}

	if (g_iPixelFormat == dRGB565)
	{
		for (i=0;i<256;i++)
		{
			l_iRed			=	_lpSrcPlt[i]>>11;
			l_iGreen		=	(_lpSrcPlt[i]>>5)&63;
			l_iBlue			=	_lpSrcPlt[i]&31;

			int iGray		=	((l_iRed * 30) + (l_iGreen * 29) + (l_iBlue * 11)) / 100;

			l_iRed			=	min(iGray*_iGrade/100,31);

			_lpDestPlt[i]	=	(l_iRed << 11);
		}
		return;
	}

	for (i=0;i<256;i++)
	{
		l_iBlue			=	_lpSrcPlt[i]>>11;
		l_iGreen		=	(_lpSrcPlt[i]>>5)&63;
		l_iRed			=	_lpSrcPlt[i]&31;

		int iGray		=	((l_iRed * 30) + (l_iGreen * 29) + (l_iBlue * 11)) / 100;

		l_iRed			=	min(iGray*_iGrade/100,31);

		_lpDestPlt[i]	=	l_iRed;
	}

}

//	그린은 RED 0,GREEN-1/2 ,BLUE+12	-	30%
void
GREENPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor)
{
	int l_iRed,l_iGreen,l_iBlue,i;

	if (g_iPixelFormat == dRGB555)
	{
		for (i=0;i<256;i++)
		{
			l_iRed		=	_lpSrcPlt[i]>>10;
			l_iGreen	=	(_lpSrcPlt[i]>>5)&31;
			l_iBlue		=	_lpSrcPlt[i]&31;

			int iGray	=	((l_iRed * 30) + (l_iGreen * 59) + (l_iBlue * 11)) / 100;

			l_iGreen	=	min(iGray*_iGrade/100,31);

			_lpDestPlt[i]	=	(l_iGreen << 5);
		}
		return;
	}

	for (i=0;i<256;i++)
	{
		l_iRed		=	_lpSrcPlt[i]>>11;
		l_iGreen	=	(_lpSrcPlt[i]>>5)&63;
		l_iBlue		=	_lpSrcPlt[i]&31;

		int iGray	=	((l_iRed * 30) + (l_iGreen * 29) + (l_iBlue * 11)) / 100;

		l_iGreen	=	min(iGray*_iGrade/100,63);

		_lpDestPlt[i]	=	(l_iGreen << 5);
	}
}

//	블루는 RED 0,GREEN-1/2 ,BLUE+12	-	30%
void
BLUEPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor)
{
	int iRed,iGreen,iBlue,i;
/*
	_iGrade	=	31*_iGrade/100;

	if (g_iPixelFormat == dRGB555)
	{
		for (i=0;i<256;i++)
		{
			iRed			=	_lpSrcPlt[i]>>10;
			iGreen			=	(_lpSrcPlt[i]>>5)&31;
			iBlue			=	_lpSrcPlt[i]&31;

			int iGray		=	((iRed * 30) + (iGreen * 59) + (iBlue * 11)) / 100;

			iGreen			=	iGray-_iGrade/2;
			iBlue			=	iGray+_iGrade;
			iBlue			=	min(iBlue,31);
			iGreen			=	max(iGreen,0);

			iBlue			=	iBlue*200/100;
			iGreen			=	iGreen*200/100;
			iBlue			=	min(iBlue,31);
			iGreen			=	min(iGreen,31);

			_lpDestPlt[i]	=	(iGreen << 5) + iBlue;
		}
		return;
	}

	if (g_iPixelFormat == dRGB565)
	{
		for (i=0;i<256;i++)
		{
			iRed			=	_lpSrcPlt[i]>>11;
			iGreen			=	(_lpSrcPlt[i]>>5)&63;
			iBlue			=	_lpSrcPlt[i]&31;

			int iGray		=	((iRed * 30) + (iGreen * 29) + (iBlue * 11)) / 100;

			iGreen			=	iGray-_iGrade/2;
			iBlue			=	iGray+_iGrade;
			iBlue			=	min(iBlue,31);
			iGreen			=	max(iGreen,0);

			iBlue			=	iBlue*200/100;
			iGreen			=	iGreen*200/100;
			iBlue			=	min(iBlue,31);
			iGreen			=	min(iGreen,31);

			_lpDestPlt[i]	=	(iGreen << 6) + iBlue;
		}
		return;
	}

	for (i=0;i<256;i++)
	{
		iBlue			=	_lpSrcPlt[i]>>11;
		iGreen			=	(_lpSrcPlt[i]>>5)&63;
		iRed			=	_lpSrcPlt[i]&31;

		int iGray		=	((iRed * 30) + (iGreen * 29) + (iBlue * 11)) / 100;

		iGreen			=	iGray-_iGrade/2;
		iBlue			=	iGray+_iGrade;
		iBlue			=	min(iBlue,31);
		iGreen			=	max(iGreen,0);

		iBlue			=	iBlue*200/100;
		iGreen			=	iGreen*200/100;
		iBlue			=	min(iBlue,31);
		iGreen			=	min(iGreen,31);

		_lpDestPlt[i]	=	(iBlue << 10) + (iGreen << 6);
	}

	return;
*/
	if (g_iPixelFormat == dRGB555)
	{
		for (i=0;i<256;i++)
		{
			iRed			=	_lpSrcPlt[i]>>10;
			iGreen			=	(_lpSrcPlt[i]>>5)&31;
			iBlue			=	_lpSrcPlt[i]&31;

			int iGray		=	((iRed * 30) + (iGreen * 59) + (iBlue * 11)) / 100;

			iBlue			=	min(iGray*_iGrade/100,31);
			iGreen			=	max(iGray-(100-_iGrade/2),0);

			_lpDestPlt[i]	=	(iGreen << 5) + iBlue;
		}
		return;
	}

	if (g_iPixelFormat == dRGB565)
	{
		for (i=0;i<256;i++)
		{
			iRed			=	_lpSrcPlt[i]>>11;
			iGreen			=	(_lpSrcPlt[i]>>5)&63;
			iBlue			=	_lpSrcPlt[i]&31;

			int iGray		=	((iRed * 30) + (iGreen * 29) + (iBlue * 11)) / 100;

			iBlue			=	min(iGray*_iGrade/100,31);
			iGreen			=	max(iGray-(100-_iGrade/2),0);
			iGreen			*=	2;

			_lpDestPlt[i]	=	(iGreen << 5) + iBlue;
		}
		return;
	}

	for (i=0;i<256;i++)
	{
		iBlue			=	_lpSrcPlt[i]>>11;
		iGreen			=	(_lpSrcPlt[i]>>5)&63;
		iRed			=	_lpSrcPlt[i]&31;

		int iGray		=	((iRed * 30) + (iGreen * 29) + (iBlue * 11)) / 100;

		iBlue			=	min(iGray*_iGrade/100,31);
		iGreen			=	max(iGray-(100-_iGrade/2),0);
		iGreen			*=	2;

		_lpDestPlt[i]	=	(iBlue << 11)+(iGreen << 5);
	}
}

//	노랑은 RED  ,GREEN/2,BLUE 0		-	30%
void
YELLOWPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor)
{
	int l_iRed,l_iGreen,l_iBlue,i;

	if (g_iPixelFormat == dRGB555)
	{
		for (i=0;i<256;i++)
		{
			l_iRed			=	_lpSrcPlt[i]>>10;
			l_iGreen		=	(_lpSrcPlt[i]>>5)&31;
			l_iBlue			=	_lpSrcPlt[i]&31;

			int iGray		=	((l_iRed * 30) + (l_iGreen * 59) + (l_iBlue * 11)) / 100;

			l_iRed			=	min(iGray*_iGrade/100,31);
			l_iGreen		=	min(iGray*_iGrade/2/100,31);

			_lpDestPlt[i]	=	(l_iRed << 10) + (l_iGreen << 5);
		}
		return;
	}

	if (g_iPixelFormat == dRGB565)
	{
		for (i=0;i<256;i++)
		{
			l_iRed		=	_lpSrcPlt[i]>>11;
			l_iGreen	=	(_lpSrcPlt[i]>>5)&63;
			l_iBlue		=	_lpSrcPlt[i]&31;

			int iGray		=	((l_iRed * 30) + (l_iGreen * 29) + (l_iBlue * 11)) / 100;

			l_iRed			=	min(iGray*_iGrade/100,31);
			l_iGreen		=	min(iGray*_iGrade/100,63);

			_lpDestPlt[i]	=	(l_iRed << 11) + (l_iGreen << 5);
		}
		return;
	}

	for (i=0;i<256;i++)
	{
		l_iBlue		=	_lpSrcPlt[i]>>11;
		l_iGreen	=	(_lpSrcPlt[i]>>5)&63;
		l_iRed		=	_lpSrcPlt[i]&31;

		int iGray		=	((l_iRed * 30) + (l_iGreen * 29) + (l_iBlue * 11)) / 100;

		l_iRed			=	min(iGray*_iGrade/100,31);
		l_iGreen		=	min(iGray*_iGrade/100,63);

		_lpDestPlt[i]	=	(l_iGreen << 5) + l_iRed;
	}

}

//	보라는 RED/2,0        ,BLUE		-	25%
void
PURPLEPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor)
{
	int l_iRed,l_iGreen,l_iBlue,i;

	if (g_iPixelFormat == dRGB555)
	{
		for (i=0;i<256;i++)
		{
			l_iRed			=	_lpSrcPlt[i]>>10;
			l_iGreen		=	(_lpSrcPlt[i]>>5)&31;
			l_iBlue			=	_lpSrcPlt[i]&31;

			int iGray		=	((l_iRed * 30) + (l_iGreen * 59) + (l_iBlue * 11)) / 100;

			l_iRed			=	min(iGray*_iGrade/2/100,31);
			l_iBlue			=	min(iGray*_iGrade/100,31);

			_lpDestPlt[i]	=	(l_iRed << 10) + l_iBlue;
		}
		return;
	}

	if (g_iPixelFormat == dRGB565)
	{
		for (i=0;i<256;i++)
		{
			l_iRed			=	_lpSrcPlt[i]>>11;
			l_iGreen		=	(_lpSrcPlt[i]>>5)&63;
			l_iBlue			=	_lpSrcPlt[i]&31;

			int iGray		=	((l_iRed * 30) + (l_iGreen * 29) + (l_iBlue * 11)) / 100;

			l_iRed			=	min(iGray*_iGrade/2/100,31);
			l_iBlue			=	min(iGray*_iGrade/100,31);

			_lpDestPlt[i]	=	(l_iRed << 11) + l_iBlue;
		}
		return;
	}

	for (i=0;i<256;i++)
	{
		l_iBlue			=	_lpSrcPlt[i]>>11;
		l_iGreen		=	(_lpSrcPlt[i]>>5)&63;
		l_iRed			=	_lpSrcPlt[i]&31;

		int iGray		=	((l_iRed * 30) + (l_iGreen * 29) + (l_iBlue * 11)) / 100;

		l_iRed			=	min(iGray*_iGrade/2/100,31);
		l_iBlue			=	min(iGray*_iGrade/100,31);

		_lpDestPlt[i]	=	(l_iBlue << 11) + l_iRed;
	}
}

void
GRAYPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor)
{
	int l_iRed,l_iGreen,l_iBlue,i,iGray;

	if (g_iPixelFormat == dRGB555)
	{
		for (i=0;i<256;i++)
		{
			l_iRed		=	_lpSrcPlt[i]>>10;
			l_iGreen	=	(_lpSrcPlt[i]>>5)&31;
			l_iBlue		=	_lpSrcPlt[i]&31;

			iGray		=	((l_iRed * 30) + (l_iGreen * 59) + (l_iBlue * 11)) / 100;

			_lpDestPlt[i]	=	(iGray << 10) + (iGray << 5) + iGray;
		}
		return;
	}

	for (i=0;i<256;i++)
	{
		l_iRed		=	_lpSrcPlt[i]>>11;
		l_iGreen	=	(_lpSrcPlt[i]>>5)&63;
		l_iBlue		=	_lpSrcPlt[i]&31;

		iGray		=	((l_iRed * 30) + (l_iGreen * 29) + (l_iBlue * 11)) / 100;

		_lpDestPlt[i]	=	(iGray << 11) + (iGray << 6) + iGray;
	}
}

void
REVERSEPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor)
{
	int l_iRed,l_iGreen,l_iBlue,i;

	if (g_iPixelFormat == dRGB555)
	{
		for (i=0;i<256;i++)
		{
			l_iRed		=	_lpSrcPlt[i]>>10;
			l_iGreen	=	(_lpSrcPlt[i]>>5)&31;
			l_iBlue		=	_lpSrcPlt[i]&31;

			l_iBlue		=	31	-	l_iBlue	;
			l_iRed		=	31	-	l_iRed	;
			l_iGreen	=	31	-	l_iGreen;

			_lpDestPlt[i]	=	(l_iRed << 10) + (l_iGreen << 5) + l_iBlue;
		}
		return;
	}

	if (g_iPixelFormat == dRGB565)
	{
		for (i=0;i<256;i++)
		{
			l_iRed		=	_lpSrcPlt[i]>>11;
			l_iGreen	=	(_lpSrcPlt[i]>>5)&63;
			l_iBlue		=	_lpSrcPlt[i]&31;

			l_iRed		=	31	-	l_iRed;
			l_iGreen	=	63	-	l_iGreen;
			l_iBlue		=	31	-	l_iBlue;

			_lpDestPlt[i]	=	(l_iRed << 11) + (l_iGreen << 5) + l_iBlue;
		}
		return;
	}

	for (i=0;i<256;i++)
	{
		l_iBlue		=	_lpSrcPlt[i]>>11;
		l_iGreen	=	(_lpSrcPlt[i]>>5)&63;
		l_iRed		=	_lpSrcPlt[i]&31;

		l_iBlue		=	31	-	l_iBlue;
		l_iGreen	=	63	-	l_iGreen;
		l_iRed		=	31	-	l_iRed;
		_lpDestPlt[i]	=	(l_iBlue << 11) + (l_iGreen << 5) + l_iRed;
	}
}
void
BLACKPLT(WORD *_lpSrcPlt,WORD *_lpDestPlt,int _iGrade,WORD _wDestColor)
{
	memset(_lpDestPlt,0,sizeof(WORD)*256);

}

/////////////////////////////////////////////////////////////////////////////////////////
//	픽셸 변환
/////////////////////////////////////////////////////////////////////////////////////////

inline	WORD
BRIGHTPIXEL555(WORD srcColor,int grade)
{
	int l_iRed,l_iGreen,l_iBlue;

	l_iRed		=	srcColor>>10;
	l_iGreen	=	(srcColor>>5)&31;
	l_iBlue		=	srcColor&31;

	srcColor	=	(l_iRed << 10) + (l_iGreen << 5) + l_iBlue;

	return	srcColor;
}

inline	WORD
BRIGHTPIXEL565(WORD srcColor,int grade)
{
	int l_iRed,l_iGreen,l_iBlue;

	l_iRed		=	srcColor>>10;
	l_iGreen	=	(srcColor>>5)&31;
	l_iBlue		=	srcColor&31;

	srcColor	=	(l_iRed << 10) + (l_iGreen << 5) + l_iBlue;

	return	srcColor;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	각종 이미지의 변환
/////////////////////////////////////////////////////////////////////////////////////////
inline WORD
RGB24ToRGB565(int r,int g,int b)
{	r	=	(r>>3)<<11;
	g	=	(g>>2)<<5;
	b	=	(b>>3);

	return (WORD) (r+g+b);
}

inline WORD
RGB24ToRGB555(int r,int g,int b)
{	r	=	(r>>3)<<10;
	g	=	(g>>3)<<5;
	b	=	(b>>3);

	return (WORD) (r+g+b);
}

inline WORD
RGB24ToBGR565(int r,int g,int b)
{	r	=	(r>>3);
	g	=	(g>>2)<<5;
	b	=	(b>>3)<<11;

	return (WORD) (r+g+b);
}

inline WORD
RGB24To16(int r,int g,int b)
{	if (g_iPixelFormat	==	dRGB565)	return	RGB24ToRGB565(r,g,b);
	if (g_iPixelFormat	==	dRGB555)	return	RGB24ToRGB555(r,g,b);
	return	RGB24ToBGR565(r,g,b);
}

inline WORD
RGB24To16(DWORD color)
{	int	r	=	GetRValue(color);
	int	g	=	GetGValue(color);
	int	b	=	GetBValue(color);

	return RGB24To16(r,g,b);
}

inline	DWORD	
RGB565To24(WORD color)
{
	int	r	=	(color>>11    )<<3;
	int	g	=	((color>>5)&63)<<2;
	int	b	=	(color&31	  )<<3;

	return RGB(r,g,b);
}

inline void
RGB16To24(WORD color,BYTE &r,BYTE &g,BYTE &b)
{
	if (g_iPixelFormat	==	dRGB565)
	{
		r	=	(color>>11    )<<3;
		g	=	((color>>5)&63)<<2;
		b	=	(color&31	  )<<3;
		return;
	}
	if (g_iPixelFormat	==	dRGB555)
	{
		r	=	(color>>10    )<<3;
		g	=	((color>>5)&31)<<3;
		b	=	(color&31	  )<<3;
		return;
	}

	r	=	(color&31	  )<<3;
	g	=	((color>>5)&63)<<2;
	b	=	(color>>11    )<<3;
}

inline DWORD
RGB16To24(WORD color)
{	
	int	r,g,b;

	if (g_iPixelFormat	==	dRGB565)
	{
		r	=	(color>>11    )<<3;
		g	=	((color>>5)&63)<<2;
		b	=	(color&31	  )<<3;

		return RGB(r,g,b);
	}

	if (g_iPixelFormat	==	dRGB555)
	{	
		r	=	(color>>10    )<<3;
		g	=	((color>>5)&31)<<3;
		b	=	(color&31	  )<<3;

		return RGB(r,g,b);
	}

	r	=	(color&31	  )<<3;
	g	=	((color>>5)&63)<<2;
	b	=	(color>>11    )<<3;

	return RGB(r,g,b);
}


inline WORD
RGB565ToBGR565(WORD color)
{	WORD	r	=	color>>11;
	WORD	g	=	color&0x7e0;
	WORD	b	=	(color&31)<<11;

	return (r+g+b);
}

inline WORD
RGB565ToRGB555(WORD color)
{	WORD	r	=	(color&0xf800)>>1;
	WORD	g	=	(color&0x7c0)>>1;
	WORD	b	=	color&31;

	return (r+g+b);
}

inline WORD
BGR565ToRGB555(WORD color)
{	WORD	b	=	(color&0xf800)>>1;
	WORD	g	=	(color&0x7c0)>>1;
	WORD	r	=	color&31;

	return (r+g+b);
}

inline WORD
ToRGB565(WORD color)
{	WORD	r	=	(color&0x7c00)<<1;
	WORD	g	=	(color&0x3e0)<<1;
	WORD	b	=	color&31;

	return (r+g+b);
}

inline WORD
ToBGR565(WORD color)
{	WORD	r	=	(color&0x7c00)>>10;
	WORD	g	=	(color&0x3e0)<<1;
	WORD	b	=	(color&31)<<11;

	return (r+g+b);
}

inline WORD
CONVERTPIXEL(WORD color)
{	if (g_iPixelFormat	==	dRGB565)	return	ToRGB565(color);
	if (g_iPixelFormat	==	dBGR565)	return	ToBGR565(color);

	return	color;
}

BOOL
CONVERTSPRITE(WORD *sprite)
{	
	if (g_iPixelFormat	==	dRGB555)
		return TRUE;
	if (!sprite)
		return	g_eh.addStaticLog("Error!! in CONVERTSPRITE - pointer 'sprite' is valid");

	int   i,j,k,xs ,ys,count,BufferPoint=4,size;

	xs	=	sprite[0];
	ys	=	sprite[1];

	for(j=0;j<ys;j++)
	{	count	=	sprite[BufferPoint];
		BufferPoint++;

		for(i=0;i<count;i++)
		{	size				=	sprite[BufferPoint+1];
			BufferPoint			+=	2;

			for (k=0;k<size;k++)
			{	WORD color	=	sprite[BufferPoint+k];
				sprite[BufferPoint+k]	=	CONVERTPIXEL(color);
			}
			BufferPoint	+=	size;
		}
	}

	return TRUE;
}

BOOL
CONVERTIMAGE(WORD *image)
{	
	if (!image)
		return	g_eh.addStaticLog("Error!! in CONVERTIMAGE - pointer 'image' is valid");

	int	i;
	int	size=image[0]*image[1];

	if (g_iPixelFormat	==	dRGB565)
	{	
		for (i=0;i<size;i++)
			image[i+2]	=	ToRGB565(image[i+2]);
	}
	else
	if (g_iPixelFormat	==	dBGR565)
	{	
		for (i=0;i<size;i++)
			image[i+2]	=	ToBGR565(image[i+2]);
	}

	return TRUE;
}

BOOL
CONVERTIMAGETo555(WORD *image)
{	if (!image)	return	g_eh.addStaticLog("Error!! in CONVERTIMAGE - pointer 'image' is valid");

	int	i;
	int	size=image[0]*image[1];

	if (g_iPixelFormat	==	dRGB565)
	{	for (i=0;i<size;i++)	image[i+2]	=	RGB565ToRGB555(image[i+2]);
	}
	else
	if (g_iPixelFormat	==	dBGR565)
	{	for (i=0;i<size;i++)	image[i+2]	=	BGR565ToRGB555(image[i+2]);
	}

	return TRUE;
}

BOOL
CONVERTPALETTE(WORD *plt,int range)
{	if (!plt)	return	g_eh.addStaticLog("Error!! in CONVERTPALETTE - pointer 'plt' is valid");
	int	i;

	if (g_iPixelFormat	==	dRGB565)
	{	for (i=0;i<range;i++)	plt[i]	=	ToRGB565(plt[i]);
	}
	else
	if (g_iPixelFormat	==	dBGR565)
	{	for (i=0;i<range;i++)	plt[i]	=	ToBGR565(plt[i]);
	}

	return TRUE;
}

WORD*
CONVERTPALETTE(BYTE *plt)
{	WORD	*result;
	result	=	_New(WORD,256,NULL,"CONVERTPALETTE result");

	for (int i=0;i<256;i++)	result[i]	=	RGB24To16(plt[i*3],plt[i*3+1],plt[i*3+2]);

	return result;
}

WORD*
CONVERT24TO16(BYTE *buffer,int xs,int ys)
{	WORD	*result;

	result		=	_New(WORD,xs*ys+2,NULL,"CONVERT24TO16 result");
	result[0]	=	xs;
	result[1]	=	ys;

	for (int i=0;i<xs*ys;i++)
	{	result[i+2]	=	RGB24To16(buffer[i*3],buffer[i*3+1],buffer[i*3+2]);
	}

	return result;
}

WORD*
PCX24TO16(BYTE *buffer,int xs,int ys)
{	WORD	*result;

	result		=	_New(WORD,xs*ys+2,NULL,"PCX24TO16 result");
	result[0]	=	xs;
	result[1]	=	ys;

	for(int j = 0; j < ys; j++)
	{	int	src	= j * xs * 3;
		int	dest= j * xs + 2;

		for(int i = 0; i < xs; i++,dest++,src++)
			result[dest]	=	RGB24To16(buffer[src],buffer[src + xs],buffer[src + xs* 2]);
	}


	return result;
}

WORD*
CONVERT8TO16(BYTE *buffer,BYTE *plt,int xs,int ys)
{	WORD	*result;
	WORD	*plt2	=	CONVERTPALETTE(plt);

	result		=	_New(WORD,xs*ys+2,NULL,"CONVERT8TO16 result");
	result[0]	=	xs;
	result[1]	=	ys;

	for (int i=0;i<xs*ys;i++)
	{	int index	=	buffer[i];
		result[i+2]	=	plt2[index];
	}

	_Free(plt2);

	return result;
}


BYTE*
CONVERT16TO24(WORD *buffer)
{	BYTE	*result;
	int		xs	=	buffer[0],ys =	buffer[1];

	result	=	_New(BYTE,xs*ys*3,NULL,"CONVERT16TO24 result");

	for (int i=0;i<xs*ys;i++)
		RGB16To24(buffer[i+2],result[i*3],result[i*3+1],result[i*3+2]);

	return result;
}


