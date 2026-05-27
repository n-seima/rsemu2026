#if !defined(AFX_CPETLIST_H__926AAEF5_2578_4AA2_967C_6E93A7C58D97__INCLUDED_)
#define AFX_CPETLIST_H__926AAEF5_2578_4AA2_967C_6E93A7C58D97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cPetList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cPetList dialog

#include	"windowExtendAPI.h"
#include	"CPetDefine.h"

class cPetList : public CDialog
{
	// Construction
	cPetDefine	m_aPet[2];
	cPetDefine	m_Pet;
	cPetDefine*	m_SavePoint;

	void		MakeCode(char* _strCode , int _iIndex);
public:
	cPetList(CWnd* pParent = NULL);   // standard constructor
	CComboBox	m_cbPetType;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	BOOL		m_bIsVirgin;

	DECLARE_MESSAGE_MAP()
public:
	cPetDefine*	getPet()
	{
		return	&m_Pet;
	}

	void		setPetInfo(cPetDefine* _Pet);
	void		OnRemovePet1();
	void		OnRemovePet2();
	void		OnSave();
	void		OnSave(int _iIndex);

	virtual BOOL OnInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPETLIST_H__926AAEF5_2578_4AA2_967C_6E93A7C58D97__INCLUDED_)
