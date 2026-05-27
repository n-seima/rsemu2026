#ifndef	_classMainQuest_H
#define	_classMainQuest_H

#include	"SFC.H"
#include	"quest_common.H"

class	cChapterInfo
{
public:
	int		m_iHighQuestCount;
	char	m_strName[c_iChapterNameLength];
	char	m_strComment[c_iChapterCommentLength];
	char	m_strEpilogComment[c_iChapterEpilogCommentLength];

	char	m_strHighQuest[c_iMaxHighQuestCount][c_iHighQuestNameLength];
};

class	cChapterManager
{
public:
	int				m_iChapterCount;
	cChapterInfo	m_aChapterList[c_iMaxChapterCount];

					cChapterManager();
	virtual			~cChapterManager();

	int				getChapterCommentSize(int _iChapter);
	int				getChapterEpilogCommentSize(int _iChapter);
	char*			getChapterName(int _iIndex);
	char*			getChapterComment(int _iIndex,int _iCharCount=-1);
	char*			getChapterEpilogComment(int _iIndex,int _iCharCount=-1);
	char*			getHighQuestName(int _iChapter,int _iHighQuest);
	inline int		getChapterCount(){return m_iChapterCount;}	// √¿ ∞πº∏¶ ∏Æœ.
	inline int		getPartCount(int _iChapterIndex){return m_aChapterList[_iChapterIndex].m_iHighQuestCount;}
};

extern	cChapterManager	g_chapterManager;


#endif