#ifndef _classMAPWND_H
#define _classMAPWND_H

#include "SFC.H"
#include "cWINDOW.H"
#include "cIMAGE.H"
#include "cMAP.H"
#include "cBARMENU.H"

enum							// 09.07.28 오브젝트 데이타.
{
	eSELECT_FIXED_OBJECT,
		eSELECT_TINY_OBJECT,
		eSELECT_BUILDING_OBJECT,
		eSELECT_ACTOR_OBJECT,
};
	
struct sSELECT_DATA{			// 09.07.31 오브젝트 데이타.	

	int					iIndex;
	int					iX;
	int					iY;
	int					iClass;
	int					iObject;
	cADD_ON_OBJECT		aAddonObject[dMAX_ADD_ON_OBJECT_COUNT];

};

class cMAPWND	:	public cDIBWND
{
public:
	sSELECT_DATA*		m_pSelectData;					// 09.07.28 선택한 오브젝트 데이타.
	void				saveSelectData();	// 09.07.28
	void				resetSelectData();				// 09.07.28
	void				copySelectData();				// 09.07.28
	void				cutSelectData();				// 09.07.28
	void				drawSelectDataCursor();				// 09.07.28
	void				copyFixedSelectData(sSELECT_DATA* _pSelectData);				// 09.07.28
	void				copyActorSelectData(sSELECT_DATA* _pSelectData);				// 09.07.28
	void				cutActorSelectData(sSELECT_DATA* _pSelectData);				// 09.07.28
	void				copyTinySelectData(sSELECT_DATA* _pSelectData);				// 09.07.28
	cPOINT				getLeftSelectDataPoint();								// 09.07.28	
	BOOL				m_bObjectArea;					// 09.08.03  영역선택.

	int					TPW,TPH;
	cPOINT				Mouse,OldMouse,OldPos,Cursor,OldCursor,m_posWorld;
	BOOL				m_isRepaint,bLININGBOX,bDRAWCURSOR;
	cRECT				LiningBox,BufferRect;
	WORD				*LineBuffer[4];
	HMENU				m_hMenu;
	cBARMENU			m_bmBuildingBlock;
	BOOL				m_bIsWantDrawSelectBox;
	BOOL				bisAutoSaveFirst;
	BOOL				m_bObjectCopy;					//	09.07.09 오브젝트 복사
	BOOL				m_bObjectCut;					//	09.07.09 오브젝트 붙여넣기
	BOOL				m_bLineObjectCreate;			//	09.07.13 선에따라 오브젝트생성.
	cPOINT				m_posLineStart;					//	09.07.13 선 스타트점.
	cPOINT				m_posSnapLineLast;				//	09.07.14 선스냅..
	BOOL				m_bSnapLine;					// 	09.07.14 스냅..

	//JBC	정적 오브젝트 복사 붙여넣기를 위해서 선언.	08-08-13
	int					m_iCopyCount;					// 복사할 오브젝트 갯수

	BOOL				m_bAdditionPrintArea;	// insu add 추가 출력여부 (영역)
	BOOL				m_bAdditionPrintActor;	// insu add 추가 출력여부 (액터)
						cMAPWND();
						~cMAPWND();

	BOOL				Init(HINSTANCE hInst,HWND hWnd,cRECT_SIZE *rect,int xs=100,int ys=100);
	void				SetScrollBar();

	void				Run();
	void				Draw();
	void				DrawScaledScreen();
	void				drawMapToFile(char *_lpstrFileName,int _iScale);
	void				DrawToolBar();
	void				DrawCursor();
	void				ReBuildAll();

	void				VScroll(WORD wScroll,BOOL draw=TRUE );
	void				HScroll(WORD wScroll,BOOL draw=TRUE );

	void				UpdateTiles();
	void				UpdateStaticObjects();
	void				UpdateBuilding();
	void				UpdateAreas();
	void				UpdateEvents();
	void				UpdateCopyOrCut(); // 09.07.09 복사 붙여넣기 업데이트함수.
	void				resetCopyOrCut();
	void				UpdateEdit();
	BOOL				UpdateToolBar();

	void				lineObjectCreate(int _sx, int _sy, int _ex, int _ey);
	void				lineObjectCreate();
	void				DrawLineObjectCursor(int _sx, int _sy, int _ex, int _ey);	//09.07.30 추가
	//JBC	이동 불가 타일 설정	08-09-10
	void				UpdateCannotMoveTile();

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃cMW_CHARACTER.cpp
	void				updateTinyObjects();
//┃cMW_CHARACTER.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃cMW_CHARACTER.cpp
	void				UpdateCharacter();
	void				SettingCharacter();
	void				SetLeader();
	void				SetWard();
//┃cMW_CHARACTER.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃patrolRoad_mw.cpp
	void				UpdatePatrolRoad();
//┃patrolRoad_mw.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	void				UpdateCommand(int iMenu);
	void				PopupArea();
	void				popEditMenu();

	void				UpdateCursorShape();			//	커서 모양을 바꿔 준다.

	void				Draw(int x,int y,int xs,int ys);
	void				DrawObjectSelectPoint();
	

	int					getScale();


	static LRESULT CALLBACK	WNDProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK WNDProcMovefield(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

extern	cMAPWND		g_MAPWND;
extern  cPOINT		g_posDisplayMax;
extern	BOOL		g_isDrawingMap;
extern	BOOL		g_iBrushSize;
extern	int			g_iScale;
extern	cRECT		g_rectMapwnd;
extern	cPOINT		g_posMapwnd;
extern	int			g_iTBSize;

#define	IDM_REMOVE_AREA			2000
#define	IDM_EDIT_AREA			2001

#define	IDM_SELECT_ALL			3000
#define	IDM_SELECT_CHARACTER	3001
#define	IDM_SELECT_OBJECT		3002
#define	IDM_SELECT_BUILDING		3003

#define	IDM_PREVIOUS_WORK		3004
#define	IDM_REMOVE				3005
#define	IDM_EDIT_ACTOR			3006
#define	IDM_LINK_MONSTER		3007
#define	IDM_REMOVE_LINK			3008
#define	IDM_SET_LEADER			3009
#define	IDM_SET_WARD			3010
#define	IDM_SELECT_CHARACTER_SAME_KIND	3018

#define	IDM_REMOVE_ROAD			4001
#define	IDM_EDIT_ROAD			4002

#define	IDM_CHECK_SHADOW					3011
#define	IDM_REMOVE_SHADOW					3012
#define	IDM_SELECT_SAME_OBJECT_IN_FIELD		3013
#define	IDM_SELECT_SAME_OBJECT_IN_SCREEN	3014
#define	IDM_LINK_TO_EVENT_AREA				3015

#define IDM_SELECT_AREA_COPY				3016
#define IDM_SELECT_AREA_PASTE				3017


#endif