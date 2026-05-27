#ifndef _classSET_H_
#define _classSET_H_

#include <windows.h>
#include "cMAIN.H"
#include "cANM.H"
#include "SFC.H"
#include "cWINDOW.H"

#define	IDM_MAX_ANM		1002
#define	IDM_MAX_FRAME	1003

#define	IDM_MINIMGCOL	2000
#define	IDM_MINIMGROW	2001
#define	IDM_MINFRMCOL	2002

#define	IDC_INDEX		0
#define	IDC_ANIMATION	1
#define	IDC_DIRECT		2
#define	IDC_MAX_FRAME	3

class cSET
{
public:
	//	에니메이션 셋팅..
	static	cSPIN		maxAnm,maxFrame,minImgRow,minImgCol,minFrmCol;
	static	cLISTVIEW	ANMLIST;

	static	void		SetDetailAnm(HWND hwnd);
	static	void		SetDetailAnmData(HWND hwnd);
	static	void		initDETAILANM(HWND hDlg);
	static	void		resetData();		//	기본 분류에 맞춰 방향을 설정..
	static	void		resetCount(int ac);	//	에니메이션 카운트 재 설정..

	static	void		initSETANM(HWND hDlg);
	static	void		SetAnm();
	static	BOOL		SetAnmData(HWND hDlg);
	static	void		loadANMSET(HWND hDlg);
	static	void		AddAnm(HWND _hDlg);
	static	void		InsertAnm(HWND _hDlg);
	static	void		RemoveAnm(HWND _hDlg);

	static	LRESULT CALLBACK	procDETAILANM(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK	procSETANM(HWND,UINT,WPARAM,LPARAM);

	//	툴 셋팅..
	static	void		SetTool();
	static	void		initSETTOOL(HWND hDlg);
	static	void		SetTool(HWND hwnd);
	static	void		drawColorBox(int index,LPARAM lParam);
	static	void		chooseColor(int index,HWND hwnd);

	static	LRESULT CALLBACK	procSETTOOL(HWND,UINT,WPARAM,LPARAM);

};

#endif