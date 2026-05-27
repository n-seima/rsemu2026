#ifndef _classCompareBox_h
#define	_classCompareBox_h

#include "cWINDOW.h"
#include "cPROJECT.h"
#include "cMAP.h"
/*
	 파일 비교 다이알로그 
	
	  파일 2개를 선택하여 비교를 한다..
	  비교하면서 다른 점을 출력..
	  비교하는 파일은 맵파일, RPD, 아이템 데이터 이정도??
	  

*/
enum
{//	비교할 파일 
	eCOMPARE_NONE,
	eCOMPARE_MAP_FILE,
	eCOMPARE_ITEM_FILE,	
	eCOMPARE_RPD_FILE,
};

class cCompareBox
{
public:

	static	HWND					s_hWnd;
	static	HINSTANCE				s_hInst;
	static	char					s_strSelectedFirstFilePath[MAX_PATH];		//	첫번째로 선택한 파일 경로
	static	char					s_strSelectedSecondFilePath[MAX_PATH];		//	두번째로 선택한 파일 경로
	static	int						s_iCompareKind;								//	비교할 종류?? 
	
	static	cBASIC_ITEM*			s_lpFirstBasicItem;
	
	static	CCustomItemManager*		s_lpFirstPremiumItem;			
	static	CCustomItemManager*		s_lpFirstCustomItem;			
	static	cITEM_PREFIX_MANAGER*	s_lpFirstItemPrefixManager;
	static	cArcaManager*			s_lpFirstArcaManager;			
	static	cItemSetManager*		s_lpFirstItemSetManager;		
	static	cPieceItemManager*		s_lpFirstPieceItemManager;		
	
	static	cBASIC_ITEM*			s_lpSecondBasicItem;			
	
	static	CCustomItemManager*		s_lpSecondPremiumItem;			
	static	CCustomItemManager*		s_lpSecondCustomItem;			
	static	cITEM_PREFIX_MANAGER*	s_lpSecondItemPrefixManager;	
	static	cArcaManager*			s_lpSecondArcaManager;			
	static	cItemSetManager*		s_lpSecondItemSetManager;		
	static	cPieceItemManager*		s_lpSecondPieceItemManager;	

	static	cMAP*					s_lpFirstMap;
	static	cMAP*					s_lpSecondMap;


	static	void					reset();
	static	void					compareFile();
	static	void					compareMapFile();
	static	void					compareItemFile();
	static	void					compareRPDFile();

	static	int						compareBasicItem();
	static	int						comparePremiumItem();
	static	int						compareCustomItem();
	static	int						compareItemPrefix();
	static	int						compareArca();
	static	int						compareItemSet();
	static	int						comparePieceItem();

	static	int						compareMapBasicInfo();
	static	int						compareMapBaseInfo20031111();
	static	int						compareMapBaseInfo20050527();
	static	int						compareMapInfo();
	static	int						compareMap();
	static	int						compareCharacterSetInMap();
	static	int						compareActorSetInMap();
	static	int						compareBuildingInMap();
	static	int						compareAreaInMap();
	static	int						compareObjectInMap();
	static	int						compareValueInMap();
	static	int						compareShopManagerInMap();
	static	int						compareCustomItemManagerInMap();
	static	int						comparePatrolRoadManagerInMap();
	static	int						compareCharacterPatternPackInMap();
	static	int						compareKarmaContainerInMap();
	static	int						compareEtcInMap();

	static	BOOL					allocItemDataMemory();		// 아이템 데이터 메모리 할당~
	static	void					killItemDataMamory();		//	아이템데이터 메모리 해제~!
	static	BOOL					allocMapDataMemory();		//	맵 데이터 메모리 할당~
	static	void					killMapDataMamory();		//	맵 데이터 메모리 해제~!

	static	void					selectFile(int _iSelectFileKind);
	static	BOOL					open(HWND _hWnd, HINSTANCE _hInstance);	//	창을 열고..
	static	void					updateBox();
	static	void					cancelSelectData();
	static	void					addTextToEdit(int _iEditID, char* _strText);
	static	LRESULT CALLBACK		procDlg(HWND _hWnd, UINT _uMassage, WPARAM _wParam, LPARAM _lParam);

};


#endif