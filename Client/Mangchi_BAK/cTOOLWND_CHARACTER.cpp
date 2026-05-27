#include	"cDRAW.H"
#include	"cTEXT.H"
#include	"cTOOLWND.H"
#include	"cMAIN.H"
#include	"cOBJECT.H"
#include	"cBUILDING.H"
#include	"cAREA.H"
#include	"cMAPWND.H"
#include	"cMESSAGE.H"
#include	"cMAP.H"
#include	"cSET.H"

#define		IDM_ADD_HERO			1000
#define		IDM_EDIT_CHARACTER		1001
#define		IDM_REMOVE_CHARACTER	1002
#define		IDM_SET_EVENT			1005
#define		IDM_ADD_MONSTER			1003
#define		IDM_ADD_NPC				1004

/*
int
GetCorrectExpByLevelGap(int iSharedExp,int _iPlayerLevel,int _iMonsterLevel)
{
	//	→ 레벨이 같거나 몬스터 레벨이 더 높다.
	//	보정 없음.
	int	iLevelGap	=	_iPlayerLevel-_iMonsterLevel;
	
	if	(iLevelGap	<	0)
		iLevelGap	=	-iLevelGap;
	
	//	획득 경험치 = (파티 경험치 + 파티 보너스 경험치) * ((50-(자신의 레벨- 몬스터 레벨))/50)		→ 자신의 레벨이 더 높은 경우
	//	획득 경험치 = (파티 경험치 + 파티 보너스 경험치) * (1/(자신의 레벨- 몬스터 레벨))			→ 자신의 레벨이 50이상 높은 경우
	int			iCorrectValue=	75;
	LONGLONG	llExp		=	iSharedExp;
	
	if	(iLevelGap	>=	iCorrectValue)
		llExp		=	llExp/iLevelGap;
	else
		if	(iLevelGap	>	0)
			llExp		=	llExp*(iCorrectValue-iLevelGap)/iCorrectValue;
		
	int	iHighLevelFactor	=	_iPlayerLevel-99;

	
	if	(_iPlayerLevel >=800)
		llExp	=	llExp*(100-(iHighLevelFactor*100/910)     )*75/(_iPlayerLevel- 425)/100;
	else
	if	(_iPlayerLevel >=700)
		llExp	=	llExp*(100-(iHighLevelFactor*100/915)     )*75/(_iPlayerLevel- 425)/100;
	else
	if	(_iPlayerLevel >=600)
		llExp	=	llExp*(100-(iHighLevelFactor*100/920)     )*75/(_iPlayerLevel- 425)/100;
	else
	if	(_iPlayerLevel >=500)
		llExp	=	llExp*(100-(iHighLevelFactor*100/925)     )*75/(_iPlayerLevel- 425)/100;
	else
	if	(_iPlayerLevel >=400)
		llExp	=	llExp*(100-(iHighLevelFactor*100/930)     )/100;
	else
	if	(_iPlayerLevel >=300)
		llExp	=	llExp*(100-(iHighLevelFactor*100/940)     )/100;
	else
	if	(_iPlayerLevel >=250)
		llExp	=	llExp*(100-(iHighLevelFactor*100/990)     )/100;
	
	iSharedExp	=	(int)llExp;
	
	//500레벨 이상 경험치 획득 규칙 수정
	//		500~599 레벨 : 획득 경험치 * (1 - ((자신 레벨 - 99) / 925)) / 2 -> 획득 경험치 * (1 - ((자신 레벨 - 99) / 925)) * (75 / (레벨 - 425))
	//		600~699 레벨 : 획득 경험치 * (1 - ((자신 레벨 - 99) / 920)) / 3 -> 획득 경험치 * (1 - ((자신 레벨 - 99) / 920)) * (75 / (레벨 - 425))
	//		700~799 레벨 : 획득 경험치 * (1 - ((자신 레벨 - 99) / 915)) / 4 -> 획득 경험치 * (1 - ((자신 레벨 - 99) / 915)) * (75 / (레벨 - 425))
	//		800~999 레벨 : 획득 경험치 * (1 - ((자신 레벨 - 99) / 910)) / 5 -> 획득 경험치 * (1 - ((자신 레벨 - 99) / 910)) * (75 / (레벨 - 425))
	
	//		400~499 레벨 : 획득 경험치 * (1 - ((자신 레벨 - 99) / 930))						0.676344086	0.569892473
	//		500~599 레벨 : 획득 경험치 * (1 - ((자신 레벨 - 99) / 925)) / 2						0.283243243	0.22972973
	//		600~699 레벨 : 획득 경험치 * (1 - ((자신 레벨 - 99) / 920)) / 3						0.151811594	0.115942029
	//		700~799 레벨 : 획득 경험치 * (1 - ((자신 레벨 - 99) / 915)) / 4						0.08579235	0.058743169
	//		800~999 레벨 : 획득 경험치 * (1 - ((자신 레벨 - 99) / 910)) / 5						0.045934066	0.002197802
	
	return	max(iSharedExp,1);
}
*/
//
//	캐릭터 관리창 업데이트
//
void
cTOOLWND::UpdateCharacter()
{	
	if	(!IsCorrectMode(dWORK_CHARACTER))
		return;

	if	((LBUTTON || RBUTTON) && !client.isIN(Mouse.x,Mouse.y))
	{	//09.07.10  추가... 
		return;
	}
	
	if (LEFTDBLCLICK)
	{
		int	iSelect	=	GetCharacterIndex();

		if (iSelect	!=	0xffff)
		{	g_lpActmap->m_setCharacter.m_iSelectCharacter	=	iSelect;
			MAIN.ChangeMode(dWORK_CHARACTER);

			cCHARACTER	*lpCharacter	=	g_lpActmap->m_setCharacter.getSelectCharacter();

			if (cSET_CHARACTER::EditCharacter(hWND,&g_lpActmap->m_setCharacter))
			{
				g_lpActmap->LostVirgin();
				Draw();
			}


			return;
		}
	}

	if (TOUCHRBUTTON)
	{
		POINT	Pos;
		HMENU	hMenu	=	CreatePopupMenu();
		int		iSelect	=	GetCharacterIndex();

		AppendMenu(hMenu,MFT_STRING,IDM_ADD_MONSTER	,dMSG_ADD_MONSTER);
		AppendMenu(hMenu,MFT_STRING,IDM_ADD_NPC		,dMSG_ADD_NPC);

		if (iSelect	!=	0xffff)
		{
			MAIN.ChangeMode(dWORK_CHARACTER);

			AppendMenu(hMenu,MFT_SEPARATOR					,0,NULL);
			AppendMenu(hMenu,MFT_STRING,IDM_EDIT_CHARACTER	,dMSG_EDIT);
			AppendMenu(hMenu,MFT_STRING,IDM_REMOVE_CHARACTER,dMSG_REMOVE);
			AppendMenu(hMenu,MFT_STRING,IDM_SET_EVENT		,dMSG_SET_EVENT);
			AppendMenu(hMenu,MFT_SEPARATOR		,0					,NULL);						// 09.07.24 추가  -----------------
			AppendMenu(hMenu,MFT_STRING			,IDM_EDIT_ACTOR		,dMSG_ACTOR_SETTING);		// 09.07.24 추가 케릭터 데이터설정
			
			g_lpActmap->m_setCharacter.m_iSelectCharacter	=	iSelect;
			Draw();
		}

		GetCursorPos(&Pos);
		TrackPopupMenu(hMenu,TPM_RIGHTBUTTON|TPM_TOPALIGN|TPM_LEFTALIGN,Pos.x,Pos.y,0,hWND,NULL);
		DestroyMenu(hMenu);
	}

	if (TOUCHLBUTTON)
	{
		int	iSelect	=	GetCharacterIndex();

		if (iSelect	!=	0xffff)
		{	g_lpActmap->m_setCharacter.m_iSelectCharacter	=	iSelect;
			MAIN.ChangeMode(dWORK_CHARACTER);

			Draw();
		}
	}
}	//	cTOOLWND::UpdateMonster()

//
//	마우스 위치에 인덱스를 구한다.
//
int
cTOOLWND::GetCharacterIndex()
{
	if	(!IsCorrectMode(dWORK_CHARACTER))
		return	0xffff;

	int	x		=	2;
	int	y		=	g_iTBSize;
	int	iCount	=	CharacterPos;

	int	index		=	g_lpActmap->m_setCharacter.getIndex(CharacterPos);
	int	iBarHeight	=	72;

	for (int i=index;i	<	dJOB_COUNT && iCount < g_lpActmap->m_setCharacter.m_iCount;i++)
	{
		if (g_lpActmap->m_setCharacter.m_aCharacter[i].m_wSerial	!=	0xffff)
		{
			int	xp	=	x;
			int	yp	=	y+(iCount-CharacterPos)*iBarHeight;

			cRECT	SelectRect;

			SelectRect.Set(xp,yp,cDRAW::WIDTH-2	,yp+iBarHeight-1);

			if (SelectRect.isIN(Mouse.x,Mouse.y))	return	i;

			iCount++;
		}
	}

	return	0xffff;
}	//	cTOOLWND::GetCharacterIndex()

//
//	영웅관리창을 그려준다.
void
cTOOLWND::DrawCharacter()
{
	if (!IsCorrectMode(dWORK_CHARACTER))	return;

	cDRAW::Fill(_SKYBLUE);
	cDRAW::Box(0	,0,0,cDRAW::WIDTH,cDRAW::HEIGHT,2);

	int	x			=	2;
	int	y			=	g_iTBSize;
	int	iCount		=	CharacterPos;
	int	iBarHeight	=	72;//54;

	TPH				=	(cDRAW::HEIGHT - y) / iBarHeight;

	SetScrollBar(g_lpActmap->m_setCharacter.m_iCount,&CharacterPos);
//	SetScrollBar(lpArea->m_iCount-2,&AreaPos);

	int	index	=	g_lpActmap->m_setCharacter.getIndex(CharacterPos);

	char	*astrKeyMonster[]	=	{""," key"};
	char	*astrEventMonster[]	=	{"","[E]"};

	for (int i	=	index;i < dMAX_CHARACTER && iCount < g_lpActmap->m_setCharacter.m_iCount;i++)
	{
		if (g_lpActmap->m_setCharacter.m_aCharacter[i].m_wSerial	!=	0xffff)
		{
			cCHARACTER	*lpCharacter	=	&g_lpActmap->m_setCharacter.m_aCharacter[i];

			int	xp	=	x+1;
			int	yp	=	y+(iCount-CharacterPos)*iBarHeight;

			if	(yp+iBarHeight	>=	cDRAW::HEIGHT)
				break;

			char		*lpstrEvent		=	astrEventMonster[lpCharacter->isExistEvent()];

			cJOB		*lpJob			=	lpCharacter->getJob();

			int			iExp	=	0;
			int			iLevel	=	(lpCharacter->m_wMinLevel+lpCharacter->m_wMaxLevel)/2;
			int			iHP		=	0;

			{
				LONGLONG	llExp	=	lpJob->m_dwDeathExp;

				llExp	*=	(iLevel+4);
				llExp	*=	10;
				llExp	=	llExp*lpCharacter->m_wCorrectExp/100;
				iExp	=	(int)llExp/100;

				iExp	=	GetCorrectExpByLevelGap(iExp,iLevel);
			}
/*	{
				LONGLONG	llExp	=	lpJob->m_dwDeathExp;

				llExp	*=	(iLevel+4);
				llExp	*=	10;
				llExp	=	llExp*lpCharacter->m_wCorrectExp/100;
				llExp	=	llExp/100;

				iHP		=	GetCorrectExpByLevelGap2((int)llExp,iLevel);
			}*/
			{
				LONGLONG	llMaxHP			=	lpJob->m_iMaxHP;
				int			iConstitution	=	(int)lpJob->m_sConstitution*((iLevel-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;

				llMaxHP	=	lpJob->m_iMaxHP+iConstitution*lpJob->m_wCorrectMaxHPConstitutionFactor*10+iLevel*lpJob->m_wCorrectMaxHPFactor*10;
				llMaxHP	=	llMaxHP*lpCharacter->m_wCorrectHP/100;
				iHP		=	(int)llMaxHP/100;
			}

			char	strText[20];
			
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
			sprintf(strText, "%s.%s.%s", g_strMonsterConcept[iMagicGrade+1], g_strMonsterConcept[iPhysical+1], g_strMonsterConcept[iDefence+1]);

			if	(i	==	g_lpActmap->m_setCharacter.m_iSelectCharacter)
			{
				cDRAW::Fill	(0			,xp+2,yp+3	,cDRAW::WIDTH-3	,yp+iBarHeight-2);

				xp++;
				yp++;

				cDRAW::Fill	(_LTBLUE	,xp  ,yp+1	,cDRAW::WIDTH-4	,yp+iBarHeight-4);
				cDRAW::Box	(_BLUE		,xp  ,yp+1	,cDRAW::WIDTH-4	,yp+iBarHeight-4);

				xp		+=	38;
				cTEXT::Put	(xp+2,yp+3		,LTYELLOW,_ms("%s(%d)%s%s",lpCharacter->m_strName,g_lpActmap->m_setActor.getCharacterCount(lpCharacter->m_wSerial),astrKeyMonster[lpCharacter->m_bf1IsKeyMonster],lpstrEvent));
				cTEXT::Put	(xp+130,yp+3		,LTYELLOW, strText);
				cTEXT::Put	(xp+2,yp+3+16	,LTYELLOW,_ms("%s[%s]",lpCharacter->getJobName(),g_strMonsterLevel[lpJob->m_wMonsterLevel]));
				cTEXT::Put	(xp+2,yp+3+28	,LTYELLOW,_ms("%s %d~%d",dMSG_GENERATE_LEVEL,lpCharacter->m_wMinLevel,lpCharacter->m_wMaxLevel));

				if	(lpCharacter->m_wCorrectExp	!=	100)
					cTEXT::Put	(xp+2,yp+3+40	,LTGREEN,_ms("EP - %s",GetMoneyString(iExp)));
				else
					cTEXT::Put	(xp+2,yp+3+40	,LTYELLOW,_ms("EP - %s",GetMoneyString(iExp)));

				if	(lpCharacter->m_wCorrectHP	!=	100)
					cTEXT::Put	(xp+2,yp+3+52	,LTGREEN,_ms("HP - %s",GetMoneyString(iHP)));
				else
					cTEXT::Put	(xp+2,yp+3+52	,LTYELLOW,_ms("HP - %s",GetMoneyString(iHP)));
//				xp--;
//				yp--;
			}
			else
			{
				cDRAW::Fill	(0				,xp+2,yp+3	,cDRAW::WIDTH-3	,yp+iBarHeight-2);
				cDRAW::Fill	(_DEEPGRAY		,xp  ,yp+1	,cDRAW::WIDTH-5	,yp+iBarHeight-4);
				cDRAW::Box	(0				,xp  ,yp+1	,cDRAW::WIDTH-5	,yp+iBarHeight-4);

				xp		+=	38;

				cTEXT::Put	(xp+2,yp+3		,WHITE	,_ms("%s(%d)%s%s",lpCharacter->m_strName,g_lpActmap->m_setActor.getCharacterCount(lpCharacter->m_wSerial),astrKeyMonster[lpCharacter->m_bf1IsKeyMonster],lpstrEvent));
				cTEXT::Put	(xp+130,yp+3		,WHITE, strText);
				cTEXT::Put	(xp+2,yp+3+16	,WHITE	,_ms("%s[%s]",lpCharacter->getJobName(),g_strMonsterLevel[lpJob->m_wMonsterLevel]));
				cTEXT::Put	(xp+2,yp+3+28	,WHITE	,_ms("%s %d~%d",dMSG_GENERATE_LEVEL,lpCharacter->m_wMinLevel,lpCharacter->m_wMaxLevel));

				if	(lpCharacter->m_wCorrectExp	!=	100)
					cTEXT::Put	(xp+2,yp+3+40	,LTGREEN,_ms("EP - %s",GetMoneyString(iExp)));
				else
					cTEXT::Put	(xp+2,yp+3+40	,WHITE,_ms("EP - %s",GetMoneyString(iExp)));

				if	(lpCharacter->m_wCorrectHP	!=	100)
					cTEXT::Put	(xp+2,yp+3+52	,LTYELLOW,_ms("HP- %s",GetMoneyString(iHP)));
				else
					cTEXT::Put	(xp+2,yp+3+52	,WHITE,_ms("HP - %s",GetMoneyString(iHP)));
			}

			xp		-=	38;

			cDRAW::Fill	(_WHITE		,xp+3+36,yp+3+13	,cDRAW::WIDTH-8	,yp+3+13);
			
			int			iBody			=	lpCharacter->getBody();
			cANMQ		*lpBody			=	g_pBody[iBody];

			if	(g_aMonsterPalette[iBody].m_wCount)
				memcpy(lpBody->pPLT,&g_aMonsterPalette[iBody].m_pPalette[lpJob->m_wPalette],512);

			lpBody->FitPutColor	(xp+4,yp+3,36,iBarHeight-4,4,0);
			lpBody->FitPut		(xp+2,yp+2,36,iBarHeight-4,4);

			{
				int	iVariation	=	lpJob->m_wVariation;

				if	(iVariation)
				{
					g_apMonsterVariation[iBody][iVariation-1]->FitPutColor(xp+4,yp+3,36,iBarHeight-4,4,0);
					g_apMonsterVariation[iBody][iVariation-1]->FitPut		(xp+4,yp+3,36,iBarHeight-4,4,0);
				}
			}

			iCount++;
		}
	}
}	//	cTOOLWND::DrawCharacter()

//
//	팝업 메뉴
void
cTOOLWND::UpdateCharacterCommand(int menu)
{
	if (!IsCorrectMode(dWORK_CHARACTER))	return;

	switch(menu)
	{
		case	IDM_ADD_MONSTER	:
		{
			if (cSET_CHARACTER::AddCharacter(hWND,dJOB_GROUP_MONSTER,&g_lpActmap->m_setCharacter))
				g_lpActmap->LostVirgin();

			MAIN.Draw();
			break;
		}

		case	IDM_EDIT_ACTOR			:				// 09.07.24 케릭터 데이터설정!
		{
			SettingCharacter();

			break;
		}

		case	IDM_ADD_NPC		:
		{
			if (cSET_CHARACTER::AddCharacter(hWND,dJOB_GROUP_NPC,&g_lpActmap->m_setCharacter))
				g_lpActmap->LostVirgin();

			MAIN.Draw();
			break;
		}

		case	IDM_EDIT_CHARACTER	:
		{
			if (cSET_CHARACTER::EditCharacter(hWND,&g_lpActmap->m_setCharacter))	g_lpActmap->LostVirgin();

			MAIN.Draw();
			break;
		}

		case	IDM_REMOVE_CHARACTER	:
		{
			if(cMSG::YESNO(dMSG_REMOVE_SELECT_JOB,dMSG_DO_YOU_WNAT_REMOVE_SELECT_JOB)	==	IDYES)
			{
				g_lpActmap->LostVirgin();
				g_lpActmap->m_setCharacter.removeSelectCharacter();
				g_lpActmap->m_setActor.reBuild();
			}

			MAIN.Draw();
			break;
		}

		case	IDM_SET_EVENT			:
		{
			cCHARACTER	*lpCharacter	=	g_lpActmap->m_setCharacter.getSelectCharacter();
			cSET_CHARACTER::SetEvent(hWND,lpCharacter);
			break;
		}
	}
}	//	cTOOLWND::UpdateCharacterCommand(int menu)

void
cTOOLWND::SettingCharacter()								// 09.07.24 케릭터데이터설정
{
	
	cCHARACTER	*lpCharacter	=	g_lpActmap->m_setCharacter.getSelectCharacter();
	
	cACTOR actor;
	actor.reset();
	strcpy(actor.m_strName, lpCharacter->m_strName);

	actor.m_wType				=	0xffff;
	actor.m_wDirect				=	0xffff;
	actor.m_isChangeDirect		=	0xffff;
	actor.m_dwRegenerationCycle	=	0xffffffff;
	actor.m_wGenerateMethod		=	0xffff;
	actor.m_wIsBlockToAutoRegen	=	0xffff;
	actor.m_wMoveArea			=	0xffff;
	actor.m_wPatrolRoute		=	0xffff;
	actor.m_wWanderCycle		=	0xffff;
	actor.m_wBattleInclination	=	0xffff;

	cSET_FIELD_CHARACTER::Init(hWND,&actor,TRUE);

	int iCount = g_lpActmap->m_setActor.m_iCount;

	for(int i=0;i<iCount;++i)
	{
		
		cACTOR* lpActor = g_lpActmap->m_setActor.getActor(i);

		if	(!lpActor)
			continue;

		if	(lpActor->m_wCharacter == lpCharacter->m_wSerial)
		{

			if	(actor.m_wType				!=	0xffff		)
				lpActor->m_wType				=	actor.m_wType;
// 서 있는 방향 설정
			if	(actor.m_wDirect			!=	0xffff		)
				lpActor->m_wDirect				=	actor.m_wDirect;
// 서 있는 동안 방향을 바꿀거냐?
			if	(actor.m_isChangeDirect		!=	0xffff		)
				lpActor->m_isChangeDirect		=	actor.m_isChangeDirect;
// 리젠 주기
			if	(actor.m_dwRegenerationCycle	!=	0xffffffff	)
				lpActor->m_dwRegenerationCycle	=	actor.m_dwRegenerationCycle;
// 리젠 패턴
			if	(actor.m_wGenerateMethod	!=	0xffff	)
				lpActor->m_wGenerateMethod	=	actor.m_wGenerateMethod;
// 자동 리젠
			if	(actor.m_wIsBlockToAutoRegen!=	0xffff		)
				lpActor->m_wIsBlockToAutoRegen	=	actor.m_wIsBlockToAutoRegen;
// 행동 영역
			if	(actor.m_wMoveArea			!=	0xffff		)
				lpActor->m_wMoveArea			=	actor.m_wMoveArea;
// 순찰 루트(없으면 행동 반경 안에서 얼레 벌레~)
			if	(actor.m_wPatrolRoute		!=	0xffff		)
				lpActor->m_wPatrolRoute			=	actor.m_wPatrolRoute;
// 이동 주기 - 0 안움직인다.
			if	(actor.m_wWanderCycle		!=	0xffff		)
				lpActor->m_wWanderCycle			=	actor.m_wWanderCycle;
// 전투 성향.
			if	(actor.m_wBattleInclination	!=	0xffff		)
				lpActor->m_wBattleInclination	=	actor.m_wBattleInclination;
			
		}
	}

}