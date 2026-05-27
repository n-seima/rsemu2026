#ifndef _classSetSecretDungeon_h
#define	_classSetSecretDungeon_h

#include "cSET.H"
#include "cITEM.H"
#include "secretDungeon.H"

class	cMakeFloor	:	public cSET
{
public:
	static	cSecretDungeonFloorDefine		s_makeFloor;

	static	BOOL		Open(HWND _hWnd,cSecretDungeonFloorDefine *_lpEdit);	//	시작
	static	void		InitData(HWND _hDlg);
	static	BOOL		FinishWork(HWND hDlg);

	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);
};

class	cMakeSecretDungeon	:	public cSET
{
public:
	static	cSecretDungeon		s_makeDungeon;
	static	HWND					s_hWnd;
	static	cLISTCONTROL			s_listFloor;	//	드롭 아이템 리스트
	static	int						s_iSelectFloor;

	static	BOOL					Open(HWND _hWnd,cSecretDungeon *_lpEdit);	//	시작
	static	void					InitData(HWND _hDlg);
	static	void					Draw(HWND hDlg);

	static	void					ChangeFloorCount(HWND hDlg,int _iCount);
	static	void					ChangeGuildDungeon(HWND hDlg,int _iCount);
	static	BOOL					FinishWork(HWND hDlg);
	static	BOOL					EditFloor(HWND hDlg);

	static	void					FillInfo(HWND hDlg,BOOL _bIsClear=FALSE);
	static	void					UpdateFloorList();

	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);
};

class	cSetSecretDungeon	:	public cSET
{
public:
	static	HWND					s_hWnd;
	static	cLISTCONTROL			s_listDungeon;	//	던젼 리스트
	static	cLISTCONTROL			s_listFloor;	//	층별 정보 리스트

	static	BOOL					Open(HWND _hWnd);	//	시작
	static	void					InitData(HWND _hDlg);
	static	void					Draw(HWND hDlg);

	static	void					AddDungeon(HWND hDlg);
	static	void					EditDungeon(HWND hDlg);
	static	void					RemoveDungeon(HWND hDlg);

	static	void					ChangeDungeon(HWND hDlg,int _iDungeon);
	static	void					UpdateDungeonList(HWND hDlg,int _iSelect);

	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);

	static	LRESULT CALLBACK		ProcDungeonList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam);
};


#endif