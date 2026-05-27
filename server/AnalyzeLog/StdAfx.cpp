// stdafx.cpp : source file that includes just the standard includes
//	AnalyzeLog.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#ifdef	_FOR_USA
LPCTSTR		g_lpszRegistry		=	"software\\L&K Logic Korea\\Red Stone for USA";
LPCTSTR		lpszTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(test) for USA";
LPCTSTR		lpszBalanceRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for USA";
#elif	_FOR_INTERNATIONAL
LPCTSTR		g_lpszRegistry		=	"software\\L&K Logic Korea\\Red Stone for International";
LPCTSTR		lpszTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(test) for International";
LPCTSTR		lpszBalanceRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for International";
#elif	_FOR_THAI
LPCTSTR		g_lpszRegistry		=	"software\\L&K Logic Korea\\Red Stone for Thai";
LPCTSTR		lpszTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(test) for Thai";
LPCTSTR		lpszBalanceRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for Thai";
#elif	_FOR_JAPAN
LPCTSTR		g_lpszRegistryOld	=	"software\\L&K Logic Korea\\Red Stone";
LPCTSTR		lpszTestRegistryOld	=	"software\\L&K Logic Korea\\Red Stone(test)";
LPCTSTR		lpszBalanceRegistryOld=	"software\\L&K Logic Korea\\Red Stone(balance)";
LPCTSTR		g_lpszRegistry		=	"software\\L&K Logic Korea\\Red Stone for japan";
LPCTSTR		lpszTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(test) for japan";
LPCTSTR		lpszBalanceRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for japan";
#else	
LPCTSTR		g_lpszRegistry		=	"software\\L&K Logic Korea\\Red Stone";
LPCTSTR		lpszTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(test)";
LPCTSTR		lpszBalanceRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance)";
#endif

HKEY		g_hRegKey	=	0;
