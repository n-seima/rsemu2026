#ifndef	_classMainQuest_H
#define	_classMainQuest_H

#include	"mainQuestChapterInfo.H"

class	cChapterManager
{
public:
	int				m_iChapterCount;
	cChapterInfo	m_aChapterList[c_iMaxChapterCount];

					cChapterManager();
	virtual			~cChapterManager();

	cChapterInfo*	get(int _iIndex);
	BOOL			addChapter(char *_lpstrName);
	BOOL			modifyChapterName(int _iChapterIndex,char *_lpstrName);
	BOOL			changeChapterComment(int _iChapterIndex,char *_lpstrComment);
	BOOL			removeChapter(int _iChapterIndex);
	BOOL			removeHighQuest(int _iChapterIndex,int _iHighQuest);

	void			reset();
};

extern	cChapterManager	g_chapterManager;

extern	char	g_strMainQuestValue[c_iMaxMainQuestValueCount][c_iMainQuestValueNameLength];
extern	char	g_strMainQuestSwitch[c_iMaxMainQuestSwitchCount][c_iMainQuestSwitchNameLength];

#endif