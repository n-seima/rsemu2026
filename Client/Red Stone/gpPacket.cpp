#include "CGamePlay.H"
#include "CMessage.H"
#include "CHero.H"
#include "CMap.H"
#include "arca.H"
#include "cSound.H"
#include "cEvent.H"
#include "cActor.H"
#include "CWindowInterface.H"
#include "banner.H"
#include "fireCracker.H"
#include "bm_event.H"
#include "hero_mini_pet.H"
#include "duel.H"
#include "commonUI.H"
#include "Cbank.h"		// 09.08.20
#include "CUpgradeShop.h"		// 09.08.25
void
CGamePlay::OperateItemUseResult(int _iResult)
{
	switch(_iResult)	//	결과
	{
		case	eUIR_FAILED_BY_VALID_INVENTORY_SLOT	:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UIR_FAILED_BY_VALID_INVENTORY_SLOT	);
			break;

		case	eUMCIR_ALREADY_EXIST_SHAPE			:	//	이미 추가된 모양의 양탄자다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UMCIR_ALREADY_EXIST_SHAPE	);
			break;

		case	eUMCIR_OK						:	//	이미 추가된 모양의 양탄자다.
			break;

		case	eUIR_CAN_NOT_MEMORY_FIELD			:
			g_lpHero->warning(dMSG_CAN_NOT_MEMORY_FIELD);
			break;

		case	eUIR_TOO_FAR						://	너무 멀어
			g_lpHero->warning(dMSG_TARGET_TOO_FAR);
			break;

		case	eUIR_FAILED							://	실패.. -o-
			g_lpHero->warning(dMSG_CAN_NOT_USE_ITEM);
			break;

		case	eUIR_WRONG_ITEM_DATA				:
			s_agent.sendAskItemData();
			break;

		case	eUIR_NOT_EMBODY_ITEM				://	아직 구현이 안된 아이템
			g_lpHero->warning(dMSG_NOT_EMBODY_ITEM);
			break;

		case	eUIR_LACK_ABILITY					://	아직 구현이 안된 아이템
			g_lpHero->warning(dMSG_LACK_ABILITY_ITEM_TO_USE);
			break;

		case	eUIR_UPGRADE_FAILED					:
			break;

		case	eUIR_NO_MORE_ATTACH_PREFIX			://	더이상 접두사를 붙일수 없어!!
			break;

		case	eUIR_IS_NOT_WEAPON					://	무기가 아니다!!
			break;

		case	eUIR_NO_MORE_IMPROVE_WEAPON_DAMAGE	://	이 이상 무기의 성능을 향상 시킬 수 없다.
			break;

		case	eUIR_IS_NOT_ARMOR					://	갑옷이 아니다.
			break;

		case	eUIR_NO_MORE_IMPROVE_ARMOR_DEFENSE	://	더 이상 갑옷의 성능을 향상 시킬 수 없다.
			break;

		case	eUIR_CAN_NOT_USE_ITEM				://	사용할 수 없는 아이템이다.
			g_lpHero->warning(dMSG_CAN_NOT_USE_ITEM);
			break;

		case	eUIR_DEATH_BY_REACTION				://	부작용으로 죽게 된다.-_-
			break;

		case	eUIR_STRANGE_OPTION_ITEM			://	이상한 옵션이 붙어 있다.
			break;

		case	eUIR_IS_NOT_CONSUME_ITEM			://	소비형 아이템이 아니다.
			g_lpHero->warning(dMSG_NOT_CONSUME_ITEM);
			break;

		case	eUIR_IS_NOT_CORRECT_STATUS			://	그 아이템을 쓸 만한 상황이 아녀.. -o-
			g_lpHero->warning(dMSG_IS_NOT_CORRECT_STATUS_FOR_USE_ITEM);
			break;
			
		case	eUIR_USELESS_STATUS_TO_USE_ITEM		:
			g_lpHero->warning(dMSG_USELESS_STATUS_TO_USE_ITEM);
			break;

		case	eUIR_CAN_NOT_USE_THIS_FIELD			://	그 아이템을 쓸 만한 상황이 아녀.. -o-
			g_lpHero->warning(dMSG_CAN_NOT_USE_THIS_FIELD_ITEM);
			break;

		case	eUIR_INCORRECT_TARGET				://	대상이 적절치 않다.
			g_lpHero->warning(dMSG_UIR_INCORRECT_TARGET);
			break;
	}
}
void
CGamePlay::operateSummonEventMonster()		//	소환 이벤트 몬스터
{

#ifndef	_DEBUG
	return;
#endif

	if(!g_hero.isOperator())
		return;
	
	char	strDataFN[64]	=	"data/summon_event_monster.txt";
	CDecoder	decoder;
	if	(!decoder.Upload(strDataFN,NULL))
	{

		return;
	}

	DWORD	dwCode	=	0;
	char*	str		=	NULL;
	CG_SUMMON_EVENT_MONSTER	packet;
	
	packet.base.set(sizeof(CG_SUMMON_EVENT_MONSTER),dCG_SUMMON_EVENT_MONSTER);	

	packet.wIndex					=	0xffff;
	packet.wDropItemIndex			=	0xffff;
	packet.bf10CorrectAP			=	100;
	packet.bf10CorrectEXP			=	100;
	packet.bf10CorrectHP			=	100;
	packet.bf10MaxLevel				=	999;
	packet.bf10MinLevel				=	1;
	packet.bf5Count					=	0;
	packet.bf8CorrectFinalDamage	=	100;
	packet.bf8MagicResistance		=	0;
	packet.bf8ScaleX				=	100;
	packet.bf8ScaleY				=	100;
	packet.bf8StrangeResistance		=	100;
	packet.bf7DropItemCount			=	0;
	packet.bf8DropItemChance		=	0;
	strcpy(packet.strName,"no name");
	packet.m_bf1IsImmunePhysicalDamage	=	0;
	packet.m_bf1IsImmuneMagicDamage		=	0;
	packet.bf4DropKind					=	0;
	packet.bf1IsDefaultItemDrop			=	0;
	packet.bf1IsDefaultGoldDrop			=	0;
	packet.bf1IsOwner					=	FALSE;
	memset(packet.aiOption,0xffff,sizeof(packet.aiOption));
	while(dwCode!=dCODE_END )
	{	
		str = decoder.getWord();
		dwCode	=	decoder.analCode(str);
		if(strcmp(str, "is_immune_physical_damage") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 1)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			packet.m_bf1IsImmunePhysicalDamage = iValue;
			continue;
		}
		if(strcmp(str, "is_drop_item_owner") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 1)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			packet.bf1IsOwner = iValue;
			continue;
		}
		if(strcmp(str, "drop_item_option1") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 255)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			packet.aiOption[0] = iValue;
			continue;
		}
		if(strcmp(str, "drop_item_option2") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 255)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			packet.aiOption[1] = iValue;
			continue;
		}
		if(strcmp(str, "drop_item_option3") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 255)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			packet.aiOption[2] = iValue;
			continue;
		}
		if(strcmp(str, "drop_item_option4") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 255)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			packet.aiOption[3] = iValue;
			continue;
		}
		if(strcmp(str, "is_immune_magic_damage") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 1)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			packet.m_bf1IsImmuneMagicDamage = iValue;
			continue;
		}
		if(strcmp(str, "is_default_item_drop") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 1)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			packet.bf1IsDefaultItemDrop = iValue;
			continue;
		}
		if(strcmp(str, "is_default_gold_drop") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 1)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			packet.bf1IsDefaultGoldDrop = iValue;
			continue;
		}
		if(strcmp(str, "drop_kind") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 2)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			packet.bf4DropKind = iValue;
			continue;
		}
		if(strcmp(str, "name") == 0)
		{	
			char* strName	=	decoder.getStream();
			DWORD	dwNameCode	=	decoder.analCode(strName);
			if(dwNameCode	==	dCODE_END)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(strlen(strName)>=dNAME_LENGTH)
			{
				AddSystemMessage(LTYELLOW,"%s", "name is long!!");
				return;
			}
			strcpy(packet.strName,strName);
			continue;
		}
		if(strcmp(str, "index") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			int aiApplyIndex[] = 
			{	//	소환 가능한 몬스터 인덱스
				4,		313,	564,
				14,		323,	574,
				29,		338,	598,
				383,	634,	114,
				423,	674,	134,
				443,	694,	144,
				453,	704,	234,
				552,	803,	899,
				904,	909,	944,
				949,	1279,	959,
				964,	1280,	1004,
				1009,	1017,	1049,
				1054,	1062,	815,
				816,	817,
				0xffff,
			};
#ifndef	_IS_DEV_CLIENT
			for(int i=0;;++i)
			{
				if(aiApplyIndex[i] == iValue)
					break;
				if(aiApplyIndex[i] == 0xffff)
				{
					AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
					return;
				}
			}
#endif
			packet.wIndex = iValue;
			continue;
		}
		if(strcmp(str, "drop_item_index") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue >= dBASIC_ITEM_COUNT)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;			
			}
			if(iValue < 0 || iValue >= dBASIC_ITEM_COUNT)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;			
			}			
			if(g_aBasicItem[iValue].m_iSerial == 0xffff)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;			
			}	
			packet.wDropItemIndex = iValue;
			continue;
		}
		if(strcmp(str, "drop_item_count") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 30)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;			
			}
			packet.bf7DropItemCount = iValue;
			continue;
		}
		if(strcmp(str, "drop_item_chance") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 100)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;			
			}
			packet.bf8DropItemChance = iValue;
			continue;
		}
		if(strcmp(str, "correct_attack_power") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 500)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;			
			}
			packet.bf10CorrectAP = iValue;
			continue;
		}
		if(strcmp(str, "correct_exp") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 500)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;			
			}
			packet.bf10CorrectEXP = iValue;
			continue;
		}
		if(strcmp(str, "correct_hp") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 500)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;			
			}
			packet.bf10CorrectHP = iValue;
			continue;
		}
		if(strcmp(str, "max_level") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 1 || iValue > 999 || iValue < packet.bf10MinLevel)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;			
			}
			packet.bf10MaxLevel = iValue;
			continue;
		}
		if(strcmp(str, "min_level") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 1 || iValue > 999 || iValue > packet.bf10MaxLevel)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;			
			}
		
			packet.bf10MinLevel = iValue;
			continue;
		}
		if(strcmp(str, "count") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 10 )
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;			
			}
			packet.bf5Count = iValue;
			continue;
		}
		if(strcmp(str, "correct_final_damage") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 1 || iValue > 100 )
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;			
			}
			packet.bf8CorrectFinalDamage  = iValue;
			continue;
		}
		if(strcmp(str, "magic_resistance") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 200 )
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;			
			}
			packet.bf8MagicResistance = iValue;
			continue;
		}
		if(strcmp(str, "scaleX") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 10 || iValue > 200 )
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;			
			}
			packet.bf8ScaleX = iValue;
			continue;
		}
		if(strcmp(str, "scaleY") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 10 || iValue > 200 )
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;			
			}
			packet.bf8ScaleY = iValue;
			continue;
		}
		if(strcmp(str, "strange_resistance") == 0)
		{	
			int iValue	=	decoder.getNumber();
			if(iValue	==	dCODE_VALID_NUMBER)
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;
			}
			if(iValue < 0 || iValue > 100 )
			{
				AddSystemMessage(LTYELLOW,"%s %s", "strange!!", str);
				return;			
			}
			packet.bf8StrangeResistance  = iValue;
			continue;
		}		
	}

	s_agent.sendPacket((char*)&packet,packet.base.wSize);	
}
void
CGamePlay::OperateSiegeWarfareNotice(SG_SIEGE_WARFARE_NOTICE	*_lpPacket)
{
	char	strText[256];

	switch(_lpPacket->wWork)
	{
		case	eSW_BEGIN_NOTICE		:
			sprintf(strText,dMSG_REMAIN_TIME_TO_BEGIN_GUILD_BATTLE_FORM,_lpPacket->wValue);
			CGamePlay::Notice(strText);
			break;

		case	eSW_END_NOTICE			:
			sprintf(strText,dMSG_REMAIN_TIME_TO_END_GUILD_BATTLE_FORM,_lpPacket->wValue);
			CGamePlay::Notice(strText);
			break;

		case	eSW_BEGIN				:
			CGamePlay::Notice(dMSG_BEGUN_GUILD_BATTLE);
			break;

		case	eSW_END					:
			break;

		case	eSW_SUCCESS_TO_DEFENSE	:
			g_msgBox.cPopup("",dMSG_SUCCESS_TO_DEFENSE_GUILD_HALL,dMSG_OK);
			g_msgBox.setButtonId(eMBI_RETURN_TO_GUILD_HALL_BY_END_BATTLE);
			break;
		case	eSW_SUCCESS_TO_INVADE	:
			if	(s_bIsSiegeWarfareField)
			{
				g_msgBox.cPopup("",dMSG_SUCCESS_TO_CAPTURE_GUILD_HALL,dMSG_OK);
				g_msgBox.setButtonId(eMBI_RETURN_TO_GUILD_HALL_BY_END_BATTLE);
			}
			break;
		case	eSW_FAILED_TO_DEFENSE	:
			g_msgBox.cPopup("",dMSG_FAILED_TO_DEFENSE_GUILD_HALL,dMSG_OK);
			g_msgBox.setButtonId(eMBI_RETURN_TO_GUILD_HALL_BY_END_BATTLE);
			break;
		case	eSW_FAILED_TO_INVADE	:
			g_msgBox.cPopup("",dMSG_FAILED_TO_CAPTURE_GUILD_HALL,dMSG_OK);
			g_msgBox.setButtonId(eMBI_RETURN_TO_GUILD_HALL_BY_END_BATTLE);
			break;
	}
}

void
CGamePlay::OperateDoorWork(SG_DOOR_MESSAGE *_lpPacket)
{
	cAreaDoorInfo	*lpDoor		=	(cAreaDoorInfo	 *)g_map.m_area.getArea(_lpPacket->wArea);

	if	(!lpDoor)
		return;

	g_hero.m_bf1IsWaitDoorWorkResult	=	FALSE;

	switch(_lpPacket->wMessage)
	{
		case	eDWM_CAN_NOT_DESTROY_DOOR	:
			AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_DESTROY_DOOR);
			break;

		case	eDWM_OPEN					:
			if	(STRICMP(_lpPacket->strCasterName,g_hero.m_strName)	==	0)
				lpDoor->m_bf1IsOpenedByHero	=	TRUE;
			else
				lpDoor->m_bf1IsOpenedByHero	=	FALSE;

			g_esm.play("open_door.wav");
			break;

		case	eDWM_IS_FULL_SECRET_DUNGEON	:
			AddSystemMessage(LTYELLOW,dMSG_FULL_SECRET_DUNGEON);
			break;

		case	eDWM_IS_REQURE_PARTY		:
			AddSystemMessage(LTYELLOW,dMSG_ENTER_TO_SECRET_DUNGEON_FAILED_BY_NOT_PARTY);
			break;

		case	eDWM_UNLOCK_BY_SKILL	:	//	스킬로 열었다.
			g_esm.play("Lock Pick_hit.wav");
			AddSystemMessage(LTYELLOW,dMSG_UNLOCK_DOOR_BY_SKILL_FORM,_lpPacket->strCasterName);
			break;

		case	eDWM_UNLOCK_BY_ATTACK	:
			AddSystemMessage(LTYELLOW,dMSG_UNLOCK_BY_DESTROY_LOCK);
			break;

		case	eDWM_UNLOCK_BY_KEY		:	//	열쇠로 열었다.
		{
			if(STRICMP(_lpPacket->strCasterName,g_hero.m_strName) == 0)
			{
				cItem	*lpItem	=	g_hero.getItem(_lpPacket->wKey);

				if (lpItem)
					lpItem->decreaseCount(1);
			}

			AddSystemMessage(LTYELLOW,dMSG_UNLOCK_DOOR_BY_KEY_FORM,_lpPacket->strCasterName);
			break;
		}

		case	eDWM_TRY_DISARM_FAILED_BY_LOW_SKILL_LEVEL	:
			AddSystemMessage(LTYELLOW,dMSG_FAILED_DISARM_BY_LOW_DISARM_LEVEL);
			break;

		case	eDWM_DISARM				:	//	해체 했다.
			g_esm.play("Disarm Trap_hit.wav");
			AddSystemMessage(LTYELLOW,dMSG_DISARM_TRAP_BY_SKILL_FORM,_lpPacket->strCasterName);
			break;

		case	eDWM_DISARM_FAILED		:	//	해체 실패 했다.
			AddSystemMessage(LTYELLOW,dMSG_DISARM_TRAP_FAILED);
			break;

		case	eDWM_DISARM_FAILED_AND_EXPLOSION_TRAP		:
			g_esm.play("Trap_act.wav");
			AddSystemMessage(LTYELLOW,dMSG_DISARM_FAILED_AND_EXPLOSION_TRAP);
			break;

		case	eDWM_IS_LOCKED			:	//	잠겨 있는 문이다.
			g_map.changeDoorStatus(_lpPacket->wArea,1-lpDoor->m_bf1IsActive,lpDoor->m_bf1IsHide,TRUE,1-lpDoor->m_bf1IsDisarmed,1-lpDoor->m_bf1IsOpened,lpDoor->m_bf1IsActiveByKarma);
			break;
		case	eDWM_IS_UNLOCKED		:	//	열려 있는 문이다.
			g_map.changeDoorStatus(_lpPacket->wArea,1-lpDoor->m_bf1IsActive,lpDoor->m_bf1IsHide,FALSE,1-lpDoor->m_bf1IsDisarmed,1-lpDoor->m_bf1IsOpened,lpDoor->m_bf1IsActiveByKarma);
			break;
		case	eDWM_IS_CLOSED			:	//	해체 실패 했다.
			g_map.changeDoorStatus(_lpPacket->wArea,1-lpDoor->m_bf1IsActive,lpDoor->m_bf1IsHide,1-lpDoor->m_bf1IsUnlocked,1-lpDoor->m_bf1IsDisarmed,TRUE,lpDoor->m_bf1IsActiveByKarma);
			break;
		case	eDWM_IS_OPENED			:	//	열려 있는 문입니다.
			g_map.changeDoorStatus(_lpPacket->wArea,1-lpDoor->m_bf1IsActive,lpDoor->m_bf1IsHide,1-lpDoor->m_bf1IsUnlocked,1-lpDoor->m_bf1IsDisarmed,FALSE,lpDoor->m_bf1IsActiveByKarma);
			break;
		case	eDWM_IS_SLEEP_HIDE		:	//	사라진 문이다.
			g_map.changeDoorStatus(_lpPacket->wArea,TRUE,lpDoor->m_bf1IsHide,1-lpDoor->m_bf1IsUnlocked,1-lpDoor->m_bf1IsDisarmed,1-lpDoor->m_bf1IsOpened,lpDoor->m_bf1IsActiveByKarma);
			break;

		case	eDWM_IS_HIDE_DOOR		:	//	감춰져 있는 문이다.
			g_map.changeDoorStatus(_lpPacket->wArea,1-lpDoor->m_bf1IsActive,TRUE,1-lpDoor->m_bf1IsUnlocked,1-lpDoor->m_bf1IsDisarmed,1-lpDoor->m_bf1IsOpened,lpDoor->m_bf1IsActiveByKarma);
			break;

		case	eDWM_IS_DISARMED		:
			g_map.changeDoorStatus(_lpPacket->wArea,1-lpDoor->m_bf1IsActive,lpDoor->m_bf1IsHide,1-lpDoor->m_bf1IsUnlocked,FALSE,1-lpDoor->m_bf1IsOpened,lpDoor->m_bf1IsActiveByKarma);
			break;

		case	eDWM_CAN_NOT_UNLOCK_DOOR_BY_NOT_EXIST_AVAIL_KEY		:	//	그 자물쇠 열만한 열쇠가 없다.
			AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_UNLOCK_DOOR_BY_NOT_EXIST_AVAIL_KEY);
			break;

		case	eDWM_NOT_EXIST_AVAIL_KEY_OR_SKILL_FOR_UNLOCK_DOOR	:	//	그 자물쇠 열만한 스킬이 없다.
			AddSystemMessage(LTYELLOW,dMSG_NOT_EXIST_AVAIL_SKILL_OR_KEY_TO_UNLOCK);
			break;

		case	eDWM_DO_YOU_WANT_UNLOCK_DOOR_USE_KEY	:
		{
			cItem	*lpKey	=	g_hero.getInventoryItem(_lpPacket->wKey);

			if (!lpKey)
				break;

			char	strText[256];

			sprintf(strText,dMSG_DO_YOU_WANT_UNLOCK_USE_KEY_FORM,lpKey->getName());

			g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_UNLOCK_DOOR_BY_USE_KEY);
			g_hero.m_wWorkDoor	=	_lpPacket->wArea;
			break;
		}

		case	eDWM_EXPLOSION_TRAP_BY_ATTACK						:	//	함정이 발동해서 터졌다.
			g_esm.play("Trap_act.wav");
			AddSystemMessage(LTYELLOW,dMSG_EXPLOSION_TRAP);
			break;

		case	eDWM_TRY_DISARM_FAILED_BY_LOW_CP				:	//	함정이 발동해서 터졌다.
			AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_DISARM_BY_LOW_CP);
			break;
		case	eDWM_CAN_NOT_DISARM_TRAP					:	//	함정이 발동해서 터졌다.
			AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_DISARM_TRAP);
			break;
		case	eDWM_UNLOCK_BY_SKILL_FAILED					:		//	열기 실패했다.
			AddSystemMessage(LTYELLOW,dMSG_UNLOCK_FAILED);
			break;

		case	eDWM_IS_ARMED_TRAP	:		//	함정이 걸려 있는 문이다.
		{
			g_msgBox.cPopup("",dMSG_IS_ARMED_DO_YOU_WANT_DISARM_THIS_TRAP,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_DOOR_DISARM_TRAP);
			g_hero.m_wWorkDoor	=	_lpPacket->wArea;
			break;
		}

		case	eDWM_CAN_NOT_UNLOCK_DOOR_BY_LOW_UNLOCK_LEVEL:	//	자물쇠 해체 레벨이 낮아서 해체 불가
			AddSystemMessage(LTYELLOW,dMSG_UNLOCK_FAILED_BY_LOW_UNLOCK_LEVEL);
			break;

		case	eDWM_TRY_DISARM_BY_SKILL:
			g_esm.play("Disarm Trap_hit.wav");
			AddSystemMessage(LTYELLOW,dMSG_TRY_DISARM_TRAP);
			break;

		case	eDWM_TRY_UNLOCK_BY_SKILL:
			g_esm.play("Lock Pick_act.wav");
//	타격효과음 "Lock Pick_hit.wav"

			AddSystemMessage(LTYELLOW,dMSG_TRY_UNLOCK_DOOR_BY_SKILL);
			break;

		case	eDWM_TRY_UNLOCK_FAILED_BY_LOW_CP:
			AddSystemMessage(LTYELLOW,dMSG_UNLOCK_FAILED_BY_LOW_CP);
			break;

		case	eDWM_TRY_DISARM_FAILED_BY_BUSY:	//	현재 상태에서는 함정 해체 작업을 시도 할 수 없다.
			break;
		case	eDWM_TRY_UNLOCK_FAILED_BY_BUSY:
			break;
		case	eDWM_EXPLOSION_TRAP_BY_CAN_NOT_DETECT_TRAP	:	//	찾지 못한 함정이 발동해서 터졌다.
			g_esm.play("Trap_act.wav");
			AddSystemMessage(LTYELLOW,dMSG_EXPLOSION_TRAP);
			break;

		case	eDWM_FAILED_TO_ENTER_THE_SECRET_DUNGEON		:
			g_map.m_area.m_wInstanceFieldGate	=	0xffff;
			s_agent.sendAskDoorInfo(_lpPacket->wArea);
			break;

		case	eDWM_ALREADY_USE_SECRET_DUNGEON			:
			AddSystemMessage(LTYELLOW,dMSG_ALREADY_USE_SECRET_DUNGEON_PARTY);
			break;

		case	eDWM_UNLOCK_BY_KARMA					:
			g_esm.play("Lock Pick_hit.wav");
			break;
	}

}

void
CGamePlay::OperateFieldTrapWork(SG_CLOSE_RANGE_FIELD_TRAP_WORK *_lpPacket)
{
	g_hero.m_bf1IsWaitTrapWorkResult	=	FALSE;

	switch(_lpPacket->wWork)
	{
		case	eFTW_CATCH		:		//	결렸다!
			g_hero.m_wCaughtFieldTrap	=	_lpPacket->wTrapIndex;
			g_msgBox.cPopup("",dMSG_WARNING_CAUGHT_BY_TRAP,dMSG_OK);
			break;

		case	eFTW_DISARMED_BY_SKILL	:	//	해체 했다.
			g_esm.play("Disarm Trap_hit.wav");
			AddSystemMessage(WHITE,dMSG_DISARM_TRAP_BY_SKILL_FORM,_lpPacket->strName);
			g_map.m_area.disactiveTrap(_lpPacket->wTrapIndex);
			break;

		case	eFTW_DETECTED	:	//	탐지 했다.
			g_hero.m_wWorkFieldTrap		=	_lpPacket->wTrapIndex;
			g_msgBox.cPopup("",dMSG_DETECT_TRAP_DO_YOU_WANT_TRY_DISARM_THIS_TRAP,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_DISARM_FIELD_TRAP);
			break;

		case	eFTW_ACTIVE		:	//	활성화 됨
			g_map.m_area.activeTrap(_lpPacket->wTrapIndex);
			break;

		case	eFTW_DISACTIVE	:	//	비 활성
			g_map.m_area.disactiveTrap(_lpPacket->wTrapIndex);
			break;

		case	eFTW_FAILED_BY_BAD_STATUS	:	//	그 작업을 할 상태가 아니다.
			AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_ACTION_BY_BAD_STATUS);
			break;

		case	eFTW_FAILED_DISARM_BY_RANGE:	//	사정거리를 벗어났다.
			AddSystemMessage(LTYELLOW,dMSG_FTW_FAILED_DISARM_BY_RANGE);
			break;

		case	eFTW_FAILED_BY_IS_DISACTIVE		:
			g_map.m_area.disactiveTrap(_lpPacket->wTrapIndex);
			break;

		case	eFTW_FAILED_BY_IS_CAN_NOT_DISARM_TRAP_BY_LOW_DISARM_LEVEL:	//	해체 스킬이 있는데 레벨이 낮아서 해체 못함
			AddSystemMessage(LTYELLOW,dMSG_FAILED_DISARM_BY_LOW_DISARM_LEVEL);
			break;

		case	eFTW_FAILED_BY_NOT_USE_ABLE_SKILL:	//	해체 스킬이 없다.
			AddSystemMessage(LTYELLOW,dMSG_NOT_EXIST_DISARM_SKILL);
			break;

		case	eFTW_DISARM_FAILED_AND_EXPLOSION_TRAP:	//	함정 해체 실패하고 폭발했다.
			g_esm.play("Trap_act.wav");
			AddSystemMessage(LTYELLOW,dMSG_DISARM_FAILED_AND_EXPLOSION_TRAP);
			break;

		case	eFTW_DISARM_BY_SKILL_FAILED		:		//	스킬로 함정 해체 시도 했는데 실패 했다.
			AddSystemMessage(LTYELLOW,dMSG_DISARM_TRAP_FAILED);
			break;

		case	eFTW_TRY_DISARM					:
			AddSystemMessage(LTYELLOW,dMSG_TRY_DISARM_TRAP);
			break;

		case	eFTW_TRY_DISARM_FAILED_BY_IS_LOW_CP:
			AddSystemMessage(LTYELLOW,dMSG_TRY_DISARM_TRAP_FAILED_BY_LOW_CP);
			break;

		case	eFTW_EXPLOSION	:	//	터졌다.
			g_esm.play("Trap_act.wav");
			break;

		case	eFTW_FAILED_BY_VALID_TRAP	:
			break;

		case	eFTW_FAILED_BY_IS_CAN_NOT_DETECT:
			break;

		case	eFTW_TRY_DISARM_FAILED_BY_BUSY:
			break;
	}
}

void
CGamePlay::OperateArcaWork(SG_CLOSE_RANGE_ARCA_WORK	*_lpPacket)
{
	g_hero.m_bf1IsWaitArcaWorkResult	=	FALSE;

	switch(_lpPacket->wWork)
	{
		case	eAW_OPEN			:
			if	(_lpPacket->wValue1	==	g_lpHero->m_wSerial)
				g_esm.play("Lock Pick_hit.wav");
			g_arcaManager.openArca(_lpPacket->wArcaIndex);
			break;
		case	eAW_IS_HIDE		:
			g_arcaManager.operateAW_IsHideArca(_lpPacket->wArcaIndex);
			break;
		case	eAW_IS_TRAP		:
			g_arcaManager.operateAW_IsTrapArca(_lpPacket->wArcaIndex);
			break;
		case	eAW_IS_LOCKED		:
			g_arcaManager.operateAW_IsLockedArca(_lpPacket->wArcaIndex);
			break;
		case	eAW_IS_FAILED		:
			g_arcaManager.operateAW_IsFailedWork(_lpPacket->wArcaIndex);
			break;

		case	eAW_DO_YOU_WANT_UNLOCK_ARCA_BY_USE_KEY	:
		{
			cItem	*lpKey	=	g_hero.getInventoryItem(_lpPacket->wValue1);

			if (!lpKey)
				break;

			char	strText[256];

			sprintf(strText,dMSG_DO_YOU_WANT_UNLOCK_USE_KEY_FORM,lpKey->getName());

			g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_UNLOCK_ARCA_BY_USE_KEY);
			g_arcaManager.m_wWorkArca	=	_lpPacket->wArcaIndex;
			break;
		}

		case	eAW_UNLOCK_FAILED_BY_ALREADY_UNLOCKED	:
			AddSystemMessage(LTYELLOW,dMSG_ARCA_ATTACK_FAILED_BY_ALREADY_UNLOCKED);
			break;

		case	eAW_UNLOCK_BY_KEY	:
			g_esm.play("Lock Pick_hit.wav");
			g_arcaManager.operateAW_UnlockByKey(_lpPacket->wArcaIndex,_lpPacket->strName,_lpPacket->wValue1);
			break;

		case	eAW_UNLOCK_BY_SKILL	:
			g_arcaManager.operateAW_UnlockBySkill(_lpPacket->wArcaIndex,_lpPacket->strName);
			g_esm.play("Lock Pick_hit.wav");
			break;

		case	eAW_CAN_NOT_UNLOCK	:
			AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_UNLOCK_ARCA);
			break;

		case	eAW_EXPLOSION_TRAP	:
			g_esm.play("Trap_act.wav");
			WarningMessage(dMSG_EXPLOSION_ARCA_TRAP);
			break;

		case	eAW_FULL_INVENTORY	:
			AddSystemMessage(LTYELLOW,dMSG_GET_ARCA_ITEM_FAILED_BY_INVENTORY_FULL);
			break;
		case	eAW_FULL_GOLD		:
			AddSystemMessage(LTYELLOW,dMSG_GET_ARCA_GOLD_FAILED_BY_GOLD_FULL);
			break;

		case	eAW_REMOVE			:
			g_arcaManager.removeArca(_lpPacket->wArcaIndex,TRUE);
			break;

		case	eAW_UNLOCK_BY_ATTACK:
			if	(_lpPacket->wValue1	==	g_lpHero->m_wSerial)
				AddSystemMessage(LTYELLOW,dMSG_UNLOCK_BY_DESTROY_LOCK);
			g_arcaManager.unlokcByAttack(_lpPacket->wArcaIndex);
			break;

		case	eAW_ATTACK_FAILED_BY_ALREADY_UNLOCKED:
			AddSystemMessage(LTYELLOW,dMSG_ARCA_ATTACK_FAILED_BY_ALREADY_UNLOCKED);
			break;

		case	eAW_ATTACK_ARCA_FAILED_BY_LOW_LEVEL	:
			AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_DESTROY_LOCK_BY_LOW_LEVEL_FORM,_lpPacket->wValue1);
			break;

		case	eAW_ATTACK_ARCA_FAILED_BY_INVINCIBLE:
			AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_DESTROY_LOCK);
			break;

		case	eAW_FAILED_BY_BAD_STATUS	:
			AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_ACTION_BY_BAD_STATUS);
			break;

		case	eAW_TRY_UNLOCK_BY_SKILL:
			AddSystemMessage(LTYELLOW,dMSG_TRY_UNLOCK_BY_SKILL);
			g_esm.play("Lock Pick_act.wav");
			break;

		case	eAW_UNLOCK_FAILED_BY_LOW_UNLOCK_LEVEL:
			AddSystemMessage(LTYELLOW,dMSG_UNLOCK_FAILED_BY_LOW_UNLOCK_LEVEL);
			break;

		case	eAW_TRY_UNLOCK_FAILED_BY_LOW_CP:
			AddSystemMessage(LTYELLOW,dMSG_UNLOCK_FAILED_BY_LOW_CP);
			break;

		case	eAW_DISARM_FAILED_BY_ALREADY_DISARMED:
			AddSystemMessage(LTYELLOW,dMSG_DISARM_FAILED_BY_ALREADY_DISARMED);
			break;

		case	eAW_DISARM_FAILED_BY_NOT_EXIST_DISARM_SKILL:
			AddSystemMessage(LTYELLOW,dMSG_NOT_EXIST_DISARM_SKILL);
			break;

		case	eAW_TRY_DISARM_FAILED_BY_LOW_CP:
			AddSystemMessage(LTYELLOW,dMSG_TRY_DISARM_TRAP_FAILED_BY_LOW_CP);
			break;

		case	eAW_TRY_DISARM_BY_SKILL:
			AddSystemMessage(LTYELLOW,dMSG_TRY_DISARM_TRAP);
			break;

		case	eAW_UNLOCK_FAILED		:
			AddSystemMessage(LTYELLOW,dMSG_UNLOCK_FAILED);
			break;

		case	eAW_DISARM_FAILED_AND_EXPLOSION_TRAP	:
			g_esm.play("Trap_act.wav");
			AddSystemMessage(LTYELLOW,dMSG_DISARM_FAILED_AND_EXPLOSION_TRAP);
			break;

		case	eAW_FAILED_DISARM_BY_LOW_DISARM_LEVEL	:
			AddSystemMessage(LTYELLOW,dMSG_FAILED_DISARM_BY_LOW_DISARM_LEVEL);
			break;

		case	eAW_FAILED_DISARM						:
			AddSystemMessage(LTYELLOW,dMSG_DISARM_TRAP_FAILED);
			break;

		case	eAW_DISARMED_BY_SKILL					:
			g_esm.play("Disarm Trap_hit.wav");
			g_arcaManager.operateAW_DisarmTrap(_lpPacket->wArcaIndex);
			AddSystemMessage(WHITE,dMSG_DISARM_TRAP_BY_SKILL_FORM,_lpPacket->strName);
			break;

		case	eAW_TRY_UNLOCK_FAILED_BY_BUSY			:	//	그 스킬을 쓸만한 상황이 아니다.
			break;

		case	eAW_ATTACK_ARCA_FAILED					:
			break;

		case	eAW_ATTACK_ARCA							:
			break;

		case	eAW_TRY_DISARM_FAILED_BY_BUSY			:
			break;

		case	eAW_EXPLOSION_TRAP_BY_NOT_DETECTED		:
			g_esm.play("Trap_act.wav");
			g_arcaManager.operateAW_DisarmTrap(_lpPacket->wArcaIndex);
			break;

		case	eAW_IS_OWNED_OTHER						:
			AddSystemMessage(LTYELLOW,dMSG_IS_OWNED_OTHER_ARCA);
			break;
	}
}


void
CGamePlay::OperateSecretDungeonMessage(SG_SECRET_DUNGEON_MESSAGE *_lpPacket)
{
	switch(_lpPacket->wMessage)
	{
		case	eSDM_GET_GUILD_HONOR_POINT		:
		{
			if	(_lpPacket->iValue1	<	0)
				AddSystemMessage(LTYELLOW,dMSG_REDUCE_GUILD_HONOR_POINT_FORM,-_lpPacket->iValue1);

			g_guild.m_iGuildHonorPoint	=	_lpPacket->iValue2;
			break;
		}

		case	eSDM_DISSOLUTION_PARTY			:
			WarningMessage(dMSG_SDM_DISSOLUTION_PARTY);
			g_countDown.begin(_lpPacket->iValue1,(void *)&s_textBold24M,cDRAW::HEIGHT/12,TRUE);
			break;

		case	eSDM_GUILD_DUNGEON_COMPLETE		:
		case	eSDM_COMPLETE					:
		{
			if(_lpPacket->wMessage == eSDM_COMPLETE)
			{
				if	(_lpPacket->iValue2	==	0)
					WarningMessage(_ms(dMSG_SDM_COMPLETE_FORM,_lpPacket->iValue1));
				else
				if	(_lpPacket->iValue2	==	1)
					WarningMessage(_ms(dMSG_SDM_END_FORM,_lpPacket->iValue1));

			}
			else if(_lpPacket->wMessage == eSDM_GUILD_DUNGEON_COMPLETE)
			{
				if	(_lpPacket->iValue2	==	0)
					WarningMessage(_ms(dMSG_GDM_COMPLETE_FORM,_lpPacket->iValue1));
				else
				if	(_lpPacket->iValue2	==	1)
					WarningMessage(_ms(dMSG_GDM_END_FORM,_lpPacket->iValue1));
			}

			g_countDown.begin(_lpPacket->iValue1,(void *)&s_textBold24M,cDRAW::HEIGHT/12,TRUE);
			break;
		}

		case	eSDM_WARNING_DISAPEAR_DUNGEON	:
		{
			Notice(_ms(dMSG_WARNING_DISAPEAR_DUNGEON_FORM,_lpPacket->iValue1));
			break;
		}

		case	eSDM_WARNING_DISAPEAR_GUILD_DUNGEON	:
		{
			Notice(_ms(dMSG_WARNING_DISAPEAR_GUILD_DUNGEON_FORM,_lpPacket->iValue1));
			break;
		}
		
		

		case	eSDM_BEGIN_COUNTDOWN_TIMER		:
		{
			g_countDown.begin(_lpPacket->iValue1,(void *)&s_textBold24M,cDRAW::HEIGHT/12,TRUE);
			break;
		}
		case	eSDM_BEGIN_EVENT				:
		{
			s_bIsProgressEvent	=	TRUE;
			break;
		}
		case	eSDM_END_EVENT					:
		{
			s_bIsProgressEvent	=	FALSE;
			break;
		}
		case	eSDM_FOCUS_ON_GROUND			:
			g_am.setGroundFocus(_lpPacket->iValue1,_lpPacket->iValue2,0);
			break;

		case	eSDM_FOCUS_ON_CHARACTER			:
			g_am.setCharacterFocus(_lpPacket->iValue1);
			break;

		case	eSDM_RELEASE_FOCUS				:
			g_am.releaseKarmaFocus();
			break;

		case	eSDM_STRESS_MINIMAP				:
			g_am.m_posStress.x	=	_lpPacket->iValue1;
			g_am.m_posStress.y	=	_lpPacket->iValue2;
			break;

		case	eSDM_RELEASE_STRESS_MINIMAP		:
			g_am.m_posStress.x	=	-1;
			g_am.m_posStress.y	=	-1;
			break;
	}
}

void
CGamePlay::OperateEtcWork(SG_ETC_WORK *_lpPacket)
{
	CActor	*lpActor	=	NULL;

	if	(_lpPacket->wWork	<	eEW_ARRAY_INDEX_ZERO_IS_ACTOR_SERIAL_BORDER	)
	{
		lpActor		=	g_am.getTestedActor(_lpPacket->aiValue[0]);

		if	(!lpActor)
			return;
	}

	switch(_lpPacket->wWork)
	{
		case eEW_ADD_BIT:
			{	
				CActor	*lpTarget		=	g_am.getTestedActor(_lpPacket->aiValue[0]);
				
				if	(!lpTarget)
					return;

				lpTarget->addBit(_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
				break;
			}
		case eEW_REMOVE_BIT:
			{	
				CActor	*lpTarget		=	g_am.getTestedActor(_lpPacket->aiValue[0]);
				
				if	(!lpTarget)
					return;

				lpTarget->removeBit(_lpPacket->aiValue[1],_lpPacket->aiValue[2], _lpPacket->aiValue[3]);
				break;
			}
		case	eEW_ASK_GVG_AVATAR_ORG_DATA_RESULT	:
		{
			int		iResult			=	_lpPacket->aiValue[0];

			switch(iResult)
			{
				case	eGET_GVG_AVATAR_ORG_DATA_RESULT_SUCCESS:
				case	eGET_GVG_AVATAR_ORG_DATA_RESULT_SUCCESS_BY_IS_MOVEFIELD:
					AddSystemMessage(LTGREEN,dMSG_COMPLETE_SYNC_GVG_AVATAR_DATA);
					g_bIsWaitGetGVGAvatarOrgData=	FALSE;
					break;

				case	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_ORG_AVATAR:
					g_msgBox.cPopup("",dMSG_CAN_NOT_FIND_ORG_AVATAR,dMSG_OK);
					break;

				case	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED	:
				case	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_AVATAR		:
				case	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_LOGINSERVER	:
				case	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_WORLDSERVER	:
				case	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_DBCSERVER	:
					g_msgBox.cPopup("",_ms(dMSG_FAILED_SYNC_GVG_AVATAR_BY_INCORRECT_SERVER_STATUS_FORM	,iResult),dMSG_OK);
					break;
			}

			break;
		}
		
		case	eEW_TRANS_GUILD_TO_GVG_RESULT	:
		{
			int	iResult			=	_lpPacket->aiValue[0];
			int	iServerIndex	=	_lpPacket->aiValue[1];
			int	iYear			=	_lpPacket->aiValue[2];
			int	iMonth			=	_lpPacket->aiValue[3];
			int	iDay			=	_lpPacket->aiValue[4];

			switch(iResult)
			{
				case	eTRAN_GUILD_TO_GVG_RESULT_SUCCESS	:
					break;

				case	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_PERIOD	:
					g_msgBox.cPopup("",_ms(dMSG_TRANS_GUILD_TO_GVG_SERVER_RULE,iYear,iMonth,iDay),dMSG_OK);
					break;

				case	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_MISMATCH_LEVEL	:
					g_msgBox.cPopup("",dMSG_TRAN_GUILD_TO_GVG_RESULT_FAILED_BY_MISMATCH_LEVEL,dMSG_OK);
					break;

				case	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_WAIT_FOR_RESULT	:
					g_msgBox.cPopup("",dMSG_WAIT_FOR_TRANS_GUILD_RESULT,dMSG_OK);
					break;

				case	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_GUILD_NOT_EXIST	:
					g_msgBox.cPopup("",dMSG_CAN_NOT_TRANS_GUILD_IN_GVG_SERVER,dMSG_OK);
					break;

				default	:
					g_msgBox.cPopup("",_ms(dMSG_TRANS_GUILD_FAILED,iResult),dMSG_OK);
					break;
			}

			break;
		}

		case	eEW_TRANS_AVATAR_TO_GVG_RESULT	:
		{
			int	iResult			=	_lpPacket->aiValue[0];
			int	iServerIndex	=	_lpPacket->aiValue[1];
			int	iYear			=	_lpPacket->aiValue[2];
			int	iMonth			=	_lpPacket->aiValue[3];
			int	iDay			=	_lpPacket->aiValue[4];

			switch(iResult)
			{
				case	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_ALREADY_EXIST_DIFF_AVATAR	:
					g_msgBox.cPopup("",dMSG_TRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_ALREADY_EXIST_DIFF_AVATAR,dMSG_OK);
					break;
					
				case	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_NOT_OWNED_GUILD	:
					g_msgBox.cPopup("",dMSG_CAN_NOT_TRAS_TO_GVG_WITH_NOT_OWNED_GUILD,dMSG_OK);
					break;
					
				case	eTRAN_AVATAR_TO_GVG_RESULT_SUCCESS	:
					break;

				case	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_PERIOD	:
					g_msgBox.cPopup("",_ms(dMSG_TRANS_AVATAR_TO_GVG_SERVER_RULE,iYear,iMonth,iDay),dMSG_OK);
					break;

				case	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_WAIT_FOR_RESULT	:
					g_msgBox.cPopup("",dMSG_WAIT_FOR_TRANS_AVATAR_RESULT,dMSG_OK);
					break;

				default	:
					g_msgBox.cPopup("",_ms(dMSG_TRANS_AVATAR_FAILED,iResult),dMSG_OK);
					break;
			}

			break;
		}

		case	eEW_ATTACK_DOPPELLGANGER:
			{
				
				CActor		*lpTarget	=	g_am.getTestedActor(_lpPacket->aiValue[0]);
				
				if(!lpTarget)
					break;

				if(lpTarget->isDeath())
					break;

				lpTarget->m_wAttackDoppelgangerCount			=	_lpPacket->aiValue[1];
				lpTarget->m_iAttackDoppelgangerDamage			=	_lpPacket->aiValue[2];
				lpTarget->m_wActionPerionAttackDoppelganger		=	_lpPacket->aiValue[3];
				lpTarget->m_fAttackDoppelgangerFrame			=	0;
				lpTarget->m_wAttackDoppelgangerAnm				=	dACT_ACTION_1;

				break;
			}
		case	eEW_DRAW_IMAGE:
			{

				int iIndex			= _lpPacket->aiValue[0];
				int iX				= _lpPacket->aiValue[1];
				int iY				= _lpPacket->aiValue[2];
				int iTime			= _lpPacket->aiValue[3];
				int iDrawingMethod	= _lpPacket->aiValue[4];
				int iDistance		= _lpPacket->aiValue[5];

				addKarmaDrawImage(iIndex,iX,iY,iTime,iDrawingMethod,iDistance);
				break;

			}
		case	eEW_OFF_DRAWING_IMAGE:
			{

				int iIndex			= _lpPacket->aiValue[0];
				int iX				= _lpPacket->aiValue[1];
				int iY				= _lpPacket->aiValue[2];

				offKarmaDrawImage(iIndex,iX,iY);
				break;

			}

		case	eEW_OPEN_INTERFACE:
			{

				int		iInterface			= _lpPacket->aiValue[0];
				BOOL	bIsClose			= _lpPacket->aiValue[1];
				BOOL	bIsCanNotControl	= _lpPacket->aiValue[2];
				openInterface(iInterface,bIsClose,bIsCanNotControl);
				break;

			}

		case	eEW_RESET_ITEM_COOLTIME:
			{
				memset(g_hero.m_awItemCoolTime,0,sizeof(g_hero.m_awItemCoolTime));
				break;
			}
		case	eEW_SUMMON_EVENT_MONSTER:
			{
				operateSummonEventMonster();
				break;
			}
		case	eEW_DO_YOU_WANT_REALLY_BEGIN_GUILD_DUNGEN			:
		{
			g_guildManager.m_wPlayLevelForGuildDungen	=	_lpPacket->aiValue[0];

			g_msgBox.cPopup("",dMSG_DO_YOU_WANT_TRY_GUILD_DUNGEN,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_START_GUILD_DUNGEN);

			break;
		}
		case	eEW_INSTANCE_GUILD_BATTLE						:
		{
			int	iMessage	=	_lpPacket->aiValue[0];

			switch(iMessage)
			{
				case	0	:	//	길드전 남은 시간
					g_iRemainGuildBattleTime	=	_lpPacket->aiValue[1];
					break;
			}
			break;
		}

		case	eEW_CANCEL_TO_TRY_DUEL_BY_CHANGE_PARTY_MEMBER	:
		{
			g_duelManager.reset();
			g_msgBox.cPopup("",dMSG_CANCEL_TO_TRY_DUEL_BY_CHANGE_PARTY_MEMBER,dMSG_OK);
			break;
		}

		case	eEW_INCREASE_ALLIGNMENT	:
		{
			int	iIncreaseValue	=	_lpPacket->aiValue[0];
			int	iValue			=	_lpPacket->aiValue[1];

			g_hero.m_sAllignment=	iValue;

			if	(iIncreaseValue	==	0	&&	iValue	==	0)
				CGamePlay::AddSystemMessage(WHITE,dMSG_RESET_ALLIGNMENT_POINT);
			else
				CGamePlay::AddSystemMessage(WHITE,dMSG_INCREASE_ALLIGNMENT_POINT_FORM,iIncreaseValue);

			break;
		}
		case	eEW_DECREASE_ALLIGNMENT	:
		{
			int	iDecreaseValue	=	_lpPacket->aiValue[0];
			int	iValue			=	_lpPacket->aiValue[1];

			g_hero.m_sAllignment=	iValue;

			CGamePlay::AddSystemMessage(WHITE,dMSG_DECREASE_ALLIGNMENT_POINT_FORM,iDecreaseValue);

			break;
		}
		case	eEW_SET_ALLIGNMENT		:
		{
			int	iItem			=	_lpPacket->aiValue[0];
			int	iValue			=	(short)_lpPacket->aiValue[1];

			cItem	*lpItem		=	g_hero.getInventoryItem(iItem);

			if	(lpItem)
				lpItem->decreaseCount(1);

			g_hero.m_sAllignment=	iValue;

			CGamePlay::AddSystemMessage(WHITE,dMSG_SET_ALLIGNMENT_POINT_FORM,iValue);


			break;
		}
		case	eEW_INCREASE_BONUS_GUILD_POINT	:
		{
			int	iValue			=	_lpPacket->aiValue[0];

			g_guild.m_bf6BonusGuildPoint	=	iValue;
			g_guild.m_wGuildPoint			=	_lpPacket->aiValue[1];

			CGamePlay::AddSystemMessage(LTGREEN,dMSG_INCREASE_GUILD_POINT);
			
			break;
		}
		case	eEW_CH5_REVERSE_ITEM_PENELTY	:
		{
			if	(g_hero.m_bf4CH5PeneltyHPBonusCount		!=	g_hero.m_bf4CH5HPBonusCount	||	
				g_hero.m_bf4CH5PeneltyCPBonusCount		!=	g_hero.m_bf4CH5CPBonusCount)
				CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RESET_CH5_HP_CP_BONUS);

			g_hero.m_bf4CH5PeneltyHPBonusCount		=	g_hero.m_bf4CH5HPBonusCount;
			g_hero.m_bf4CH5PeneltyCPBonusCount		=	g_hero.m_bf4CH5CPBonusCount;

			break;
		}
		case	eEW_REBIRTH	:
		{
			g_hero.rebirth(_lpPacket->aiValue[0]);
			break;
		}
		case	eEW_CH5_RECOVER_REVERSE_PENELTY	:
		{
			if	(_lpPacket->aiValue[0]	==	0)
			{
				g_hero.m_bf4CH5PeneltyHPBonusCount	=	0;

				CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RECOVER_CH5_HP_BONUS_PENELTY);
			}
			else
			{
				g_hero.m_bf4CH5PeneltyCPBonusCount	=	0;

				CGamePlay::AddSystemMessage(LTGREEN,dMSG_RECOVER_CH5_CP_BONUS_PENELTY);
			}
			break;
		}

		case	eEW_RECOVER_BONUS_GUILD_POINT_PENELTY	:
		{
			g_guild.m_bf6BonusGuildPoint		=	_lpPacket->aiValue[0];
			g_guild.m_bf6PeneltyBonusGuildPoint=	0;

			CGamePlay::AddSystemMessage(LTGREEN,dMSG_RECOVER_CH5_BONUS_GUILD_POINT_PENELTY);
			break;
		}
		case	eEW_CH5_POINT_BONUS		:	
		{
			if	(_lpPacket->aiValue[0]	==	0)	//	스탯
			{
				g_hero.m_bf4CH5StateBonusCount	=	_lpPacket->aiValue[1];
				g_hero.m_wLevelPoint			=	_lpPacket->aiValue[2];

				CGamePlay::AddSystemMessage(LTGREEN,dMSG_INCREASE_CH5_STATE_BONUS);
			}
			else
			if	(_lpPacket->aiValue[0]	==	1)	//	스킬
			{
				g_hero.m_bf4CH5SkillBonusCount	=	_lpPacket->aiValue[1];
				g_hero.m_dwSkillExperience		=	_lpPacket->aiValue[2];

				CGamePlay::AddSystemMessage(LTGREEN,dMSG_INCREASE_CH5_SKILL_BONUS);
			}
			else
			if	(_lpPacket->aiValue[0]	==	2)	//	체력
			{
				g_hero.m_bf4CH5HPBonusCount	=	_lpPacket->aiValue[1];

				CGamePlay::AddSystemMessage(LTGREEN,dMSG_INCREASE_CH5_HP_BONUS);
			}
			else
			if	(_lpPacket->aiValue[0]	==	3)	//	CP
			{
				g_hero.m_bf4CH5CPBonusCount	=	_lpPacket->aiValue[1];

				CGamePlay::AddSystemMessage(LTGREEN,dMSG_INCREASE_CH5_CP_BONUS);
			}
			break;
		}

		case	eEW_EXTEND_REVERSIBLE_VALIDATE	:
		{
			g_hero.m_wReversalFrameYear			=	_lpPacket->aiValue[0];
			g_hero.m_wReversalFrameMonth		=	_lpPacket->aiValue[1];
			g_hero.m_wReversalFrameDay			=	_lpPacket->aiValue[2];
			g_hero.m_wReversalFrameHour			=	_lpPacket->aiValue[3];
			g_hero.m_wReversalFrameMinute		=	_lpPacket->aiValue[4];

			CGamePlay::AddSystemMessage(LTGREEN,dMSG_EXTENDED_REVERSAL_FRAME_VALIDATE);

			break;
		}

		case	eEW_RESTRAINT_MINIPET		:
		{
			g_hero.iMiniPet()->restraintMiniPet(_lpPacket->aiValue[0]);
			break;
		}

		case	eEW_CHANGE_MINIPET_PLACE	:
		{
			g_hero.iMiniPet()->changeMiniPetPlace(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
			break;
		}
		case	eEW_MINIPET_ACTION			:
			lpActor->miniPetAction(_lpPacket->aiValue[3]);

			if	(_lpPacket->aiValue[1])
			{
				if	(_lpPacket->aiValue[2])
					g_hero.iMiniPet()->playFX(eMPFX_KILL_BOSS_MONSTER,_lpPacket->aiValue[3]);
				else
					g_hero.iMiniPet()->playFX(eMPFX_KILL_MONSTER,_lpPacket->aiValue[3]);
			}

			break;
		case	eEW_READY_TO_FIRE_CHARGE_MINIPET_SKILL	:
			if	(_lpPacket->aiValue[1]	==	0)
				g_hero.m_wIsReadyToFireChargedMiniPetSkill	=	_lpPacket->aiValue[0];
			else
				g_hero.m_wIsReadyToFireChargedMiniPetSkill2	=	_lpPacket->aiValue[0];
			break;
		case	eEW_SWALLOW		:
		{
			g_am.m_iSmoothScrollTime	=	dSYNC_FPS*5;
			break;
		}

		case	eEW_INCREASE_HUNT_EXP				:
		{
			g_hero.m_bonusExp.correctValue(_lpPacket->aiValue[1],_lpPacket->aiValue[0]);
			break;
		}
		case	eEW_INCREASE_ITEM_DROP_CHANCE		:
		{
			g_hero.m_bonusItemDropChance.correctValue(_lpPacket->aiValue[1],_lpPacket->aiValue[0]);
			break;
		}		
		case	eEW_QUESTION_FOR_ADD_RELATED_PLACE_OF_GUILD		:
		{
			char	strText[256];

			int		iField	=	_lpPacket->aiValue[0];
			int		iGold	=	_lpPacket->aiValue[1];
			iGold			<<=	16;
			iGold			+=	_lpPacket->aiValue[2];

			sprintf(strText,dMSG_QUESTION_FOR_ADD_RELATED_PLACE_OF_GUILD_FORM,GetMoneyString(iGold),g_aMapSimpleInfo[iField].m_strName);

			g_guild.m_wRelatedPlaceOfGuildForNewPlace	=	iField;
			g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_ADD_RELATED_PLACE_OF_GUILD);
			
			break;
		}
		case	eEW_CHANGE_OPEN_GUILD_HALL_STATUS				:
		{
			g_guild.m_bf1IsPermitVisitor	=	_lpPacket->aiValue[0];

			if	(g_guild.m_bf1IsPermitVisitor)
				AddSystemMessage(LTYELLOW,dMSG_PERMIT_GUILD_HALL_VISTORS);
			else
				AddSystemMessage(LTYELLOW,dMSG_DENY_GUILD_HALL_VISTORS);
			break;
		}
		case	eEW_END_GUILD_POINT_BATTLE						:
		{
			int	iMethod		=	_lpPacket->aiValue[0];

			int	iTotalPoint	=	_lpPacket->aiValue[1];
			iTotalPoint		<<=	16;
			iTotalPoint		+=	_lpPacket->aiValue[2];

			int	iPeneltyPoint=	_lpPacket->aiValue[3];
			iPeneltyPoint	<<=	16;
			iPeneltyPoint	+=	_lpPacket->aiValue[4];

			int		iHonorPoint	=	max(iTotalPoint-iPeneltyPoint,0);

			char	strText[256];

			if	(iMethod	==	0)
				sprintf(strText,dMSG_COMPLETE_TO_GUILD_POINT_BATTLE_FORM,iTotalPoint,iPeneltyPoint,iHonorPoint);
			else
				sprintf(strText,dMSG_FAILED_TO_GUILD_POINT_BATTLE_FORM,iTotalPoint,iPeneltyPoint,iHonorPoint);

			g_msgBox.cPopup("",strText,dMSG_OK);
			g_msgBox.setButtonId(eMBI_RETURN_TO_THE_GUILD_HALL_BY_END_GUILD_POINT_BATTLE);

			break;
		}
		case	eEW_CHANGE_REMAIN_GUILD_STATEGY_POINT_COUNT		:
		{
			s_iRemainGuildStategyPoint	=	_lpPacket->aiValue[0];

			if	(s_iRemainGuildStategyPoint	!=	c_iGuildStategyPointCount)
			{
				if	(g_hero.m_wTeam	==	0)
				{
					AddSystemMessage(LTYELLOW,dMSG_DESTROYED_GUILD_STATEGY_POINT_BY_ENEMY);
					AddSystemMessage(LTYELLOW,dMSG_REDUCE_GUILD_CREST_HP);
					AddSystemMessage(LTYELLOW,dMSG_REMOVE_HP_BONUS_BY_GUILD_STATEGY_POINT);
				}
				else
				{
					AddSystemMessage(LTYELLOW,dMSG_DESTROY_GUILD_STATEGY_POINT);
					AddSystemMessage(LTYELLOW,dMSG_REDUCE_GUILD_CREST_HP);
					AddSystemMessage(LTYELLOW,dMSG_INCREASE_ATTACK_DEFENSE_POWER);
				}
			}
			break;
		}
		case	eEW_CAN_NOT_OPEN_GUILD_SHOP_BY_ITEM_ZERO		:
		{
			AddSystemMessage(LTYELLOW,dMSG_EW_CAN_NOT_OPEN_GUILD_SHOP_BY_ITEM_ZERO);
			break;
		}

		case	eEW_FEED_ITEM_TO_GUILD_PET						:
		{
			DWORD	dwExp	=	_lpPacket->aiValue[3];
			dwExp			<<=	16;
			dwExp			+=	_lpPacket->aiValue[4];
			
			g_guild.feedItemToGuildPet(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2],dwExp,_lpPacket->aiValue[5]);

			break;
		}

		case	eEW_CAN_NOT_FEED_ITEM_BY_OVER_MAX_GUILD_PET_EXP	:
			break;

		case	eEW_OPEN_FEED_GUILD_GUARDIAN_AND_PET_WINDOW		:
			g_gwFeedGuildPet.open();
			break;

		case	eEW_CAN_FIND_GUILD_GUARDIAN_IN_SIGHT			:
		{
			AddSystemMessage(LTYELLOW,dMSG_CAN_FIND_GUILD_GUARDIAN_IN_SIGHT);
			break;
		}
		case	eEW_REMOVE_NEAREST_GUILD_GUARDIAN	:
		{
			cSTRING	string;

			string.Add(dMSG_REMOVE_NEAREST_GUILD_GUARDIAN);
			string.Add(dMSG_REMAIN_SUMMON_ABLE_GUILD_GUARDIAN_COUNT_FORM,_lpPacket->aiValue[1]-_lpPacket->aiValue[0]);

			AddSystemMessage(WHITE,string.String);
			break;
		}
		case	eEW_SUMMON_GUILD_GUARDIAN:
		{
			int	iRemainCount	=	_lpPacket->aiValue[1]-_lpPacket->aiValue[0];
			cSTRING	string;

			string.Add(dMSG_SUMMON_GUILD_GUARDIAN_FORM,g_lpstrGuildGuardianName[_lpPacket->aiValue[2]]);

			if	(iRemainCount	==	0)
				string.Add(dMSG_GUILD_GUARDIAN_COUNT_FULL_IN_FIELD);
			else
				string.Add(dMSG_REMAIN_SUMMON_ABLE_GUILD_GUARDIAN_COUNT_FORM,iRemainCount);

			AddSystemMessage(WHITE,string.String);
			break;
		}

		case	eEW_RENT_GUILD_PET_COUNT_OVER		:
			AddSystemMessage(LTYELLOW,dMSG_EW_RENT_GUILD_PET_COUNT_OVER_FORM,g_lpstrGuildPetName[_lpPacket->aiValue[1]],_lpPacket->aiValue[0]);
			break;

		case	eEW_SGG_IS_USE_ABLE_IN_GUILD_BATTLE	:
			AddSystemMessage(LTYELLOW,dMSG_EW_SGG_IS_USE_ABLE_IN_GUILD_BATTLE);
			break;
		case	eEW_SGG_IS_USE_ABLE_DEFENSE_GUILD	:
			AddSystemMessage(LTYELLOW,dMSG_EW_SGG_IS_USE_ABLE_DEFENSE_GUILD);
			break;

		case	eEW_TOO_MANY_SUMMON_GUILD_GUARDIAN						:
			AddSystemMessage(LTYELLOW,dMSG_EW_TOO_MANY_SUMMON_GUILD_GUARDIAN);
			break;

		case	eEW_EXIST_GUILD_GUARDIAN_OR_CREST_IN_TO_CLOSE_RANGE		:
			AddSystemMessage(LTYELLOW,dMSG_EW_EXIST_GUILD_GUARDIAN_OR_CREST_IN_TO_CLOSE_RANGE);
			break;

		case	eEW_RECORD_GUILD_HONOR_POINT							:
		{
			int		iHonorPoint		=	_lpPacket->aiValue[0];
			iHonorPoint				<<=	16;
			iHonorPoint				+=	_lpPacket->aiValue[1];
		
			break;
		}

		case	eEW_DO_YOU_WANT_REALLY_BEGIN_GUILD_POINT_BATTLE			:
		{
			g_msgBox.cPopup("",dMSG_DO_YOU_WANT_TRY_GUILD_POINT_BATTLE,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_TRY_GUILD_POINT_BATTLE);

			break;
		}

		case	eEW_LOW_GOLD_FOR_ADD_VILLAGE_IN_GUILD_RELATED_PLACE_LIST	:
		{
			int		iGold	=	_lpPacket->aiValue[0];
			iGold			<<=	16;
			iGold			+=	_lpPacket->aiValue[1];
			AddSystemMessage(WHITE,dMSG_LOW_GOLD_FOR_ADD_VILLAGE_IN_GUILD_RELATED_PLACE_LIST_FORM,GetMoneyString(iGold));
			break;
		}
		case	eEW_ADD_RELATED_PLACE_OF_GUILD		:
		{
			int		iGold	=	_lpPacket->aiValue[1];
			iGold			<<=	16;
			iGold			+=	_lpPacket->aiValue[2];
			g_guild.addRelatedPlace(_lpPacket->aiValue[0],iGold);

			if	(g_hero.isGuildMaster())
			{
				g_hero.m_iGold	-=	iGold;
				AddSystemMessage(WHITE,dMSG_ADD_RELATED_PLACE_OF_GUILD_FROM,g_aMapSimpleInfo[_lpPacket->aiValue[0]].m_strName);
			}
			break;
		}
		case	eEW_REMOVE_RELATED_PLACE_OF_GUILD	:
		{
			g_guild.removeRelatedPlace(_lpPacket->aiValue[0]);

			if	(g_hero.isGuildMaster())
				AddSystemMessage(WHITE,dMSG_REMOVE_RELATED_PLACE_OF_GUILD_FROM,g_aMapSimpleInfo[_lpPacket->aiValue[0]].m_strName);
			break;
		}
		case	eEW_CHANGE_TODAY_WITHDRAW_GUILD_INVENTORY_ITEM_COUNT	:
		{
			g_hero.m_abTodayWithdrawItemCount[_lpPacket->aiValue[0]]	=	_lpPacket->aiValue[1];
			break;
		}
		case	eEW_CHANGE_TODAY_WITHDRAW_GUILD_INVENTORY_GOLD		:
		{
			int		iGold	=	_lpPacket->aiValue[0];
			iGold			<<=	16;
			iGold			+=	_lpPacket->aiValue[1];

			g_hero.m_iTodayWithdrawGoldInGuild	=	iGold;
			break;
		}
		case	eEW_CHANGE_GUILD_INVENTORY_PERMIT_WITHDRAW_ITEM_COUNT:	//	인덱스:직위:개수
			g_guild.m_aWithdrawItemCountPerDay[_lpPacket->aiValue[0]][_lpPacket->aiValue[1]]	=	_lpPacket->aiValue[2];

			if	(g_hero.isGuildMaster())
				AddSystemMessage(WHITE,dMSG_COMPLETE_REQUEST_WORK);
			break;

		case	eEW_CHANGE_GUILD_INVENTORY_PERMIT_WITHDRAW_GOLD:		//	직위:골드
		{
			int	iGold1	=	_lpPacket->aiValue[1];
			int	iGold2	=	_lpPacket->aiValue[2];
			int	iGold	=	(iGold1<<16)+iGold2;

			g_guild.m_aiWithdrawGoldLimitPerDay[_lpPacket->aiValue[0]]	=	iGold;

			if	(g_hero.isGuildMaster())
				AddSystemMessage(WHITE,dMSG_COMPLETE_REQUEST_WORK);
			break;
		}

		case	eEW_CHANGE_GUILD_INVENTORY_STORAGE_SIZE		:		//	인덱스:사이즈
			g_guild.m_abInventorySize[0]	=	_lpPacket->aiValue[0];
			g_guild.m_abInventorySize[1]	=	_lpPacket->aiValue[1];
			g_guild.m_abInventorySize[2]	=	_lpPacket->aiValue[2];
			g_guild.m_abInventorySize[3]	=	_lpPacket->aiValue[3];
			AddSystemMessage(WHITE,dMSG_CHANGED_GUILD_INVENTORY_STORAGE_SIZE);
			break;

		case	eEW_CHANGE_GUILD_HALL_LEVEL						:
			g_guild.m_wHallLevel	=	_lpPacket->aiValue[0];
			break;

		case	eEW_FIRE_GIGA_LIGHTNING							:
		{
			int		iSkill			=	_lpPacket->aiValue[0];
			int		iCaster			=	_lpPacket->aiValue[1];

			CActiveSkill	*lpAS	=	g_sm.getActiveSkill(iSkill,iCaster);

			if	(!lpAS)
				break;

			((cAS_GigaLightning*)lpAS)->fireSkill();

			break;
		}
		
		case	eEW_BEGIN_SERCH_RANDOM_PLACE_BY_GIGA_LIGHTNING	:
		{
			int		iSkill			=	_lpPacket->aiValue[0];
			int		iCaster			=	_lpPacket->aiValue[1];
			int		iPattern		=	_lpPacket->aiValue[2];
			int		iStep			=	_lpPacket->aiValue[3];
			int		iRange			=	_lpPacket->aiValue[4];

			CActiveSkill	*lpAS	=	g_sm.getActiveSkill(iSkill,iCaster);

			if	(!lpAS)
				break;

			((cAS_GigaLightning*)lpAS)->serchRandomPlace(iPattern,iStep,iRange);

			break;
		}
			
		case	eEW_USE_ACTION	:
		{
			int	iX			=	_lpPacket->aiValue[1];
			int	iY			=	_lpPacket->aiValue[2];
			int	iAction		=	_lpPacket->aiValue[3];
			int	iDirect		=	_lpPacket->aiValue[4];
			int	iFPS		=	_lpPacket->aiValue[5];

			lpActor->stop(iX,iY);
			lpActor->action(iX,iY,iAction);
			lpActor->setDirect(iDirect);
			lpActor->setExclusiveAction(TRUE);

			lpActor->m_iFPS	=	iFPS;
			break;
		}

		case	eEW_DISCONNECT_BY_HACKSHIELD				:
		{
			cMSG::Put("Red Stone",dMSG_FOUND_TRY_HACKING);

			cMAIN::Exit(TRUE);
			break;
		}
		case	eEW_SLEEP_TARGET							:
		{
			lpActor			=	g_am.getTestedActor(_lpPacket->aiValue[0],FALSE);

			if	(!lpActor)
				break;

			g_esm.play("Sleepy Hole2.wav");

			g_hero.m_rectSight.isIn(_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
			break;
		}
		case	eEW_CHANGE_DEFENSIVE_STATUS					:
		{
			g_hero.m_defensePower.m_sCorrectValue					=	_lpPacket->aiValue[0];
			g_hero.m_defensePower.m_sCorrectPercentageValue		=	_lpPacket->aiValue[1];
			g_hero.m_defensePower.m_wTimeForCorrectValue			=	_lpPacket->aiValue[2];
			g_hero.m_defensePower.m_wTimeForCorrectPercentageValue	=	_lpPacket->aiValue[3];

			break;
		}
		case	eEW_SET_MARIONETTE_STATUS					:
		{
			lpActor->m_bf1IsMarionette	=	_lpPacket->aiValue[1];
			break;
		}

		case	eEW_TARGET_ALREADY_OWNED_PRINCSS_WEAPON	:
		{
			AddSystemMessage(LTYELLOW,dMSG_TARGET_ALREADY_OWNED_PRINCSS_WEAPON);
			break;
		}

		case	eEW_CAN_NOT_RECALL_ACTOR_IN_JAIL		:
		{
			WarningMessage(dMSG_TARGET_IN_JAIL);
			
			break;
		}
		case	eEW_INTERRUPTING_ARMOR				:
		{
			g_hero.setInterruptingArmor(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);

			break;
		}
		case	eEW_DECLINE_RECOVER_HP_EFFICIENT	:
		{
			g_hero.declineRecoverHPEfficient(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
			break;
		}

		case	eEW_INLINE_STRENGTH			:
		{
			g_hero.m_strength.m_sCorrectValue	=	(short)_lpPacket->aiValue[0];
			g_hero.m_strength.m_wTime			=	_lpPacket->aiValue[1];
			break;
		}

		case	eEW_INLINE_AGILITY			:
		{
			g_hero.m_agility.m_sCorrectValue	=	(short)_lpPacket->aiValue[0];
			g_hero.m_agility.m_wTime			=	_lpPacket->aiValue[1];
			break;
		}

		case	eEW_INLINE_CONSTITUTION		:
		{
			g_hero.m_constitution.m_sCorrectValue	=	(short)_lpPacket->aiValue[0];
			g_hero.m_constitution.m_wTime			=	_lpPacket->aiValue[1];
			break;
		}

		case	eEW_INLINE_WISDOM			:
		{
			g_hero.m_wisdom.m_sCorrectValue	=	(short)_lpPacket->aiValue[0];
			g_hero.m_wisdom.m_wTime			=	_lpPacket->aiValue[1];
			break;
		}

		case	eEW_INLINE_INTELLIGENCE		:
		{
			g_hero.m_intelligence.m_sCorrectValue	=	(short)_lpPacket->aiValue[0];
			g_hero.m_intelligence.m_wTime			=	_lpPacket->aiValue[1];
			break;
		}

		case	eEW_INLINE_LUCK				:
		{
			g_hero.m_luck.m_sCorrectValue	=	(short)_lpPacket->aiValue[0];
			g_hero.m_luck.m_wTime			=	_lpPacket->aiValue[1];
			break;
		}

		case	eEW_FAILED_TO_USE_POS_BY_ALREADY_EXIST_TARGET	:
		{
			char	strText[256];

			sprintf(strText,dMSG_FAILED_TO_USE_SKILL_BY_LIMIT_SAME_TIME_CASTING_TARGET_FORM,_lpPacket->aiValue[0]);

			WarningMessage(strText);

			break;
		}

		case	eEW_MULTIPLE_DAMAGE					:
		{
			CHitInfo	hitInfo;

			hitInfo.reset(TRUE);
			hitInfo.setHit();
			hitInfo.m_dwPhysicalDamage	=	_lpPacket->aiValue[1]*100;
			hitInfo.m_dwMagicDamage		=	_lpPacket->aiValue[2]*100;

			lpActor->hitMultipleDamage(&hitInfo,_lpPacket->aiValue[3],_lpPacket->aiValue[4],_lpPacket->aiValue[5]);

			break;
		}

		case	eEW_ANIMATE_PARTNER_TIME			:
			g_hero.m_wAnimatePartnerTime	=	_lpPacket->aiValue[0];
			break;

		case	eEW_HWABYUNG_TIME					:
			g_hero.m_wHwabyungTime			=	_lpPacket->aiValue[0];
			break;

		case	eEW_PLOT_OF_SHADOW_TIME				:
			g_hero.m_wPlotOfShadowTime		=	_lpPacket->aiValue[0];
			break;

		case	eEW_LUCKY_SAVE_THROW_ITEM			:
			g_lpHero->addHitEffect(0,-g_lpHero->getBody()->m_sprite.m_iHeight+10	,g_im.m_wCriticalHitEffect,0,6,0);
			break;

		case	eEW_DRAIN_BLOOD						:
		{
			CActor	*lpTarget	=	g_am.getTestedActor(_lpPacket->aiValue[1]);
			int		iBlood		=	_lpPacket->aiValue[2]*100;
			int		iSkill		=	_lpPacket->aiValue[3];
			int		iRemainHP	=	_lpPacket->aiValue[4];

			cAbility	ability;

			ability.set(iSkill,1);

			CHitInfo	hitInfo;

			hitInfo.reset(TRUE);
			hitInfo.setHit();
			hitInfo.m_dwMagicDamage	=	iBlood;
			
			LONGLONG	llMaxHp	=	lpActor->m_iMaxHP;
			lpActor->m_iHP		=	(int)(llMaxHp * iRemainHP/10000);
			lpActor->m_iHP		=	max(lpActor->m_iHP,1);

			lpActor->hit(lpTarget,&ability,&hitInfo);
			lpTarget->addHealEffectAndInfo(g_im.m_wRegenHPImage,iBlood/100,eHEOP_FOOT);
			lpTarget->increaseHP(iBlood);

			break;
		}

		case	eEW_SET_IMPULSE_TIME				:
			g_hero.setImpulseTime(_lpPacket->aiValue[0]);
			break;

		case	eEW_SHARE_DAMAGE_BY_DAMAGE_POOL		:
			g_hero.forcedHealHP(_lpPacket->aiValue[0]*100);
			break;

		case	eEW_CAN_NOT_CREATE_MORE_ILLUSION	:
			WarningMessage(dMSG_CAN_NOT_CREATE_MORE_ILLUSION);
			break;

		case	eEW_SET_REVERSAL_POWER_TIME			:
		{
			g_hero.setReversalPowerTime(_lpPacket->aiValue[0]);
			g_hero.buildPower();
			break;
		}

		case	eEW_CAN_NOT_TRANS_TARGET_TO_FROG	:
		{
			if	(_lpPacket->aiValue[0]	==	0xffff)
			{
				AddSystemMessage(WHITE,dMSG_EW_CAN_NOT_TRANS_TARGET_TO_FROG_BY_BOSS);
			}
			else
			if	(_lpPacket->aiValue[0]	==	0xfffe)
			{
				AddSystemMessage(WHITE,dMSG_EW_CAN_NOT_TRANS_TARGET_TO_FROG_BY_HP);
			}
			else
			{
				AddSystemMessage(WHITE,dMSG_EW_CAN_NOT_TRANS_TARGET_TO_FROG_FORM,_lpPacket->aiValue[0]);
			}

			break;
		}
		case	eEW_LIMIT_SHUT_IN_MAGIC_BOX_ACTOR_COUNT	:
		{
			AddSystemMessage(WHITE,dMSG_EW_LIMIT_SHUT_IN_MAGIC_BOX_ACTOR_COUNT);
			break;
		}

		case	eEW_FINISH_LIGHTNING_WINDER			:
		{
			break;
		}
		case	eEW_SHUT_IN_MAGIC_BOX				:
		{
			lpActor->shutInMagicBox();
			break;
		}

		case	eEW_CRUSH_MAGIC_BOX					:
		{
			lpActor->crushMagicBox(_lpPacket->aiValue[1]);
			break;
		}

		case	eEW_JUDGE_ITEM						:
		{
			int	iItem		=	_lpPacket->aiValue[0];
			int	iPrice		=	(int)_lpPacket->aiValue[1]*0xffff+(int)_lpPacket->aiValue[2];

			g_hero.judgeItem(iItem,iPrice);
			break;
		}
		case	eEW_MERGE_ITEM						:
		{
			int	iItem	=	_lpPacket->aiValue[0];
			int	iPrice	=	(int)_lpPacket->aiValue[1]*0xffff+(int)_lpPacket->aiValue[2];

			g_hero.mergeItem(iItem,iPrice);
			break;
		}
		case	eEW_OPEN_JUDGE_OF_ITEM_WINDOW		:
		{
			g_gwJudgeToItem.open(_lpPacket->aiValue[0]);
			break;
		}
		case	eEW_OPEN_MERGE_ITEM_PIECE_WINDOW	:
		{
			g_gwMergeItem.open(_lpPacket->aiValue[0]);
			break;
		}

		case	eEW_REMOVE_ITEM_PREFIX				:
		{
			g_gwRemovePrefix.open(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
			break;
		}

		case	eEW_QUEST_COMPLETE					:
		{
			CQuestItem	*lpQuest	=	CQuest::GetQuest(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);

			if	(!lpQuest)
				break;

			if(!_lpPacket->aiValue[2])
				AddSystemMessage(WHITE,dMSG_QUEST_COMPLETE_FORM,lpQuest->m_strName);

			g_hero.SaveCompleteQuest(_lpPacket->aiValue[0], _lpPacket->aiValue[1]);

			break;
		}

		case	eEW_BITE_HANGER						:
		{
			lpActor->m_wBiter		=	_lpPacket->aiValue[1];

			CActor	*lpBiter		=	g_am.getTestedActor(_lpPacket->aiValue[1]);

			if	(!lpBiter)
				break;

			lpBiter->m_wBiteTarget	=	lpActor->m_wSerial;
			break;
		}

		case	eEW_THE_OTHER_SELF_COUNT			:
		{
			lpActor->m_wTheOtherSelfCount						=	_lpPacket->aiValue[1];

			if	(lpActor->m_wTheOtherSelfCount)
				lpActor->m_enchantedImage.m_bf1IsExistTheOtherSelf	=	TRUE;
			else
				lpActor->m_enchantedImage.m_bf1IsExistTheOtherSelf	=	FALSE;
			break;
		}

		case	eEW_DAMAGED_BY_SHAKLE				:
		{
			lpActor->m_wShakleSkill							=	_lpPacket->aiValue[1];
			lpActor->m_enchantedImage.m_bf1IsBindByShakle	=	TRUE;

			g_esm.playActionSound(&g_aSkill[_lpPacket->aiValue[1]]);

			lpActor->changeShakleAnm(2);
			break;
		}

		case	eEW_SHAKLE_SKILL					:
		{
			lpActor->m_wShakleSkill							=	_lpPacket->aiValue[1];
			lpActor->m_enchantedImage.m_bf1IsBindByShakle	=	TRUE;
			lpActor->changeShakleAnm(1);
			break;
		}

		case	eEW_MAKE_SHAKLE						:
		{
			lpActor->m_wShakleSkill							=	_lpPacket->aiValue[1];
			lpActor->m_enchantedImage.m_bf1IsBindByShakle	=	TRUE;

			g_esm.playCastingSound(&g_aSkill[_lpPacket->aiValue[1]]);

			lpActor->changeShakleAnm(0);
			break;
		}

		case	eEW_MAKE_SKILL_ZONE					:
		{
			cAbility	ability;

			ability.set(_lpPacket->aiValue[1],10);

			g_esm.playCastingSound(ability.getSkill());
			
			g_sm.castGasTypeSkill(lpActor,_lpPacket->aiValue[2],_lpPacket->aiValue[3],&ability);
			break;
		}
		case	eEW_TORTURE_DAMAGE					:
		{
			lpActor->addHitEffectAndInfo(g_im.m_wHitTortureDamageImage,0,_lpPacket->aiValue[1]);

			break;
		}
		case	eEW_SUCCESS_AID_ATTACK				:
		{
			CSkill	*lpSkill	=	&g_aSkill[_lpPacket->aiValue[1]];

			if	(lpSkill->m_wAidAttackImage	!=	0xffff)
			{
				lpActor->addEffect(lpSkill->m_wAidAttackImageOutputPart,lpSkill->m_wAidAttackImage);

				if	(lpSkill->m_bf1PlayCastSoundByEnchantAidSkill)
					g_esm.playCastingSound(lpSkill);
			}
			break;
		}

		case	eEW_STEAL_GOLD						:
		{
			g_hero.m_iGold		-=	_lpPacket->aiValue[0];
			AddSystemMessage(LTYELLOW,dMSG_STEAL_GOLD_FORM,_lpPacket->aiValue[0]);
			break;
		}

		case	eEW_ZOMBIE_BOMB	:
		{
			g_hero.m_wNotificationOfDeathCounter	=	_lpPacket->aiValue[0];
			break;
		}
			
		case	eEW_NOTIFICATION_OF_DEATH_COUNTER	:
		{
			g_hero.m_wNotificationOfDeathCounter	=	_lpPacket->aiValue[0];
			break;
		}

		case	eEW_CHANGE_REBIRTH_COUNT					:
		{
			g_hero.m_bf4RebirthCount	=	_lpPacket->aiValue[0];

			break;
		}

		case	eEW_CHANGE_USE_SEAL_SPHERE_OF_DAWN_COUNT	:
		{
			g_hero.m_bf4UseSealSphereOfDawnCount	=	_lpPacket->aiValue[0];

			break;
		}
		
		case	eEW_LEVEL_DRAIN						:
		{
			if	(lpActor->isHero())
			{
				g_hero.m_level.m_sCorrectValue	=	(short)_lpPacket->aiValue[1];
				g_hero.m_level.m_wTime			=	_lpPacket->aiValue[2];

				if	(_lpPacket->aiValue[3])
				{
					if	(g_hero.m_level.m_sCorrectValue	==	0)
					{
						g_hero.m_strength.cure();
						g_hero.m_constitution.cure();
						g_hero.m_agility.cure();
						g_hero.m_charisma.cure();
						g_hero.m_intelligence.cure();
					}
					else
					{
						g_hero.m_strength.correctValue((short)_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
						g_hero.m_constitution.correctValue((short)_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
						g_hero.m_agility.correctValue((short)_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
						g_hero.m_charisma.correctValue((short)_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
						g_hero.m_intelligence.correctValue((short)_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
					}
				}

				g_hero.buildPower();
			}
			else
			{
				lpActor->m_wLevel	=	_lpPacket->aiValue[4];
			}

			if	((short)_lpPacket->aiValue[1]	<	0	&&	_lpPacket->aiValue[5]	==	0)
				lpActor->addEffect(eHEOP_SHOULDER,g_im.m_wLevelDrain);
			break;
		}

		case	eEW_UNDER_EFFECT_TWO_MOUTH_ONE_SOUND_EVENT	:
		{
			g_msgBox.cPopup("",dMSG_UNDER_EFFECT_TWO_MOUTH_ONE_SOUND_EVENT,dMSG_OK);
			break;
		}

		case	eEW_UNDER_EFFECT_GOLD_RUSH_EVENT	:
		{
			g_msgBox.cPopup("",dMSG_UNDER_EFFECT_GOLD_RUSH_EVENT,dMSG_OK);
			break;
		}


		case	eEW_CONFIRM_USE_SOUND_OF_LEADERS_BELL	:
		{
			g_hero.m_bf1IsUseAbleSoundOfLeadersBellStatus	=	TRUE;
			g_hero.askUseSoundOfLeadersBell(s_iReadyToUseItem);
			break;
		}

		case	eEW_REQUIRE_MORE_TIME_FOR_USE_SOUND_OF_LEADERS_BELL	:
		{
			int	iCoolTime	=	_lpPacket->aiValue[0];
			int	iUpkeepTime	=	_lpPacket->aiValue[1];

			if	(iUpkeepTime	==	0)
				g_msgBox.cPopup("",_ms(dMSG_REQUIRE_MORE_TIME_FOR_USE_SOUND_OF_LEADERS_BELL_FORM,iCoolTime/60,iCoolTime%60),dMSG_OK);
			else
				g_msgBox.cPopup("",_ms(dMSG_ALREADY_UNDER_EFFECT_SOUND_OF_LEADERS_BELL_FORM,iCoolTime/60,iCoolTime%60),dMSG_OK);
			break;
		}

		case	eEW_SHAKE_SCREEN					:
			g_shaker.add(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[3],_lpPacket->aiValue[4],_lpPacket->aiValue[2]);
			break;

		case	eEW_FAILED_TO_CREATE_SECRET_DUNGEON_BY_LIMIT_LEVEL	:
			g_msgBox.cPopup("",_ms(dMSG_FAILED_TO_CREATE_SECRET_DUNGEON_BY_LIMIT_LEVEL_FORM,_lpPacket->aiValue[0],_lpPacket->aiValue[1]),dMSG_OK);
			break;
		case	eEW_FAILED_TO_ENTER_SECRET_DUNGEON_BY_LIMIT_LEVEL	:
			g_msgBox.cPopup("",_ms(dMSG_FAILED_TO_ENTER_THE_SECRET_DUNGEON_BY_LIMIT_LEVEL_FORM,_lpPacket->aiValue[0],_lpPacket->aiValue[1]),dMSG_OK);
			break;

		case	eEW_PERMISSION_LOGOUT	:
			g_countDown.reset();
			g_hero.m_GGG.ResetGGG();
			AskExitGame(g_hero.m_wLogoutWork,TRUE,TRUE);
			break;

		case	eEW_TRY_LOGOUT:
			if (_lpPacket->aiValue[0]	==	_lpPacket->aiValue[1])
			{
				if(_lpPacket->aiValue[2])
					WarningMessage(_ms(dMSG_GUILD_DUNGEON_LOGOUT_FORM,_lpPacket->aiValue[0]));
				else
					WarningMessage(_ms(dMSG_SECRET_LOGOUT_FORM,_lpPacket->aiValue[0]));
			}

			g_hero.setRestraintTimeByLogout(_lpPacket->aiValue[0]);
			g_countDown.begin(_lpPacket->aiValue[0],(void *)&s_textBold24M,cDRAW::HEIGHT/12);
			break;

		case	eEW_CAN_NOT_MOVE_FIELD_DURING_BATTLE			:
			WarningMessage(_ms(dMSG_CAN_NOT_MOVE_FIELD_DURING_BATTLE_FORM,_lpPacket->aiValue[0]));
			break;
		
		case	eEW_CAN_NOT_EXIT_GAME_DURING_BATTLE:
			WarningMessage(_ms(dMSG_CAN_NOT_EXIT_GAME_DURING_BATTLE_FORM,_lpPacket->aiValue[0]));
			break;

		case	eEW_CANCEL_LOGOUT_TIMER_BY_ACTION:
			WarningMessage(dMSG_CANCEL_LOGOUT_TIMER_BY_ACTION);
			g_hero.setRestraintTimeByLogout(0xffff);
			g_countDown.reset();
			break;

		case	eEW_END_OF_REMAIN_SECOND_FOR_USE_GATE_GLOVE		:
			g_hero.m_wRemainSecondForUseGateGlove	=	0;
			break;

		case	eEW_USE_GATE_GLOVE_REMAIN_SECOND				:
			g_hero.m_wRemainSecondForUseGateGlove	=	_lpPacket->aiValue[0];
			AddSystemMessage(WHITE,dMSG_USE_GATE_GLOVE_FAILED_BY_REMAIN_SECOND_FORM,_lpPacket->aiValue[0]);
			break;

		case	eEW_WITHDRAW_CARROT_AGREEMENT_FAILED			:
			g_gwCarrotShop.m_wIsWaitResult		=	FALSE;

			g_msgBox.cPopup("",dMSG_GAME_CARROT_AGREEMENT_FAILED,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_MOVE_TO_USE_GAME_CARROT_AGREEMENT_PAGE);
			break;

		case	eEW_OPEN_CARROT_SHOP							:
		{
			int		iCarrot	=	_lpPacket->aiValue[0];
			iCarrot			<<=	16;
			iCarrot			+=	_lpPacket->aiValue[1];
			
			g_gwCarrotShop.m_iRemainCarrotCount	=	iCarrot;
			g_gwCarrotShop.m_wWaitOpenResultTime	=	0;
			g_gwCarrotShop.open();
			break;
		}
		case	eEW_FAILED_TO_PUT_BANNER:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_FAILED_TO_PUT_BANNER);
			break;
		}
		case	eEW_FAILED_TO_BANNER_FULL:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_FAILED_TO_BANNER_FULL);
			break;
		}
		case	eEW_SUCCESS_TO_PUT_BANNER:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_SUCCESS_TO_PUT_BANNER);
			cItem	*lpItem	=	g_hero.getItem(_lpPacket->aiValue[0]);

			if	(lpItem	)
				lpItem->decreaseCount(1);

			g_gwPutBannerWnd.close();
			break;
		}
		case	eEW_REMOVE_BANNER:
		{
			g_bannerManager.remove(_lpPacket->aiValue[0]);
			break;
		}
		case	eEW_FAILED_TO_PUT_BANNER_BY_TO_CLOSE:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_FAILED_TO_PUT_BANNER_BY_TO_CLOSE);
			break;
		}
		case	eEW_FAILED_TO_PUT_BANNER_BY_INCORRECT_FIELD:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_FAILED_TO_PUT_BANNER_BY_INCORRECT_FIELD);
			break;
		}

		case	eEW_DESTROY_ITEM_BY_MOVE_FIELD				:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_DESTROY_ITEM_BY_MOVE_FIELD);
			g_hero.arrangeInventory(TRUE);
			break;
		}

		case	eEW_TREASURE_MAP_DESTROYED_WHEN_USE			:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TREASURE_MAP_DESTROYED_WHEN_USE);
			break;
		}

		case	eEW_FAILED_TO_USE_TREASURE_MAP				:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_VALID_TREASURE_BOX);
			break;
		}

		case	eEW_FOUND_GOLD_IN_TREASURE_BOX				:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_FOUND_GOLD_IN_TREASURE_BOX);
			break;
		}

		case	eEW_OPEN_TREASURE_BOX						:
		{
			g_arcaManager.removeArca(0x7fff);
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_FIND_RED_STONE_BUT_FEEL_RED_STONES_AIR);
			break;
		}
		case	eEW_BOOST_EXP_SECOND_BY_TREASURE_MAP_EVENT	:
		{
			g_hero.m_wBoostExpSecondByTreasureMapEvent	=	_lpPacket->aiValue[0];
			CGamePlay::WarningMessage(dMSG_UNSEAL_RED_STONE_AIR);
			break;
		}
		case	eEW_BOOST_GOLD_SECOND_BY_TREASURE_MAP_EVENT	:
		{
			g_hero.m_wBoostGoldSecondByTreasureMapEvent	=	_lpPacket->aiValue[0];
			CGamePlay::WarningMessage(dMSG_UNSEAL_RED_STONE_AIR);
			break;
		}
		case	eEW_BOOST_EXP_BY_TREASER_MAP_EVENT_TIME_OVER:
		{
			g_iBoostExpSecondByTreasureMapEventInServer		=	0;
			break;
		}

		case	eEW_CHANGE_EXTRA_BAG_SIZE	:
		{
			g_hero.m_bf6ExtraInventorySize	=	_lpPacket->aiValue[0];
			CGamePlay::AddSystemMessage(LTGREEN,dMSG_EXAPND_INVENTORY);

			break;
		}

		case	eEW_BOOST_GOLD_BY_TREASER_MAP_EVENT_TIME_OVER:
		{
			g_iBoostGoldSecondByTreasureMapEventInServer	=	0;
			break;
		}
		case	eEW_DESTROY_TREASURE_MAP_BY_MOVE_FIELD		:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TREASURE_MAP_DESTROYED);
			break;
		}

		case	eEW_BERSERK_COUNT_OVER						:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_BERSERK_COUNT_OVER);
			break;
		}

		case	eEW_ALREADY_BERSERK_STATUS					:
		{
			g_hero.m_wBoostTime	=	_lpPacket->aiValue[0];
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_ALREADY_BERSERK_STATUS_FORM,_lpPacket->aiValue[0]);
			break;
		}

		case	eEW_TIME_OVER_BERSERK_TIME					:
		{
			g_hero.m_wBoostTime			=	0;
			g_hero.m_wBoostPower		=	100;
			g_hero.buildPower();
			break;
		}

		case	eEW_USE_BERSERK_TITLE						:
		{
			g_hero.m_bf3UseBoostCount	=	_lpPacket->aiValue[0];
			g_hero.m_wBoostTime			=	_lpPacket->aiValue[1];
			g_hero.m_wBoostPower		=	_lpPacket->aiValue[2];
			g_hero.buildPower();
			break;
		}

		case	eEW_CHANGE_MAGIC_RESISTANCE	:
		{
			g_hero.m_allMagicResistance.m_sCorrectValue	=	_lpPacket->aiValue[0];
			g_hero.m_allMagicResistance.m_wTime			=	_lpPacket->aiValue[1];
			break;
		}
		case	eEW_CHANGE_FIRE_RESISTANCE	:
		{
			g_hero.m_fireResistance.m_sCorrectValue		=	_lpPacket->aiValue[0];
			g_hero.m_fireResistance.m_wTime				=	_lpPacket->aiValue[1];
			break;
		}
		case	eEW_CHANGE_WATER_RESISTANCE	:
		{
			g_hero.m_waterResistance.m_sCorrectValue	=	_lpPacket->aiValue[0];
			g_hero.m_waterResistance.m_wTime			=	_lpPacket->aiValue[1];
			break;
		}
		case	eEW_CHANGE_EARTH_RESISTANCE	:
		{
			g_hero.m_earthResistance.m_sCorrectValue	=	_lpPacket->aiValue[0];
			g_hero.m_earthResistance.m_wTime			=	_lpPacket->aiValue[1];
			break;
		}
		case	eEW_CHANGE_WIND_RESISTANCE	:
		{
			g_hero.m_windResistance.m_sCorrectValue		=	_lpPacket->aiValue[0];
			g_hero.m_windResistance.m_wTime				=	_lpPacket->aiValue[1];
			break;
		}
		case	eEW_CHANGE_LIGHT_RESISTANCE	:
		{
			g_hero.m_lightResistance.m_sCorrectValue	=	_lpPacket->aiValue[0];
			g_hero.m_lightResistance.m_wTime			=	_lpPacket->aiValue[1];
			break;
		}
		case	eEW_CHANGE_DARK_RESISTANCE	:
		{
			g_hero.m_darkResistance.m_sCorrectValue		=	_lpPacket->aiValue[0];
			g_hero.m_darkResistance.m_wTime				=	_lpPacket->aiValue[1];
			break;
		}
		case	eEW_OPEN_PIGEON_POST_SHOP	:
		{
			g_gwShop.openForPigeonPost((short)_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
			break;
		}

		case	eEW_CHANGE_NONAME_COUPLE_RING_NAME_TO_LAST_TRADER	:
		{
			cItem	*lpCoupleRing			=	g_hero.getNonameCoupleRing();

			if	(lpCoupleRing)
				lpCoupleRing->changeToCoupleRing(s_tradeManager.m_strLastestTradeUser);
			break;
		}

		case	eEW_TARGET_IS_PLAY_GUILD_BATTLE						:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TARGET_IS_PLAY_GUILD_BATTLE);
			break;
		}

		case	eEW_TARGET_IS_IN_PREMIUM_ZONE	:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TARGET_EXIST_IN_PREMIUM_ZONE);
			break;
		}
		case	eEW_END_FIRE_WORK_TIME			:
		{
			g_fireCrackerManager.endFireWorkTime();
			break;
		}

		case	eEW_FIRE_FIRE_WORK				:
		{
			g_fireCrackerManager.add(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2],_lpPacket->aiValue[3],_lpPacket->aiValue[4]);
			break;
		}

		case	eEW_USE_FIRE_WORK				:
		{
			WarningMessage(dMSG_WAIT_FOR_FIRST_FIREWORK);
			g_fireCrackerManager.useFireWork(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
			break;
		}

		case	eEW_CHANGE_ISPI_SETTING			:
		{
			g_hero.changeISPISetting(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
			break;
		}

		case	eEW_USE_BOTTOMLESS_BOX_SUCCESS	:
		{
			g_hero.useBottomlessBox(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
			break;
		}

		case	eEW_SET_CHAPTER					:
		{
			g_hero.setChapter(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
			break;
		}

		case	eEW_CAN_NOT_RETURN_TO_GUILD_HALL_DURING_BATTLE	:
			WarningMessage(_ms(dMSG_CAN_NOT_RETURN_TO_GUILD_HALL_DURING_BATTLE_FORM,_lpPacket->aiValue[0]));
			break;

		case	eEW_CAN_NOT_RETURN_TO_VILLAGE_DURING_BATTLE	:
			WarningMessage(_ms(dMSG_CAN_NOT_RETURN_TO_VILLAGE_DURING_BATTLE_FORM,_lpPacket->aiValue[0]));
			break;

		case	eEW_CAN_NOT_TELEPORT_DURING_BATTLE			:
			g_hero.m_bf1IsWaitTeleportResult	=	FALSE;
			WarningMessage(_ms(dMSG_CAN_NOT_TELEPORT_DURING_BATTLE_FORM,_lpPacket->aiValue[0]));
			break;

		case	eEW_CAN_NOT_USE_GATE_GLOVE_DURING_BATTLE	:
			WarningMessage(_ms(dMSG_CAN_NOT_USE_GATE_GLOVE_DURING_BATTLE_FORM,_lpPacket->aiValue[0]));
			break;

		case	eEW_CAN_NOT_FIRE_CRACKER_BY_ALREADY_USE_FIRE_CRACKER	:
			break;

		case	eEW_TRANS		:
		{
			if	(lpActor)
				lpActor->addEffect(eHEOP_FOOT,g_im.m_wTransImage);

			break;
		}

		case	eEW_SET_COOL_TIME										:
		{
			g_hero.m_awSkillCoolTime[_lpPacket->aiValue[0]]	=	_lpPacket->aiValue[1];
			break;
		}

		case	eEW_SET_ITEM_COOL_TIME										:
		{
			g_hero.m_awItemCoolTime[_lpPacket->aiValue[0]]	=	_lpPacket->aiValue[1];
			break;
		}

		case	eEW_IS_LIMIT_ACTIVE_COUNT_SKILL							:
		{
			WarningMessage(_ms(dMSG_LIMIT_ACTIVE_COUNT_SKILL_FORM,g_aSkill[_lpPacket->aiValue[0]].m_strName,_lpPacket->aiValue[1]));
			break;
		}
		case	eEW_LIMIT_COPY_ACTOR_COUNT								:
		{
			WarningMessage(dMSG_CAN_NOT_COPY_MORE_ACTOR);
			break;
		}

		case	eEW_TRANS_TO_WEAPON										:
		{
			if	(_lpPacket->aiValue[0]	==	g_hero.m_iSerial)
				g_hero.m_wPrincessWeaponOwner	=	_lpPacket->aiValue[1];

			if	(_lpPacket->aiValue[1]	!=	0xffff)
			{
				CActor	*lpActor	=	g_am.getActor(_lpPacket->aiValue[1]);

				if	(lpActor)
					lpActor->addEffect(eHEOP_FOOT,g_im.m_wTransImage);
			}
			else
			{
				CActor	*lpActor	=	g_am.getActor(_lpPacket->aiValue[0]);

				if	(lpActor)
				{
					lpActor->m_enchantedImage.m_bf1IsTransToWeapon	=	FALSE;

					lpActor->addEffect(eHEOP_FOOT,g_im.m_wTransImage);

					lpActor->setPos(_lpPacket->aiValue[2],_lpPacket->aiValue[3]);
				}
			}
			break;
		}

		case	eEW_DISPLAY_MAIN_QUEST_EPILOG							:
		{
			g_gwMainQuestEvent.openEpilog(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
			break;
		}

		case	eEW_ANSWER_RESULT_TO_WORD_QUIZ		:
		{
			if	(_lpPacket->aiValue[0]	==	0)
			{
				WarningMessage(dMSG_IS_INCORRECT_SOLUTION);
			}
			else
			{
				WarningMessage(_ms(dMSG_IS_CORRECT_SOLUTION_AND_GET_SCORE_FORM,_lpPacket->aiValue[0]));

				g_eventWordQuiz.m_wScore	+=	_lpPacket->aiValue[0];
			}
			break;
		}

		case	eEW_WORDQUIZ_SURVIVOR_COUNT			:
		{
			g_eventWordQuiz.m_wCommerCount	=	_lpPacket->aiValue[0];
			g_eventWordQuiz.m_wLiveUserCount=	_lpPacket->aiValue[1];

			break;
		}

		case	eEW_FAILED_DISARM_ACTOR_TRAP_BY_RANGE				:
		{
			AddSystemMessage(LTYELLOW,dMSG_FTW_FAILED_DISARM_BY_RANGE);
			break;
		}

		case	eEW_TRY_DISARM_ACTOR_TRAP_FAILED_BY_LOW_CP			:
		{
			AddSystemMessage(LTYELLOW,dMSG_TRY_DISARM_TRAP_FAILED_BY_LOW_CP);
			break;
		}

		case	eEW_TRY_DISARM_ACTOR_TRAP							:
		{
			AddSystemMessage(LTYELLOW,dMSG_TRY_DISARM_TRAP);
			break;
		}

		case	eEW_DISARM_ACTOR_TRAP_FAILED_BY_IS_CAN_NOT_DISARM_TRAP_BY_LOW_DISARM_LEVEL:
		{
			AddSystemMessage(LTYELLOW,dMSG_FAILED_DISARM_BY_LOW_DISARM_LEVEL);
			break;
		}

		case	eEW_DISARM_ACTOR_TRAP_FAILED_BY_SKILL_FAILED		:
		{
			AddSystemMessage(LTYELLOW,dMSG_DISARM_TRAP_FAILED);
			break;
		}

		case	eEW_FAILED_DISARM_ACTOR_TRAP_BY_IS_CAN_NOT_DETECT	:
		{
			break;
		}

		case	eEW_FAILED_DISARM_ACTOR_TRAP						:
		{
			break;
		}

		case	eEW_FAILED_DISARM_ACTOR_TRAP_BY_SAME_TEAM			:
		{
			AddSystemMessage(LTYELLOW,dMSG_EW_FAILED_DISARM_ACTOR_TRAP_BY_SAME_TEAM);
			break;
		}

		case	eEW_TRY_DISARM_ACTOR_TRAP_FAILED_BY_BUSY			:
		{
			break;
		}
		case	eEW_MAKE_SMALL_FLAME	:	//	작은 불꽃 생성
		{
			g_sm.makeSmallFlame(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2],_lpPacket->aiValue[3],_lpPacket->aiValue[4]);
			break;
		}
		case	eEW_MAKE_FIRE_WALL		:		//	파이어 월 생성
		{
			g_sm.makeFireWall(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2],_lpPacket->aiValue[3],_lpPacket->aiValue[4]);
			break;
		}
		case	eEW_REMOVE_FIRE_WALL	:	//	파이어 월 제거
		{
			g_sm.removeFireWall(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2],_lpPacket->aiValue[3],_lpPacket->aiValue[4]);
			break;
		}

		case	eEW_CHANGE_SAGE_BOOK_COLOR	:
		{
			lpActor->changeSageBookColor(_lpPacket->aiValue[1]);
			break;
		}

		case	eEW_BOSS_BATTLE_EVENT		:
		{
			g_bossMonsterEvent.beginBossBattleEvent(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);
			break;
		}

		case	eEW_SET_FOCUS				:
		{
			g_am.setGroundFocus(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2]);

			break;
		}

		case	eEW_GUILD_EXP				:
		{
			int	iExp	=	_lpPacket->aiValue[0];
			iExp		<<=	16;
			iExp		+=	_lpPacket->aiValue[1];

			AddSystemMessage(WHITE,dMSG_INCREASE_GUILD_EXP_FORM,iExp);

			break;
		}

		case	eEW_CH5_RELEASE_ITEM_REVERSION	:
		{
			g_hero.releaseItemReversion(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);

			break;
		}

		case	eEW_CH5_RESET_ONE_SKILL	:
		{
			g_hero.resetOneSkill(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2],_lpPacket->aiValue[3]);
			

			break;
		}
		case	eEW_CH5_RESET_ONE_STATE	:
		{
			g_hero.resetOneState(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2],_lpPacket->aiValue[3]);

			break;
		}

		case	eEW_CH5_ITEM_PREFIX_LEVEL_UP	:
		{
			cItem	*lpItem			=	g_hero.getInventoryItem(_lpPacket->aiValue[0]);
			cItem	*lpTargetItem	=	g_hero.getInventoryItem(_lpPacket->aiValue[1]);

			int		iOldPrefx		=	_lpPacket->aiValue[2];
			int		iNewPrefx		=	_lpPacket->aiValue[3];

			if	(lpItem)
				lpItem->decreaseCount(1);

			if	(lpTargetItem)
			{
				g_msgBox.cPopup("",_ms(dMSG_UPGRADE_PREFIX_RESULT_FORM,lpTargetItem->getName(),g_aItemPrefix[iOldPrefx].m_str1stPrefix,g_aItemPrefix[iNewPrefx].m_str1stPrefix)
							,dMSG_OK);
			}

			break;
		}

		case	eEW_CH5_UPGRADE_ITEM	:
		{
			cItem	*lpItem			=	g_hero.getInventoryItem(_lpPacket->aiValue[0]);
			cItem	*lpTargetItem	=	g_hero.getInventoryItem(_lpPacket->aiValue[1]);

			if	(lpItem)
				lpItem->decreaseCount(1);

			if	(lpTargetItem)
			{
				cItem	oldItem;

				oldItem.copy(lpTargetItem);

				int	iPrefix		=	_lpPacket->aiValue[2];
				int	iValue1		=	_lpPacket->aiValue[3];
				int	iValue2		=	_lpPacket->aiValue[4];
				int	iOldValue1	=	oldItem.m_aPrefix[iPrefix].m_aValue[0];
				int	iOldValue2	=	oldItem.m_aPrefix[iPrefix].m_aValue[1];

				cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[oldItem.m_aPrefix[iPrefix].m_wPrefix];

				lpTargetItem->upgradeItem(iPrefix,iValue1,iValue2,FALSE);

				char	strText[512];

				sprintf(strText,dMSG_UPGRADE_ITEM_RESULT_FORM,lpTargetItem->getName(),lpPrefix->m_str1stPrefix,iOldValue1,iValue1);

				g_msgBox.cPopup("",strText,dMSG_OK);
			}
			break;
			
		}

		case	eEW_UPDATE_RED_STONE_COUNT	:
		{
			int	iPart		=	_lpPacket->aiValue[0];
			int	iHeaven		=	_lpPacket->aiValue[1];
			int	iHell		=	_lpPacket->aiValue[2];
			int	iRedDevil	=	_lpPacket->aiValue[3];
			int	iIncrease	=	_lpPacket->aiValue[4];

			if	(iIncrease)
			{
				if	(iPart <= 1)
					AddSystemMessage(WHITE,dMSG_INCREASE_HEAVEN_HELL_RED_STONE_FORM,g_lpstrRedStoneReceiver[iPart]);
				else
					AddSystemMessage(WHITE,dMSG_INCREASE_RED_DEVILS_RED_STONE_FORM,g_lpstrRedStoneReceiver[iPart]);
			}

			g_iHeavenRedStoneCount	=	iHeaven;
			g_iHellRedStoneCount	=	iHell;
			g_iRedDevilRedStoneCount=	iRedDevil;

			break;
		}

		case	eEW_CANCEL_CAST_BARD_TITLE		:
		{
			AddSystemMessage(LTYELLOW,dMSG_CANCEL_CASTING_BARD_TITLE);
			break;
		}

		case	eEW_USE_BARD_TITLE				:
		{
			g_esm.play(_ms("bard%.2d.wav",random(3)+1));
			AddSystemMessage(LTGREEN,dMSG_WAIT_TO_END_SONG_THEN_ACTIVE_EFFECT);
			break;
		}

		case	eEW_UPDATE_BARD_TITLE_COOL_TIME	:
		{
			g_hero.m_bf5BardTitleCoolTime	=	_lpPacket->aiValue[0];
			break;
		}

		case	eEW_REQUIRE_PORTAL_ITEM_TO_USE_THAT_TITLE	:
		{
			AddSystemMessage(LTYELLOW,dMSG_REQUIRE_PORTAL_ITEM_TO_USE_THAT_TITLE);
			break;			
		}

		case	eEW_OPEN_RELAY_STATION						:
		{
//			g_gwRelayStation.open();
			break;			
		}

		case	eEW_OPERATE_DUEL_MATCH	:
		{
			switch(_lpPacket->aiValue[0])
			{
				case	0	:	//	결투 시작
				{
					g_duelManager.beginBattle(_lpPacket->aiValue[1]);
					break;
				}

				case	2	:	//	결투 종료
					WarningMessage(dMSG_END_DUEL);
					break;

				case	3	:	//	결투 상대가 정해졌다.
					g_duelManager.reset();
					AddSystemMessage(WHITE,dMSG_DECIDED_DUEL_MATCH_TARGET_AND_MOVE_TO_DUEL_FIELD);
					break;

				case	4	:	//	결투 시작까지 10초 남았다.
					g_countDown.begin(_lpPacket->aiValue[1],(void *)&s_textBold24M,cDRAW::HEIGHT/12,dMSG_READY_TO_BATTLE_REMAIN_SOMETIME_FORM);
					break;

				case	5	:	//	부활까지 10초 남았다.
					g_countDown.begin(_lpPacket->aiValue[1],(void *)&s_textBold24M,cDRAW::HEIGHT/12,dMSG_REMAIN_TIME_TO_REBIRTH_FORM);
					break;
			}

			break;
		}

		case	eEW_DUEL_MATCH_APPLICANT_INFO	:
		{
			g_duelManager.updateCount(_lpPacket->aiValue[0],_lpPacket->aiValue[1]);

			break;
		}

		case	eEW_OPEN_DUEL_RANKING_WINDOW	:
		{
			g_commonUI.open(eCUI_DUEL_RANKING,dMSG_WEEKLY_DUEL_RANK,400,10,380,450);

			break;
		}

		case	eEW_REMAIN_TIME_FOR_USE_DUEL_BUFF	:
		{
			AddSystemMessage(LTYELLOW,dMSG_REMAIN_TIME_FOR_USE_DUEL_BUFF_FORM,_lpPacket->aiValue[0]);
			break;
		}

		case	eEW_END_DUEL_BATTLE_BY_OUT_ALL_TARGET_TEAM_MEMBER	:
		{
			g_msgBox.cPopup("",dMSG_END_DUEL_BATTLE_BY_OUT_ALL_TARGET_TEAM_MEMBER,dMSG_OK);
			g_msgBox.setButtonId(eMBI_RETURN_TO_THE_VILLAGE_BY_END_OF_GUILD_BATTLE);
			break;
		}

		case	eEW_EAT_DUEL_FIELD_ARCA_ITEM	:
		{
			AddSystemMessage(WHITE,dMSG_EW_EAT_DUEL_FIELD_ARCA_ITEM_FORM,g_aBasicItem[_lpPacket->aiValue[0]].m_strName);
			break;
		}

		case	eEW_RESET_WEEKLY_DUEL_RECORD	:
		{
			g_hero.m_duelRecord.m_iWeeklyWinPoint		=	0;	//	주간 결투 승점
			g_hero.m_duelRecord.m_iWeeklyBattleCount	=	0;	//	주간 결투 횟수
			g_hero.m_duelRecord.m_iWeeklyWinCount		=	0;	//	주간 결투 승리 횟수
			g_hero.m_duelRecord.m_iWeeklyDefeatCount	=	0;	//	주간 결투 패배 횟수
			g_hero.m_duelRecord.m_iWeeklyDrawCount		=	0;	//	주간 결투 무승부 횟수

			break;
		}
		case	eEW_CAN_NOT_PRINCSS_WEAPON_CHANGE:
		{
			AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_PRINCSS_WEAPON_CHANGE);
			break;
		}
		case	eEW_SCREEN_START_EVENT			:
		{
			switch(_lpPacket->aiValue[0])
			{
			case 0://스크린 흑백
				g_hero.m_wBlackScreenCount = _lpPacket->aiValue[1];
				break;
			case 1://티비 
				g_hero.m_wTVScreenCount = _lpPacket->aiValue[1];
				break;
			}
			break;
		}
		case	eEW_DISJOINTING_ITEM_RESULT_FLASE			:
		{
			AddSystemMessage(LTYELLOW,dMSG_DISJOINTING_ITEM_FAILED);
			break;
		}
		case eEW_BUY_GOLD_BAR			:		// 금괴 사기~~~ 09.08.19
			{
				
				int iCount				= _lpPacket->aiValue[0];
				
				int iTax				= _lpPacket->aiValue[1];
				LONGLONG	llGold		=	100000000 * iCount;
				LONGLONG	llTaxGold	=	( llGold + (llGold * (iTax/10.0)/100.0) );
				
				g_bank.m_wGoldBarCount	=	iCount;
				g_bank.m_wGoldBarTax	=	iTax;
				
				char strMessage[512] = "";
				sprintf(strMessage,dMSG_BUY_GOLD_BAR,iCount,GetMoneyString(llTaxGold), iTax/10.0f);
				g_msgBox.cPopup("",strMessage,dMSG_YES, dMSG_NO);
				g_msgBox.setButtonId(eMBI_BUY_GOLD_BAR);
				
				break;
			}
			
		case eEW_BUY_GOLD_BAR_ERR			:		// 금괴 사기 에러~~~~ 09.08.19
			{
				
				g_msgBox.cPopup("",dMSG_BUY_GOLD_BAR_ERR,dMSG_OK);
				
				break;
			}
			
		case eEW_BUY_GOLD_BAR_MESSAGE	:		// 금괴 사기 메세지~~~~ 09.08.20
			{
				LONGLONG	llGold		=	100000000 * g_bank.m_wGoldBarCount;
				LONGLONG	llTaxGold	=	( llGold + (llGold * (g_bank.m_wGoldBarTax/10.0)/100.0) );
				cItem	addItem;
				
				cItem::GenerateItemByBaseItem(&addItem,dITEM_INDEX_GOLD_BAR,g_bank.m_wGoldBarCount);
				AddSystemMessage(WHITE,dMSG_BUY_ITEM_FORM,dMSG_GOLD_BAR,g_bank.m_wGoldBarCount,CGamePlay::GetMoneyString(llTaxGold));
				g_hero.m_iGold -= llTaxGold;
				g_hero.addItemToInventory((cItem*)&addItem);
				g_bank.m_wGoldBarCount	=	0;
				g_bank.m_wGoldBarTax	=	0;
				
				break;
			}
		case eEW_OPEN_UPGRADE_SHOP	:		// 강화창을 연다 패킷처리~~~ 09.08.25
			{
				g_gwUpgradeShop.open();
				break;
			}
		case eEW_REALLY_ITEM_UPGRADE	:		// 정말로 강화 할꺼임?~~~ 09.08.27
			{
				
				char strMaterials[256] = "";
				
				int iGold				= _lpPacket->aiValue[0] * 100000;
				int iStoneOfMestery		= _lpPacket->aiValue[1];
				int iTantilless			= _lpPacket->aiValue[2];
				int iCrystal			= _lpPacket->aiValue[3];
				
				if(iGold)
					sprintf(strMaterials,"%s  <c:LTYELLOW>%s<n>\n", dMSG_KOR_GOLD , GetMoneyString(iGold));
				if(iStoneOfMestery)
					sprintf(strMaterials,"%s%s  <c:LTYELLOW>%d%s<n>\n",strMaterials, dMSG_ITEM_STONEOFMYSTERY, iStoneOfMestery, dMSG_COUNT);
				if(iTantilless)
					sprintf(strMaterials,"%s%s  <c:LTYELLOW>%d%s<n>\n",strMaterials, dMSG_ITEM_TANTILLESS, iTantilless, dMSG_COUNT);
				if(iCrystal)	
					sprintf(strMaterials,"%s%s  <c:LTYELLOW>%d%s<n>\n",strMaterials, dMSG_ITEM_CRYSTAL, iCrystal, dMSG_COUNT);
				
				cItem* lpItem = g_hero.getInventoryItem(g_gwUpgradeShop.getUpgradeWeaponItemSlot());
				if(!lpItem)
					break;
				
				cBasicItem* lpBasic = lpItem->getBasicItem(TRUE);
				
				if(!lpBasic)
					break;
				
				cItem	copyItem;
				cItem::GenerateItemByBaseItem(&copyItem,lpBasic->getUpgradePenaltyItemSerial(),1);
				
				char strMessage[512] = "";
				char strTitle[32] = "";
				sprintf(strMessage,dMSG_REALLY_UPGRADE_FORM,strMaterials, copyItem.getName());
				sprintf(strTitle,"%s %s", dMSG_ITEM, dMSG_UPGRADE);
				g_msgBox.cPopup(strTitle,strMessage,dMSG_YES, dMSG_NO);
				g_msgBox.setButtonId(eMBI_UPGRADE_ETERNAL_ITEM);
				g_msgBox.setSize(340,170);
				
				break;
			}
		case eEW_UPGRADE_MATERIALS_ERR:		// 재료가 부족해용~~ 09.08.26
			g_msgBox.cPopup("",dMSG_NOT_MATERIALS,dMSG_OK);
			g_gwUpgradeShop.close();
			break;
		case eEW_UPGRADE_SUCCESS:		// 강화가 성공했따 09.08.26
			{
				OperateUpgradeSuccess(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2],_lpPacket->aiValue[3] );
				break;
			}
		case eEW_UPGRADE_FAILED:			// 강화가 실패 했다.~09.08.26
			{								
				OperateUpgradeFailed(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2],_lpPacket->aiValue[3] );
				break;
			}
		case eEW_DRAG_ITEM_DIVIDE:		// 스택나누기
			{	
				
				int iDragItemSlot	= _lpPacket->aiValue[0];
				int iFocusItemSlot	= _lpPacket->aiValue[1];
				int iItemCount		= _lpPacket->aiValue[2];						
				
				cItem* lpDragItem	= g_hero.getInventoryItem(iDragItemSlot);
				
				cItem	copyItem;
				cItem::GenerateItemByBaseItem(&copyItem,lpDragItem->m_wBaseItem,iItemCount);
				memcpy(copyItem.m_aOption, lpDragItem->m_aOption , sizeof(copyItem.m_aOption));
				memcpy(copyItem.m_aPrefix, lpDragItem->m_aPrefix , sizeof(copyItem.m_aPrefix));

				lpDragItem->decreaseCount(iItemCount);				
				if	(iFocusItemSlot	>=	dOWN_ITEM_COUNT)
					g_hero.m_aExtraInventory[iFocusItemSlot-dOWN_ITEM_COUNT].copy(&copyItem);
				else
					g_hero.m_aItems[iFocusItemSlot].copy(&copyItem);
				
				g_hero.arrangeInventory();
				break;
			}
		case eEW_DISJOINTING_RESULT : //09.09.07 아이템 분해 결과
			{
				int iRemoveItemSlot		= _lpPacket->aiValue[0];
				int iTantillessPiece		= _lpPacket->aiValue[1];
				int iStoneOfMesteryPiece	= _lpPacket->aiValue[2];
				int iCrystal				= _lpPacket->aiValue[3];
				

				cItem* lpRemoveItem = g_hero.getInventoryItem(iRemoveItemSlot);
				if(lpRemoveItem)
				{
					AddSystemMessage(WHITE,dMSG_REMOVE_ITEM_BY_QUEST_FORM,lpRemoveItem->m_bCount, lpRemoveItem->getName());
					g_hero.removeItem(iRemoveItemSlot,1);
				}

				if(iTantillessPiece)
				{
					cItem	addItem;
					cItem::GenerateItemByBaseItem(&addItem,dITEM_INDEX_TANTILLESS_PIECE,iTantillessPiece);
					AddSystemMessage(WHITE,dMSG_RECEIVE_ITEM_COUNT_BY_QUEST_FORM,iTantillessPiece,addItem.getName());
					g_hero.addItemToInventory((cItem*)&addItem);

				}
				if(iStoneOfMesteryPiece)
				{
					cItem	addItem;
					cItem::GenerateItemByBaseItem(&addItem,dITEM_INDEX_STONEOFMYSTERY_PIECE,iStoneOfMesteryPiece);
					AddSystemMessage(WHITE,dMSG_RECEIVE_ITEM_COUNT_BY_QUEST_FORM,iStoneOfMesteryPiece,addItem.getName());
					g_hero.addItemToInventory((cItem*)&addItem);

				}
				
				if(iCrystal)
				{
					cItem	addItem;
					cItem::GenerateItemByBaseItem(&addItem,dITEM_INDEX_CRYSTAL,iCrystal);
					AddSystemMessage(WHITE,dMSG_RECEIVE_ITEM_COUNT_BY_QUEST_FORM,iCrystal,addItem.getName());
					g_hero.addItemToInventory((cItem*)&addItem);
				}
				break;
			}	
		case eEW_CHANGE_WEATHER:
			{		// 기후변화 
				g_map.m_wWeather = _lpPacket->aiValue[0];
				break;
			}
		case	eEW_CHANGE_CONSENSUS:
			{
				g_hero.m_wConsensusTime = _lpPacket->aiValue[0];

				break;
			}
		case eEW_CHANGE_FLEE:
			{
				g_hero.m_wFleeTime = _lpPacket->aiValue[0];
				g_hero.moveTo(_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
				break;
			}
		case eEW_ADD_EXTRA_EFFECT_IMAGE:
			{		// 부가효과 이미지 추가.
				CActor		*lpActor	=	g_am.getTestedActor(_lpPacket->aiValue[0]);
				if(!lpActor)
					break;
				lpActor->addEffect(_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
				break;
			}
		case eEW_OPEN_COPY_OR_CUT_PREFIX_WINDOW:
			{		// 복사창 or 잘라내기 
				g_gwCopyOrCutPrefix.open(_lpPacket->aiValue[0],_lpPacket->aiValue[1],_lpPacket->aiValue[2]);
				break;
			}
		case	eEW_CHANGE_FIGHTING_SPIRIT:
			{
				g_hero.m_wFightingSpiritTime = _lpPacket->aiValue[0];

				break;
			}
		case	eEW_JUMP:
			{
				
				CActor		*lpTarget	=	g_am.getTestedActor(_lpPacket->aiValue[0]);
				
				if(!lpTarget)
					break;

				if(lpTarget->isDeath())
					break;
				if(lpTarget->m_bIsEffectJump)
					break;
				
				lpTarget->m_wIncreaseHeightEffectJump	=	_lpPacket->aiValue[1];
				lpTarget->m_wEffectJumpSpeed			=	_lpPacket->aiValue[2];
				lpTarget->m_wJumpFrameCount				=	0;
				lpTarget->m_bIsEffectJump				=	TRUE;
				break;
			}
		case eEW_DISPELL:
			{
				g_hero.m_maxHP.m_sCorrectValue				= 0;
				g_hero.m_maxHP.m_sCorrectPercentageValue	= 0;

				g_hero.cureWrongStatus();

				g_hero.buildPower();

				break;
			}
		case	eEW_END_TRANSFORMATION_MONSTER:
			{

				g_hero.m_wTransformationLevel	= 0;
				g_hero.m_wTransformationSkin	= 0xffff;
				g_hero.m_wTransformationSkill	= 0xffff;
				g_hero.m_wTransformationTime	= 0;
				g_hero.buildPower();

				break;				
			}

		case eEW_SUCCESS_INCREASE_CARVING_LEVEL:
			{
				int iUseItem		= _lpPacket->aiValue[0];
				int	iTargetItem		= _lpPacket->aiValue[1];
				int iIncreaseValue	= _lpPacket->aiValue[2];

				cItem		*lpItem			=	g_hero.getInventoryItem(iUseItem);
				cItem		*lpTargetItem	=	g_hero.getInventoryItem(iTargetItem);		
				if(!lpItem  || !lpTargetItem)
					break;

				lpTargetItem->increaseCarvingLevel(iIncreaseValue);

				g_hero.useItemToItem(iUseItem,iTargetItem);	

				g_msgBox.cPopup("",dMSG_CARVING_SUCCESS,dMSG_OK);
				CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CARVING_SUCCESS);
				break;

			}
		case eEW_RESULT_CARVE_LEVEL_MESSAGE:
			{
				char strMessage[512] = "";
				int iActor		= _lpPacket->aiValue[0];
				int	iDestItem	= _lpPacket->aiValue[1];
				int	iLevel		= _lpPacket->aiValue[2];
				BOOL bIsFailed	= _lpPacket->aiValue[3];
				cItem	item;
				cItem::GenerateItemByBaseItem(&item,iDestItem,1);
				item.setCarvingLevel(iLevel);
				CActor		*lpTarget	=	g_am.getTestedActor(iActor);
				if(!lpTarget)
					break;
				if(bIsFailed)
					sprintf(strMessage, "%s %s %s %s", lpTarget->getName(), dMSG_HERR, item.getName(FALSE),dMSG_CARVING_FAILED);
				else
					sprintf(strMessage, "%s %s %s %s", lpTarget->getName(),dMSG_HERR,  item.getName(FALSE),dMSG_CARVING_SUCCESS);

				g_hero.WarningMessage(strMessage);
				break;
			}
		case eEW_SEND_GOLD_LOG:
			{
				LONGLONG llFrontGoldAll			= _lpPacket->aiValue[0];
				LONGLONG llBackGoldAll			= _lpPacket->aiValue[1];
				LONGLONG llFrontGettingGold		= _lpPacket->aiValue[2];
				LONGLONG llBackGettingGold		= _lpPacket->aiValue[3];
				
				LONGLONG	llGoldAll		=	(llFrontGoldAll << 32) | llBackGoldAll;
				LONGLONG	llGettingGold	=	(llFrontGettingGold<< 32) | llBackGettingGold;
				char strMessage[256] = "";

				sprintf(strMessage,"Gold : <c:LTYELLOW>%s<n>", GetMoneyString(llGoldAll));

				if	(llGettingGold<0)
					sprintf(strMessage,"%s , <c:LTRED>%s<n>", strMessage, GetMoneyString(llGettingGold));
				else
					sprintf(strMessage,"%s , <c:LTGREEN>%s<n>", strMessage, GetMoneyString(llGettingGold));

				CGamePlay::AddSystemMessage(WHITE, strMessage);
				break;
			}
	}
}

void
CGamePlay::OperateUpgradeSuccess(int _iRemoveItemSlot, int _iNextEternalItemSerial,int _iUpgradeItemSlot, int _iUpgradeItemNormalSlot)
{
	
		int iRemoveItemSlot				= _iRemoveItemSlot;
		int iNextEternalItemSerial		= _iNextEternalItemSerial;
		int iUpgradeItemSlot			= _iUpgradeItemSlot;
		int iUpgradeItemNormalSlot		= _iUpgradeItemNormalSlot;
		
		cItem* lpRemoveItem = g_hero.getInventoryItem(iRemoveItemSlot);

		CItemPrefixInfo		m_aWeaponPrefix[dITEM_PREFIX_COUNT];	
		memcpy(m_aWeaponPrefix, lpRemoveItem->m_aPrefix,sizeof(CItemPrefixInfo) * dITEM_PREFIX_COUNT);
	
		cItem	addItem;				
		cItem::GenerateItemByBaseItem(&addItem,iNextEternalItemSerial,1);
		cBasicItem* lpBasic = addItem.getBasicItem(TRUE);
		if(!lpBasic)
			return;
		addItem.m_bDurability = lpBasic->getDurability();
		memcpy(addItem.m_aPrefix, m_aWeaponPrefix,sizeof(CItemPrefixInfo) * dITEM_PREFIX_COUNT);
		char strRemoveItemName[128] = "";
		char strForm[512] = "";
		char strNextItemName[128] = "";	
		
		if(lpRemoveItem)	
			strcpy(strRemoveItemName,lpRemoveItem->getName(TRUE));
		
		strcpy(strNextItemName, addItem.getName(TRUE));

		AddSystemMessage(WHITE,dMSG_SUCCESS_UPGRADE);
		
		if(iUpgradeItemSlot != 0xffff)
		{
			cItem* lpItem = g_hero.getInventoryItem(iUpgradeItemSlot);
			AddSystemMessage(WHITE,dMSG_REMOVE_ITEM_BY_QUEST_FORM,1,lpItem->getName());
			cBasicItem* lpBasic = lpItem->getBasicItem(TRUE);

			if(lpBasic && lpBasic->isUpgradeEternalItemNotPenalty())
				AddSystemMessage(WHITE,dMSG_COATING_ITEM_EVAPORATION);

			g_hero.removeItem(iUpgradeItemSlot,1);

		}
		if(iUpgradeItemNormalSlot != 0xffff)
		{
			cItem* lpItem = g_hero.getInventoryItem(iUpgradeItemNormalSlot);
			AddSystemMessage(WHITE,dMSG_REMOVE_ITEM_BY_QUEST_FORM,1,lpItem->getName());
			g_hero.removeItem(iUpgradeItemNormalSlot,1);
		}
		
		g_hero.removeItem(iRemoveItemSlot,1);
		
		
		
		g_hero.addItemToInventory((cItem*)&addItem);
		
		
		sprintf(strForm, "%s %s %s\n%s",strRemoveItemName, dMSG_RIGHT_ARROW, strNextItemName,dMSG_SUCCESS_UPGRADE);
			
		g_gwUpgradeShop.close();
		g_msgBox.cPopup("",strForm,dMSG_OK);

		sprintf(strForm,"%s %s %s %s" , strRemoveItemName, dMSG_RIGHT_ARROW,strNextItemName,dMSG_UPGRADE_SUCCESS_FORM);		
		
		AddSystemMessage(WHITE,strForm);

	s_UpgradeFieldTitle.init(dMSG_UPGRADE_SUCCESS, g_iFieldScreenHeight/4.3);	

}
void
CGamePlay::OperateUpgradeFailed(int _iRemoveItemSlot, int _iUpgradePenaltyItemSerial,int _iUpgradeItemSlot, int _iUpgradeItemNormalSlot)
{

	int iRemoveItemSlot				= _iRemoveItemSlot;
	int iUpgradePenaltyItemSerial	= _iUpgradePenaltyItemSerial;
	int iUpgradeItemSlot			= _iUpgradeItemSlot;
	int iUpgradeItemNormalSlot		= _iUpgradeItemNormalSlot;
	BOOL	isNotPenalty			= FALSE;
	
	cItem* lpItem = g_hero.getInventoryItem(iRemoveItemSlot);
	CItemPrefixInfo		m_aWeaponPrefix[dITEM_PREFIX_COUNT];	
	memcpy(m_aWeaponPrefix, lpItem->m_aPrefix,sizeof(CItemPrefixInfo) * dITEM_PREFIX_COUNT);
	char strRemoveItemName[128] = "";
	char strMsgBoxForm[512] = "";
	char strSystemMsgForm[512] = "";
	char strPenaltyItemName[128] = "";
	
	if(lpItem)	
		strcpy(strRemoveItemName,lpItem->getName(TRUE));

	AddSystemMessage(WHITE,dMSG_FAILED_UPGRADE);

	if(iUpgradeItemSlot != 0xffff)
	{
		cItem* lpItem = g_hero.getInventoryItem(iUpgradeItemSlot);
		AddSystemMessage(WHITE,dMSG_REMOVE_ITEM_BY_QUEST_FORM,1,lpItem->getName());
		cBasicItem* lpBasic = lpItem->getBasicItem(TRUE);
		
		if(lpBasic && lpBasic->isUpgradeEternalItemNotPenalty())
			isNotPenalty = TRUE;

		g_hero.removeItem(iUpgradeItemSlot,1);
	}
	if(iUpgradeItemNormalSlot != 0xffff)
	{
		cItem* lpItem = g_hero.getInventoryItem(iUpgradeItemNormalSlot);
		AddSystemMessage(WHITE,dMSG_REMOVE_ITEM_BY_QUEST_FORM,1,lpItem->getName());
		g_hero.removeItem(iUpgradeItemNormalSlot,1);
	}
	
	
	if(iUpgradePenaltyItemSerial !=0xffff)
	{
		g_hero.removeItem(iRemoveItemSlot,1);
		cItem	addItem;				
		cItem::GenerateItemByBaseItem(&addItem,iUpgradePenaltyItemSerial,1);
		cBasicItem* lpBasic = addItem.getBasicItem(TRUE);
		if(!lpBasic)
			return;
		addItem.m_bDurability = lpBasic->getDurability();
		memcpy(addItem.m_aPrefix, m_aWeaponPrefix,sizeof(CItemPrefixInfo) * dITEM_PREFIX_COUNT);		
		strcpy(strPenaltyItemName,addItem.getName(TRUE));

		g_hero.addItemToInventory((cItem*)&addItem);
	}
	else
	{
		strcpy(strPenaltyItemName, strRemoveItemName);

	}
	
	sprintf(strMsgBoxForm, "%s %s %s\n%s",strRemoveItemName, dMSG_RIGHT_ARROW ,strPenaltyItemName ,dMSG_FAILED_UPGRADE);
	sprintf(strSystemMsgForm,"%s %s %s %s", strRemoveItemName, dMSG_RIGHT_ARROW, strPenaltyItemName, dMSG_UPGRADE_FAILED_FORM);
	g_gwUpgradeShop.close();
	g_msgBox.cPopup("",strMsgBoxForm,dMSG_OK);
	
	if(isNotPenalty)
	{
		if(iUpgradePenaltyItemSerial !=0xffff)
		{
			AddSystemMessage(WHITE,dMSG_COATING_ITEM_EFFECT_GENERATION);
		}
		else
		{
			AddSystemMessage(WHITE,dMSG_COATING_ITEM_EFFECT_GENERATION);
		}
	
	}
	AddSystemMessage(WHITE,strSystemMsgForm);
	g_hero.m_wBlackScreenCount = 2;
	s_UpgradeFieldTitle.init(dMSG_UPGRADE_FAILED, g_iFieldScreenHeight/4.3);
}




void
CGamePlay::OperateSetAnm(SG_SET_ANM* _lpPacket)
{
	for (int i=0;i<_lpPacket->wCount;i++)
	{
		cSetAnmPacketActorInfo	*lpData	=	&_lpPacket->aActorList[i];

		CActor	*lpActor	=	g_am.getTestedActor(lpData->m_bf12Actor,FALSE);

		if	(!lpActor)
			continue;

		if	(lpData->m_bf15ReplayCount)
			lpActor->setReplayCount(lpData->m_bf15ReplayCount);

		if	(lpData->m_bf5Anm)
			lpActor->setAnm(lpData->m_bf5Anm-1,TRUE);

		if	(lpData->m_bf4Direct	<=	dDIRECT_NW)
			lpActor->setDirect(lpData->m_bf4Direct,TRUE);
	}
}


void
CGamePlay::OperateGuildBattleKillNotice(SG_GUILD_BATTLE_KILL_NOTICE* _lpPacket)
{
	g_guildBattleInfo.m_iTeam1KillPoint	=	_lpPacket->iGuild1KillPoint;
	g_guildBattleInfo.m_iTeam2KillPoint	=	_lpPacket->iGuild2KillPoint;
	
	if	(_lpPacket->wIsDuel	==	FALSE)
		AddSystemMessage(WHITE,dMSG_GUILD_KILL_NOTICE_FORM,_lpPacket->strKiller,_lpPacket->strTarget,g_guildBattleInfo.m_strTeamName[_lpPacket->wKillerTeam],_lpPacket->wGetPoint);
	else
	{
		DWORD	dwColor	=	LTRED;

		if	(_lpPacket->wKillerTeam	==	g_hero.m_wTeam)
			dwColor	=	LTGREEN;

		if	(g_bIsObserverModeForDuel)
			dwColor	=	WHITE;

		AddSystemMessage(dwColor,dMSG_KILL_TARGET_FOR_DUEL_FORM,_lpPacket->strKiller,_lpPacket->strTarget);
	}
}

void
CGamePlay::OperateGuildBattleInfo(SG_GUILD_BATTLE_INFO* _lpPacket)
{
	g_guildBattleInfo.m_bIsBeginBattle	=	TRUE;
	g_guildBattleInfo.m_iOwnTeam		=	g_lpHero->m_wTeam;

	strcpy(g_guildBattleInfo.m_strTeamName[0],_lpPacket->strTeam1Name);
	strcpy(g_guildBattleInfo.m_strTeamName[1],_lpPacket->strTeam2Name);

	if	(g_bIsDuelServer && _lpPacket->wIsTrialGame	==	FALSE && g_hero.m_wOperatorLevel	<	4)
	{
		if	(g_lpHero->m_wTeam	==	0)
		{
			strcpy(g_guildBattleInfo.m_strTeamName[0],dMSG_OUR_TEAM);
			strcpy(g_guildBattleInfo.m_strTeamName[1],dMSG_ENEMY_TEAM);
		}
		else
		{
			strcpy(g_guildBattleInfo.m_strTeamName[0],dMSG_ENEMY_TEAM);
			strcpy(g_guildBattleInfo.m_strTeamName[1],dMSG_OUR_TEAM);
		}
	}

	if	(g_bIsDuelServer && g_hero.m_wOperatorLevel	>=	4)
	{
		s_bIsHideInterface	=	TRUE;
		s_bIsBlockUseSkill	=	TRUE;
		s_bIsObserverMode	=	TRUE;
		g_bIsObserverModeForDuel=	TRUE;
	}

	g_guildBattleInfo.m_iTeam1KillPoint	=	_lpPacket->iGuild1KillPoint;
	g_guildBattleInfo.m_iTeam2KillPoint	=	_lpPacket->iGuild2KillPoint;
}
