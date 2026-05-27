#include "mainQuest.h"

cChapterManager	g_chapterManager;

char	g_strMainQuestSwitch[c_iMaxMainQuestValueCount][32];
char	g_strMainQuestValue[c_iMaxMainQuestSwitchCount][32];

cChapterManager::cChapterManager()
{
	reset();
}

cChapterManager::~cChapterManager()
{
}

BOOL
cChapterManager::addChapter(char *_lpstrName)
{
	if	(m_iChapterCount		>=	c_iMaxChapterCount)
		return	FALSE;

	cChapterInfo	*lpChapter	=	&m_aChapterList[m_iChapterCount];

	lpChapter->reset();

	lpChapter->modifyChapterName(_lpstrName);

	m_iChapterCount++;

	return	TRUE;
}

BOOL
cChapterManager::modifyChapterName(int _iChapterIndex,char *_lpstrName)
{
	if	(_iChapterIndex	>=	m_iChapterCount)
		return	FALSE;

	m_aChapterList[_iChapterIndex].modifyChapterName(_lpstrName);

	return	TRUE;
}

BOOL
cChapterManager::changeChapterComment(int _iChapterIndex,char *_lpstrComment)
{
	if	(_iChapterIndex	>=	m_iChapterCount)
		return	FALSE;

	m_aChapterList[_iChapterIndex].changeComment(_lpstrComment);

	return	TRUE;
}

BOOL
cChapterManager::removeChapter(int _iChapterIndex)
{
	if	(_iChapterIndex	>=	m_iChapterCount)
		return	FALSE;

	for (int i=_iChapterIndex;i<m_iChapterCount-1;i++)
		m_aChapterList[i].copy(&m_aChapterList[i+1]);

	m_aChapterList[m_iChapterCount-1].reset();

	m_iChapterCount--;

	return	TRUE;
}

BOOL
cChapterManager::removeHighQuest(int _iChapterIndex,int _iHighQuest)
{
	if	(_iChapterIndex	>=	m_iChapterCount)
		return	FALSE;

	return	m_aChapterList[_iChapterIndex].removeHighQuest(_iHighQuest);
}

void
cChapterManager::reset()
{
	for (int i=0;i<c_iMaxMainQuestValueCount;i++)
	{
		sprintf(g_strMainQuestValue[i],"quest value #%.3d",i);
		sprintf(g_strMainQuestSwitch[i],"quest switch #%.3d",i);
	}

	m_iChapterCount	=	0;
}

cChapterInfo*
cChapterManager::get(int _iChapterIndex)
{
	if	(_iChapterIndex	>=	m_iChapterCount)
		return	NULL;

	return	&m_aChapterList[_iChapterIndex];
}
