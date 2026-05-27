#ifndef _classTOOLWND_H
#define _classTOOLWND_H

#include	"SFC.H"
#include	"cWINDOW.H"
#include	"cIMAGE.H"
#include	"cBARMENU.H"
#include	"cJOB.H"

#define		dBI_TILE					0
#define		dBI_STATIC_OBJECT			1
#define		dBI_FREE_OBJECT				2
#define		dBI_HOUSE					3
#define		dBI_AREA					4
#define		dBI_HERO					5
#define		dBI_NPC						6

#define		dBI_SIZE1					7 
#define		dBI_SIZE2					8 
#define		dBI_SIZE3					9 
#define		dBI_SIZE4					10
#define		dBI_SIZE5					11

#define		dBI_PAINT					12
#define		dBI_ROAD					13

#define		dBI_FREQ1					14
#define		dBI_FREQ2					15
#define		dBI_FREQ3					16

#define		dBI_FREE_TILE				17
#define		dBI_BLOCKED_TILE			18
#define		dBI_TILE_EXPAND				19

#define		dBI_ACTIVE_BORDER			20



#define		dTB_TILE					10000
#define		dTB_STATIC_OBJECT			10001
#define		dTB_FREE_OBJECT				10002
#define		dTB_HOUSE					10003
#define		dTB_AREA					10004
#define		dTB_HERO					10005
#define		dTB_NPC						10006

#define		dTB_SIZE1					10007 
#define		dTB_SIZE2					10008 
#define		dTB_SIZE3					10009 
#define		dTB_SIZE4					10010
#define		dTB_SIZE5					10011

#define		dTB_PAINT					10012
#define		dTB_ROAD					10013

#define		dTB_FREQ1					10014
#define		dTB_FREQ2					10015
#define		dTB_FREQ3					10016

#define		dTB_FREE_TILE				10017
#define		dTB_BLOCKED_TILE			10018

#define		dTB_TILE_EXPAND				10019

class cTOOLWND	:	public cDIBWND
{
public:
	int					TPL,POSITION,ScrollCount,ScrollPos,TPH;
	int					StaticPos,FreePos,BuildingPos,TilePos,CharacterPos,HousePos,AreaPos,RoadPos;
	int					m_iTBSIZE,m_iICONSIZE,m_iWPOS;
	cBARMENU			m_WorkButton,m_ToolButton;					//	툴 버튼
	BYTE				m_abInput[3];			// 오브젝트 선택관련 넘버
	int					m_iInputCnt;			// bInput[?] 저장할 ?의 위치지장
	int					m_iPreInput;			// 이전 선택한 오브젝트
	int					m_iIndex;
	char				m_strStaticCtrStr[3];	// 스태틱 컨트롤 문자열	
						cTOOLWND();
						~cTOOLWND();

	BOOL				Init(HINSTANCE hInst,HWND hWnd,cRECT_SIZE *rect);
	void				Run();

	void				Draw();
	void				Draw(int x,int y,int xs,int ys);

	void				UpdateStaticObjects();
	void				UpdateFreeObjects();

	void				UpdateCommand(int menu);
	void				UpdateStaticObjectCommand(int menu);
	void				UpdateFreeObjectCommand(int menu);

	void				SelectObjects();
	void				InputKeypad(int _iType);
	void				SaveScrollPos(int _iScrollPos);
/////////////////////////////////////////////////////////////////////////
//	건물 관련
/////////////////////////////////////////////////////////////////////////
	int					m_isEditBlockOfBuilding;
	int					m_iSelectEditBlockBuilding,m_iBuildingBlockPos;

	void				UpdateBuildings();
	void				UpdateBuildingBlocks();
	void				UpdateBuildingCommand(int menu);
	void				DrawBuildings();
	void				DrawBuildingBlocks();
	void				beginEditBlockOfBuilding(int _iBuilding);
	void				endEditBlockOfBuilding();

/////////////////////////////////////////////////////////////////////////
//	타일 관련
/////////////////////////////////////////////////////////////////////////

	void				UpdateTiles();
	void				UpdateTileCommand(int menu);
	void				DrawTiles();

/////////////////////////////////////////////////////////////////////////
//	영역 관련
/////////////////////////////////////////////////////////////////////////
	BOOL				m_isEditSpecialArea;
	void				UpdateAreas();
	void				UpdateAreaCommand(int menu);
	void				DrawAreas();
	void				InitAddSpecialArea(HWND hdlg);
	void				AddSpecialArea(HWND hdlg);

	static	LRESULT CALLBACK	s_procAddSpecialArea(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);

/////////////////////////////////////////////////////////////////////////
//	길 관련
/////////////////////////////////////////////////////////////////////////
	void				DrawRoad();
	void				UpdateRoad();
	void				UpdateRoadCommand(int menu);

/////////////////////////////////////////////////////////////////////////
//	직업 관련
/////////////////////////////////////////////////////////////////////////

	BOOL				m_isEditJob;						//	직업을 만드는게 아니고 수정하는거다.
	int					m_iEditJob;							//	작업하고 있는 직업.

/////////////////////////////////////////////////////////////////////////
//	몬스터 처리
/////////////////////////////////////////////////////////////////////////

	int					GetCharacterIndex();
	void				UpdateCharacter();
	void				UpdateCharacterCommand(int menu);
	void				DrawCharacter();
	void				SettingCharacter();				// 09.07.24 오브젝트 데이터설정



	void				DrawStaticObjects();
	void				DrawFreeObjects();
	void				DrawNPC();

	void				SetScrollBar(int count,int *pos);
	void				VScroll(WORD wScroll);

	void				ChangeMode(DWORD mode);

	cPOINT				Mouse;

	static LRESULT CALLBACK	WNDProc(HWND, UINT, WPARAM, LPARAM);
};

extern	cTOOLWND	TOOLWND;
extern  cTOOLBAR*		g_lpcToolBar;	
#endif