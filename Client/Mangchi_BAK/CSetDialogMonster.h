#ifndef _classSetDialogMonster_H
#define _classSetDialogMonster_H

#include "cSET.H"
#include "cLISTCONTROL.H"

class CSetDialogMonster : public cSET
{
public:
	static	HWND					s_hWnd, s_hSlider;
	static	HWND					s_hListTab, s_hInfoTab, s_hListTabDlg, s_hInfoTabDlg;
	static	HWND					s_hPageList, s_hPageFindList, s_hPageInfo, s_hPageEvent, s_hPageJob, s_hPageDeath;
	static	cLISTCONTROL			s_listMonster, s_listFindMonster;
	static	cDIBWND					s_dibMonsterBaseInfo, s_dibMonsterGeneralInfo, s_dibMonsterJob, s_dibMonsterDeath, s_hKarmaDetailWnd;
	
	static	cSCROLL_BAR				s_sbMonsterKarma;	
	static	CKarmaContainer			s_aEvent[eMECC_COUNT];
	static	cCHARACTER				*s_lpEditCharacter;
	static	CKarma					*s_lpStoredKarma, *s_lpSelectKarma;
			
	static	int						s_iSelOldList, s_iSelOldInfo, s_nMonsterCount, s_nFindMonsterCount;
	static	int						s_iSpeechListWndPos, s_iSelectMonsterData, s_iSelectFindMonsterData, s_iSelectCondition, s_iKarmaDetailWndPos;
	static	BOOL					s_bEnableDialog;
	
	static	void					Init(HWND hwnd);
	static	void					ShowDialog();

	static	void					SelectCondition();
	static	void					SelectKarma();
	static	void					RemoveMonster(HWND _hDlg);
	static	void					RemoveAllMonster(HWND _hDlg);
	static	void					EditMonsterBaseData(HWND _hDlg);
	static	void					EditMonsterEvent(HWND _hDlg);
	static	void					EnableButton(BOOL bFlag);	
	
	static	void					InitPageData(HWND);
	static	void					InitSetDialogListBoxData(int);

	static	void					ResetMonsterListBox(int);
	static	void					ResetMonsterInfoBox(int);
	static	void					SetEvent();
	static	void					InitSetCharacterEventData();
	
	static	void					DrawPageList(int);
	static	void					DrawPageInfo(int);
	static	void					DrawPageMonsterBaseInfo();
	static	void					DrawPageMonsterGeneralInfo();
	static	void					DrawMonsterKarma();
	static	void					DrawKarmaDetail();
	static	void					DrawPageMonsterJob();
	static	void					DrawPageMonsterDeath();
		
	static	LRESULT CALLBACK		ProcSetDialog(HWND,UINT,WPARAM,LPARAM);
	
	// Page Left
	static	BOOL CALLBACK			ProcPageMonsterList(HWND,UINT,WPARAM,LPARAM);
	static	BOOL CALLBACK			ProcPageMonsterFind(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcMonsterListWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcMonsterFindListWnd(HWND,UINT,WPARAM,LPARAM);

	// Page Right
	static	BOOL CALLBACK			ProcPageMonsterInfo(HWND,UINT,WPARAM,LPARAM);
	static	BOOL CALLBACK			ProcPageMonsterEvent(HWND,UINT,WPARAM,LPARAM);
	static	BOOL CALLBACK			ProcPageMonsterJob(HWND,UINT,WPARAM,LPARAM);
	static	BOOL CALLBACK			ProcPageMonsterDeath(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcMonsterBaseInfoWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcMonsterGeneralInfoWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT	CALLBACK		ProcMonsterKarmaDetailWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcMonsterJobWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcMonsterDeathWnd(HWND,UINT,WPARAM,LPARAM);
};

#endif