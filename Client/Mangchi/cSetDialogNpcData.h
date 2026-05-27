#ifndef _classSetDialogNpcData_H
#define _classSetDialogNpcData_H

#include "cSET.H"
#include "cLISTCONTROL.H"

class cSetDialogNpcData : public cSET
{
public:
	static	HWND					s_hWnd, s_hSlider, s_hListTab, s_hListTabDlg;
	static	HWND					s_hPageList, s_hPageFindList;
	static	cLISTCONTROL			s_listNpc, s_FindNpc;
	static	cDIBWND					s_dibNpcSpeech, s_dibNpcInfo;
	static	cSCROLL_BAR				s_sbNpcSpeech;

	static	int						s_nNpcCount, s_nFindNpcCount,s_iSelectSpeech,s_iSelectSpeech2;
	static	int						s_iSpeechListWndPos,s_iSelectNpcData, s_iSelectFindNpcData, s_iSelOldList;
	static	BOOL					s_bEnableDialog;

	static	void					Init(HWND hwnd);
	static	void					InitPageData();
	static	void					InitSetDialogListBoxData(int sel);
	static	void					ResetNpcListBox(int sel);
		
	static	void					ShowDialog();
	static	void					Draw();
	static	void					DrawPageList(int sel);
	static	void					DrawNpcInfo();
	static	void					DrawNpcSpeech();
	
	static	void					EditSpeech(HWND _hDlg);
	static	void					EditNpcInfo(HWND _hDlg);
	static	void					RemoveSpeech(HWND _hDlg);
	static	void					RemoveAllSpeech(HWND _hDlg);
	static	void					RemmoveNpc(HWND _hDlg);
	static	void					RemoveAllNpc(HWND _hDlg);
	static	void					EnableButton(BOOL bFlag);
	static	int						GetSelectSpeech(HWND hwnd);
	static	int						SelectSpeech(HWND hwnd);
		
	static	LRESULT CALLBACK		ProcSetDialog(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcNpcSpeechWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK		ProcNpcInfoWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK		ProcNpcListWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcNpcFindListWnd(HWND,UINT,WPARAM,LPARAM);
	static	BOOL CALLBACK			ProcPageNpcList(HWND, UINT, WPARAM, LPARAM);
	static	BOOL CALLBACK			ProcPageNpcFindList(HWND, UINT, WPARAM, LPARAM);
};

#endif
