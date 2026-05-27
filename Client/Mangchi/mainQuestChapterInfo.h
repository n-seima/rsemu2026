#ifndef	_classMainQuestChapterInfo_H
#define	_classMainQuestChapterInfo_H

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

			cChapterInfo();

	void	reset();

	void	modifyChapterName(char *_lpstrName);
	BOOL	addHighQuest(char *_lpstrName);
	BOOL	modifyHighQuestName(int _iChapterIndex,char *_lpstrName);
	void	changeComment(char *_lpstrComment);
	void	changeEpilogComment(char *_lpstrComment);

	void	copy(cChapterInfo *_lpChapter);
	BOOL	removeHighQuest(int _iIndex);
};


#endif