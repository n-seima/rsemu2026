// cMiniPetList.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "cPetList.h"
#include "cMessage.h"
#include "cDECODER.h"
#include "CSkill.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cMiniPetList dialog

cPetList::cPetList(CWnd* pParent): CDialog(IDD_PET_LIST , pParent)
{
}

void cPetList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cMiniPetList)
	//DDX_Control(pDX, IDC_COMBO_MINIPET_TYPE, m_cbPetType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(cPetList, CDialog)
	ON_BN_CLICKED(IDC_REMOVE_PET1, OnRemovePet1)
	ON_BN_CLICKED(IDC_REMOVE_PET2, OnRemovePet2)
	ON_BN_CLICKED(IDOK, OnSave)
END_MESSAGE_MAP()

BOOL 
cPetList::OnInitDialog() 
{
	char	strCode[1024];
	ZeroMemory(strCode , 1024);
	
	MakeCode(strCode , 0);
	
	SetDlgItemText(IDC_PET_CODE1,strCode);

	ZeroMemory(strCode , 1024);
	
	MakeCode(strCode , 1);
	
	SetDlgItemText(IDC_PET_CODE2,strCode);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void
cPetList::setPetInfo(cPetDefine* _Pet)
{
	m_SavePoint = _Pet;
	memcpy(m_aPet , _Pet , sizeof(cPetDefine) * 2);
}

void
cPetList::OnRemovePet1()
{
	char	strCode[1024];

	strcpy(strCode,"[\"\"] [0] [0] [0] [0] [0] [0] [0] [0] [0]");
	
	SetDlgItemText(IDC_PET_CODE1,strCode);
}

void
cPetList::OnRemovePet2()
{
	char	strCode[1024];

	strcpy(strCode,"[\"\"] [0] [0] [0] [0] [0] [0] [0] [0] [0]");
	
	SetDlgItemText(IDC_PET_CODE2,strCode);
}

void
cPetList::MakeCode(char* _strCode , int _iIndex)
{
	char	strName[20];

	memcpy(strName,m_aPet[_iIndex].m_strName,10);

	strName[10]	=	NULL;

	sprintf(_strCode,"[\"%s\"] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d]",
		strName ,
		m_aPet[_iIndex].m_wBreedingTime,
		m_aPet[_iIndex].m_bf14Loyalty,
		m_aPet[_iIndex].m_bf2TamerSlot,
		m_aPet[_iIndex].m_bf10Level,
		m_aPet[_iIndex].m_bf22CurrentHP,
		m_aPet[_iIndex].m_bf10Job,
		m_aPet[_iIndex].m_bf5BoostCount,
		m_aPet[_iIndex].m_bf1Job,
		m_aPet[_iIndex].m_dwExperience);
}

void
cPetList::OnSave()
{
	OnSave(0);
	OnSave(1);
}

void
cPetList::OnSave(int _iIndex)
{
	char strCode[1024];
	ZeroMemory(strCode , 0);

	GetDlgItemText(IDC_PET_CODE1 + _iIndex , strCode , 1024);

	cDECODER	decoder;

	if	(stricmp(strCode,"") == 0)
		return;
	
	if	(!decoder.UploadBuffer(strCode,NULL))
		return;
	
	{
		char	*lpstr;
		BOOL	temp;
		
		lpstr	=	decoder.GetStream();

		if	(lpstr)
			memcpy(m_aPet[_iIndex].m_strName,lpstr,10);
		else
			memset(m_aPet[_iIndex].m_strName,0,sizeof(m_aPet[_iIndex].m_strName));

		m_aPet[_iIndex].m_wBreedingTime		= decoder.GetNumber(&temp);
		m_aPet[_iIndex].m_bf14Loyalty		= decoder.GetNumber(&temp);
		m_aPet[_iIndex].m_bf2TamerSlot		= decoder.GetNumber(&temp);
		m_aPet[_iIndex].m_bf10Level			= decoder.GetNumber(&temp);
		m_aPet[_iIndex].m_bf22CurrentHP		= decoder.GetNumber(&temp);
		m_aPet[_iIndex].m_bf10Job			= decoder.GetNumber(&temp);
		m_aPet[_iIndex].m_bf5BoostCount		= decoder.GetNumber(&temp);
		m_aPet[_iIndex].m_bf1Job			= decoder.GetNumber(&temp);
		m_aPet[_iIndex].m_dwExperience		= decoder.GetNumber(&temp);
	}	

	if	(_iIndex == 0)
		memcpy(m_SavePoint , &m_aPet[0] , sizeof(cPetDefine));
	else
	if	(_iIndex == 1)
		memcpy(m_SavePoint+1, &m_aPet[1] , sizeof(cPetDefine));
}