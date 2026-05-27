#include	"cSET.H"
#include	"cSKILL.H"
#include	"cJOB.H"
#include	"cDRAW.H"
#include	"cTEXT.H"
#include	"cMAIN.H"
#include	"cITEM_PREFIX_EFFECT.H"
#include	"cPROJECT.H"
#include	"resource.H"
#include	"cTOOLWND.H"
#include	"cMAPWND.H"

HWND			cJOB_LIST::s_hWnd		=	NULL;
cDIBWND			cJOB_LIST::s_dibBodyList;
cDIBWND			cJOB_LIST::s_dibJobData;
cSCROLL_BAR		cJOB_LIST::s_sbBodyList;
cLISTCONTROL	cJOB_LIST::s_listJob;		//	직업 리스트

int				cJOB_LIST::s_iSelectBodyGroup	=	dJOB_GROUP_MONSTER;
int				cJOB_LIST::s_iSelectJobDataTab	=	0;
int				cJOB_LIST::s_aSelectBody[3]		=	{0,0,0};
int				cJOB_LIST::s_aScrollBarPos[3]	=	{0xffff,0,0};	//	스크롤바의 위치

cJOB			cJOB_LIST::s_makeJob;
int				cJOB_LIST::s_iBodyPerWindow	=	0;		//	한번에 나올 수 있는 몸뚱아리 수
int				cJOB_LIST::s_iBodyWidth		=	66;
int				cJOB_LIST::s_iBodyHeight	=	80;

//
//	영웅추가하기 대화상자 초기화
void
cJOB_LIST::Init(HWND hwnd,HINSTANCE hinst)
{
	s_hInst		=	hinst;

	DialogBox( s_hInst, "JOB_LIST",hwnd,(DLGPROC)s_procSetJobList);

	cPROJECT::SaveJob();

	if (g_lpActmap->m_setCharacter.reBuild())	g_lpActmap->m_setActor.reBuild();

	TOOLWND.Draw();
	g_MAPWND.Draw();
}	//	cJOB_LIST::Init(HWND hwnd,HINSTANCE hinst)

//
//	영웅 추가 대화상자를 초기화 한다.
void
cJOB_LIST::InitJobList(HWND hDlg)
{
	s_hWnd	=	hDlg;
	if (s_aScrollBarPos[0]	==	0xffff)
	{
		s_aScrollBarPos[0]	=	0;
		s_aScrollBarPos[1]	=	g_BodyList.m_iHeroBodyCount;
		s_aScrollBarPos[2]	=	g_BodyList.m_iHeroBodyCount+g_BodyList.m_iMonsterBodyCount;
		s_aSelectBody[0]	=	s_aScrollBarPos[0];
		s_aSelectBody[1]	=	s_aScrollBarPos[1];
		s_aSelectBody[2]	=	s_aScrollBarPos[2];
	}

	TC_ITEM item;

	HWND	hTabJobType	=	GetDlgItem(hDlg,IDC_TAB_BODY_GROUP);

/*	item.mask		=	TCIF_TEXT | TCIF_PARAM;
	item.pszText	=	dMSG_HERO;
	item.lParam		=	0;
	TabCtrl_InsertItem(hTabJobType,0, &item );
*/
	item.mask		=	TCIF_TEXT | TCIF_PARAM;
	item.pszText	=	"Monster";
	item.lParam		=	0;
	TabCtrl_InsertItem(hTabJobType,0, &item );

	item.mask		=	TCIF_TEXT | TCIF_PARAM;
	item.pszText	=	"NPC";
	item.lParam		=	1;
	TabCtrl_InsertItem(hTabJobType,1, &item );

	TabCtrl_SetCurSel(hTabJobType,s_iSelectBodyGroup-1);		//	선택한 탭..


	HWND	hTabJobData	=	GetDlgItem(hDlg,IDC_VIEW_DATA_TAB);

	item.mask		=	TCIF_TEXT | TCIF_PARAM;
	item.pszText	=	dMSG_BASIC_DATA;
	item.lParam		=	0;
	TabCtrl_InsertItem(hTabJobData,0, &item );
/*
	item.mask		=	TCIF_TEXT | TCIF_PARAM;
	item.pszText	=	dMSG_MAGIC_AND_SKILL;
	item.lParam		=	1;
	TabCtrl_InsertItem(hTabJobData,1, &item );
*/
	item.mask		=	TCIF_TEXT | TCIF_PARAM;
	item.pszText	=	dMSG_EQUIPMENT;
	item.lParam		=	1;
	TabCtrl_InsertItem(hTabJobData,1, &item );

	TabCtrl_SetCurSel(hTabJobData,s_iSelectJobDataTab);		//	선택한 탭..

	HWND	hBodyList	=	GetDlgItem(hDlg,IDC_BODY_LIST);
	HWND	hJobList	=	GetDlgItem(hDlg,IDC_JOB_LIST);
	HWND	hJobData	=	GetDlgItem(hDlg,IDC_JOB_INFO);

	//	몸뚱아리 리스트
	{
		s_sbBodyList.Init(hBodyList);
		s_sbBodyList.InitWidth();

		s_dibBodyList.lpszRegister	=	"JOB_BODY_LIST";
		s_dibBodyList.Init(hBodyList,(WNDPROC)s_procDibBodyList);

		s_iBodyPerWindow	=	s_dibBodyList.width()/(s_iBodyWidth+2);		//	한번에 나올 수 있는 몸뚱아리 수

		s_sbBodyList.SetWidth(g_BodyList.m_iBodyCount,s_iBodyPerWindow);
		s_sbBodyList.SetWidthPos(s_aScrollBarPos[s_iSelectBodyGroup]);
		s_sbBodyList.correctWidthPossition(s_aSelectBody[s_iSelectBodyGroup]);
	}

//	직업 리스트
	{
		s_listJob.Init		(hJobList		,2, FALSE, (WNDPROC)s_procJobDataList);		//	3개의 컬럼
		s_listJob.AddColumn	("Job List"		,6*34);
		s_listJob.HideColumn();
	}

	s_dibJobData.lpszRegister	=	"JOB_DATA";
	s_dibJobData.Init(hJobData,(WNDPROC)s_procDibJobData);

	UpdateList(hDlg);

	Draw(hDlg);
}	//	cJOB_LIST::InitAddJob(HWND hDlg)

// JobDataList
LRESULT CALLBACK
cJOB_LIST::s_procJobDataList(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		DrawJobData(hwnd);
		break;
	}
	return (0L);
}


//
//	몸뚱아리 리스트
void
cJOB_LIST::DrawBodyList(HWND hDlg)
{
	if	(!s_dibBodyList.Active())
		return;

	cDRAW::Fill(0);

	int	j,x=0,y=24;
	int	index	=	s_sbBodyList.m_iWidthPos;

	x	=	(s_dibBodyList.width()-s_iBodyPerWindow*(s_iBodyWidth+2))/2;

	g_BodyList.arrangeData(s_iSelectBodyGroup);

	int	iJobCount	=	g_BodyList.getRegistJobCount();

//	현재 선택한 탭에 등록된 직업의 수
	{
		cTEXT::SetBoldFont();
		cDRAW::Fill(_BLUE,0,1,cDRAW::WIDTH,16);
		switch(s_iSelectBodyGroup)
		{
			case	dJOB_GROUP_NPC		:	cTEXT::Put(6,4,WHITE,_ms("%s : %d",dMSG_REGIST_NPC_COUNT,iJobCount));break;
			case	dJOB_GROUP_MONSTER	:	cTEXT::Put(6,4,LTCYAN,_ms("%s : %d",dMSG_REGIST_MONSTER_COUNT,iJobCount));break;
		}
	}


	for (j=0;j<s_iBodyPerWindow;j++)
	{
		int	iBody	=	g_BodyList.getBody(index);

		if(iBody	!=	0xffff)
		{
			if (index	==	s_aSelectBody[s_iSelectBodyGroup])
			{
				cDRAW::Fill(_LTBLUE,x+1,y,x+s_iBodyWidth+1,y+s_iBodyHeight+20);
				cDRAW::Box(_LTGREEN,x+1,y,x+s_iBodyWidth+1,y+s_iBodyHeight+20);

				g_pBody[iBody]->FitPut(x+3,y+1,s_iBodyWidth-4,s_iBodyHeight,4,TRUE);
			}
			else
			{
				g_pBody[iBody]->FitPut(x+3,y+1,s_iBodyWidth-4,s_iBodyHeight,4);
			}

			if (g_BodyList.getJobCount(index)	>	0)
					cTEXT::cPut(x+ (s_iBodyWidth +2) /2,y+s_iBodyHeight+4,LTYELLOW,_ms("[ %d ]",g_BodyList.getJobCount(index)));
			else	cTEXT::cPut(x+ (s_iBodyWidth +2) /2,y+s_iBodyHeight+4,DKYELLOW,"empty");
		}

		index++;

		if (index >= g_BodyList.m_iBodyCount)	break;

		x	+=	(s_iBodyWidth+2);
	}
	cTEXT::SetNormalFont();

	s_dibBodyList.Draw();
}	//	cJOB_LIST::DrawBodyList(HWND hDlg)


//
//	직업 리스트
void
cJOB_LIST::DrawJobList(HWND hDlg)
{
	s_listJob.Draw();
}	//	cJOB_LIST::DrawJobList(HWND hDlg)

//
//	직업 데이터
void
cJOB_LIST::DrawJobData(HWND hDlg)
{
	if (!s_dibJobData.Active()) return;
	
	cDRAW::Fill(0);
	
	if (s_listJob.m_iSelect	!=	0xffff)	
	{
		int iSelectJob	=	s_listJob.GetSelectData(0);
		if (iSelectJob	==	0x7fffffff)	
			return;
		
		cJOB	*lpJob	=	g_denJob.getJob(iSelectJob);
		
		if(s_iSelectJobDataTab == 0)
		{
			int height = 15, ilxpos  = 10,	icxpos = 190, irxpos = 390;
			
			cDRAW::Box(_WHITE, 5, 5, 585, height * 17 + 1);	
			
			cTEXT::Put(ilxpos, height*1,WHITE,"Profession Name: "); cTEXT::Put(icxpos, height*1,WHITE,"palette: "); cTEXT::Put(irxpos, height*1,WHITE,"View: ");		
			cTEXT::Put(ilxpos, height*2,WHITE,"level: "); cTEXT::Put(icxpos, height*2,WHITE,"Basic health: "); cTEXT::Put(irxpos, height*2,WHITE,"Basic horsepower: ");
			cTEXT::Put(ilxpos, height*3,WHITE,"concept: "); cTEXT::Put(icxpos, height*3,WHITE,"Calculate stamina (LV): "); cTEXT::Put(irxpos, height*3,WHITE,"Calculate stamina (CON): ");
			cTEXT::Put(ilxpos, height*4,WHITE,"kind: "); cTEXT::Put(icxpos, height*4,WHITE,"Propensity: "); cTEXT::Put(irxpos, height*4,WHITE,"Grade: ");
			cTEXT::Put(ilxpos, height*5,WHITE,"appearance:"); cTEXT::Put(icxpos, height*5,WHITE,"Combat tendency: ");
			
			cTEXT::Put(ilxpos,	  height*7,WHITE,"Number of reincarnations required for training: ");										
			cTEXT::Put(ilxpos+15, height*8,WHITE,"Real Boss");			
			cTEXT::Put(ilxpos+115,height*8,WHITE,"Trainable");	
			cTEXT::Put(ilxpos+200,height*8,WHITE, "taming resistance: ");
			cTEXT::Put(405		 ,height*8,WHITE,"Intelligence application formula correction");
			
			cTEXT::Put(ilxpos+70,height*1,YELLOW, lpJob->m_strName);	
			cTEXT::Put(icxpos+90,height*1,YELLOW, _ms("%02d", lpJob->m_wPalette));
			cTEXT::Put(irxpos+90,height*1,YELLOW, _ms("%d Pixel", lpJob->m_sSight));
			
			cTEXT::Put(ilxpos+70,height*2,YELLOW, _ms("%d ~ %d", lpJob->m_iLevel, lpJob->m_wLimitLevel));
			cTEXT::Put(icxpos+90,height*2,YELLOW, _ms("%d", lpJob->m_iMaxHP/100));
			cTEXT::Put(irxpos+90,height*2,YELLOW, _ms("%d", lpJob->m_iMaxCP/100));
			
			int iMagicGrade = 0, iPhysical = 0, iDefence = 0;
			iMagicGrade	= cSET_JOB::GetMonsterMagicConcept(lpJob);
			
			if(iMagicGrade == dGRADE_MONSTER_HARD)
			{
				// 09.12.14 수정
				cABILITY	*lpAbility	=	(cABILITY*)&lpJob->m_aAbility[0];
				cSKILL		*lpSkill	=	&g_aSkill[lpAbility->m_wSkill];

				if(lpSkill->m_wDamageAttribute % 2 == 0)
					iPhysical = dGRADE_MONSTER_WEAK;
				else
					iPhysical = cSET_JOB::GetMonsterPhysicalAttackConcpet(lpJob);
			}
			else
				iPhysical = cSET_JOB::GetMonsterPhysicalAttackConcpet(lpJob);
			
			iDefence = cSET_JOB::GetMonsterDefenceConcept(lpJob);
			
			cTEXT::Put(ilxpos+70,height*3,YELLOW, _ms("%s.%s.%s", g_strMonsterConcept[iMagicGrade+1], g_strMonsterConcept[iPhysical+1], g_strMonsterConcept[iDefence+1]));
			cTEXT::Put(icxpos+90,height*3,YELLOW, _ms("%d.%.1d",lpJob->m_wCorrectMaxHPFactor/10,lpJob->m_wCorrectMaxHPFactor%10));
			cTEXT::Put(irxpos+90,height*3,YELLOW, _ms("%d.%.1d",lpJob->m_wCorrectMaxHPConstitutionFactor/10,lpJob->m_wCorrectMaxHPConstitutionFactor%10));
			
			cTEXT::Put(ilxpos+70,height*4,YELLOW, g_strJobCategory[lpJob->m_wKind]);
			cTEXT::Put(icxpos+90,height*4,YELLOW, g_strAllignment[lpJob->m_wAllignment]);
			cTEXT::Put(irxpos+90,height*4,YELLOW, g_strMonsterGrade[lpJob->m_wMonsterLevel]);
			
			cTEXT::Put(ilxpos+70,height*5,YELLOW, _ms("Step %d",lpJob->m_wVariation));
			cTEXT::Put(icxpos+90,height*5,YELLOW, g_strBattleInclination[lpJob->m_wBattleInclination]);	
			cTEXT::Put(200		,height*7,YELLOW, _ms("%d times",lpJob->m_bf3RequireRebirthCountForTame));
			
			cDRAW::Box(_WHITE, ilxpos,	   height*8+1, ilxpos+10 , height*9-5);
			cDRAW::Box(_WHITE, ilxpos+100, height*8+1, ilxpos+110, height*9-5);
			cTEXT::Put(ilxpos + 270, height*8,YELLOW,_ms("%d",lpJob->m_bf15TameResistanceValue));
			cDRAW::Box(_WHITE, irxpos, height*8+1, irxpos+10, height*9-5);
			
			if(lpJob->m_bf1IsZinBoss)
				cDRAW::Fill(_LTYELLOW, ilxpos, height*8+1, ilxpos+10, height*9-5);
			
			if(lpJob->m_bf1IsEnableTameMonster)
				cDRAW::Fill(_LTYELLOW, ilxpos+100, height*8+1, ilxpos+110,height *9-5);	
			
			if(lpJob->m_bf1IsReviseIntCalc)                                                                                                         
				cDRAW::Fill(_LTYELLOW, irxpos, height*8+1, irxpos+10, height*9-5);
			
			cTEXT::Put(ilxpos	 ,height*10,WHITE,"force:"); cTEXT::Put(ilxpos+145,height*10,WHITE,"health: ");
			cTEXT::Put(ilxpos+290,height*10,WHITE,"Knowledge: "); cTEXT::Put(ilxpos+435,height*10,WHITE,"un: ");
			cTEXT::Put(ilxpos	 ,height*11,WHITE,"Agility: "); cTEXT::Put(ilxpos+145,height*11,WHITE,"Charisma: ");
			cTEXT::Put(ilxpos+290,height*11,WHITE,"wisdom: "); cTEXT::Put(ilxpos+435,height*11,WHITE,"level: ");
			
			cTEXT::Put(ilxpos+ 70,height*10,YELLOW,_ms("%d",lpJob->m_sStrength)); cTEXT::Put(ilxpos+215,height*10,YELLOW,_ms("%d",lpJob->m_sConstitution));
			cTEXT::Put(ilxpos+360,height*10,YELLOW,_ms("%d",lpJob->m_sIntelligence)); cTEXT::Put(ilxpos+505,height*10,YELLOW,_ms("%d",lpJob->m_sLuck));
			cTEXT::Put(ilxpos+ 70,height*11,YELLOW,_ms("%d",lpJob->m_sAgility)); cTEXT::Put(ilxpos+215,height*11,YELLOW,_ms("%d",lpJob->m_sCharisma));
			cTEXT::Put(ilxpos+360,height*11,YELLOW,_ms("%d",lpJob->m_sWisdom)); cTEXT::Put(ilxpos+505,height*11,YELLOW,_ms("%d.%.3d %%",lpJob->m_wBasicPowerLevelFactor/1000,lpJob->m_wBasicPowerLevelFactor%1000));
			
			cTEXT::Put(ilxpos	 ,height*13,WHITE,"Minimum damage: "); cTEXT::Put(ilxpos+145,height*13,WHITE,"Level factor: ");
			cTEXT::Put(ilxpos+290,height*13,WHITE,"Max Damage: "); cTEXT::Put(ilxpos+435,height*13,WHITE,"Level factor: ");
			cTEXT::Put(ilxpos	 ,height*14,WHITE,"Basic Defense: "); cTEXT::Put(ilxpos+145,height*14,WHITE,"Level factor: ");
			cTEXT::Put(ilxpos+290,height*14,WHITE,"Movement speed: "); cTEXT::Put(ilxpos+435,height*14,WHITE,"Attack time: ");
			cTEXT::Put(ilxpos+  0,height*15,WHITE,"Critical Shot: "); cTEXT::Put(ilxpos+145,height*15,WHITE,"Crushblow: ");
			cTEXT::Put(ilxpos+290,height*15,WHITE,"Hit correction: "); cTEXT::Put(ilxpos+435,height*15,WHITE,"Avoidance correction: ");
			cTEXT::Put(ilxpos+  0,height*16,WHITE,"Blocking probability: ");
			
			cTEXT::Put(ilxpos+ 70,height*13,YELLOW,_ms("%d",lpJob->m_sMinDamage));
			cTEXT::Put(ilxpos+215,height*13,YELLOW,_ms("%d.%.2d",lpJob->m_wMinDamageLevelFactor/100,lpJob->m_wMinDamageLevelFactor%100));
			cTEXT::Put(ilxpos+360,height*13,YELLOW,_ms("%d",lpJob->m_sMaxDamage));
			cTEXT::Put(ilxpos+505,height*13,YELLOW,_ms("%d.%.2d",lpJob->m_wMaxDamageLevelFactor/100,lpJob->m_wMaxDamageLevelFactor%100));
			
			cTEXT::Put(ilxpos+ 70,height*14,YELLOW,_ms("%d",lpJob->m_sDefensivePower));
			cTEXT::Put(ilxpos+215,height*14,YELLOW,_ms("%d.%.2d",lpJob->m_wDefensePowerLevelFactor/100,lpJob->m_wDefensePowerLevelFactor%100));
			cTEXT::Put(ilxpos+360,height*14,YELLOW,_ms("%d Pixel",lpJob->m_wMoveSpeed));
			cTEXT::Put(ilxpos+505,height*14,YELLOW,_ms("%d.%.2d seconds",lpJob->m_wAttackSpeed/100,lpJob->m_wAttackSpeed%100));
			
			cTEXT::Put(ilxpos+ 70,height*15,YELLOW,_ms("%d %%",lpJob->m_wCriticalShot));
			cTEXT::Put(ilxpos+215,height*15,YELLOW,_ms("%d %%",lpJob->m_wCrushBlow));
			
			if (lpJob->m_sCorrectHit < 0)
				cTEXT::Put(ilxpos+360,height*15,YELLOW,_ms("-%d.%.2d",-lpJob->m_sCorrectHit/100,-lpJob->m_sCorrectHit%100));
			else
				cTEXT::Put(ilxpos+360,height*15,YELLOW,_ms("%d.%.2d",lpJob->m_sCorrectHit/100,lpJob->m_sCorrectHit%100));
			
			if (lpJob->m_sCorrectDodge<0)	
				cTEXT::Put(ilxpos+505,height*15,YELLOW,_ms("-%d.%.2d %%",-lpJob->m_sCorrectDodge/100,-lpJob->m_sCorrectDodge%100));
			else
				cTEXT::Put(ilxpos+505,height*15,YELLOW,_ms("%d.%.2d %%",lpJob->m_sCorrectDodge/100,lpJob->m_sCorrectDodge%100));
			
			cTEXT::Put(ilxpos+ 70,height*16,YELLOW,_ms("%d %%",lpJob->m_wBlocking));
			
			// 저항	
			cDRAW::Box(_WHITE, 5, height * 17 + 5, 585, height * 24 + 1);
			
			cTEXT::Put(ilxpos	 ,height*18,WHITE,"Curse series: "); cTEXT::Put(ilxpos+145,height*18,WHITE,"Status abnormality: ");
			cTEXT::Put(ilxpos+290,height*18,WHITE,"Decreased ability: "); cTEXT::Put(ilxpos+435,height*18,WHITE,"blind: ");
			cTEXT::Put(ilxpos	 ,height*19,WHITE,"addiction: "); cTEXT::Put(ilxpos+145,height*19,WHITE,"sleep: ");
			cTEXT::Put(ilxpos+290,height*19,WHITE,"cold: "); cTEXT::Put(ilxpos+435,height*19,WHITE,"Freeze: ");
			cTEXT::Put(ilxpos	 ,height*20,WHITE,"rigid: "); cTEXT::Put(ilxpos+145,height*20,WHITE,"Petrification: ");
			cTEXT::Put(ilxpos+290,height*20,WHITE,"confusion: "); cTEXT::Put(ilxpos+435,height*20,WHITE,"Charming: ");
			cTEXT::Put(ilxpos	 ,height*21,WHITE,"critical hit: "); cTEXT::Put(ilxpos+145,height*21,WHITE,"Final blow: ");
			
			cTEXT::Put(ilxpos+ 70,height*18,YELLOW,_ms("%d %%",lpJob->m_sCurseResistance));
			cTEXT::Put(ilxpos+215,height*18,YELLOW,_ms("%d %%",lpJob->m_sBadStatusResistance));
			cTEXT::Put(ilxpos+360,height*18,YELLOW,_ms("%d %%",lpJob->m_sDeclinePowerResistance));
			cTEXT::Put(ilxpos+505,height*18,YELLOW,_ms("%d %%",lpJob->m_sBlindResistance));
			cTEXT::Put(ilxpos+ 70,height*19,YELLOW,_ms("%d %%",lpJob->m_sPoisonResistance));
			cTEXT::Put(ilxpos+215,height*19,YELLOW,_ms("%d %%",lpJob->m_sSleepResistance));
			cTEXT::Put(ilxpos+360,height*19,YELLOW,_ms("%d %%",lpJob->m_sColdResistance));
			cTEXT::Put(ilxpos+505,height*19,YELLOW,_ms("%d %%",lpJob->m_sFreezeResistance));
			cTEXT::Put(ilxpos+ 70,height*20,YELLOW,_ms("%d %%",lpJob->m_sStunResistance));
			cTEXT::Put(ilxpos+215,height*20,YELLOW,_ms("%d %%",lpJob->m_sStoneResistance));
			cTEXT::Put(ilxpos+360,height*20,YELLOW,_ms("%d %%",lpJob->m_sConfuseResistance));
			cTEXT::Put(ilxpos+505,height*20,YELLOW,_ms("%d %%",lpJob->m_sCharmingResistance));
			cTEXT::Put(ilxpos+ 70,height*21,YELLOW,_ms("%d %%",lpJob->m_sCriticalResistance));
			cTEXT::Put(ilxpos+215,height*21,YELLOW,_ms("%d %%",lpJob->m_sCrushResistance));
			
			cTEXT::Put(ilxpos	 ,height*23,WHITE,"Flame: ");		
			cTEXT::Put(ilxpos+100,height*23,WHITE,"water: ");
			cTEXT::Put(ilxpos+195,height*23,WHITE,"wind: ");		
			cTEXT::Put(ilxpos+290,height*23,WHITE,"light: ");
			cTEXT::Put(ilxpos+385,height*23,WHITE,"Earth: ");
			cTEXT::Put(ilxpos+480,height*23,WHITE,"dark: ");
			
			cTEXT::Put(ilxpos+ 40,height*23,YELLOW,_ms("%d %%",lpJob->m_sFireResistance));
			cTEXT::Put(ilxpos+140,height*23,YELLOW,_ms("%d %%",lpJob->m_sWaterResistance));
			cTEXT::Put(ilxpos+235,height*23,YELLOW,_ms("%d %%",lpJob->m_sWindResistance));
			cTEXT::Put(ilxpos+330,height*23,YELLOW,_ms("%d %%",lpJob->m_sLightResistance));
			cTEXT::Put(ilxpos+425,height*23,YELLOW,_ms("%d %%",lpJob->m_sEarthResistance));
			cTEXT::Put(ilxpos+520,height*23,YELLOW,_ms("%d %%",lpJob->m_sDarkResistance));
			
			// 스킬
			cDRAW::Box(_WHITE, 5, height * 24 + 5, 585, height * 36 + 1);
			
			cTEXT::Put(ilxpos	 ,height*25,WHITE,"Skill/Magic");		
			
			int icount = 0;
			for(int i = 0; i < dMONSTER_ABILITY_COUNT; ++i)
			{
				cABILITY	*lpAbility	=	(cABILITY*)&lpJob->m_aAbility[i];
				if	(lpAbility->m_wSkill	==	0xffff)
					continue;
				
				cSKILL	*lpSkill	=	&g_aSkill[lpAbility->m_wSkill];
				
				cTEXT::Put(ilxpos, height * (26 + icount),YELLOW,lpSkill->m_strName);
				cTEXT::Put(ilxpos + 200,height * (26 + icount),YELLOW,_ms("%d+Lv/%d.%.2d",lpAbility->m_sLevel,lpJob->m_aLevelForAddSkillLevel[i]/100,lpJob->m_aLevelForAddSkillLevel[i]%100));
				
				icount++;
			}
		}
		else
		{
			int height = 15, lxpos  = 10,	rxpos = 210;
			
			cDRAW::Box(_WHITE, 5, 5, 585, height * 36 + 1);	
			
			cTEXT::Put(lxpos, height * 1,WHITE, "experience level x ");
			cTEXT::Put(lxpos, height * 3,WHITE, "gold "); cTEXT::Put(lxpos + 120, height * 3,WHITE, "+ level x ");
			cTEXT::Put(lxpos, height * 5,WHITE, "gold drop probability");
			cTEXT::Put(lxpos, height * 7,WHITE, "drop item information");
			
			
			cTEXT::Put(lxpos + 100, height * 1,YELLOW, _ms("%d.%.2d",lpJob->m_dwDeathExp/100,lpJob->m_dwDeathExp%100));
			cTEXT::Put(lxpos + 35 , height * 3,YELLOW, _ms("%d",lpJob->m_wGold));
			cTEXT::Put(lxpos + 180, height * 3,YELLOW, _ms("%d.%.2d",lpJob->m_wGoldPerLevel/100,lpJob->m_wGoldPerLevel%100));
			cTEXT::Put(lxpos + 100, height * 5,YELLOW, _ms("%d %%",lpJob->m_wMoneyDroppingRate));
			
			for (int i = 0; i<dDROPPING_ITEM_COUNT;i++)
			{
				int	iItemType	=	lpJob->m_aDroppingItem[i].m_wItemType;
				
				if (iItemType	!=	0xffff)
				{
					int	iValue	=	lpJob->m_aDroppingItem[i].m_dwDroppingRate;
					
					cTEXT::Put(lxpos      ,height * (9 + i),YELLOW, _ms(g_strItemCategory[iItemType]));	
					cTEXT::Put(lxpos + 100,height * (9 + i),YELLOW, _ms("%d.%.2d%",iValue/100,iValue%100));
					cTEXT::Put(lxpos + 160,height * (9 + i),YELLOW, _ms("%d EA",lpJob->m_aDroppingItem[i].m_bf8Count));
				}
			}
			
		}
	}
	s_dibJobData.Draw();
}	//	cJOB_LIST::DrawJobData(HWND hDlg)

//
//	dib윈도우에 내용들 그려주기
void
cJOB_LIST::Draw(HWND hDlg)
{
	DrawBodyList(hDlg);
	DrawJobList(hDlg);
	DrawJobData(hDlg);
}	//	cJOB_LIST::Draw(HWND hDlg)

//
//	몸통을 선택한다.
BOOL
cJOB_LIST::SelectBody(HWND hwnd,int _iX,int _iY)
{
	int	j;
	int	index	=	s_sbBodyList.m_iWidthPos;

	int	x		=	(s_dibBodyList.width()-s_iBodyPerWindow*(s_iBodyWidth+2))/2;

	for (j=0;j<s_iBodyPerWindow;j++)
	{
		cRECT	rect;

		rect.Set(x,0,x+s_iBodyWidth+2,s_dibBodyList.height());

		if (rect.isIN(_iX,_iY))
		{
			if (s_aSelectBody[s_iSelectBodyGroup]	==	index)	return	FALSE;

			s_aSelectBody[s_iSelectBodyGroup]	=	index;
			return	TRUE;
		}
		index++;

		if (index >= g_BodyList.m_iBodyCount)	break;

		x	+=	(s_iBodyWidth+2);
	}

	return	FALSE;
}	//	cJOB_LIST::SelectBody(HWND hwnd,int x,int y)

//
//
void
cJOB_LIST::UpdateList(HWND hDlg)
{
//	직업 리스트에 추가
	s_listJob.ResetItems();

	for (int i=0;i<dMAX_JOB_COUNT;i++)
	{
		cJOB	*lpJob	=	g_denJob.getJob(i);

		if (lpJob)
		{
			if (lpJob->m_wType	==	s_iSelectBodyGroup)
			{
				if (lpJob->m_wBody	==	g_BodyList.getBody(s_aSelectBody[s_iSelectBodyGroup]))
				{
					s_listJob.AddItem(0,_ms("[%.3d]%s(%s)",lpJob->m_iSerial,lpJob->m_strName,g_strMonsterLevel[lpJob->m_wMonsterLevel]),lpJob->m_iSerial);
					s_listJob.IncreaseLineCounter();
				}
			}
		}
	}

//	정렬
//	s_listJob.Sort(0);
	s_listJob.Select(0);

	if (s_listJob.m_iLineCounter	==	0)
	{
		EnableWindow(GetDlgItem(s_hWnd,IDC_REMOVE),FALSE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_EDIT),FALSE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_COPY),FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(s_hWnd,IDC_REMOVE),TRUE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_EDIT),TRUE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_COPY),TRUE);
	}
}	//	cJOB_LIST::UpdateList(HWND hDlg)

//
//	영웅 추가 대화상자를 초기화 한다.
void
cJOB_LIST::AddJob(HWND hDlg)
{
	cJOB	*lpJob	=	cSET_JOB::NewJob(hDlg,s_iSelectBodyGroup,s_aSelectBody[s_iSelectBodyGroup]);

	if (!lpJob)	return;

	if (g_denJob.addJob(lpJob)	==	NULL)	return;

	int	iIndex	=	g_BodyList.getBodyIndex(lpJob->m_wBody);

	if (iIndex	!=	s_aSelectBody[s_iSelectBodyGroup])	
	{
		s_aSelectBody[s_iSelectBodyGroup]	=	iIndex;
		s_sbBodyList.correctWidthPossition(s_aSelectBody[s_iSelectBodyGroup]);
	}

	UpdateList(hDlg);
//	선택
	s_listJob.select(0,lpJob->m_strName);

	Draw(hDlg);

}	//	cJOB_LIST::InitAddJob(HWND hDlg)


//
//	영웅 추가 대화상자를 초기화 한다.
void
cJOB_LIST::EditJob(HWND hDlg)
{
	int iSelectJob	=	s_listJob.GetSelectData(0);

	if (iSelectJob	==	0x7fffffff)	return;

	cJOB	*lpOldJob	=	g_denJob.getJob(iSelectJob);

	if (!lpOldJob)	return;

	int Index = s_listJob.GetSelectLine();

	cJOB	*lpJob	=	cSET_JOB::NewJob(hDlg,s_iSelectBodyGroup,s_aSelectBody[s_iSelectBodyGroup],iSelectJob);

	if (!lpJob)	return;

	lpOldJob->copy(lpJob);

	UpdateList(hDlg);
//	선택
	s_listJob.Select(Index);	

	Draw(hDlg);

}	//	cJOB_LIST::InitAddJob(HWND hDlg)

void
cJOB_LIST::ChangeBodyGroup(HWND hDlg)
{
	HWND	hTab	=	GetDlgItem(hDlg,IDC_TAB_BODY_GROUP);
	int iSelectTab	=	TabCtrl_GetCurSel(hTab);

	if (iSelectTab+1==	s_iSelectBodyGroup)	return;

	s_iSelectBodyGroup	=	iSelectTab+1;

	s_sbBodyList.SetWidth(g_BodyList.m_iBodyCount,s_iBodyPerWindow);
	s_sbBodyList.SetWidthPos(s_aScrollBarPos[s_iSelectBodyGroup]);
	s_sbBodyList.correctWidthPossition(s_aSelectBody[s_iSelectBodyGroup]);


	UpdateList(hDlg);
	s_listJob.Select(0);

	Draw(hDlg);
}

void
cJOB_LIST::RemoveSelectJob(HWND hDlg)
{
	if (s_listJob.m_iLineCounter	==	0)	return;

	if (cMSG::YESNO(dMSG_REMOVE_SELECT_JOB,dMSG_DO_YOU_WNAT_REMOVE_SELECT_JOB)==IDNO)	return;

	int	iJob	=	s_listJob.GetSelectData(0);

	g_denJob.remove(iJob);

	iJob		=	s_listJob.m_iSelect;

	UpdateList(hDlg);

	if (iJob	>=	s_listJob.m_iLineCounter)	iJob--;

	s_listJob.Select(iJob);

	Draw(hDlg);
}

void
cJOB_LIST::CopySelectJob(HWND hDlg)
{
	if (s_listJob.m_iLineCounter	==	0)	return;

	int	iJob	=	s_listJob.GetSelectData(0);

	cJOB	*lpJob	=	g_denJob.getJob(iJob);

	if (!lpJob)	return;

	int		iIndex	=	2;
	char	strName[32],strTempName[32];

	strcpy(strTempName,lpJob->m_strName);

	strTempName[28]	=	0;

	while(1)
	{
		strcpy(strName,_ms("%s[%d]",strTempName,iIndex++));

		if (g_denJob.getJob(strName)	==	NULL)	break;
	}

	lpJob	=	g_denJob.addJob(lpJob);

	strcpy(lpJob->m_strName,strName);

	UpdateList(hDlg);
//	s_listJob.select(0,lpJob->m_strName);

	Draw(hDlg);
}


//
//	유니크 아이템 스크립트 문장(?)을 해석 한다.
BOOL
cJOB_LIST::DecodeMonsterScriptFile(HWND _hDlg)
{
	char	strFn[512]	=	"";
	char	strFolder[512] = "";

	if (!cFILE::LoadBox(strFn, "Text file {*.txt}\0*.txt\0\0", "txt", NULL, "Load unique item script data file"))
	{
		SetLogFolder();
		return	FALSE;
	}

	if (g_denJob.m_iCount > 0)
		if (cMSG::YESNO(_ms("Decoding the '%s' file.",strFn),"Do you want to initialize the existing monster data before decoding?")==IDYES)
			g_denJob.reset();

	int	iCount	=	g_denJob.m_iCount;
/*
	for (int i=0;i<iCount;i++)
		if (g_denJob.m_aJobList[i].m_wType	==	dJOB_GROUP_MONSTER)
			g_denJob.remove(i);
*/
	if (!DecodeMonsterScriptFile(_hDlg,strFn))	return	FALSE;


	return	TRUE;
}	//	cJOB_LIST::DecodeUniqueScript(HWND _hDlg)

int	l_iIncorrectMonsterSkillNameCount	=	0;

//
//	일반 아이템 스크립트 문장(?)을 해석 한다.
BOOL
cJOB_LIST::DecodeMonsterScriptFile(HWND _hDlg,char *_strFName)
{
	cDECODER	decoder;

	if	(!decoder.Upload(_strFName,NULL))
		return	FALSE;

	BOOL	isCry		=	TRUE;
	int		iErrorCount	=	0,iCount	=	g_denJob.m_iCount;

	l_iIncorrectMonsterSkillNameCount	=	0;

	while(1)
	{
		char	*strOneLine	=	decoder.GetStream(13);

		if	(!strOneLine)
			break;

		if	(!DecodeMonsterScript(strOneLine,isCry))
		{
			iErrorCount++;
		
			if	(iErrorCount ==	3 )
			{
				cMSG::Put("Item data file analysis","Further errors will not be displayed.");
				isCry	=	FALSE;
			}

			continue;
		}

		if	(s_makeJob.m_iSerial	==	0xffff)
			continue;

//		g_aBasicItem[iAddCount].m_wEquippedShape	=	s_makeItem.m_wEquippedShape;iAddCount++;continue;

		if	(!g_denJob.addJob(&s_makeJob))	
		{
			iErrorCount++;
			continue;
		}
	}

//	ResetItemListBox(_hDlg,TRUE);

//	DrawItemList(_hDlg);

	cMSG::Put("Monster data file analysis results","%d monsters were added.\n\nProblems occurred with %d monsters.",g_denJob.m_iCount-iCount,iErrorCount);

	if	(l_iIncorrectMonsterSkillNameCount)
		cMSG::Put("Monster data file analysis results","There are %d incorrect skills.\n\nPlease check the result.txt file.",l_iIncorrectMonsterSkillNameCount);

	return	TRUE;
}	//	cJOB_LIST::DecodeUniqueScript(HWND _hDlg)

//
//	스크립트 문장(?)을 해석 한다.
BOOL
cJOB_LIST::DecodeMonsterScript(char *_strScript,BOOL _isCry)
{
	cDECODER	decoder;

	s_makeJob.reset();

	if	(strlen(_strScript)	<=	80)
		return	TRUE;

	decoder.UploadBuffer(_strScript,NULL);

	char	*lpStream;
	int		iValidCount	=	0;
	int		iGrade		=	0;

	while(1)
	{	
		lpStream	=	decoder.GetStream('	');

		if (!lpStream)
		{
			iValidCount++;

			if (iValidCount	>=	100)	break;

			iGrade++;

			continue;
		}

		switch(iGrade)
		{
			case	0	:	break;	//	인덱스 무시~
			case	1	:	//	타입
			{
				for (int i=0;;i++)
				{
					if (stricmp(g_strJobCategory[i],"end"	)==0)	break;
					if (stricmp(g_strJobCategory[i],lpStream)==0)
					{
						s_makeJob.m_wKind	=	i;
						break;
					}
				}
				break;		
			}
			case	2	:	//	몸뚱아리
				s_makeJob.m_wBody	=	atoi(lpStream)+200;
				break;

			case	3	:	//	이름
				strcpy(s_makeJob.m_strName,lpStream);
				break;

			case	4	:	break;	//	영문 이름
			case	5	:	break;	//	키

			case	6	:	//	팔레트
				s_makeJob.m_wPalette	=	atoi(lpStream);
				break;	

			case	7	:	break;	//	설명
			case	8	:	//	비고
			{
				for (int i=0;;i++)
				{
					if (stricmp(g_strMonsterLevel[i],"end"	)==0)	break;
					if (stricmp(g_strMonsterLevel[i],lpStream)==0)
					{	s_makeJob.m_wMonsterLevel	=	i;
						break;
					}
				}
				break;		
			}

			case	9	:	//	힘
				s_makeJob.m_sStrength	=	atoi(lpStream);
				break;
			case	10	:	//	민첩성
				s_makeJob.m_sAgility	=	atoi(lpStream);
				break;
			case	11	:	//	건강
				s_makeJob.m_sConstitution=	atoi(lpStream);
				break;
			case	12	:	//	지능
				s_makeJob.m_sIntelligence=	atoi(lpStream);
				break;
			case	13	:	//	지혜
				s_makeJob.m_sWisdom		=	atoi(lpStream);
				break;
			case	14	:	//	카리스마
				s_makeJob.m_sCharisma	=	atoi(lpStream);
				break;
			case	15	:	//	운
				s_makeJob.m_sLuck		=	atoi(lpStream);
				break;

			case	16	:	break;	//	∑
			
			case	17	:	//	레벨별  기본 능력치 증가
				s_makeJob.m_wBasicPowerLevelFactor=	StringToNumber(lpStream,10,3);
				break;

			case	18	:	break;	//	∑

			case	19	:	//	기본 체력
				s_makeJob.m_iMaxHP	=	atoi(lpStream)*100;
				break;

			case	20	:	//	최대 체력 계산 인자
				s_makeJob.m_wCorrectMaxHPFactor	=	StringToNumber(lpStream,10000,1);
				break;
			case	21	:	//	최대 체력 계산 건강 인자
				s_makeJob.m_wCorrectMaxHPConstitutionFactor	=	StringToNumber(lpStream,6500,1);
				break;
			case	22	:	//	최대 CP
				s_makeJob.m_iMaxCP	=	atoi(lpStream)*100;
				break;
			case	23	:	//	최소 데미지
				s_makeJob.m_sMinDamage	=	atoi(lpStream);
				break;
			case	24	:	//	최대 데미지
				s_makeJob.m_sMaxDamage	=	atoi(lpStream);
				break;
			case	25	:	//	최소 데미지 계산 인자
				s_makeJob.m_wMinDamageLevelFactor	=	StringToNumber(lpStream,10,2);
				break;
			case	26	:	//	최대 데미지 계산 인자
				s_makeJob.m_wMaxDamageLevelFactor	=	StringToNumber(lpStream,10,2);
				break;
			case	27	:	//	방어력
				s_makeJob.m_sDefensivePower	=	atoi(lpStream);
				break;

			case	28	:	//	레벨에 따른 방어력 보정
				s_makeJob.m_wDefensePowerLevelFactor=	StringToNumber(lpStream,10,2);
				break;

			case	29	:	//	이동 속도
				s_makeJob.m_wMoveSpeed	=	atoi(lpStream);
				break;

			case	30	:	//	공격 속도
				s_makeJob.m_wAttackSpeed=	StringToNumber(lpStream,10,2);
				break;

			case	31	:	//	크리티컬 샷 날릴 확률
				s_makeJob.m_wCriticalShot=	atoi(lpStream);
				break;

			case	32	:	//	크러쉬 블러우 날릴 확률
				s_makeJob.m_wCrushBlow	=	atoi(lpStream);
				break;
			case	33	:	//	명중률 보정
				s_makeJob.m_sCorrectHit	=	atoi(lpStream);
				break;

			case	34	:	//	회피율 보정
				s_makeJob.m_sCorrectDodge=	atoi(lpStream);
				break;

			case	35	:	//	블럭율
				s_makeJob.m_wBlocking	=	atoi(lpStream);
				break;

			case	36	:	//	불꽃 저항
				s_makeJob.m_sFireResistance	=	atoi(lpStream);
				break;
			case	37	:	//	물 저항
				s_makeJob.m_sWaterResistance=	atoi(lpStream);
				break;
			case	38	:	//	바람 저항
				s_makeJob.m_sWindResistance	=	atoi(lpStream);
				break;
			case	39	:	//	대지 저항
				s_makeJob.m_sEarthResistance=	atoi(lpStream);
				break;
			case	40	:	//	빛 저항
				s_makeJob.m_sLightResistance=	atoi(lpStream);
				break;
			case	41	:	//	어둠 저항
				s_makeJob.m_sDarkResistance	=	atoi(lpStream);
				break;

			case	42	:	//	최소 레벨
				s_makeJob.m_iLevel			=	atoi(lpStream);
				break;

			case	43	:	//	한계 레벨
				s_makeJob.m_wLimitLevel		=	atoi(lpStream);
				break;

			case	44	:	//	주는 경험치
			{
				s_makeJob.m_dwDeathExp		=	StringToNumber(lpStream,655,2);
				break;
			}

			case	45	:	
				s_makeJob.m_sSight			=	atoi(lpStream);
				break;//	시야

			case	46	:	//	장님 저항
				s_makeJob.m_sBlindResistance=	atoi(lpStream);
				break;

			case	47	:	//	중독 저항
				s_makeJob.m_sPoisonResistance=	atoi(lpStream);
				break;

			case	48	:	//	잠 저항
				s_makeJob.m_sSleepResistance=	atoi(lpStream);
				break;

			case	49	:	//	콜드 저항
				s_makeJob.m_sColdResistance=	atoi(lpStream);
				break;

			case	50	:	//	프리즈 저항
				s_makeJob.m_sFreezeResistance=	atoi(lpStream);
				break;

			case	51	:	//	스턴 저항
				s_makeJob.m_sStunResistance=	atoi(lpStream);
				break;

			case	52	:	//	석화 저항
				s_makeJob.m_sStoneResistance=	atoi(lpStream);
				break;

			case	53	:	//	혼란 저항
				s_makeJob.m_sConfuseResistance=	atoi(lpStream);
				break;

			case	54	:	//	유혹 저항
				s_makeJob.m_sCharmingResistance=	atoi(lpStream);
				break;

			case	55	:	//	상태이상 저항
				s_makeJob.m_sBadStatusResistance=	atoi(lpStream);
				break;

			case	56	:	//	능력치 저하 저항
				s_makeJob.m_sDeclinePowerResistance=	atoi(lpStream);
				break;

			case	57	:	//	저주 저항
				s_makeJob.m_sCurseResistance=	atoi(lpStream);
				break;

			case	58	:	//	크티컬 저항
				s_makeJob.m_sCriticalResistance=	atoi(lpStream);
				break;

			case	59	:	//	크러쉬 저항
				s_makeJob.m_sCrushResistance	=	atoi(lpStream);
				break;

			case	60	:
			{
				int	iAllign	=	0;

				for (int i=0;i<6;i++)
					if (stricmp(g_strAllignment[i],lpStream)==0)
						iAllign	=	i;

				s_makeJob.m_wAllignment	=	iAllign;

				switch(iAllign)
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

					default		:
						ERRMSG("Woooo!!","Unintelligible word!! in case 60 : %s",lpStream);
						break;
				}
				break;
			}

			case	61	:	//	스킬 1
			case	63	:	//	스킬 2
			case	65	:	//	스킬 3
			case	67	:	//	스킬 4
			case	69	:	//	스킬 5
			case	71	:	//	스킬 6
			case	73	:	//	스킬 7
			case	75	:	//	스킬 8
			case	77	:	//	스킬 9
			case	79	:	//	스킬 10
			{
				int	iSlot	=	(iGrade-61)/2;
				s_makeJob.m_aAbility[iSlot].m_wSkill		=	cSKILL::GetIndex(lpStream,16);

				if	(s_makeJob.m_aAbility[iSlot].m_wSkill	==	0xffff)
				{
					MSGOUT("No skill! Job [%s] skill name [%s]",s_makeJob.m_strName,lpStream);
					l_iIncorrectMonsterSkillNameCount++;
				}
				break;
			}

			case	62	:
			case	64	:
			case	66	:
			case	68	:
			case	70	:
			case	72	:
			case	74	:
			case	76	:
			case	78	:
			case	80	:
			{
				int	iSlot			=	(iGrade-62)/2;
				s_makeJob.m_aAbility[iSlot].m_wLevel		=	atoi(lpStream);
				lpStream	=	strstr(lpStream,"/");
				s_makeJob.m_aLevelForAddSkillLevel[iSlot]	=	StringToNumber(lpStream+1,100,2);
				break;
			}

			case	81	:	//	드랍 아이템 1
			{
				int	iKind	=	cBASIC_ITEM::GetItemKind(lpStream);
				if (iKind	==	0xffff)	ERRMSG("error!!","The type of %s's 1st drop item '%s' is unknown.",s_makeJob.m_strName,lpStream);
				s_makeJob.m_aDroppingItem[0].m_wItemType		=	iKind;
				break;
			}

			case	82	:
			{
				s_makeJob.m_aDroppingItem[0].m_dwDroppingRate	=	StringToNumber(lpStream,100,2);;
				break;
			}

			case	83	:	//	드랍 아이템 2
			{
				int	iKind	=	cBASIC_ITEM::GetItemKind(lpStream);
				if (iKind	==	0xffff)	ERRMSG("error!!","The type of %s's 1st drop item '%s' is unknown.",s_makeJob.m_strName,lpStream);
				s_makeJob.m_aDroppingItem[1].m_wItemType		=	iKind;
				break;
			}

			case	84	:
			{
				s_makeJob.m_aDroppingItem[1].m_dwDroppingRate	=	StringToNumber(lpStream,100,2);
				break;
			}

			case	85	:	//	드랍 아이템 3
			{
				int	iKind	=	cBASIC_ITEM::GetItemKind(lpStream);
				if (iKind	==	0xffff)	ERRMSG("error!!","The type of %s's 1st drop item '%s' is unknown.",s_makeJob.m_strName,lpStream);
				s_makeJob.m_aDroppingItem[2].m_wItemType		=	iKind;
				break;
			}

			case	86	:
			{
				s_makeJob.m_aDroppingItem[2].m_dwDroppingRate	=	StringToNumber(lpStream,100,2);;
				break;
			}

			case	87	:	//	드랍 아이템 4
			{
				int	iKind	=	cBASIC_ITEM::GetItemKind(lpStream);
				s_makeJob.m_aDroppingItem[3].m_wItemType		=	iKind;
				if (iKind	==	0xffff)	ERRMSG("error!!","The type of %s's 1st drop item '%s' is unknown.",s_makeJob.m_strName,lpStream);
				break;
			}

			case	88	:
			{
				s_makeJob.m_aDroppingItem[3].m_dwDroppingRate	=	StringToNumber(lpStream,100,2);;
				break;
			}

			case	89	:	//	드랍 아이템 5
			{
				int	iKind	=	cBASIC_ITEM::GetItemKind(lpStream);
				if (iKind	==	0xffff)	ERRMSG("error!!","The type of %s's 1st drop item '%s' is unknown.",s_makeJob.m_strName,lpStream);
				s_makeJob.m_aDroppingItem[4].m_wItemType		=	iKind;
				break;
			}

			case	90	:
			{
				s_makeJob.m_aDroppingItem[4].m_dwDroppingRate	=	StringToNumber(lpStream,100,2);;
				break;
			}

			case	91	:	//	드랍 아이템 6
			{
				int	iKind	=	cBASIC_ITEM::GetItemKind(lpStream);
				if (iKind	==	0xffff)	ERRMSG("error!!","The type of %s's 1st drop item '%s' is unknown.",s_makeJob.m_strName,lpStream);
				s_makeJob.m_aDroppingItem[5].m_wItemType		=	iKind;
				break;
			}

			case	92	:
			{
				s_makeJob.m_aDroppingItem[5].m_dwDroppingRate	=	StringToNumber(lpStream,100,2);;
				break;
			}

			case	93	:	//	골드
			{
				int	iStringLength				=	strlen(lpStream);
				int	iPlusLocate					=	0;

				for (int i=0;i<iStringLength;i++)
				{
					if (lpStream[i] == '+')
					{
						iPlusLocate	=	i;
						i			=	iStringLength;
					}
				}

				s_makeJob.m_wGold				=	atoi(lpStream);
				s_makeJob.m_wGoldPerLevel		=	StringToNumber(lpStream+iPlusLocate+1,655,2);
				break;
			}

			case	94	:	//	돈 떨어질 확률
				s_makeJob.m_wMoneyDroppingRate	=	atoi(lpStream);
				s_makeJob.m_iSerial				=	0;
				s_makeJob.m_wType				=	dJOB_GROUP_MONSTER;
				break;

			case	95	:	//	길들이기 유무
				if (atoi(lpStream))
					s_makeJob.m_bf1IsEnableTameMonster	=	TRUE;
				else
					s_makeJob.m_bf1IsEnableTameMonster	=	FALSE;
				break;

			case	96	:	//	길들이기 저항
				s_makeJob.m_bf15TameResistanceValue	=	atoi(lpStream);
				break;

			case	97	:	//	길들이기 저항
				s_makeJob.m_wVariation				=	atoi(lpStream);
				break;

		}

		iGrade++;
	}

	return	TRUE;
}	//	cSET_BASIC_ITEM::DecodeUniqueScript(HWND _hDlg)


LRESULT CALLBACK 
cJOB_LIST::s_procSetJobList(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{	switch(message)
	{	case WM_INITDIALOG	:
			InitJobList(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDOK);
			break;

		case WM_NOTIFY		:
		{
			NMHDR*	notyfyInfo	=	((NMHDR*)lParam);
			if (notyfyInfo->code ==	TCN_SELCHANGE)
			{
				switch(notyfyInfo->idFrom)
				{
					case	IDC_TAB_BODY_GROUP	:
						ChangeBodyGroup(hDlg);
						break;
					case	IDC_VIEW_DATA_TAB	:
						{
							HWND	hTab		=	GetDlgItem(hDlg,IDC_VIEW_DATA_TAB);
							s_iSelectJobDataTab =	TabCtrl_GetCurSel(hTab);
							DrawJobData(hDlg);
						}
						break;
				}
			}
			break;
		}

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {
				case	IDCANCEL		:
					EndDialog(hDlg,IDOK);
					break;

				case	IDCANCEL2		:
				{
					for (int i=0;i<dMAX_JOB_COUNT;i++)
					{
						cJOB	*lpJob	=	g_denJob.getJob(i);

						if (!lpJob)	break;

						lpJob->m_wCorrectMaxHPFactor	/=	10;
					}
					break;
				}
				case	IDCANCEL3		:
				{
					for (int i=0;i<dMAX_JOB_COUNT;i++)
					{
						cJOB	*lpJob	=	g_denJob.getJob(i);

						if (!lpJob)	break;

						lpJob->m_wCorrectMaxHPConstitutionFactor	/=	10;
					}
					break;
				}

				case	IDC_ADD			:
				{
					AddJob(hDlg);
					break;
				}

				case	IDC_IMPORT		:
				{
					DecodeMonsterScriptFile(hDlg);
					break;
				}

				case	IDC_EDIT		:
				{
					EditJob(hDlg);
					break;
				}

				case	IDC_REMOVE		:
				{
					RemoveSelectJob(hDlg);
					break;
				}

				case	IDC_COPY		:
				{
					CopySelectJob(hDlg);
					break;
				}
			}
			break;
	}

	return (0L);
}

LRESULT CALLBACK 
cJOB_LIST::s_procDibBodyList(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	switch(uMsg)
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
			if (s_sbBodyList.HScroll(wParam))
			{
				s_aScrollBarPos[s_iSelectBodyGroup]	=	s_sbBodyList.GetWidthPos();
				Draw(hwnd);
			}
			break;
		}

		case WM_LBUTTONDOWN	:
		{
			if(SelectBody(hwnd,LOWORD(lParam),HIWORD(lParam)))
			{
				UpdateList(hwnd);
				s_listJob.Select(0);

				Draw(hwnd);
			}
			break;
		}

		default				:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}

LRESULT CALLBACK 
cJOB_LIST::s_procDibJobData(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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