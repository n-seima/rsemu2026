#include "CAgent.H"
#include "CActor.H"
#include "CGamePlay.H"
#include "CSkill.H"
#include "cMESSAGE.H"
#include "CSound.H"
#include "CEvent.H"
#include "CGuardianPost.H"
#include "CWindowInterface.H"
#include "trap.H"
#include "trapSkill.H"
#include "duel.H"

void
cPACKET_HANDLER::ReceiveRemoveTrap(SERVER_PACKETS *_lpPacket)
{
	SG_REMOVE_TRAP	*lpPacket	=	(SG_REMOVE_TRAP	*)_lpPacket;

	g_trapManager.removeTrap(lpPacket->wSerial);
}

void
cPACKET_HANDLER::ReceiveAddTrap(SERVER_PACKETS *_lpPacket)
{
	SG_ADD_TRAP	*lpPacket	=	(SG_ADD_TRAP	*)_lpPacket;

	cTrap	trap;

	trap.m_wSerial			=	lpPacket->wSerial;
	trap.m_wSkill			=	lpPacket->wSkill;
	trap.m_pos.x			=	lpPacket->wPosX;
	trap.m_pos.y			=	lpPacket->wPosY;
	trap.m_wRange			=	lpPacket->wRange;
	trap.m_bf3Team			=	lpPacket->bf3Team;
	trap.m_bf8DetectLevel	=	lpPacket->bf8DetectLevel;

	g_trapManager.addTrap(&trap);
}

void
cPACKET_HANDLER::ReceiveExplosionTrap(SERVER_PACKETS *_lpPacket)
{
	SG_EXPLOSION_TRAP	*lpPacket	=	(SG_EXPLOSION_TRAP	*)_lpPacket;

	cAbility	ability;
	CSkill		*lpSkill;
	BOOL		bIsPlayHitSound	=	TRUE;

	ability.set(lpPacket->bf12Skill,lpPacket->bf8Level);
	lpSkill		=	ability.getSkill();

	if	(lpPacket->bf1IsFirst)
	{
//		g_esm.playCastingSound(lpSkill);
		g_esm.playExplosionSound(lpSkill);

		if	(lpSkill->m_wExplosionImage	!=	0xffff)
			g_sm.castExplosion(g_lpHero,lpPacket->wPosX,lpPacket->wPosY,&ability);
	}

	for (int i=0;i<lpPacket->bf6TargetCount;i++)
	{
		if (i	>=	2)
			bIsPlayHitSound	=	FALSE;

		cWIDE_AREA_DAMAGE_E	*lpInfo	=	&lpPacket->aData[i];
		CHitInfo			hitInfo;

		hitInfo.m_dwPhysicalDamage	=	lpInfo->wPhysicalDamage*100;		//	얼마나? 아파? ;;
		hitInfo.m_dwMagicDamage		=	lpInfo->wMagicDamage*100;		//	얼마나? 아파? ;;
		hitInfo.m_dwResultField		=	lpInfo->dwResultField;	//	1 히트,2 크리티컬,4 크러쉬 블로우,8 블럭,16 히트 액션

		CActor	*lpTarget			=	g_am.getTestedActor(lpInfo->wActor);

		if (!lpTarget)
			continue;
/*
		if (hitInfo.isHit() && hitInfo.getDamage() == 0)
		{
			if (bIsPlayHitSound)
				g_esm.playHitSound(lpSkill);
		}
		else*/
		lpTarget->hit(NULL,&ability,&hitInfo,0,bIsPlayHitSound);
	}
}

void
cPACKET_HANDLER::ReceiveExplosionTrapSkill(SERVER_PACKETS *_lpPacket)
{
	SG_EXPLOSION_TRAP_SKILL	*lpPacket		=	(SG_EXPLOSION_TRAP_SKILL	*)_lpPacket;
	cTrapSkill				*lpSkill		=	g_trapSkillManager.getSkill(lpPacket->bf11Skill);

	if (!lpSkill)
		return;

	for (int i=0;i<lpPacket->bf6TargetCount;i++)
	{
		if (i	<	2)
			g_esm.play(lpSkill->m_strHitSound);

		cTrapSkillDamageE	*lpInfo	=	&lpPacket->aData[i];
		CActor				*lpTarget=	g_am.getTestedActor(lpInfo->wActor);

		if	(!lpTarget)
			continue;

		if	(lpInfo->wPhysicalDamage+lpInfo->wMagicDamage)
			lpTarget->addHitEffectAndInfo(lpSkill->m_wHitImage,lpInfo->wPhysicalDamage,lpInfo->wMagicDamage,lpSkill->m_bf4HitImagePos,lpSkill->m_bf4HitImageAnm);
		else
			lpTarget->addHitInfo(dHIT_INFO_RED_MISS);
	}
}

void
cPACKET_HANDLER::ReceiveExistFireWallList(SERVER_PACKETS *_lpPacket)
{
	SG_EXIST_FIRE_WALL_LIST	*lpPacket	=	(SG_EXIST_FIRE_WALL_LIST	*)_lpPacket;

	for	(int i=lpPacket->wWallCount;i;)
	{
		--i;

		cSimpleFireWallInfo	*lpInfo	=	&lpPacket->aWallList[i];

		g_sm.makeFireWall(lpInfo->m_wSkill,lpInfo->m_wX1,lpInfo->m_wY1,lpInfo->m_wX2,lpInfo->m_wY2);
	}
}

void
cPACKET_HANDLER::ReceiveExistTrapList(SERVER_PACKETS *_lpPacket)
{
	SG_EXIST_TRAP_LIST	*lpPacket	=	(SG_EXIST_TRAP_LIST	*)_lpPacket;

	for (int i=0;i<lpPacket->wTrapCount;i++)
	{
		cTrap	trap;

		trap.m_wSerial			=	lpPacket->aTrapInfo[i].m_wSerial;
		trap.m_wSkill			=	lpPacket->aTrapInfo[i].m_wSkill;
		trap.m_pos.x			=	lpPacket->aTrapInfo[i].m_wX;
		trap.m_pos.y			=	lpPacket->aTrapInfo[i].m_wY;
		trap.m_wRange			=	lpPacket->aTrapInfo[i].m_wRange;
		trap.m_bf3Team			=	lpPacket->aTrapInfo[i].m_bf3Team;
		trap.m_bf8DetectLevel	=	lpPacket->aTrapInfo[i].m_bf8DetectLevel;

		g_trapManager.addTrap(&trap);
	}
}

void
cPACKET_HANDLER::ReceiveExistActiveFieldTrapList(SERVER_PACKETS *_lpPacket)
{
	SG_EXIST_ACTIVE_FIELD_TRAP_LIST	*lpPacket	=	(SG_EXIST_ACTIVE_FIELD_TRAP_LIST*)_lpPacket;

	for (int i=0;i<lpPacket->wTrapCount;i++)
	{
		int				iTrapArea	=	lpPacket->awTrapList[i];
		cAreaTrapInfo	*lpTrapArea	=	(cAreaTrapInfo	*)&g_map.m_area.m_aArea[iTrapArea];

		lpTrapArea->m_wIsActive		=	TRUE;
	}
}


void
cPACKET_HANDLER::ReceiveFieldTrapWork(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::OperateFieldTrapWork((SG_CLOSE_RANGE_FIELD_TRAP_WORK	*)_lpPacket);
}

void
cPACKET_HANDLER::ReceiveSecretDungeonMessage(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::OperateSecretDungeonMessage((SG_SECRET_DUNGEON_MESSAGE *)_lpPacket);
}

void
cPACKET_HANDLER::ReceivePlaySound(SERVER_PACKETS *_lpPacket)
{
	SG_PLAY_SOUND	*lpPacket	=	(SG_PLAY_SOUND	*)_lpPacket;

	//JBC	경로 설정이 2중으로 되있어서 사운드 출력이 안되고 있는 문제 수정. 08-10-02
	g_esm.play(lpPacket->strSoundFile);
}

void
cPACKET_HANDLER::ReceiveEtcWork(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::OperateEtcWork((SG_ETC_WORK *)_lpPacket);
}

void
cPACKET_HANDLER::ReceiveSetAnm(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::OperateSetAnm((SG_SET_ANM*)_lpPacket);
}

void
cPACKET_HANDLER::ReceiveDuelResult(SERVER_PACKETS *_lpPacket)//	이벤트
{
	SG_DUEL_RESULT	*lpPacket	=	(SG_DUEL_RESULT*)_lpPacket;

	{
		if	(g_hero.m_wOperatorLevel	>=	4)
			g_msgBox.cPopup(dMSG_RESULT_OF_DUEL,"",dMSG_OK);
		else		
		if	(lpPacket->wWinTeam	==	g_lpHero->m_wTeam)
			g_msgBox.cPopup(dMSG_WIN,"",dMSG_OK);
		else
		if	(lpPacket->wWinTeam	==	2)
			g_msgBox.cPopup(dMSG_DRAW,"",dMSG_OK);
		else
			g_msgBox.cPopup(dMSG_LOSE,"",dMSG_OK);

		g_msgBox.setButtonId(eMBI_RETURN_TO_THE_VILLAGE_BY_END_OF_GUILD_BATTLE);

		int	iX=80,iY=6,iTextWidth = 80,iTextHeight=16;
		int	iLineCount	=	0;

		g_msgBox.addTextNode(iX,iY,dFITTEXT_RIGHT,WHITE,dMSG_ID);
		iX	+=	60;
		g_msgBox.addTextNode(iX,iY,dFITTEXT_WIDTH_CENTER,WHITE,dMSG_KILL_COUNT);
		iX	+=	iTextWidth;
		g_msgBox.addTextNode(iX,iY,dFITTEXT_WIDTH_CENTER,WHITE,dMSG_DEATH_COUNT);
		iX	+=	iTextWidth;

		if	(lpPacket->iTrialGameVPForWinner)
		{
			char	strTrialGameName[64];

			sprintf(strTrialGameName,"%s %s",lpPacket->strTrialGameName,dMSG_WINNING_POINT);

			g_msgBox.addTextNode(iX,iY,dFITTEXT_WIDTH_CENTER,WHITE,strTrialGameName);
			iX	+=	iTextWidth;
		}
		else
		{
			g_msgBox.addTextNode(iX,iY,dFITTEXT_WIDTH_CENTER,WHITE,dMSG_WEEKLY_VP);
			iX	+=	iTextWidth;
		}

		g_msgBox.addTextNode(iX,iY,dFITTEXT_WIDTH_CENTER,WHITE,dMSG_DUEL_POINT);
		iY	+=	6;

		for (int iTeam=0;iTeam<2;iTeam++)
		{
			DWORD	dwColor	=	LTRED;
			
			int	iVP=0,iDuelPoint=0,iTrialGameVP=0;

			if	(iTeam	==	lpPacket->wWinTeam)
			{
				dwColor		=	LTGREEN;
				iVP			=	lpPacket->iVPForWinner;
				iDuelPoint	=	lpPacket->iDuelPointForWinner;
				iTrialGameVP=	lpPacket->iTrialGameVPForWinner;
			}
			else
			{
				iVP			=	lpPacket->iVPForDefeater;
				iDuelPoint	=	lpPacket->iDuelPointForDefeater;
				iTrialGameVP=	lpPacket->iTrialGameVPForDefeater;
			}

			for (int iMember=0;iMember<5;iMember++)
			{
				cDuelBattlerInfoForResult	*lpInfo	=	&lpPacket->m_aBattler[iTeam][iMember];

				if	(lpInfo->m_strName[0]	==	NULL)
					continue;

				iY	+=	16;
				iX	=	80;
				g_msgBox.addTextNode(iX,iY,dFITTEXT_RIGHT,dwColor,lpInfo->m_strName);
				iX	+=	60;
				g_msgBox.addTextNode(iX,iY,dFITTEXT_WIDTH_CENTER,LTYELLOW,_ms("%d",lpInfo->m_wKillCount));	//	죽인 횟수
				iX	+=	iTextWidth;
				g_msgBox.addTextNode(iX,iY,dFITTEXT_WIDTH_CENTER,LTYELLOW,_ms("%d",lpInfo->m_wDeathCount));	//	죽은 횟수
				iX	+=	iTextWidth;

				if	(lpPacket->iTrialGameVPForWinner)
				{
					if	(iTrialGameVP)
						g_msgBox.addTextNode(iX,iY,dFITTEXT_WIDTH_CENTER,LTYELLOW,_ms("%d(<c:LTGREEN>+%d<n>)",lpInfo->m_iTrialGameVP,iTrialGameVP));	//	WCG 승점
					else	
						g_msgBox.addTextNode(iX,iY,dFITTEXT_WIDTH_CENTER,LTYELLOW,_ms("%d",lpInfo->m_iTrialGameVP));	//	WCG 승점
					iX	+=	iTextWidth;
				}
				else
				{
					if	(iVP)
						g_msgBox.addTextNode(iX,iY,dFITTEXT_WIDTH_CENTER,LTYELLOW,_ms("%d(<c:LTGREEN>+%d<n>)",lpInfo->m_iWeeklyVP,iVP));	//	주간 승점
					else
						g_msgBox.addTextNode(iX,iY,dFITTEXT_WIDTH_CENTER,LTYELLOW,_ms("%d",lpInfo->m_iWeeklyVP));	//	주간 승점

					iX	+=	iTextWidth;
				}

				if	(iDuelPoint)
					g_msgBox.addTextNode(iX,iY,dFITTEXT_WIDTH_CENTER,LTYELLOW,_ms("%d(<c:LTGREEN>+%d<n>)",lpInfo->m_iDuelPoint,iDuelPoint));	//	결투 포인트
				else	
					g_msgBox.addTextNode(iX,iY,dFITTEXT_WIDTH_CENTER,LTYELLOW,_ms("%d",lpInfo->m_iDuelPoint));	//	결투 포인트

				iLineCount++;
			}
		}

		g_msgBox.setSize(500,(iLineCount*iTextHeight)+46);
	}
}

void
cPACKET_HANDLER::ReceiveDuelRanking(SERVER_PACKETS *_lpPacket)
{
	SG_DUEL_RANKING	*lpPacket	=	(SG_DUEL_RANKING*)_lpPacket;
	
	strcpy(g_duelManager.m_strTrialGameName,lpPacket->strTrialGameName);

	g_duelRanking.updateRank(lpPacket->bf2RankType,lpPacket->bf10Count,lpPacket->aList);
}

void
cPACKET_HANDLER::ReceiveTryDuelBattle(SERVER_PACKETS *_lpPacket)//	결투 신청함
{
	SG_TRY_DUEL_BATTLE	*lpPacket	=	(SG_TRY_DUEL_BATTLE	*)_lpPacket;

	strcpy(g_duelManager.m_strTrialGameName,lpPacket->strTrialGameName);

	if	(lpPacket->wApplicantCount	==	0xffff)
	{
		int		iType	=	lpPacket->wType;
		char	strDuelType[32],strTryDuelBattleText[256];

		if	(iType	<	eDUEL_TRIAL_GAME)
			sprintf(strDuelType,"%d vs %d",iType+1,iType+1);
		else
			sprintf(strDuelType,lpPacket->strTrialGameName);

		sprintf(strTryDuelBattleText,dMSG_TRY_DUEL_BATTLE_FORM,strDuelType);

		CGamePlay::AddSystemMessage(WHITE,strTryDuelBattleText);
	}
	else
	{
		CGamePlay::ClosePopupWindow();
		g_duelManager.updateCount(lpPacket->wType,lpPacket->wApplicantCount);
	}
}

//	결투 신청함
void
cPACKET_HANDLER::ReceiveDuelMsg(SERVER_PACKETS *_lpPacket)
{
	SG_DUEL_MSG	*lpPacket	=	(SG_DUEL_MSG	*)_lpPacket;

	char	strMessage[256];

	switch(lpPacket->wMessage)
	{
		case	eDMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_ENTRY_PARTY		:
			sprintf(strMessage,dMSG_DMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_ENTRY_PARTY_FORM,lpPacket->strText);
			break;
		case	eDMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_ENTRY				:
			sprintf(strMessage,dMSG_DMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_ENTRY_FORM,lpPacket->strText);
			break;
		case	eDMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_SEASON	:
			strcpy(strMessage,dMSG_DMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_SEASON);
			break;
	}

	switch(lpPacket->wOutputType)
	{
		case	eSM_SYSTEM		:
			CGamePlay::AddSystemMessage(BTPURPLE,strMessage);
			break;
		case	eSM_CHATTING	:
			CGamePlay::AddSystemMessage(BTPURPLE,strMessage);
			break;
		case	eSM_NOTICE		:
			CGamePlay::Notice(strMessage);
			break;
			
		case	eSM_WARNING		:
			CGamePlay::WarningMessage(strMessage);
			break;
			
		case	eSM_MSG_BOX		:
			g_msgBox.cPopup("",strMessage,dMSG_OK);
			break;
	}
}