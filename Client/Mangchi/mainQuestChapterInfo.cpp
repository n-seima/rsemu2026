#include	"mainQuestChapterInfo.H"

cChapterInfo::cChapterInfo()
{
	reset();
}

void
cChapterInfo::reset()
{
	m_iHighQuestCount	=	0;
}

void
cChapterInfo::modifyChapterName(char *_lpstrName)
{
	strncpy(m_strName,_lpstrName,c_iChapterNameLength-1);
}

BOOL
cChapterInfo::addHighQuest(char *_lpstrName)
{
	if	(m_iHighQuestCount	>=	c_iMaxHighQuestCount)
		return	FALSE;

	strncpy(m_strHighQuest[m_iHighQuestCount],_lpstrName,c_iHighQuestNameLength-1);

	m_iHighQuestCount++;

	return	TRUE;	
}

BOOL
cChapterInfo::modifyHighQuestName(int _iChapterIndex,char *_lpstrName)
{
	if	(_iChapterIndex	>=	m_iHighQuestCount)
		return	FALSE;

	strncpy(m_strHighQuest[_iChapterIndex],_lpstrName,c_iHighQuestNameLength-1);

	return	TRUE;	
}

void
cChapterInfo::changeComment(char *_lpstrComment)
{
	strncpy(m_strComment,_lpstrComment,c_iChapterCommentLength-1);
}

void
cChapterInfo::changeEpilogComment(char *_lpstrComment)
{
	strncpy(m_strEpilogComment,_lpstrComment,c_iChapterEpilogCommentLength-1);
}

void
cChapterInfo::copy(cChapterInfo *_lpChapter)
{
	memcpy(this,_lpChapter,sizeof(cChapterInfo));
}

BOOL
cChapterInfo::removeHighQuest(int _iIndex)
{
	if	(_iIndex	>=	m_iHighQuestCount)
		return	FALSE;

	for (int i=_iIndex;i<m_iHighQuestCount-1;i++)
		strcpy(m_strHighQuest[i],m_strHighQuest[i+1]);

	m_iHighQuestCount--;

	return	TRUE;
}