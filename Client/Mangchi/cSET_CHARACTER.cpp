#include	"cSET.H"
#include	"cSKILL.H"
#include	"cJOB.H"
#include	"cDRAW.H"
#include	"cTEXT.H"
#include	"cMAIN.H"
#include	"cITEM_PREFIX_EFFECT.H"
#include	"resource.H"

BOOL			cSET_CHARACTER::s_isEditCharacter	=	FALSE;
HWND			cSET_CHARACTER::s_hWnd				=	NULL;
cDIBWND			cSET_CHARACTER::s_dibBodyList;
cDIBWND			cSET_CHARACTER::s_dibJobData;
cLISTCONTROL	cSET_CHARACTER::s_listJob;									//	직업 리스트

cCHARACTER_SET	*cSET_CHARACTER::s_lpCharacterSet;
cSCROLL_BAR		cSET_CHARACTER::s_sbBodyList;		//	몸뚱아리 리스트의 스크롤바
int				cSET_CHARACTER::s_iBodyPerWindow;	//	윈도우에 나타나는 몸통의 수
int				cSET_CHARACTER::s_iSelectBody;		//	선택한 몸뚱아리
int				cSET_CHARACTER::s_iJobGroup;		//	직업 그룹 
cCHARACTER		cSET_CHARACTER::s_makeCharacter;
int				cSET_CHARACTER::s_iBodyCount;
int				cSET_CHARACTER::s_iBodyWidth		=	66;
int				cSET_CHARACTER::s_iBodyHeight		=	80;
int				cSET_CHARACTER::s_iSelectJobDataTab	=	0;	//	선택한 직업 데이터 탭

//
//	캐릭터 추가
BOOL
cSET_CHARACTER::AddCharacter(HWND hwnd,int _iJobGroup,cCHARACTER_SET	*_lpCharacterSet)
{
	s_lpCharacterSet	=	_lpCharacterSet;

	s_makeCharacter.reset();

	if (Init(hwnd,_iJobGroup))
	{
		s_lpCharacterSet->add(&s_makeCharacter);

		return	TRUE;
	}

	return	FALSE;
}

//
//	캐릭터 데이터 수정
BOOL
cSET_CHARACTER::EditCharacter(HWND hwnd,cCHARACTER_SET	*_lpCharacterSet)
{
	s_lpCharacterSet	=	_lpCharacterSet;

	s_isEditCharacter	=	TRUE;

	cCHARACTER	*lpCharacter	=	s_lpCharacterSet->getSelectCharacter();

	if (lpCharacter)	s_makeCharacter.copy(lpCharacter);
	else				return	FALSE;

	int iJobGroup	=	lpCharacter->getJob()->m_wType;

	if (Init(hwnd,iJobGroup))
	{
		int	iSerial	=	lpCharacter->m_wSerial;

		lpCharacter->copy(&s_makeCharacter);

		lpCharacter->m_wSerial	=	iSerial;

		return	FALSE;
	}

	return	FALSE;
}

//
//	추가하기 대화상자 초기화
BOOL
cSET_CHARACTER::Init(HWND hwnd,int _iJobGroup)
{
	s_iJobGroup			=	_iJobGroup;

//	초기화
	{
		g_BodyList.arrangeData(s_iJobGroup);

		s_iBodyCount	=	g_BodyList.getUsedBodyCount();
	}

	if (DialogBox( s_hInst,"SET_CHARACTER_DATA",hwnd,(DLGPROC)s_procSetCharacter)==IDOK)	return	TRUE;

	return	FALSE;
}

//
//	영웅 추가 대화상자를 초기화 한다.
//
void
cSET_CHARACTER::InitSetCharacter(HWND hDlg)
{
	s_hWnd				=	hDlg;

	HWND	hName		=	GetDlgItem(hDlg,IDC_NAME);
	HWND	hJobList	=	GetDlgItem(hDlg,IDC_JOB_LIST);
	HWND	hBodyList	=	GetDlgItem(hDlg,IDC_BODY_LIST);

//	s_iSelectBody		=	0;

	int		iPoisotion	=	-1;

	if	(s_makeCharacter.m_wBaseJob	!=	0xffff)
		s_iSelectBody	=	s_makeCharacter.getJob()->m_wBody;

	{
		for (int i=0;i<g_BodyList.m_iBodyCount;i++)
		{
			if	(g_BodyList.getBody(i)	==	s_iSelectBody)
				iPoisotion	=	i;
		}

		if	(iPoisotion	==	-1)
		{
			s_iSelectBody	=	0;
			iPoisotion		=	0;
		}
	}

//	직업 리스트
	{
		s_sbBodyList.Init(hBodyList);
		s_sbBodyList.InitWidth();

		s_dibBodyList.lpszRegister	=	"JOB_BODY_LIST";
		s_dibBodyList.Init(hBodyList,(WNDPROC)s_procDibBodyList);

		s_iBodyPerWindow	=	s_dibBodyList.width()/(s_iBodyWidth+2);		//	한번에 나올 수 있는 몸뚱아리 수

		s_sbBodyList.SetWidth(s_iBodyCount,s_iBodyPerWindow);
		s_sbBodyList.SetWidthPos(0);
		s_sbBodyList.correctWidthPossition(iPoisotion);
	}
//	직업 리스트
	{
		s_listJob.Init		(hJobList		,1);		//	3개의 컬럼
		s_listJob.AddColumn	("Job List"		,6*34);
		s_listJob.HideColumn();

		UpdateList();

		if	(s_makeCharacter.m_wBaseJob	!=	0xffff)
			s_listJob.selectByData(0,s_makeCharacter.m_wBaseJob);
	}

	{
		HWND	hSkin	=	GetDlgItem(hDlg,IDC_SKIN);

		for	(int	i=0;;i++)
		{
			if	(stricmp(g_astrNpcSkin[i],"end")==0)
				break;

			cCOMBOBOX::Add(hSkin,g_astrNpcSkin[i],i);
		}

		cCOMBOBOX::SelectByData(hSkin,s_makeCharacter.m_bf8Skin);
	}

//	알맹이 데이터
	{
		SetDlgItemText(hDlg,IDC_NAME,s_makeCharacter.m_strName);
		SetDlgItemInt(hDlg,IDC_LEVEL,s_makeCharacter.m_wMinLevel,FALSE);
		SetDlgItemInt(hDlg,IDC_LEVEL_MAX,s_makeCharacter.m_wMaxLevel,FALSE);

		SetDlgItemInt(hDlg,IDC_SCALE_X	,s_makeCharacter.m_wScaleX,FALSE);
		SetDlgItemInt(hDlg,IDC_SCALE_Y	,s_makeCharacter.m_wScaleY,FALSE);

		SetDlgItemInt(hDlg,IDC_CORRECT_FINAL_DAMAGE	,s_makeCharacter.m_wCorrectFinalDamage,FALSE);

		SetDlgItemInt(hDlg,IDC_CORRECT_EXP	,s_makeCharacter.m_wCorrectExp,FALSE);
		SetDlgItemInt(hDlg,IDC_CORRECT_HP	,s_makeCharacter.m_wCorrectHP,FALSE);
		SetDlgItemInt(hDlg,IDC_CORRECT_AP	,s_makeCharacter.m_wCorrectAP,FALSE);

		SetDlgItemInt(hDlg,IDC_AB_PH_DAMAGE_CHANCE		,s_makeCharacter.m_asAbsorbMagicDamage[0][0],TRUE);
		SetDlgItemInt(hDlg,IDC_AB_FIRE_DAMAGE_CHANCE	,s_makeCharacter.m_asAbsorbMagicDamage[1][0],TRUE);
		SetDlgItemInt(hDlg,IDC_AB_WATER_DAMAGE_CHANCE	,s_makeCharacter.m_asAbsorbMagicDamage[2][0],TRUE);
		SetDlgItemInt(hDlg,IDC_AB_WIND_DAMAGE_CHANCE	,s_makeCharacter.m_asAbsorbMagicDamage[3][0],TRUE);
		SetDlgItemInt(hDlg,IDC_AB_EARTH_DAMAGE_CHANCE	,s_makeCharacter.m_asAbsorbMagicDamage[4][0],TRUE);
		SetDlgItemInt(hDlg,IDC_AB_LIGHT_DAMAGE_CHANCE	,s_makeCharacter.m_asAbsorbMagicDamage[5][0],TRUE);
		SetDlgItemInt(hDlg,IDC_AB_DARK_DAMAGE_CHANCE	,s_makeCharacter.m_asAbsorbMagicDamage[6][0],TRUE);
		
		SetDlgItemInt(hDlg,IDC_AB_PH_DAMAGE_RATE		,s_makeCharacter.m_asAbsorbMagicDamage[0][1],TRUE);
		SetDlgItemInt(hDlg,IDC_AB_FIRE_DAMAGE_RATE		,s_makeCharacter.m_asAbsorbMagicDamage[1][1],TRUE);
		SetDlgItemInt(hDlg,IDC_AB_WATER_DAMAGE_RATE		,s_makeCharacter.m_asAbsorbMagicDamage[2][1],TRUE);
		SetDlgItemInt(hDlg,IDC_AB_WIND_DAMAGE_RATE		,s_makeCharacter.m_asAbsorbMagicDamage[3][1],TRUE);
		SetDlgItemInt(hDlg,IDC_AB_EARTH_DAMAGE_RATE		,s_makeCharacter.m_asAbsorbMagicDamage[4][1],TRUE);
		SetDlgItemInt(hDlg,IDC_AB_LIGHT_DAMAGE_RATE		,s_makeCharacter.m_asAbsorbMagicDamage[5][1],TRUE);
		SetDlgItemInt(hDlg,IDC_AB_DARK_DAMAGE_RATE		,s_makeCharacter.m_asAbsorbMagicDamage[6][1],TRUE);
	}

	SetDlgItemText(hDlg,IDOK,dMSG_OK);

	SetFocus(hName);

	int		iIndex	=	0;

	SetDlgItemText(hDlg,IDC_NAME		,s_makeCharacter.m_strName);

	SetDlgItemInt(hDlg,IDC_CORRECT_USE_SKILL_CHANCE,s_makeCharacter.m_bf8CorrectUseSkillChance,FALSE);
	CheckDlgButton(hDlg,IDC_INVINCIBLE		,s_makeCharacter.m_bf1IsInvincible);
	CheckDlgButton(hDlg,IDC_NOT_EXP			,s_makeCharacter.m_bf1IsNotExp);
	CheckDlgButton(hDlg,IDC_NOT_OWN_ITEM	,s_makeCharacter.m_bf1IsNotDropItem);
	CheckDlgButton(hDlg,IDC_IS_KEY_MONSTER	,s_makeCharacter.m_bf1IsKeyMonster);
	CheckDlgButton(hDlg,IDC_NOT_OWN_GOLD	,s_makeCharacter.m_bf1IsNotDropGold);
	CheckDlgButton(hDlg,IDC_REVISE_INT_CALC	,s_makeCharacter.m_bf1IsReviseIntelligence);

	CheckDlgButton(hDlg,IDC_CHARMING_IMMUNE	,s_makeCharacter.m_bf1IsImmuneCharming);
	CheckDlgButton(hDlg,IDC_STUN_IMMUNE		,s_makeCharacter.m_bf1IsImmuneStun	);
	CheckDlgButton(hDlg,IDC_STONE_IMMUNE	,s_makeCharacter.m_bf1IsImmuneStone	);
	CheckDlgButton(hDlg,IDC_FREEZE_IMMUNE	,s_makeCharacter.m_bf1IsImmuneFreeze	);
	CheckDlgButton(hDlg,IDC_NOCK_BACK_IMMUNE,s_makeCharacter.m_bf1IsImmuneKnockBack);
	CheckDlgButton(hDlg,IDC_P_DAMAGE_IMMUNE,s_makeCharacter.m_bf1IsImmunePhysicalDamage);
	CheckDlgButton(hDlg,IDC_M_DAMAGE_IMMUNE,s_makeCharacter.m_bf1IsImmuneMagicDamage);

	//JBC	혼란 면역.		08-08-27
	CheckDlgButton(hDlg,IDC_CONFUSE_IMMUNE,s_makeCharacter.m_bf1IsImmuneConfuse);
	CheckDlgButton(hDlg,IDC_IS_SET_BOSS_MONSTER,s_makeCharacter.m_bf1IsSetBoss);
	CheckDlgButton(hDlg,IDC_CAN_NOT_TAME,s_makeCharacter.m_bf1IsBlockToTame);


	Draw();
}

void
cSET_CHARACTER::Draw()
{
	DrawBodyList();
	DrawJobList();
	DrawJobData();
}

void
cSET_CHARACTER::DrawJobList()
{
	s_listJob.Draw();
}


void
cSET_CHARACTER::DrawBodyList()
{
	if (!s_dibBodyList.Active())
		return;

	cDRAW::Fill(0);

	int	j,x=0,y=	4;
	int	index	=	0;
	int	iCount	=	0;

	x	=	(s_dibBodyList.width()-s_iBodyPerWindow*(s_iBodyWidth+2))/2;

	for (j=0;j<s_iBodyPerWindow+s_sbBodyList.m_iWidthPos && index<dBODY_COUNT;index++)
	{
		if(g_BodyList.getJobCount(index)	>	0)
		{
			if (j>=s_sbBodyList.m_iWidthPos)
			{
				int iBody	=	g_BodyList.getBody(index);

				if (g_aMonsterPalette[iBody].m_wCount && g_pBody[iBody]->pPLT)
					memcpy(g_pBody[iBody]->pPLT,&g_aMonsterPalette[iBody].m_pPalette[0],512);

				if (g_BodyList.getBody(index)	==	s_iSelectBody)
				{
					cDRAW::Fill(_LTBLUE,x+1,y,x+s_iBodyWidth+1,y+s_iBodyHeight+2);
					cDRAW::Box(_LTGREEN,x+1,y,x+s_iBodyWidth+1,y+s_iBodyHeight+2);

					g_pBody[iBody]->FitPut(x+3,y+1,s_iBodyWidth-4,s_iBodyHeight,4,TRUE);
				}
				else	g_pBody[iBody]->FitPut(x+3,y+1,s_iBodyWidth-4,s_iBodyHeight,4);

				x	+=	(s_iBodyWidth+2);
			}
			j++;
		}

		if (j>= g_BodyList.m_iBodyCount)	break;
	}

	s_dibBodyList.Draw();
}

void
cSET_CHARACTER::DrawJobData()
{
}

//
//	몸통을 선택한다.
BOOL
cSET_CHARACTER::SelectBody(HWND hwnd,int _iX,int _iY)
{
	int	j;
	int	index	=	0;

	int	x		=	(s_dibBodyList.width()-s_iBodyPerWindow*(s_iBodyWidth+2))/2;

	for (j=0;j<s_iBodyPerWindow+s_sbBodyList.m_iWidthPos;index++)
	{
		if(g_BodyList.getJobCount(index)	>	0)
		{
			if (j>=s_sbBodyList.m_iWidthPos)
			{
				cRECT	rect;
				rect.Set(x,0,x+s_iBodyWidth+2,s_dibBodyList.height());

				if (rect.isIN(_iX,_iY))
				{
					int	iBody	=	g_BodyList.getBody(index);

					if (s_iSelectBody	==	iBody)	return	FALSE;

					s_iSelectBody	=	iBody;

					return	TRUE;
				}
				x	+=	(s_iBodyWidth+2);
			}
			j++;
		}

		if (index >= g_BodyList.m_iBodyCount)	break;
	}

	return	FALSE;
}	//	cJOB_LIST::SelectBody(HWND hwnd,int x,int y)

//
//	설정된 데이터를 읽어들인다.
BOOL
cSET_CHARACTER::GetCharacterData(HWND hDlg)
{
	cJOB	*lpJob;
	char	strName[18];
	int		iIndex	=	2;
	BOOL	temp;

//	기본 직업 설정
	{
		s_makeCharacter.m_wBaseJob	=	s_listJob.GetSelectData();
	}//	기본 직업 설정

	lpJob	=	s_makeCharacter.getJob();

//	캐릭터 이름 설정
	{
		GetDlgItemText(hDlg,IDC_NAME	,strName	,sizeof(strName));

		if	(strcmp(strName,"")	==	0)
		{
			strcpy(strName,lpJob->m_strName);

			char	strCmpName[64];
			int		iLength	=	strlen(strName);

			memcpy(strCmpName,strName+iLength-3,4);

			if	(stricmp(strCmpName,"ZIN")==0)
				strName[strlen(strName)-4]	=	NULL;
			else
			{
				memcpy(strCmpName,strName+iLength-2,3);

				if	(stricmp(strCmpName,"EX")==0)
					strName[strlen(strName)-3]	=	NULL;
			}
		}

		if	(strlen(strName)	>	dNAME_LENGTH-2)
		{
			cMSG::Put("경고!","캐릭터 이름이 너무 깁니다. 영문 16자 한글 8자 이하로 맞춰 주시기 바랍니다.");
			return	FALSE;
		}
		
		strcpy(s_makeCharacter.m_strName,strName);
	}//	캐릭터 이름 설정

	{
		HWND	hSkin	=	GetDlgItem(hDlg,IDC_SKIN);

		s_makeCharacter.m_bf8Skin	=	cCOMBOBOX::GetData(hSkin);
	}

//	레벨 설정
	{
		s_makeCharacter.m_wMinLevel				=	GetDlgItemInt(hDlg,IDC_LEVEL	,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_wMinLevel	=	g_denJob.getJob(s_makeCharacter.m_wBaseJob)->m_iLevel;
		s_makeCharacter.m_wMaxLevel				=	GetDlgItemInt(hDlg,IDC_LEVEL_MAX,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_wMaxLevel	=	s_makeCharacter.m_wMinLevel;

		s_makeCharacter.m_wCorrectFinalDamage	=	GetDlgItemInt(hDlg,IDC_CORRECT_FINAL_DAMAGE	,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_wCorrectFinalDamage	=	10000;

		s_makeCharacter.m_wScaleX				=	GetDlgItemInt(hDlg,IDC_SCALE_X	,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_wScaleX	=	100;
		s_makeCharacter.m_wScaleY				=	GetDlgItemInt(hDlg,IDC_SCALE_Y	,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_wScaleY	=	100;

		s_makeCharacter.m_wCorrectExp				=	GetDlgItemInt(hDlg,IDC_CORRECT_EXP	,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_wCorrectExp	=	100;
		s_makeCharacter.m_wCorrectHP				=	GetDlgItemInt(hDlg,IDC_CORRECT_HP	,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_wCorrectHP	=	100;
		s_makeCharacter.m_wCorrectAP				=	GetDlgItemInt(hDlg,IDC_CORRECT_AP	,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_wCorrectAP	=	100;


		s_makeCharacter.m_asAbsorbMagicDamage[0][0]	=	GetDlgItemInt(hDlg,IDC_AB_PH_DAMAGE_CHANCE		,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_asAbsorbMagicDamage[0][0]	=	0;
		s_makeCharacter.m_asAbsorbMagicDamage[1][0]	=	GetDlgItemInt(hDlg,IDC_AB_FIRE_DAMAGE_CHANCE	,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_asAbsorbMagicDamage[1][0]	=	0;
		s_makeCharacter.m_asAbsorbMagicDamage[2][0]	=	GetDlgItemInt(hDlg,IDC_AB_WATER_DAMAGE_CHANCE	,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_asAbsorbMagicDamage[2][0]	=	0;
		s_makeCharacter.m_asAbsorbMagicDamage[3][0]	=	GetDlgItemInt(hDlg,IDC_AB_WIND_DAMAGE_CHANCE	,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_asAbsorbMagicDamage[3][0]	=	0;
		s_makeCharacter.m_asAbsorbMagicDamage[4][0]	=	GetDlgItemInt(hDlg,IDC_AB_EARTH_DAMAGE_CHANCE	,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_asAbsorbMagicDamage[4][0]	=	0;
		s_makeCharacter.m_asAbsorbMagicDamage[5][0]	=	GetDlgItemInt(hDlg,IDC_AB_LIGHT_DAMAGE_CHANCE	,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_asAbsorbMagicDamage[5][0]	=	0;
		s_makeCharacter.m_asAbsorbMagicDamage[6][0]	=	GetDlgItemInt(hDlg,IDC_AB_DARK_DAMAGE_CHANCE	,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_asAbsorbMagicDamage[6][0]	=	0;

		s_makeCharacter.m_asAbsorbMagicDamage[0][1]	=	GetDlgItemInt(hDlg,IDC_AB_PH_DAMAGE_RATE		,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_asAbsorbMagicDamage[0][1]	=	0;
		s_makeCharacter.m_asAbsorbMagicDamage[1][1]	=	GetDlgItemInt(hDlg,IDC_AB_FIRE_DAMAGE_RATE		,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_asAbsorbMagicDamage[1][1]	=	0;
		s_makeCharacter.m_asAbsorbMagicDamage[2][1]	=	GetDlgItemInt(hDlg,IDC_AB_WATER_DAMAGE_RATE		,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_asAbsorbMagicDamage[2][1]	=	0;
		s_makeCharacter.m_asAbsorbMagicDamage[3][1]	=	GetDlgItemInt(hDlg,IDC_AB_WIND_DAMAGE_RATE		,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_asAbsorbMagicDamage[3][1]	=	0;
		s_makeCharacter.m_asAbsorbMagicDamage[4][1]	=	GetDlgItemInt(hDlg,IDC_AB_EARTH_DAMAGE_RATE		,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_asAbsorbMagicDamage[4][1]	=	0;
		s_makeCharacter.m_asAbsorbMagicDamage[5][1]	=	GetDlgItemInt(hDlg,IDC_AB_LIGHT_DAMAGE_RATE		,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_asAbsorbMagicDamage[5][1]	=	0;
		s_makeCharacter.m_asAbsorbMagicDamage[6][1]	=	GetDlgItemInt(hDlg,IDC_AB_DARK_DAMAGE_RATE		,&temp,FALSE);
		if	(!temp)	s_makeCharacter.m_asAbsorbMagicDamage[6][1]	=	0;

		s_makeCharacter.m_wScaleX				=	max(s_makeCharacter.m_wScaleX,10);
		s_makeCharacter.m_wScaleY				=	max(s_makeCharacter.m_wScaleY,10);
		s_makeCharacter.m_wScaleX				=	min(s_makeCharacter.m_wScaleX,200);
		s_makeCharacter.m_wScaleY				=	min(s_makeCharacter.m_wScaleY,200);

		if	(s_makeCharacter.m_wMaxLevel<s_makeCharacter.m_wMinLevel)
			s_makeCharacter.m_wMaxLevel=s_makeCharacter.m_wMinLevel;

		s_makeCharacter.m_bf8CorrectUseSkillChance=	GetDlgItemInt(hDlg,IDC_CORRECT_USE_SKILL_CHANCE,&temp,FALSE);
		s_makeCharacter.m_bf1IsInvincible		=	IsDlgButtonChecked(hDlg,IDC_INVINCIBLE);
		s_makeCharacter.m_bf1IsNotExp			=	IsDlgButtonChecked(hDlg,IDC_NOT_EXP);
		s_makeCharacter.m_bf1IsNotDropItem		=	IsDlgButtonChecked(hDlg,IDC_NOT_OWN_ITEM);
		s_makeCharacter.m_bf1IsKeyMonster		=	IsDlgButtonChecked(hDlg,IDC_IS_KEY_MONSTER);
		s_makeCharacter.m_bf1IsNotDropGold		=	IsDlgButtonChecked(hDlg,IDC_NOT_OWN_GOLD);
		s_makeCharacter.m_bf1IsReviseIntelligence=	IsDlgButtonChecked(hDlg,IDC_REVISE_INT_CALC);


		s_makeCharacter.m_bf1IsImmuneCharming	=	IsDlgButtonChecked(hDlg,IDC_CHARMING_IMMUNE);
		s_makeCharacter.m_bf1IsImmuneStun		=	IsDlgButtonChecked(hDlg,IDC_STUN_IMMUNE);
		s_makeCharacter.m_bf1IsImmuneStone		=	IsDlgButtonChecked(hDlg,IDC_STONE_IMMUNE);
		s_makeCharacter.m_bf1IsImmuneFreeze		=	IsDlgButtonChecked(hDlg,IDC_FREEZE_IMMUNE);
		s_makeCharacter.m_bf1IsImmuneKnockBack	=	IsDlgButtonChecked(hDlg,IDC_NOCK_BACK_IMMUNE);
		s_makeCharacter.m_bf1IsImmunePhysicalDamage	=	IsDlgButtonChecked(hDlg,IDC_P_DAMAGE_IMMUNE);
		s_makeCharacter.m_bf1IsImmuneMagicDamage	=	IsDlgButtonChecked(hDlg,IDC_M_DAMAGE_IMMUNE);
		s_makeCharacter.m_bf1IsImmuneConfuse	=	IsDlgButtonChecked(hDlg,IDC_CONFUSE_IMMUNE);
		s_makeCharacter.m_bf1IsSetBoss			=	IsDlgButtonChecked(hDlg,IDC_IS_SET_BOSS_MONSTER);
		s_makeCharacter.m_bf1IsBlockToTame		=	IsDlgButtonChecked(hDlg,IDC_CAN_NOT_TAME);
	}

	return	TRUE;
}

//
//	
void
cSET_CHARACTER::UpdateList()
{
//	직업 리스트에 추가
	s_listJob.ResetItems();

	for (int i=0;i<dMAX_JOB_COUNT;i++)
	{
		cJOB	*lpJob	=	g_denJob.getJob(i);

		if (lpJob)
		{
			if (lpJob->m_wType	==	s_iJobGroup)
			{
				if	(lpJob->m_wBody	==	s_iSelectBody)
				{
					s_listJob.AddItem(0,_ms("%s[%s]",lpJob->m_strName,g_strMonsterLevel[lpJob->m_wMonsterLevel]),lpJob->m_iSerial);
					s_listJob.IncreaseLineCounter();
				}
			}
		}
	}

//	정렬
//	s_listJob.Sort(0);
	s_listJob.Select(0);

	if (s_listJob.m_iLineCounter	<=	0)	EnableWindow(GetDlgItem(s_hWnd,IDOK),FALSE);
	else									EnableWindow(GetDlgItem(s_hWnd,IDOK),TRUE);
}

//
//
void
cSET_CHARACTER::SyncCharacterName(HWND _hDlg)
{
	if (!s_isEditCharacter)	return;

	char	strName[64];

	GetDlgItemText(_hDlg,IDC_NAME	,strName	,sizeof(strName));

	if (strcmp(strName,"")	==	0)	return;

	cCHARACTER	*lpCharacter	=	s_lpCharacterSet->getSelectCharacter();

	if (!lpCharacter)	return;

	g_lpActmap->m_setActor.changeName(lpCharacter->m_wSerial,strName);
	g_lpActmap->LostVirgin();

	MAIN.Draw();
}	//	cSET_CHARACTER::SyncCharacterName(HWND _hDlg)


LRESULT CALLBACK 
cSET_CHARACTER::s_procSetCharacter(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{	case WM_INITDIALOG	:
			InitSetCharacter(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDCANCEL);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
			{
				case	IDOK				:
					if (GetCharacterData(hDlg))
						EndDialog(hDlg,IDOK);
					break;

				case	IDC_APPLY_NAME		:
					SyncCharacterName(hDlg);
					break;

				case	IDC_VIEW_JOB_DATA	:
				{
					int iSelectJob	=	s_listJob.GetSelectData(0);

					if	(iSelectJob	==	0x7fffffff)
						break;

					cJOB	*lpOldJob	=	g_denJob.getJob(iSelectJob);

					if	(!lpOldJob)	
						break;

					cSET_JOB::NewJob(hDlg,s_iJobGroup,s_iSelectBody,iSelectJob);

					break;
				}

				case	IDCANCEL			:
					EndDialog(hDlg,IDCANCEL);
					break;

				case	IDC_SET_EVENT		:
				{
					SetEvent(hDlg,&s_makeCharacter);
					break;
				}
			}
			break;
	}

	return (0L);
}


LRESULT CALLBACK
cSET_CHARACTER::s_procDibBodyList(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibBodyList.Active();
			s_dibBodyList.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		case WM_HSCROLL		:
		{
			if (s_sbBodyList.HScroll(wParam))	Draw();
			break;
		}

		case WM_LBUTTONDOWN		:
		{
			if(SelectBody(hwnd,LOWORD(lParam),HIWORD(lParam)))
			{
				UpdateList();
				s_listJob.Select(0);

				Draw();
			}
			break;
		}

		default					:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}
LRESULT CALLBACK 
cSET_CHARACTER::s_procDibJobData(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibJobData.Active();
			s_dibJobData.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}