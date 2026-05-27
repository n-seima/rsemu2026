#if !defined(AFX_QUESTLIST_H__6B860794_8393_4DA4_B77F_0419FB6557B3__INCLUDED_)
#define AFX_QUESTLIST_H__6B860794_8393_4DA4_B77F_0419FB6557B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// questList.h : header file
//
#include	"windowExtendAPI.h"
#include	"definePacketData.h"

/////////////////////////////////////////////////////////////////////////////
// cQuestList dialog

class	CAvatarData;

class cQuestList : public CDialog
{
// Construction
public:
	cListView		m_listCompleteQuest;
	cListView		m_listQuest;
	cListView		m_listMainQuestValue;
	WORD			m_awMainQuestValue[10];
	tsProcessQuestField	m_aProcessQuest[dMAX_PROCESS_QUEST_COUNT];
	BYTE			m_aCompleteQuest[dQUEST_BUFFER_SIZE];
	BYTE			m_aCompleteQuest2[dQUEST_BUFFER_EXTEND_SIZE];
	BYTE			m_aCompleteArbeit[dARBEIT_BUFFER_SIZE];
	BYTE			m_aCompleteArbeit2[dARBEIT_BUFFER_SIZE];
	int				m_iQuestType;
	CAvatarData		*m_lpPlayer;

	void			updateList();
	void			setPlayer(CAvatarData 	*_lpPlayer);

	cQuestList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(cQuestList)
	enum { IDD = IDD_QUEST_LIST };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cQuestList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cQuestList)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCancelQuest();
	afx_msg void OnMainQuest();
	afx_msg void OnArbeit();
	afx_msg void OnChangeValue();
	afx_msg void OnCancelQuestCompleteInfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUESTLIST_H__6B860794_8393_4DA4_B77F_0419FB6557B3__INCLUDED_)
