#include "mainQuest.h"
#include "cTEXT.h"

cChapterManager	g_chapterManager;

cChapterManager::cChapterManager()
{
	m_iChapterCount	=	0;
}

cChapterManager::~cChapterManager()
{
}

char*
cChapterManager::getChapterName(int _iChapter)
{
	if	(_iChapter	>=	m_iChapterCount)
		return	NULL;

	return	m_aChapterList[_iChapter].m_strName;
}

int
cChapterManager::getChapterCommentSize(int _iChapter)
{
	if	(_iChapter	>=	m_iChapterCount)
		return	NULL;

	char	*lpstrComment	=	m_aChapterList[_iChapter].m_strComment;
	
	int		iTextLength		=	strlen(lpstrComment);
	int		iTextPoint		=	0;
	int		iCharCount		=	0;

	while(1)
	{
		if	(iTextPoint	>=	iTextLength)
			return	iCharCount;

		while(lpstrComment[iTextPoint]	==	' ')
		{
			if	(Is2ByteChar(lpstrComment[iTextPoint]))
				iTextPoint++;
			iTextPoint++;
		}

		if	(Is2ByteChar(lpstrComment[iTextPoint]))
			iTextPoint++;

		iTextPoint++;
		iCharCount++;
	}

	return	0;
}

int
cChapterManager::getChapterEpilogCommentSize(int _iChapter)
{
	if	(_iChapter	>=	m_iChapterCount)
		return	NULL;

	char	*lpstrComment	=	m_aChapterList[_iChapter].m_strEpilogComment;
	
	int		iTextLength		=	strlen(lpstrComment);
	int		iTextPoint		=	0;
	int		iCharCount		=	0;

	while(1)
	{
		if	(iTextPoint	>=	iTextLength)
			return	iCharCount;

		while(lpstrComment[iTextPoint]	==	' ')
		{
			if	(Is2ByteChar(lpstrComment[iTextPoint]))
				iTextPoint++;
			iTextPoint++;
		}

		if	(Is2ByteChar(lpstrComment[iTextPoint]))
			iTextPoint++;

		iTextPoint++;
		iCharCount++;
	}

	return	0;
}

char*
cChapterManager::getChapterEpilogComment(int _iChapter,int _iCharCount)
{
	if	(_iChapter	>=	m_iChapterCount)
		return	NULL;

	static	char	strChapterComment[1024];

	char	*lpstrComment	=	m_aChapterList[_iChapter].m_strEpilogComment;

	if	(_iCharCount	==	-1)
		return	lpstrComment;

	int		iTextLength		=	strlen(lpstrComment);
	int		iTextPoint		=	0;
	int		iCharCount		=	0;

	while(1)
	{
		if	(iCharCount	>=	_iCharCount	||	iTextPoint	>=	iTextLength)
		{
			memcpy(strChapterComment,lpstrComment,iTextPoint);
			strChapterComment[iTextPoint]	=	NULL;

			return	strChapterComment;
		}

		while(lpstrComment[iTextPoint]	==	' ')
		{
			if	(Is2ByteChar(lpstrComment[iTextPoint]))
				iTextPoint++;
			iTextPoint++;
		}

		if	(Is2ByteChar(lpstrComment[iTextPoint]))
			iTextPoint++;

		iTextPoint++;
		iCharCount++;
	}

	return	lpstrComment;
}

char*
cChapterManager::getChapterComment(int _iChapter,int _iCharCount)
{
	if	(_iChapter	>=	m_iChapterCount)
		return	NULL;

	static	char	strChapterComment[1024];

	char	*lpstrComment	=	m_aChapterList[_iChapter].m_strComment;

	if	(_iCharCount	==	-1)
		return	lpstrComment;

	int		iTextLength		=	strlen(lpstrComment);
	int		iTextPoint		=	0;
	int		iCharCount		=	0;

	while(1)
	{
		if	(iCharCount	>=	_iCharCount	||	iTextPoint	>=	iTextLength)
		{
			memcpy(strChapterComment,lpstrComment,iTextPoint);
			strChapterComment[iTextPoint]	=	NULL;

			return	strChapterComment;
		}

		while(lpstrComment[iTextPoint]	==	' ')
		{
			if	(Is2ByteChar(lpstrComment[iTextPoint]))
				iTextPoint++;
			iTextPoint++;
		}

		if	(Is2ByteChar(lpstrComment[iTextPoint]))
			iTextPoint++;

		iTextPoint++;
		iCharCount++;
	}

	return	lpstrComment;
}

char*
cChapterManager::getHighQuestName(int _iChapter,int _iHighQuest)
{
	if	(_iChapter	>=	m_iChapterCount)
		return	NULL;

	return	m_aChapterList[_iChapter].m_strHighQuest[_iHighQuest];
}

