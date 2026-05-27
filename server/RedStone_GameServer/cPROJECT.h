#ifndef _classPROJECT_H_
#define _classPROJECT_H_

#include "SFC.H"
#include "CKarma.H"

class cPROJECT
{
public:
	char				m_strName[128];
	char				m_strMapFolder[512];
	char				m_strDungeonFolder[512];
	char				m_strProjectFolder[512];
	int					m_iCloseQuestCount;
	WORD				m_awCloseQuestList[1024];
	CKarmaContainer		m_kcMonsterDeath,m_kcMonsterGen;

						cPROJECT()
						{
							if(s_isExist)	MessageBox(GetActiveWindow(),"already exist cPROJECT class","error",MB_ICONERROR);

							s_isExist	=	TRUE;
						}

	BOOL				setProjectFolder();
	BOOL				setMapFolder();
	BOOL				setDungeonFolder(char *_strDungeonMapFolder=NULL);
	char*				exportMapFileName(char *_strFn);	//	_strFn에서 프로젝트 맵 폴더를 제외한 맵 이름을 뽑아낸다.

	BOOL				loadProject(char *fn);
	BOOL				isCloseQuest(int _iQuestSerial);	

	static	BOOL		s_isExist;
};

extern	cPROJECT		g_project;		//	프로젝트 데이터

#endif