#ifndef _tool_h
#define	_tool_h

#include	"CNemo.h"

#define	dMAX_EXPERIENCE_INFO_COUNT	1024

class	cGetQuestExperienceInfo
{
public:
	CTimeInfo	m_time;
	DWORD		m_dwGetExperience;

	void		reset();
};

class	cHackingToolUserInfo
{
public:
	char		m_strId[20];
	char		m_strName[20];
	WORD		m_wIsSuspected;
	int			m_iCount;

	cGetQuestExperienceInfo	m_aList[dMAX_EXPERIENCE_INFO_COUNT];

	void	reset();
	BOOL	addInfo(CTimeInfo *_lpTime,int _iIncreaseExp);
};


class	cHackingToolUserList
{
public:
	int						m_iCount;
	cHackingToolUserInfo	m_aUser[1024*10];

	void	reset();
	void	run();
	BOOL	addInfo(char *_lpstrId,char *_lpstrName,CTimeInfo *_lpTime,int _iIncreaseExp);
};

extern	cHackingToolUserList	tool;

#endif