#include "message.h"
#include "stdafx.h"

#ifdef	_FOR_KOREA
char	*g_strMessage[]	=
{
//	0~5
	"",
	"   .",
};


char	*g_strFormMessage[]	=
{
//	0~5
	"    .\n\n%s",
};

#elif	_FOR_CHINA
char	*g_strMessage[]	=
{
//	0~5
	"",
	"",
	"",
};


char	*g_strFormMessage[]	=
{
//	0~5
	"\n\n%s",
};

#elif	_FOR_INTER

char	*g_strMessage[]	=
{
//	0~5
	"Error",
	"The registry call failed.",
	"The folder intalled the game cannot be discovered.",
};


char	*g_strFormMessage[]	=
{
//	0~5
	"This folder cannot be discovered.\n\n%s",
};

#elif	_FOR_ENG

char	*g_strMessage[]	=
{
//	0~5
	"Error",
	"The registry call failed.",
	"The folder intalled the game cannot be discovered.",
};


char	*g_strFormMessage[]	=
{
//	0~5
	"This folder cannot be discovered.\n\n%s",
};
	
#elif	_FOR_THAI

char	*g_strMessage[]	=
{
//	0~5
	"?",
	"? ",
	"  ",
};


char	*g_strFormMessage[]	=
{
//	0~5
	"\n\n%s",
};


#endif