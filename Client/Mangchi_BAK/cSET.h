#ifndef _classSET_H
#define _classSET_H

#include "SFC.H"
#include "cWINDOW.H"
#include "cMAP.H"
#include "cJOB.H"
#include "cAREA.H"
#include "cITEM.H"
#include "cITEM_PREFIX.H"
#include "cSKILL.H"
#include "cLISTCONTROL.H"
#include "CQuest.H"

class	cSET
{
public:
	static	void		Init(HINSTANCE hinst,HWND hwnd);

	static	BOOL		NewMap(HINSTANCE hinst,HWND hwnd);
	static	BOOL		ResetMap(HINSTANCE hinst,HWND hwnd);
	static	BOOL		SetMap(HINSTANCE hinst,HWND hwnd);

	static	BOOL		_isAddMap;
	static	HINSTANCE	s_hInst;
	static	HWND		s_hMasterWnd;
};

class	cNEWMAP	:	public cSET
{
public:
	static	cMAP_INFO	_MapInfo;
	static	char		_strFieldName[512],_strOverlapName[512];

	static	BOOL		Init(HWND hwnd);
	static	BOOL		Setting(HWND hwnd);

	static	LRESULT CALLBACK	WndProc(HWND,UINT,WPARAM,LPARAM);
};

class	cSETMAP		:	public cSET
{
public:
	static	cMAP_INFO	_MapInfo;

	static	BOOL		Init(HWND hwnd);
	static	BOOL		Setting(HWND hwnd);

	static	LRESULT CALLBACK	WndProc(HWND,UINT,WPARAM,LPARAM);
};


//
//	카르마 값 설정
//
class	cSETVALUE	:	public cSET
{
public:
	static	cLISTCONTROL	s_ValueList;		//	카르마 값 리스트
	static	cDIBWND			s_ListView;			//	DIB 우니도우
	static	HWND			s_hTab;				//	탭 컨트롤의 윈도우 핸들
	static	int				s_iSelectTab;

	static	BOOL			SetValue(HINSTANCE hinst,HWND hwnd);	//	띄우자
	static	void			AddValue(HWND hwnd);

	static	void			InitListBox(HWND hwnd);					//	초기화
	static	void			DrawList(HWND hDlg,BOOL value=FALSE);	//	리스트 그리기
	static	void			ChangeTab(HWND hdlg);
	static	void			RemoveValue(HWND hwnd);

	static	LRESULT CALLBACK	s_procListBox(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

class	cSETQUEST	:	public cSET
{
public:
	static	cLISTCONTROL		s_listQuest;
	static	cDIBWND				s_dibQuestObject;
	static	int					s_iState,s_iSelectQuest,s_iSelectQuestType;				//	수정이냐? 아님 추가냐?
	static	CQuest				*s_lpQuest;

	static	BOOL				SetQuest(HINSTANCE hinst,HWND hwnd);	//	띄우자

	static	void				ChangeQuestType(HWND hwnd);
	static	void				InitQuestBox(HWND hwnd);				//	퀘스트 설정 창 초기화
	static	void				SetSelectQuest();

	static	void				DrawQuest(HWND hDlg);
	static	void				RemoveQuest(HWND hwnd);

	static	LRESULT CALLBACK	s_procQuestBox(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static	LRESULT CALLBACK	s_procListView(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procObjectView(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//	퀘스트 추가
	static	BOOL				NewQuest(HINSTANCE hinst,HWND hwnd);	//	띄우자
	static	void				InitNewQuestBox(HWND hwnd);				//	퀘스트 설정 창 초기화
	static	BOOL				AddQuest(HWND hwnd);
	static	BOOL				ReviseQuest(HWND hwnd);
	static	void				FilterQuest(HWND _hDlg);		// 09.08.10

	static	LRESULT CALLBACK	s_procAddQuest(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procQuestList(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static	LRESULT CALLBACK	s_procQuestHelpBox(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static	void				InitQuestHelpBox(HWND hwnd);
};

class	cSET_AREA	:	public	cSET
{
public:
	static	cRECT					s_Rectangle;
	static	BOOL					s_isEditArea;
	static	BOOL					s_isSpecialArea;
	static	CConversationContainer	s_cc;

	static	BOOL				SetArea(HINSTANCE hinst,HWND hwnd,cRECT *rect,BOOL edit=FALSE,BOOL special=FALSE);

	static	void				InitSetAreaBox(HWND hwnd);
	static	BOOL				GetAreaData(HWND hwnd);
	static	void				Reset(HWND hDlg,cAREA_INFO *lpArea);
	static	void				InitPortalAreaForm(HWND hDlg,cAREA_INFO *lpArea,cRECT *_lpRect);
	static	void				InitDoorAreaForm(HWND hDlg,cAREA_INFO *lpArea,cRECT *_lpRect);
	static	void				InitArcaAreaForm(HWND hDlg,cAREA_INFO *lpArea,cRECT *_lpRect);

	static	void				InitTrapAreaForm(HWND hDlg,cAREA_INFO *lpArea,cRECT *_lpRect);
	static	void				SetSpecialArea(HWND hDlg,int kind);

	static	LRESULT CALLBACK	s_procSetArea(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
// 10.01.15 추가
class	cSET_AREA_EDIT	:	public	cSET
{
public:
	static	HWND					s_hWnd;
	static	CConversationContainer	s_cc;
	static	cLISTCONTROL			s_listArea;
	static	int						s_iAreaListWndPos, s_iSelectAreaList, s_iAreaCount;
	static	BOOL					s_bEnableDialog;

	static	BOOL				SetAreaEdit(HWND hwnd);
	static	void				InitAreaData(HWND hWnd);
	static	void				InitAreaDataBox();
	static	void				ShowAreaEdit(){		ShowWindow(s_hWnd, SW_SHOW);	}
	static	void				ResetAreData();
	static	void				Reset(HWND hDlg,cAREA_INFO *lpArea);
	static	void				InitArcaAreaForm(HWND hDlg,cAREA_INFO *_lpArea,cRECT *_lpRect);
	static	void				InitDoorAreaForm(HWND hDlg,cAREA_INFO *lpArea,cRECT *_lpRect);
	static	void				InitPortalAreaForm(HWND hDlg,cAREA_INFO *lpArea,cRECT *_lpRect);
	static	void				InitTrapAreaForm(HWND hDlg,cAREA_INFO *lpArea,cRECT *_lpRect);
	static	BOOL				GetAreaData(HWND hDlg);
	static	void				MoveArea();

	static	LRESULT CALLBACK	s_procSetAreaEdit(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procAreaListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
#define	dJOB_LIST_HERO		0
#define	dJOB_LIST_NPC		1
#define	dJOB_LIST_MONSTER	2

class	cSELECT_JOB	:	public cSET
{
public:
	static	int					s_iWidth,s_iHeight;
	static	int					s_iJobBoxWidth,s_iJobBoxHeight;	
	static	int					s_iJPW,s_iJPH;
	static	int					s_iLeft,s_iTop;
	static	int					s_iMode,s_iJobCount;
	static	cBODY_DATA			*s_lpDefaultJob;

	static	cDIBWND				s_JobListView;
	static	cSCROLL_BAR			s_ScrollBar;

	static	int					SelectHeroJob(HINSTANCE hinst,HWND hwnd);
	static	int					SelectNPCJob(HINSTANCE hinst,HWND hwnd);
	static	int					SelectMonsterJob(HINSTANCE hinst,HWND hwnd);

	static	void				InitListBox(HWND hwnd);
	static	BOOL				SelectJob(HWND hwnd,int x,int y);

	static	void				Draw(HWND hwnd);

	static	LRESULT CALLBACK	s_procListBox(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

///////////////////////////////////////////////////////////////////////////////////////////
//	어빌러티 설정
///////////////////////////////////////////////////////////////////////////////////////////
class	cSET_ABILITY	:	public	cSET
{
public:
	static	cABILITY			*s_lpAbility;
	static	int					s_iBaseJob,s_iSelectAbility	;
	static	cLISTCONTROL		s_AbilityList,s_SkillList;

	static	BOOL				SetAbility(HINSTANCE hinst,HWND hwnd,int basejob,cABILITY *lpAbility);
	static	BOOL				InitSetAbility(HWND hwnd);
	static	void				InitAbilityList();
	static	void				InitSkillList();

	static	void				AddAbility(HWND hwnd);
	static	void				RemoveAbility(HWND hwnd);

	static	void				DrawList(HWND hDlg);
	static	void				ArrangementAbility();	//	설정한 어빌러티 데이터를 읽어 오고 정렬 시킨다.

	static	LRESULT CALLBACK	s_procSetAbility(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	ProcAbilityListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	////////////////////////////////////////
	//	어빌러티 레벨과 경험치 설정
	////////////////////////////////////////
	static	void				SetAbilityData(HWND hwnd);

	static	LRESULT CALLBACK	s_procSetAbilityData(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};


///////////////////////////////////////////////////////////////////////////////////////////
//	직업 리스트
///////////////////////////////////////////////////////////////////////////////////////////

class	cJOB_LIST		:	public	cSET
{
public:
	static	HWND				s_hWnd;									//	윈도우 핸들
	static	cDIBWND				s_dibBodyList,s_dibJobData;					//	dib윈도우들
	static	cJOB				s_makeJob,s_copyJob;						//	작업중인 직업과 카피해 놓은 직업
	static	int					s_iSelectBodyGroup,s_iSelectJobDataTab;		//	선택한 탭
	static	int					s_iBodyPerWindow;							//	한번에 나올 수 있는 몸뚱아리 수
	static	int					s_iBodyWidth,s_iBodyHeight;					//	몸뚱아리 사이즈
	static	int					s_aSelectBody[3];							//	선택한.. 몸뚱아리-_-
	static	int					s_aScrollBarPos[3];							//	스크롤바의 위치
	static	cSCROLL_BAR			s_sbBodyList;								//	몸뚱아리 리스트의 스크롤바
	static	cLISTCONTROL		s_listJob;									//	직업 리스트

	static	void				Init(HWND hwnd,HINSTANCE hinst);		//	으흠~
	static	void				InitJobList(HWND hwnd);					//	직업 리스트 초기화
	static	void				DrawBodyList(HWND hDlg);				//	바디-_- 리스트
	static	void				DrawJobList(HWND hDlg);					//	직업 리스트
	static	void				DrawJobData(HWND hDlg);					//	직업 데이터 그려주기
	static	void				Draw(HWND hwnd);

	static	BOOL				SelectBody(HWND hwnd,int x,int y);		//	몸통 선택
	static	void				ChangeBodyGroup(HWND hDlg);
	static	void				AddJob(HWND hDlg);						//	직업 추가
	static	void				EditJob(HWND hDlg);
	static	void				RemoveSelectJob(HWND hDlg);
	static	void				CopySelectJob(HWND hDlg);

	static	void				UpdateList(HWND hDlg);					//	직업 리스트 업데이트

	static	BOOL				DecodeMonsterScriptFile(HWND _hDlg);				//	
	static	BOOL				DecodeMonsterScriptFile(HWND _hDlg,char *_strFName);//	
	static	BOOL				DecodeMonsterScript(char *_strScript,BOOL _isCry);	//	

	static	LRESULT CALLBACK	s_procSetJobList(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procDibBodyList(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procDibJobData(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);

/////////////////////////////////////////////////////////////////////////////////////
//	영웅의 데이터 설정
/////////////////////////////////////////////////////////////////////////////////////

	static	void				InitAddJob(HWND hDlg);
	static	BOOL				GetJobData(HWND hDlg);

	static	LRESULT CALLBACK	s_procJob1Dib(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procJob2Dib(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);

	// 09.12.14
	static	LRESULT CALLBACK	s_procJobDataList(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam);
};

///////////////////////////////////////////////////////////////////////////////////////////
//	직업 데이터 설정
///////////////////////////////////////////////////////////////////////////////////////////
#define	dMAKE_JOB_HERO		0
#define	dMAKE_JOB_NPC		1
#define	dMAKE_JOB_MONSTER	2

#define dGRADE_MONSTER_HARD		0
#define dGRADE_MONSTER_NORMAL	1
#define dGRADE_MONSTER_WEAK		2


class	cSET_JOB		:	public	cSET
{
public:
	static	BOOL				s_isEditJob;								//	직업 수정중이냐?
	static	BOOL				s_isFailedWork;								//	작업 실패
	static	int					s_iJobGroup;								//	직업 그룹
	static	cJOB				s_makeJob;									//	작업중인 직업과 카피해 놓은 직업

	static	cJOB*				NewJob(HWND hwnd,int _iJobGroup,int _iBody,int _iEditJob=0xffff);	//	으흠~

/////////////////////////////////////////////////////////////////////////////////////
//	영웅의 데이터 설정
/////////////////////////////////////////////////////////////////////////////////////
	static	cDIBWND				s_dibJob1List,s_dibJob2List;					//	dib윈도우들
	static	cSCROLL_BAR			s_sbJob1List,s_sbJob2List;
	static	int					s_iBodyWidth,s_iBodyHeight;						//	직업 리스트에 찍힐 이미지의 가로 세로 사이즈
	static	int					s_iBodyPerWindow;								//	한번에 나올 수 있는 몸뚱아리 수
	static	int					s_iSelectJob1Body;			//	선택한 직업 2의 몸뚱아리
	static	int					s_iSelectJob1Index;			//	선택한 직업 2의 몸뚱아리
	static	int					s_iSelectSkillCategory;
	static	int					s_iSelectSkill;
	static	cLISTCONTROL		s_listAbility,s_listSkill;
	static	HWND				s_hAbilityListTab;
	static	int					s_iSelectJob;

	static	void				InitSetJobData(HWND hDlg);
	static	void				InitAbilityList(HWND hDlg);
	static	void				UpdateAbilityList(HWND _hDlg);
	static	void				AddAbility(HWND hwnd);
	static	void				RemoveAbility(HWND hwnd);

	static	BOOL				GetJobData(HWND hDlg);

	static	LRESULT CALLBACK	s_procSetJobData(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procList1(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/////////////////////////////////////////////////////////////////////////////////////
//	영웅의 어빌러티 설정
/////////////////////////////////////////////////////////////////////////////////////

	static	void				DrawAbilityList(HWND hwnd);

	static	void				InitJobAbility(HWND hDlg);

	static	LRESULT CALLBACK	s_procAbilityList(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procList2(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

///////////////////////////////////////////////////////////////////////////////////////////
//	죽었을때 데이터
///////////////////////////////////////////////////////////////////////////////////////////
	static	cLISTCONTROL		s_listDropItemList,s_listAddItemList;	//	드롭 아이템 리스트와 추가 가능 아이템 리스트
	static	HWND				s_hDeathDataDlg;

	static	void				AddDropItem(HWND hDlg);;				//	아이템 추가
	static	void				RemoveDropItem(HWND hDlg);				//	아이템 제거

	static	BOOL				InitDeathData(HWND hDlg);				
	static	void				UpdateDropItemList(HWND hDlg,int _iSelect=0);	//	떨어지는 아이템 리스트 업데이트
	static	void				UpdateAddItemList(HWND hDlg);					//	아이템 리스트 업데이트
	static	void				DrawDeathData(HWND hDlg);

	static	int					GetMonsterMagicConcept(cJOB *_lpEditJob);
	static	int					GetMonsterPhysicalAttackConcpet(cJOB *_lpEditJob);
	static	int					GetMonsterDefenceConcept(cJOB *_lpEditJob);

	static	LRESULT CALLBACK	s_procDropItemList(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procDeathData(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
};


///////////////////////////////////////////////////////////////////////////////////////////
//	영웅 설정
///////////////////////////////////////////////////////////////////////////////////////////

#define	dMAKE_CHARACTER_HERO		0
#define	dMAKE_CHARACTER_NPC			1
#define	dMAKE_CHARACTER_MONSTER		2

class	cSET_CHARACTER	:	public	cSET
{
public:
	static	HWND				s_hWnd;
	static	cDIBWND				s_dibBodyList,s_dibJobData;
	static	cCHARACTER			s_makeCharacter;
	static	cLISTCONTROL		s_listJob;									//	직업 리스트
	static	cCHARACTER_SET*		s_lpCharacterSet;
	static	cSCROLL_BAR			s_sbBodyList;	//	직업 리스트의 스크롤바
	static	int					s_iMakeWhat;
	static	int					s_iBodyPerWindow;
	static	int					s_iBodyCount;		//	몸뚱아리 수
	static	int					s_iSelectBody;		//	선택한 몸뚱아리
	static	int					s_iJobGroup;		//	직업 그룹 
	static	int					s_iBodyWidth,s_iBodyHeight;
	static	int					s_iSelectJobDataTab;
	static	BOOL				s_isEditCharacter;	//	캐릭터를 수정한다.

	static	void				Draw();
	static	void				DrawBodyList();
	static	void				DrawJobList();
	static	void				DrawJobData();
	static	void				UpdateList();

	static	BOOL				SelectBody(HWND hwnd,int _iX,int _iY);	//	몸통을 선택한다.

	static	BOOL				AddCharacter(HWND hwnd,int _iJobGroup,cCHARACTER_SET	*_lpCharacterSet);
	static	BOOL				EditCharacter(HWND hwnd,cCHARACTER_SET	*_lpCharacterSet);
	static	BOOL				Init(HWND hwnd,int _iJobGroup);
	
	static	void				InitSetCharacter(HWND hDlg);
	static	BOOL				GetCharacterData(HWND hDlg);
	static	void				SyncCharacterName(HWND _hDlg);


	static	LRESULT CALLBACK	s_procSetCharacter(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procDibBodyList(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procDibJobData(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


///////////////////////////////////////////////////////////////////////////////////////////
//	필드 캐릭터 설정
///////////////////////////////////////////////////////////////////////////////////////////
	static	int					s_iSelectCondition;
	static	CKarmaContainer		s_aEvent[eMECC_COUNT];
	static	CKarma				*s_lpStoredKarma;
	static	cCHARACTER			*s_lpEditCharacter;

	
	static	void				SetEvent(HWND _hDlg,cCHARACTER *_lpEditCharacter);
	static	void				InitSetCharacterEventData(HWND _hDlg);
	static	void				SelectCondition(HWND hDlg);	//	상황 선택
	static	void				RemoveEvent(HWND hDlg);
	static	void				CompleteWork(HWND hDlg);

	static	void				CopyEvent(HWND hDlg);
	static	void				PasteEvent(HWND hDlg);
	static	void				DownEvent(HWND _hDlg);
	static	void				UpEvent(HWND _hDlg);

	static	LRESULT CALLBACK	s_procSetCharacterEventData(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
};

///////////////////////////////////////////////////////////////////////////////////////////
//	필드 캐릭터 설정
///////////////////////////////////////////////////////////////////////////////////////////

class	cSET_FIELD_CHARACTER	:	public	cSET
{
public:
	static	HWND					s_hWnd;
	static	cACTOR*					s_lpEditActor;
	static	int						s_iEditActor;
	static	BOOL					s_isEditMultiActor;
	static	CConversationContainer	g_backupConversationContainer;

	static	BOOL					Init(HWND hwnd,cACTOR *_lpActor,BOOL _isMulti=FALSE);
	static	BOOL					SetData(HWND hwnd);
	static	BOOL					GetData(HWND hDlg);
	static	BOOL					AddDialog(HWND _lphWnd,BOOL _isEdit=FALSE);
	static	void					ShowGenerateMethodWindow(HWND _hDlg, BOOL _bShow);
	static	LRESULT CALLBACK		DlgProc(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
};

///////////////////////////////////////////////////////////////////////////////////////////
//	필드 캐릭터 설정   09.11.30 추가
///////////////////////////////////////////////////////////////////////////////////////////
class	cSET_CHARACTER_TREE_DATA	:	public	cSET
{
public:
	static	HWND				s_hWnd;	
	static	cTREEVIEW			s_treeView;
	static	cCHARACTER			*s_lpEditCharacter;
	static	CKarma				*s_lpStoredKarma, *s_lpSelectKarma, s_aStoredKarma;
	static	CKarmaContainer		s_aEvent[eMECC_COUNT];
	static	cDIBWND				s_hKarmaDetailWnd;
	static	int					s_iSelectCondition, s_iCopiedKaramaCount;
	static	int					s_iKarmaDetailWndPos;
	static	BOOL				s_bIsSelectKarma;

	static	void				SetTreeEvent(HWND _hDlg,cCHARACTER *_lpEditCharacter);
	static	void				InitSetCharacterEventTreeData(HWND _hDlg);
	static	void				UpdateKarma(HWND _hDlg);
	static	void				FillTree();
	static	void				DrawKarmaDetail();
	
	static	void				RemoveEvent(HWND _hDlg);
	static	void				UpEvent(HWND _hDlg);
	static	void				DownEvent(HWND _hDlg);
	static	void				CopyEvent(HWND _hDlg);
	static	void				PasteEvent(HWND _hDlg);
	static	void				CompleteWork(HWND _hDlg);

	static	LRESULT CALLBACK	s_procSetCharacterEventTreeData(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK	s_ProcKarmaDetailWnd(HWND,UINT,WPARAM,LPARAM);
};

///////////////////////////////////////////////////////////////////////////////////////////
//	폰트 설정  09.12.18 추가
///////////////////////////////////////////////////////////////////////////////////////////
#define	dCOLOR_OPTION_MAX		32
#define dSLIDER_MIN_POS			0
#define dSLIDER_MAX_POS			255

enum{
	eCOLOR_FILENAME			,			// 파일이름
	eCOLOR_MAPNAME			,			// 맵이름
	eCOLOR_AREA_COUNT		,			// 영역수
	eCOLOR_CHARACTER_COUNT	,			// 캐릭터수
	eCOLOR_SAVE_YESNO		,			// 임시 저장 여부
	eCOLOR_MAP_DRAW			,			// 맵 그리기
	eCOLOR_TILE_INDEX		,			// 타일인덱스
	eCOLOR_GIRD				,			// 격자
	eCOLOR_DOOR				,			// DOOR
	eCOLOR_NPC_NAME			,			// NPC 이름
	eCOLOR_MONSTER_NAME		,			// MONSTER 이름
	eCOLOR_NPC_TYPE			,			// NPC 타입
	eCOLOR_LEADER			,			// 리더나 피보호자
	eCOLOR_SKIN				,			// 스킨
	eCOLOR_OBJECT_SERIAL	,			// 오브젝트 시리얼
	eCOLOR_AREA_NAME		,			// 영역이름
	eCOLOR_AREA_NORMAL		,			// 일반영역
	eCOLOR_AREA_NOTICE_BOARD,			// 게시판
	eCOLOR_AREA_DOOR		,			// 문
	eCOLOR_PORTAL_EXIT		,			// 출구
	eCOLOR_PORTAL_LOAD_MAP	,			// 불러들일 맵
	eCOLOR_PORTAL_CMAP		,			// 연결된 맵
	eCOLOR_PORTAL_HIDDEN	,			// 감춰진 포탈
	eCOLOR_PORTAL_HIDDEN_IN	,			// 감춰진 내부 이동 포탈
	eCOLOR_PORTAL_ONE_WAY	,			// 일반통행 포탈
	eCOLOR_AREA_TRAP		,			// 함정
	eCOLOR_EVENT_AREA		,			// 이벤트 영역
	eCOLOR_ARCA				,			// 보물상자
	eCOLOR_NAME_AREA		,			// 이름표시 영역
	eCOLOR_MONSTER_ACTIVE	,			// 몬스터 활동 영역
	eCOLOR_MONSTER_INFO		,			// 몬스터 영역 정보	
	eCOLOR_AREA_ETC			,			// 나머지 영역들
};

class	cSET_FONT	:	public	cSET
{
public:
	static	HWND				s_hWnd, s_hSliderR, s_hSliderG, s_hSliderB;
	static	COLORREF			s_ColorOption[dCOLOR_OPTION_MAX], s_OldColorOption[dCOLOR_OPTION_MAX], s_SampleColor;
	static	HDC					s_Paintdc, s_BackPaintdc;
	static	HBITMAP				s_hbackbitmap, s_holdbackbitmap;
	static	HBRUSH				s_MyBrush, s_OldBrush;
	static	cDIBWND				s_dibSampleText;

	static	int					s_iColorOptionCount, s_iSelectFont;
	
	static	void				InitFont(HWND hwnd);
	static	void				InitFontData(HWND hWnd);
	static	void				SaveOldData();
	static	void				SaveFontData(BOOL _type = TRUE);
	static	void				ShowDialog();
	static	void				ResetColorOption();
	static	void				DrawSampleText();

	static	LRESULT CALLBACK	s_procSetFont(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK	s_ProcSampleTextWnd(HWND,UINT,WPARAM,LPARAM);

};


///////////////////////////////////////////////////////////////////////////////////////////
//	기본 아이템 설정
///////////////////////////////////////////////////////////////////////////////////////////
class	cSET_BASIC_ITEM
{
public:
	static	HWND				s_hWnd;
	static	int					s_iSelectItem,s_iSelectKind;
	static	cSCROLL_BAR			s_ScrollBar,s_scrollItemInfo;
	static	cBASIC_ITEM*		s_lpSelectItem;
	static	cBASIC_ITEM			s_clipItem;
	static	BOOL				s_isInitializing;
	static	BOOL				s_isSort,s_isUniqueFirst,s_bIsShowSerial;

	static	int					s_iLinePos,s_iLineCount,s_iLineCountPerPage;
	static	int					s_iItemListWidth,s_iItemListHeight;	//	아이템 리스트 박스 너비와 높이

	static	cDIBWND				s_dibItemList,s_dibItemInfo;		//	아이템 리스트와 선택된 아이템의 설명이 나오는곳
	static	cRECT				s_rectItemList;						//	아이템 리스트 윈도우


	static	BOOL				Init(HWND hwnd);					//	기본 아이템 리스트창을 띄운다.
	static	void				InitItemListBox(HWND hwnd);			//	기본 아이템 리스트 박스를 초기화 한다.
	static	void				ResetItemListBox(HWND hwnd,BOOL _bSort=FALSE);
	static	int					SelectItem(int _iX,int _iY);		//	아이템 선택
	static	void				PlaceItemToViewAble(HWND _hWnd,int _iItem);


	static	void				DrawItemList(HWND hwnd);			//	기본 아이템 리스트 박스를 그려준다.(-_-?)
	static	void				DrawItemInfo();


	static	LRESULT CALLBACK	s_procSetBasicItemList(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procItemListWnd(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procItemInfoWnd(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);

//
//	아이템 추가
//
	static	cDIBWND				s_dibUniqueItemData,s_dibItemGenerateData,s_dibIconShape,s_dibEquippedShape,s_dibFieldShape;		//	아이템 효과 dib,아이템 모양 dib
	static	cBASIC_ITEM			s_makeItem;
	static	BOOL				s_bEditItem;
	static	int					s_iSelectGerateData,s_iSelectUniqueData,s_iEditItemKind;

	static	BOOL				Add(HWND hwnd,int _iEditItem=0xffff);

	static	void				InitItemAddBox(HWND hwnd);
	static	void				DrawAddItemWnd(HWND hwnd);			//	아이템 추가 박스를 그려준다.
	static	void				DrawItemGerateData();
	static	void				DrawUniqueData();
	static	void				FillItemAddBox(HWND hwnd);
	static	BOOL				GetItemAddBoxData(HWND hwnd);
	static	int					SelectUniqueData(int x,int y);
	static	int					SelectGenerateData(int _iX,int _iY);

	static	BOOL				DecodeUniqueScriptFile(HWND _hDlg);//화일을 분석해서 유니크 아이템으로 등록한다.
	static	BOOL				DecodeUniqueScriptFile(HWND _hDlg,char *_strFName);//화일을 분석해서 유니크 아이템으로 등록한다.
	static	BOOL				DecodeUniqueScript(char *_strStream,BOOL _isCry=TRUE);//_strStream으로 유니크 데이터로 가공한다.
	static	BOOL				DecodeUniqueScript2(char *_strScript);

	static	BOOL				DecodeUniqueScript(HWND _hDlg);//스크림트 입력창에 데이터를 유니크 데이터로 가공한다.

	static	BOOL				DecodeItemScriptFile(HWND _hDlg);//화일을 분석해서 아이템으로 등록한다.
	static	BOOL				DecodeItemScriptFile(HWND _hDlg,char *_strFName);//화일을 분석해서 아이템으로 등록한다.
	static	BOOL				DecodeItemScript(char *_strStream,BOOL _isCry=TRUE);//_strStream으로 아이템 데이터로 가공한다.
	static	BOOL				DecodeItemScript(HWND _hDlg);//스크림트 입력창에 데이터를 아이템 데이터로 가공한다.

	static	BOOL				CopyItemData(HWND _hDlg);
	static	BOOL				CopyBaseItemData(HWND _hDlg);

	
	static	LRESULT CALLBACK	s_procAddBasicItem(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procItemGerateDataWnd(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procUniqueItemDataWnd(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procItemShapeWnd(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procEquippedShapeWnd(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procFieldShapeWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//
//	아이템 모양 선택
//
	static	cDIBWND				s_dibItemShapeList;
	static	int					s_iSelectShapeKind,s_iSelectItemShape;
	static	cSCROLL_BAR			s_scrollItemShapeList;

	static	int					s_iItemWidth,s_iItemHeight,s_iItemShapeListWidth,s_iItemShapeListHeight,s_iISPW,s_iISPH,s_iItemShapeLineCount,s_iItemCount;
								//	각종 사이즈

	
	static	BOOL				SelectItemShape(HWND _hWnd,int _iSelectShapeKind);
	static	int					SelectItemShape(int x,int y);

	static	void				InitItemShapeList(HWND _hDlg);
	static	void				DrawItemShapeList(HWND _hDlg);


	static	LRESULT CALLBACK	s_procSelectItemShape(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procItemShapeListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static	LRESULT	CALLBACK	s_procItemCheckDataWnd(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);		// 09.08.07
	static	void				InitCheckData(HWND _hDlg);						// 09.08.07
	static	void				GetItemCheckData(HWND _hDlg);					// 09.08.07
	static	void				SetCheckDataList(HWND _hDlg);					// 09.08.07
};

///////////////////////////////////////////////////////////////////////////////////////////
//	아이템 접두사
///////////////////////////////////////////////////////////////////////////////////////////
class	cSET_ITEM_PREFIX
{
public:
	static	HWND				s_hWnd;
	static	int					s_iSelectPrefix,s_iFilter;
	static	cSCROLL_BAR			s_sbDetailInfo;
	static	cITEM_PREFIX*		s_lpSelectPrefix;
	static	cITEM_PREFIX		s_clipPrefix,s_makePrefix;
	static	cLISTCONTROL		s_listPrefix;	//	접두사 리스트

	static	cDIBWND				s_dibPrefixDetailInfo;	//	접두사의 세부 정보

	static	BOOL				Init(HWND hwnd);					//	기본 아이템 리스트창을 띄운다.
	static	void				InitItemPrefixListBox(HWND hwnd);			//	기본 아이템 리스트 박스를 초기화 한다.
	static	void				ResetPrefixListBox(HWND hwnd);
	static	int					SelectPrefix(int _iX,int _iY);		//	아이템 선택
	static	void				PlacePrefixToViewAble(HWND _hWnd,int _iItem);
	static	BOOL				DecodeEffectScriptFile(HWND _hDlg);
	static	BOOL				DecodeEffectScriptFile(char *_strFile);
	static	BOOL				DecodeEffectScript(char *_strText);
	static	BOOL				DecodeFile(HWND _hDlg);
	static	BOOL				DecodeFile(char *_strFile);
	static	BOOL				DecodeScript(char *_strText);

	static	void				DrawPrefixList(HWND hwnd);			//	기본 아이템 리스트 박스를 그려준다.(-_-?)
	static	void				DrawPrefixInfo(HWND _hWwnd);
	static	void				Draw(HWND _hWwnd);

	static	LRESULT CALLBACK	s_procItemPrefixListEvent(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	
	static	LRESULT CALLBACK	s_procItemPrefixList(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procPrefixListWnd(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procPrefixInfoWnd(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);



//////////////////////////////////////////////////////////////////////
//	접두사 추가
//////////////////////////////////////////////////////////////////////
	static	BOOL				s_isEditPrefix;
	static	int					s_iLastPrefixType,s_iEditPrefix;

	static	BOOL				Add(HWND hwnd,int _iEditPrefix);
	static	void				InitSetItemPrefixDlg(HWND _hDlg);//	아이템 부가효과 설정 박스 초기화
	static	void				FillSetItemPrefixDlg(HWND _hDlg);//	아이템 부가효과 설정 박스 내용 채우기
	static	BOOL				GetSetItemPrefixDlg(HWND _hDlg);//	아이템 부가효과 설정 박스 내용 얻어오기
	static	BOOL				DecodeScript(HWND _hDlg);//	스크립트 문장(?)을 해석 한다.
	static	LRESULT CALLBACK	s_procSetPrefix(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
};

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃프로젝트 설정
//┃cSET_PROJECT.cpp
class	cSET_PROJECT	:	public	cSET
{
public:

	static	void				Init(HWND _hWnd);
	static	void				SetData(HWND _hWnd);
	static	BOOL				GetData(HWND _hWnd);
	static	void				RemoveJob(HWND _hWnd);
	static	void				AddJob(HWND _hWnd);

	static	LRESULT CALLBACK	s_procDlg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
//┃cSET_PROJECT.cpp
//┃프로젝트 설정
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

class	cSetCorrectTile		:	public	cSET
{
public:
	static	void				Open(HWND _hWnd);
	static	BOOL				FinishWork(HWND _hWnd);

	static	LRESULT CALLBACK	ProcDlg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class	cPatrolRoad;

class	cSetPatrolRoad		:	public	cSET
{
public:
	static	cPatrolRoad			s_patrolRoad;
	static	BOOL				Open(HWND _hWnd,cPatrolRoad *_lpRoad=NULL);
	static	BOOL				FinishWork(HWND _hWnd);

	static	LRESULT CALLBACK	ProcDlg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class	cHelpDialog	:	public cSET
{
public:
	static	char				*s_pText;
	static	char				s_strTitle[256];

	static	BOOL				Open(HWND _hWnd,char *_lpstrDialogName,char *_lpstrTitle,char *_lpstrFn);
	static	BOOL				Init(HWND _hWnd);
	static	LRESULT CALLBACK	ProcDlg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#define		dITEM_SHAPE_KIND_ICON		0
#define		dITEM_SHAPE_KIND_EQUIPPED	1
#define		dITEM_SHAPE_KIND_FIELD		2

#endif