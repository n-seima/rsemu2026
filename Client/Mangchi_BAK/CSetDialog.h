#ifndef _classSetDialog_H
#define _classSetDialog_H

#include "cSET.H"
#include "CDialog.H"


class	CSetDialog	:	public cSET
{
public:
	static	HWND					s_hWnd;
	static	CConversationContainer	s_ccEdit;	//	편집중인 대화 컨테이너
	static	CConversationContainer	s_storeCC;
	static	cDIBWND					s_dibSpeechList,s_dibTrigger;
	static	BOOL					s_bIsEventArea;
	static	int						s_iSpeechListWndWidth,s_iTriggerWndWidth,s_iSelectTrigger;
	static	int						s_iSelectSpeech,s_iSelectSpeech2;
	static	char					s_strTalkerName[64];
	static	cSCROLL_BAR				s_sbSpeechList;
	static	cSCROLL_BAR				s_sbTriggerList;

	static	CConversationContainer*	Init(HWND hwnd,char *_lpstrTalkerName,CConversationContainer *_lpccEdit,BOOL _bIsArea=FALSE);
	static	void					InitSetDialogBoxData(HWND _hDlg);
	static	BOOL					GetDialogData(HWND hwnd);

	static	void					DrawTrigger(HWND _hDlg);//조건 그리기
	static	void					DrawSpeechList(HWND _hDlg);//대화 리스트 그리기

	static	void					AddTrigger(HWND _hDlg);//조건 추가
	static	void					EditTrigger(HWND _hDlg);//조건 편집
	static	void					CheckTriggerCompleteMethod(HWND _hDlg);//	트리거 만족 조건

	static	void					RemoveTrigger(HWND _hDlg);//조건 제거

	static	void					UpSelectSpeech(HWND _hDlg);
	static	void					DownSelectSpeech(HWND _hDlg);

	static	void					AddSpeech(HWND _hDlg);//대사 추가
	static	void					EditSpeech(HWND _hDlg);//대사 수정
	static	void					RemoveSpeech(HWND _hDlg);//대사 제거
	static	void					CopySpeech(HWND _hDlg);
	static	void					PasteSpeech(HWND _hDlg);

	static	int						GetSelectSpeech(HWND hwnd);	//	대사 선택
	static	int						SelectSpeech(HWND hwnd);	//	대사 선택
	static	int						GetSelectTrigger(HWND hwnd);	//	조건 선택
	static	int						SelectTrigger(HWND hwnd);		//	조건 선택

	static	LRESULT CALLBACK		ProcSetDialog(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcSpeechListWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcTriggerWnd(HWND,UINT,WPARAM,LPARAM);

};

class	CSetSpeech	:	public cSET
{
public:
	static	HWND					s_hWnd;
	static	HFONT					s_hFont;

	static	CConversation			s_cEdit;	//	편집중인 대화 컨테이너
	static	CConversation			s_storeConversation;	//	편집중인 대화 컨테이너
	static	cDIBWND					s_dibContents,s_dibReaction;
	static	cSCROLL_BAR				s_sbContents,s_sbReaction;

	static	int						s_iContentsWndPos,s_iReactionWndPos;
	static	int						s_iSelectContent,s_iSelectContent2;
	static	BOOL					s_bIsEdit;
	static	BOOL					s_bIsEventArea;
	static	char					s_strTalkerName[64];

	static	BOOL					GetSpeechData(HWND _hDlg);	//	데이터 얻기

	static	CConversation*			AddSpeech(HWND hwnd,char *_lpstrTalker,CConversation* _lpcEdit,BOOL _bIsArea=FALSE);
	static	void					InitBox(HWND _hDlg);
	static	void					DrawContentsWnd(HWND _hDlg);//대화 리스트 그리기
	static	void					DrawReactionWnd(HWND _hDlg);//대화 리스트 그리기
	static	int						SelectContent(HWND hwnd);	//	컨텐츠 선택
	static	int						GetSelectContent(HWND hwnd);//	선택된 컨텐츠 얻기

	static	void					MoveUpContents(HWND _hDlg);		//	위로 이동
	static	void					MoveDownContents(HWND _hDlg);	//	아래로 내림

	static	void					CopyContents(HWND _hDlg);	//	복사
	static	void					PasteContents(HWND _hDlg);	//	붙여넣기
	
	static	void					AddContents(HWND _hDlg);	//최초 분기 추가
	static	void					EditContents(HWND _hDlg);	//선택된 최초 분기 편집
	static	void					RemoveContents(HWND _hDlg);	//선택된 최초 분기 제거

	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcContentsWnd(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcReactionWnd(HWND,UINT,WPARAM,LPARAM);

};

extern	char	*g_strDialogWindowSizeList[];

#endif