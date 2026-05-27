// ExpCalc.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "ExpCalc.h"
#include "SFC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ExpCalc dialog


ExpCalc::ExpCalc(CWnd* pParent /*=NULL*/)
	: CDialog(ExpCalc::IDD, pParent)
{
	//{{AFX_DATA_INIT(ExpCalc)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ExpCalc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ExpCalc)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ExpCalc, CDialog)
	//{{AFX_MSG_MAP(ExpCalc)
	ON_BN_CLICKED(IDCANCEL2, OnCancel2)
	ON_BN_CLICKED(IDC_CALCULATE, OnCalculate)
	ON_BN_CLICKED(IDC_CAL_REBIRTH_EXP, OnCalRebirthExp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ExpCalc message handlers

void ExpCalc::OnCancel2() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void ExpCalc::OnCalculate() 
{
	// TODO: Add your control notification handler code here
	int			aiLevel[10][2];
	int			aiExp[10][2];
	LONGLONG	llExpSum	=	0;

	aiLevel[0][0]	=	GetDlgItemInt(IDC_LEVEL11);
	aiLevel[0][1]	=	GetDlgItemInt(IDC_LEVEL12);

	aiExp[0][0]		=	GetDlgItemInt(IDC_EXP11);
	aiExp[0][1]		=	GetDlgItemInt(IDC_EXP12);

	for (int i=0;i<1;i++)
	{
		if (aiLevel[i][0] == 0 || aiLevel[i][1] == 0 || aiLevel[i][0] >= 1000 || aiLevel[i][1] >= 1000)	break;

		int	iCorrectLevel	=	aiLevel[i][1];
		int	iDownLevel		=	aiLevel[i][0];
		int	iCorrectExp		=	aiExp[i][1];
		int	iDownExp		=	aiExp[i][0];

		if (iCorrectLevel == iDownLevel)
			llExpSum	+=	(iCorrectExp-iDownExp);
		else
		for (int iLevel = iDownLevel;iLevel<=iCorrectLevel;iLevel++)
		{
			if (iLevel	==	iDownLevel)	
				llExpSum	+=	(g_aExpTable[iLevel]-iDownExp);
			else
			if (iLevel	==	iCorrectLevel)	
				llExpSum	+=	iCorrectExp;
			else
				llExpSum	+=	g_aExpTable[iLevel];
		}
	}

	char	strExp[64];

	int		iHighExp	=	llExpSum/1000000000;
	int		iLowExp		=	llExpSum%1000000000;

	if	(iHighExp)
		sprintf(strExp,"%d%.9d",iHighExp,iLowExp);
	else
		sprintf(strExp,"%d",iLowExp);

	SetDlgItemText(IDC_RESULT,strExp);
}	//	void ExpCalc::OnCalculate()


int
ExpCalc::GetExp(int _iLevel1,int _iExp1,int _iLevel2,int _iExp2)
{
	if	(_iLevel1	==	_iLevel2)
		return	abs(_iExp1-_iExp2);

	if	(_iLevel1	>	_iLevel2)
	{
		nsSfc::swap(_iLevel1,_iLevel2);
		nsSfc::swap(_iExp1,_iExp2);
	}

	int	iExpSum	=	0;

	iExpSum	+=	g_aExpTable[_iLevel1]-_iExp1;

	for (int i=_iLevel1+1;i<_iLevel2;i++)
		iExpSum	+=	g_aExpTable[i];

	iExpSum	+=	_iExp2;

	return	iExpSum;
}


void ExpCalc::OnCalRebirthExp() 
{
	if	(!m_lpPlayer)
	{
#ifdef	_FOR_KOREA
		cMSG::Error("불러온 캐릭터 정보가 없습니다.");
#else
		cMSG::Error("invlaid character data");
#endif
		return;
	}

	int	iLevel	=	GetDlgItemInt(IDC_REBIRTH_LEVEL);
	int	iExp	=	GetDlgItemInt(IDC_REBIRTH_EXP);

	if	(iLevel	<	600)
	{
#ifdef	_FOR_KOREA
		cMSG::Error("최소 전생 레벨은 600입니다.");
#else
		cMSG::Error("rebirth level check failed(600 <= )");
#endif
		return;
	}

	if	(iLevel	==	600 && iExp == 0)
	{
#ifdef	_FOR_KOREA
		cMSG::Error("보정할 경험치가 없습니다.");
#else
		cMSG::Error("revise exp zero");
#endif
		return;
	}

#ifdef	_FOR_KOREA
	if	(cMSG::YESNO("구 전생자 경험치 보상","레벨 600에서 레벨 %d에 경험치 %d까지의 경험치를 현재 캐릭터에게 더해줍니다.\n\n계속하시겠습니까?",iLevel,iExp) == IDNO)
		return;
#else
	if	(cMSG::YESNO("compensate Rebirth Exp","from Lv 600 to Lv %d and Exp %d\n\nContinue?",iLevel,iExp) == IDNO)
		return;
#endif

	for (int i=600;i<iLevel;++i)
		m_lpPlayer->increaseExp(g_aExpTable[i]);

	m_lpPlayer->increaseExp(iExp);

#ifdef	_FOR_KOREA
	cMSG::Put("구 전생자 경험치 보상 결과","레벨 %d,경험치 %d",m_lpPlayer->m_iLevel,m_lpPlayer->m_iExperience);
#else
	cMSG::Put("result of compensate Rebirth Exp","Level %d,Exp %d",m_lpPlayer->m_iLevel,m_lpPlayer->m_iExperience);
#endif
}

BOOL
ExpCalc::open(cPLAYER_DATA	*_lpPlayer)
{
	m_lpPlayer	=	_lpPlayer;

	if	(m_lpPlayer)
	{
		m_iOrgLevel	=	m_lpPlayer->m_iLevel;
		m_iOrgExp	=	m_lpPlayer->m_iExperience;
	}

	DoModal();

	if	(m_lpPlayer && m_lpPlayer->m_iLevel	!=	m_iOrgLevel || m_lpPlayer->m_iExperience !=	m_iOrgExp)
	{
#ifdef	_FOR_KOREA
		if	(cMSG::YESNO("구 전생자 경험치 보상 결과","경험치 보상에 의해 아래와 같이 레벨 변동이 발생했습니다.\n\n레벨 %d,경험치 %d -> 레벨 %d,경험치 %d\n\n적용하시겠습니까?",m_iOrgLevel,m_iOrgExp,m_lpPlayer->m_iLevel,m_lpPlayer->m_iExperience) == IDNO)
		{
			m_lpPlayer->m_iLevel		=	m_iOrgLevel;
			m_lpPlayer->m_iExperience	=	m_iOrgExp;

			return	FALSE;
		}
#else
		if	(cMSG::YESNO("result of compensate Rebirth Exp","change level\n\nLevel %d,Exp %d -> Level %d,Exp %d\n\nAre you Apply?",m_iOrgLevel,m_iOrgExp,m_lpPlayer->m_iLevel,m_lpPlayer->m_iExperience) == IDNO)
		{
			m_lpPlayer->m_iLevel		=	m_iOrgLevel;
			m_lpPlayer->m_iExperience	=	m_iOrgExp;

			return	FALSE;
		}
#endif
		if	(m_lpPlayer->m_bf4RebirthCount	==	0)
		{
	#ifdef	_FOR_KOREA
			cMSG::Error("전생한 캐릭터가 아닙니다.");
	#else
			cMSG::Error("it's not rebirth character");
	#endif
			return	FALSE;
		}

		return	TRUE;
	}

	return	FALSE;
}
