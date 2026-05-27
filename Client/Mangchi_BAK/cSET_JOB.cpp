//
//	몬스터에서 떨어지는 아이템?
//

#include	"cSET.H"
#include	"cSKILL.H"
#include	"cJOB.H"
#include	"cDRAW.H"
#include	"cTEXT.H"
#include	"cMAIN.H"
#include	"cITEM_PREFIX_EFFECT.H"
#include	"resource.H"
#include	"rebirth_define.H"

BOOL		cSET_JOB::s_isEditJob	=	FALSE;
BOOL		cSET_JOB::s_isFailedWork=	FALSE;
cJOB		cSET_JOB::s_makeJob;

int			cSET_JOB::s_iBodyPerWindow		=	0;		//	한번에 나올 수 있는 몸뚱아리 수
int			cSET_JOB::s_iBodyWidth			=	58;
int			cSET_JOB::s_iBodyHeight			=	76;
int			cSET_JOB::s_iJobGroup			=	0;
int			cSET_JOB::s_iSelectJob1Index	=	0;		//	선택한 직업 1의 몸뚱아리
int			cSET_JOB::s_iSelectJob1Body		=	0;		//	선택한 직업 1의 몸뚱아리
int			cSET_JOB::s_iSelectSkillCategory=	dJOB_CODE_MONSTER;
int			cSET_JOB::s_iSelectSkill		=	0;

cLISTCONTROL	cSET_JOB::s_listAbility;
cLISTCONTROL	cSET_JOB::s_listSkill;
HWND			cSET_JOB::s_hAbilityListTab	=	NULL;
int				cSET_JOB::s_iSelectJob;

//
//	직업설정하기 대화상자 초기화
cJOB*
cSET_JOB::NewJob(HWND hwnd,int _iJobGroup,int _iBody,int _iEditJob)
{
	s_isFailedWork			=	FALSE;
	s_iJobGroup				=	_iJobGroup;
	s_iSelectJob1Index		=	_iBody;

	if	(s_iSelectJob1Index	>=	201)
		s_iSelectJob1Body		=	g_BodyList.getBody(s_iSelectJob1Index-201);
	else
		s_iSelectJob1Body		=	g_BodyList.getBody(s_iSelectJob1Index);

	if (_iEditJob	!=	0xffff)
	{
		s_makeJob.copy(g_denJob.getJob(_iEditJob));
		s_isEditJob	=	TRUE;
	}
	else
	{
		s_makeJob.reset();
		s_isEditJob	=	FALSE;
	}


	PROPSHEETPAGE   psp;
	PROPSHEETHEADER psh;
	HPROPSHEETPAGE  hpsp[5];

	psp.dwSize		= sizeof( PROPSHEETPAGE );
	psp.dwFlags		= PSP_DEFAULT;
	psp.hInstance	= s_hInst;

	psp.pszTemplate = "JOB_BASIC_DATA";
	psp.pfnDlgProc  = (DLGPROC)s_procSetJobData;
	hpsp[0]			= CreatePropertySheetPage( &psp );

	psp.pszTemplate = "JOB_DEATH_DATA";
	psp.pfnDlgProc  = (DLGPROC)s_procDeathData;
	hpsp[1]			= CreatePropertySheetPage( &psp );

	memset( &psh, 0, sizeof( PROPSHEETHEADER ) );

	switch(s_iJobGroup)
	{
		case	dJOB_GROUP_MONSTER	:
			psh.pszCaption = dMSG_SET_MONSTER_JOB;
			break;

		case	dJOB_GROUP_NPC		:
			psh.pszCaption = dMSG_SET_NPC_JOB;
			break;
	}


	psh.dwSize     = sizeof( PROPSHEETHEADER );
	psh.dwFlags    = PSH_USEICONID | PSH_NOAPPLYNOW;
	psh.hInstance  = s_hInst;
	psh.hwndParent = hwnd;
	psh.pszIcon    = "SMALL";
	psh.phpage     = hpsp;
	psh.nPages     = 2;

	PropertySheet( &psh );

	if	(s_isFailedWork)
		return	NULL;
/*
	for (int i=0;i<dMAX_MONSTER_ABILITY_COUNT;i++)
	{
		int	iSkill	=	s_makeJob.m_aAbility[i].m_wSkill;

		if (iSkill	!=	0xffff)
		{
			if (g_aSkill[iSkill].m_wEnableJob	!=	s_makeJob.m_wBody	&&
				g_aSkill[i].m_wEnableJob		!=	0xffff)

				s_makeJob.m_aAbility[i].reset();
		}
	}
*/
	return	&s_makeJob;
//	DialogBox( s_hInst,"HERO_ADD",hwnd,(DLGPROC)s_procSetJobData);
}	//	cSET_JOB::Init(HWND hwnd,BOOL editJob)

//
//	어빌러티 리스트 업데이트
void
cSET_JOB::UpdateAbilityList(HWND _hDlg)
{
	s_listAbility.ResetItems();

	for (int i = 0; i<dMONSTER_ABILITY_COUNT;i++)
	{
		cABILITY	*lpAbility	=	(cABILITY*)&s_makeJob.m_aAbility[i];

		if	(lpAbility->m_wSkill	==	0xffff)
			continue;

		cSKILL		*lpSkill	=	&g_aSkill[lpAbility->m_wSkill];

		s_listAbility.AddItem(0,lpSkill->m_strName,i);
		s_listAbility.AddItem(1,_ms("%d+Lv/%d.%.2d",lpAbility->m_sLevel,s_makeJob.m_aLevelForAddSkillLevel[i]/100,s_makeJob.m_aLevelForAddSkillLevel[i]%100));

		s_listAbility.IncreaseLineCounter();
	}

	s_listAbility.Select(0);

	s_listAbility.Draw();

}	//	cSET_JOB::UpdateAbilityList(HWND _hDlg)

//
//	선택된 기술을 어빌러티에 추가
void
cSET_JOB::AddAbility(HWND hwnd)
{
	int		iIndex	=	-1;

	for (int i = 0; i<dMONSTER_ABILITY_COUNT;i++)
	{
		cABILITY	*lpAbility	=	(cABILITY*)&s_makeJob.m_aAbility[i];

		if (lpAbility->m_wSkill	==	0xffff)
		{
			iIndex	=	i;
			break;
		}
	}

	if (iIndex	==	-1)	return;

	BOOL	temp;
	HWND	hSkillList		=	GetDlgItem(hwnd,IDC_SKILL);
	HWND	hAbilityList	=	GetDlgItem(hwnd,IDC_ABILITY_LIST);

	int	iSkill			=	cCOMBOBOX::GetData(hSkillList);
	int	iLevel			=	GetDlgItemInt(hwnd,IDC_SKILL_LEVEL,&temp,FALSE),iLevelPerLevel;

	{
		char	strText[65];
		GetDlgItemText(hwnd,IDC_SKILL_LEVEL_PER_LEVEL,strText,65);
		iLevelPerLevel	=	StringToNumber(strText,655,2);
	}

	s_makeJob.m_aAbility[iIndex].m_wSkill	=	iSkill;
	s_makeJob.m_aAbility[iIndex].m_wLevel	=	iLevel;
	s_makeJob.m_aLevelForAddSkillLevel[i]	=	iLevelPerLevel;

	s_listAbility.AddItem(0,g_aSkill[iSkill].m_strName,iIndex);
	s_listAbility.AddItem(1,_ms("%d+Lv/%d.%.2d",iLevel,iLevelPerLevel/100,iLevelPerLevel%100));
	s_listAbility.IncreaseLineCounter();

	s_listAbility.Select(iIndex);

	DrawAbilityList(hwnd);
}	//	cSET_JOB::AddAbility(HWND hwnd)

//
//	선택된 기술을 어빌러티에 추가
void
cSET_JOB::RemoveAbility(HWND hwnd)
{
	int	i;

	for (i=0;i<s_listAbility.m_iLineCounter;i++)
	{
		if	(s_listAbility.m_aSelected[i])
		{
			int	iAbility	=	s_listAbility.GetData(i);

			s_makeJob.m_aAbility[iAbility].m_wSkill	=	0xffff;
		}
	}

	UpdateAbilityList(hwnd);
}	//	cSET_JOB::RemoveAbility(HWND hwnd)

// 09.12.09 추가
// 마법공격체크
int
cSET_JOB::GetMonsterMagicConcept(cJOB *_lpEditJob)
{
	int iMagicGrade;
	
	// 몬스터 스킬 체크
	int icount = 0;
	for(int i = 0; i < dMONSTER_ABILITY_COUNT; ++i)
	{
		cABILITY	*lpAbility	=	(cABILITY*)&_lpEditJob->m_aAbility[i];
		if	(lpAbility->m_wSkill	==	0xffff)
				continue;
				
		icount++;
	}

	// 마법 능력 체크
	if(icount == 0)
		iMagicGrade = dGRADE_MONSTER_WEAK;
	else if(icount == 1)
	{
		cABILITY	*lpAbility	=	(cABILITY*)&_lpEditJob->m_aAbility[0];
		cSKILL		*lpSkill	=	&g_aSkill[lpAbility->m_wSkill];

		if(lpSkill->m_wDamageAttribute % 2 == 0)
			iMagicGrade = dGRADE_MONSTER_HARD;
		else if(lpSkill->m_wDamageAttribute == 1)
			iMagicGrade = dGRADE_MONSTER_WEAK;
		else
			iMagicGrade = dGRADE_MONSTER_NORMAL;
	}
	else 
	{
		cABILITY	*lpAbility	=	(cABILITY*)&_lpEditJob->m_aAbility[0];
		cSKILL		*lpSkill	=	&g_aSkill[lpAbility->m_wSkill];
		
		// 마법
		if(lpSkill->m_wDamageAttribute % 2 == 0)
			iMagicGrade = dGRADE_MONSTER_HARD;	
		else
		{
			BOOL bTemp = FALSE;

			for(int i = 1; i < dMONSTER_ABILITY_COUNT; ++i)
			{
				cABILITY	*lpAbility2	=	(cABILITY*)&_lpEditJob->m_aAbility[i];

				if	(lpAbility2->m_wSkill	==	0xffff)
					continue;
				
				cSKILL		*lpSkill2	=	&g_aSkill[lpAbility2->m_wSkill];

				if(lpSkill2->m_wDamageAttribute % 2 == 0)
				{
					bTemp = TRUE;
					break;
				}
			}
			
			// 물리
			if(lpSkill->m_wDamageAttribute == 1)
				bTemp == TRUE ? iMagicGrade = dGRADE_MONSTER_NORMAL : iMagicGrade = dGRADE_MONSTER_WEAK;
			else	// 마법 + 물리
				bTemp == TRUE ? iMagicGrade = dGRADE_MONSTER_HARD : iMagicGrade = dGRADE_MONSTER_NORMAL;
		}
	}
	
	return iMagicGrade;
}

// 09.12.09 추가
// 물리공격체크
int
cSET_JOB::GetMonsterPhysicalAttackConcpet(cJOB *_lpEditJob)
{
	int AttackGrade;
	int iLevel;
	
	double C59	= 800;
	double C65	= _lpEditJob->m_sMinDamage;
	double C66	= _lpEditJob->m_wMinDamageLevelFactor * 0.01;
	double C67	= _lpEditJob->m_sMaxDamage;
	double C68	= _lpEditJob->m_wMaxDamageLevelFactor * 0.01;
	double C69	= _lpEditJob->m_sStrength;
	double C70	= _lpEditJob->m_wBasicPowerLevelFactor * 0.001;
			
	double result1 = ((((C65*100)+(C59-1)*(C66*100))+((C65*100)+(C59-1)*(C66*100))*C69*((C59-1)*(C70*1000)+100000)/100000/200)/100);
	double result2 = ((((C67*100)+(C59-1)*(C68*100))+((C67*100)+(C59-1)*(C68*100))*C69*((C59-1)*(C70*1000)+100000)/100000/200)/100);

	int Attack = (int)((result1 + result2)/2);

	// 몬스터 등급 체크
	if(_lpEditJob->m_wMonsterLevel == 4 || _lpEditJob->m_wMonsterLevel == 5)
		iLevel = 4;
	else if(_lpEditJob->m_wMonsterLevel >= 6) 
		iLevel = 5;
	else 
		iLevel = _lpEditJob->m_wMonsterLevel;

	if(iLevel == 0 || iLevel == 1)
	{
		if(Attack < 600)
			AttackGrade = dGRADE_MONSTER_WEAK;
		else if(Attack > 900)
			AttackGrade = dGRADE_MONSTER_HARD;
		else
			AttackGrade = dGRADE_MONSTER_NORMAL;
	}
	else if(iLevel == 2)
	{
		if(Attack < 390)
			AttackGrade = dGRADE_MONSTER_WEAK;
		else if(Attack > 580)
			AttackGrade = dGRADE_MONSTER_HARD;
		else
			AttackGrade = dGRADE_MONSTER_NORMAL;
	}
	else if(iLevel == 3 || iLevel == 4)
	{
		if(Attack < 450)
			AttackGrade = dGRADE_MONSTER_WEAK;
		else if(Attack > 650)
			AttackGrade = dGRADE_MONSTER_HARD;
		else
			AttackGrade = dGRADE_MONSTER_NORMAL;
	}
	else if(iLevel == 5)
	{
		if(Attack < 600)
			AttackGrade = dGRADE_MONSTER_WEAK;
		else if(Attack > 1000)
			AttackGrade = dGRADE_MONSTER_HARD;
		else
			AttackGrade = dGRADE_MONSTER_NORMAL;
	}

	return AttackGrade;
}

// 09.12.09 추가
// 방어력체크
int
cSET_JOB::GetMonsterDefenceConcept(cJOB *_lpEditJob)
{
	int DefenceGrade;
	int iLevel;

	// 몬스터 등급 체크
	if(_lpEditJob->m_wMonsterLevel == 4 || _lpEditJob->m_wMonsterLevel == 5)
		iLevel = 4;

	else if(_lpEditJob->m_wMonsterLevel >= 6) 
		iLevel = 5;
	else 
		iLevel = _lpEditJob->m_wMonsterLevel;

	double C59	= 800;
	double E65	= _lpEditJob->m_sDefensivePower;
	double E66	= _lpEditJob->m_wDefensePowerLevelFactor * 0.01;
	double E67	= _lpEditJob->m_sConstitution;
	double E68	= _lpEditJob->m_wBasicPowerLevelFactor * 0.001;

	double result = ((E65*100)+(C59-1)*(E66*100))*(100+(E67*((C59-1)*(E68*1000)+100000)/100000))/100/100;
	
	if(iLevel == 0)
	{
		if(result < 1950)
			DefenceGrade = dGRADE_MONSTER_WEAK;
		else if(result > 2400)
			DefenceGrade = dGRADE_MONSTER_HARD;
		else
			DefenceGrade = dGRADE_MONSTER_NORMAL;
	}
	else if(iLevel == 1)
	{
		if(result < 2500)
			DefenceGrade = dGRADE_MONSTER_WEAK;
		else if(result > 3500)
			DefenceGrade = dGRADE_MONSTER_HARD;
		else
			DefenceGrade = dGRADE_MONSTER_NORMAL;
	}
	else if(iLevel == 2)
	{
		if(result < 5000)
			DefenceGrade = dGRADE_MONSTER_WEAK;
		else if(result > 6500)
			DefenceGrade = dGRADE_MONSTER_HARD;
		else
			DefenceGrade = dGRADE_MONSTER_NORMAL;
	}
	else if(iLevel == 3 || iLevel == 4)
	{
		if(result < 17000)
			DefenceGrade = dGRADE_MONSTER_WEAK;
		else if(result > 25000)
			DefenceGrade = dGRADE_MONSTER_HARD;
		else
			DefenceGrade = dGRADE_MONSTER_NORMAL;
	}
	else if(iLevel == 5)
	{
		if(result < 30000)
			DefenceGrade = dGRADE_MONSTER_WEAK;
		else if(result > 50000)
			DefenceGrade = dGRADE_MONSTER_HARD;
		else
			DefenceGrade = dGRADE_MONSTER_NORMAL;
	}

	return DefenceGrade;
}


//
//	영웅 추가 대화상자를 초기화 한다.
void
cSET_JOB::InitSetJobData(HWND hDlg)
{
	int		i;

	HWND	hName1			=	GetDlgItem(hDlg,IDC_JOB1_NAME);
	HWND	hComboPalette	=	GetDlgItem(hDlg,IDC_JOB1_BODY_COLOR	);

	//	팔레트
	{
		CPalette	*lpPalettes	=	&g_aMonsterPalette[s_iSelectJob1Body];

		if (lpPalettes->m_wCount)
		{
			for (i=0;i<lpPalettes->m_wCount;i++)
				cCOMBOBOX::Add(hComboPalette,lpPalettes->m_pPalette[i].m_strName,i);
		}

		cCOMBOBOX::SelectByData(hComboPalette,s_makeJob.m_wPalette);
	}


	//	어빌러티
	{
		HWND	hSkillList		=	GetDlgItem(hDlg,IDC_SKILL);
		HWND	hAbilityList	=	GetDlgItem(hDlg,IDC_ABILITY_LIST);

		for (i=0;i<dMAX_SKILL;i++)
		{
			cSKILL	*lpSkill	=	&g_aSkill[i];

			if	(lpSkill->m_wSerial		==	0xffff)
				continue;
			if	(lpSkill->m_wEnableJob	!=	dJOB_CODE_MONSTER	)
				continue;

			cCOMBOBOX::Add(hSkillList,lpSkill->m_strName,i);
		}

		if	(cCOMBOBOX::SelectByData(hSkillList,s_iSelectSkill)	==	CB_ERR)
			cCOMBOBOX::Select(hSkillList,0);

		SetDlgItemInt(hDlg,IDC_SKILL_LEVEL,1,FALSE);
		SetDlgItemText(hDlg,IDC_SKILL_LEVEL_PER_LEVEL,"10.00");

		s_listAbility.Init		(hAbilityList	,2		,FALSE	,s_procList1);	//	2개의 컬럼
		s_listAbility.AddColumn	(dMSG_NAME		,6*32	);
		s_listAbility.AddColumn	(dMSG_LEVEL		,6*18	);

		s_listAbility.Draw();

		UpdateAbilityList(hDlg);
	}

	SetFocus(hName1);

	SetDlgItemText(hDlg,IDC_JOB1_NAME	,s_makeJob.m_strName);
/*
	HWND	hComboAttackPart=	GetDlgItem(hDlg,IDC_ATTACK_PART		);
	//	공격부위 추가
	{
		for (i=0;i<=dATTACK_PART_LOWER;i++)	cCOMBOBOX::Add(hComboAttackPart,g_strAttackPart[i],i);

		cCOMBOBOX::SelectByData(hComboAttackPart,s_makeJob.m_bf4AttackPart);
	}
*/
	//	전투성향
	{
		HWND	hBattleInclination	=	GetDlgItem(hDlg,IDC_BATTLE_INCLINATION);

		for (int i=0;i<4;i++)	cCOMBOBOX::Add(hBattleInclination,g_strBattleInclination[i],i);

		cCOMBOBOX::SelectByData(hBattleInclination,s_makeJob.m_wBattleInclination);
	}	//	전투성향

	//	등급
	{
		HWND	hMonsterGrade		=	GetDlgItem(hDlg,IDC_MONSTER_GRADE);
		
		for (int i=0;;i++)
		{
			if (stricmp(g_strMonsterGrade[i],"end")==0)
				break;
			
			cCOMBOBOX::Add(hMonsterGrade,g_strMonsterGrade[i],i);
		}
		
		cCOMBOBOX::SelectByData(hMonsterGrade,s_makeJob.m_wMonsterLevel);
	}	//	전투성향
	
	//	조련을 위해 필요한 전생 횟수
	{
		HWND	hRebirthCountForTame	=	GetDlgItem(hDlg,IDC_REQUIRE_REBIRTH_COUNT_FOR_TAME);

		for (int i=0;i<=dLIMIT_REBIRTH_COUNT;i++)
		{
			cCOMBOBOX::Add(hRebirthCountForTame,_ms("%d회"),i);
		}

		cCOMBOBOX::SelectByData(hRebirthCountForTame,s_makeJob.m_bf3RequireRebirthCountForTame);
	}	//	전투성향
	
	//	분류
	{
		HWND	hKind				=	GetDlgItem(hDlg,IDC_KIND);

		int		iIndex	=	0;

		while(1)
		{
			if (stricmp(g_strJobCategory[iIndex],"end")==0)	break;

			cCOMBOBOX::Add(hKind,g_strJobCategory[iIndex],iIndex);

			iIndex++;
		}

		cCOMBOBOX::SelectByData(hKind,s_makeJob.m_wKind);
	}	//	분류

	//	성향
	{
		HWND	hKind		=	GetDlgItem(hDlg,IDC_ALLIGNMENT);

		for (i=1;i<6;i++)
			cCOMBOBOX::Add(hKind,g_strAllignment[i],i);

		cCOMBOBOX::SelectByData(hKind,s_makeJob.m_wAllignment);
	}	//	성향

	{
		//	외관
		HWND	hComboShape				=	GetDlgItem(hDlg,IDC_SHAPE);

		for (int iIndex=0;iIndex<4;iIndex++)
			cCOMBOBOX::Add(hComboShape,_ms("%d 단계",iIndex),iIndex);

		cCOMBOBOX::SelectByData(hComboShape,s_makeJob.m_wVariation);
	}

//	데이터를 채운다.

	SetDlgItemInt(hDlg,IDC_LEVEL				,s_makeJob.m_iLevel								,FALSE	);
	SetDlgItemInt(hDlg,IDC_LIMIT_LEVEL			,s_makeJob.m_wLimitLevel						,FALSE	);

    SetDlgItemInt(hDlg,IDC_HP					,s_makeJob.m_iMaxHP/100							,FALSE	);
    SetDlgItemInt(hDlg,IDC_CP					,s_makeJob.m_iMaxCP/100							,FALSE	);
    SetDlgItemInt(hDlg,IDC_SIGHT				,s_makeJob.m_sSight								,FALSE	);

	//	체력 팩터
	{
		char	strHPFactor[20];

		sprintf(strHPFactor,"%d.%.1d",s_makeJob.m_wCorrectMaxHPFactor/10,s_makeJob.m_wCorrectMaxHPFactor%10);
		SetDlgItemText(hDlg,IDC_HP_FACTOR			,strHPFactor);

		sprintf(strHPFactor,"%d.%.1d",s_makeJob.m_wCorrectMaxHPConstitutionFactor/10,s_makeJob.m_wCorrectMaxHPConstitutionFactor%10);
		SetDlgItemText(hDlg,IDC_CON_FACTOR			,strHPFactor);
	}

	//	기본 능력치/팩터
	{
		char	strLevelFactor[20];

		SetDlgItemInt(hDlg,IDC_POWER			,s_makeJob.m_sStrength				,FALSE	);
		SetDlgItemInt(hDlg,IDC_DEXTERITY		,s_makeJob.m_sAgility				,FALSE	);
		SetDlgItemInt(hDlg,IDC_CONSTITUTION		,s_makeJob.m_sConstitution			,FALSE	);
		SetDlgItemInt(hDlg,IDC_CHARISMA			,s_makeJob.m_sCharisma				,FALSE	);
		SetDlgItemInt(hDlg,IDC_INTELIGENCE		,s_makeJob.m_sIntelligence			,FALSE	);
		SetDlgItemInt(hDlg,IDC_WISDOM			,s_makeJob.m_sWisdom				,FALSE	);
		SetDlgItemInt(hDlg,IDC_LUCK				,s_makeJob.m_sLuck					,FALSE	);

		sprintf(strLevelFactor,"%d.%.3d",s_makeJob.m_wBasicPowerLevelFactor/1000,s_makeJob.m_wBasicPowerLevelFactor%1000);
		SetDlgItemText(hDlg,IDC_BASIC_POWER_LEVEL_FACTOR	,strLevelFactor);

		SetDlgItemInt(hDlg,IDC_TAME_RESISTANCE	,s_makeJob.m_bf15TameResistanceValue	,FALSE	);
		CheckDlgButton(hDlg,IDC_ENABLE_TAME		,s_makeJob.m_bf1IsEnableTameMonster);
		CheckDlgButton(hDlg,IDC_ZIN_BOSS		,s_makeJob.m_bf1IsZinBoss);
		CheckDlgButton(hDlg,IDC_REVISE_INT_CALC	,s_makeJob.m_bf1IsReviseIntCalc);
	}	//	기본 능력치/팩터

	//	최소 데미지
	{
		char	strLevelFactor[20];

		sprintf(strLevelFactor,"%d.%.2d",s_makeJob.m_wMinDamageLevelFactor/100,s_makeJob.m_wMinDamageLevelFactor%100);
		SetDlgItemInt(hDlg,IDC_MIN_DAMAGE			,s_makeJob.m_sMinDamage				,FALSE	);
		SetDlgItemText(hDlg,IDC_MIN_DAMAGE_LEVEL_FACTOR,strLevelFactor);
	}	//	최소 데미지

	//	최대 데미지
	{
		char	strLevelFactor[20];

		sprintf(strLevelFactor,"%d.%.2d",s_makeJob.m_wMaxDamageLevelFactor/100,s_makeJob.m_wMaxDamageLevelFactor%100);
		SetDlgItemInt(hDlg,IDC_MAX_DAMAGE			,s_makeJob.m_sMaxDamage				,FALSE	);
		SetDlgItemText(hDlg,IDC_MAX_DAMAGE_LEVEL_FACTOR,strLevelFactor);
	}	//	최대 데미지

	//	방어력
	{
		char	strLevelFactor[20];

		sprintf(strLevelFactor,"%d.%.2d",s_makeJob.m_wDefensePowerLevelFactor/100,s_makeJob.m_wDefensePowerLevelFactor%100);
	    SetDlgItemInt(hDlg,IDC_DEFENSIVE_POWER		,s_makeJob.m_sDefensivePower		,FALSE	);
		SetDlgItemText(hDlg,IDC_DEFENSE_POWER_LEVEL_FACTOR,strLevelFactor);
	}//	방어력

	SetDlgItemInt(hDlg,IDC_CRITICAL_SHOT,s_makeJob.m_wCriticalShot,FALSE);
	SetDlgItemInt(hDlg,IDC_CRUSH_SHOT,s_makeJob.m_wCrushBlow,FALSE);

	//	각종 확률
	{
		char	strText[20];

		if (s_makeJob.m_sCorrectHit<	0)
				sprintf(strText,"-%d.%.2d",-s_makeJob.m_sCorrectHit/100,-s_makeJob.m_sCorrectHit%100);
		else	sprintf(strText,"%d.%.2d",s_makeJob.m_sCorrectHit/100,s_makeJob.m_sCorrectHit%100);
		SetDlgItemText(hDlg,IDC_CORRECT_HIT,strText);

		if (s_makeJob.m_sCorrectDodge<	0)
				sprintf(strText,"-%d.%.2d",-s_makeJob.m_sCorrectDodge/100,-s_makeJob.m_sCorrectDodge%100);
		else	sprintf(strText,"%d.%.2d",s_makeJob.m_sCorrectDodge/100,s_makeJob.m_sCorrectDodge%100);
		SetDlgItemText(hDlg,IDC_CORRECT_DODGE,strText);

	    SetDlgItemInt(hDlg,IDC_BLOCKING	,s_makeJob.m_wBlocking	,TRUE	);
	}//	각종 확률

	//	이동/공격 속도
	{
		SetDlgItemInt(hDlg,IDC_PPS						,s_makeJob.m_wMoveSpeed						,FALSE	);

		char	strText[20];
		sprintf(strText,"%d.%.2d",s_makeJob.m_wAttackSpeed/100,s_makeJob.m_wAttackSpeed%100);
		SetDlgItemText(hDlg,IDC_ACPS,strText);
	}	//	이동/공격 속도

	// 09.12.09 추가   몬스터 컨셉
	{		
		char	strText[20];

		int iMagicGrade = 0, iPhysical = 0, iDefence = 0;
		iMagicGrade	= GetMonsterMagicConcept(&s_makeJob);
	
		if(iMagicGrade == dGRADE_MONSTER_HARD)
		{
			// 09.12.14 수정
			cABILITY	*lpAbility	=	(cABILITY*)&s_makeJob.m_aAbility[0];
			cSKILL		*lpSkill	=	&g_aSkill[lpAbility->m_wSkill];

			if(lpSkill->m_wDamageAttribute % 2 == 0)
				iPhysical = dGRADE_MONSTER_WEAK;
			else
				iPhysical =GetMonsterPhysicalAttackConcpet(&s_makeJob);
		}
		else
			iPhysical =GetMonsterPhysicalAttackConcpet(&s_makeJob);

		iDefence = GetMonsterDefenceConcept(&s_makeJob);
		sprintf(strText, "%s.%s.%s", g_strMonsterConcept[iMagicGrade+1], g_strMonsterConcept[iPhysical+1], g_strMonsterConcept[iDefence+1]);
		SetDlgItemText(hDlg,IDC_EDIT_MONSTER_CONCEPT,strText);
	}

    SetDlgItemInt(hDlg,IDC_BAD_STATUS_REGISTANCE	,s_makeJob.m_sBadStatusResistance		,TRUE	);
    SetDlgItemInt(hDlg,IDC_DECLINE_POWER_REGISTANCE	,s_makeJob.m_sDeclinePowerResistance	,TRUE	);
	SetDlgItemInt(hDlg,IDC_CURSE_REGISTANCE			,s_makeJob.m_sCurseResistance			,TRUE	);

    SetDlgItemInt(hDlg,IDC_BLIND					,s_makeJob.m_sBlindResistance			,TRUE	);
    SetDlgItemInt(hDlg,IDC_POISON					,s_makeJob.m_sPoisonResistance			,TRUE	);
    SetDlgItemInt(hDlg,IDC_SLEEP					,s_makeJob.m_sSleepResistance			,TRUE	);
    SetDlgItemInt(hDlg,IDC_COLD						,s_makeJob.m_sColdResistance			,TRUE	);
    SetDlgItemInt(hDlg,IDC_FREEZE					,s_makeJob.m_sFreezeResistance			,TRUE	);
    SetDlgItemInt(hDlg,IDC_STUN						,s_makeJob.m_sStunResistance			,TRUE	);
    SetDlgItemInt(hDlg,IDC_STONE					,s_makeJob.m_sStoneResistance			,TRUE	);
    SetDlgItemInt(hDlg,IDC_CONFUSE					,s_makeJob.m_sConfuseResistance			,TRUE	);
    SetDlgItemInt(hDlg,IDC_CHARMING					,s_makeJob.m_sCharmingResistance		,TRUE	);
    SetDlgItemInt(hDlg,IDC_CRITICAL_RESISTANCE		,s_makeJob.m_sCriticalResistance		,TRUE	);
    SetDlgItemInt(hDlg,IDC_CRUSH_RESISTANCE			,s_makeJob.m_sCrushResistance			,TRUE	);

    SetDlgItemInt(hDlg,IDC_FIRE_REGISTANCE			,s_makeJob.m_sFireResistance			,TRUE	);
    SetDlgItemInt(hDlg,IDC_WATER_REGISTANCE			,s_makeJob.m_sWaterResistance			,TRUE	);
    SetDlgItemInt(hDlg,IDC_WIND_REGISTANCE			,s_makeJob.m_sWindResistance			,TRUE	);
    SetDlgItemInt(hDlg,IDC_LIGHT_REGISTANCE			,s_makeJob.m_sLightResistance			,TRUE	);
    SetDlgItemInt(hDlg,IDC_EARTH_REGISTANCE			,s_makeJob.m_sEarthResistance			,TRUE	);
    SetDlgItemInt(hDlg,IDC_DARK_REGISTANCE			,s_makeJob.m_sDarkResistance			,TRUE	);

}	//	cSET_JOB::InitSetJobData(HWND hDlg)

//
//	설정된 데이터를 읽어들인다.
BOOL
cSET_JOB::GetJobData(HWND hDlg)
{
	char	strName[2][32];
	int		iIndex	=	2;
	BOOL	temp;

	GetDlgItemText(hDlg,IDC_JOB1_NAME	,strName[0]	,sizeof(strName[0]));	//	기본 직업 이름
	GetDlgItemText(hDlg,IDC_JOB2_NAME	,strName[1]	,sizeof(strName[1]));	//	두번째 직업 이름

	s_makeJob.m_wBody		=	s_iSelectJob1Body;		//	직업 1의 몸뚱아리
	s_makeJob.m_wType		=	s_iJobGroup;			//	직업 군

////////////////////////////////
//	이름
////////////////////////////////
	{
		if (strcmp(strName[0],"")	==	0)	//	이름이 비었다.
			strcpy(strName[0],g_BodyList.getName(s_iSelectJob1Index));

		//	직업 이름이 중복 되는지 확인하자.
		if (!s_isEditJob)		//	수정 작업이 아니다.
		{
			if (g_denJob.getJob(strName[0])	!=	NULL)
			{
				int		iIndex	=	2;
				char	strTempName[32];

				strName[0][28]	=	0;
				strcpy(strTempName,strName[0]);

				while(1)
				{
					strcpy(strName[0],_ms("%s[%d]",strTempName,iIndex++));

					if (g_denJob.getJob(strName[0])	==	NULL)	break;
				}

				ERRMSG(dMSG_ERROR_NEW_JOB,"%s\n\n\"%s\"",dMSG_ALREADYEXIST_FOLLOW_NAME_JOB,strName[0]);
			}
		}
		else
		{
			if (strcmp(strName[0],s_makeJob.m_strName)	!=	0)	//	원래 있던 이름과 다르다
			{
				if (g_denJob.getJob(strName[0])	!=	NULL)
				{
					int		iIndex	=	2;
					char	strTempName[32];

					strName[0][28]	=	0;
					strcpy(strTempName,strName[0]);

					while(1)
					{
						strcpy(strName[0],_ms("%s[%d]",strTempName,iIndex++));

						if (g_denJob.getJob(strName[0])	==	NULL)	break;
					}

					ERRMSG(dMSG_ERROR_NEW_JOB,"%s\n\n\"%s\"",dMSG_ALREADYEXIST_FOLLOW_NAME_JOB,strName[0]);
				}
			}
		}

		strcpy(s_makeJob.m_strName,strName[0]);
	}	//	이름 설정

	//	팔레트
	{
		HWND	hComboPalette	=	GetDlgItem(hDlg,IDC_JOB1_BODY_COLOR	);

		s_makeJob.m_wPalette	=	cCOMBOBOX::GetData(hComboPalette);
	}

	//	각종 직업 데이터
	{
		char	strText[100];
		int		iValue;

		s_makeJob.m_wBattleInclination		=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_BATTLE_INCLINATION));	//	전투성향
		s_makeJob.m_wKind					=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_KIND));	//	분류
		s_makeJob.m_wAllignment				=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_ALLIGNMENT));	//	성향
		s_makeJob.m_wMonsterLevel			=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_MONSTER_GRADE));	//	전투성향
		s_makeJob.m_wVariation				=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_SHAPE));	//	전투성향
		s_makeJob.m_bf3RequireRebirthCountForTame=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_REQUIRE_REBIRTH_COUNT_FOR_TAME));	//	조련을 위해 필요한 전생 횟수

		switch(s_makeJob.m_wAllignment)
		{
			case	1	:
				s_makeJob.m_sAllignment	=	-100;
				break;
			case	2	:
				s_makeJob.m_sAllignment	=	-50;
				break;
			case	3	:
				s_makeJob.m_sAllignment	=	0;
				break;
			case	4	:
				s_makeJob.m_sAllignment	=	80;
				break;
			case	5	:
				s_makeJob.m_sAllignment	=	100;
				break;
		}


		s_makeJob.m_iLevel					=	GetDlgItemInt(hDlg,IDC_LEVEL					,&temp,FALSE	);
		s_makeJob.m_wLimitLevel				=	GetDlgItemInt(hDlg,IDC_LIMIT_LEVEL				,&temp,FALSE	);
		s_makeJob.m_wLimitLevel				=	max(s_makeJob.m_wLimitLevel,s_makeJob.m_iLevel);

		s_makeJob.m_iMaxHP					=	GetDlgItemInt(hDlg,IDC_HP						,&temp,FALSE	)*100;
		s_makeJob.m_iMaxCP					=	GetDlgItemInt(hDlg,IDC_CP						,&temp,FALSE	)*100;
		s_makeJob.m_sSight					=	GetDlgItemInt(hDlg,IDC_SIGHT					,&temp,FALSE	);

		//	체력 팩터
		{
			GetDlgItemText(hDlg,IDC_HP_FACTOR,strText,sizeof(strText));
			iValue										=	StringToNumber(strText,10000,1);
			s_makeJob.m_wCorrectMaxHPFactor				=	min(iValue,0xffff);

			GetDlgItemText(hDlg,IDC_CON_FACTOR,strText,sizeof(strText));
			iValue										=	StringToNumber(strText,6500,1);
			s_makeJob.m_wCorrectMaxHPConstitutionFactor	=	iValue;
		}

		{
			s_makeJob.m_bf15TameResistanceValue	=	GetDlgItemInt(hDlg,IDC_TAME_RESISTANCE		,&temp,FALSE	);
			s_makeJob.m_bf1IsEnableTameMonster	=	IsDlgButtonChecked(hDlg,IDC_ENABLE_TAME);
			s_makeJob.m_bf1IsZinBoss			=	IsDlgButtonChecked(hDlg,IDC_ZIN_BOSS);
			s_makeJob.m_bf1IsReviseIntCalc		=	IsDlgButtonChecked(hDlg,IDC_REVISE_INT_CALC	);
		}

		//	기본 능력
		{
			s_makeJob.m_sStrength				=	GetDlgItemInt(hDlg,IDC_POWER					,&temp,FALSE	);
			s_makeJob.m_sAgility				=	GetDlgItemInt(hDlg,IDC_DEXTERITY				,&temp,FALSE	);
			s_makeJob.m_sConstitution			=	GetDlgItemInt(hDlg,IDC_CONSTITUTION				,&temp,FALSE	);
			s_makeJob.m_sCharisma				=	GetDlgItemInt(hDlg,IDC_CHARISMA					,&temp,FALSE	);
			s_makeJob.m_sIntelligence			=	GetDlgItemInt(hDlg,IDC_INTELIGENCE				,&temp,FALSE	);
			s_makeJob.m_sWisdom					=	GetDlgItemInt(hDlg,IDC_WISDOM					,&temp,FALSE	);
			s_makeJob.m_sLuck					=	GetDlgItemInt(hDlg,IDC_LUCK						,&temp,FALSE	);

			GetDlgItemText(hDlg,IDC_BASIC_POWER_LEVEL_FACTOR,strText,sizeof(strText));
			iValue								=	StringToNumber(strText,100,3);
			s_makeJob.m_wBasicPowerLevelFactor	=	iValue;
		}	//	기본 능력

		//	최소 데미지
		{
			s_makeJob.m_sMinDamage				=	GetDlgItemInt(hDlg,IDC_MIN_DAMAGE				,&temp,FALSE	);

			GetDlgItemText(hDlg,IDC_MIN_DAMAGE_LEVEL_FACTOR,strText,sizeof(strText));
			iValue								=	StringToNumber(strText,100,2);
			s_makeJob.m_wMinDamageLevelFactor	=	iValue;
		}	//	최소 데미지

		//	최대 데미지
		{
			s_makeJob.m_sMaxDamage				=	GetDlgItemInt(hDlg,IDC_MAX_DAMAGE				,&temp,FALSE	);

			GetDlgItemText(hDlg,IDC_MAX_DAMAGE_LEVEL_FACTOR,strText,sizeof(strText));
			iValue								=	StringToNumber(strText,100,2);
			s_makeJob.m_wMaxDamageLevelFactor	=	iValue;
		}	//	최대 데미지

		//	방어력
		{
			s_makeJob.m_sDefensivePower			=	GetDlgItemInt(hDlg,IDC_DEFENSIVE_POWER			,&temp,FALSE	);

			GetDlgItemText(hDlg,IDC_DEFENSE_POWER_LEVEL_FACTOR,strText,sizeof(strText));
			iValue								=	StringToNumber(strText,100,2);
			s_makeJob.m_wDefensePowerLevelFactor	=	iValue;
		}	//	방어력

		//	이동/공격 속도
		{
			s_makeJob.m_wMoveSpeed				=	GetDlgItemInt(hDlg,IDC_PPS					,&temp,FALSE	);

			GetDlgItemText(hDlg,IDC_ACPS,strText,sizeof(strText));
			iValue								=	StringToNumber(strText,10,2);
			s_makeJob.m_wAttackSpeed=	iValue;
		}	//	이동/공격 속도

		s_makeJob.m_wCriticalShot	=		GetDlgItemInt(hDlg,IDC_CRITICAL_SHOT,&temp,FALSE);
		s_makeJob.m_wCrushBlow		=		GetDlgItemInt(hDlg,IDC_CRUSH_SHOT	,&temp,FALSE);

		//	각종 확률
		{
			GetDlgItemText(hDlg,IDC_CORRECT_HIT,strText,sizeof(strText));
			iValue						=	StringToNumber(strText,100,2);
			s_makeJob.m_sCorrectHit		=	iValue;

			GetDlgItemText(hDlg,IDC_CORRECT_DODGE,strText,sizeof(strText));
			iValue						=	StringToNumber(strText,100,2);
			s_makeJob.m_sCorrectDodge	=	iValue;

			s_makeJob.m_wBlocking		=	GetDlgItemInt(hDlg,IDC_BLOCKING					,&temp,TRUE	);
		}	//	각종 확률

//		s_makeJob.m_bf4AttackPart			=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_ATTACK_PART));
		s_makeJob.m_iHP						=	s_makeJob.m_iMaxHP;

		s_makeJob.m_sBadStatusResistance	=	GetDlgItemInt(hDlg,IDC_BAD_STATUS_REGISTANCE	,&temp,TRUE	);
		s_makeJob.m_sDeclinePowerResistance	=	GetDlgItemInt(hDlg,IDC_DECLINE_POWER_REGISTANCE	,&temp,TRUE	);
		s_makeJob.m_sCurseResistance		=	GetDlgItemInt(hDlg,IDC_CURSE_REGISTANCE			,&temp,TRUE	);

		s_makeJob.m_sBlindResistance		=	GetDlgItemInt(hDlg,IDC_BLIND					,&temp,TRUE	);
		s_makeJob.m_sPoisonResistance		=	GetDlgItemInt(hDlg,IDC_POISON					,&temp,TRUE	);
		s_makeJob.m_sSleepResistance		=	GetDlgItemInt(hDlg,IDC_SLEEP					,&temp,TRUE	);
		s_makeJob.m_sColdResistance			=	GetDlgItemInt(hDlg,IDC_COLD						,&temp,TRUE	);
		s_makeJob.m_sFreezeResistance		=	GetDlgItemInt(hDlg,IDC_FREEZE					,&temp,TRUE	);
		s_makeJob.m_sStunResistance			=	GetDlgItemInt(hDlg,IDC_STUN						,&temp,TRUE	);
		s_makeJob.m_sStoneResistance		=	GetDlgItemInt(hDlg,IDC_STONE					,&temp,TRUE	);
		s_makeJob.m_sConfuseResistance		=	GetDlgItemInt(hDlg,IDC_CONFUSE					,&temp,TRUE	);
		s_makeJob.m_sCharmingResistance		=	GetDlgItemInt(hDlg,IDC_CHARMING					,&temp,TRUE	);
		s_makeJob.m_sCriticalResistance		=	GetDlgItemInt(hDlg,IDC_CRITICAL_RESISTANCE		,&temp,TRUE	);
		s_makeJob.m_sCrushResistance		=	GetDlgItemInt(hDlg,IDC_CRUSH_RESISTANCE			,&temp,TRUE	);

		s_makeJob.m_sFireResistance			=	GetDlgItemInt(hDlg,IDC_FIRE_REGISTANCE			,&temp,TRUE	);
		s_makeJob.m_sWaterResistance		=	GetDlgItemInt(hDlg,IDC_WATER_REGISTANCE			,&temp,TRUE	);
		s_makeJob.m_sWindResistance			=	GetDlgItemInt(hDlg,IDC_WIND_REGISTANCE			,&temp,TRUE	);
		s_makeJob.m_sLightResistance		=	GetDlgItemInt(hDlg,IDC_LIGHT_REGISTANCE			,&temp,TRUE	);
		s_makeJob.m_sEarthResistance		=	GetDlgItemInt(hDlg,IDC_EARTH_REGISTANCE			,&temp,TRUE	);
		s_makeJob.m_sDarkResistance			=	GetDlgItemInt(hDlg,IDC_DARK_REGISTANCE			,&temp,TRUE	);
	}

	return	TRUE;
}	//	cSET_JOB::GetJobData(HWND hDlg)

LRESULT CALLBACK 
cSET_JOB::s_procSetJobData(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{	switch(message)
	{	case WM_INITDIALOG	:
			InitSetJobData(hDlg);
			break;

		case WM_CLOSE		:
			s_isFailedWork	=	TRUE;
			EndDialog(hDlg,IDCANCEL);
			return TRUE;

		case WM_NOTIFY		:
		{
			NMHDR*	notyfyInfo	=	((NMHDR*)lParam);

			if (wParam==0)
			{
				switch(notyfyInfo->code)
				{
					case	PSN_RESET	:
						s_isFailedWork	=	TRUE;
						s_makeJob.reset();
						break;

					case	PSN_APPLY	:
						if (!GetJobData(hDlg))	//	직업 데이터를 얻어 온다.
						{
							s_isFailedWork	=	TRUE;
							s_makeJob.reset();

							return	TRUE;
							break;
						}
						s_isFailedWork	=	FALSE;
						break;
				}
			}
			break;
		}

		case	WM_COMMAND	:
			switch( LOWORD( wParam ) )
            {
				case	IDC_ADD_ABILITY				:
					AddAbility(hDlg);
					break;

				case	IDC_REMOVE_ABILITY			:
					RemoveAbility(hDlg);
					break;
			}
			break;
	}

	return (0L);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	어빌러티 설정
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////



//
//	어빌러티 리스트 초기화
//
void
cSET_JOB::InitJobAbility(HWND hDlg)
{
	HWND	hList1			=	GetDlgItem(hDlg,IDC_ABILITY_LIST1);
	HWND	hcomboLevel		=	GetDlgItem(hDlg,IDC_LEVEL);

	s_hAbilityListTab		=	hDlg;
	s_iSelectJob			=	s_iSelectJob1Body;

	for (int i=0;i<=50;i++)
		cCOMBOBOX::Add(hcomboLevel,_ms("Level %d"),i);

	CheckDlgButton(hDlg,IDC_JOB1,TRUE);

	s_listAbility.Init		(hList1					,4		,TRUE	,s_procList1);		//	2개의 컬럼
	s_listAbility.AddColumn	("No"					,6*4	);
	s_listAbility.AddColumn	(dMSG_NAME				,6*30	);
	s_listAbility.AddColumn	(dMSG_LEVEL				,6*6	);

	InitAbilityList(hDlg);

	DrawAbilityList(hDlg);
}

void
cSET_JOB::InitAbilityList(HWND hDlg)
{
	int		i;

	s_iSelectJob	=	s_iSelectJob1Body;

//	추가 가능 스킬/마법
	cLISTCONTROL	*lpList		=	&s_listAbility;
	cABILITY		*lpAbility	=	(cABILITY		*)s_makeJob.m_aAbility;

	lpList->ResetItems();

//
//	사용 할 수 없는 스킬을 솎아 낸다.
	for (i=0;i<dMAX_MONSTER_ABILITY_COUNT;i++)
	{
		int	iSkill	=	lpAbility[i].m_wSkill;

		if (iSkill	!=	0xffff)
		{
			if (g_aSkill[iSkill].m_wEnableJob	!=	s_iSelectJob1Body	&&
				g_aSkill[i].m_wEnableJob		!=	0xffff)

				lpAbility[i].reset();
		}
	}


	for (i=0;i<dMAX_MONSTER_ABILITY_COUNT;i++)
	{
		int	iSkill	=	lpAbility[i].m_wSkill;

		if (iSkill	!=	0xffff)
		{
			if (g_aSkill[iSkill].m_wEnableJob	==	s_iSelectJob)
			{
				lpList->AddItem(0,_ms("%2d",lpList->m_iLineCounter+1)	,i	);
				lpList->AddItem(1,g_aSkill[iSkill].m_strName				);
				lpList->AddItem(2,dMSG_EXCLUSIVE_SKILL);
				lpList->AddItem(3,_ms("%2d",lpAbility[i].m_sLevel)			);

				lpList->IncreaseLineCounter();
			}
		}
	}

	for (i=0;i<dMAX_MONSTER_ABILITY_COUNT;i++)
	{
		int	iSkill	=	lpAbility[i].m_wSkill;

		if (iSkill	!=	0xffff)
		{
			if (g_aSkill[iSkill].m_wEnableJob	==	0xffff)	
			{
				lpList->AddItem(0,_ms("%2d",lpList->m_iLineCounter+1)	,i	);
				lpList->AddItem(1,g_aSkill[iSkill].m_strName				);
				lpList->AddItem(2,dMSG_COMMON_SKILL);
				lpList->AddItem(3,_ms("%2d",lpAbility[i].m_sLevel)			);

				lpList->IncreaseLineCounter();
			}
		}
	}

//
//	추가 가능 스킬/마법
//
	s_listSkill.ResetItems();

	for (i=0;i<cSKILL::s_iSkillCount;i++)
		if (g_aSkill[i].m_wEnableJob	==	s_iSelectJob)
		{
			if (s_makeJob.getAbilityIndex(i)	==	0xffff)
			{
				s_listSkill.AddItem(0,g_aSkill[i].m_strName	,i);
				s_listSkill.AddItem(1,dMSG_EXCLUSIVE_SKILL);

				s_listSkill.IncreaseLineCounter();
			}
		}

	for (i=0;i<cSKILL::s_iSkillCount;i++)
		if (g_aSkill[i].m_wEnableJob	==	0xffff)
		{
			if (s_makeJob.getAbilityIndex(i)	==	0xffff)
			{
				s_listSkill.AddItem(0,g_aSkill[i].m_strName	,i);
				s_listSkill.AddItem(1,dMSG_COMMON_SKILL);

				s_listSkill.IncreaseLineCounter();
			}
		}
}

void
cSET_JOB::DrawAbilityList(HWND hwnd)
{
	HWND	hcomboLevel	=	GetDlgItem(hwnd,IDC_LEVEL);

	if (s_listAbility.m_iSelect	!=	0xffff)
	{
		int	iAbility	=	s_listAbility.GetData(s_listAbility.m_iSelect);
		int	iLevel		=	s_makeJob.m_aAbility[iAbility].m_wLevel;

		cCOMBOBOX::SelectByData(hcomboLevel,iLevel);

		ShowWindow(hcomboLevel,SW_SHOW);
	}
	else
	{
		ShowWindow(hcomboLevel,SW_HIDE);
	}

//	MSGOUT("%d",s_listAbility.m_iSelect);

	s_listSkill.Draw();

	s_listAbility.Draw();
}


LRESULT CALLBACK
cSET_JOB::s_procAbilityList(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{	case WM_INITDIALOG	:
			InitJobAbility(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDOK);
			break;

		case WM_NOTIFY		:
		{
			NMHDR*	notyfyInfo	=	((NMHDR*)lParam);

			if (wParam==0)
			{
				switch(notyfyInfo->code)
				{
					case	PSN_SETACTIVE	:
					{
//						if (s_iSelectJob2Body	==	0xffff)
//						{
//							if (IsDlgButtonChecked(hDlg,IDC_JOB2))
//							{
								InitAbilityList(hDlg);
								DrawAbilityList(hDlg);
//							}
//						}	else	EnableWindow(GetDlgItem(hDlg,IDC_JOB2),TRUE);
						break;
					}
				}
			}
			break;
		}

		case WM_COMMAND		:
			switch( LOWORD( wParam ) )
            {
				case	IDC_ADD_ABILITY				:
					AddAbility(hDlg);
					break;

				case	IDC_REMOVE_ABILITY			:
					RemoveAbility(hDlg);
					break;

				case	IDC_LEVEL					:
				{
					switch (HIWORD(wParam))
                    {	case	CBN_SELCHANGE		:
						{
							int	iAbility	=	s_listAbility.GetSelectData();
							int	iLevel		=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_LEVEL));

							s_listAbility.SetSelectItemStr(3,_ms("%2d",iLevel));
							s_makeJob.m_aAbility[iAbility].m_wLevel	=	iLevel;

							DrawAbilityList(hDlg);
							break;
						}
					}
					break;
				}
			}
			break;
	}

	return (0L);
}


LRESULT CALLBACK
cSET_JOB::s_procList1(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN		:
		{
			int	iAbility=	s_listAbility.GetSelectData();

			if (iAbility==	0x7fffffff)	break;

			HWND	hcomboLevel	=	GetDlgItem(s_hAbilityListTab,IDC_LEVEL);

			ShowWindow(hcomboLevel,SW_SHOW);

			if (s_listAbility.m_iSelect	!=	0xffff)
			{
				int	iAbility	=	s_listAbility.GetData(s_listAbility.m_iSelect);
				int	iLevel		=	s_makeJob.m_aAbility[iAbility].m_wLevel;

				cCOMBOBOX::SelectByData(hcomboLevel,iLevel);
			}

			break;
		}

		default					:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}


LRESULT CALLBACK
cSET_JOB::s_procList2(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN		:
		{
			int	iSkill	=	s_listSkill.GetSelectData();

			if (iSkill	==	0xffff)	break;

			int	iAbility=	s_makeJob.getAbilityIndex(iSkill);

			if (iAbility==	0xffff)	break;

			break;
		}

		default					:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}


/////////////////////////////////////////////////////////////////////////////////////////
//	죽었을때 데이터 설정
/////////////////////////////////////////////////////////////////////////////////////////
cLISTCONTROL		cSET_JOB::s_listDropItemList;	//	드롭 아이템 리스트
cLISTCONTROL		cSET_JOB::s_listAddItemList;	//	추가 가능 아이템 리스트
HWND				cSET_JOB::s_hDeathDataDlg	=	NULL;

BOOL
cSET_JOB::InitDeathData(HWND hDlg)
{
	s_hDeathDataDlg	=	hDlg;

	HWND	hDropItemList,hAddItemList;

	hDropItemList	=	GetDlgItem(hDlg,IDC_DROP_ITEM_LIST);	//	떨어뜨리는 아이템 리스트
	hAddItemList	=	GetDlgItem(hDlg,IDC_ADD_ITEM_LIST);		//	추가 가능한 아이템 리스트

	//	바닥에 딸어지는 아이템 데이터/리스트
	{
		SetDlgItemText(hDlg,IDC_GOLD,_ms("%d.%.2d",s_makeJob.m_wGoldPerLevel/100,s_makeJob.m_wGoldPerLevel%100));
		SetDlgItemInt(hDlg,IDC_BASIC_GOLD,s_makeJob.m_wGold,FALSE);
		SetDlgItemText(hDlg,IDC_DEATH_EXP,_ms("%d.%.2d",s_makeJob.m_dwDeathExp/100,s_makeJob.m_dwDeathExp%100));
		SetDlgItemInt(hDlg,IDC_GOLD_DROP_RATE,s_makeJob.m_wMoneyDroppingRate,FALSE);

		s_listDropItemList.Init		(hDropItemList	,3		,FALSE	,s_procDropItemList);		//	2개의 컬럼
		s_listDropItemList.AddColumn(dMSG_NAME		,6*22	);
		s_listDropItemList.AddColumn(dMSG_FREQUENCY	,6*14	);
		s_listDropItemList.AddColumn(dMSG_COUNT		,6*7	);

		UpdateDropItemList(hDlg);
	}
	
	//	바닥에 딸어지는 아이템 데이터
	{
		s_listAddItemList.Init		(hAddItemList	,1		,FALSE	);	//	2개의 컬럼
		s_listAddItemList.AddColumn	(dMSG_NAME		,6*30	);
		s_listAddItemList.HideColumn();

		UpdateAddItemList(hDlg);
	}

	DrawDeathData(hDlg);
	return	TRUE;
}	//	cSET_JOB::InitDeathData(HWND hDlg)

void
cSET_JOB::DrawDeathData(HWND hDlg)
{
	hDlg	=	s_hDeathDataDlg;

	int	iSelect	=	s_listDropItemList.m_iSelect;

	if (iSelect	!=	0xffff)
	{
		EnableWindow(GetDlgItem(hDlg,IDC_DROP_RATE),TRUE);
		EnableWindow(GetDlgItem(hDlg,IDC_ITEM_COUNT),TRUE);
		EnableWindow(GetDlgItem(hDlg,IDC_EXTRA_LEVEL),TRUE);

		int	iValue	=	s_makeJob.m_aDroppingItem[iSelect].m_dwDroppingRate;

//		SetDlgItemInt(hDlg,IDC_DROP_RATE,s_makeJob.m_aDroppingItem[iSelect].m_dwDroppingRate,FALSE);
		SetDlgItemText(hDlg,IDC_DROP_RATE,_ms("%d.%.2d",iValue/100,iValue%100));
		SetDlgItemInt(hDlg,IDC_ITEM_COUNT,s_makeJob.m_aDroppingItem[iSelect].m_bf8Count,FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(hDlg,IDC_DROP_RATE),FALSE);
		EnableWindow(GetDlgItem(hDlg,IDC_ITEM_COUNT),FALSE);
		EnableWindow(GetDlgItem(hDlg,IDC_EXTRA_LEVEL),FALSE);
	}

	s_listDropItemList.Draw();
	s_listAddItemList.Draw();
}	//	cSET_JOB::DrawDeathData(HWND hDlg)

//
//	아이템 리스트 업데이트
void
cSET_JOB::UpdateDropItemList(HWND hDlg,int _iSelect)
{
	s_listDropItemList.ResetItems();

	for (int i = 0; i<dDROPPING_ITEM_COUNT;i++)
	{
		int	iItemType	=	s_makeJob.m_aDroppingItem[i].m_wItemType;

		if (iItemType	!=	0xffff)
		{
			s_listDropItemList.AddItem(0,g_strItemCategory[iItemType],i);

			int	iValue	=	s_makeJob.m_aDroppingItem[i].m_dwDroppingRate;

			s_listDropItemList.AddItem(1,_ms("%d.%.2d％",iValue/100,iValue%100));
			s_listDropItemList.AddItem(2,_ms("%d",s_makeJob.m_aDroppingItem[i].m_bf8Count));
			s_listDropItemList.IncreaseLineCounter();
		}
	}

	s_listDropItemList.Select(_iSelect);

	if (s_listDropItemList.m_iSelect==	0xffff)	
		EnableWindow(GetDlgItem(hDlg,IDC_REMOVE_ITEM),FALSE);
	else
		EnableWindow(GetDlgItem(hDlg,IDC_REMOVE_ITEM),TRUE);
}

//
//	아이템 리스트 업데이트
void
cSET_JOB::UpdateAddItemList(HWND hDlg)
{
	int		iIndex	=	0;

	s_listAddItemList.ResetItems();

	while(1)
	{
		if (strcmp(g_strItemCategory[iIndex],"end")==0)	break;

		s_listAddItemList.AddItem(0,g_strItemCategory[iIndex],iIndex);
		s_listAddItemList.IncreaseLineCounter();

		iIndex++;
	}

	s_listAddItemList.Select(0);

	if (s_listDropItemList.m_iLineCounter	>=	dDROPPING_ITEM_COUNT	||	s_listAddItemList.m_iLineCounter	<=	0)
			EnableWindow(GetDlgItem(hDlg,IDC_ADD_ITEM),FALSE);
	else	EnableWindow(GetDlgItem(hDlg,IDC_ADD_ITEM),TRUE);

}	//	cSET_JOB::UpdateAddItemList(HWND hDlg)

void
cSET_JOB::AddDropItem(HWND hDlg)
{
	if (s_listDropItemList.m_iLineCounter	>=	dDROPPING_ITEM_COUNT	||	s_listAddItemList.m_iLineCounter	<=	0)
		return;

	int	iItem	=	0,iCount	=	0;

	for (int i = 0; i<dDROPPING_ITEM_COUNT;i++)
	{
		iItem	=	s_makeJob.m_aDroppingItem[i].m_wItemType;

		if (iItem	==	0xffff)
		{	
			iItem	=	i;
			break;
		}
		else	iCount++;
	}

	s_makeJob.m_aDroppingItem[iItem].reset();
	s_makeJob.m_aDroppingItem[iItem].m_wItemType	=	s_listAddItemList.GetSelectData();

	UpdateDropItemList(hDlg,iCount);

	if (s_listDropItemList.m_iLineCounter	>=	dDROPPING_ITEM_COUNT)	EnableWindow(GetDlgItem(hDlg,IDC_ADD_ITEM),FALSE);

	DrawDeathData(hDlg);
}

void
cSET_JOB::RemoveDropItem(HWND hDlg)
{
	int	iItem	=	s_listDropItemList.GetSelectData();		//	현재 선택한 아이템

	s_makeJob.m_aDroppingItem[iItem].reset();

	int iSelect		=	s_listDropItemList.m_iSelect;

	if (iSelect	>=	s_listDropItemList.m_iLineCounter -1)	iSelect--;

	UpdateDropItemList(hDlg,iSelect);

	if (s_listDropItemList.m_iLineCounter	<=	0)	EnableWindow(GetDlgItem(hDlg,IDC_ADD_ITEM),FALSE);
	if (s_listDropItemList.m_iLineCounter	<	dDROPPING_ITEM_COUNT)	EnableWindow(GetDlgItem(hDlg,IDC_ADD_ITEM),TRUE);

	DrawDeathData(hDlg);
}

LRESULT CALLBACK
cSET_JOB::s_procDeathData(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{	case WM_INITDIALOG	:
			InitDeathData(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDOK);
			break;

		case WM_COMMAND		:
			switch( LOWORD( wParam ) )
            {
				case	IDC_ADD_ITEM	:
				{
					AddDropItem(hDlg);
					break;
				}

				case	IDC_REMOVE_ITEM		:
				{
					RemoveDropItem(hDlg);
					break;
				}

				case	IDC_BASIC_GOLD		:
				{
					switch (HIWORD(wParam))
                    {
						case	EN_CHANGE	:
						{
							BOOL	temp;
							int		iValue	=	GetDlgItemInt(hDlg,LOWORD( wParam ),&temp,FALSE);

							s_makeJob.m_wGold	=	min(iValue,0xffff);
							break;
						}
					}
					break;
				}

				case	IDC_GOLD			:
				{
					switch (HIWORD(wParam))
                    {
						case	EN_CHANGE	:
						{
							char	strText[56];

							GetDlgItemText(hDlg,LOWORD( wParam ),strText,56);

							s_makeJob.m_wGoldPerLevel	=	StringToNumber(strText,655,2);
							break;
						}
					}
					break;
				}


				case	IDC_DEATH_EXP		:
				{
					switch (HIWORD(wParam))
                    {
						case	EN_CHANGE	:
						{
							char	strText[52];
							
							GetDlgItemText(hDlg,IDC_DEATH_EXP,strText,52);
							s_makeJob.m_dwDeathExp	=	StringToNumber(strText,655,2);
							break;
						}
					}
					break;
				}

				case	IDC_GOLD_DROP_RATE	:
				{
					switch (HIWORD(wParam))
                    {
						case	EN_CHANGE	:
						{
							BOOL	temp;
							WORD	wValue	=	GetDlgItemInt(hDlg,IDC_GOLD_DROP_RATE,&temp,FALSE);

							if (temp)	s_makeJob.m_wMoneyDroppingRate	=	min(wValue,100);
							break;
						}
					}
					break;
				}

				case	IDC_DROP_RATE		:
				{
					switch (HIWORD(wParam))
                    {
						case	EN_CHANGE	:
						{
							int	iIndex	=	s_listDropItemList.GetSelectData();

							char	strRate[20];
							int		iValue;
							GetDlgItemText(hDlg,IDC_DROP_RATE,strRate,sizeof(strRate));

							iValue	=	StringToNumber(strRate,100,2);

							if (iValue	==	0)	iValue	=	1;

							s_makeJob.m_aDroppingItem[iIndex].m_dwDroppingRate	=	iValue;

							s_listDropItemList.SetSelectItemStr(1,_ms("%d.%.2d％",iValue/100,iValue%100));
							s_listDropItemList.Draw();
							break;
						}
					}
					break;
				}


				case	IDC_ITEM_COUNT		:
				{
					switch (HIWORD(wParam))
                    {
						case	EN_CHANGE	:
						{
							int	iIndex	=	s_listDropItemList.GetSelectData();

							BOOL	temp;
							DWORD	iValue	=	GetDlgItemInt(hDlg,IDC_ITEM_COUNT,&temp,FALSE);

							if (temp)	
							{	
								s_makeJob.m_aDroppingItem[iIndex].m_bf8Count	=	(BYTE)min(iValue,0xff);
								s_listDropItemList.SetSelectItemStr(2,_ms("%d",s_makeJob.m_aDroppingItem[iIndex].m_bf8Count));
								s_listDropItemList.Draw();
							}
							break;
						}
					}
					break;
				}

				default		:
					break;
			}
			break;
	}

	return (0L);
}

LRESULT CALLBACK
cSET_JOB::s_procDropItemList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN		:
		{
			DrawDeathData(hDlg);
			break;
		}

		default					:
			return( DefWindowProc( hDlg, uMsg, wParam, lParam ) );
	}

	return (0L);
}