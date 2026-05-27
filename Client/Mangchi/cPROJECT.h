#ifndef _classPROJECT_H
#define _classPROJECT_H

#include "SFC.H"
#include "cWINDOW.H"
#include "cLISTCONTROL.H"
#include "cJOB.H"
#include "setItem.H"
#include "cITEM.H"
#include "cITEM_PREFIX.H"
#include "arca.H"
#include "pieceItem.H"
#include "cKARMA.H"


#define	dWORK_SAVE_MAP			0
#define	dWORK_LOAD_MAP			1
#define	dWORK_ADD_MAP			2
#define	dWORK_SAVE_PROJECT		3
#define	dWORK_LOAD_PROJECT		4

class	CQuest;
class	cSecretDungeonManager;
class	cChapterManager;
class	cBookManager;

#define		dMAP_LIST_COUNT		1024

class cPROJECT
{
public:
	static	char				s_strName[128];
	static	CKarmaContainer		s_kcMonsterDeath;
	static	CKarmaContainer		s_kcMonsterGen;

	static	BOOL				NewProject(HINSTANCE hinst,HWND hwnd,BOOL isFirst=	FALSE);
	static	BOOL				GetProject(HWND hwnd);
	static	BOOL				LoadBlockedTileInfo();



	static	void				GetMapFolder(char *_lpstrFolder);
	static	void				GetDungeonFolder(char *_lpstrFolder);

	static	BOOL				SetScenarioFolder();			//	프로젝트 리스트가 있는 폴더
	static	BOOL				SetProjectFolder();				//	프로젝트 폴더
	static  BOOL				SetTempMapFolder(char *_lpstrDestFolder = NULL);		// 09.07.24 추가

	static	BOOL				SetMapFolder(char *_lpstrDestFolder=NULL);			//	프로젝트의 맵들이 저장된 폴더
	static	BOOL				SetDungeonFolder();				//	프로젝트의 던젼설정들이 저장된 폴더

	static	char*				SelectMap(HINSTANCE hinst,HWND hwnd);	//	맵을 선택한다.

	static	char*				ExportMapFileName(char *_strFn);	//	_strFn에서 프로젝트 맵 폴더를 제외한 맵 이름을 뽑아낸다.

	static	void				SaveBook(cFILE *_lpFile,cBookManager *_lpBM);
	static	void				LoadBook(cFILE *_lpFile,BOOL _bIsOldData,cBookManager *_lpBM);

	static	BOOL				SaveProject();
	static	BOOL				LoadProject();
	static	char				*GetFile(char *str);

	static	LRESULT CALLBACK	s_procNewProject(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/////////////////////////////////////////////////////////////////////////////////
//	맵 저장/불러오기,프로젝트 저장/불러오기 인터페이스
/////////////////////////////////////////////////////////////////////////////////
	static	char				s_strFileName[128];		//	화일을 불러오거나 저장할대 화일 이름
	static	char				s_strMapFolder[256];	//	마지막으로 저장하거나 불로온 맵 화일의 폴더

//	맵 저장
	static	BOOL				SaveMap(HINSTANCE hinst,HWND hwnd);
	static	BOOL				SaveMap(char *fn=NULL,int index=0xffff,char *_lpstrDestFileName=NULL,BOOL _bIsLocalize=FALSE);

	static	BOOL				IsMapFolder(char *_lpstrFoder);	//	프로젝트에 속한 맵 폴더 맞다.


//	맵 불러오기
	static	char *				GetMapName(HINSTANCE hinst,HWND hwnd);
	static	BOOL				LoadMap(HINSTANCE hinst,HWND hwnd);
	static	BOOL				LoadMap(char *fn,int index=0xffff);

//	맵 추가하기
	static	BOOL				AddMap(HINSTANCE hinst,HWND hwnd);
	static	BOOL				AddMap(char *fn);


//	프로젝트 불러오기
	static	BOOL				LoadProject(HINSTANCE hinst,HWND hwnd);
	static	BOOL				LoadProject(char *fn,CQuest *_lpQuest=NULL,CQuest *_lpArbeit=NULL,cSecretDungeonManager *_lpSDM=NULL,
											cChapterManager *_lpCM=NULL,cBookManager *_lpBM=NULL,CKarmaContainer *_lpKC=NULL,CKarmaContainer *_lpKCMopnsterGen=NULL,BOOL _bIsForLocalize=FALSE);

//	프로젝트 저장하기
	static	BOOL				SaveProject(HINSTANCE hinst,HWND hwnd);
	static	BOOL				SaveProject(char *fn,char *_lpstrDestFolder=NULL,CQuest *_lpQuest=NULL,CQuest *_lpArbeit=NULL,cSecretDungeonManager *_lpSDM=NULL,
											cChapterManager *_lpCM=NULL,cBookManager *_lpBM=NULL,CKarmaContainer *_lpKC=NULL,CKarmaContainer	*_lpKCMonsterGen=NULL,BOOL _bIsLocalize=FALSE);

//	아이템 데이터 저장/불러오기
	static	BOOL				SaveItem(char *_lpstrFileName=NULL,CCustomItemManager *_lpPremiumItem=NULL,CCustomItemManager *_lpCustomItem=NULL,
										cITEM_PREFIX_MANAGER *_lpIPM=NULL,cArcaManager *_lpAM=NULL,cItemSetManager *_lpISM=NULL,cPieceItemManager *_lpPIM=NULL,cBASIC_ITEM *_lpBIL=NULL,BOOL _bIsLocalize=FALSE);
	static	BOOL				LoadItem(char *_lpstrFileName=NULL,CCustomItemManager *_lpPremiumItem=NULL,CCustomItemManager *_lpCustomItem=NULL,
										cITEM_PREFIX_MANAGER *_lpIPM=NULL,cArcaManager *_lpAM=NULL,cItemSetManager *_lpISM=NULL,cPieceItemManager *_lpPIM=NULL,cBASIC_ITEM *_lpBIL=NULL);

//	직업 데이터 저장하기
	static	BOOL				ReadCorrectJobData(cJOB_DEN *_lpDen);
	static	BOOL				SaveJob(cJOB_DEN *_lpJob=NULL,char *_lpstrFn=NULL);
	static	BOOL				LoadJob(cJOB_DEN *_lpJobDen=NULL,char *_lpstrFileName=NULL);

	static	UINT APIENTRY		hookOpenMap(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam);
};

#endif