// cMiniPetList.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "cMiniPetList.h"
#include "cMessage.h"
#include "cDECODER.h"
#include "../common/CSkill.h"
#include "CharacterInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cMiniPetList dialog


cMiniPetList::cMiniPetList(CWnd* pParent /*=NULL*/)
	: CDialog(cMiniPetList::IDD, pParent)
{
	//{{AFX_DATA_INIT(cMiniPetList)
	//}}AFX_DATA_INIT
}


void cMiniPetList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cMiniPetList)
	DDX_Control(pDX, IDC_COMBO_MINIPET_TYPE, m_cbMiniPetType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cMiniPetList, CDialog)
	//{{AFX_MSG_MAP(cMiniPetList)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_ADD_BY_CODE, OnAddByCode)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_GET_SELECTED_PET_INFO, OnGetSelectedPetInfo)
	ON_BN_CLICKED(IDC_FILL_DATE_BY_CURRENT_TIME, OnFillDateByCurrentTime)
	ON_BN_CLICKED(IDC_GET_CODE, OnGetCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cMiniPetList message handlers

void
cMiniPetList::setMiniPet(CAvatarData *_lpPlayer,cMiniPetCommon* _lpMiniPet,int _iYear,int _iMonth,int _iDay,int _iHour,int _iMinute)
{
	m_lpPlayer	=	_lpPlayer;

	memcpy(m_aMiniPet,_lpMiniPet,sizeof(m_aMiniPet));

	m_reversalFramevalidate.m_wYear	=	_iYear+2000;
	m_reversalFramevalidate.m_wMonth=	_iMonth;
	m_reversalFramevalidate.m_wDay	=	_iDay;
	m_reversalFramevalidate.m_wHour	=	_iHour;
	m_reversalFramevalidate.m_wMinute=	_iMinute;
}

BOOL cMiniPetList::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_bIsVirgin	=	TRUE;
	m_listMiniPet.init(GetDlgItem(IDC_LIST)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	::SendMessage(GetDlgItem(IDC_LIST)->m_hWnd,WM_SETFONT,(WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 0 );

	m_listMiniPet.addColumn(0,40,"Slot");
	m_listMiniPet.addColumn(1,76,"Type");
	m_listMiniPet.addColumn(2,96,"name");
	m_listMiniPet.addColumn(3,32,"LV");
	m_listMiniPet.addColumn(4,32,"S.P");
	m_listMiniPet.addColumn(5,86,"Bio");
	m_listMiniPet.addColumn(6,86,"Energy");
	m_listMiniPet.addColumn(7,86,"Mineral");
	m_listMiniPet.addColumn(8,124,"Expire Date");

	{
		m_cbMiniPetType.ResetContent();

		for (int i=0;i<nsMiniPetType::Count;i++)	
			m_cbMiniPetType.AddString(g_strMiniPetName[i]);

		OnReset();
	}

	SetDlgItemText(IDC_RESTRAINT_FIRE,_ms("%d",m_lpPlayer->m_bf4EatFireMiniPetCount));
	SetDlgItemText(IDC_RESTRAINT_WATER,_ms("%d",m_lpPlayer->m_bf4EatWaterMiniPetCount));
	SetDlgItemText(IDC_RESTRAINT_WIND,_ms("%d",m_lpPlayer->m_bf4EatWindMiniPetCount));
	SetDlgItemText(IDC_RESTRAINT_EARTH,_ms("%d",m_lpPlayer->m_bf4EatEarthMiniPetCount));
	SetDlgItemText(IDC_RESTRAINT_LIGHT,_ms("%d",m_lpPlayer->m_bf4EatLightMiniPetCount));
	SetDlgItemText(IDC_RESTRAINT_DARK,_ms("%d",m_lpPlayer->m_bf4EatDarkMiniPetCount));

	SetDlgItemText(IDC_RESTRAINT_BONUS_STRENGTH,_ms("+%d",m_lpPlayer->m_bf4EatFireMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET));
	SetDlgItemText(IDC_RESTRAINT_BONUS_WISDOM,_ms("+%d",m_lpPlayer->m_bf4EatWaterMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET));
	SetDlgItemText(IDC_RESTRAINT_BONUS_AGILITY,_ms("+%d",m_lpPlayer->m_bf4EatWindMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET));
	SetDlgItemText(IDC_RESTRAINT_BONUS_CONSTITUTION,_ms("+%d",m_lpPlayer->m_bf4EatEarthMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET));
	SetDlgItemText(IDC_RESTRAINT_BONUS_INTELLIGENCE,_ms("+%d",m_lpPlayer->m_bf4EatLightMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET));
	SetDlgItemText(IDC_RESTRAINT_BONUS_CHARISMA,_ms("+%d",m_lpPlayer->m_bf4EatDarkMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET));

	updateList();

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void cMiniPetList::updateList()
{
	m_listMiniPet.reset();

	int	iCount	=	0;

	for (int i=0;i<c_iMiniPetCount;i++)
	{
		cMiniPetCommon	*lpMiniPet	=	&m_aMiniPet[i];

		if	(lpMiniPet->m_bf7Level	==	0)
			continue;

		char	strName[32];

		memcpy(strName,lpMiniPet->m_strName,c_iMiniPetNameLength);
		strName[c_iMiniPetNameLength]	=	NULL;

		m_listMiniPet.addItem(iCount,0,_ms("%d",i),i);
		m_listMiniPet.setItem(iCount,1,g_strMiniPetName[lpMiniPet->m_bf5Type]);
		m_listMiniPet.setItem(iCount,2,strName);
		m_listMiniPet.setItem(iCount,3,_ms("%d",lpMiniPet->m_bf7Level));

		int	iSum	=	max(lpMiniPet->m_iVitalPoint+lpMiniPet->m_iEnergyPoint+lpMiniPet->m_iMineralPoint,1);

		m_listMiniPet.setItem(iCount,4,_ms("%d",lpMiniPet->m_bf7SkillPoint));
		
		m_listMiniPet.setItem(iCount,5,_ms("%d:%.2d%%",lpMiniPet->m_iVitalPoint,((LONGLONG)lpMiniPet->m_iVitalPoint)*100/iSum));
		m_listMiniPet.setItem(iCount,6,_ms("%d:%.2d%%",lpMiniPet->m_iEnergyPoint,((LONGLONG)lpMiniPet->m_iEnergyPoint)*100/iSum));
		m_listMiniPet.setItem(iCount,7,_ms("%d:%.2d%%",lpMiniPet->m_iMineralPoint,((LONGLONG)lpMiniPet->m_iMineralPoint)*100/iSum));
		m_listMiniPet.setItem(iCount,8,_ms("%d/%d/%d %d:%d",lpMiniPet->m_bf5ExpireYear+2000,lpMiniPet->m_bf4ExpireMonth,lpMiniPet->m_bf5ExpireDay,
											lpMiniPet->m_bf5ExpireHour,lpMiniPet->m_bf6ExpireMinute));

		iCount++;
	}
}

void cMiniPetList::OnRemove() 
{
	BOOL	bIsSuccess;

	int	iIndex	=	m_listMiniPet.getSelectedItemLParam(&bIsSuccess);

	if	(!bIsSuccess)
	{
		cMSG::Put("warning","please select a mini pet in left list");
		return;
	}

	memset(&m_aMiniPet[iIndex],0,sizeof(cMiniPetCommon));

	updateList();

	m_bIsVirgin	=	FALSE;
}

void cMiniPetList::OnAdd() 
{
	int iEmptySlot	=	-1;

	for (int i = 0 ;i<c_iMiniPetCount;i++)
	{
		if	(m_aMiniPet[i].m_bf7Level	==	0)
		{
			iEmptySlot	=	i;
			break;
		}
	}

	if	(iEmptySlot	==	-1)
	{
		cMSG::Put("warning","minipet slot full!!");
		return;
	}

	if	(!getEditMiniPetInfo())
		return;
	reviseMiniPetInfo();

	memcpy(&m_aMiniPet[iEmptySlot],&m_miniPet,sizeof(cMiniPetCommon));

	updateList();

	m_bIsVirgin	=	FALSE;
	// TODO: Add your control notification handler code here
	
}

void cMiniPetList::OnAddByCode() 
{
	// TODO: Add your control notification handler code here

	int iEmptySlot	=	-1;

	for (int i = 0 ;i<c_iMiniPetCount;i++)
	{
		if	(m_aMiniPet[i].m_bf7Level	==	0)
		{
			iEmptySlot	=	i;
			break;
		}
	}

	if	(iEmptySlot	==	-1)
	{
		cMSG::Put("warning","minipet slot full!!");
		return;
	}

	char	strCode[1024];

	GetDlgItemText(IDC_CODE,strCode,1024);

	cMiniPetCommon	miniPet;
	cDECODER		decoder;
	BOOL			bIsSuccess;

	decoder.UploadBuffer(strCode,NULL);

//	이름
	{
		char	*lpstrName	=	decoder.GetStream();

		if	(strlen(lpstrName) < 1 || strlen(lpstrName) > c_iMiniPetNameLength)
		{
			cMSG::Put("warning","incorrect name length\n\n1~10");
			return;
		}

		memcpy(miniPet.m_strName,lpstrName,c_iMiniPetNameLength);
	}

//	레벨,경험치,타입
	{
		int	iLevel	=	decoder.GetNumber(&bIsSuccess);

		if	(!bIsSuccess)
			return;

		if	(iLevel	<=	0	||	iLevel	>	100)
		{
			cMSG::Put("warning","incorrect level\n\n1~100");
			return;
		}

		miniPet.m_bf7Level	=	iLevel;

		int	iExp	=	decoder.GetNumber(&bIsSuccess);

		if	(!bIsSuccess)
			return;

		if	(iExp	>	0xffff)
		{
			cMSG::Put("warning","incorrect exp\n\n0~65535");
			return;
		}

		miniPet.m_bf16Exp	=	iExp;

		int	iType	=	decoder.GetNumber(&bIsSuccess);

		if	(!bIsSuccess)
			return;

		if	(iType	<	0	||	iType	>=	nsMiniPetType::Count)
		{
			cMSG::Put("warning","incorrect type\n\n0~23");
			return;
		}

		miniPet.m_bf5Type	=	iType;
	}
//	유효기간
	{
		int	iExpireYear	=	decoder.GetNumber(&bIsSuccess);

		if	(!bIsSuccess)
			return;

		int	iExpireMonth	=	decoder.GetNumber(&bIsSuccess);

		if	(!bIsSuccess)
			return;

		int	iExpireDay	=	decoder.GetNumber(&bIsSuccess);

		if	(!bIsSuccess)
			return;

		int	iExpireHour	=	decoder.GetNumber(&bIsSuccess);

		if	(!bIsSuccess)
			return;

		int	iExpireMinute	=	decoder.GetNumber(&bIsSuccess);

		if	(!bIsSuccess)
			return;

		if	(iExpireYear	<	2007	||	iExpireYear	>=	2032)
		{
			cMSG::Put("warning","incorrect year\n\n2007~2031");
			return;
		}

		if	(iExpireMonth	>	12)
		{
			cMSG::Put("warning","incorrect month\n\n1~12");
			return;
		}

		if	(iExpireDay	>	31)
		{
			cMSG::Put("warning","incorrect day\n\n1~31");
			return;
		}

		if	(iExpireHour	>=	24)
		{
			cMSG::Put("warning","incorrect hour\n\n0~23");
			return;
		}

		if	(iExpireMinute	>=	60)
		{
			cMSG::Put("warning","incorrect minute\n\n0~59");
			return;
		}

		miniPet.m_bf5ExpireYear=	iExpireYear-2000;
		miniPet.m_bf4ExpireMonth=	iExpireMonth;
		miniPet.m_bf5ExpireDay=	iExpireDay;
		miniPet.m_bf5ExpireHour=	iExpireHour;
		miniPet.m_bf6ExpireMinute=iExpireMinute;
	}

//	진화 형태
	{
		miniPet.m_iVitalPoint	=	decoder.GetNumber(&bIsSuccess);;
		if	(!bIsSuccess)
			return;
		miniPet.m_iEnergyPoint=	decoder.GetNumber(&bIsSuccess);;
		if	(!bIsSuccess)
			return;
		miniPet.m_iMineralPoint=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)
			return;
	}

//	스킬 포인트 체크
	{
		int	iPointSum	=	0;

		for(int i=0;i<c_iMiniPetSkillCount;i++)
		{
			int	iSkill	=	decoder.GetNumber(&bIsSuccess);;
			if	(!bIsSuccess)
				return;
			int	iLevel	=	decoder.GetNumber(&bIsSuccess);;
			if	(!bIsSuccess)
				return;

			if	(iSkill	<	0	||	iSkill	>=	dMAX_SKILL)
				return;

			if	(g_aSkill[iSkill].m_wSerial	==	0xffff)
				return;

			if	(iLevel	<	0	||	iLevel	>	5)
				return;

			cSKILL	*lpSkill	=	&g_aSkill[iSkill];

			miniPet.m_aAbility[i].m_bf11Skill	=	iSkill;
			miniPet.m_aAbility[i].m_bf4Level	=	iLevel;

			if	(iSkill	==	0	&&	iLevel	==	0)
			{
				if	(m_miniPet.m_bf5Type	<	6	&&	i	>=	5)
					continue;
			}
			
			if	(i	<	5)
			{
				if	(lpSkill->m_bf5MiniPetType	!=	miniPet.m_bf5Type%6)
				{
					cMSG::Put("warning","incorrect skill %s",lpSkill->m_strName);
					return;
				}
			}
			else
			if	(lpSkill->m_bf5MiniPetType	!=	miniPet.m_bf5Type)
			{
				cMSG::Put("warning","incorrect skill %s",lpSkill->m_strName);
				return;
			}

			for	(int j=0;j<iLevel;j++)
			{
				if	(g_aSkill[iSkill].m_bf1IsDefaultMiniPetSkill	&&	j	==	0)
					continue;

				iPointSum	+=	j+1;
			}
		}

		int	iCorrectPoint	=	miniPet.m_bf7Level-1;
		int	iSkillPoint		=	decoder.GetNumber(&bIsSuccess);;

		if	(iCorrectPoint	!=	iSkillPoint+iPointSum)
		{
			cMSG::Put("warning","incorrect skill info");
			return;
		}

		miniPet.m_bf7SkillPoint	=	iSkillPoint;
	}

	memcpy(&m_aMiniPet[iEmptySlot],&miniPet,sizeof(cMiniPetCommon));

	updateList();
}

void cMiniPetList::OnOK() 
{
	getEditMiniPetInfo();

	// TODO: Add extra validation here
	if	(m_bIsVirgin	==	FALSE)
	{
#ifdef	_FOR_KOREA
		if	(cMSG::YESNO("미니펫 업데이트","미니펫 데이터가 변경되었습니다. 수정된 데이터를 적용 하시겠습니까?")==IDNO)
			return;
#else
		if	(cMSG::YESNO("upadte minipet","modified mini pet data. do you want apply this data?")==IDNO)
			return;
#endif
	}

	CDialog::OnOK();
}

void cMiniPetList::OnCancel() 
{
	CDialog::OnCancel();
}

void
cMiniPetList::resetEditMiniPet()
{
	memset(&m_miniPet,0,sizeof(m_miniPet));

	strcpy(m_miniPet.m_strName,"noname");
	m_miniPet.m_bf5Type		=	0;
	m_miniPet.m_bf7Level	=	1;
	m_miniPet.m_bf16Exp		=	0;

	m_miniPet.m_iVitalPoint	=	0;
	m_miniPet.m_iEnergyPoint=	0;
	m_miniPet.m_iMineralPoint=	0;
}

void
cMiniPetList::fillEditMiniPetInfo()
{
	m_cbMiniPetType.SetCurSel(m_miniPet.m_bf5Type);

	char	strName[20];

	memcpy(strName,m_miniPet.m_strName,c_iMiniPetNameLength);

	strName[c_iMiniPetNameLength]	=	NULL;

	SetDlgItemText(IDC_NAME,strName);
	SetDlgItemInt(IDC_LEVEL,m_miniPet.m_bf7Level);
	SetDlgItemInt(IDC_EXP,m_miniPet.m_bf16Exp);

	SetDlgItemInt(IDC_BIO,m_miniPet.m_iVitalPoint);
	SetDlgItemInt(IDC_ENERGY,m_miniPet.m_iEnergyPoint);
	SetDlgItemInt(IDC_MINERAL,m_miniPet.m_iMineralPoint);

	if	(m_miniPet.m_bf5ExpireYear)
		SetDlgItemInt(IDC_YEAR,m_miniPet.m_bf5ExpireYear+2000);
	else
		SetDlgItemInt(IDC_YEAR,m_miniPet.m_bf5ExpireYear);
	SetDlgItemInt(IDC_MONTH,m_miniPet.m_bf4ExpireMonth);
	SetDlgItemInt(IDC_DAY,m_miniPet.m_bf5ExpireDay);
	SetDlgItemInt(IDC_HOUR,m_miniPet.m_bf5ExpireHour);
	SetDlgItemInt(IDC_MINUTE,m_miniPet.m_bf6ExpireMinute);

	SetDlgItemInt(IDC_YEAR2,m_reversalFramevalidate.m_wYear);
	SetDlgItemInt(IDC_MONTH2,m_reversalFramevalidate.m_wMonth);
	SetDlgItemInt(IDC_DAY2,m_reversalFramevalidate.m_wDay);
	SetDlgItemInt(IDC_HOUR2,m_reversalFramevalidate.m_wHour);
	SetDlgItemInt(IDC_MINUTE2,m_reversalFramevalidate.m_wMinute);
}

void
cMiniPetList::reviseMiniPetInfo()
{
	int	iType		=	m_miniPet.m_bf5Type%6;
	int	iSkillCount	=	0;

	for (int i=0;i<dMAX_SKILL && iSkillCount < c_iMiniPetSkillCount;i++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_miniPet.m_aAbility[iSkillCount];
		cSKILL				*lpSkill	=	&g_aSkill[i];

		if	(lpSkill->m_wSerial	==	0xffff)
			continue;

		if	(lpSkill->m_bf5MiniPetType	!=	(UINT)iType)
			continue;

		lpAbility->m_bf11Skill	=	i;
		lpAbility->m_bf4Level	=	0;

		if	(lpSkill->m_bf1IsDefaultMiniPetSkill)
			lpAbility->m_bf4Level	=	1;

		iSkillCount++;
	}
	
	m_miniPet.m_bf7SkillPoint	=	m_miniPet.m_bf7Level-1;

	if	(m_miniPet.m_bf5Type	<=	nsMiniPetType::SpritOfDark)
		return;

	for (i=0;i<dMAX_SKILL && iSkillCount < c_iMiniPetSkillCount;i++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_miniPet.m_aAbility[iSkillCount];
		cSKILL				*lpSkill	=	&g_aSkill[i];

		if	(lpSkill->m_wSerial	==	0xffff)
			continue;

		if	(lpSkill->m_bf5MiniPetType	!=	m_miniPet.m_bf5Type)
			continue;

		lpAbility->m_bf11Skill	=	i;
		lpAbility->m_bf4Level	=	0;

		iSkillCount++;
	}

	m_miniPet.m_bf7SkillPoint	=	m_miniPet.m_bf7Level-1;
}

BOOL
cMiniPetList::getEditMiniPetInfo()
{
	char	strName[20];

	GetDlgItemText(IDC_NAME,strName,20);

	if	(strlen(strName)	>	c_iMiniPetNameLength)
	{
		cMSG::Put("warning","incorrect name length\n\n1~10");
		return	FALSE;
	}

	memcpy(m_miniPet.m_strName,strName,c_iMiniPetNameLength);

	m_miniPet.m_bf5Type		=	m_cbMiniPetType.GetCurSel();
	m_miniPet.m_bf7Level	=	GetDlgItemInt(IDC_LEVEL);
	m_miniPet.m_bf16Exp		=	GetDlgItemInt(IDC_EXP);

	if	(m_miniPet.m_bf7Level	>	100)
	{
		cMSG::Put("warning","incorrect level\n\n1~100");
		return	FALSE;
	}

	m_miniPet.m_iVitalPoint	=	GetDlgItemInt(IDC_BIO,NULL,FALSE);
	m_miniPet.m_iEnergyPoint=	GetDlgItemInt(IDC_ENERGY,NULL,FALSE);
	m_miniPet.m_iMineralPoint=	GetDlgItemInt(IDC_MINERAL,NULL,FALSE);

	int	iExpireYear			=	GetDlgItemInt(IDC_YEAR,NULL,FALSE);
	int	iExpireMonth		=	GetDlgItemInt(IDC_MONTH,NULL,FALSE);
	int	iExpireDay			=	GetDlgItemInt(IDC_DAY,NULL,FALSE);
	int	iExpireHour			=	GetDlgItemInt(IDC_HOUR,NULL,FALSE);
	int	iExpireMinute		=	GetDlgItemInt(IDC_MINUTE,NULL,FALSE);

	m_editReversalFrameValidate.m_wYear		=	GetDlgItemInt(IDC_YEAR2,NULL,FALSE);
	m_editReversalFrameValidate.m_wMonth	=	GetDlgItemInt(IDC_MONTH2,NULL,FALSE);
	m_editReversalFrameValidate.m_wDay		=	GetDlgItemInt(IDC_DAY2,NULL,FALSE);
	m_editReversalFrameValidate.m_wHour		=	GetDlgItemInt(IDC_HOUR2,NULL,FALSE);
	m_editReversalFrameValidate.m_wMinute	=	GetDlgItemInt(IDC_MINUTE2,NULL,FALSE);

	if	(m_reversalFramevalidate.m_wYear	!=	m_editReversalFrameValidate.m_wYear	)
		m_bIsVirgin	=	FALSE;
	if	(m_reversalFramevalidate.m_wMonth!=	m_editReversalFrameValidate.m_wMonth)
		m_bIsVirgin	=	FALSE;
	if	(m_reversalFramevalidate.m_wDay	!=	m_editReversalFrameValidate.m_wDay	)
		m_bIsVirgin	=	FALSE;
	if	(m_reversalFramevalidate.m_wHour	!=	m_editReversalFrameValidate.m_wHour	)
		m_bIsVirgin	=	FALSE;
	if	(m_reversalFramevalidate.m_wMinute!=	m_editReversalFrameValidate.m_wMinute)
		m_bIsVirgin	=	FALSE;

	if	(iExpireYear	==	0)
	{
		CTimeInfo	curTime;

		curTime.update();
		curTime.increaseDay(30);

		iExpireYear			=	curTime.m_wYear;
		iExpireMonth		=	curTime.m_wMonth;
		iExpireDay			=	curTime.m_wDay;
		iExpireHour			=	curTime.m_wHour;
		iExpireMinute		=	curTime.m_wMinute;
	}

	if	(iExpireYear	<	2007	||	iExpireYear	>=	2032)
	{
		cMSG::Put("warning","incorrect year\n\n2007~2031");
		return	FALSE;
	}

	if	(iExpireMonth	>	12)
	{
		cMSG::Put("warning","incorrect month\n\n1~12");
		return	FALSE;
	}

	if	(iExpireDay	>	31)
	{
		cMSG::Put("warning","incorrect day\n\n1~31");
		return	FALSE;
	}

	if	(iExpireHour	>=	24)
	{
		cMSG::Put("warning","incorrect hour\n\n0~23");
		return	FALSE;
	}

	if	(iExpireMinute	>=	60)
	{
		cMSG::Put("warning","incorrect minute\n\n0~59");
		return	FALSE;
	}

	m_miniPet.m_bf5ExpireYear=	iExpireYear-2000;
	m_miniPet.m_bf4ExpireMonth=	iExpireMonth;
	m_miniPet.m_bf5ExpireDay=	iExpireDay;
	m_miniPet.m_bf5ExpireHour=	iExpireHour;
	m_miniPet.m_bf6ExpireMinute=iExpireMinute;

	strcpy(m_miniPet.m_strName,"noname");

	return	TRUE;
}

void cMiniPetList::OnReset()
{
	resetEditMiniPet();

	fillEditMiniPetInfo();
}

void cMiniPetList::OnUpdate() 
{
	BOOL	bIsSuccess;

	int	iIndex	=	m_listMiniPet.getSelectedItemLParam(&bIsSuccess);

	if	(!bIsSuccess)
	{
		cMSG::Put("warning","please select a mini pet in left list");
		return;
	}

	if	(!getEditMiniPetInfo())
		return;

	reviseMiniPetInfo();

	memcpy(&m_aMiniPet[iIndex],&m_miniPet,sizeof(cMiniPetCommon));

	updateList();
	m_bIsVirgin	=	FALSE;
}

void cMiniPetList::OnGetSelectedPetInfo() 
{
	BOOL	bIsSuccess;

	int	iIndex	=	m_listMiniPet.getSelectedItemLParam(&bIsSuccess);

	if	(!bIsSuccess)
	{
		cMSG::Put("warning","please select a mini pet in left list");
		return;
	}

	memcpy(&m_miniPet,&m_aMiniPet[iIndex],sizeof(cMiniPetCommon));

	fillEditMiniPetInfo();
	// TODO: Add your control notification handler code here
}

void cMiniPetList::OnFillDateByCurrentTime() 
{
	// TODO: Add your control notification handler code here

	CTimeInfo	curTime;

	curTime.update();

	SetDlgItemInt(IDC_YEAR,curTime.m_wYear);
	SetDlgItemInt(IDC_MONTH,curTime.m_wMonth);
	SetDlgItemInt(IDC_DAY,curTime.m_wDay);
	SetDlgItemInt(IDC_HOUR,curTime.m_wHour);
	SetDlgItemInt(IDC_MINUTE,curTime.m_wMinute);
}

void cMiniPetList::OnGetCode() 
{
	BOOL	bIsSuccess;

	int	iIndex	=	m_listMiniPet.getSelectedItemLParam(&bIsSuccess);

	if	(!bIsSuccess)
	{
		cMSG::Put("warning","please select a mini pet in left list");
		return;
	}

	cMiniPetCommon	*lpMiniPet	=	&m_aMiniPet[iIndex];

	char	strCode[1024];

	sprintf(strCode,"[\"%s\"] [%d,%d,%d] [%d/%d/%d %d:%d] [%d,%d,%d] [%d:%d] [%d:%d] [%d:%d] [%d:%d] [%d:%d] [%d:%d] [%d:%d] [%d:%d] [%d:%d] [%d:%d] [%d:%d] [%d]",
	lpMiniPet->m_strName,
	lpMiniPet->m_bf7Level,	lpMiniPet->m_bf16Exp,	lpMiniPet->m_bf5Type,
	lpMiniPet->m_bf5ExpireYear+2000,lpMiniPet->m_bf4ExpireMonth,lpMiniPet->m_bf5ExpireDay,lpMiniPet->m_bf5ExpireHour,lpMiniPet->m_bf6ExpireMinute,
	lpMiniPet->m_iVitalPoint,lpMiniPet->m_iEnergyPoint,lpMiniPet->m_iMineralPoint,
	lpMiniPet->m_aAbility[0].m_bf11Skill,lpMiniPet->m_aAbility[0].m_bf4Level,
	lpMiniPet->m_aAbility[1].m_bf11Skill,lpMiniPet->m_aAbility[1].m_bf4Level,
	lpMiniPet->m_aAbility[2].m_bf11Skill,lpMiniPet->m_aAbility[2].m_bf4Level,
	lpMiniPet->m_aAbility[3].m_bf11Skill,lpMiniPet->m_aAbility[3].m_bf4Level,
	lpMiniPet->m_aAbility[4].m_bf11Skill,lpMiniPet->m_aAbility[4].m_bf4Level,
	lpMiniPet->m_aAbility[5].m_bf11Skill,lpMiniPet->m_aAbility[5].m_bf4Level,
	lpMiniPet->m_aAbility[6].m_bf11Skill,lpMiniPet->m_aAbility[6].m_bf4Level,
	lpMiniPet->m_aAbility[7].m_bf11Skill,lpMiniPet->m_aAbility[7].m_bf4Level,
	lpMiniPet->m_aAbility[8].m_bf11Skill,lpMiniPet->m_aAbility[8].m_bf4Level,
	lpMiniPet->m_aAbility[9].m_bf11Skill,lpMiniPet->m_aAbility[9].m_bf4Level,
	lpMiniPet->m_aAbility[10].m_bf11Skill,lpMiniPet->m_aAbility[10].m_bf4Level,
	lpMiniPet->m_bf7SkillPoint);

	SetDlgItemText(IDC_CODE,strCode);
}
