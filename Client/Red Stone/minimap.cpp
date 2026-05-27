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
#include "CWorldMap.h"

#define dMINIMAP_OPTION_WND_WIDTH 130
#define dMAX_QUEST_MONSTER	dMAX_PROCESS_QUEST_COUNT*5
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
cMAP::getMMCPosToReal(CPos *_lpPosReal)
{
	if (!m_tgaMinimap.m_p16IMAGE)
		return	FALSE;

	if (_lpPosReal	==	NULL)
		return	TRUE;

	int	iDX			=	s_posMouse.x-s_mmFrame.x1;
	int	iDY			=	s_posMouse.y-s_mmFrame.y1;

	_lpPosReal->x	=	m_iPixelWidth*(m_posMM.x+iDX)/m_tgaMinimap.m_iWidth;
	_lpPosReal->y	=	m_iPixelHeight*(m_posMM.y+iDY)/m_tgaMinimap.m_iHeight;

	return	TRUE;
}

void
cMAP::updateMinimap(cRECT *_lpFrame,CPos *_lpPosHero)
{
	if	(!m_tgaMinimap.m_p16IMAGE)
		return;

	m_iMMDx			=	0;
	m_iMMDy			=	0;

	s_mmFrame.set(_lpFrame);

	m_iMMFrameWidth	=	s_mmFrame.getWidth();
	m_iMMFrameHeight=	s_mmFrame.getHeight();

	m_iMMFrameWidth	=	min(m_iMMFrameWidth	,m_tgaMinimap.m_iWidth);
	m_iMMFrameHeight=	min(m_iMMFrameHeight,m_tgaMinimap.m_iHeight);

	m_posMM.x		=	_lpPosHero->x*m_tgaMinimap.m_iWidth/m_iPixelWidth- m_iMMFrameWidth/2;
	m_posMM.y		=	_lpPosHero->y*m_tgaMinimap.m_iHeight/m_iPixelHeight- m_iMMFrameHeight/2;

	if	(m_posMM.x	<										0	)
		m_iMMDx	=	-m_posMM.x;
	if	(m_posMM.y	<										0	)
		m_iMMDy	=	-m_posMM.y;
	if	(m_posMM.x	>	m_tgaMinimap.m_iWidth-m_iMMFrameWidth	)
		m_iMMDx	=	(m_tgaMinimap.m_iWidth-m_iMMFrameWidth)-m_posMM.x;
	if	(m_posMM.y	>	m_tgaMinimap.m_iHeight-m_iMMFrameHeight	)
		m_iMMDy	=	(m_tgaMinimap.m_iHeight-m_iMMFrameHeight)-m_posMM.y;

	m_posMM.x			+=	m_iMMDx;
	m_posMM.y			+=	m_iMMDy;

	if	(m_posMM.y	<	0)
		m_posMM.y		=	0;
	if	(m_posMM.x	<	0)
		m_posMM.x		=	0;

}

//
//	미니맵 출력
BOOL
cMAP::drawMinimap(BOOL _bIsHalfBlending,BOOL _bIsView)
{
	if (!m_tgaMinimap.m_p16IMAGE)
		return FALSE;

	CPos	pos;
	int		i;
	BOOL	bIsFocus			=	FALSE;
	BOOL	bIsDrawFirstAreaName=	FALSE;

	if (s_mmFrame.isIn(s_posMouse.x,s_posMouse.y))
		bIsDrawFirstAreaName	=	TRUE;

	if (_bIsView == FALSE)
	{
		if (_bIsHalfBlending)
			cDRAW::Fill(0,s_mmFrame.x1,s_mmFrame.y1,s_mmFrame.x1+m_iMMFrameWidth,s_mmFrame.y1+m_iMMFrameHeight);	
		else
			cDRAW::FillHB(0,s_mmFrame.x1,s_mmFrame.y1,s_mmFrame.x1+m_iMMFrameWidth,s_mmFrame.y1+m_iMMFrameHeight);	
	}
	else
	{
		if (_bIsHalfBlending)
			cDRAW::PutClippedImageHB(s_mmFrame.x1,s_mmFrame.y1,m_iMMFrameWidth,m_iMMFrameHeight,m_tgaMinimap.m_p16IMAGE,m_posMM.x,m_posMM.y);	
		else
			cDRAW::PutClippedImage(s_mmFrame.x1,s_mmFrame.y1,m_iMMFrameWidth,m_iMMFrameHeight,m_tgaMinimap.m_p16IMAGE,m_posMM.x,m_posMM.y);
	}

	cDRAW::SetClippginArea(s_mmFrame.x1,s_mmFrame.y1,s_mmFrame.x2,s_mmFrame.y2);

	int		iFPS,iFrameCount,iFrame;

	cRECT	rectForMark;
	cRECT	rectFrame;

	rectFrame.set(m_posMM.x,m_posMM.y,m_posMM.x+s_mmFrame.getWidth(),m_posMM.y+s_mmFrame.getHeight());
	rectForMark.set(&rectFrame);

	rectForMark.x1	-=	5;
	rectForMark.y1	-=	5;
	rectForMark.x2	+=	5;
	rectForMark.y2	+=	5;

	if (bIsDrawFirstAreaName)
	{
		int	iNamedAreaWidth	=	80;
		int	iNamedAreaHeight=	32;

		CGame::s_sText.setClipArea(&s_mmFrame);

		for (i=0;i<m_area.m_wNamedAreaCount;i++)
		{
			cAreaInfo	*lpArea	=	m_area.getNameArea(i);
			CPos		pos;
			cRECT		rect;

			lpArea->getCenter(&pos);

			pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
			pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;

			rect.set(pos.x,pos.y,pos.x+iNamedAreaWidth,pos.y+iNamedAreaHeight);
			if (!rect.isCrash(&rectForMark))
				continue;

			pos.x	-=	m_posMM.x;
			pos.y	-=	m_posMM.y;

			pos.x	=	pos.x+s_mmFrame.x1-iNamedAreaWidth/2-6;
			pos.y	=	pos.y+s_mmFrame.y1-iNamedAreaHeight/2+2;

			int	iAreaNameSize	=	strlen(lpArea->m_strName);

			CGame::s_sText.c2LinePut(pos.x,pos.y,100,32,0,WHITE,dFITTEXT_CENTER, lpArea->m_strName);
		}

		CGame::s_sText.releaseClipArea();
	}
#ifdef	_DEBUG
// 	for (i=0;i<dMAX_AREA;i++)	//	테스트
// 	{
// 		cAreaInfo	*lpArea	=	&m_area.m_aArea[i];
// 
// 		if	(lpArea->m_wSerial	==	0xffff)
// 			continue;
// 
// 		if	(STRICMP(lpArea->m_strName,"용거북") == NULL)
// 		{
// 			cRECT		rect;
// 
// 			rect.x1	=	lpArea->x1;
// 			rect.y1	=	lpArea->y1;
// 			rect.x2	=	lpArea->x2;
// 			rect.y2	=	lpArea->y2;
// 
// 			rect.x1	=	rect.x1*m_tgaMinimap.m_iWidth/m_iPixelWidth;
// 			rect.y1	=	rect.y1*m_tgaMinimap.m_iHeight/m_iPixelHeight;
// 			rect.x2	=	rect.x2*m_tgaMinimap.m_iWidth/m_iPixelWidth;
// 			rect.y2	=	rect.y2*m_tgaMinimap.m_iHeight/m_iPixelHeight;
// 
// 			rect.x1	-=	m_posMM.x;
// 			rect.y1	-=	m_posMM.y;
// 			rect.x2	-=	m_posMM.x;
// 			rect.y2	-=	m_posMM.y;
// 
// 			rect.x1	=	rect.x1	+s_mmFrame.x1;
// 			rect.y1	=	rect.y1	+s_mmFrame.y1;
// 			rect.x2	=	rect.x2	+s_mmFrame.x1;
// 			rect.y2	=	rect.y2	+s_mmFrame.y1;
// 			
// 			cDRAW::Box(_LTGREEN,rect.x1,rect.y1,rect.x2,rect.y2);
// 		}
// 	}
#endif

	for (i=0;i<m_area.m_wTouhableAreaCount;i++)
	{
		int			iArea	=	m_area.m_aTouchableArea[i];
		int			iAnm	=	eEANM_MINIMAP_GATE;
		CPos		pos;
		cAreaInfo	*lpArea	=	&m_area.m_aArea[iArea];
		BOOL		bIsSecretDungeonGate	=	FALSE;

		if (lpArea->m_wKind	==	dAREA_DOOR)
		{
			cAreaDoorInfo	*lpDoor	=	(cAreaDoorInfo	*)lpArea;

			if (iArea	==	m_area.m_wInstanceFieldGate	||	g_hero.getSecretDungeonKey(lpDoor->m_wLinkSecretDungeon))
				bIsSecretDungeonGate	=	TRUE;
		}

		if	(m_area.m_aArea[iArea].m_bf1IsTestPortal	&&	m_area.m_aArea[iArea].m_bf15LinkObject	==	0x7000	&&	g_bIsTestServer	==	FALSE)
			continue;

		if	(bIsSecretDungeonGate	==	FALSE	&&	m_area.m_aArea[iArea].m_wKind	!=	dAREA_PORTAL)
			continue;

		if	(bIsSecretDungeonGate)
			iAnm	=	eEANM_MINIMAP_WEAPON_GATE;
		else
		switch(m_area.m_aArea[iArea].m_gateShape)
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
			// insu add
			case	ePAS_INNER_DUNGEON	:
				iAnm	=	eEANM_MINIMAP_INNER_DUNGEON;
				break;
			case	ePAS_INNER_TOWER	:
				iAnm	=	eEANM_MINIMAP_INNER_TOWER;
				break;
			case	ePAS_INNER_BUILDING	:
				iAnm	=	eEANM_MINIMAP_INNER_BUILDING;
				break;
			// insu add
		}

		m_area.m_aArea[iArea].getCenter(&pos);

		pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
		pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;

		if (!rectForMark.isIn(pos.x,pos.y))
			continue;

		pos.x	-=	m_posMM.x;
		pos.y	-=	m_posMM.y;

		iFPS		=	g_anmEtc.GetFPS(iAnm);
		iFrameCount	=	g_anmEtc.GetFrameCount(iAnm);
		iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount;

		// insu add
		if (iAnm == eEANM_MINIMAP_INNER_DUNGEON || iAnm == eEANM_MINIMAP_INNER_TOWER || iAnm == eEANM_MINIMAP_INNER_BUILDING)
		{
			pos.x -= (g_anmEtc.getSpriteWidth(iAnm, 0, 0)/3);
			pos.y -= (g_anmEtc.getSpriteHeight(iAnm, 0, 0)/3);

			g_anmEtc.Put(s_mmFrame.x1+pos.x+1,s_mmFrame.y1+pos.y+1,iAnm,0,iFrame,75,75,dPUT_NORMAL,0); 
			g_anmEtc.PutReg(s_mmFrame.x1+pos.x,s_mmFrame.y1+pos.y,iAnm,0,iFrame, 75, 75);
		}
		else
		{
			g_anmEtc.Put(s_mmFrame.x1+pos.x+1,s_mmFrame.y1+pos.y+1,iAnm,0,iFrame,100,100,dPUT_NORMAL,0); 
			g_anmEtc.PutReg(s_mmFrame.x1+pos.x,s_mmFrame.y1+pos.y,iAnm,0,iFrame);
		}
		// insu add end
	}

	for (i=0;i<g_am.m_iActorCount;i++)
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
			if	(stricmp(lpstrName,"콜린")	==	0)
				lpstrName	=	lpstrName;

			CQuestItem	*lpQuest=	g_hero.getProprietyQuest(lpstrName,lpActor->m_wNpcActorSerial);

			if	(g_hero.isProcessQeustClient(lpstrName,lpActor->m_wNpcActorSerial))
				iMark	=	eNPC_PROCESS_QUEST_CLIENT;
			else
			if	(g_hero.isNpcAboutProcessQuest(lpstrName,lpActor->m_wNpcActorSerial))
				iMark	=	eNPC_PROCESS_QUEST_REFERENCE_NPC;
			else
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
		else
		if	(lpActor->m_wActorKind	>=	eNPC_PROCESS_QUEST_CLIENT	&&	lpActor->m_wActorKind	<=	eNPC_PROCESS_QUEST_TARGET_MONSTER	)
			iMark	=	lpActor->m_wActorKind;

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


#ifdef	_DEBUG
		if	(g_strMinimapCheckNpc[0] && STRICMP(lpActor->m_strName,g_strMinimapCheckNpc) == 0)//테스트
			iMark	=	eNPC_FIRST_HELPER;
#endif

		if	(iMark	>= eNPC_WEAPON_MERCHANT)
		{
			int		iKind	=	iMark	-	eNPC_WEAPON_MERCHANT;

			CPos	pos;

			pos.x	=	lpActor->m_pos.x;
			pos.y	=	lpActor->m_pos.y;

			pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
			pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;

			if	(!rectForMark.isIn(pos.x,pos.y))
				continue;

			pos.x		-=	m_posMM.x;
			pos.y		-=	m_posMM.y;

			g_sprInterface2.Put(s_mmFrame.x1+pos.x+1,s_mmFrame.y1+pos.y+1,eMDII_WEAPON_SHOP+iKind,100,100,dPUT_COLOR,0);
			g_sprInterface2.Put(s_mmFrame.x1+pos.x,s_mmFrame.y1+pos.y,eMDII_WEAPON_SHOP+iKind);
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

			if	(!rectForMark.isIn(pos.x,pos.y))
				continue;

			pos.x	-=	m_posMM.x;
			pos.y	-=	m_posMM.y;

			pos.x	=	s_mmFrame.x1+pos.x;
			pos.y	=	s_mmFrame.y1+pos.y;

			if	(g_bIsDuelServer && g_duelManager.m_uiBeginBattleTime	&&	g_hero.m_wOperatorLevel	>=	4)
				g_sprInterface.Put(pos.x,pos.y-2,eSN_1+lpActor->m_wTeam);
			else
			if	(s_bIsGuildBattleField && g_hero.m_wOperatorLevel	>=	4)
				g_sprInterface.Put(pos.x,pos.y-2,eSN_1+lpActor->m_wTeam);
			else
			if	(bIsGuildObject)
			{
				cDRAW::XLine(_SKYBLUE,pos.x-1,pos.x+1,pos.y);
				cDRAW::YLine(_SKYBLUE,pos.y-1,pos.y+1,pos.x);
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

					g_sprInterface.Put(pos.x,pos.y-2,eSN_1+iIndex);
				}
			}
			else
			{
				cDRAW::XLine(_LTGREEN,pos.x-1,pos.x+1,pos.y);
				cDRAW::YLine(_LTGREEN,pos.y-1,pos.y+1,pos.x);
			}
		}
	}

	drawHuntingArea(&rectFrame);
	drawQuestLinkDungeonArrow(&rectForMark);
	drawQuestMonsterArea(&rectForMark);
	// insu add
	for (i=0;i<g_am.m_iActorCount;i++)		// 캐릭터 수
	{
		CActor	*lpActor	=	&g_aActor[g_am.m_aActorList[i]];
		
		if	(lpActor->isHero())
			continue;
		
		int	iMark	=	-1;
		
		
		switch(lpActor->m_wActorKind)
		{
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
				if	(g_hero.isNpcAboutProcessQuest(lpstrName,lpActor->m_wNpcActorSerial))
					iMark	=	eNPC_PROCESS_QUEST_REFERENCE_NPC;
				else
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
		else
			if	(lpActor->m_wActorKind	>=	eNPC_PROCESS_QUEST_CLIENT	&&	lpActor->m_wActorKind	<=	eNPC_PROCESS_QUEST_TARGET_MONSTER	)
				iMark	=	lpActor->m_wActorKind;
			
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
			
#ifdef	_DEBUG
			if	(g_strMinimapCheckNpc[0] && STRICMP(lpActor->m_strName,g_strMinimapCheckNpc) == 0)//테스트
				iMark	=	eNPC_FIRST_HELPER;
#endif
			
			if	(iMark	>= eNPC_WEAPON_MERCHANT)
			{
				int		iKind	=	iMark	-	eNPC_WEAPON_MERCHANT;
				
				CPos	pos;
				
				pos.x	=	lpActor->m_pos.x;
				pos.y	=	lpActor->m_pos.y;
				
				pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
				pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;
				
				if	(!rectForMark.isIn(pos.x,pos.y))
				{
					if(iMark == eNPC_PROCESS_QUEST_REFERENCE_NPC || iMark == eNPC_PROCESS_QUEST_CLIENT)
					{
						// insu modify
						if (lpActor->m_wActorKind != eNPC_MONSTER)
							drawMinimapOffscreenQuestNpcArrow(lpstrName, &pos, &rectForMark);
						// insu modify end
					}
					continue;
				}
				pos.x		-=	m_posMM.x;
				pos.y		-=	m_posMM.y;
				
				if(iMark == eNPC_PROCESS_QUEST_REFERENCE_NPC || iMark == eNPC_PROCESS_QUEST_CLIENT)
				{
					// insu modify
					if (lpActor->m_wActorKind != eNPC_MONSTER)
						drawMinimapOnscreenQuestNpcArrow(lpstrName, &pos, &s_mmFrame);
					// insu modify end
				}
				
			}
	}
	// insu add end


//	디텍팅 이블
	for (i=0;i<g_am.m_iDetectingActorCount;i++)
	{
		CPos	pos;

		pos.x	=	g_am.m_aDetetingActorList[i].bf16X;
		pos.y	=	g_am.m_aDetetingActorList[i].bf15Y;

		pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
		pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;

		if (!rectForMark.isIn(pos.x,pos.y))
			continue;

		pos.x	-=	m_posMM.x;
		pos.y	-=	m_posMM.y;

		pos.x	=	s_mmFrame.x1+pos.x;
		pos.y	=	s_mmFrame.y1+pos.y;
		
		cDRAW::XLine(_LTRED,pos.x-1,pos.x+1,pos.y);
		cDRAW::YLine(_LTRED,pos.y-1,pos.y+1,pos.x);
	}

	if (g_am.m_posStress.x	!=	-1)
	{
		CPos	pos;

		pos.x	=	g_am.m_posStress.x;
		pos.y	=	g_am.m_posStress.y;

		pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
		pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;

		if (rectForMark.isIn(pos.x,pos.y))
		{
			int	iBeginImage	=	56;
			int	iEndImage	=	65;
			
			pos.x		-=	m_posMM.x;
			pos.y		-=	m_posMM.y;

			pos.x		=	s_mmFrame.x1+pos.x;
			pos.y		=	s_mmFrame.y1+pos.y;

			iFPS		=	24;
			iFrameCount	=	eMDII_STRESS_END-eMDII_STRESS_BEGIN+1;
			iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%(iFrameCount+iFrameCount-2);

			if (iFrame	>=	iFrameCount)
				iFrame	=	iFrameCount-(iFrame-iFrameCount+1);

			g_sprInterface2.Put(pos.x+1,pos.y+1,iFrame+eMDII_STRESS_BEGIN,100,100,dPUT_COLOR,0);
			g_sprInterface2.Put(pos.x,pos.y,iFrame+eMDII_STRESS_BEGIN);
		}
	}

	if (CGamePlay::s_bIsOXQuizField && g_hero.isOperator())
	{
		for (i=0;i<g_am.m_iActorCount;i++)
		{
			CActor	*lpActor	=	&g_aActor[g_am.m_aActorList[i]];

			if (lpActor->isHighOperator())
				continue;

			CPos	pos;

			pos.x	=	lpActor->m_pos.x;
			pos.y	=	lpActor->m_pos.y;

			pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
			pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;

			if	(!rectForMark.isIn(pos.x,pos.y))
				continue;

			pos.x	-=	m_posMM.x;
			pos.y	-=	m_posMM.y;

			pos.x	=	s_mmFrame.x1+pos.x;
			pos.y	=	s_mmFrame.y1+pos.y;

			WORD	wColor	=	_LTGRAY;

			if (lpActor->m_bf1IsQuizEventLoser == TRUE)
			{
				cDRAW::XLine(_LTRED,pos.x-1,pos.x+1,pos.y);
				cDRAW::YLine(_LTRED,pos.y-1,pos.y+1,pos.x);
			}
			else
			if (lpActor->m_bf1IsQuizEventWinner == FALSE)
			{
				cDRAW::XLine(_LTGREEN,pos.x-1,pos.x+1,pos.y);
				cDRAW::YLine(_LTGREEN,pos.y-1,pos.y+1,pos.x);
			}
		}
	}

	cArcaInfo	*lpTreasureBox	=	g_arcaManager.getArca(0x7fff);

	if	(lpTreasureBox)
	{
		CPos	pos;

		pos.x	=	lpTreasureBox->m_wX;
		pos.y	=	lpTreasureBox->m_wY;

		pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
		pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;

		if	(rectForMark.isIn(pos.x,pos.y))
		{
			pos.x	-=	m_posMM.x;
			pos.y	-=	m_posMM.y;

			pos.x	=	s_mmFrame.x1+pos.x;
			pos.y	=	s_mmFrame.y1+pos.y;

			g_sprInterface2.Put(pos.x+1,pos.y+1,eMDII_TREASURE_ARCA,100,100,dPUT_COLOR,0);
			g_sprInterface2.Put(pos.x  ,pos.y  ,eMDII_TREASURE_ARCA);
		}
	}

	pos.x		=	s_mmFrame.x1+m_iMMFrameWidth/2-m_iMMDx;
	pos.y		=	s_mmFrame.y1+m_iMMFrameHeight/2-m_iMMDy;

	iFPS		=	g_anmEtc.GetFPS(eEANM_MINIMAP_HERO_SHINING);
	iFrameCount	=	g_anmEtc.GetFrameCount(eEANM_MINIMAP_HERO_SHINING);
	iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount;

	cDRAW::XLine(LTGREEN,pos.x-1,pos.x+1,pos.y);
	cDRAW::YLine(LTGREEN,pos.y-1,pos.y+1,pos.x);
	g_anmEtc.PutReg(pos.x,pos.y,eEANM_MINIMAP_HERO_SHINING,0,iFrame);

	if (bIsDrawFirstAreaName	==	FALSE)
	{
		int	iNamedAreaWidth	=	80;
		int	iNamedAreaHeight=	32;

		CGame::s_sText.setClipArea(&s_mmFrame);

		for (i=0;i<m_area.m_wNamedAreaCount;i++)
		{
			cAreaInfo	*lpArea	=	m_area.getNameArea(i);
			CPos		pos;
			cRECT		rect;

			lpArea->getCenter(&pos);

			pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
			pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;

			rect.set(pos.x,pos.y,pos.x+iNamedAreaWidth,pos.y+iNamedAreaHeight);
			if (!rect.isCrash(&rectForMark))
				continue;

			pos.x	-=	m_posMM.x;
			pos.y	-=	m_posMM.y;

			pos.x	=	pos.x+s_mmFrame.x1-iNamedAreaWidth/2-6;
			pos.y	=	pos.y+s_mmFrame.y1-iNamedAreaHeight/2+2;

			int	iAreaNameSize	=	strlen(lpArea->m_strName);

			CGame::s_sText.c2LinePut(pos.x,pos.y,100,32,0,WHITE,dFITTEXT_CENTER, lpArea->m_strName);
		}

		CGame::s_sText.releaseClipArea();
	}

	cDRAW::FreeClippginArea();

	return	TRUE;
}


void
cMAP::drawQuestMonsterArea(cRECT* _rectForMark, BOOL _bFogmap)
{	
	if (!m_bmCheckBox.isCheck(eMMOPTION_QUEST_MONSTER))
	return;
	
	CQuest	*lpQuestManager;
	int iIndex = 0;
	int iLimit = 0;
	int iQMonsterCnt = 0; // 퀘스트 관련 몬스터수
	cSTRING strQMonster[dMAX_QUEST_MONSTER];	// 퀘스트 관련 몬스터 이름
	vector<int> vtMoveArea;
	
	if (g_map.m_iSelectQuest < dMAX_PROCESS_QUEST_COUNT)
	{
		iIndex = g_map.m_iSelectQuest;
		iLimit = 1;
	}
	if (g_map.m_iSelectQuest == dMAX_PROCESS_QUEST_COUNT)
	{
		iIndex = 0;
		iLimit = dMAX_PROCESS_QUEST_COUNT;
	}
	for(int i=0 ; i<iLimit ; i++) 
	{
		if	(g_map.m_iQuestIndex[iIndex] == 0xffff)
			continue;
		if	(g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questKind	==	eQT_NORMAL	)
			lpQuestManager	=	&g_quest;
		if	(g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questKind	==	eQT_ARBEIT	)
			lpQuestManager	=	&g_questArbeit;
		
		int iQuestIndex	= g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questIndex;
		int iProcessLevel = g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questProcessLevel;
		CQuestItem	*lpQuest	=	lpQuestManager->getQuest(iQuestIndex);						
		CQuestItem::cQuestInfo	*lpQuestInfo	= lpQuest->getInfo(iProcessLevel);
		
		BOOL bAdd = FALSE;
		for(int j=0 ; j<lpQuestInfo->m_wLinkFieldCount ; j++)
		{
			if (lpQuestInfo->m_pLinkField[j] == g_hero.m_wCurrentField)
				bAdd = TRUE;
		}
		
		if (!bAdd)
		{
			iIndex++;
			continue;
		}

		if(lpQuestInfo->m_wTargetMonsterCount)
		{
			for(int j=0 ; j<lpQuestInfo->m_wTargetMonsterCount ; j++)
			{
				strQMonster[iQMonsterCnt].Add("%s", lpQuestInfo->m_pTargetMonsterList[j].m_pStrName);
				iQMonsterCnt++;
			}
		}
		iIndex++;
	}

	for(i=0 ; i<c_iMaxFieldActorCount ; i++) // 일단 맵에 있는 액터자료를 기반으로 출력
	{		
		if (g_map.m_aSaveActor[i].m_iSerial == -1)
			continue;

		char* strName = g_map.m_aSaveActor[i].m_strName;

		for(int j=0 ; j<iQMonsterCnt ; j++)
		{
			if(stricmp(strName, strQMonster[j].String) == 0)
			{
				vtMoveArea.push_back(g_map.m_aSaveActor[i].m_wMoveArea);				
			}
		}
	}

	for(i=0 ; i<vtMoveArea.size() ; i++)
	{
		cRECT rt;
		rt.set(	g_map.m_area.m_aArea[vtMoveArea[i]].x1*m_tgaMinimap.m_iWidth/m_iPixelWidth - m_posMM.x + s_mmFrame.x1,
				g_map.m_area.m_aArea[vtMoveArea[i]].y1*m_tgaMinimap.m_iHeight/m_iPixelHeight - m_posMM.y + s_mmFrame.y1,
				g_map.m_area.m_aArea[vtMoveArea[i]].x2*m_tgaMinimap.m_iHeight/m_iPixelHeight - m_posMM.x + s_mmFrame.x1,
				g_map.m_area.m_aArea[vtMoveArea[i]].y2*m_tgaMinimap.m_iHeight/m_iPixelHeight - m_posMM.y + s_mmFrame.y1);

		if (_bFogmap)
		{
			rt.set(	_rectForMark->x1+g_map.m_area.m_aArea[vtMoveArea[i]].x1*m_tgaMinimap.m_iWidth/m_iPixelWidth,
					_rectForMark->y1+g_map.m_area.m_aArea[vtMoveArea[i]].y1*m_tgaMinimap.m_iHeight/m_iPixelHeight,
					_rectForMark->x1+g_map.m_area.m_aArea[vtMoveArea[i]].x2*m_tgaMinimap.m_iHeight/m_iPixelHeight,
					_rectForMark->y1+g_map.m_area.m_aArea[vtMoveArea[i]].y2*m_tgaMinimap.m_iHeight/m_iPixelHeight);
			if (_rectForMark->x1 < rt.x1+5)
				rt.add(5, 0);
		}
	
		int iFPS		=	g_anmEtc.GetFPS(eEANM_MINIMAP_HERO_SHINING);
		int iFrameCount	=	g_anmEtc.GetFrameCount(eEANM_MINIMAP_HERO_SHINING);
		int iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount;
		
		if (!_bFogmap)
		{
			if (s_mmFrame.x1 < rt.x1+5)
				rt.add(5, 0);
		}

		g_anmEtc.Put(rt.x2-(rt.x2-rt.x1)/2, rt.y2-(rt.y2-rt.y1)/2, eEANM_MINIMAP_HERO_SHINING, 0, iFrame, 100, 100, dPUT_EXTREAM_LIGHT, 5); // dPUT_EXTREAM_LIGHT
	}
	return;
}





// insu function add
//┌---------------------------------------------------------------┐
//│ _iIndex 로 이동하는 포탈이 있는지 확인한다.                   │
//│                                                               │
//│ int _iIndex          :  맵인덱스                              │
//└---------------------------------------------------------------┘
BOOL
cMAP::isPortalDest(int _iIndex)
{
	for (int i=0 ; i<dMAX_AREA ; i++)
	{
		cAreaInfo	*lpArea	=	&m_area.m_aArea[i];
		if (lpArea->m_wKind != dAREA_PORTAL)
			continue;

		if (lpArea->m_pstrString == NULL)
			continue;
		
		if (lpArea->m_gateShape == ePAS_HIDDEN || lpArea->m_gateShape == ePAS_HIDDEN_INNER_PORTAL || lpArea->m_gateShape == ePAS_ONE_WAY_GATE)
			continue;

		char Buffer[4];
		strncpy(Buffer, &lpArea->m_pstrString[1], 3);

		if (_iIndex == atoi(Buffer))
			return TRUE;
	}
	return FALSE;
}
// insu function add end







// insu function add
void
cMAP::setQuestLinkFieldIndex()
{
	m_vtPortaldestIndex.clear();
	for (int i=0 ; i<dMAX_AREA ; i++)
	{
		cAreaInfo	*lpArea	=	&m_area.m_aArea[i];
		if (lpArea->m_wKind != dAREA_PORTAL)	// 포탈 area 만 걸러낸다.
			continue;

		if (lpArea->m_pstrString == NULL)
			continue;
		
		if (lpArea->m_gateShape >= ePAS_ONE_WAY_GATE && lpArea->m_gateShape <= ePAS_HIDDEN_INNER_PORTAL)
			continue;

		char Buffer[4];
		strncpy(Buffer, &lpArea->m_pstrString[1], 3);

		int iIndex = g_gwWorldMap.getInfoSurfaceFieldIndex(atoi(Buffer));
		m_vtPortaldestIndex.push_back(iIndex);		
	}
}
// insu function add end



// insu function add
int
cMAP::getDungeonFirstFloor(const int _iMapIndex)
{
	char* lpstr = &g_aMapSimpleInfo[_iMapIndex].m_strFileName[5];
	int istrLen = strlen(lpstr)-5;

	char strFirstFloorName[dMAP_NAME_LENGTH];
	char strFirstFloorNameF[dMAP_NAME_LENGTH];
	memset(strFirstFloorName, '\0', dMAP_NAME_LENGTH);
	memset(strFirstFloorNameF, '\0', dMAP_NAME_LENGTH);
	
	for(int i=istrLen-1 ; i>0 ; i--)
	{
		if (lpstr[i] == 'B' || lpstr[i] == 'F') // B나 F를 찾는다.
		{
			if (i+1 == istrLen-5)
				return _iMapIndex;

			if (lpstr[i+1] >= 64 && lpstr[i+1] <= 122)
				return _iMapIndex;

			strncpy(strFirstFloorName, lpstr, i+1);
			strncpy(strFirstFloorNameF, lpstr, i);
			if (lpstr[i] == 'B')
				strcat(strFirstFloorNameF, "F");
			if (lpstr[i] == 'F')
				strcat(strFirstFloorNameF, "F");
			char* str = &lpstr[i+1];
			int iIndex = atoi(str);
			if (iIndex == 1)
			{
				strcat(strFirstFloorNameF, "01.rmd");
				strcat(strFirstFloorName, "01.rmd");
			}
			if (iIndex != 1)
			{
				strcat(strFirstFloorName, "01.rmd");
				strcat(strFirstFloorNameF, "01.rmd");
			}
			for(int j=0 ; j<dMAX_MAP_COUNT ; j++)
			{
				char* strFName = &g_aMapSimpleInfo[j].m_strFileName[5];
				if (!stricmp(strFName, strFirstFloorNameF))
					return g_aMapSimpleInfo[j].m_wSerial;
			}
			for(j=0 ; j<dMAX_MAP_COUNT ; j++)
			{
				char* strFName = &g_aMapSimpleInfo[j].m_strFileName[5];
				if (!stricmp(strFName, strFirstFloorName))
				{					
					return g_aMapSimpleInfo[j].m_wSerial;
				}
			}
		}
		if (lpstr[i] == '_') // _ 전까지만 검사한다.
			break;
	}

	return _iMapIndex;
}
// insu function add end


//
//	미니맵 출력
BOOL
cMAP::drawEventNpcToolTip()
{
	if (!m_tgaMinimap.m_p16IMAGE)
		return FALSE;

	if (g_map.m_bOptionState)
		return FALSE;

	int		i;
	BOOL	bIsFocus	=	FALSE;

	cRECT		rectFrame;

	rectFrame.set(m_posMM.x,m_posMM.y,m_posMM.x+s_mmFrame.getWidth(),m_posMM.y+s_mmFrame.getHeight());

	for (i=0;i<g_am.m_iActorCount;i++)
	{
		CActor	*lpActor	=	&g_aActor[g_am.m_aActorList[i]];

		if	(lpActor->isHero())
			continue;

		int		iMark		=	-1;

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
			iMark	=	lpActor->m_wActorKind;
			break;
		}
		char	*lpstrName	=	lpActor->getName();

		if	(lpActor->isNpc() && lpstrName)
		{
			CQuestItem	*lpQuest=	g_hero.getProprietyQuest(lpstrName,lpActor->m_wNpcActorSerial);

			if	(g_hero.isProcessQeustClient(lpstrName,lpActor->m_wNpcActorSerial))
				iMark	=	eNPC_PROCESS_QUEST_CLIENT;
			else
			if	(g_hero.isNpcAboutProcessQuest(lpstrName,lpActor->m_wNpcActorSerial))
				iMark	=	eNPC_PROCESS_QUEST_REFERENCE_NPC;
			else
			if (lpQuest)
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
		else
		if	(lpActor->m_wActorKind	>=	eNPC_PROCESS_QUEST_CLIENT	&&	lpActor->m_wActorKind	<=	eNPC_PROCESS_QUEST_TARGET_MONSTER	)
			iMark	=	lpActor->m_wActorKind;

		if	(lpActor->m_wActorKind	==	eNPC_FIRST_HELPER)
			iMark	=	eNPC_FIRST_HELPER;

		if	(iMark	==	-1)
		{	// 수정..
			if	(
				(lpActor->m_wActorKind	>=	eNPC_WEAPON_MERCHANT	&&	lpActor->m_wActorKind	<=	eNPC_BANK) ||
				 (lpActor->m_wActorKind	>=	eNPC_DOUMI	&&	lpActor->m_wActorKind	<=	eNPC_FIRST_HELPER) ||
				 (lpActor->m_wActorKind	>=	eNPC_WEAPON_MERCHANT_KNIGHT_WARRIOR	&&	lpActor->m_wActorKind	<=	eNPC_WEAPON_MERCHANT_NECROMANCER_DEVIL) ||
				 (lpActor->m_wActorKind == eNPC_WEAPON_MERCHANT_SOUL_BRINGER) ||
				 (lpActor->m_wActorKind	>= eNPC_GUILD_HALL_TELEPORTER &&	lpActor->m_wActorKind	<= eNPC_EVENT_HELPER) ||
				 (lpActor->m_wActorKind	== eNPC_ADVENTURER_SOCIETY) || (lpActor->m_wActorKind == eNPC_WEAPON_MERCHANT_OPTICALIST)
				 )
				iMark	=	lpActor->m_wActorKind;
		}

		if	(iMark	>= eNPC_WEAPON_MERCHANT)
		{
			CPos	pos;

			pos.x	=	lpActor->m_pos.x;
			pos.y	=	lpActor->m_pos.y;

			pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
			pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;

			if	(!rectFrame.isIn(pos.x,pos.y))
				continue;

			pos.x	-=	m_posMM.x;
			pos.y	-=	m_posMM.y;

			cRECT	rect;

			rect.set(s_mmFrame.x1+pos.x-5,s_mmFrame.y1+pos.y-5,s_mmFrame.x1+pos.x+5,s_mmFrame.y1+pos.y+5);

			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				int		iPixelSize	=	s_text.getPixelSize(g_pstrNpcName[iMark])+10;
				int		iX			=	s_mmFrame.x1+pos.x;
				int		iY			=	s_mmFrame.y1+pos.y-12-5;

				if	(iX	+	iPixelSize/2+4	>=	cDRAW::WIDTH)
					iX	=	cDRAW::WIDTH	-	(iPixelSize/2+4);

				if	(iX	-	iPixelSize/2	<	0)
					iX	=	iPixelSize/2;

				iY	=	max(iY,10);

				rect.set(iX-iPixelSize/2,iY-5,iX+iPixelSize/2,iY+19-4);

				s_ttCommon.drawBox(&rect,0,32);

				s_text.cPut(iX,iY,WHITE,g_pstrNpcName[iMark]);
			}
		}
	}

	cArcaInfo	*lpTreasureBox	=	g_arcaManager.getArca(0x7fff);

	if	(lpTreasureBox)
	{
		CPos	pos;

		pos.x	=	lpTreasureBox->m_wX;
		pos.y	=	lpTreasureBox->m_wY;

		pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
		pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;

		pos.x	-=	m_posMM.x;
		pos.y	-=	m_posMM.y;

		cRECT	rect;

		rect.set(s_mmFrame.x1+pos.x-5,s_mmFrame.y1+pos.y-5,s_mmFrame.x1+pos.x+5,s_mmFrame.y1+pos.y+5);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			char	*lpstrText	=	dMSG_TREASURE_ARCA;
			int		iPixelSize	=	s_text.getPixelSize(lpstrText)+10;

			int		iWidth		=	min(100,iPixelSize);
			int		iPixelHeight=	s_text.getTextHeight(iWidth,lpstrText);
			int		iX			=	s_mmFrame.x1+pos.x;
			int		iY			=	s_mmFrame.y1+pos.y-iPixelHeight-6-5;

			if	(iX	+	iWidth/2+4+4	>=	cDRAW::WIDTH)
				iX	=	cDRAW::WIDTH	-	(iWidth/2+4+4);

			if	(iX-iWidth/2-4	<	0)
				iX	=	iWidth/2+4+4;

			iY	=	max(iY,10);

			rect.set(iX-iWidth/2-4,iY-5,iX+iWidth/2+4,iY+iPixelHeight+5);

			s_ttCommon.drawBox(&rect,0,32);

			s_text.cPut(rect.x1+5,rect.y1+6,iWidth,100,0,WHITE,dFITTEXT_WIDTH_CENTER,dMSG_TREASURE_ARCA);
		}
	}

	if	(s_bIsSecretDungeon)
		return	TRUE;

	for (i=0;i<m_area.m_wTouhableAreaCount;i++)
	{
		int			iArea	=	m_area.m_aTouchableArea[i];
		CPos		pos;
		BOOL		bIsSecretDungeonGate	=	FALSE;
		cAreaInfo	*lpArea	=	&m_area.m_aArea[iArea];

		if (lpArea->m_wKind	==	dAREA_DOOR)
		{
			cAreaDoorInfo	*lpDoor	=	(cAreaDoorInfo	*)lpArea;

			if (iArea	==	m_area.m_wInstanceFieldGate	||	g_hero.getSecretDungeonKey(lpDoor->m_wLinkSecretDungeon))
				bIsSecretDungeonGate	=	TRUE;
		}

		if	(m_area.m_aArea[iArea].m_bf1IsTestPortal	&&	m_area.m_aArea[iArea].m_bf15LinkObject	==	0x7000	&&	g_bIsTestServer	==	FALSE)
			continue;

		if	(bIsSecretDungeonGate	==	FALSE	&&	m_area.m_aArea[iArea].m_wKind	!=	dAREA_PORTAL)
			continue;
		if	(m_area.m_aArea[iArea].m_gateShape	==	ePAS_LEAVE_GUILD_HALL)
			continue;

		m_area.m_aArea[iArea].getCenter(&pos);

		pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
		pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;

		if	(!rectFrame.isIn(pos.x,pos.y))
			continue;

		pos.x	-=	m_posMM.x;
		pos.y	-=	m_posMM.y;

		cRECT	rect;

		rect.set(s_mmFrame.x1+pos.x-5,s_mmFrame.y1+pos.y-5,s_mmFrame.x1+pos.x+5,s_mmFrame.y1+pos.y+5);
		
		if	(!rect.isIn(s_posMouse.x,s_posMouse.y))
			continue;
		
		char	*lpstrText;
		cSTRING strDLevelInfo;

		if	(bIsSecretDungeonGate)
			lpstrText		=	dMSG_SECRET_DUNGEON;
		else
		{
			if	(m_area.m_aArea[iArea].m_pstrString	==	NULL)
				continue;

			int	iTargetField=	atoi(m_area.m_aArea[iArea].m_pstrString+1);
			lpstrText		=	g_aMapSimpleInfo[iTargetField].m_strName;
			if (m_area.m_aArea[i].m_gateShape == 0 || m_area.m_aArea[i].m_gateShape >= ePAS_INNER_DUNGEON)
			{
					// 현재맵의 타일이 던젼형식이 아니라면 스킵한다.
				if ( g_map.m_iTileSet < eTS_CAVE || g_map.m_iTileSet > eTS_HEAVEN)
				{
					cWorldMapFieldInfo		m_info;
					if (!m_info.readData())
						return	FALSE;
					
					m_info.setCurrentField(g_hero.m_wCurrentField);
					
					if(m_info.m_iSelectFieldIndex != -1 && m_info.m_aFieldList[m_info.m_iSelectFieldIndex].m_wLinkDungeonCount)
					{
						for(int i=0 ; i<m_info.m_aFieldList[m_info.m_iSelectFieldIndex].m_wLinkDungeonCount ; i++)
						{
							int iLinkDindex = m_info.m_aFieldList[m_info.m_iSelectFieldIndex].m_awLinkDungeon[i];
							
							if (!m_info.m_aDungeonList[iLinkDindex].m_wLowLevel && !m_info.m_aDungeonList[iLinkDindex].m_wHighLevel)
								continue;

							if (strstr(lpstrText, g_aMapSimpleInfo[m_info.m_aDungeonList[iLinkDindex].m_awDungeonList[0]].m_strName))
							{							
								strDLevelInfo.Add(	"%s\nLv<c:LTGREEN>%d<n>~<c:LTGREEN>%d<n>", lpstrText, 
									m_info.m_aDungeonList[iLinkDindex].m_wLowLevel, 
									m_info.m_aDungeonList[iLinkDindex].m_wHighLevel);
								break;
							}						
						}
					}
				}
			}
		}

		int		iPixelSize	=	s_text.getPixelSize(lpstrText)+10;

		int		iWidth		=	min(100,iPixelSize);
		int		iPixelHeight=	s_text.getTextHeight(iWidth,lpstrText);
		int		iX			=	s_mmFrame.x1+pos.x;
		int		iY			=	s_mmFrame.y1+pos.y-iPixelHeight-6-5;

		if	(iX	+	iWidth/2+4+4	>=	cDRAW::WIDTH)
			iX	=	cDRAW::WIDTH	-	(iWidth/2+4+4);

		if	(iX-iWidth/2-4	<	0)
			iX	=	iWidth/2+4+4;

		iY	=	max(iY,10);

		rect.set(iX-iWidth/2-4,iY-5,iX+iWidth/2+4,iY+iPixelHeight+5);

		if (m_area.m_aArea[i].m_gateShape == 0 && strDLevelInfo.String != NULL ||
			m_area.m_aArea[i].m_gateShape >= ePAS_INNER_DUNGEON && strDLevelInfo.String != NULL ||
			m_area.m_aArea[i].m_gateShape == ePAS_INNER_PORTAL && strDLevelInfo.String != NULL)
		{
			int itemp = s_text.getTextHeight(iWidth, strDLevelInfo.String);
			rect.y2 = rect.y1+10+itemp;
			s_ttCommon.drawBox(&rect,0,32);
			s_text.cPut(rect.x1+5,rect.y1+6,iWidth,100,0,WHITE,dFITTEXT_WIDTH_CENTER,strDLevelInfo.String);
			drawQuestLinkDungeonToolTip(&rect, lpArea);
			return TRUE;
		}
		else
		{
			s_ttCommon.drawBox(&rect,0,32);
			s_text.cPut(rect.x1+5,rect.y1+6,iWidth,100,0,WHITE,dFITTEXT_WIDTH_CENTER,lpstrText);
		}

		drawQuestLinkDungeonToolTip(&rect, lpArea);

	}

	return	TRUE;
}
// insu modify end

void
cMAP::drawQuestLinkDungeonToolTip(cRECT* _rectForMark, cAreaInfo* _lpArea)
{	
	if (!m_bmCheckBox.isCheck(eMMOPTION_QUEST_FIELD))
		return;

	if ( g_map.m_iTileSet >= eTS_CAVE && g_map.m_iTileSet <= eTS_HEAVEN || g_map.m_iTileSet == eTS_ROOM || g_map.m_iTileSet ==eTS_GUILD_HALL)
		return;

	cSTRING strQuestField;
	int iAddBoxX = 0;

	vector<int>::iterator itr=m_vtPortaldestIndex.begin();
	for (int i=0 ; i<dMAX_AREA ; i++)
	{
		cAreaInfo	*lpArea	=	&m_area.m_aArea[i];
		if (lpArea->m_wKind != dAREA_PORTAL)	// 포탈 area 만 걸러낸다.
			continue;
		if (lpArea->m_pstrString == NULL)
			continue;
		
		if (lpArea->m_gateShape >= ePAS_ONE_WAY_GATE && lpArea->m_gateShape <= ePAS_HIDDEN_INNER_PORTAL)
			continue;

		char Buffer[4];
		memset(Buffer, '\0', 4);
		strncpy(Buffer, &lpArea->m_pstrString[1], 3);

		strncpy(Buffer, &lpArea->m_pstrString[1], 3);
		int iPotalDest = atoi(Buffer);	// 포탈의 목적지

		// 퀘스트 관련 던젼이 있는지 찾는다.
		CQuest	*lpQuestManager;
		int iIndex = 0;
		int iLimit = 0;
		
		if (g_map.m_iSelectQuest < dMAX_PROCESS_QUEST_COUNT)
		{
			iIndex = g_map.m_iSelectQuest;
			iLimit = 1;
		}
		if (g_map.m_iSelectQuest == dMAX_PROCESS_QUEST_COUNT)
		{
			iIndex = 0;
			iLimit = dMAX_PROCESS_QUEST_COUNT;
		}

		for(int j=0 ; j<iLimit ; j++) 
		{
			if	(g_map.m_iQuestIndex[iIndex] ==	0xffff)
				continue;

			if	(g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questKind	==	eQT_NORMAL	)
				lpQuestManager	=	&g_quest;
			if	(g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questKind	==	eQT_ARBEIT	)
				lpQuestManager	=	&g_questArbeit;
			
			int iQuestIndex	= g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questIndex;
			int iProcessLevel = g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questProcessLevel;
			CQuestItem	*lpQuest	=	lpQuestManager->getQuest(iQuestIndex);						
			CQuestItem::cQuestInfo	*lpQuestInfo	= lpQuest->getInfo(iProcessLevel);

			if(lpQuestInfo->m_wLinkFieldCount)	// 퀘스트의 링크필드가 있다면 필드수 만큼 반복한다.
			{				
				for(int k=0 ; k<lpQuestInfo->m_wLinkFieldCount ; k++)
				{
					BOOL bDraw = FALSE;
					int iMoveFieldCnt = 0;
					for(int L=0 ; L < dSAVEROAD_COUNT ; L++)
					{
						if (m_srField[L].m_iQuestLinkFieldIndex == 0xffff)
							break;
						
						if (m_srField[L].m_iQuestLinkFieldIndex == k &&
							m_srField[L].m_iQuestListIndex		== iIndex)
						{
							if (m_srField[L].m_bLinkField && m_srField[L].m_aiFieldIndex[1] == iPotalDest)
								bDraw = TRUE;
							if (!m_srField[L].m_bLinkField && m_srField[L].m_aiFieldIndex[1] == *itr)
								bDraw = TRUE;
							if (bDraw)
							{
								iMoveFieldCnt = 0;
								for(int m=1 ; m < dSAVEROAD_FIELDINDEX ; m++)
								{
									if (m_srField[L].m_aiFieldIndex[m] != 0xffff)
										iMoveFieldCnt++;
								}
							}
						}												
					}
					
					
					if (bDraw)
					{
						int iArrowType = 0xffff;
						CPos pos;
						
						pos.x	=	lpArea->x1+(lpArea->x2-lpArea->x1)/2-20;
						pos.y	=	lpArea->y1;
						
						pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
						pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;						

						
						if (!stricmp(_lpArea->m_pstrString, lpArea->m_pstrString))
						{
							if (iMoveFieldCnt > 1)
							{
								if (strQuestField.String!=NULL)
								{
									if (strstr(strQuestField.String, lpQuestManager->getName(iQuestIndex)))
										continue;
								}
								strQuestField.Add(dMSG_MINIMAP_QUEST_PORTAL_TOOLTIP_REMAIN, lpQuestManager->getName(iQuestIndex), iMoveFieldCnt);
								iAddBoxX = 25;
								
							}
							else
							{
								if (strQuestField.String!=NULL)
								{
									if (strstr(strQuestField.String, lpQuestManager->getName(iQuestIndex)))
										continue;
								}
								strQuestField.Add(dMSG_MINIMAP_QUEST_PORTAL_TOOLTIP_CORRECT, lpQuestManager->getName(iQuestIndex));
							}
							continue;
						}
					}
				}
			}
			iIndex++;
		}
		itr++;
	}

	if (strQuestField.String != NULL)
	{
		int istrHeight = CGame::s_sText.getTextHeight(140+iAddBoxX, strQuestField.String);
		
		cRECT rtView;
		rtView.set(	_rectForMark->x2 - (_rectForMark->x2-_rectForMark->x1)/2 - 75,
			_rectForMark->y2, 
			_rectForMark->x2-(_rectForMark->x2-_rectForMark->x1)/2 + 75, 
			_rectForMark->y2+istrHeight+10);
		
		if (rtView.x2 > s_mmFrame.x2)
		{
			rtView.x1 -= (rtView.x2-s_mmFrame.x2);
			rtView.x2 -= (rtView.x2-s_mmFrame.x2);
		}

		rtView.x1 -= iAddBoxX;
		
		s_ttCommon.drawBox(&rtView, 0, 32);
		CGame::s_sText.put(rtView.x1+5, rtView.y1+5, 140+iAddBoxX, istrHeight, 0, WHITE, strQuestField.String);
	}
}
// insu function add end

void
cMAP::FindQuestLinkField()
{
	for (int i=0 ; i<dSAVEROAD_COUNT ; i++)
		m_srField[i].reset();

	CQuest	*lpQuestManager;
	int iCnt = 0;
	for( i=0 ; i<dMAX_PROCESS_QUEST_COUNT ; i++) 
	{
		if	(g_map.m_iQuestIndex[i] ==	0xffff)
			continue;
		
		if	(g_hero.m_aProcessQuest[g_map.m_iQuestIndex[i]].questKind	==	eQT_NORMAL	)
			lpQuestManager	=	&g_quest;
		if	(g_hero.m_aProcessQuest[g_map.m_iQuestIndex[i]].questKind	==	eQT_ARBEIT	)
			lpQuestManager	=	&g_questArbeit;
		
		int iQuestIndex	= g_hero.m_aProcessQuest[g_map.m_iQuestIndex[i]].questIndex;
		int iProcessLevel = g_hero.m_aProcessQuest[g_map.m_iQuestIndex[i]].questProcessLevel;
		CQuestItem	*lpQuest	=	lpQuestManager->getQuest(iQuestIndex);						
		CQuestItem::cQuestInfo	*lpQuestInfo	= lpQuest->getInfo(iProcessLevel);
		
		if(lpQuestInfo->m_wLinkFieldCount)
		{				
			for(int k=0 ; k<lpQuestInfo->m_wLinkFieldCount ; k++)
			{
				cWM_road	road, resultPath;					
				g_gwWorldMap.setInforeadData();

				g_gwWorldMap.setInfoCurrentField(g_hero.m_wCurrentField);				
				int iEndFiledIndex =  g_gwWorldMap.getInfoSurfaceFieldIndex(lpQuestInfo->m_pLinkField[k]);
				int iResult = getDungeonFirstFloor(lpQuestInfo->m_pLinkField[k]);				

				if (g_hero.m_wCurrentField == lpQuestInfo->m_pLinkField[k])	// 현재필드와 퀘스트관련필드가 같은 경우 
				{
					if (g_hero.m_wCurrentField == iResult)
					{
						m_srField[iCnt].m_bLinkField = TRUE; // 이게 참이면 필드인덱스1번째 배열값을 맵인덱스로 비교한다.
						m_srField[iCnt].m_iQuestListIndex = i;
						m_srField[iCnt].m_iQuestLinkFieldIndex = k;
						m_srField[iCnt].m_aiFieldIndex[1]=iResult;
						iCnt++;
						continue;
					}
				}

				if (g_hero.m_wCurrentField != lpQuestInfo->m_pLinkField[k])			// 현재필드와 퀘스트관련필드가 다른 경우
				{
					// 길을 찾는다.

					// 현재필드에 연결된 던전인데 복층인 경우
					if (lpQuestInfo->m_pLinkField[k] != iResult && isPortalDest(iResult))
					{
						m_srField[iCnt].m_bLinkField = TRUE;
						m_srField[iCnt].m_iQuestListIndex = i;
						m_srField[iCnt].m_iQuestLinkFieldIndex = k;
						m_srField[iCnt].m_aiFieldIndex[1]=iResult;
						iCnt++;
						continue;
					}
					
					// 
					if (lpQuestInfo->m_pLinkField[k] == iResult)
					{
						resultPath.m_wNodeCount = 0;
						g_gwWorldMap.getInfoShortestNode(g_gwWorldMap.getInfoCurrentFieldIndex(), iEndFiledIndex, &road, &resultPath);
					}
					if (lpQuestInfo->m_pLinkField[k] != iResult)
					{
						iEndFiledIndex = g_gwWorldMap.getInfoSurfaceFieldIndex(iResult);
						resultPath.m_wNodeCount = 0;
						g_gwWorldMap.getInfoShortestNode(g_gwWorldMap.getInfoCurrentFieldIndex(), iEndFiledIndex, &road, &resultPath);
					}
					
				}
				
				if (resultPath.m_wNodeCount)	// 길이 존재한다
				{
					m_srField[iCnt].m_iQuestListIndex = i;
					m_srField[iCnt].m_iQuestLinkFieldIndex = k;
					for(int L=0 ; L < resultPath.m_wNodeCount ; L++)
						m_srField[iCnt].m_aiFieldIndex[L]= resultPath.m_awNodeList[L];
					iCnt++;
					continue;
				}

				if (!resultPath.m_wNodeCount)
				{
					m_srField[iCnt].m_bLinkField = TRUE;
					m_srField[iCnt].m_iQuestListIndex = i;
					m_srField[iCnt].m_iQuestLinkFieldIndex = k;
					m_srField[iCnt].m_aiFieldIndex[1] = iResult;
					iCnt++;
					continue;
				}
			}
		}
	}
}

// insu function add
void
cMAP::drawQuestList()
{
	int iPutPosX=s_mmFrame.x1, iPutPosY=s_mmFrame.y1;		// 출력좌표	
	int iPutLineCnt=1;	// 출력 줄수 
	int iLinepx = g_sprInterface.getSpriteHeight(eWINDOW_TITLE_QUEST_OLD);
	int iQuestCount = g_hero.getProcessQeustCount();	// 퀘스트 수

	// 출력좌표 셋팅! (출력될 창의 좌상단 좌표)
	iPutPosX -= (g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD)+6);
	iPutPosY -= 2;

#ifndef _DEBUG
	iPutPosX = s_rectTitle.x1-g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD);
	iPutPosY = 0;
#endif

	if (m_tgaMinimap.m_p16IMAGE == NULL)
	{
		iPutPosX = s_rectTitle.x2-g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD);
		iPutPosY =s_rectTitle.y2 + 2;
	}
	
	if	(iQuestCount	<=	0)	// 받은 퀘스트가 없다!
	{
		cRECT rttemp;
		rttemp.set(iPutPosX, iPutPosY+g_sprInterface.getSpriteHeight(eWINDOW_TITLE_QUEST_OLD)-10,
			iPutPosX+g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD)-1, 
			iPutPosY+g_sprInterface.getSpriteHeight(eWINDOW_TITLE_QUEST_OLD)+iLinepx);

		cDRAW::Fill(BLACK, rttemp.x1, rttemp.y1, rttemp.x2, rttemp.y2);		
		cDRAW::SetClippginArea(rttemp.x1, rttemp.y1, rttemp.x2, rttemp.y2+7);
		g_sprInterface.Put(rttemp.x1, rttemp.y2-475, eWINDOW);
		cDRAW::FreeClippginArea();
		g_sprInterface.Put(iPutPosX, iPutPosY, eWINDOW_TITLE_QUEST_OLD);
		s_text.cPut(iPutPosX+g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD)/2, iPutPosY+iPutLineCnt*iLinepx+2, WHITE, dMSG_NOT_EXIST_PROCESS_QUEST);
		return;
	}
	else	// 받은 퀘스트가 있을때!
	{
		cRECT rttemp;
		rttemp.set(iPutPosX, iPutPosY+g_sprInterface.getSpriteHeight(eWINDOW_TITLE_QUEST_OLD)-10,
			iPutPosX+g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD), 
			iPutPosY+g_sprInterface.getSpriteHeight(eWINDOW_TITLE_QUEST_OLD)+(dMAX_PROCESS_QUEST_COUNT+1)*iLinepx);

		cDRAW::Fill(BLACK, rttemp.x1, rttemp.y1, rttemp.x2, rttemp.y2);
		cDRAW::SetClippginArea(rttemp.x1, rttemp.y1, rttemp.x2, rttemp.y2+7);
		g_sprInterface.Put(rttemp.x1, rttemp.y2-475, eWINDOW);
		cDRAW::FreeClippginArea();
		g_sprInterface.Put(iPutPosX, iPutPosY, eWINDOW_TITLE_QUEST_OLD);
		iPutLineCnt = dMAX_PROCESS_QUEST_COUNT;
	}

	// 닫기버튼 처리
	if (g_map.m_iQuestListCloseBtnState == eMM_CLOSEBTN_OVER)
		g_sprInterface.Put(iPutPosX+g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD)-g_sprInterface.getSpriteWidth(eWINDOW_CLOSE_ACTIVE), iPutPosY, eWINDOW_CLOSE_ACTIVE);
	if (g_map.m_iQuestListCloseBtnState == eMM_CLOSEBTN_DOWN)
		g_sprInterface.Put(iPutPosX+g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD)-g_sprInterface.getSpriteWidth(eWINDOW_CLOSE_ACTIVE), iPutPosY, eWINDOW_CLOSE_PRESSED);

	CQuest	*lpQuestManager;
	int iPutCnt = 1;
	int iIndex = 0;
	for (int i=0;i<dMAX_PROCESS_QUEST_COUNT;i++,iIndex++)
	{			
		if ( m_iQuestIndex[iIndex] ==	0xffff )		
		{
			// 제일 밑줄부터 출력해준다. -empty- 라고..
			s_text.cPut(iPutPosX+g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD)/2, iPutPosY+iPutLineCnt*iLinepx,GRAY, "- EMPTY -");
			iPutLineCnt--;
			continue;
		}
		if	(g_hero.m_aProcessQuest[m_iQuestIndex[iIndex]].questKind	==	eQT_NORMAL	)
			lpQuestManager	=	&g_quest;
		if	(g_hero.m_aProcessQuest[m_iQuestIndex[iIndex]].questKind	==	eQT_ARBEIT	)
			lpQuestManager	=	&g_questArbeit;

		int			iQuestIndex	=	g_hero.m_aProcessQuest[m_iQuestIndex[iIndex]].questIndex;
		CQuestItem	*lpQuest	=	lpQuestManager->getQuest(iQuestIndex);
		char		strQuestName[128];

		if	(lpQuest->m_wQuestKind	==	eQK_MAIN_STORY)
			strcpy(strQuestName,dMSG_PROCESS_MAIN_QUEST);
		else
		{
			// 완료한 퀘스트인가?
			if	(g_hero.m_aProcessQuest[i].isComplete)
				sprintf(strQuestName,"%s (<c:LTGREEN>C<n>)",lpQuest->m_strName);
			else
				strcpy(strQuestName,lpQuest->m_strName);
		}

		if (i == m_iSelectQuest && i != dMAX_PROCESS_QUEST_COUNT)
		{
			s_text.cPut(iPutPosX+g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD)/2, iPutPosY+(iPutCnt*iLinepx)+2, LTYELLOW, strQuestName);
			iPutCnt++;
		}
		else
		{
			if ( i == m_iFocusQuest && i != dMAX_PROCESS_QUEST_COUNT)
			{
				s_text.cPut(iPutPosX+g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD)/2, iPutPosY+(iPutCnt*iLinepx)+2, LTGRAY, strQuestName);
				iPutCnt++;
			}
			else
			{
				s_text.cPut(iPutPosX+g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD)/2, iPutPosY+(iPutCnt*iLinepx)+2, GRAY, strQuestName);
				iPutCnt++;
			}
		}
	}
	if (m_iSelectQuest == dMAX_PROCESS_QUEST_COUNT)
	{
		s_text.cPut(iPutPosX+g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD)/2, iPutPosY+((dMAX_PROCESS_QUEST_COUNT+1)*iLinepx)+2, LTYELLOW, dMSG_MINIMAP_SHOW_ALL);
	}
	else
	{
		if (m_iFocusQuest == dMAX_PROCESS_QUEST_COUNT)
			s_text.cPut(iPutPosX+g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD)/2, iPutPosY+((dMAX_PROCESS_QUEST_COUNT+1)*iLinepx)+2, LTGRAY, dMSG_MINIMAP_SHOW_ALL);
		else
			s_text.cPut(iPutPosX+g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD)/2, iPutPosY+((dMAX_PROCESS_QUEST_COUNT+1)*iLinepx)+2, GRAY, dMSG_MINIMAP_SHOW_ALL);
	}
}
// insu function add end



// insu function add
void
cMAP::drawMinimapOffscreenQuestNpcArrow(char* _strName, CPos* _pos, cRECT* _rectForMark)
{
	if (!m_bmCheckBox.isCheck(eMMOPTION_QUEST_NPC))
		return;

	int iArrowType = 0xffff;
		
	CQuest	*lpQuestManager;

	int iIndex = 0;
	int iLimit = 0;

	if (g_map.m_iSelectQuest < dMAX_PROCESS_QUEST_COUNT)
	{
		iIndex = g_map.m_iSelectQuest;
		iLimit = 1;
	}
	if (g_map.m_iSelectQuest == dMAX_PROCESS_QUEST_COUNT)
	{
		iIndex = 0;
		iLimit = dMAX_PROCESS_QUEST_COUNT;
	}
	for(int i = 0 ; i< iLimit ; i++) 
	{
		if	(g_map.m_iQuestIndex[iIndex] == 0xffff)
		{
			return;
		}
		if	(g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questKind	==	eQT_NORMAL	)
			lpQuestManager	=	&g_quest;
		if	(g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questKind	==	eQT_ARBEIT	)
			lpQuestManager	=	&g_questArbeit;
		
		int			iQuestIndex	=	g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questIndex;
		int iProcessLevel = g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questProcessLevel;
		CQuestItem	*lpQuest	=	lpQuestManager->getQuest(iQuestIndex);
		
		/**/
		if (!lpQuest->m_aInfo[iProcessLevel].m_wLinkNpcCount)
		{
			iIndex++;
			continue;
		}
		int imarginX = 7;
		int imarginY = 5;
		// 퀘스트이름으로 같은 종류의 퀘스트 인지 판별한다.
		for(int j=0 ; j <lpQuest->m_aInfo[iProcessLevel].m_wLinkNpcCount ; j++)
		{
			
			if (stricmp(_strName, lpQuest->m_aInfo[iProcessLevel].m_pNpcList[j].m_pStrName) == 0)	// 퀘스트이름이 같다!
			{
				// insu modify .. 주석부분 삭제
				/*
				if (_pos->x+g_sprInterface2.getSpriteWidth(eeeMINIMAP_ARROW_RIGHT_UP) >= _rectForMark->x2 && _pos->y < _rectForMark->y1+g_sprInterface2.getSpriteHeight(eeMINIMAP_ARROW_LEFT))	// →↑
				{					
					_pos->x = _rectForMark->x2;
					_pos->y = _rectForMark->y1+imarginY;
					iArrowType = eeeMINIMAP_ARROW_RIGHT_UP;
					_pos->x -= (g_sprInterface2.getSpriteWidth(iArrowType)+imarginX);
				}
				else if (_pos->x+g_sprInterface2.getSpriteWidth(eeeMINIMAP_ARROW_RIGHT_DOWN) >= _rectForMark->x2 && _pos->y > _rectForMark->y2-g_sprInterface2.getSpriteHeight(eeMINIMAP_ARROW_LEFT))	// →↓
				{					
					_pos->x = _rectForMark->x2;
					_pos->y = _rectForMark->y2;
					iArrowType = eeeMINIMAP_ARROW_RIGHT_DOWN;
					_pos->x -= (g_sprInterface2.getSpriteWidth(iArrowType)+imarginX);
					_pos->y -= (g_sprInterface2.getSpriteHeight(iArrowType)+imarginY);
				}
				else if (_pos->x >= _rectForMark->x2 && _pos->y >= _rectForMark->y1 && _pos->y <= _rectForMark->y2)	// →
				{					
					_pos->x = _rectForMark->x2;		
					iArrowType = eeMINIMAP_ARROW_RIGHT;
					_pos->x -= (g_sprInterface2.getSpriteWidth(iArrowType)+imarginX);
					_pos->y -= g_sprInterface2.getSpriteHeight(iArrowType)/2;
				}
				
				else if (_pos->x-g_sprInterface2.getSpriteWidth(eeeMINIMAP_ARROW_RIGHT_UP) <= _rectForMark->x1 && _pos->y < _rectForMark->y1+g_sprInterface2.getSpriteHeight(eeeMINIMAP_ARROW_RIGHT_UP))	// ←↑
				{
					_pos->x = _rectForMark->x1+imarginX;
					_pos->y = _rectForMark->y1+imarginY;
					iArrowType = eeMINIMAP_ARROW_LEFT_UP;
				}
				else if (_pos->x-g_sprInterface2.getSpriteWidth(eeMINIMAP_ARROW_LEFT_DOWN) <= _rectForMark->x1 && _pos->y > _rectForMark->y2-g_sprInterface2.getSpriteHeight(eeMINIMAP_ARROW_LEFT_DOWN))	// ←↓
				{
					_pos->x = _rectForMark->x1;
					_pos->y = _rectForMark->y2;
					iArrowType = eeMINIMAP_ARROW_LEFT_DOWN;
					_pos->x += imarginX-1;
					_pos->y -= (g_sprInterface2.getSpriteHeight(iArrowType)+imarginY+2);
				}
				else if (_pos->x <= _rectForMark->x1 && _pos->y >= _rectForMark->y1 && _pos->y <= _rectForMark->y2)		// ←
				{
					_pos->x = _rectForMark->x1;
					iArrowType = eeMINIMAP_ARROW_LEFT;
					_pos->x += imarginX;
					_pos->y -= g_sprInterface2.getSpriteHeight(iArrowType)/2;
				}
				
				else if (_pos->y > _rectForMark->y2 && _pos->x > _rectForMark->x1 && _pos->x < _rectForMark->x2)		// ↓
				{
					_pos->y = _rectForMark->y2;
					iArrowType = eMINIMAP_ARROW_DOWN;
					_pos->y -= (g_sprInterface2.getSpriteHeight(iArrowType)+imarginX);
					_pos->x -= g_sprInterface2.getSpriteWidth(iArrowType)/2;
				}
				else if (_pos->y < _rectForMark->y1 && _pos->x > _rectForMark->x1 && _pos->x < _rectForMark->x2)		// ↑
				{
					_pos->y = _rectForMark->y1+imarginY;					
					iArrowType = eMINIMAP_ARROW_UP;
					_pos->x -= g_sprInterface2.getSpriteWidth(iArrowType)/2;
					}
				*/
				// insu add
				iArrowType = ChoiceArrowDirection(_pos, _rectForMark);
				// insu add 
				if (iArrowType == 0xffff)
					return;
				
				_pos->x		-=	m_posMM.x;
				_pos->y		-=	m_posMM.y;
				
				g_sprInterface2.Put(s_mmFrame.x1+_pos->x+1, s_mmFrame.y1+_pos->y+1, iArrowType);
			}
			
		}
		iIndex++;
	}
}




// insu function add
int 
cMAP::ChoiceArrowDirection(CPos* _pos, cRECT* _rectForMark)
{
	int iArrowType = 0xffff;
	int imarginX = 7;
	int imarginY = 5;

	if (_pos->x+g_sprInterface2.getSpriteWidth(eMINIMAP_ARROW_RIGHT_UP) >= _rectForMark->x2 && 
		_pos->y < _rectForMark->y1+g_sprInterface2.getSpriteHeight(eMINIMAP_ARROW_LEFT))	// →↑
	{					
		_pos->x = _rectForMark->x2;
		_pos->y = _rectForMark->y1+imarginY;
		iArrowType = eMINIMAP_ARROW_RIGHT_UP;
		_pos->x -= (g_sprInterface2.getSpriteWidth(iArrowType)+imarginX);
	}
	else if (_pos->x+g_sprInterface2.getSpriteWidth(eMINIMAP_ARROW_RIGHT_DOWN) >= _rectForMark->x2 && 
			 _pos->y > _rectForMark->y2-g_sprInterface2.getSpriteHeight(eMINIMAP_ARROW_LEFT))	// →↓
	{					
		_pos->x = _rectForMark->x2;
		_pos->y = _rectForMark->y2;
		iArrowType = eMINIMAP_ARROW_RIGHT_DOWN;
		_pos->x -= (g_sprInterface2.getSpriteWidth(iArrowType)+imarginX);
		_pos->y -= (g_sprInterface2.getSpriteHeight(iArrowType)+imarginY);
	}
	else if (_pos->x >= _rectForMark->x2 && _pos->y >= _rectForMark->y1 && _pos->y <= _rectForMark->y2)	// →
	{					
		_pos->x = _rectForMark->x2;		
		iArrowType = eMINIMAP_ARROW_RIGHT;
		_pos->x -= (g_sprInterface2.getSpriteWidth(iArrowType)+imarginX);
		_pos->y -= g_sprInterface2.getSpriteHeight(iArrowType)/2;
		return iArrowType;
	}
	else if (_pos->x-g_sprInterface2.getSpriteWidth(eMINIMAP_ARROW_RIGHT_UP) <= _rectForMark->x1 && 
			 _pos->y < _rectForMark->y1+g_sprInterface2.getSpriteHeight(eMINIMAP_ARROW_RIGHT_UP))	// ←↑
	{
		_pos->x = _rectForMark->x1+imarginX;
		_pos->y = _rectForMark->y1+imarginY;
		iArrowType = eMINIMAP_ARROW_LEFT_UP;
	}
	else if (_pos->x-g_sprInterface2.getSpriteWidth(eMINIMAP_ARROW_LEFT_DOWN) <= _rectForMark->x1 &&
			 _pos->y > _rectForMark->y2-g_sprInterface2.getSpriteHeight(eMINIMAP_ARROW_LEFT_DOWN))	// ←↓
	{
		_pos->x = _rectForMark->x1;
		_pos->y = _rectForMark->y2;
		iArrowType = eMINIMAP_ARROW_LEFT_DOWN;
		_pos->x += imarginX-1;
		_pos->y -= (g_sprInterface2.getSpriteHeight(iArrowType)+imarginY+2);
	}
	else if (_pos->x <= _rectForMark->x1 && _pos->y >= _rectForMark->y1 && _pos->y <= _rectForMark->y2)		// ←
	{
		_pos->x = _rectForMark->x1;
		iArrowType = eMINIMAP_ARROW_LEFT;
		_pos->x += imarginX;
		_pos->y -= g_sprInterface2.getSpriteHeight(iArrowType)/2;
	}
	else if (_pos->y > _rectForMark->y2 && _pos->x > _rectForMark->x1 && _pos->x < _rectForMark->x2)		// ↓
	{
		_pos->y = _rectForMark->y2;
		iArrowType = eMINIMAP_ARROW_DOWN;
		_pos->y -= (g_sprInterface2.getSpriteHeight(iArrowType)+imarginX);
		_pos->x -= g_sprInterface2.getSpriteWidth(iArrowType)/2;
	}
	else if (_pos->y < _rectForMark->y1 && _pos->x > _rectForMark->x1 && _pos->x < _rectForMark->x2)		// ↑
	{
		_pos->y = _rectForMark->y1+imarginY;					
		iArrowType = eMINIMAP_ARROW_UP;
		_pos->x -= g_sprInterface2.getSpriteWidth(iArrowType)/2;
	}
	return iArrowType;
}
// insu function add end



// insu function add
void
cMAP::drawQuestLinkDungeonArrow(cRECT* _rectForMark, BOOL _bFogmap)
{
	if (!m_bmCheckBox.isCheck(eMMOPTION_QUEST_FIELD))
		return;
	if ( g_map.m_iTileSet >= eTS_CAVE && g_map.m_iTileSet <= eTS_HEAVEN || g_map.m_iTileSet == eTS_ROOM || g_map.m_iTileSet ==eTS_GUILD_HALL)
		return;
	vector<int>::iterator itr=m_vtPortaldestIndex.begin();
	for (int i=0; i<dMAX_AREA && itr != m_vtPortaldestIndex.end() ; i++)	
	{
		cAreaInfo	*lpArea	=	&m_area.m_aArea[i];
		if (lpArea->m_wKind != dAREA_PORTAL)	// 포탈 area 만 걸러낸다.
			continue;

		if (lpArea->m_pstrString == NULL)
			continue;
		
		if (lpArea->m_gateShape >= ePAS_ONE_WAY_GATE && lpArea->m_gateShape <= ePAS_HIDDEN_INNER_PORTAL)
			continue;
		// 퀘스트 관련 던젼이 있는지 찾는다.
		CQuest	*lpQuestManager;
		int iIndex = 0;
		int iLimit = 0;

		char Buffer[4];
		
		strncpy(Buffer, &lpArea->m_pstrString[1], 3);
		int iPotalDest = atoi(Buffer);	// 포탈의 목적지
		
		if (g_map.m_iSelectQuest < dMAX_PROCESS_QUEST_COUNT)
		{
			iIndex = g_map.m_iSelectQuest;
			iLimit = 1;
		}
		if (g_map.m_iSelectQuest == dMAX_PROCESS_QUEST_COUNT)
		{
			iIndex = 0;
			iLimit = dMAX_PROCESS_QUEST_COUNT;
		}

		BOOL bDraw = FALSE;

		for(int j=0 ; j<iLimit ; j++) 
		{
			if	(g_map.m_iQuestIndex[iIndex] ==	0xffff)
				continue;

			if	(g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questKind	==	eQT_NORMAL	)
				lpQuestManager	=	&g_quest;
			if	(g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questKind	==	eQT_ARBEIT	)
				lpQuestManager	=	&g_questArbeit;
			
			int iQuestIndex	= g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questIndex;
			int iProcessLevel = g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questProcessLevel;
			CQuestItem	*lpQuest	=	lpQuestManager->getQuest(iQuestIndex);						
			CQuestItem::cQuestInfo	*lpQuestInfo	= lpQuest->getInfo(iProcessLevel);
						
			if(lpQuestInfo->m_wLinkFieldCount)
			{
				for(int k=0 ; k<lpQuestInfo->m_wLinkFieldCount ; k++)
				{
					for(int L=0 ; L<dSAVEROAD_COUNT ; L++)
					{
						if (m_srField[L].m_iQuestLinkFieldIndex == 0xffff)
							break;

						if (m_srField[L].m_iQuestLinkFieldIndex == k &&
							m_srField[L].m_iQuestListIndex		== iIndex)
						{
							if (m_srField[L].m_bLinkField && m_srField[L].m_aiFieldIndex[1] == iPotalDest)
								bDraw = TRUE;
							if (!m_srField[L].m_bLinkField && m_srField[L].m_aiFieldIndex[1] == *itr)
								bDraw = TRUE;
						}
						if (bDraw)
							break;
					}
					if (bDraw)
					break;
				}
			}
			iIndex++;	
			if (bDraw)
				break;
		}
		itr++;

		if (bDraw)
		{
			int iArrowType = 0xffff;
			CPos pos;
			
			pos.x	=	lpArea->x1;
			pos.y	=	lpArea->y1;
			
			pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
			pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;						
			
			
			if (_bFogmap)
			{
				int iFPS		=	g_anmEtc.GetFPS(eEANM_MINIMAP_ARROW_DOWN);
				int iFrameCount	=	g_anmEtc.GetFrameCount(eEANM_MINIMAP_ARROW_DOWN);
				int iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount;
				
				if (_rectForMark->y1+pos.y-g_anmEtc.getSpriteHeight(eEANM_MINIMAP_ARROW_DOWN, 0, 0) <= m_iPutPosY)
				{
					g_anmEtc.PutAlpha(	_rectForMark->x1+pos.x-g_anmEtc.getSpriteWidth(eEANM_MINIMAP_ARROW_UP, 0, 0)/2,
						_rectForMark->y1+pos.y,
						eEANM_MINIMAP_ARROW_UP, 0, iFrame, 100, 100, m_iFogmapAlpha);
					continue;
				}
				
				g_anmEtc.PutAlpha(	_rectForMark->x1+pos.x-g_anmEtc.getSpriteWidth(eEANM_MINIMAP_ARROW_DOWN, 0, 0)/2,
					_rectForMark->y1+pos.y-g_anmEtc.getSpriteHeight(eEANM_MINIMAP_ARROW_DOWN, 0, 0),
					eEANM_MINIMAP_ARROW_DOWN, 0, iFrame, 100, 100, m_iFogmapAlpha);
				continue;
			}
			
			if (!_rectForMark->isIn(pos.x, pos.y))	// 미니맵 뷰 범위 밖에 존재한다!
			{
				iArrowType = ChoiceArrowDirection(&pos, _rectForMark);
				
				if (iArrowType == 0xffff)
					continue;
				pos.x -= m_posMM.x;
				pos.y -= m_posMM.y;
				
				g_sprInterface2.Put(s_mmFrame.x1+pos.x, s_mmFrame.y1+pos.y, iArrowType);
			}
			
			else	// 미니맵 뷰 범위 안에 존재한다.
			{
				pos.x -= m_posMM.x;
				pos.y -= m_posMM.y;
				
				int iFPS		=	g_anmEtc.GetFPS(eEANM_MINIMAP_ARROW_DOWN);
				int iFrameCount	=	g_anmEtc.GetFrameCount(eEANM_MINIMAP_ARROW_DOWN);
				int iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount;
				int iY = iFrame/2;
				
				if (s_mmFrame.y1+pos.y-g_anmEtc.getSpriteHeight(eEANM_MINIMAP_ARROW_DOWN, 0, 0)/2 <= s_mmFrame.y1)
				{								
					g_anmEtc.PutAlpha(	s_mmFrame.x1+pos.x-g_anmEtc.getSpriteWidth(eEANM_MINIMAP_ARROW_UP, 0, 0)/2,
						s_mmFrame.y1+pos.y+iY,
						eEANM_MINIMAP_ARROW_UP, 0, iFrame, 100, 100, 32);
					continue;
				}
				g_anmEtc.PutAlpha(	s_mmFrame.x1+pos.x-g_anmEtc.getSpriteWidth(eEANM_MINIMAP_ARROW_DOWN, 0, 0)/2,
					s_mmFrame.y1+pos.y-g_anmEtc.getSpriteHeight(eEANM_MINIMAP_ARROW_DOWN, 0, 0)-iY,
					eEANM_MINIMAP_ARROW_DOWN, 0, iFrame, 100, 100, 32);
			}						
		}
	}
}
// insu function add end


void
cMAP::drawQuestListDetail(int _iQuestListIndex, int _iPutPosX, int _iPutPosY)
{
	_iQuestListIndex = m_iQuestIndex[_iQuestListIndex];
	if (_iQuestListIndex == 0xffff)
		return;

	cSTRING strInfo;
	
	CQuest	*lpQuestManager;
	if	(g_hero.m_aProcessQuest[_iQuestListIndex].isActive		==	FALSE		)
		return;	

	if	(g_hero.m_aProcessQuest[_iQuestListIndex].questKind	==	eQT_NORMAL	)
		lpQuestManager	=	&g_quest;
	if	(g_hero.m_aProcessQuest[_iQuestListIndex].questKind	==	eQT_ARBEIT	)
		lpQuestManager	=	&g_questArbeit;

	int			iQuestIndex	=	g_hero.m_aProcessQuest[_iQuestListIndex].questIndex;
	CQuestItem	*lpQuest	=	lpQuestManager->getQuest(iQuestIndex);
	
	int						iProcessLevel	= g_hero.m_aProcessQuest[_iQuestListIndex].questProcessLevel;
	CQuestItem::cQuestInfo	*lpQuestInfo	= lpQuest->getInfo(iProcessLevel);
	int						iClientField	= lpQuest->getClientField(iProcessLevel);
	
	tsProcessQuestField		*lpQuestExplain	= &g_hero.m_aProcessQuest[_iQuestListIndex];
	char					*lpstrExplain	= lpQuest->getExplain(lpQuestExplain->questProcessLevel, lpQuestExplain->isComplete);
	
	// 관련 NPC 부분 
	if (lpQuestInfo->m_wLinkNpcCount)
	{
		BOOL bAddTitle = FALSE;
		for(int j=0 ; j<lpQuestInfo->m_wLinkNpcCount ; j++)
		{
			if (bAddTitle == FALSE)
			{
				strInfo.Add("<c:CTBLUE>%s<n>\n", dMSG_LINK_QUEST_NPC);
				bAddTitle = TRUE;
			}
			strInfo.Add("  <c:LTYELLOW>%s<n>  \n", lpQuestInfo->m_pNpcList[j].m_pStrName);
			bAddTitle = TRUE;
		}
	}
	// 관련 NPC 부분 끝
	
	// 관련Monster부분		
	if (lpQuestInfo->m_wTargetMonsterCount)
	{
		BOOL bAddTitle	=	FALSE;
		for (int j=0; j<lpQuestInfo->m_wTargetMonsterCount ; j++)
		{
			if (bAddTitle	==	FALSE)
			{
				if (strInfo.String != NULL)
					strInfo.Add("\n<c:CTBLUE>%s<n>\n",dMSG_LINK_QUEST_MONSTER);
				else
					strInfo.Add("<c:CTBLUE>%s<n>\n",dMSG_LINK_QUEST_MONSTER);
				bAddTitle	=	TRUE;
			}
			strInfo.Add("  <c:LTYELLOW>%s<n>  ", lpQuestInfo->m_pTargetMonsterList[j].m_pStrName);
		}			
	}
	// 관련Monster부분 끝
	
	// 관련Item부분
	if (lpQuestInfo->m_wRequireItemCount)
	{
		if (strInfo.String != NULL)
			strInfo.Add("\n<c:CTBLUE>%s<n>\n",dMSG_LINK_QUEST_ITEM);
		else
			strInfo.Add("<c:CTBLUE>%s<n>\n",dMSG_LINK_QUEST_ITEM);
		for (int k=0 ; k <lpQuestInfo->m_wRequireItemCount ; k++)
		{
			int iItem		= lpQuestInfo->m_pRequireItemList[k*2];
			int	iItemCount	= lpQuestInfo->m_pRequireItemList[k*2+1];
			int	iCount		= g_hero.getItemCountInInventory(iItem);
			char	strColor[12]=	"WHITE";
			
			if (iCount	>=	iItemCount)
				strcpy(strColor,"LTGREEN");
			
			strInfo.Add("<c:%s>%s - %d/<c:LTYELLOW>%d<n>",strColor,g_aBasicItem[iItem].m_strName,iCount,iItemCount);			
		}
	}// 관련Item부분 끝
	
	// 퀘스트설명부분
	if (lpstrExplain)
	{
		if (strInfo.String != NULL)
			strInfo.Add("\n<c:CTBLUE>%s<n>\n",dMSG_QUEST_INFO);
		else
			strInfo.Add("<c:CTBLUE>%s<n>\n",dMSG_QUEST_INFO);
		strInfo.Add("%s",lpstrExplain);
	}// 퀘스트설명부분 끝

	int iQUESTINFO_WIDTH = g_sprInterface.getSpriteWidth(eWINDOW_TITLE_QUEST_OLD);
	int iTextHeight = CGame::s_sText.getTextHeight(iQUESTINFO_WIDTH-10, strInfo.String);
	cRECT rect;
	rect.set(_iPutPosX, _iPutPosY, _iPutPosX+iQUESTINFO_WIDTH-2, _iPutPosY+iTextHeight+10);
	s_ttCommon.drawBox(&rect,0,32);
	CGame::s_sText.put(rect.x1+5, rect.y1+5, iQUESTINFO_WIDTH-10, iTextHeight, 0, WHITE, strInfo.String);
}

// insu function add

void
cMAP::drawMinimapOption()
{
	int iCheckBoxHeight = g_sprInterface.getSpriteHeight(eSMWI_CHECK_BOX);
	
	// 옵션 창이 뜰 위치를 설정 
	m_rtOptionWnd.x1 = s_rectTitle.x2-dMINIMAP_OPTION_WND_WIDTH;
	m_rtOptionWnd.y1 = s_rectTitle.y2;
	m_rtOptionWnd.x2 = s_rectTitle.x2;
	
	// 체크박스의 위치를 설정
	for( int i=0 ; i<m_bmCheckBox.m_wCount ; i++)
	{
		m_bmCheckBox.move(i, m_rtOptionWnd.x1+5, m_rtOptionWnd.y1+5+i*iCheckBoxHeight);
		if (i == m_bmCheckBox.m_wCount-1)
			m_rtOptionWnd.y2 = m_rtOptionWnd.y1+5+(i+1)*iCheckBoxHeight+2;
	}	

	// 출력
	s_ttCommon.drawBox(&m_rtOptionWnd, 0, 32);
	m_bmCheckBox.draw();
}


void
cMAP::drawMinimapOnscreenQuestNpcArrow(char* _strName, CPos* _pos, cRECT* _mmFrame, int _iPutPosX, int _iPutPosY, int _iAlpha2)
{

	if (!m_bmCheckBox.isCheck(eMMOPTION_QUEST_NPC))
		return;

	CQuest	*lpQuestManager;	

	int iIndex = 0;
	int iLimit = 0;

	if (g_map.m_iSelectQuest < dMAX_PROCESS_QUEST_COUNT)
	{
		iIndex = g_map.m_iSelectQuest;
		iLimit = 1;
	}
	if (g_map.m_iSelectQuest == dMAX_PROCESS_QUEST_COUNT)
	{
		iIndex = 0;
		iLimit = dMAX_PROCESS_QUEST_COUNT;
	}

	for(int i = 0 ; i< iLimit ; i++) 
	{
		if	(g_map.m_iQuestIndex[iIndex] == 0xffff)
			continue;

		if	(g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questKind	==	eQT_NORMAL	)
			lpQuestManager	=	&g_quest;
		if	(g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questKind	==	eQT_ARBEIT	)
			lpQuestManager	=	&g_questArbeit;

		if (lpQuestManager == NULL)
		{
			return;
		}
		
		int	iQuestIndex	=	g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questIndex;
		int iProcessLevel = g_hero.m_aProcessQuest[g_map.m_iQuestIndex[iIndex]].questProcessLevel;
		CQuestItem	*lpQuest	=	lpQuestManager->getQuest(iQuestIndex);	
		
		if (!lpQuest->m_aInfo[iProcessLevel].m_wLinkNpcCount)
		{
			iIndex++;
			continue;
		}
		
		// 퀘스트이름으로 같은 종류의 퀘스트 인지 판별한다.
		for(int j=0 ; j <lpQuest->m_aInfo[iProcessLevel].m_wLinkNpcCount ; j++)
		{
			if (stricmp(_strName, lpQuest->m_aInfo[iProcessLevel].m_pNpcList[j].m_pStrName) == 0)
			{
				if (_iPutPosX == 0xffff && _iPutPosY == 0xffff)
				{
					
					int iFPS		=	g_anmEtc.GetFPS(eEANM_MINIMAP_ARROW_DOWN);
					int iFrameCount	=	g_anmEtc.GetFrameCount(eEANM_MINIMAP_ARROW_DOWN);
					int iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount;
					int iY = iFrame/2;
									

					g_sprInterface2.Put(_mmFrame->x1+_pos->x+1,
										_mmFrame->y1+_pos->y+1,
										eIWI_WEAPON_JOB3_2, 100,100,dPUT_COLOR,0);
					g_sprInterface2.Put(_mmFrame->x1+_pos->x,
										_mmFrame->y1+_pos->y,
										eIWI_WEAPON_JOB3_2);
					
					if (_mmFrame->y1+_pos->y+1-g_anmEtc.getSpriteHeight(eEANM_MINIMAP_ARROW_DOWN, 0, 0)/2 <= _mmFrame->y1)
					{
						g_anmEtc.PutReg(_mmFrame->x1+_pos->x+1-g_anmEtc.getSpriteWidth(eEANM_MINIMAP_ARROW_UP, 0, 0)/2-1,
										_mmFrame->y1+_pos->y+1-iY,
										eEANM_MINIMAP_ARROW_UP,0,iFrame, 100, 100);
						continue;
					}
					g_anmEtc.PutReg(_mmFrame->x1+_pos->x+1-g_anmEtc.getSpriteWidth(eEANM_MINIMAP_ARROW_DOWN, 0, 0)/2-1,
									_mmFrame->y1+_pos->y+1-g_anmEtc.getSpriteHeight(eEANM_MINIMAP_ARROW_DOWN, 0, 0)-iY,
									eEANM_MINIMAP_ARROW_DOWN,0,iFrame, 100, 100);
				}
				else
  				{
  					int iFPS		=	g_anmEtc.GetFPS(eEANM_MINIMAP_ARROW_DOWN);
  					int iFrameCount	=	g_anmEtc.GetFrameCount(eEANM_MINIMAP_ARROW_DOWN);
  					int iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount;
  
					if (_iAlpha2 == 32)
  					{
  						g_sprInterface2.Put(_iPutPosX, _iPutPosY, eIWI_WEAPON_JOB3_2);
  						g_sprInterface2.Put(_iPutPosX, _iPutPosY, eIWI_WEAPON_JOB3_2);

						if (_iPutPosY-g_anmEtc.getSpriteHeight(eEANM_MINIMAP_ARROW_DOWN, 0, 0) <= m_iPutPosY)
						{
							g_anmEtc.PutReg(_iPutPosX-g_anmEtc.getSpriteWidth(eEANM_MINIMAP_ARROW_UP, 0, 0)/2-1,
  											_iPutPosY-g_anmEtc.getSpriteHeight(eEANM_MINIMAP_ARROW_UP, 0, 0),
  											eEANM_MINIMAP_ARROW_UP,0,iFrame);
							continue;
						}
						g_anmEtc.PutReg(_iPutPosX-g_anmEtc.getSpriteWidth(eEANM_MINIMAP_ARROW_DOWN, 0, 0)/2-1,
  									_iPutPosY-g_anmEtc.getSpriteHeight(eEANM_MINIMAP_ARROW_DOWN, 0, 0),
  									eEANM_MINIMAP_ARROW_DOWN,0,iFrame);
  					}
					else
  					{
  						g_sprInterface2.Put(_iPutPosX, _iPutPosY, 
  											eIWI_WEAPON_JOB3_2, 100, 100, dPUT_ALPHA_BLENDING, _iAlpha2);
  						g_sprInterface2.Put(_iPutPosX, _iPutPosY, 
  											eIWI_WEAPON_JOB3_2, 100, 100, dPUT_ALPHA_BLENDING, _iAlpha2);
						
						if (_iPutPosY-g_anmEtc.getSpriteHeight(eEANM_MINIMAP_ARROW_DOWN, 0, 0) <= m_iPutPosY)
						{
							g_anmEtc.PutAlpha(	_iPutPosX-g_anmEtc.getSpriteWidth(eEANM_MINIMAP_ARROW_UP, 0, 0)/2,
  												_iPutPosY-g_anmEtc.getSpriteHeight(eEANM_MINIMAP_ARROW_UP, 0, 0), 
  												eEANM_MINIMAP_ARROW_UP, 0, iFrame, 100, 100, m_iFogmapAlpha);
							continue;
						}
						g_anmEtc.PutAlpha(	_iPutPosX-g_anmEtc.getSpriteWidth(eEANM_MINIMAP_ARROW_DOWN, 0, 0)/2,
  											_iPutPosY-g_anmEtc.getSpriteHeight(eEANM_MINIMAP_ARROW_DOWN, 0, 0), 
  											eEANM_MINIMAP_ARROW_DOWN, 0, iFrame, 100, 100, m_iFogmapAlpha);	
  					}
				}
			}
		}
		iIndex++;
	}
}
// insu function add end



// insu function add
void
cMAP::drawHuntingArea(cRECT* _rectFrame)
{
	for (int i=0;i<m_area.m_iCount;i++)
	{
		CPos		pos;
		BOOL		bIsSecretDungeonGate	=	FALSE;
		cAreaInfo	*lpArea	=	&m_area.m_aArea[i];
		int iIConX,iIConY,iLevel_Low,iLevel_High=0;

		if (lpArea->m_wSerial == 0xffff)
			continue;

		if (lpArea->m_wKind == dAREA_HUNTING_AREA)
		{
			cAreaHunting* lpHunt = (cAreaHunting*)lpArea;

			iIConX = lpHunt->m_dwIconX;
			iIConY = lpHunt->m_dwIconY;
			iLevel_Low = lpHunt->m_dwLevel_Low;
			iLevel_High = lpHunt->m_dwLevel_High;
		}
		else
			continue;


		pos.x = iIConX*dTILE_WIDTH;
		pos.y = iIConY*dTILE_HEIGHT;

		pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
		pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;

		if	(!_rectFrame->isIn(pos.x,pos.y))
			continue;

		pos.x	-=	m_posMM.x;
		pos.y	-=	m_posMM.y;

		cRECT	rect;		
		rect.set(s_mmFrame.x1+pos.x-5,s_mmFrame.y1+pos.y-5,s_mmFrame.x1+pos.x+5,s_mmFrame.y1+pos.y+5);

		int iFPS		=	g_anmEtc.GetFPS(eEANM_MINIMAP_HUNTING_AREA);
		int iFrameCount	=	g_anmEtc.GetFrameCount(eEANM_MINIMAP_HUNTING_AREA);
		int iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount;
		
		g_anmEtc.PutReg(s_mmFrame.x1+pos.x+1-g_anmEtc.getSpriteWidth(eEANM_MINIMAP_HUNTING_AREA, 0, 0)/2,
						s_mmFrame.y1+pos.y+1-g_anmEtc.getSpriteHeight(eEANM_MINIMAP_HUNTING_AREA, 0, 0)/2,
						eEANM_MINIMAP_HUNTING_AREA,0,iFrame);

		if	(!rect.isIn(s_posMouse.x,s_posMouse.y))
			continue;

		if (g_map.m_bOptionState)
			continue;
		
		int itemp3 = ((s_mmFrame.x1 + lpArea->x1*m_tgaMinimap.m_iWidth/m_iPixelWidth-m_posMM.x) < s_mmFrame.x1)? s_mmFrame.x1 : s_mmFrame.x1 + lpArea->x1*m_tgaMinimap.m_iWidth/m_iPixelWidth-m_posMM.x;
		int itemp4 = ((s_mmFrame.y1 + lpArea->y1*m_tgaMinimap.m_iHeight/m_iPixelHeight-m_posMM.y) < s_mmFrame.y1)? s_mmFrame.y1 : s_mmFrame.y1 + lpArea->y1*m_tgaMinimap.m_iHeight/m_iPixelHeight-m_posMM.y;
		int itemp5 = ((s_mmFrame.x1 + lpArea->x2*m_tgaMinimap.m_iWidth/m_iPixelWidth-m_posMM.x) > s_mmFrame.x2)? s_mmFrame.x2 : (s_mmFrame.x1 + lpArea->x2*m_tgaMinimap.m_iWidth/m_iPixelWidth-m_posMM.x);
		int itemp6 = ((s_mmFrame.y1 + lpArea->y2*m_tgaMinimap.m_iHeight/m_iPixelHeight-m_posMM.y) > s_mmFrame.y2)? s_mmFrame.y2 : (s_mmFrame.y1 + lpArea->y2*m_tgaMinimap.m_iHeight/m_iPixelHeight-m_posMM.y);
		cDRAW::FillAB(0, itemp3, itemp4, itemp5, itemp6, 10);
		
		cSTRING strInfo;
		cSTRING strLevel;
		cSTRING strMontserName[dMAX_HUNTING_AREA_MONSTER_COUNT];
		BOOL bQuestMonster[dMAX_HUNTING_AREA_MONSTER_COUNT]={FALSE,};
		int iMonsterCnt = 0;
		strInfo.Add("<c:LTRED>%s<n>\n", lpArea->m_strName);
		strLevel.Add("Level <c:LTGREEN>%d<n>  ~  <c:LTGREEN>%d<n>", iLevel_Low, iLevel_High);

		// 사냥터 영역 안에 있는 몬스터를 찾는다.
		for (int j=0 ; j<m_iSavedActorCount ; j++)
		{
			cBASE_NPC_INFO* lpActor = &m_aSaveActor[j];

			lpActor->m_strName;
			
			if (lpActor->m_wActorKind != eAK_MONSTER)
				continue;

			if (lpArea->x1 <= lpActor->m_pos.x && lpActor->m_pos.x <= lpArea->x2 &&
				lpArea->y1 <= lpActor->m_pos.y && lpActor->m_pos.y <= lpArea->y2)
			{
				if (iMonsterCnt == 0)
				{
					strMontserName[iMonsterCnt].Add("%s", lpActor->m_strName);
					iMonsterCnt++;
				}
				else if (iMonsterCnt != 0 )		// 같은 몬스터라면 추가되지 않도록 한다.
				{
					BOOL bBreak = FALSE;
					for (int k=0 ; k<iMonsterCnt ; k++)
					{
						if (strstr(strMontserName[k].String, lpActor->m_strName) != NULL)	// 이름이 존재한다!
						{							
							bBreak = TRUE;
							break;
						}
					}
					if (bBreak) continue;

					strMontserName[iMonsterCnt].Add("%s", lpActor->m_strName);
					iMonsterCnt++;
				}
			}
		}
		
		// 영역 안의 몬스터중 퀘스트관련 몬스터인지 확인한다.
		CQuest	*lpQuestManager;
		int iIndex = 0;
		int iLimit = 0;
		
		if (g_map.m_iSelectQuest < dMAX_PROCESS_QUEST_COUNT)
		{
			iIndex = g_map.m_iSelectQuest;
			iLimit = 1;
		}
		if (g_map.m_iSelectQuest == dMAX_PROCESS_QUEST_COUNT)
		{
			iIndex = 0;
			iLimit = dMAX_PROCESS_QUEST_COUNT;
		}
		for(j=0 ; j<iLimit ; j++) 
		{
			if	(g_hero.m_aProcessQuest[iIndex].isActive		==	FALSE		)
			{
				continue;
			}
			if	(g_hero.m_aProcessQuest[iIndex].questKind	==	eQT_NORMAL	)
				lpQuestManager	=	&g_quest;
			if	(g_hero.m_aProcessQuest[iIndex].questKind	==	eQT_ARBEIT	)
				lpQuestManager	=	&g_questArbeit;
			
			int iQuestIndex	= g_hero.m_aProcessQuest[iIndex].questIndex;
			int iProcessLevel = g_hero.m_aProcessQuest[iIndex].questProcessLevel;
			CQuestItem	*lpQuest	=	lpQuestManager->getQuest(iQuestIndex);						
			CQuestItem::cQuestInfo	*lpQuestInfo	= lpQuest->getInfo(iProcessLevel);

			if(lpQuestInfo->m_wTargetMonsterCount)
			{
				for(int k=0 ; k<lpQuestInfo->m_wTargetMonsterCount ; k++)
				{
					for(int l=0 ; l<iMonsterCnt ; l++)
					{
						if (lpQuestInfo->m_pTargetMonsterList[k].m_wField == g_hero.m_wCurrentField && 
							stricmp(lpQuestInfo->m_pTargetMonsterList[k].m_pStrName, strMontserName[l].String) == 0)
							bQuestMonster[l] = TRUE;
					}
				}
			}
			iIndex++;
		}

		memcpy(&rect,&s_rectTitle,sizeof(cRECT));

		int iHUNT_TOOLTIP_WIDTH = 130;


		rect.x2	=	rect.x1-2;
		rect.x1	-=	iHUNT_TOOLTIP_WIDTH;
		
		int istrInfoHeight = CGame::s_sText.getTextHeight(iHUNT_TOOLTIP_WIDTH-4, strInfo.String);
		int istrLevelHeight = CGame::s_sText.getTextHeight(iHUNT_TOOLTIP_WIDTH-4, strLevel.String);
		rect.y1 = rect.y2;
		rect.y2 = rect.y1+istrInfoHeight+istrLevelHeight+10;

		s_ttCommon.drawBox(&rect,0,16);
		s_ttCommon.drawFrame(&rect, FALSE);
		CGame::s_sText.put(rect.x1+5, rect.y1+5, iHUNT_TOOLTIP_WIDTH-10, istrInfoHeight*(CGame::s_sText.getFontHeight()+3), 0, WHITE, strInfo.String);
		CGame::s_sText.put(rect.x1+5, rect.y1+5+istrInfoHeight+1, iHUNT_TOOLTIP_WIDTH-10, istrInfoHeight*(CGame::s_sText.getFontHeight()+4), 0, WHITE, strLevel.String);

		rect.y1 = rect.y2+1;
		rect.y2 += 10;
		for (j=0 ; j<iMonsterCnt ; j++)
			rect.y2 += (CGame::s_sText.getTextHeight(iHUNT_TOOLTIP_WIDTH-15, strMontserName[j].String)+1);
		
		s_ttCommon.drawBox(&rect,0,16);
		s_ttCommon.drawFrame(&rect, FALSE);

		int iPutPosY = 0;
		for( j=0 ; j<iMonsterCnt ; j++)
		{			
			CGame::s_sText.put(rect.x1+5, rect.y1+5+iPutPosY, iHUNT_TOOLTIP_WIDTH-10, rect.getHeight(), 0, WHITE, strMontserName[j].String);
			if (bQuestMonster[j] == TRUE)
				g_sprInterface2.Put(rect.x2-g_sprInterface2.getSpriteWidth(eSHORT_RED_Q)-3, rect.y1+5+iPutPosY, eSHORT_RED_Q);
			iPutPosY+=(CGame::s_sText.getTextHeight(iHUNT_TOOLTIP_WIDTH-10, strMontserName[j].String)+2);
		}		
	}
}
// insu function add end



// insu function add
void
cMAP::drawFogmapHuntingArea()
{
	for (int i=0;i<m_area.m_iCount;i++)
	{
		CPos		pos;
		BOOL		bIsSecretDungeonGate	=	FALSE;
		cAreaInfo	*lpArea	=	&m_area.m_aArea[i];

		int iIConX,iIConY,iLevel_Low,iLevel_High=0;

		if (lpArea->m_wSerial == 0xffff)
			continue;

		if (lpArea->m_wKind == dAREA_HUNTING_AREA)
		{
			cAreaHunting* lpHunt = (cAreaHunting*)lpArea;

			iIConX = lpHunt->m_dwIconX;
			iIConY = lpHunt->m_dwIconY;
			iLevel_Low = lpHunt->m_dwLevel_Low;
			iLevel_High = lpHunt->m_dwLevel_High;
		}
		else
			continue;


		pos.x = iIConX*dTILE_WIDTH;
		pos.y = iIConY*dTILE_HEIGHT;

		pos.x	=	pos.x*m_tgaMinimap.m_iWidth/m_iPixelWidth;
		pos.y	=	pos.y*m_tgaMinimap.m_iHeight/m_iPixelHeight;

		int iFPS		=	g_anmEtc.GetFPS(eEANM_MINIMAP_HUNTING_AREA);
		int iFrameCount	=	g_anmEtc.GetFrameCount(eEANM_MINIMAP_HUNTING_AREA);
		int iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount;
				
		g_anmEtc.PutAlpha(	m_iPutPosX+pos.x-(g_anmEtc.getSpriteWidth(eEANM_MINIMAP_HUNTING_AREA, 0, 0)/2),
							m_iPutPosY+pos.y-(g_anmEtc.getSpriteHeight(eEANM_MINIMAP_HUNTING_AREA, 0, 0)/2), 
							eEANM_MINIMAP_HUNTING_AREA, 0, iFrame, 100, 100, m_iFogmapAlpha);		
	}
}
// insu function add end