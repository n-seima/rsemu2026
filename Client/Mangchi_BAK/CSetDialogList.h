#ifndef _classSetDialogList_H
#define _classSetDialogList_H

#include "cSET.H"
#include "cLISTCONTROL.H"

class	CSetDialogList	:	public cSET
{
public:
	static	HWND					s_hWnd;
	static	cLISTCONTROL			s_listSpeaker;
	static	cDIBWND					s_dibSpeech;
	static	int						s_iSpeechListWndPos,s_iSelectSpeaker;

	static	void					Init(HWND hwnd);
	static	void					InitSetDialogListBoxData(HWND _hDlg);
	static	void					ResetSpeakerListBox(HWND _hDlg);

	static	void					Draw(HWND _hDlg);//대화 리스트 그리기

	static	void					EditSpeech(HWND _hDlg);//대화 편집
	static	void					RemoveSpeech(HWND _hDlg);//대화 제거
	static	void					RemoveAllSpeech(HWND _hDlg);

	static	int						GetSelectSpeech(HWND hwnd);	//	대사 선택
	static	int						SelectSpeech(HWND hwnd);	//	대사 선택

	static	LRESULT CALLBACK		ProcSetDialog(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcSpeechListWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcSpeechWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};


#endif