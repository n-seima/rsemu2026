#ifndef _class_set_character_pattern_h
#define _class_set_character_pattern_h

#include "pattern.h"

#include "cSet.H"
#include "mainQuest.H"

class	cSetCharacterPattern	:	public cSET
{
public:
	static	cCharacterPatternPack	s_editPattern;
	static	int						s_iSelectPattern;

	static	HWND					s_hWnd;
	static	cLISTCONTROL			s_listPattern;	//	던젼 리스트

	static	BOOL					Open(HWND _hWnd);	//	시작
	static	void					InitData(HWND _hDlg);
	static	void					Draw(HWND hDlg);

	static	void					AddPattern(HWND hDlg,BOOL _bIsModifySelectPattern=FALSE);
	static	void					RemovePattern(HWND hDlg);
	static	void					SelectPattern(HWND hDlg);
	static	void					UpdatePatternList(HWND hDlg,int _iSelect);
	static	void					FillPatternScript(HWND hDlg);

	static	void					CopyEx(HWND hDlg);


	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK		ProcPatternList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam);
};

#endif