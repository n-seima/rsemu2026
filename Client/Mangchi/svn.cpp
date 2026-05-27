#include	"svn.h"
#include	"cMESSAGE.h"

//cSvnUtil	g_svn;

using	namespace	W2Lib;

BOOL
IsLockedSVN(char *fn)
{
//	int		iIsLocked;

//	if	(!IsFile(fn)	||	g_svn.isCorrectFile(cUniCode::AsciiToUnicode(fn),NULL,&iIsLocked)	==	FALSE)
//		return	TRUE;

	//return	iIsLocked;
	return	NULL;
}

BOOL
CheckSVNStatus(char *fn,int _iWork)
{
	switch(_iWork)
	{
		case	eSVN_FOR_WRITE	:
			if	(IsLockedSVN(fn)	==	FALSE)
			{
				//if	(cMSG::YESNO("파일 이용","이 파일을 이용 하려면 먼저 락을 걸어야 합니다.\n\n락을 거시겠습니까?")	==	IDNO)
				//	return	FALSE;


			}

			if	(!LockSVN(fn))
				return	FALSE;
			break;
	}

	return	TRUE;
}

BOOL
LockSVN(char *fn,BOOL _bIsByCommand)
{
	return	TRUE;

	int		iIsLocked	=	0;

	if	(!IsFile(fn))//	||	g_svn.isCorrectFile(cUniCode::AsciiToUnicode(fn),NULL,&iIsLocked)	==	FALSE)
	{
		if	(_bIsByCommand)
			cMSG::Put("Lock Turtle", "This file is not registered in Turtle.\n\nTo use this function, please register the file in Turtle.");

		return	TRUE;
	}

	/*
	if	(iIsLocked)
	{
		if	(_bIsByCommand)
			cMSG::Put("거북이 락 걸기","이미 락이 걸려 있는 상태입니다.");

		return	TRUE;
	}
	*/

//	g_svn.addFile(cUniCode::AsciiToUnicode(fn));
//	g_svn.cmdLock();
//	g_svn.isCorrectFile(cUniCode::AsciiToUnicode(fn),NULL,&iIsLocked);	//	락이 걸렸나 확인

	if	(iIsLocked	==	0)
	{
		if (cMSG::YESNO("Lock the turtle", "Unable to lock.\n\nDo you want to update the file with the latest data?") == IDNO)
			return	FALSE;

//		g_svn.addFile(cUniCode::AsciiToUnicode(fn));

//		if	(!g_svn.cmdUpdate())
//			return	FALSE;

//		g_svn.addFile(cUniCode::AsciiToUnicode(fn));
//		g_svn.cmdLock();
//		g_svn.isCorrectFile(cUniCode::AsciiToUnicode(fn),NULL,&iIsLocked);	//	락이 걸렸나 확인

		if	(iIsLocked	==	FALSE)
		{
			cMSG::Error("Locking turtle", "Unable to lock.");
			return	FALSE;
		}

		return	FALSE;
	}

	return	TRUE;
}

BOOL
UnlockSVN(char *fn,BOOL _bIsByCommand)
{
	return	TRUE;

	int		iIsLocked	=	0;

	if	(!IsFile(fn))//	||	g_svn.isCorrectFile(cUniCode::AsciiToUnicode(fn),NULL,&iIsLocked)	==	FALSE)
	{
		if	(_bIsByCommand)
			cMSG::Put("Unlock turtle", "This file is not registered in turtle.\n\nTo use this function, please register the file in turtle.");

		return	TRUE;
	}
/*
	if	(iIsLocked	==	FALSE)
	{
		if	(_bIsByCommand)
			cMSG::Put("거북이 락 풀기","이미 락이 풀려 있는 상태입니다.");

		return	TRUE;
	}
*/
//	g_svn.addFile(cUniCode::AsciiToUnicode(fn));
//	g_svn.cmdUnlock();
//	g_svn.isCorrectFile(cUniCode::AsciiToUnicode(fn),NULL,&iIsLocked);	//	락이 걸렸나 확인

	if	(iIsLocked	==	TRUE)
	{
		cMSG::Error("Unlock turtle", "Unable to unlock.");

		return	FALSE;
	}

	return	TRUE;
}

BOOL
UpdateFileSVN(char *fn,BOOL _bIsByCommand)
{
//	int		iIsLocked;

	if	(!IsFile(fn))//	||	g_svn.isCorrectFile(cUniCode::AsciiToUnicode(fn),NULL,&iIsLocked)	==	FALSE)
	{
		if	(_bIsByCommand)
			cMSG::Put("Turtle Update", "This file is not registered in Turtle.\n\nTo use this function, please register the file in Turtle.");

		return	TRUE;
	}

//	g_svn.addFile(cUniCode::AsciiToUnicode(fn));
//	g_svn.cmdUpdate();

	return	TRUE;
}

BOOL
CommitFileSVN(char *fn,BOOL _bIsByCommand)
{
//	int		iIsLocked,iResult=TRUE;

	if	(!IsFile(fn))//	||	g_svn.isCorrectFile(cUniCode::AsciiToUnicode(fn),NULL,&iIsLocked)	==	FALSE)
	{
		if	(_bIsByCommand)
			cMSG::Put("Send file to turtle server", "This file is not registered in turtle.\n\nTo use this function, please register the file in turtle.");

		return	TRUE;
	}
/*
	if	(iIsLocked)
	{
		if	(!UnlockSVN(fn,_bIsByCommand))
			return	FALSE;
	}
*/
//	g_svn.addFile(cUniCode::AsciiToUnicode(fn));

//	if	(!g_svn.cmdCommit())
//		return	FALSE;

	return	TRUE;
}
