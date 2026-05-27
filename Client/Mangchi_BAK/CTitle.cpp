#include "CTitle.h"
#include "CJob.h"

CTitleManager	g_titleManager;

//
//	데이터 읽기
BOOL
CTitleManager::loadData(char *_lpstrfn)
{
	if	(_lpstrfn	==	NULL)
		_lpstrfn	=	"data/title.dat";

	cFILE	file;

	if (!file.Open(_lpstrfn,"rb"))
		return	ERRMSG("error in CTitleManager::loadData","can not find '%s' file.",_lpstrfn);

	file.Read(&m_iTitleCount,4);
	file.Read(m_aTitles,sizeof(m_aTitles));
	file.Close();

	return	TRUE;
}

//
//	데이터 읽기
BOOL
CTitleManager::saveData(char *_lpstrfn)
{
	cFILE	file;

	if (!file.Open(_lpstrfn,"wb"))
		return	ERRMSG("error in CTitleManager::saveData","can not open '%st' file.",_lpstrfn);

	file.Write(&m_iTitleCount,4);
	file.Write(m_aTitles,sizeof(m_aTitles));
	file.Close();

	return	TRUE;
}