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
			cMSG::Put("거북이 락 걸기","거북이에 등록된 파일이 아닙니다.\n\n이 기능을 이용하시려면 먼지 해당 파일을 거북이에 등록해 주시기 바랍니다.");

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
		if	(cMSG::YESNO("거북이 락 걸기","락을 걸 수 없습니다.\n\n해당 파일을 최신 데이터로 업데이트 하시겠습니까?")	==	IDNO)
			return	FALSE;

//		g_svn.addFile(cUniCode::AsciiToUnicode(fn));

//		if	(!g_svn.cmdUpdate())
//			return	FALSE;

//		g_svn.addFile(cUniCode::AsciiToUnicode(fn));
//		g_svn.cmdLock();
//		g_svn.isCorrectFile(cUniCode::AsciiToUnicode(fn),NULL,&iIsLocked);	//	락이 걸렸나 확인

		if	(iIsLocked	==	FALSE)
		{
			cMSG::Error("거북이 락 걸기","락을 걸 수 없습니다.");
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
			cMSG::Put("거북이 락 풀기","거북이에 등록된 파일이 아닙니다.\n\n이 기능을 이용하시려면 먼지 해당 파일을 거북이에 등록해 주시기 바랍니다.");

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
		cMSG::Error("거북이 락 풀기","락을 풀 수 없습니다.");

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
			cMSG::Put("거북이 업데이트","거북이에 등록된 파일이 아닙니다.\n\n이 기능을 이용하시려면 먼지 해당 파일을 거북이에 등록해 주시기 바랍니다.");

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
			cMSG::Put("거북이 서버에 파일 전송","거북이에 등록된 파일이 아닙니다.\n\n이 기능을 이용하시려면 먼지 해당 파일을 거북이에 등록해 주시기 바랍니다.");

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
