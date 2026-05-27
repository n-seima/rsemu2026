#ifndef	_dialog_box_h
#define	_dialog_box_h

#include "CGamePlay.H"

class	cDialogBox	:	public CGamePlay
{
public:
	CSpeech				m_speech;
	BOOL				m_bIsReleasedMouseButton;
	cRECT				m_rectDialogBoxFrame,m_rectDialogBoxTextFrame;
	CPos				m_posArrow;
	int					m_iTalker,m_iSpeechHeight,m_iFocusSpeech;
	CPos				m_posEventPosition;
	int					m_iDialogBoxWidth,m_iDialogBoxHeight,m_iDialogBoxFaceWidth;
	int					m_iSelectSpeechLineGap,m_iSpeechLineGap,m_iLineGap;
	WORD				m_wEventArea;
	BOOL				m_bDoNotCloseDialogBox;		// 다른인터페이스가 열려있을때 닫지 않는다.
	int					m_iAddWidth;
	CPos				m_posOldView;

	cDialogBox();

	void				reset();
	void				initComplexDialog();
	void				setDialogBoxFrame(int _iWidth,int _iHeight,int _iFaceWidth);
	void				initDialogBoxFrame();
	void				beginComplexDialog(int _iTalker,int _iEventArea);//	복합대화 시작
	void				closeComplexDialog();//	복합대화 종료
	inline	BOOL		isTalker(int _iSerial)
	{	
		if (m_iTalker == _iSerial) 
			return TRUE;
		return FALSE;
	}// 이녀석 대화중?
	inline	BOOL		isTalking()
	{
		if	(m_iTalker	==	-1)
			return	FALSE;

		return	TRUE;
	}

	void				drawComplexDialog();	//	그리기
	void				updateComplexDialog();	//	업데이트
	void				selectSpeech(int _iSpeech);//	대사를 선택했다.
	void				CheckInstruction(char* _lpstrDialog, char* _strPut, int _istrPutlen);

};

extern	cDialogBox		g_dialogBox;

#endif