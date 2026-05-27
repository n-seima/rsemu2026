#ifndef	_use_svn_h
#define	_use_svn_h

#include	"sfc.h"
#include	"../svn/svnUtil.h"
#include	"../svn/WWCommon.h"

extern	cSvnUtil	g_svn;

enum
{
	eSVN_FOR_WRITE,
};

extern	BOOL		CheckSVNStatus(char *fn,int _iWork=eSVN_FOR_WRITE);
extern	BOOL		LockSVN(char *fn,BOOL _bIsByCommand=FALSE);
extern	BOOL		UnlockSVN(char *fn,BOOL _bIsByCommand=FALSE);
extern	BOOL		UpdateFileSVN(char *fn,BOOL _bIsByCommand=FALSE);
extern	BOOL		CommitFileSVN(char *fn,BOOL _bIsByCommand=FALSE);
extern	BOOL		IsLockedSVN(char *fn);

#endif