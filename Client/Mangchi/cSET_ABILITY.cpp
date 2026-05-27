#include	"cSET.H"
#include	"cSKILL.H"
#include	"cJOB.H"
#include	"resource.H"


cABILITY		l_aTempAbility[dABILITY_COUNT];
cABILITY		*cSET_ABILITY::s_lpAbility		=	NULL;
int				cSET_ABILITY::s_iBaseJob		=	0xffff;
cLISTCONTROL	cSET_ABILITY::s_AbilityList;
cLISTCONTROL	cSET_ABILITY::s_SkillList;
int				cSET_ABILITY::s_iSelectAbility	=	0xffff;

BOOL
cSET_ABILITY::SetAbility(HINSTANCE hinst,HWND hwnd,int basejob,cABILITY *lpAbility)
{
	if (basejob	==	0xffff)	return	FALSE;

	s_lpAbility	=	lpAbility;
	s_iBaseJob	=	basejob;

	memcpy(l_aTempAbility,s_lpAbility,sizeof(cABILITY)*25);

	DialogBox( hinst, "SET_ABILITY",hwnd,(DLGPROC)s_procSetAbility);

	return	TRUE;
}

BOOL
cSET_ABILITY::InitSetAbility(HWND hwnd)
{
	HWND	hSkillList	=	GetDlgItem(hwnd,IDC_SKILL_LIST);
	HWND	hAbilityList=	GetDlgItem(hwnd,IDC_ABILITY_LIST);

	s_SkillList.Init		(hSkillList				,2		,TRUE);		//	2개의 컬럼
	s_SkillList.AddColumn	(dMSG_NAME				,6*34);
	s_SkillList.AddColumn	(dMSG_KIND				,6*12);

	s_AbilityList.Init		(hAbilityList			,3		,TRUE,ProcAbilityListWnd);		//	2개의 컬럼
	s_AbilityList.AddColumn	(dMSG_NAME				,6*34	);
	s_AbilityList.AddColumn	(dMSG_KIND				,6*12	);
	s_AbilityList.AddColumn	(dMSG_LEVEL				,6*6	);

	InitSkillList();
	InitAbilityList();

	DrawList(hwnd);

	return	TRUE;
}

//////////////////////////////////////////////////////////////
//	리스트를 그려준다.
//////////////////////////////////////////////////////////////


void
cSET_ABILITY::DrawList(HWND hDlg)
{
	s_SkillList.Draw();
	s_AbilityList.Draw();
}	//	cSET_ABILITY::DrawList(HWND hDlg)


void
cSET_ABILITY::InitSkillList()
{
	int	i;

	for (i=0;i<cSKILL::s_iSkillCount;i++)
		if (g_aSkill[i].m_wEnableJob	==	s_iBaseJob)
		{
			s_SkillList.AddItem(0,g_aSkill[i].m_strName	,i);
			s_SkillList.AddItem(1,dMSG_EXCLUSIVE_SKILL);

			s_SkillList.IncreaseLineCounter();
		}

	for (i=0;i<cSKILL::s_iSkillCount;i++)
		if (g_aSkill[i].m_wEnableJob	==	0xffff)
		{
			s_SkillList.AddItem(0,g_aSkill[i].m_strName	,i);
			s_SkillList.AddItem(1,dMSG_COMMON_SKILL);

			s_SkillList.IncreaseLineCounter();
		}
}

void
cSET_ABILITY::InitAbilityList()
{
	int		i;

	s_AbilityList.ResetItems();

	for (i=0;i<25;i++)
	{
		int	iSkill	=	l_aTempAbility[i].m_wSkill;

		if (iSkill	!=	0xffff)
		{
			if (g_aSkill[iSkill].m_wEnableJob	==	s_iBaseJob)
			{
				s_AbilityList.AddItem(0,g_aSkill[iSkill].m_strName	,iSkill);
				s_AbilityList.AddItem(1,dMSG_EXCLUSIVE_SKILL);
				s_AbilityList.AddItem(2,_ms("%d",l_aTempAbility[i].m_sLevel));
				s_AbilityList.IncreaseLineCounter();
			}
		}
	}

	for (i=0;i<25;i++)
	{
		int	iSkill	=	l_aTempAbility[i].m_wSkill;

		if (iSkill	!=	0xffff)
		{
			if (g_aSkill[iSkill].m_wEnableJob	==	0xffff)
			{
				s_AbilityList.AddItem(0,g_aSkill[iSkill].m_strName	,iSkill);
				s_AbilityList.AddItem(1,dMSG_COMMON_SKILL);
				s_AbilityList.AddItem(2,_ms("%d",l_aTempAbility[i].m_sLevel));
				s_AbilityList.IncreaseLineCounter();
			}
		}
	}
}

void
cSET_ABILITY::AddAbility(HWND hwnd)
{
	int	i,j;

	for (i=0;i<s_SkillList.m_iLineCounter;i++)
	{
		if (s_SkillList.m_aSelected[i])
		{
			int	iSkill	=	s_SkillList.GetData(i);
			int	iSlot	=	0xffff;

			for (j=0;j<25;j++)
			{
				if (l_aTempAbility[j].m_wSkill	==	iSkill							)	iSkill	=	0xffff;
				if (l_aTempAbility[j].m_wSkill	==	0xffff	&&	iSlot	==	0xffff	)	iSlot	=	j;
			}

			if (iSkill	==	0xffff)	continue;
			if (iSlot	==	0xffff)	continue;

			l_aTempAbility[iSlot].m_wSkill	=	iSkill;
			l_aTempAbility[iSlot].m_sLevel	=	1;
		}
	}

	InitAbilityList();

	s_AbilityList.Draw();
}

void
cSET_ABILITY::RemoveAbility(HWND hwnd)
{
	int	i,j;

	for (i=0;i<s_AbilityList.m_iLineCounter;i++)
	{
		if (s_AbilityList.m_aSelected[i])
		{
			int	iSkill	=	s_AbilityList.GetData(i);

			for (j=0;j<25;j++)
				if (l_aTempAbility[j].m_wSkill	==	iSkill)
					l_aTempAbility[j].m_wSkill	=	0xffff;
		}
	}

	InitAbilityList();

	s_AbilityList.Draw();
}

void
cSET_ABILITY::ArrangementAbility()
{
	int	iMinIndexSkill;
	int	i,j,iAbilitySlot,iLastSkill	=	-1,iAbilityCount=0;

	memset(s_lpAbility,0xff,sizeof(cABILITY)*25);

	for (i=0;i<25;i++)	if (l_aTempAbility[i].m_wSkill	!=	0xffff)	iAbilityCount++;

	for (i=0;i<iAbilityCount;i++)
	{
		iAbilitySlot	=	0xfff;
		iMinIndexSkill	=	cSKILL::s_iSkillCount;

		for (j=0;j<25;j++)
		{
			int	iSkill	=	l_aTempAbility[j].m_wSkill;

			if (iSkill	<=	iMinIndexSkill	&&	iSkill	>	iLastSkill)
			{
				iMinIndexSkill	=	iSkill;
				iAbilitySlot	=	j;
			}
		}

		if (iAbilitySlot	==	0xffff)	break;

		iLastSkill	=	iMinIndexSkill;

		memcpy(&s_lpAbility[i],&l_aTempAbility[iAbilitySlot],sizeof(cABILITY));
	}
}

LRESULT CALLBACK
cSET_ABILITY::s_procSetAbility(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{	case WM_INITDIALOG	:
			InitSetAbility(hwnd);
			break;

		case WM_CLOSE		:
			EndDialog(hwnd,IDCANCEL);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {
				case	IDOK				:
				{
					ArrangementAbility();
					EndDialog(hwnd,IDOK);
					break;
				}

				case	IDCANCEL			:
				{
					EndDialog(hwnd,IDCANCEL);
					break;
				}

				case	IDC_ADD_ABILITY		:
				{
					AddAbility(hwnd);
					break;
				}

				case	IDC_REMOVE_ABILITY	:
				{
					RemoveAbility(hwnd);
					break;
				}
			}
			break;
	}

	return (0L);
}


//
//	어빌러티 리스트 윈도우의 예외 처리를 위한 프로콜
//
#define	IDM_SET_ABILITY_DATA	1000

LRESULT CALLBACK 
cSET_ABILITY::ProcAbilityListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_RBUTTONDOWN		:
		{
			if(s_AbilityList.Select(LOWORD(lParam),HIWORD(lParam)))
			{
				int	iSkill			=	s_AbilityList.GetSelectData();

				if (iSkill			==	0xffff)	break;

				s_iSelectAbility	=	0xffff;

				for (int i=0;i<25;i++)
					if (l_aTempAbility[i].m_wSkill	==	iSkill)
						s_iSelectAbility	=	i;

				if (s_iSelectAbility==	0xffff)	break;

				POINT	Pos;
				HMENU	hMenu	=	CreatePopupMenu();

				AppendMenu(hMenu,MFT_STRING,IDM_SET_ABILITY_DATA,dMSG_EDIT);

				GetCursorPos(&Pos);
				TrackPopupMenu(hMenu,TPM_RIGHTBUTTON|TPM_TOPALIGN|TPM_LEFTALIGN,Pos.x,Pos.y,0,hwnd,NULL);
				DestroyMenu(hMenu);
			}

			break;
		}

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {
				case	IDM_SET_ABILITY_DATA	:
				{
					if (s_iSelectAbility!=	0xffff)	SetAbilityData(hwnd);
					break;
				}

			}
			break;


		case WM_LBUTTONDBLCLK	:
		{
			if(s_AbilityList.Select(LOWORD(lParam),HIWORD(lParam)) == 1)
			{
				int	iSkill			=	s_AbilityList.GetSelectData();

				if (iSkill			==	0xffff)	break;

				s_iSelectAbility	=	0xffff;

				for (int i=0;i<25;i++)
					if (l_aTempAbility[i].m_wSkill	==	iSkill)
						s_iSelectAbility	=	i;

				if (s_iSelectAbility!=	0xffff)	SetAbilityData(hwnd);
			}
			break;
		}

		case WM_VSCROLL			:
		{
			if (s_AbilityList.VScroll(wParam))	DrawList(hwnd);
			break;
		}

		default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}




////////////////////////////////////////////////////////////////////////////////
//	어빌러티 레벨과 경험치 설정
////////////////////////////////////////////////////////////////////////////////

void
cSET_ABILITY::SetAbilityData(HWND hwnd)
{
	DialogBox( s_hInst, "SET_ABILITY_DATA",hwnd,(DLGPROC)s_procSetAbilityData);
}

LRESULT CALLBACK
cSET_ABILITY::s_procSetAbilityData(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{	case WM_INITDIALOG	:
		{
			cABILITY	*lpAbility	=	&l_aTempAbility[s_iSelectAbility];

			SetDlgItemInt(hwnd,IDC_LEVEL,lpAbility->m_sLevel,FALSE);
			break;
		}

		case WM_CLOSE		:
			EndDialog(hwnd,IDCANCEL);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {
				case	IDOK				:
				{
					BOOL		temp;

					cABILITY	*lpAbility	=	&l_aTempAbility[s_iSelectAbility];

					int		iLevel		=	GetDlgItemInt(hwnd,IDC_LEVEL,&temp	,FALSE);
					DWORD	dwExp		=	GetDlgItemInt(hwnd,IDC_EXP	,&temp	,FALSE);

					lpAbility->m_sLevel	=	iLevel;

					s_AbilityList.SetSelectItemStr(2,_ms("%d",iLevel));
					DrawList(hwnd);

					EndDialog(hwnd,IDOK);
					break;
				}

				case	IDCANCEL			:
				{
					EndDialog(hwnd,IDCANCEL);
					break;
				}
			}
			break;
	}

	return (0L);
}
