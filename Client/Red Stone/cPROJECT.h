#ifndef _classPROJECT_H
#define _classPROJECT_H

#include "cNEMO.H"

class	cPROJECT
{
public:
	char	m_strName[128];
	cNUX	*m_lpNux;
	int		m_iCloseQuestCount;
	WORD	m_awCloseQuestList[1024];

	void	init(cNUX *_lpNux=NULL);
	BOOL	setProjectFolder();					//	프로젝트 폴더로 들어간다.
	BOOL	setMapFolder();						//	맵 폴더로 들어간다.
	BOOL	readCloseQuestList();
	BOOL	readCorrectJobData();
	BOOL	load(char *_strName,cNUX *_lpNux=NULL);
	void	resetItemPrefixData();
	BOOL	loadItem();
};

extern	cPROJECT	g_project;

#endif