#include	"cDRAW.H"
#include	"cTEXT.H"
#include	"cMAPWND.H"
#include	"cMAIN.H"
#include	"cOBJECT.H"
#include	"cBUILDING.H"
#include	"cAREA.H"
#include	"cTOOLWND.H"
#include	"cMESSAGE.H"
#include	"cPROJECT.H"
#include	"cSET.H"
#include	"cJOB.H"

//
//	캐릭터 업데이트
void
cMAPWND::UpdateCharacter()
{
	if (g_iWorkMode	!=	dWORK_CHARACTER)
		return;

	if (TOUCHRBUTTON)
	{
		TOUCHRBUTTON	=	FALSE;
		MAIN.ChangeMode(dWORK_EDIT);
	}

	if (!client.isIN(Mouse.x,Mouse.y))	return;

	if (!Mouse.isSame(&OldMouse)		)	m_isRepaint	=	TRUE;	//	커서가 움직였냐?

	if (TOUCHLBUTTON)
	{
		if (Mouse.y	>	g_iTBSize || g_isDrawingMap)
		{
			cACTOR		actor;
			cCHARACTER	*lpCharacter	=	g_lpActmap->m_setCharacter.getSelectCharacter();

			if (!lpCharacter)	return;

			int	x				=	m_posWorld.x-g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE;
			int	y				=	m_posWorld.y-g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE;

			x					=	g_lpActmap->m_pos.x*dTILE_WIDTH /dSHARE_TILE+x*100/g_iScale;
			y					=	g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE+y*100/g_iScale;

			actor.m_pos.x		=	x;
			actor.m_pos.y		=	y;


			strcpy(actor.m_strName,lpCharacter->m_strName);
//			actor.m_pos.x		=	m_posWorld.x;
//			actor.m_pos.y		=	m_posWorld.y;
			actor.m_wJob		=	lpCharacter->m_wBaseJob;	//	직업 번호
			actor.m_wCharacter	=	g_lpActmap->m_setCharacter.m_iSelectCharacter;
			
			int	iFocusArea		=	g_lpActmap->m_pArea->GetFocusArea(x,y);

			if (iFocusArea	==	0xffff)
				iFocusArea	=	0;
			else
			{
				if(g_lpActmap->m_pArea->m_aArea[iFocusArea].m_wKind == dAREA_HUNTING_AREA)
					iFocusArea = 0;
			}

			actor.m_wMoveArea	=	iFocusArea;

			g_lpActmap->m_setActor.add(&actor);

			m_isRepaint	=	TRUE;

			TOOLWND.Draw();
		}

		g_lpActmap->LostVirgin();
	}

//	UpdateEdit();
}	//	cMAPWND::UpdateCharacter()

//
//	캐릭터 데이터 설정
void
cMAPWND::SettingCharacter()
{
	int	iCount		=	g_lpActmap->m_setActor.getSelectedActor(),i;
	int	*lpActorList=	g_lpActmap->m_setActor.m_aSelectedActor;

	if (iCount	>	1)
	{
		cACTOR	actor;

		actor.deepCopy(&g_lpActmap->m_setActor.m_aActor[lpActorList[0]]);

		for (i=0;i<iCount;i++)
		{
			cACTOR	*lpActor	=	&g_lpActmap->m_setActor.m_aActor[lpActorList[i]];

// 팀 설정
			if (actor.m_wType				!=	lpActor->m_wType				)	actor.m_wType				=	0xffff;
// 서 있는 방향 설정
			if (actor.m_wDirect				!=	lpActor->m_wDirect				)	actor.m_wDirect				=	0xffff;
// 서 있는 동안 방향을 바꿀거냐?
			if (actor.m_isChangeDirect		!=	lpActor->m_isChangeDirect		)	actor.m_isChangeDirect		=	0xffff;
// 리젠 주기
			if (actor.m_dwRegenerationCycle	!=	lpActor->m_dwRegenerationCycle	)	actor.m_dwRegenerationCycle	=	0xffffffff;
// 리젠 패턴
			if (actor.m_wGenerateMethod		!=	lpActor->m_wGenerateMethod		)	actor.m_wGenerateMethod		=	0xffff;
// 자동 리젠
			if (actor.m_wIsBlockToAutoRegen	!=	lpActor->m_wIsBlockToAutoRegen	)	actor.m_wIsBlockToAutoRegen	=	0xffff;
// 행동 영역
			if (actor.m_wMoveArea			!=	lpActor->m_wMoveArea			)	actor.m_wMoveArea			=	0xffff;
// 순찰 루트(없으면 행동 반경 안에서 얼레 벌레~)
			if (actor.m_wPatrolRoute		!=	lpActor->m_wPatrolRoute			)	actor.m_wPatrolRoute		=	0xffff;
// 이동 주기 - 0 안움직인다.
			if (actor.m_wWanderCycle		!=	lpActor->m_wWanderCycle			)	actor.m_wWanderCycle		=	0xffff;
// 전투 성향.
			if (actor.m_wBattleInclination	!=	lpActor->m_wBattleInclination	)	actor.m_wBattleInclination	=	0xffff;
		}

		cSET_FIELD_CHARACTER::Init(hWND,&actor,TRUE);

		for (i=0;i<iCount;i++)
		{
			cACTOR	*lpActor	=	&g_lpActmap->m_setActor.m_aActor[lpActorList[i]];

// 팀 설정
			if (actor.m_wType				!=	0xffff		)	lpActor->m_wType				=	actor.m_wType;
// 서 있는 방향 설정
			if (actor.m_wDirect				!=	0xffff		)	lpActor->m_wDirect				=	actor.m_wDirect;
// 서 있는 동안 방향을 바꿀거냐?
			if (actor.m_isChangeDirect		!=	0xffff		)	lpActor->m_isChangeDirect		=	actor.m_isChangeDirect;
// 리젠 주기
			if (actor.m_dwRegenerationCycle	!=	0xffffffff	)	lpActor->m_dwRegenerationCycle	=	actor.m_dwRegenerationCycle;
// 리젠 패턴
			if (actor.m_wGenerateMethod		!=	0xffff		)
			{	
				lpActor->m_wGenerateMethod		=	actor.m_wGenerateMethod;
				// insu add
				if (lpActor->m_wGenerateMethod == dMONSTER_GENERATE_AROUND)
				{
					lpActor->m_wGenerateAroundPosX1 = actor.m_wGenerateAroundPosX1;
					lpActor->m_wGenerateAroundPosY1 = actor.m_wGenerateAroundPosY1;
					lpActor->m_wGenerateAroundPosX2 = actor.m_wGenerateAroundPosX2;
					lpActor->m_wGenerateAroundPosY2 = actor.m_wGenerateAroundPosY2;
				}
				// insu add end
			}
// 자동 리젠
			if (actor.m_wIsBlockToAutoRegen	!=	0xffff		)	lpActor->m_wIsBlockToAutoRegen	=	actor.m_wIsBlockToAutoRegen;
// 행동 영역
			if (actor.m_wMoveArea			!=	0xffff		)	lpActor->m_wMoveArea			=	actor.m_wMoveArea;
// 순찰 루트(없으면 행동 반경 안에서 얼레 벌레~)
			if (actor.m_wPatrolRoute		!=	0xffff		)	lpActor->m_wPatrolRoute			=	actor.m_wPatrolRoute;
// 이동 주기 - 0 안움직인다.
			if (actor.m_wWanderCycle		!=	0xffff		)	lpActor->m_wWanderCycle			=	actor.m_wWanderCycle;
// 전투 성향.
			if (actor.m_wBattleInclination	!=	0xffff		)	lpActor->m_wBattleInclination	=	actor.m_wBattleInclination;
		}
	}
	else
	if	(iCount	==	1)
		cSET_FIELD_CHARACTER::Init(hWND,&g_lpActmap->m_setActor.m_aActor[lpActorList[0]]);

	Draw();
}	//	cMAPWND::SettingCharacter()



//
//	캐릭터 데이터 설정
void
cMAPWND::SetLeader()
{
	int	iCount		=	g_lpActmap->m_setActor.getSelectedActor();
	int	*lpActorList=	g_lpActmap->m_setActor.m_aSelectedActor;

	if (iCount	==	1)
	{
		g_lpActmap->m_setActor.setLeader(lpActorList[0]);

		Draw();
	}
}	//	cMAPWND::SettingCharacter()


//
//	캐릭터 데이터 설정
void
cMAPWND::SetWard()
{
	int	iCount		=	g_lpActmap->m_setActor.getSelectedActor(),i;
	int	*lpActorList=	g_lpActmap->m_setActor.m_aSelectedActor;

	if (iCount	>=	1)
	{
		for (i=0;i<iCount;i++)
		{
			cACTOR	*lpActor	=	&g_lpActmap->m_setActor.m_aActor[lpActorList[i]];

			lpActor->setWard(TRUE);
		}

		Draw();
	}
}	//	cMAPWND::SettingCharacter()