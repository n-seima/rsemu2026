#ifndef _classQuestWindow_H_
#define	_classQuestWindow_H_

#include "CGamePlay.H"
#include "cDROPDOWNLIST.h"

// 전체 목록 내 라디오버튼
enum
{
	eQM_MAIN_QUEST,
	eQM_NORMAL_QUEST,
	eQM_TITLE_QUEST,
};

#define dHISTORY_TITLE_DROPDOWN_BUTTON_COUNT	3	// 칭호 퀘스트에서 드롭다운버튼 갯수

enum
{
	eSQT_NORMAL,
	eSQT_MAIN,
};

class	CQuestWindow	:	public CGamePlay
{
public:
	int				m_iFocusQuest,m_iSelectQuest;	//	기억해 뒀다가 다시 팝업 뜨면 설정한다.
	int				m_iSelectTab,m_iFocusTab;
	CPos			m_posQuestWindow;

					CQuestWindow();

	void			open();
	void			close();
	void			draw();
	void			drawMainQuestInfo();
	void			drawNormalQuestList();
	void			drawTab();
	int				getFocusedTab();

	void			update();
	void			selectQuest(int _iQuest);
	void			selectTab(int _iTab);
	int				getFocusedQuest();

	BOOL			isOpened();

	// 전체 목록 --- 공통 --------------------------------------------------------------------
	CImageScrollBar	m_sbScrollBar;				// 스크롤바
	int				m_iSelectRadioBtn;			// 라디오버튼 선택된 값
	int				m_iProcessMainQuestIndex;	// 현재 진행중인 메인퀘스트의 인덱스! 
	cRECT			m_rtQuestListArea;			// 창 중앙 리스트가 출력될 위치

	void			setRadioBtn(cRECT* _rtPut);	
	int				getPartQuestNum(char* str);	// (쓸일은 없지만 혹 모르니 남겨둔다.)
	void			setScrollBarSize(cRECT* _rtPut);
	void			DrawQuestSimpleInfo(cRECT* _rtPut, int _iQuestType, int _iQuestIndex);
	char*			NameFiltering(char* _str);
	// 전체 목록 --- 메인퀘스트 --------------------------------------------------------------
	WORD			m_wMainQuestTotalCount[c_iMaxChapterCount][c_iMaxHighQuestCount];	// 파트별로 나눠진 퀘스트의 총 갯수
	WORD			m_wMainBonusQuestCount[c_iMaxChapterCount][c_iMaxHighQuestCount];	// 파트별로 보너스퀘스트의 완료여부를 저장한다. 미완료시에만 저장되며 해당 숫자만큼 완료갯수에서 빼준다.	
	int				m_iProcessQuestPartInQuestCount; // 현재 진행중인 메인퀘스트 파트 내 퀘스트갯수 (완료갯수때문에 필요)

	vector<cRECT>	m_vtChapterArea;		// 챕터 버튼 영역
	vector<WORD>	m_vtChapterofPartIndex; // 파트의 챕터인덱스
	vector<WORD>	m_vtChapterIndex;		// 챕터 버튼 인덱스 .. 스크롤 때문에 필요
	int				m_iFocusChapterBtn;		// 마우스 포커스가 간 챕터버튼 없다면 -1
	
	vector<cRECT>	m_vtPartArea;			// 파트 버튼 영역
	vector<WORD>	m_vtPartIndex;			// 파트 버튼 인덱스 .. 스크롤때문에 필요
	int				m_iFocusPartBtn;		// 마우스 포커스가 간 파트버튼 없다면 -1
	
	vector<cRECT>	m_vtQuestArea;			// 퀘스트 버튼 영역
	vector<WORD>	m_vtQuestIndex;		    // 퀘스트 버튼의 해당 인덱스
	vector<WORD>	m_vtQuestType;		    // 일반퀘스트나 타이틀을 출력해줄때 퀘스트타입 지정
	int				m_iSelectHistoryQuest;  // 메인퀘스트에서 선택한 퀘스트 인덱스	

	BOOL			m_bChapterBtnState[c_iMaxChapterCount]; // 챕터 버튼 상태 (펼처진 상태냐 아니냐)
	BOOL			m_bPartBtnState[c_iMaxChapterCount][c_iMaxHighQuestCount];  // 파트 버튼 상태 (펼처진 상태냐 아니냐)

	void			setMainQuestGroup();
	void			DrawMainQuestHistory(cRECT* _rtPut);
	void			FindMainQuestChapterBtn();
	void			UpdateMainQuestChapterBtn();
	void			FindMainQuestPartBtn();
	void			UpdateMainQuestPartBtn();
	void			UpdateMainQuestIndexBtn();

	// 전체 목록 --- 일반퀘스트 --------------------------------------------------------------
	// 영역정보는 메인퀘스트 히스토리를 출력할 때 사용하던 m_vtQuestArea 를 사용한다.
	int				m_iNormalQuestCnt;			// 일반 퀘스트 갯수
	int				m_iNormalQuestCompleteCnt;	// 일반 퀘스트 완료된 갯수
	int				m_iFocusNormalQuest;		// 포커스가 간 퀘스트
	int				m_iSelectNormalQuest;		// 선택한 퀘스트
	int				m_iSelectNormalQuestType;	// 선택한 퀘스트의 타입 (노말이냐 알바냐) 

	void			DrawNormalQuestHistory(cRECT* _rtPut);
	void			FindNormalQuestBtn();
	void			UpdateNormalQuestBtn();

	// 전체 목록 --- 칭호퀘스트 --------------------------------------------------------------
	int				m_iTitleQuestCnt;			// 타이틀퀘스트 갯수
	int				m_iTitleQuestCompleteCnt;	// 타이틀퀘스트 완료 갯수
	int				m_iSkillQuestCnt;			// 스킬마스터퀘스트 갯수
	int				m_iSkillQuestCompleteCnt;	// 스킬마스터퀘스트 완료 갯수	
	BOOL			m_bDropDownBtnState[dHISTORY_TITLE_DROPDOWN_BUTTON_COUNT];	// 드롭다운버튼 상태 (칭호퀘스트 + 스킬 + 칭호) (펼처진 상태냐 아니냐)
	vector<cRECT>	m_vtDropDownBtnArea;		// 드롭다운버튼 영역
	int				m_iFocusDropDownBtn;		// 마우스 포커스가 간 드롭다운버튼 없다면 -1
	int				m_iTitleQuestSelectQuest;	// 선택한 퀘스트
	int				m_iTitleQuestSelectQuestType;

	void			DrawTitleQuestHistory(cRECT* _rtPut);
	void			DrawTitleQuestHistoryDetail(cRECT* _rtPut, int* _iX, int* _iY, int _iQuestKind, int* _iPutStartPoint, BOOL _bDropDownArea);
	void			UpdateTitleQuestDropDownBtn();
	void			FindTitleQuestDropDownBtn();
	void			FindTitleQuestQuest();
	void			UpdateTitleQuestQuest();

	// 전체 목록 --- 필터관련   --------------------------------------------------------------
	cRECT			m_rtHistoryFilter;					// 필터가 출력될 위치
	cDROPDOWNLIST	m_dlmenu;							// 드롭리스트 컨트롤
	void			DrawHistoryFilter(cRECT* _rtPut);
};

extern	CQuestWindow	g_gwQuest;

#endif
