#ifndef _classSetDialogSpeech_H
#define _classSetDialogSpeech_H

#include "cSET.H"

class CSetDialogSpeech : public cSET
{
public:
	static	HWND					s_hWnd;
	static	CConversationContainer	s_ccEdit, s_storeCC;
	static	cDIBWND					s_dibSpeechList, s_dibTriggerList, s_dibContents;
	static	cSCROLL_BAR				s_sbSpeechList, s_sbTriggerList, s_sbContents;
	static	CConversation			s_cEdit,s_storeConversation;
	static	HFONT					s_hFont;
	static	CKarma					s_aStoredKarma;

	static	BOOL					s_bIsEventArea, s_bIsEdit, s_bIsSaveAddSpeech,s_bIsSaveAddContents;
	static	char					s_strTalkerName[64];

	static	int						s_iSelectSpeech,s_iSelectSpeech2,s_iSelectTrigger, s_iSelectContent,s_iSelectContent2;
	static	int						s_iSpeechListWndWidth, s_iTriggerWndWidth, s_iContentsWndPos;
	
	static	CConversationContainer*	Init(HWND _hwnd,char *_lpstrTalkerName,CConversationContainer *_lpccEdit,BOOL _bIsArea=FALSE);
	static	void					InitSetDialogBoxData(HWND _hDlg);
	static	void					InitSpeech();

	static	void					DrawSpeechList();
	static	void					DrawTrigger();
	static	void					DrawContents();
	static	void					DrawSpeechKarma();

	static	void					UpSelectSpeechList(HWND _hDlg);
	static	void					DownSelectSpeechList(HWND _hDlg);
	static	void					RemoveSpeechList(HWND _hDlg);
	static	void					CopySpeechList(HWND _hDlg);
	static	void					PasteSpeechList(HWND _hDlg);
	static	void					SaveSpeechList();

	static	void					AddTrigger(HWND _hDlg);
	static	void					EditTrigger(HWND _hDlg);
	static	void					RemoveTrigger(HWND _hDlg);
	static	void					CheckTriggerCompleteMethod(HWND _hDlg);

	static	void					MoveUpContents();
	static	void					MoveDownContents();
	static	void					CopyContents();
	static	void					PasteContents();
	static	void					RemoveContents();
	static	void					SaveContents();

	static	void					CopyKarma();
	static	void					PasteKarma();


	static	int						GetSelectSpeechList(HWND hwnd);
	static	int						SelectSpeechList(HWND hwnd);
	static	int						GetSelectTrigger(HWND hwnd);
	static	int						SelectTrigger(HWND hwnd);
	static	int						GetSelectContent(HWND hwnd);
	static	int						SelectContent(HWND hwnd);
	

	static	LRESULT CALLBACK		ProcSetDialog(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcSpeechListWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcTriggerWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcContentsWnd(HWND,UINT,WPARAM,LPARAM);
};

#endif

