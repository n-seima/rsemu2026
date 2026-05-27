#ifndef _classSetMainQuest_h
#define	_classSetMainQuest_h

#include "cSet.H"
#include "mainQuest.H"

class	cSetMainQuest	:	public cSET
{
public:
	static	cChapterInfo			s_editChapter;
	static	int						s_iSelectChapter,s_iSelectHighQuest;

	static	HWND					s_hWnd;
	static	cLISTCONTROL			s_listChapter;	//	던젼 리스트
	static	cLISTCONTROL			s_listHighQuest;	//	층별 정보 리스트

	static	BOOL					Open(HWND _hWnd);	//	시작
	static	void					InitData(HWND _hDlg);
	static	void					Draw(HWND hDlg);

	static	void					AddChapter(HWND hDlg);
	static	void					ModifyChapterName(HWND hDlg);
	static	void					RemoveChapter(HWND hDlg);
	static	void					ChangeChapter(HWND hDlg,int _iBook);
	static	void					UpdateChapterList(HWND hDlg,int _iSelect);

	static	void					AddHighQuest(HWND hDlg);
	static	void					ModifyHighQuestName(HWND hDlg);
	static	void					RemoveHighQuest(HWND hDlg);
	static	void					ChangeHighQuest(HWND hDlg,int _iBook);
	static	void					UpdateHighQuestList(HWND hDlg,int _iSelect);

	static	void					ApplyChapterContents(HWND hDlg);

	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcChapterList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK		ProcHighQuestList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam);
};


#endif