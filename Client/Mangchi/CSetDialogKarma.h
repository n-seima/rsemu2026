#ifndef _classSetDialogKarma_H
#define _classSetDialogKarma_H

#include "cSET.H"

class CSetDialogKarma : public cSET
{
public:
	static	HWND					s_hWnd, s_hKarmaTab, s_hKarmaTabDlg,s_hTrigger, s_hReaction;
	static	BOOL					s_bIsEnableDialog;
	static	int						s_iLastTab, s_iCopyTriggerCount, s_iCopyReactionCount;

	static	CKarma *				AddKarma(HWND _hwnd,CKarma *_lpEditKarma,int _iType);
	static	void					InitAddKarmaDlg(HWND _hwnd);
	static	void					InitTrigger(HWND _hdlg);
	static	void					InitReaction(HWND _hdlg);
	static	BOOL					ChangeKarmaTab(int _type);
	
	static	HWND					GetTriggerhWnd(){	return	s_hTrigger;	}
	
	static	LRESULT CALLBACK		s_ProcSetDialog(HWND,UINT,WPARAM,LPARAM);
	static	BOOL CALLBACK			s_ProcPageKarmaTrigger(HWND,UINT,WPARAM,LPARAM);
	static	BOOL CALLBACK			s_ProcPageKarmaReaction(HWND,UINT,WPARAM,LPARAM);
};	


#endif
