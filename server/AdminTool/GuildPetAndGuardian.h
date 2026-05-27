#if !defined(AFX_GUILDPETANDGUARDIAN_H__12EB84C0_BB68_48F5_8A01_5133BB725519__INCLUDED_)
#define AFX_GUILDPETANDGUARDIAN_H__12EB84C0_BB68_48F5_8A01_5133BB725519__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuildPetAndGuardian.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuildPetAndGuardian dialog

#include	"windowExtendAPI.h"

class	CGuildAdvanceInfo;

class CGuildPetAndGuardian : public CDialog
{
// Construction
public:
	CGuildPetAndGuardian(CWnd* pParent = NULL);   // standard constructor

	cListView			m_listPet,m_listGuardian,m_listExp;
	CGuildAdvanceInfo	*m_lpGuild;

	void				updateList();
	void				changePetExp();
	void				changeGuardianExp();
	void				setGuild(CGuildAdvanceInfo	*_lpGuild);

// Dialog Data
	//{{AFX_DATA(CGuildPetAndGuardian)
	enum { IDD = IDD_GUILD_PET_AND_GUARDIAN_INFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuildPetAndGuardian)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGuildPetAndGuardian)
	virtual BOOL OnInitDialog();
	afx_msg void OnExpSetToPet();
	afx_msg void OnExpSetToGuardian();
	afx_msg void OnChangePetExp();
	afx_msg void OnChangeGuardianExp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILDPETANDGUARDIAN_H__12EB84C0_BB68_48F5_8A01_5133BB725519__INCLUDED_)
