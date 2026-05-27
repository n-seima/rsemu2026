#include "cField.h"
#include "cGuild.h"
#include "cGame.h"
#include "booking_work.h"

void
cFIELD::beginSiegeWarfare()
{
	m_bIsProcessSiegeWarfare	=	FALSE;

	cGuildHallInfo	*lpHallInfo	=	g_guildManager.getHallInfoByBattleFieldSerial(m_iUniqueSerial);

	if	(!lpHallInfo)
		return;

	if	(lpHallInfo->m_wInvadeGuild==	0xffff)
		return;

	if	(lpHallInfo->m_wOwnGuild	==	0xffff)
	{
		g_game.sendWPSimpleWork(eSWGW_SIEGE_WARFARE_RESULT_SUCCESS_TO_DEFENSE,lpHallInfo->m_bf3HallLevel,lpHallInfo->m_bf5Slot);

		return;
	}

	cAreaDoorInfo	*lpDoor		=	NULL;

	{
		for (int i=0;i<10;i++)
		{
			lpDoor		=	(cAreaDoorInfo *)m_pArea->getDoorByGeneratePeriod(1,i);

			if	(!lpDoor)
				break;

			lpDoor->open();
			lpDoor->sendDoorStatus();
		}
	}
	{
		for (int i=0;i<10;i++)
		{
			lpDoor		=	(cAreaDoorInfo *)m_pArea->getDoorByGeneratePeriod(2,i);

			if	(!lpDoor)
				break;

			lpDoor->close();
			lpDoor->generate();
			lpDoor->sendDoorStatus();
		}
	}

	m_bIsProcessSiegeWarfare	=	TRUE;
	m_iRemainStategyPoint		=	c_iGuildStategyPointCount;

	addSendEtcWork(0xffff,eEW_CHANGE_REMAIN_GUILD_STATEGY_POINT_COUNT,m_iRemainStategyPoint);

	regenAllMob();	//	혹시 사라진게 있으면 모두 리젠

	for	(int iTeam=0;iTeam<dTEAM_COUNT;iTeam++)	//	오브젝트 체력 모두 회복
	{
		for (int iIndex=0;iIndex<m_aiActorCount[iTeam];iIndex++)
		{
			int		iActor		=	m_awActorList[iTeam][iIndex];
			cACTOR	*lpActor	=	getActor(iActor);

			if	(lpActor->m_bIsGuildObject)	//	길드 문장&길드 전략 포인트
			{
				lpActor->m_iHP	=	lpActor->getMaxHP();
				continue;
			}
		}
	}
}

void
cFIELD::resetSiegeWarfareStatus()
{
	cAreaDoorInfo	*lpDoor		=	NULL;

	{
		for (int i=0;i<10;i++)
		{
			lpDoor		=	(cAreaDoorInfo *)m_pArea->getDoorByGeneratePeriod(1,i);

			if	(!lpDoor)
				break;

			lpDoor->close();
		}
	}
	{
		for (int i=0;i<10;i++)
		{
			lpDoor		=	(cAreaDoorInfo *)m_pArea->getDoorByGeneratePeriod(2,i);

			if	(!lpDoor)
				break;

			lpDoor->close();
			lpDoor->generate();
		}
	}

	m_iSiegeWarfareValue		=	random(256);
	m_iRemainStategyPoint		=	c_iGuildStategyPointCount;

	for	(int iTeam=0;iTeam<dTEAM_COUNT;iTeam++)
	{
		for (int iIndex=0;iIndex<m_aiActorCount[iTeam];iIndex++)
		{
			int		iActor		=	m_awActorList[iTeam][iIndex];
			cACTOR	*lpActor	=	getActor(iActor);

			if	(lpActor->isPlayer())
			{
				CGuild	*lpGuild	=	lpActor->getGuild();
				int		iFieldSerial=	lpActor->m_wLastVillage;
				int		iGate		=	0x7ffe;

				if	(lpGuild)
				{
					iFieldSerial	=	g_guildManager.getGuildHallFieldSerial(lpGuild);

					if	(iFieldSerial	==	-1)
						iFieldSerial=	lpActor->m_wLastVillage;
					else
						iGate		=	iGate;
				}

				g_pMoveFieldUserManager->booking(lpActor,NULL,iFieldSerial,iGate);	//	마을의 길드홀 입구로 보낸다
				continue;
			}

			if	(lpActor->m_bIsGuildObject)	//	길드 문장&길드 전략 포인트
			{
				lpActor->m_iHP	=	lpActor->getMaxHP();
				continue;
			}

			addBookedRemoveActor(iActor);	//	길드 가디언 모두 제거
		}
	}
}

void
cFIELD::reportSuccessToDefense()
{
	if	(m_bIsProcessSiegeWarfare	==	FALSE)
		return;

	m_bIsProcessSiegeWarfare		=	FALSE;

	cGuildHallInfo	*lpHallInfo		=	g_guildManager.getHallInfoByBattleFieldSerial(m_iUniqueSerial);

	CGuild			*lpDefenseGuild	=	g_guildManager.getBySerial(lpHallInfo->m_wOwnGuild);
	CGuild			*lpInvadeGuild	=	g_guildManager.getBySerial(lpHallInfo->m_wInvadeGuild);

	if	(!lpDefenseGuild	||	!lpInvadeGuild)
		return;

	g_game.sendWPSimpleWork(eSWGW_SIEGE_WARFARE_RESULT_SUCCESS_TO_DEFENSE,lpHallInfo->m_bf3HallLevel,lpHallInfo->m_bf5Slot);
}

//
//
void
cFIELD::reportSuccessToInvade()
{
	if	(m_bIsProcessSiegeWarfare	==	FALSE)
		return;

	m_bIsProcessSiegeWarfare		=	FALSE;

	cGuildHallInfo	*lpHallInfo		=	g_guildManager.getHallInfoByBattleFieldSerial(m_iUniqueSerial);

	CGuild			*lpDefenseGuild	=	g_guildManager.getBySerial(lpHallInfo->m_wOwnGuild);
	CGuild			*lpInvadeGuild	=	g_guildManager.getBySerial(lpHallInfo->m_wInvadeGuild);

	if	(!lpDefenseGuild	||	!lpInvadeGuild)
		return;

	g_game.sendWPSimpleWork(eSWGW_SIEGE_WARFARE_RESULT_SUCCESS_TO_INVADE,lpHallInfo->m_bf3HallLevel,lpHallInfo->m_bf5Slot);
}

void
cFIELD::destroyGuildStategyPoint()
{
	if	(m_bIsProcessSiegeWarfare	==	FALSE)
		return;

	if	(m_iRemainStategyPoint)
	{
		m_iRemainStategyPoint--;
		addSendEtcWork(0xffff,eEW_CHANGE_REMAIN_GUILD_STATEGY_POINT_COUNT,m_iRemainStategyPoint);
	}
	else
		return;

	for	(int iTeam=0;iTeam<dTEAM_COUNT;iTeam++)
	{
		for (int iIndex=0;iIndex<m_aiActorCount[iTeam];iIndex++)
		{
			int		iActor		=	m_awActorList[iTeam][iIndex];
			cACTOR	*lpActor	=	getActor(iActor);

			if	(lpActor->m_bIsGuildCrest)
			{
				int	iDamage		=	lpActor->m_iHP	-	c_iGuildCrestHP/8;
				iDamage			=	min(iDamage,c_iGuildCrestHP/4);

				if	(iDamage	>	0)
				{
					lpActor->m_iHP	-=	iDamage;

					lpActor->addSendHitDamage(NULL,c_iGuildCrestHP/4,0,dATTACK_RESULT_FIELD_HIT,0);
				}
				continue;
			}

			if	(lpActor->m_bIsGuildObject)
				continue;

			lpActor->buildByGuildStragety();
		}
	}
}