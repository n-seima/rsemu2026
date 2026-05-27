// GuildPetAndGuardian.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "GuildPetAndGuardian.h"
#include "CGuildDefine.h"
#include "CharacterInfo.h"
#include "cMakeItem.h"
#include "cDECODER.h"
#include "cMESSAGE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuildPetAndGuardian dialog

UINT	
g_auiGuildPetExp[]	=	//	길드펫 누적 경험치
{
	0 ,
	5 ,
	17 ,
	47 ,
	111 ,
	231 ,
	433 ,
	749 ,
	1215 ,
	1873 ,
	2769 ,
	3954 ,
	5485 ,
	7422 ,
	9831 ,
	12783 ,
	16353 ,
	20622 ,
	25674 ,
	31599 ,
	38491 ,
	46449 ,
	55577 ,
	65983 ,
	77780 ,
	91087 ,
	106025 ,
	122722 ,
	141310 ,
	161924 ,
	184706 ,
	209801 ,
	237360 ,
	267537 ,
	300492 ,
	336389 ,
	375396 ,
	417686 ,
	463437 ,
	512832 ,
	566057 ,
	623303 ,
	684766 ,
	750647 ,
	821151 ,
	896487 ,
	976869 ,
	1062516 ,
	1153651 ,
	1250501 ,
	1353298 ,
	1462279 ,
	1577685 ,
	1699761 ,
	1828757 ,
	1964928 ,
	2108533 ,
	2259835 ,
	2419102 ,
	2586606 ,
	2762624 ,
	2947437 ,
	3141332 ,
	3344598 ,
	3557530 ,
	3780426 ,
	4013591 ,
	4257332 ,
	4511962 ,
	4777797 ,
	5055159 ,
	5344373 ,
	5645769 ,
	5959682 ,
	6286450 ,
	6626417 ,
	6979930 ,
	7347342 ,
	7729009 ,
	8125291 ,
	8536555 ,
	8963169 ,
	9405508 ,
	9863950 ,
	10338878 ,
	10830679 ,
	11339745 ,
	11866472 ,
	12411260 ,
	12974514 ,
	13556642 ,
	14158059 ,
	14779181 ,
	15420432 ,
	16082237 ,
	16765028 ,
	17469239 ,
	18195310 ,
	18943685 ,
	19714813 ,
	c_iMaxGuildPetExp ,
};

int
getPetLevel(int _iExp)
{
	for(int iLevel=0;iLevel<100;iLevel++)
	{
		if	(_iExp	>=	(int)g_auiGuildPetExp[iLevel]	&&	_iExp	<	(int)g_auiGuildPetExp[iLevel+1])
			return	iLevel+1;
	}

	return	100;
}


char	*lpstrPetName[4];
char	*lpstrGuardianName[4];

CGuildPetAndGuardian::CGuildPetAndGuardian(CWnd* pParent /*=NULL*/)
	: CDialog(CGuildPetAndGuardian::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGuildPetAndGuardian)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGuildPetAndGuardian::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGuildPetAndGuardian)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGuildPetAndGuardian, CDialog)
	//{{AFX_MSG_MAP(CGuildPetAndGuardian)
	ON_BN_CLICKED(IDC_EXP_SET_TO_PET, OnExpSetToPet)
	ON_BN_CLICKED(IDC_EXP_SET_TO_GUARDIAN, OnExpSetToGuardian)
	ON_BN_CLICKED(IDC_CHANGE_PET_EXP, OnChangePetExp)
	ON_BN_CLICKED(IDC_CHANGE_GUARDIAN_EXP, OnChangeGuardianExp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuildPetAndGuardian message handlers

BOOL CGuildPetAndGuardian::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	lpstrPetName[0]			=	g_strToolMessage[162];
	lpstrPetName[1]			=	g_strToolMessage[163];
	lpstrPetName[2]			=	g_strToolMessage[164];
	lpstrPetName[3]			=	g_strToolMessage[165];

	lpstrGuardianName[0]	=	g_strToolMessage[166];
	lpstrGuardianName[1]	=	g_strToolMessage[167];
	lpstrGuardianName[2]	=	g_strToolMessage[168];
	lpstrGuardianName[3]	=	g_strToolMessage[169];

	m_listPet.init(GetDlgItem(IDC_ITEM_LIST)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	::SendMessage(GetDlgItem(IDC_ITEM_LIST)->m_hWnd,WM_SETFONT,(WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 0 );

	m_listPet.addColumn(0,28,"No");
	m_listPet.addColumn(1,12*10,"Name");
	m_listPet.addColumn(2,28,"LV");
	m_listPet.addColumn(3,80,"EXP");

	m_listGuardian.init(GetDlgItem(IDC_ITEM_LIST2)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	::SendMessage(GetDlgItem(IDC_ITEM_LIST2)->m_hWnd,WM_SETFONT,(WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 0 );

	m_listGuardian.addColumn(0,28,"No");
	m_listGuardian.addColumn(1,12*10,"Name");
	m_listGuardian.addColumn(2,28,"LV");
	m_listGuardian.addColumn(3,80,"EXP");

	m_listExp.init(GetDlgItem(IDC_EXP_TABLE)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	::SendMessage(GetDlgItem(IDC_EXP_TABLE)->m_hWnd,WM_SETFONT,(WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 0 );

	m_listExp.addColumn(0,28,"LV");
	m_listExp.addColumn(1,160,"EXP");

	int	iExpIndex	=	0;
	while(1)
	{
		if	(g_auiGuildPetExp[iExpIndex]	>=	c_iMaxGuildPetExp)
			break;

		m_listExp.addItem(iExpIndex,0,_ms("%d",iExpIndex+1),iExpIndex);
		m_listExp.setItem(iExpIndex,1,_ms("%d",g_auiGuildPetExp[iExpIndex]));
		iExpIndex++;
	}

	updateList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void
CGuildPetAndGuardian::updateList()
{
	int	i;

	m_listPet.reset();
	m_listGuardian.reset();

	for (i=0;i<dGUILD_PET_TYPE_COUNT;i++)
	{
		CGuildPetInfo	*lpPet	=	&m_lpGuild->m_aGuildPet[i];

		m_listPet.addItem(i,0,_ms("%d",i+1),i);
		m_listPet.setItem(i,1,lpstrPetName[i]);
		m_listPet.setItem(i,2,_ms("%d",getPetLevel(lpPet->m_dwExperience)));
		m_listPet.setItem(i,3,_ms("%d",lpPet->m_dwExperience));
	}

	for (i=0;i<dGUILD_GUARDIAN_TYPE_COUNT;i++)
	{
		CGuildPetInfo	*lpGuardian	=	&m_lpGuild->m_aGuildGuardian[i];

		m_listGuardian.addItem(i,0,_ms("%d",i+1),i);
		m_listGuardian.setItem(i,1,lpstrGuardianName[i]);
		m_listGuardian.setItem(i,2,_ms("%d",getPetLevel(lpGuardian->m_dwExperience)));
		m_listGuardian.setItem(i,3,_ms("%d",lpGuardian->m_dwExperience));
	}
}

void
CGuildPetAndGuardian::changePetExp()
{
}

void
CGuildPetAndGuardian::changeGuardianExp()
{
}

void
CGuildPetAndGuardian::setGuild(CGuildAdvanceInfo	*_lpGuild)
{
	m_lpGuild	=	_lpGuild;
}

void CGuildPetAndGuardian::OnExpSetToPet() 
{
	// TODO: Add your control notification handler code here
	
}

void CGuildPetAndGuardian::OnExpSetToGuardian() 
{
	// TODO: Add your control notification handler code here
	
}

void CGuildPetAndGuardian::OnChangePetExp() 
{
	// TODO: Add your control notification handler code here
	
}

void 
CGuildPetAndGuardian::OnChangeGuardianExp() 
{
	char	strScript[1024];

	GetDlgItemText(IDC_EDIT_PET,strScript,sizeof(strScript));

	int	iExp		=	atoi(strScript);

	if	(iExp	<	0	||	iExp	>=	c_iMaxGuildPetExp)
		return;

	BOOL	bResult;
	int	iGuardian	=	m_listGuardian.getSelectedItemLParam(&bResult);

	if	(!bResult)
		return;

	// TODO: Add your control notification handler code here
	
}
