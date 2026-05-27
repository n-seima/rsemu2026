#include "cMAP.H"
#include "CActor.H"
#include "ChitEffect.H"
#include "effectManager.H"
#include "cJOB.H"
#include "cAREA.H"
#include "cKARMA.H"
#include "cKARMA_VALUE.H"
#include "CFieldItem.H"
#include "CGuardianPost.H"
#include "CPitchmanShop.H"
#include "CPortal.H"
#include "trap.H"
#include "cMessage.H"
#include "arca.H"
#include "duel.H"


//
//	Minpmap Display Icon Image
enum
{
	eMDII_WEAPON_SHOP	=	35,
	eMDII_STRESS_BEGIN	=	56,
	eMDII_STRESS_END	=	65,
	eMDII_TREASURE_ARCA	=	81,
	eMDII_GUILD_HALL_TELEPORTER = 187,
	eMDII_ADVENTURER_SOCIETY	= 242,
};



BOOL
cMAP::DrawFogMap()
{

	if (m_tgaMinimap.m_p16IMAGE == NULL)
		return FALSE;

	int		iCartographerLevel	=	g_hero.getTitleLevel(eTN_CARTOGRAPHER);
	
	if	(iCartographerLevel	<	s_iMinimapDisplayLevel)
		return FALSE;

	cDRAW::SetClippginArea(m_iPutPosX, m_iPutPosY, m_iPutPosX+m_iFogmapWidth, m_iPutPosY+m_iFogmapHeight);
	BOOL	bIsDrawFirstAreaName=	FALSE;

	cDRAW::BoxAB(0, 32, m_iPutPosX-2, m_iPutPosY-2, m_iPutPosX+m_iFogmapWidth-1+2, m_iPutPosY+m_iFogmapHeight-1+2, 2);

	cDRAW::FillAB(0, m_iPutPosX, m_iPutPosY, m_iPutPosX+m_iFogmapWidth-1, m_iPutPosY+m_iFogmapHeight-1, 5);
	cDRAW::PutImageAB(m_iPutPosX, m_iPutPosY, m_tgaMinimap.m_p16IMAGE, m_iFogmapAlpha);	

	
	for(int i=0 ; i<dMAX_AREA ; i++)		// 필드 이동 지역 출력
	{
		if (m_area.m_aArea[i].m_wSerial == 0xffff)
			continue;

		int			iArea	=	m_area.m_aArea[i].m_wSerial;
		cAreaInfo	*lpArea	=	&m_area.m_aArea[i];
		BOOL		bIsSecretDungeonGate	=	FALSE;
		int			iAnm	=	eEANM_MINIMAP_GATE;
		CPos		pos;

		if (lpArea->m_gateShape ==  ePAS_ENTER_GUILD_HALL)
			continue;

		if (lpArea->m_wKind	==	dAREA_DOOR)
		{
			cAreaDoorInfo	*lpDoor	=	(cAreaDoorInfo	*)lpArea;

			if (iArea	==	m_area.m_wInstanceFieldGate	||	g_hero.getSecretDungeonKey(lpDoor->m_wLinkSecretDungeon))
				bIsSecretDungeonGate	=	TRUE;
		}

		if	(m_area.m_aArea[i].m_bf1IsTestPortal	&&	m_area.m_aArea[i].m_bf15LinkObject	==	0x7000	&&	g_bIsTestServer	==	FALSE)
			continue;

		if	(bIsSecretDungeonGate	==	FALSE	&&	m_area.m_aArea[i].m_wKind	!=	dAREA_PORTAL)
			continue;

		if	(bIsSecretDungeonGate)
			iAnm	=	eEANM_MINIMAP_WEAPON_GATE;
		else
		{
			switch(m_area.m_aArea[i].m_gateShape)
			{
			case	ePAS_BUILDING_GATE	:
			case	ePAS_ONE_WAY_GATE	:
				iAnm	=	eEANM_MINIMAP_HOUSE_GATE;
				break;
				
			case	ePAS_NORMAL			:
				iAnm	=	eEANM_MINIMAP_GATE;
				break;
				
			case	ePAS_INNER_PORTAL	:
				iAnm	=	eEANM_MINIMAP_INNER_PORTAL;
				break;
				
			case	ePAS_HIDDEN_INNER_PORTAL	:
			case	ePAS_HIDDEN			:
				continue;

			case	ePAS_INNER_DUNGEON	:
				iAnm	=	eEANM_MINIMAP_INNER_DUNGEON;
				break;
			case	ePAS_INNER_TOWER	:
				iAnm	=	eEANM_MINIMAP_INNER_TOWER;
				break;
			case	ePAS_INNER_BUILDING	:
				iAnm	=	eEANM_MINIMAP_INNER_BUILDING;
				break;
			}
		}

		m_area.m_aArea[i].getCenter(&pos);

		pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
		pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;

		int iFPS		=	g_anmEtc.GetFPS(iAnm);
		int iFrameCount	=	g_anmEtc.GetFrameCount(iAnm);
		int iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount;

		if (m_area.m_aArea[i].m_gateShape == ePAS_ONE_WAY_GATE)
			continue;

		if (iAnm == ePAS_INNER_DUNGEON || iAnm == ePAS_INNER_TOWER || iAnm == ePAS_INNER_BUILDING)
		{
			pos.x -= (g_anmEtc.getSpriteWidth(iAnm, 0, 0)/3);
			pos.y -= (g_anmEtc.getSpriteHeight(iAnm, 0, 0)/3);

			g_anmEtc.PutAlpha(m_iPutPosX+pos.x, m_iPutPosY+pos.y, iAnm, 0, iFrame, 75, 75, m_iFogmapAlpha);
		}
		else
			g_anmEtc.PutAlpha(m_iPutPosX+pos.x, m_iPutPosY+pos.y, iAnm, 0, iFrame, 100, 100, m_iFogmapAlpha);
	} // 필드 이동 지역 출력 끝


	for (i=0;i<g_am.m_iActorCount;i++)	// 액터관련 아이콘들 .. 
	{
		CActor	*lpActor	=	&g_aActor[g_am.m_aActorList[i]];

		if	(lpActor->isHero())
			continue;

		int	iMark	=	-1;


		switch(lpActor->m_wActorKind)
		{	// 수정..
		case 	eNPC_WEAPON_MERCHANT_KNIGHT_WARRIOR			:
		case	eNPC_WEAPON_MERCHANT_WIZARD_WEREWOLF		:
		case	eNPC_WEAPON_MERCHANT_PRIEST_FALLEN_ANGEL	:
		case	eNPC_WEAPON_MERCHANT_ROGUE_FIGHTER			:
		case	eNPC_WEAPON_MERCHANT_LANCER_ARCHER			:
		case	eNPC_WEAPON_MERCHANT_TAMER_SUMMONER			:
		case	eNPC_WEAPON_MERCHANT_PRINCESS_MAGICAL_GIRL	:
		case	eNPC_WEAPON_MERCHANT_NECROMANCER_DEVIL		:
		case	eNPC_WEAPON_MERCHANT_SOUL_BRINGER			:
		case	eNPC_WEAPON_MERCHANT_OPTICALIST				:
			iMark	=	eNPC_WEAPON_MERCHANT;
			break;
		}

		char	*lpstrName	=	lpActor->getName();


		if	(lpActor->isNpc() && lpstrName)
		{
			CQuestItem	*lpQuest=	g_hero.getProprietyQuest(lpstrName,lpActor->m_wNpcActorSerial);
			
			if	(g_hero.isProcessQeustClient(lpstrName,lpActor->m_wNpcActorSerial))
				iMark	=	eNPC_PROCESS_QUEST_CLIENT;
			else
			{
				if	(g_hero.isNpcAboutProcessQuest(lpstrName,lpActor->m_wNpcActorSerial))
					iMark	=	eNPC_PROCESS_QUEST_REFERENCE_NPC;
				else
				{
					if	(lpQuest)
					{
						switch(lpQuest->m_wQuestKind)
						{
						case	eQK_NORMAL		:
							iMark	=	eNPC_NORMAL_QUEST;
							break;
						case	eQK_SKILL_MASTER:
							iMark	=	eNPC_SKILL_MASTER;
							break;
						case	eQK_TITLE		:
							iMark	=	eNPC_TITLE_QUEST;
							break;
						case	eQK_MAIN_STORY	:
							iMark	=	eNPC_MAJOR_QUEST;
							break;
						case	eQK_GUILD		:
							iMark	=	eNPC_GUILD_QUEST;
							break;
						}
					}
				}
			}
		}
		else
		{
			if	(lpActor->m_wActorKind	>=	eNPC_PROCESS_QUEST_CLIENT	&&	lpActor->m_wActorKind	<=	eNPC_PROCESS_QUEST_TARGET_MONSTER	)
				iMark	=	lpActor->m_wActorKind;
		}

		if	(iMark	==	-1)
		{	// 수정..
			if	((lpActor->m_wActorKind	>=	eNPC_WEAPON_MERCHANT	&&	lpActor->m_wActorKind	<=	eNPC_BANK) ||
				 (lpActor->m_wActorKind	>=	eNPC_DOUMI	&&	lpActor->m_wActorKind	<=	eNPC_FIRST_HELPER) ||
				 (lpActor->m_wActorKind	>=	eNPC_WEAPON_MERCHANT_KNIGHT_WARRIOR	&&	lpActor->m_wActorKind	<=	eNPC_WEAPON_MERCHANT_NECROMANCER_DEVIL) ||
				 (lpActor->m_wActorKind == eNPC_WEAPON_MERCHANT_SOUL_BRINGER) || (lpActor->m_wActorKind == eNPC_WEAPON_MERCHANT_OPTICALIST))
				iMark	=	lpActor->m_wActorKind;
		}

		if	(lpActor->m_wActorKind	==	eNPC_FIRST_HELPER)
			iMark	=	eNPC_FIRST_HELPER;

		if(lpActor->m_wActorKind	>= eNPC_GUILD_HALL_TELEPORTER &&	lpActor->m_wActorKind	<= eNPC_EVENT_HELPER) 
			iMark	=	eMDII_GUILD_HALL_TELEPORTER + eNPC_WEAPON_MERCHANT - eMDII_WEAPON_SHOP + lpActor->m_wActorKind - eNPC_GUILD_HALL_TELEPORTER;

		if(lpActor->m_wActorKind	== eNPC_ADVENTURER_SOCIETY
			&& iMark	!=	eNPC_NORMAL_QUEST
			&& iMark	!=	eNPC_TITLE_QUEST
			&& iMark	!=	eNPC_GUILD_QUEST
			&& iMark	!=	eNPC_MAJOR_QUEST) 
			iMark	=	eMDII_ADVENTURER_SOCIETY + eNPC_WEAPON_MERCHANT - eMDII_WEAPON_SHOP;

		if	(iMark	>= eNPC_WEAPON_MERCHANT)
		{
			int		iKind	=	iMark	-	eNPC_WEAPON_MERCHANT;

			CPos	pos;

			pos.x	=	lpActor->m_pos.x;
			pos.y	=	lpActor->m_pos.y;

			pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
			pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;

			g_sprInterface2.Put(m_iPutPosX+pos.x, m_iPutPosY+pos.y, eMDII_WEAPON_SHOP+iKind, 100, 100, dPUT_ALPHA_BLENDING, m_iFogmapAlpha);
		}

		BOOL	bIsDisplayPosition	=	FALSE;
		BOOL	bIsGuildObject		=	FALSE;

		if	(g_bIsDuelServer && g_duelManager.m_uiBeginBattleTime	&&	g_hero.m_wOperatorLevel	>=	4)
			bIsDisplayPosition	=	TRUE;
		else
		if	(s_bIsGuildBattleField)
		{
			if	(lpActor->isOwnTeam() || g_hero.m_wOperatorLevel	>=	4)
				bIsDisplayPosition	=	TRUE;

			if	(lpActor->m_wBody	==	dJOB_MONSTER_Guild_Crest	||	lpActor->m_wBody	==	dJOB_MONSTER_GuildFlag)
				bIsGuildObject	=	TRUE;

			if	(lpActor->m_wOperatorLevel	>=	4)
				bIsDisplayPosition	=	FALSE;
		}
		else
		if	(lpActor->isPlayer()	&& lpActor->isPartyMember()	&&	lpActor->m_enchantedImage.m_bf1IsTransToWeapon	==	FALSE)
			bIsDisplayPosition	=	TRUE;

		if	(bIsDisplayPosition	||	bIsGuildObject)
		{
			CPos	pos;

			pos.x	=	lpActor->m_pos.x;
			pos.y	=	lpActor->m_pos.y;

			pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
			pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;

			if	(g_bIsDuelServer && g_duelManager.m_uiBeginBattleTime	&&	g_hero.m_wOperatorLevel	>=	4)
				g_sprInterface.Put(pos.x,pos.y-2,eSN_1+lpActor->m_wTeam);
			else
			if	(s_bIsGuildBattleField && g_hero.m_wOperatorLevel	>=	4)
				g_sprInterface.Put(pos.x,pos.y-2,eSN_1+lpActor->m_wTeam);
			else
			if	(bIsGuildObject)
			{
				cDRAW::XLine(_SKYBLUE, m_iPutPosX+pos.x-1, m_iPutPosY+pos.x+1, pos.y);
				cDRAW::YLine(_SKYBLUE, m_iPutPosX+pos.y-1, m_iPutPosY+pos.y+1, pos.x);
			}
			else
			if	(lpActor->isPartyMember())
			{
				int	iIndex	=	s_partyInfo.getMemberIndex(lpActor->m_strName);

				if	(iIndex	!=	0xffff)
				{
					int	iValue	=	s_iFrameCounter%256;

					if	(iValue	>=	128)
						iValue	=	127+256-iValue;

					g_sprInterface.Put(m_iPutPosX+pos.x, m_iPutPosY+pos.y-2, eSN_1+iIndex);
				}
			}
			else
			{
				cDRAW::XLine(_LTGREEN, m_iPutPosX+pos.x-1, m_iPutPosY+pos.x+1, pos.y);
				cDRAW::YLine(_LTGREEN, m_iPutPosX+pos.y-1, m_iPutPosY+pos.y+1, pos.x);
			}
		}
	}
	// 액터관련 아이콘들 .. 끝

	for (i=0;i<g_am.m_iActorCount;i++)	// 액터관련 아이콘들 .. 
	{
		CActor	*lpActor	=	&g_aActor[g_am.m_aActorList[i]];

		if	(lpActor->isHero())
			continue;

		int	iMark	=	-1;


		switch(lpActor->m_wActorKind)
		{	// 수정..
		case 	eNPC_WEAPON_MERCHANT_KNIGHT_WARRIOR			:
		case	eNPC_WEAPON_MERCHANT_WIZARD_WEREWOLF		:
		case	eNPC_WEAPON_MERCHANT_PRIEST_FALLEN_ANGEL	:
		case	eNPC_WEAPON_MERCHANT_ROGUE_FIGHTER			:
		case	eNPC_WEAPON_MERCHANT_LANCER_ARCHER			:
		case	eNPC_WEAPON_MERCHANT_TAMER_SUMMONER			:
		case	eNPC_WEAPON_MERCHANT_PRINCESS_MAGICAL_GIRL	:
		case	eNPC_WEAPON_MERCHANT_NECROMANCER_DEVIL		:
		case	eNPC_WEAPON_MERCHANT_SOUL_BRINGER			:
		case	eNPC_WEAPON_MERCHANT_OPTICALIST				:

			iMark	=	eNPC_WEAPON_MERCHANT;
			break;
		}

		char	*lpstrName	=	lpActor->getName();


		if	(lpstrName)
		{
			CQuestItem	*lpQuest=	g_hero.getProprietyQuest(lpstrName,lpActor->m_wNpcActorSerial);
			
			if	(g_hero.isProcessQeustClient(lpstrName,lpActor->m_wNpcActorSerial))
				iMark	=	eNPC_PROCESS_QUEST_CLIENT;
			else
			{
				if	(g_hero.isNpcAboutProcessQuest(lpstrName,lpActor->m_wNpcActorSerial))
					iMark	=	eNPC_PROCESS_QUEST_REFERENCE_NPC;
				else
				{
					if	(lpQuest)
					{
						switch(lpQuest->m_wQuestKind)
						{
						case	eQK_NORMAL		:
							iMark	=	eNPC_NORMAL_QUEST;
							break;
						case	eQK_SKILL_MASTER:
							iMark	=	eNPC_SKILL_MASTER;
							break;
						case	eQK_TITLE		:
							iMark	=	eNPC_TITLE_QUEST;
							break;
						case	eQK_MAIN_STORY	:
							iMark	=	eNPC_MAJOR_QUEST;
							break;
						case	eQK_GUILD		:
							iMark	=	eNPC_GUILD_QUEST;
							break;
						}
					}
				}
			}
		}
		else
		{
			if	(lpActor->m_wActorKind	>=	eNPC_PROCESS_QUEST_CLIENT	&&	lpActor->m_wActorKind	<=	eNPC_PROCESS_QUEST_TARGET_MONSTER	)
				iMark	=	lpActor->m_wActorKind;
		}

		if	(iMark	==	-1)
		{	// 수정..
			if	((lpActor->m_wActorKind	>=	eNPC_WEAPON_MERCHANT	&&	lpActor->m_wActorKind	<=	eNPC_BANK) ||
				 (lpActor->m_wActorKind	>=	eNPC_DOUMI	&&	lpActor->m_wActorKind	<=	eNPC_FIRST_HELPER) ||
				 (lpActor->m_wActorKind	>=	eNPC_WEAPON_MERCHANT_KNIGHT_WARRIOR	&&	lpActor->m_wActorKind	<=	eNPC_WEAPON_MERCHANT_NECROMANCER_DEVIL) ||
				 (lpActor->m_wActorKind == eNPC_WEAPON_MERCHANT_SOUL_BRINGER) || (lpActor->m_wActorKind == eNPC_WEAPON_MERCHANT_OPTICALIST))
				iMark	=	lpActor->m_wActorKind;
		}

		if	(lpActor->m_wActorKind	==	eNPC_FIRST_HELPER)
			iMark	=	eNPC_FIRST_HELPER;

		if(lpActor->m_wActorKind	>= eNPC_GUILD_HALL_TELEPORTER &&	lpActor->m_wActorKind	<= eNPC_EVENT_HELPER) 
			iMark	=	eMDII_GUILD_HALL_TELEPORTER + eNPC_WEAPON_MERCHANT - eMDII_WEAPON_SHOP + lpActor->m_wActorKind - eNPC_GUILD_HALL_TELEPORTER;

		if	(iMark	>= eNPC_WEAPON_MERCHANT)
		{
			int		iKind	=	iMark	-	eNPC_WEAPON_MERCHANT;
			
			CPos	pos;
			
			pos.x	=	lpActor->m_pos.x;
			pos.y	=	lpActor->m_pos.y;
			
			pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
			pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;
			
			if(iMark == eNPC_PROCESS_QUEST_REFERENCE_NPC || iMark == eNPC_PROCESS_QUEST_CLIENT)
			{
				// insu modify
				if (lpActor->m_wActorKind != eNPC_MONSTER)
					drawMinimapOnscreenQuestNpcArrow(lpstrName, &pos, &s_mmFrame, m_iPutPosX+pos.x, m_iPutPosY+pos.y, m_iFogmapAlpha);
				// insu modify end
			}
		}
	}
	
	//	디텍팅 이블		
	if(g_am.m_iDetectingActorCount)
	{
		for (i=0;i<g_am.m_iDetectingActorCount;i++)
		{
			CPos	pos;
			
			pos.x	=	g_am.m_aDetetingActorList[i].bf16X;
			pos.y	=	g_am.m_aDetetingActorList[i].bf15Y;
			
			pos.x	=	pos.x*m_iFogmapWidth/m_iPixelWidth;
			pos.y	=	pos.y*m_iFogmapHeight/m_iPixelHeight;
			
			
			cDRAW::XLine(_LTRED, m_iPutPosX+pos.x-1, m_iPutPosX+pos.x+1, m_iPutPosY+pos.y);
			cDRAW::YLine(_LTRED, m_iPutPosY+pos.y-1, m_iPutPosY+pos.y+1, m_iPutPosX+pos.x);
		}
	}
	//	디텍팅 이블 끝
		

	if (g_am.m_posStress.x	!=	-1)
	{
		CPos	pos;
		
		pos.x	=	g_am.m_posStress.x;
		pos.y	=	g_am.m_posStress.y;
		
		pos.x	=	pos.x*m_iFogmapWidth/m_iPixelWidth - 51;		// 51 보정값
		pos.y	=	pos.y*m_iFogmapHeight/m_iPixelHeight - 15;	// 15 보정값
		
		int	iBeginImage	=	56;
		int	iEndImage	=	65;
		
		int iFPS		=	24;
		int iFrameCount	=	eMDII_STRESS_END-eMDII_STRESS_BEGIN+1;
		int iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%(iFrameCount+iFrameCount-2);
		
		if (iFrame	>=	iFrameCount)
			iFrame	=	iFrameCount-(iFrame-iFrameCount+1);
		
		g_sprInterface2.Put(m_iPutPosX+pos.x+1, m_iPutPosY+pos.y+1, iFrame+eMDII_STRESS_BEGIN, 100, 100, dPUT_ALPHA_BLENDING, m_iFogmapAlpha);
		g_sprInterface2.Put(m_iPutPosX+pos.x, m_iPutPosY+pos.y, iFrame+eMDII_STRESS_BEGIN, 100, 100, dPUT_ALPHA_BLENDING, m_iFogmapAlpha);
	}
		
	
	
	if (CGamePlay::s_bIsOXQuizField && g_hero.isOperator())
	{
		for (i=0;i<g_am.m_iActorCount;i++)
		{
			CActor	*lpActor	=	&g_aActor[g_am.m_aActorList[i]];
			
			if (lpActor->isHighOperator())
				continue;
			
			CPos	pos;
			
			pos.x	=	lpActor->m_pos.x - 51;		// 51 보정값
			pos.y	=	lpActor->m_pos.y - 15;	// 15 보정값
			
			pos.x	=	pos.x*m_iFogmapWidth/m_iPixelWidth;
			pos.y	=	pos.y*m_iFogmapHeight/m_iPixelHeight;
			
			WORD	wColor	=	_LTGRAY;
			
			if (lpActor->m_bf1IsQuizEventLoser == TRUE)
			{
				cDRAW::XLine(_LTRED,m_iPutPosX+pos.x-1, m_iPutPosX+pos.x+1, pos.y);
				cDRAW::YLine(_LTRED,m_iPutPosY+pos.y-1, m_iPutPosY+pos.y+1, pos.x);
			}
			else
				if (lpActor->m_bf1IsQuizEventWinner == FALSE)
				{
					cDRAW::XLine(_LTGREEN, m_iPutPosX+pos.x-1, m_iPutPosX+pos.x+1, pos.y);
					cDRAW::YLine(_LTGREEN, m_iPutPosY+pos.y-1, m_iPutPosY+pos.y+1, pos.x);
				}
		}
	}
	
	
	cArcaInfo	*lpTreasureBox	=	g_arcaManager.getArca(0x7fff);
		
	if	(lpTreasureBox)
	{
		CPos	pos;
		
		pos.x	=	lpTreasureBox->m_wX;
		pos.y	=	lpTreasureBox->m_wY;
		
		pos.x	=	pos.x*m_iFogmapWidth/m_iPixelWidth;		// 51 보정값
		pos.y	=	pos.y*m_iFogmapHeight/m_iPixelHeight;		// 15 보정값
		
		g_sprInterface2.Put(m_iPutPosX+pos.x+1, m_iPutPosY+pos.y+1, eMDII_TREASURE_ARCA, 100, 100, dPUT_ALPHA_BLENDING, m_iFogmapAlpha);
		g_sprInterface2.Put(m_iPutPosX+pos.x  , m_iPutPosY+pos.y  ,eMDII_TREASURE_ARCA, 100, 100, dPUT_ALPHA_BLENDING, m_iFogmapAlpha);
	}
		
	if (bIsDrawFirstAreaName	==	FALSE)
	{
		int	iNamedAreaWidth	=	80;
		int	iNamedAreaHeight=	32;
		
		
		for (i=0;i<m_area.m_wNamedAreaCount;i++)
		{
			cAreaInfo	*lpArea	=	m_area.getNameArea(i);
			CPos		pos;
			
			lpArea->getCenter(&pos);
			
			pos.x	=	pos.x*m_iFogmapWidth/m_iPixelWidth - 51;		// 51 보정값
			pos.y	=	pos.y*m_iFogmapHeight/m_iPixelHeight - 15;	// 15 보정값
			
			CGame::s_sText.setClipArea(m_iPutPosX, m_iPutPosY, m_iPutPosX+m_iFogmapWidth, m_iPutPosY+m_iFogmapHeight);	
			CGame::s_sText.cPut(m_iPutPosX+pos.x, m_iPutPosY+pos.y, 100, 32, 0, WHITE, dFITTEXT_CENTER, lpArea->m_strName);						
			CGame::s_sText.releaseClipArea();
		}
		
	}
	// 사냥터 영역 출력
	drawFogmapHuntingArea();
	// 사냥터 영역 출력 끝

	// 퀘스트관련 필드 출력
	cRECT rt;
	rt.set(m_iPutPosX, m_iPutPosY, m_iPutPosX+m_iFogmapWidth, m_iPutPosY+m_iFogmapHeight);
	drawQuestLinkDungeonArrow(&rt, TRUE);
	// 퀘스트관련 필드 출력 끝
	
	// 유저의 현재 위치 출력
	if (timeGetTime()%1200 < 1000)
	{
		g_sprWorldMap.Put(	m_iPutPosX+g_am.m_posFocus.x*m_iFogmapWidth/m_iPixelWidth, 
			m_iPutPosY+g_am.m_posFocus.y*m_iFogmapHeight/m_iPixelHeight,
			g_iWorldMapJobIconIndex[g_hero.m_wJob]);
	}
	// 유저의 현재 위치 출력 끝
	// 관련 몬스터 행동영역 출력
	drawQuestMonsterArea(&rt, TRUE);
	cDRAW::FreeClippginArea();
	
	return TRUE;
}
// insu file add end