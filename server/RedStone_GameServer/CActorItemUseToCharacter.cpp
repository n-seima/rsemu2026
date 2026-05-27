#include "cItem.H"
#include "cACTOR.H"
#include "CGame.H"
#include "CGuild.H"
#include "itemPackList.H"
#include "packetManager.H"
#include "booking_work.H"
#include "cItem.H"
#include	"actor_mini_pet.h"
#include	"secretDungeonDefine.h"

const	int	c_iUpkeepPortalStoneTime	=	30;

BOOL
cACTOR::increaseLastDamage(int _iTime,int _iValue)
{
	m_correctLastDamage.correctValue(_iValue*10,_iTime*60);

	return	TRUE;
}

BOOL
cACTOR::increaseHuntExp(int _iTime,int _iValue)
{
	m_bonusExp.correctValue(_iValue*10,_iTime*60);

	sendEtcWork(eEW_INCREASE_HUNT_EXP,m_bonusExp.m_wTime,m_bonusExp.getCorrectedValue());

	return	TRUE;
}

BOOL
cACTOR::increaseItemDropChance(int _iTime,int _iValue)
{
	m_bonusItemDropChance.correctValue(_iValue*10,_iTime*60);

	sendEtcWork(eEW_INCREASE_ITEM_DROP_CHANCE,m_bonusItemDropChance.m_wTime,m_bonusItemDropChance.getCorrectedValue());
	
	return	TRUE;
}

BOOL
cACTOR::clearDeathPenelty()
{

	m_bf12DeathPeneltySecond	=	0;
	m_wCurrentLevelDeathPenelty	=	0;		//  수치도.. 초기화시킨다..
	buildPower();
	sendDeathPeneltySecond(FALSE,TRUE);
	
	return	TRUE;
}


int
cACTOR::useMagicCarpetEmblem(cItem *_lpItem,int _iEffect)
{
	if(m_wTransformationTime)
		return eUIR_FAILED;

	if (_lpItem->isMergeEmblemItem())
	{
		cItem	item;
		cItem	*lpEmblem		=	getMagicCarpetEmblem();
		BOOL	bIsExistEmblem	=	FALSE;

		if (lpEmblem)
		{
			bIsExistEmblem		=	TRUE;
			memcpy(&item,lpEmblem,sizeof(item));
		}
		else
		{
			lpEmblem			=	_lpItem;

			if (!_lpItem->makeMagicCarpetEmblem())
				return	eUIR_FAILED;
		}

		cMagicCarpetEmblemDefine	*lpEmblemItem	=	(cMagicCarpetEmblemDefine	*)lpEmblem;

		switch(_iEffect)
		{
			case	eIE_REGEN_HP_ON_MAGIC_CARPET				:	//	양탄자위에서 체력 회복
				if (lpEmblemItem->m_bf1IsTurtleCarpet)
					return	eUMCIR_ALREADY_EXIST_SHAPE;
				lpEmblemItem->m_bf1IsTurtleCarpet	=	TRUE;
				break;

			case	eIE_NOT_DOWN_CP_ON_MAGIC_CARPET				:	//	양탄자위에서 CP 감소 안함
				if (lpEmblemItem->m_bf1IsSquirrelCarpet)
					return	eUMCIR_ALREADY_EXIST_SHAPE;
				lpEmblemItem->m_bf1IsSquirrelCarpet	=	TRUE;
				break;

			case	eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET		:	//	양탄자위에서 이동속도 증가
				if (lpEmblemItem->m_bf1IsDrakeCarpet)
					return	eUMCIR_ALREADY_EXIST_SHAPE;
				lpEmblemItem->m_bf1IsDrakeCarpet	=	TRUE;
				break;
				
			case	eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET_RED	:	//	양탄자위에서 이동속도 증가
				if (lpEmblemItem->m_bf1IsRedDrakeCarpet)
					return	eUMCIR_ALREADY_EXIST_SHAPE;
				lpEmblemItem->m_bf1IsRedDrakeCarpet	=	TRUE;
				break;
				
			case	eIE_FIGHT_MAGIC_CARPET						:	//	전투
				if (lpEmblemItem->m_bf1IsGearCarpet)
					return	eUMCIR_ALREADY_EXIST_SHAPE;
				lpEmblemItem->m_bf1IsGearCarpet	=	TRUE;
				break;

			case	eIE_CHANGE_MAGIC_CARPET_SHAPE				:
				if (lpEmblemItem->m_bf1IsWildCat)
					return	eUMCIR_ALREADY_EXIST_SHAPE;
				lpEmblemItem->m_bf1IsWildCat	=	TRUE;
				break;
		}

		if	(bIsExistEmblem)
		{
			int	iSlot = lpEmblem - m_aItems;

			sendUpdateItemData(iSlot,TRUE);
			return	eUIR_OK;
		}

		int	iSlot = _lpItem - m_aItems;

		sendUpdateItemData(iSlot,TRUE);

		return	eUMCIR_OK;
	}

	int	iCarpetShape	=	eMCS_NORMAL;

	switch(_iEffect)
	{
		case	eIE_REGEN_HP_ON_MAGIC_CARPET				:	//	양탄자위에서 체력 회복
			iCarpetShape	=	eMCS_TURTLE;
			break;

		case	eIE_NOT_DOWN_CP_ON_MAGIC_CARPET				:	//	양탄자위에서 CP 감소 안함
			iCarpetShape	=	eMCS_SQUIRREL;
			break;

		case	eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET		:	//	양탄자위에서 이동속도 증가
			iCarpetShape	=	eMCS_DRAKE;
			break;

		case	eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET_RED		:	//	양탄자위에서 이동속도 증가
			iCarpetShape	=	eMCS_RED_DRAKE;
			break;

		case	eIE_FIGHT_MAGIC_CARPET						:	//	전투
			iCarpetShape	=	eMCS_GEAR;
			break;

		case	eIE_CHANGE_MAGIC_CARPET_SHAPE				:	//	전투
			iCarpetShape	=	eMCS_WILD_CAT;
			break;
	}

	if	(!callMagicCarpet(iCarpetShape, TRUE))
		return	eUIR_FAILED;

	return	eUIR_OK;
}

int
cACTOR::castDuelBuff(int _iEffect,int _iValue1)
{
	if	(m_wDuelBuffCoolTime)
	{
		sendEtcWork(eEW_REMAIN_TIME_FOR_USE_DUEL_BUFF,m_wDuelBuffCoolTime);
		return	eUIR_FAILED;
	}

	switch(_iEffect)
	{
		case	eIE_DUEL_BUFF_FIRE		:	//	결투 버프 불
			m_wDuelBuffType		=	eDUEL_BUFF_FIRE;
			break;
		case	eIE_DUEL_BUFF_WATER		:	//	결투 버프 물
			m_wDuelBuffType		=	eDUEL_BUFF_WATER;
			break;
		case	eIE_DUEL_BUFF_WIND		:	//	결투 버프 바람
			m_wDuelBuffType		=	eDUEL_BUFF_WIND;
			break;
		case	eIE_DUEL_BUFF_EARTH		:	//	결투 버프 대지
			m_wDuelBuffType		=	eDUEL_BUFF_EARTH;
			break;
	}

	m_wDuelBuffTime		=	_iValue1;
	m_wDuelBuffCoolTime	=	c_iDuelBuffCoolTime;
	addSendActorMagicEffectStatus();

	for (int i=0;i<2;i++)
	{
		cACTOR	*lpPet	=	getPetActor(i);
		cACTOR	*lpSB	=	getSummonBeastActor(i);

		if	(lpPet)
		{
			lpPet->m_wDuelBuffType		=	m_wDuelBuffType;
			lpPet->m_wDuelBuffTime		=	_iValue1;
			lpPet->m_wDuelBuffCoolTime	=	c_iDuelBuffCoolTime;
			lpPet->addSendActorMagicEffectStatus();
		}
		if	(lpSB)
		{
			lpSB->m_wDuelBuffType		=	m_wDuelBuffType;
			lpSB->m_wDuelBuffTime		=	_iValue1;
			lpSB->m_wDuelBuffCoolTime	=	c_iDuelBuffCoolTime;
			lpSB->addSendActorMagicEffectStatus();
		}
	}

	return	eUIR_OK;
}


void
cACTOR::operateBuffItem(cItem *_lpItem, cACTOR* _lpUser)
{

	cBasicItem	*lpBasicItem		=	_lpItem->getBasicItem();
	
	if(!lpBasicItem)
		return;


	for (int i=0;i<dITEM_UNIQUE_DATA_COUNT;++i)
	{


		if(lpBasicItem->m_aUniqueData[i].m_wEffect == 0xffff)
			continue;

		if(lpBasicItem->m_aUniqueData[i].m_wEffect == eIP_BUFF_SKILL_CASTING)
		{
			cAbility		ability;
			
			ability.m_wSkill	= lpBasicItem->m_aUniqueData[i].m_aValue[0];
			ability.m_wLevel	= lpBasicItem->m_aUniqueData[i].m_aValue[1];
			
			CSkill*	lpSkill	=	ability.getSkill();
			if(!lpSkill)
				continue;
			int	iStatus			= lpBasicItem->m_aUniqueData[i].m_aValue[2];
			DWORD dwTime		= lpBasicItem->m_aUniqueData[i].m_aValue[3]*100;
			
			cACTOR cTempActor;
			cTempActor.m_strength.m_sFixValue		= iStatus;
			cTempActor.m_agility.m_sFixValue		= iStatus;
			cTempActor.m_constitution.m_sFixValue	= iStatus;
			cTempActor.m_intelligence.m_sFixValue	= iStatus;
			cTempActor.m_wisdom.m_sFixValue			= iStatus;
			cTempActor.m_charisma.m_sFixValue		= iStatus;
			cTempActor.m_luck.m_sFixValue			= iStatus;
			
			applySkillExtraEffects(&cTempActor,&ability,0,m_pos.x, m_pos.y, FALSE, FALSE, dwTime);	
			if	(lpSkill->m_bf1IsSyncSkillExtraEffectWithPlayer)	//	본인의 정보를 싱크할 필요 있을때 전송한다.
				sendApplySkillExtraEffect(m_wSerialInField,&ability, lpBasicItem->m_aUniqueData[i].m_aValue[3]);
			
		}
		else if(lpBasicItem->m_aUniqueData[i].m_wEffect == eIP_ACL_BUFF_SKILL_CASTING)
		{
			int iCarvingLevel = _lpItem->getCarvingLevel();

			cAbility		ability;
			ability.m_wSkill	= lpBasicItem->m_aUniqueData[i].m_aValue[0];
			ability.m_wLevel	= iCarvingLevel;
			CSkill* lpSkill	=	ability.getSkill();

			applySkillExtraEffects(_lpUser,&ability,0,m_pos.x,m_pos.y , FALSE ,FALSE);
			if	(lpSkill && lpSkill->m_bf1IsSyncSkillExtraEffectWithPlayer)	//	본인의 정보를 싱크할 필요 있을때 전송한다.
				sendApplySkillExtraEffect(m_wSerialInField,&ability, lpBasicItem->m_aUniqueData[i].m_aValue[3]);			
		}
	}	

}


//
//	효과 처리
int
cACTOR::eatItem(cItem *_lpItem,int _iHealEfficient, cACTOR* _lpUser)
{
	int		i,iReferenceIndex1,iReferenceIndex2,iValue1,iValue2;
	BOOL	bIsUsedEffect	=	FALSE;
	int		iEffectCount	=	0;

	operateBuffItem(_lpItem, _lpUser);			// 버프아이템효과 처리

	cBasicItem	*lpBasicItem		=	_lpItem->getBasicItem();

	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if	(lpBasicItem->m_aGenerateData[i].m_wEffect	==	0xffff)
			break;

		iReferenceIndex1	=	lpBasicItem->m_aGenerateData[i].m_aValue[0];
		iReferenceIndex2	=	lpBasicItem->m_aGenerateData[i].m_aValue[1];
		iValue1				=	_lpItem->m_aOption[iReferenceIndex1];
		iValue2				=	_lpItem->m_aOption[iReferenceIndex2];

		int	iEffect			=	lpBasicItem->m_aGenerateData[i].m_wEffect;

		if	(iEffect	== eIE_MEMORY_STONE || iEffect	== eIE_TELEPORT_STONE || iEffect	== eIE_ONE_WAY_PORTAL	||	iEffect	== eIE_PORTAL_STONE)
		{
			if	(m_lpField->m_iUniqueSerial	==	27)
				return eUIR_FAILED;
			if	(m_lpField->m_wIsBlockToWarpField)
				return eUIR_FAILED;
			if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
				return eUIR_FAILED;
		}

		switch(iEffect)
		{
			case	eIE_EXPAND_INVENTORY_BAG	:
			{
				int	iExpandInventorySize	=	_lpItem->m_aOption[0];

				if	(m_bf6ExtraInventorySize	+	iExpandInventorySize	>	dEXTRA_INVENTORY_SIZE)
				{
					sendEtcWork(eEW_EXTRA_BAG_FULL);

					return	eUIR_FAILED;
				}

				m_bf6ExtraInventorySize	+=	iExpandInventorySize;
				sendEtcWork(eEW_CHANGE_EXTRA_BAG_SIZE,m_bf6ExtraInventorySize);
				
				break;
			}

			case	eIE_MINIPET_AWAKEN50	:	//	미니펫 각성의 씨앗(50%)
				{
					cMiniPet* lpMiniPet = &m_aMiniPet[0];

					lpMiniPet->setAwaken(FALSE);
					
					break;
				}
			case	eIE_MINIPET_AWAKEN100	:	//	미니펫 각성의 열매(50%)
				{
					cMiniPet* lpMiniPet = &m_aMiniPet[0];
					
					lpMiniPet->setAwaken(TRUE);
					
					break;
				}
			case	eIE_DUEL_BUFF_FIRE		:	//	결투 버프 불
			case	eIE_DUEL_BUFF_WATER		:	//	결투 버프 물
			case	eIE_DUEL_BUFF_WIND		:	//	결투 버프 바람
			case	eIE_DUEL_BUFF_EARTH		:	//	결투 버프 대지
				if	(castDuelBuff(iEffect,iValue1)	!=	eUIR_OK)
					return	eUIR_FAILED;
				break;

			case	eIE_MINIPET_POUCH							:
			{
				if	(!iMiniPet()->unsealMiniPet(_lpItem))
					return	eUIR_FAILED;
				break;
			}
			case	eIE_RECALL_MINIPET							:
			{
				if	(!iMiniPet()->createMiniPet(_lpItem))
					return	eUIR_FAILED;
				break;
			}

			case	eIE_CH5_RESET_GUILD_SKILL_POINT				:
			{
				if	(!resetGuildSkillPoint(TRUE))
					return	eUIR_FAILED;
				break;
			}
			case	eIE_RESET_GUILD_SKILL_POINT					:
			{
				if	(!resetGuildSkillPoint())
					return	eUIR_FAILED;
				break;
			}
			
			case	eIE_ADD_TITLE	:
			{
				addTitle(iValue1,1);
				break;
			}
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃특수 효과
			case	eIE_REGEN_HP_ON_MAGIC_CARPET				:	//	양탄자위에서 체력 회복
			case	eIE_NOT_DOWN_CP_ON_MAGIC_CARPET				:	//	양탄자위에서 CP 감소 안함
			case	eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET		:	//	양탄자위에서 이동속도 증가
			case	eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET_RED	:	//	양탄자위에서 이동속도 증가
			case	eIE_FIGHT_MAGIC_CARPET						:	//	양탄자위에서 마력충전
			case	eIE_CHANGE_MAGIC_CARPET_SHAPE				:
				return	useMagicCarpetEmblem(_lpItem,iEffect);

			case	eIE_LOTTERY_ITEM				:
			{
				cItem	*lpResult	=	g_itemPackList.getRandomItem(_lpItem->m_aOption[0]);

				if	(lpResult)
				{
					_lpItem->copy(lpResult);

					if	(_lpItem->checkFirstEffect(eIE_USE_JEWEL))
					{
						_lpItem->m_bDurability	=	_lpItem->m_aOption[1];
						_lpItem->m_bCount		=	1;
					}

					sendCreateItemLog(_lpItem,eCIM_ITEM_PACK);

					return	eUIR_ROTTERY_TICKET_OK;
				}

				sendRemoveItemLog(_lpItem,eRI_LOTTERY);

				_lpItem->reset();

				return	eUIR_ROTTERY_TICKET_KWANG;
			}

			case	eIE_GS_EXPAND_PACK				://길드 마크 확장 팩
			case	eIE_GS_SPECIAL_PACK				://길드 마크 스페셜 팩
			case	eIE_GS_LUXURY_PACK				://길드 마크 럭셔리 팩
			case	eIE_GS_HANGUL_PACK				://길드 마크 한글 팩
			case	eIE_GS_ENGLISH_PACK				://길드 마크 영문 팩
			case	eIE_GS_HANMUN_PACK				://길드 마크 한문
			case	eIE_GS_ITEM_PACK				://길드 마크 아이템 팩
			case	eIE_GS_ANIMAL_PACK				://길드 마크 동물 팩
			case	eIE_GS_SIGNAL_PACK				://길드 마크 시그널 팩
			case	eIE_GS_UNIQUE1_PACK				://길드 마크 유니크1 팩
			case	eIE_GS_UNIQUE2_PACK				://길드 마크 유니크2 팩
				if	(!useGuildMarkPack(iEffect))
					return	eUIR_FAILED;
				break;

			case	eIE_GUILD_HALL_PORTAL			:
				if	(m_lpField->m_wIsBlockToWarpField)
					return eUIR_FAILED;
				if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
					return eUIR_FAILED;
				if	(!warpToGuildHall())
					return	eUIR_FAILED;
				break;

			case	eIE_CREATE_GUILD				:
				if	(!readyToMakeGuild())
					return	eUIR_FAILED;
				break;

			case	eIE_GUILD_LEVEL_UP				:	//	길드 레벨을 올려준다.
			{
				if	(!isGuildMaster())
					return	eUIR_IS_NOT_CORRECT_STATUS;

				CGuild	*lpGuild	=	getGuild();

				if	(lpGuild->m_wLevel >= iValue1)
					return	eUIR_IS_NOT_CORRECT_STATUS;

				lpGuild->makeLevel(iValue1);

				break;
			}

			case	eIE_FRONTIER_SCROLL				://	프론티어
				addTitle(eTN_FRONTIER,iValue1);
				break;

			case	eIE_TREASURE_MAP					:
				useTreasureMap();
				break;

			case	eIE_NORMAL_ITEM_LOTTERY_TICKET	:
			{
				cItem	item;

				if (iValue1	==	0)	iValue1	=	getLevel();

				for(int i=0;i<200;i++)
				{
					if (g_im.generateRandomSpecialItem(&item,iValue2,100,FALSE,FALSE))
					{
						cBasicItem	*lpBasicItem	=	item.getBasicItem();
					
						if (lpBasicItem && lpBasicItem->isEquipAbleJob(m_wJob))
						{
							int	iRequireLevel		=	lpBasicItem->m_wRequireLevel;

							if (iRequireLevel	>=	iValue1	&&	iRequireLevel	<=	iValue2)
								if (lpBasicItem->m_wKind==eIK_HELM ||lpBasicItem->m_wKind==eIK_BOOTS||lpBasicItem->m_wKind==eIK_GLOVE||
									lpBasicItem->m_wKind==eIK_NECKLACE ||lpBasicItem->m_wKind==eIK_RING||lpBasicItem->m_wKind==eIK_BELT)
								{
									_lpItem->copy(&item);

									sendCreateItemLog(_lpItem,eCIM_LOTTERY);

									return	eUIR_ROTTERY_TICKET_OK;
								}
						}
					}
				}

				{
					switch(random(3))
					{
						case	0	:
							g_im.generateItemByBaseItem(&item,63,1);
							break;

						case	1	:
							g_im.generateItemByBaseItem(&item,130,1);
							break;

						case	2	:
							g_im.generateItemByBaseItem(&item,89,1);
							break;
					}

					_lpItem->copy(&item);
					sendCreateItemLog(_lpItem,eCIM_LOTTERY);

					return	eUIR_ROTTERY_TICKET_OK;
				}

				sendRemoveItemLog(_lpItem,eRI_LOTTERY);

				_lpItem->reset();

				return	eUIR_ROTTERY_TICKET_KWANG;
			}
			case	eIE_RARE_ITEM_LOTTERY_TICKET	://	레어 아이템 복권
			{
				cItem	item;

				if (iValue1	==	0)	iValue1	=	getLevel();

				if (g_im.generateRandomSpecialItem(&item,iValue1,iValue2,TRUE,FALSE))
				{
					_lpItem->copy(&item);

					sendCreateItemLog(_lpItem,eCIM_LOTTERY);

					return	eUIR_ROTTERY_TICKET_OK;
				}

				sendRemoveItemLog(_lpItem,eRI_LOTTERY);

				_lpItem->reset();
				return	eUIR_ROTTERY_TICKET_KWANG;
			}

			case	eIE_UNIQUE_ITEM_LOTTERY_TICKET	://	유니크 아이템 복권
			{
				cItem	item;

				if (iValue1	==	0)
				{
					iValue1	=	getLevel();
					iValue1	=	max(28,iValue1);

				}

				for(int i=0;i<100;i++)
				{
					if (g_im.generateRandomSpecialItem(&item,iValue1,iValue2,FALSE,TRUE))
					{
						cBasicItem	*lpBasicItem	=	item.getBasicItem();

						if	(lpBasicItem && lpBasicItem->isEquipAbleJob(m_wJob))
						{
							int	iRequireLevel		=	item.getRequireLevel();

							if	(iRequireLevel	<=	getLevel())
							{
								_lpItem->copy(&item);

								sendCreateItemLog(_lpItem,eCIM_LOTTERY);

								return	eUIR_ROTTERY_TICKET_OK;
							}
						}
					}
				}

				{
					switch(random(3))
					{
						case	0	:
							g_im.generateItemByBaseItem(&item,308,1);
							break;

						case	1	:
							g_im.generateItemByBaseItem(&item,333,1);
							break;

						case	2	:
							g_im.generateItemByBaseItem(&item,304,1);
							break;
					}

					_lpItem->copy(&item);
					sendCreateItemLog(_lpItem,eCIM_LOTTERY);

					return	eUIR_ROTTERY_TICKET_OK;
				}

				sendRemoveItemLog(_lpItem,eRI_LOTTERY);

				_lpItem->reset();
				return	eUIR_ROTTERY_TICKET_KWANG;
			}

			case	eIE_SUPER_ITEM_LOTTERY_TICKET	://	슈퍼 유니크 아이템 복권
			{
				cItem	item;

				if (iValue1	==	0)
				{
					iValue1	=	getLevel();
					iValue1	=	max(28,iValue1);
				}

				if (g_im.generateRandomSpecialItem(&item,iValue1,iValue2,TRUE,TRUE))
				{
					_lpItem->copy(&item);

					sendCreateItemLog(_lpItem,eCIM_LOTTERY);

					return	eUIR_ROTTERY_TICKET_OK;
				}

				sendRemoveItemLog(_lpItem,eRI_LOTTERY);

				_lpItem->reset();
				return	eUIR_ROTTERY_TICKET_KWANG;
			}

			case	eIE_RESET_SKILL_POINT			://	스킬 포인트 리셋
			{
				if	(getSkillPointSum()			>	getSkillPointSumByLevel())
					return	eUIR_IS_NOT_CORRECT_STATUS;
					
				if	(!resetSkillPoint())
					return	eUIR_USELESS_STATUS_TO_USE_ITEM;

				bIsUsedEffect	=	TRUE;

				break;
			}

			case	eIE_RESET_STATE_POINT			://	스탯 초기화
			{
				if	(getCurrentStatePointSum()	>	getCorrectStatePointSum())
					return	eUIR_IS_NOT_CORRECT_STATUS;

				if	(!resetStatePoint())
					return	eUIR_USELESS_STATUS_TO_USE_ITEM;

				bIsUsedEffect	=	TRUE;
				break;
			}

			case	eIE_RESET_STRENGTH_STATE		:	//	힘 스탯 하나 재 분배
			case	eIE_RESET_AGILITY_STATE			:	//	민첩 스탯 하나 재 분배
			case	eIE_RESET_CONSTITUTION_STATE	:	//	건강 스탯 하나 재 분배
			case	eIE_RESET_INTELLIGENCE_STATE	:	//	지식 스탯 하나 재 분배
			case	eIE_RESET_WISDOM_STATE			:	//	지혜 스탯 하나 재 분배
			case	eIE_RESET_CHARISMA_STATE		:	//	카리스마 스탯 하나 재 분배
			case	eIE_RESET_LUCK_STATE			:	//	운 스탯 하나 재 분배
			{
				if	(getCurrentStatePointSum()	>	getCorrectStatePointSum())
					return	eUIR_IS_NOT_CORRECT_STATUS;

				if	(!resetOneStatePoint(iEffect-eIE_RESET_STRENGTH_STATE))
					return	eUIR_USELESS_STATUS_TO_USE_ITEM;

				bIsUsedEffect	=	TRUE;

				break;
			}

			case	eIE_MEMORY_STONE				:	//	"메모리 스톤",
			{
				if	(m_lpField->m_wIsCanNotMemoryField)
					return	eUIR_CAN_NOT_MEMORY_FIELD;

				bIsUsedEffect	=	useMemoryStone(iValue1,iValue2);

				if	(!bIsUsedEffect)
					return	eUIR_FAILED;
				break;
			}
			case	eIE_TELEPORT_STONE				:
			{
				if	(!useTeleportStone(iValue1)	&&	bIsUsedEffect	==	FALSE)
					return	eUIR_FAILED;
				bIsUsedEffect	=	TRUE;
				break;
			}

			case	eIE_PORTAL_STONE				:
			{
				if	(!usePortalStone(iValue1)	&&	bIsUsedEffect	==	FALSE)
					return	eUIR_FAILED;
				bIsUsedEffect	=	TRUE;
				break;
			}

			case	eIE_COUPLE_RING_SET				:
			{
				bIsUsedEffect	=	useCoupleRingSet(_lpItem);

				if	(!bIsUsedEffect)
					return	eUIR_FAILED;

				break;
			}


			case	eIE_FIRE_WORK									:
				if	(m_lpField->isSecretDungeon())
					return	eUIR_CAN_NOT_USE_THIS_FIELD;
				
				if	(!useFireWork(iValue1,iValue2))
					return	eUIR_INCORRECT_TIMING;
				break;

			case	eIE_REVERSIBLE_FRAME	:
			{
				if	(!iMiniPet()->extendReversalFrameTime(iValue1,TRUE))
					return	eUIR_INCORRECT_TIMING;
//	"물리 마법 구분없이 대미지가 [0]분간 [1]0％ 증가한다.",
				break;
			}
			case	eIE_INC_LAST_DAMAGE							://	최종 대미지 중가
				if	(!increaseLastDamage(iValue1,iValue2))
					return	eUIR_INCORRECT_TIMING;
//	"물리 마법 구분없이 대미지가 [0]분간 [1]0％ 증가한다.",
				break;

			case	eIE_INC_HUNT_EXP							:	//	사냥 경험치 증가
				if	(!increaseHuntExp(iValue1,iValue2))
					return	eUIR_INCORRECT_TIMING;
//	"몬스터 사냥으로 획득하는 경험치가 [0]분간 [1]％ 증가한다.",
				break;

			case	eIE_INC_HUNT_ITEM_DROP_CHANCE				:	//	사냥 경험치 증가
				if	(!increaseItemDropChance(iValue1,iValue2))
					return	eUIR_INCORRECT_TIMING;
//	"아이템 드랍 확률이 [0]분간 [1]％ 증가한다.",
				break;

			case	eIE_CLEAR_DEATH_PENELTY						:	//	죽음 페널티 회복
				if	(!clearDeathPenelty())
					return	eUIR_IS_NOT_CORRECT_STATUS;
//	"죽음 페널티를 모두 회복 시킨다.",
				break;

			case	eIE_INCREASE_MOVE_SPEED						:
				m_moveSpeed.correctValue(iValue2,iValue1*60*dSYNC_FPS);
				break;

			case	eIE_RANDOM_FIRE_WORK							:	//	랜덤 불꽃놀이
				if	(m_lpField->isSecretDungeon())
					return	eUIR_CAN_NOT_USE_THIS_FIELD;

				bIsUsedEffect	=	useFireWork();
				break;

			case	eIE_ORB_OF_SANTA								:	//	"산타의 오브",
				return	eUIR_CAN_NOT_USE_ITEM;

			case	eIE_OVER_POWER_ITEM_EQUIP						:	//	"자신 레벨보다 [0]레벨 높은 제한 레벨 아이템 사용 가능":
				return	eUIR_CAN_NOT_USE_ITEM;

			case	eIE_PERFECT_RESSURECTION						:	//	완전부활
			{
				if (!isDeath() && bIsUsedEffect == FALSE)	
					return	eUIR_IS_NOT_CORRECT_STATUS;	//	"경험치를 <c:LTGREEN>90％<n>복구하고 <c:LTGREEN>30초<n>간 최대 체력을 [0]％로 만든다.",

				bIsUsedEffect	=	TRUE;
				revive(90);
				m_maxHP.correctPercentageValue(iValue2,iValue1);
				m_iHP	=	getMaxHP();
//				addSendCureByPotion(m_iHP,
				break;
			}

			case	eIE_CALL_MAGIC_CARPET					:
				bIsUsedEffect	=	callMagicCarpet(-1, TRUE);

				if	(!bIsUsedEffect)
					return	eUIR_FAILED;
				break;

			case	eIE_INCREASE_LEVEL						:
			{
				LONGLONG	llExp	=	g_aExpTable[m_iLevel];
				llExp				=	llExp*100/m_wCorrectExp;
				increaseExperience((int)llExp+1);
				bIsUsedEffect	=	TRUE;
				break;
			}
//┃특수 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

			case	eIE_SUMMON_GUILD_GUARDIAN				:
				if	(!summonGuildGuardian(iValue1))
					return	eUSGG_FAILED;

				bIsUsedEffect		=	TRUE;
				break;

			case	eIE_RECOVER_HP							://	"체력 회복(수치)":
				if	(lpBasicItem->m_attr.isBeginnerItem && m_iLevel > 20)
				{
					iValue1		=	iValue1-(m_iLevel-20)*5;

					if (iValue1	<=	0)
						break;
				}
				bIsUsedEffect		=	TRUE;
				iValue1				*=	100;

				if	(lpBasicItem->m_wKind==	eIK_HEAL_POTION)
				{
					iMiniPet()->operateUseHealPotionWork(iValue1);

					iValue1			=	iValue1*m_wDruggistTitleFactor/100;
					iValue1			=	iValue1*_iHealEfficient/100;
				}

				if	(g_bIsDuelServer)
					iValue1	=	iValue1*g_iCorrectHealEfficientUnderDuelBattle/100;
				m_iRecoverHP		+=	iValue1;

				break;

			case	eIE_INSTANCE_RECOVER_HP_PERCENTAGE	:
				bIsUsedEffect	=	TRUE;

				if	(lpBasicItem->m_wKind	==	eIK_HEAL_POTION)
				{
					iMiniPet()->operateUseHealPotionWork(getMaxHP()*iValue1/100);

					iValue1		=	iValue1*m_wDruggistTitleFactor/100;
				}

				iValue1			=	getMaxHP()*iValue1/100;

				if	(g_bIsDuelServer)
					iValue1	=	iValue1*g_iCorrectHealEfficientUnderDuelBattle/100;

				increaseHP(iValue1);

				syncPlayerBasicStatus(TRUE);				//	대용량이므로 싱크를 바로 맞쳐준다.
				break;

			case	eIE_INSTANCE_RECOVER_HPx100	:
				bIsUsedEffect		=	TRUE;
				iValue1				*=	10000;

				if	(lpBasicItem->m_wKind==	eIK_HEAL_POTION)
				{
					iMiniPet()->operateUseHealPotionWork(iValue1);
					
					iValue1			=	iValue1*m_wDruggistTitleFactor/100;
					iValue1			=	iValue1*_iHealEfficient/100;
				}
				
				if	(g_bIsDuelServer)
					iValue1	=	iValue1*g_iCorrectHealEfficientUnderDuelBattle/100;
				m_iRecoverHP		+=	iValue1;

				syncPlayerBasicStatus(TRUE);				//	대용량이므로 싱크를 바로 맞쳐준다.
				break;

			case	eIE_DECREASE_INSTANT_HP_PERCENTAGE	:
				bIsUsedEffect	=	TRUE;
		
				iValue1			=	m_iHP*iValue1/100;

				m_iHP -= iValue1;
				m_iHP  = max(100, m_iHP);

				syncPlayerBasicStatus(TRUE);						
				break;

			case	eIE_RECOVER_HP2:

				if	(lpBasicItem->m_attr.isBeginnerItem && m_iLevel > 20)
				{
					iValue1		=	iValue1-(m_iLevel-20)*5;
					
					if (iValue1	<=	0)
						break;
				}
				bIsUsedEffect		=	TRUE;
				iValue1				*=	10000;
				
				if	(lpBasicItem->m_wKind==	eIK_HEAL_POTION)
				{
					iMiniPet()->operateUseHealPotionWork(iValue1);
					
					iValue1			=	iValue1*m_wDruggistTitleFactor/100;
					iValue1			=	iValue1*_iHealEfficient/100;
				}
				
				if	(g_bIsDuelServer)
					iValue1	=	iValue1*g_iCorrectHealEfficientUnderDuelBattle/100;
				m_iRecoverHP		+=	iValue1;
				
				syncPlayerBasicStatus(TRUE);				//	대용량이므로 싱크를 바로 맞쳐준다.
				break;
			case	eIE_RECOVER_HP_PERCENTAGE				://	"체력 회복(퍼센티지)":
				bIsUsedEffect	=	TRUE;

				if	(lpBasicItem->m_wKind	==	eIK_HEAL_POTION)
				{
					iMiniPet()->operateUseHealPotionWork(getMaxHP()*iValue1/100);

					iValue1		=	iValue1*m_wDruggistTitleFactor/100;
				}
				iValue1			=	getMaxHP()*iValue1/100;

				if	(g_bIsDuelServer)
					iValue1	=	iValue1*g_iCorrectHealEfficientUnderDuelBattle/100;
				m_iRecoverHP	+=	iValue1;
				break;

			case	eIE_RECOVER_CP							://	"CP 충전(수치)":
				if	(lpBasicItem->m_attr.isBeginnerItem && m_iLevel > 20)
				{
					iValue1		=	iValue1-(m_iLevel-20);

					if	(iValue1	<=	0)
						break;
				}

				bIsUsedEffect	=	TRUE;
				iValue1			*=	100;

				if	(lpBasicItem->m_wKind	==	eIK_CHARGING_POTION)
				{
					iMiniPet()->operateUseChargePotionWork(iValue1);

					iValue1		=	iValue1*m_wDruggistTitleFactor/100;
				}

				m_iCP			+=	iValue1;
				m_iCP			=	min(getMaxCP(),m_iCP);
				break;
			case	eIE_RECOVER_CP2							://	"CP 충전(수치)":
				if	(lpBasicItem->m_attr.isBeginnerItem && m_iLevel > 20)
				{
					iValue1		=	iValue1-(m_iLevel-20);
					
					if	(iValue1	<=	0)
						break;
				}
				
				bIsUsedEffect	=	TRUE;
				iValue1			*=	10000;
				
				if	(lpBasicItem->m_wKind	==	eIK_CHARGING_POTION)
				{
					iMiniPet()->operateUseChargePotionWork(iValue1);
					
					iValue1		=	iValue1*m_wDruggistTitleFactor/100;
				}
				
				m_iCP			+=	iValue1;
				m_iCP			=	min(getMaxCP(),m_iCP);
				syncPlayerBasicStatus(TRUE);				//	대용량이므로 싱크를 바로 맞쳐준다.
			break;
			case	eIE_CASTING_MAGIC						://마법 케스팅.
			{
				CSkill			*lpSKill	=	&g_aSkill[lpBasicItem->m_aValue[0][0]];
				cAbility		Ablilety;
				int				iRookiIndex		= m_lpField->m_skillManager.m_iRookie;
				CActiveSkill	*lpActiveSkill	= &m_lpField->m_skillManager.m_aActiveSkill[iRookiIndex];

				Ablilety.m_wSkill = lpBasicItem->m_aValue[0][0];
				Ablilety.m_wLevel = lpBasicItem->m_aValue[0][1];
				
				memcpy(&lpActiveSkill->m_ability,	&Ablilety	,sizeof(cAbility));
				lpActiveSkill->m_lpCaster		=	this;
				lpActiveSkill->m_lpTarget		=	this;
				lpActiveSkill->m_lpSkill		=	lpSKill;
				lpActiveSkill->m_wTeam			=	m_wTeam;
				lpActiveSkill->m_pos.x			=	m_pos.x;
				lpActiveSkill->m_pos.y			=	m_pos.y;
				lpActiveSkill->m_posTarget.x	=	m_pos.x;
				lpActiveSkill->m_posTarget.y	=	m_pos.y;
				lpActiveSkill->m_wRange			=	1000;

				int iResult = 0;

				cACTOR TempActor;

				memcpy(&TempActor , this , sizeof(cACTOR));

				TempActor.m_strength.m_sValue	=	lpBasicItem->m_aValue[1][0];
				TempActor.m_agility.m_sValue	=	lpBasicItem->m_aValue[1][0];
				TempActor.m_constitution.m_sValue	=	lpBasicItem->m_aValue[1][0];
				TempActor.m_intelligence.m_sValue	=	lpBasicItem->m_aValue[1][0];
				TempActor.m_wisdom.m_sValue		=	lpBasicItem->m_aValue[1][0];
				TempActor.m_charisma.m_sValue	=	lpBasicItem->m_aValue[1][0];
				TempActor.m_luck.m_sValue		=	lpBasicItem->m_aValue[1][0];

				applySkillExtraEffects(&TempActor,&Ablilety,0,m_pos.x,m_pos.y , FALSE ,TRUE);

			}
			break;

			case	eIE_RECOVER_CP_PERCENTAGE				://	"CP 충전(퍼센티지)":
				bIsUsedEffect	=	TRUE;

				if	(iValue1		==	100)
				{
					m_iCP		=	getMaxCP();
					break;
				}

				iValue1			=	getMaxCP()*iValue1/100;

				if	(lpBasicItem->m_wKind	==	eIK_CHARGING_POTION)
				{
					iMiniPet()->operateUseChargePotionWork(iValue1);
					iValue1		=	iValue1*m_wDruggistTitleFactor/100;
				}

				m_iCP			+=	iValue1;
				m_iCP			=	min(getMaxCP(),m_iCP);
				break;

			case	eIE_RECOVER_HP_CP						://	"HP,CP를 회복(수치)",
				bIsUsedEffect	=	TRUE;
				iValue1			*=	100;

				if	(lpBasicItem->m_wKind	==	eIK_CHARGING_POTION	||	lpBasicItem->m_wKind==	eIK_HEAL_POTION)
				{
					iMiniPet()->operateUseHealPotionWork(iValue1);
					iMiniPet()->operateUseChargePotionWork(iValue1);

					iValue1		=	iValue1*m_wDruggistTitleFactor/100;
				}

				if	(g_bIsDuelServer)
					iValue1	=	iValue1*g_iCorrectHealEfficientUnderDuelBattle/100;
				m_iRecoverHP	+=	iValue1;
				m_iCP			+=	iValue1;
				m_iCP			=	min(getMaxCP(),m_iCP);
				break;

			case	eIE_RECOVER_HP_CP_PERCENTAGE			://	"HP,CP를 회복(퍼센티지)",
				if	(iValue1		>=	100)
				{
					if	(m_iCP	>=	getMaxCP()	&&	m_iHP	>=	getMaxHP())
						return	eUIR_IS_NOT_CORRECT_STATUS;
				}

				bIsUsedEffect	=	TRUE;

				if	(lpBasicItem->m_wKind	==	eIK_CHARGING_POTION	||	lpBasicItem->m_wKind==	eIK_HEAL_POTION)
				{
					iMiniPet()->operateUseHealPotionWork(iValue1*getMaxHP()/100);
					iMiniPet()->operateUseChargePotionWork(iValue1*getMaxCP()/100);

					iValue1		=	iValue1*m_wDruggistTitleFactor/100;
				}

				if	(g_bIsDuelServer)
					iValue1	=	iValue1*g_iCorrectHealEfficientUnderDuelBattle/100;
				m_iRecoverHP	+=	getMaxHP()*iValue1/100;
				m_iCP			+=	getMaxCP()*iValue1/100;
				m_iCP			=	min(getMaxCP(),m_iCP);

				if	(iValue1		>=	100)
				{
					m_iCP		=	getMaxCP();
					m_iRecoverHP=	getMaxHP()-m_iHP;
				}
				break;

			case	eIE_RESURRECTION						://	"타인 부활"
				if	(!isDeath()  && bIsUsedEffect == FALSE)
					return	eUIR_IS_NOT_CORRECT_STATUS;
				bIsUsedEffect	=	TRUE;

				revive(0,iValue1);
				break;

			case	eIE_PERFECT_PARTY_REVIVE	:
			{
				if	(!perfectReviveAllPartyMember(iValue1,iValue2,_lpItem,this) && bIsUsedEffect == FALSE)
					return	eUIR_IS_NOT_CORRECT_STATUS;

				bIsUsedEffect	=	TRUE;

				break;
			}

			case	eIE_SELF_RESURRECTION					://	"본인 부활":
				if (!isDeath() && bIsUsedEffect == FALSE)
					return	eUIR_IS_NOT_CORRECT_STATUS;	//	안 뒈졌-_-는 뎁셔

				revive(0,iValue1);

				bIsUsedEffect	=	TRUE;
				break;
				
			case	eIE_HEAL_STRANGE_STATUS					://	"중화":
				if	(isStrangeStatus()  ==	FALSE)	//	차밍 상태 남은 시간
				{
					if	(bIsUsedEffect)
						break;

					if	(i	>=	dGENERATE_ITEM_DATA_COUNT-1)
						return	eUIR_IS_NOT_CORRECT_STATUS;

					if	(lpBasicItem->m_aGenerateData[i+1].m_wEffect	==	0xffff)
						return	eUIR_IS_NOT_CORRECT_STATUS;
				}
				cureStrangeStatus(iValue1);
				bIsUsedEffect		=	TRUE;
				break;

//	이상계 치료
			case	eIE_RECOVER_ALL_STRANGE_STATUS			://	"이상계 상태 치료":
				if	(isStrangeStatus()  ==	FALSE)	//	차밍 상태 남은 시간
				{
					if	(bIsUsedEffect)
						break;

					if	(i	>=	dGENERATE_ITEM_DATA_COUNT-1)
						return	eUIR_IS_NOT_CORRECT_STATUS;

					if	(lpBasicItem->m_aGenerateData[i+1].m_wEffect	==	0xffff)
						return	eUIR_IS_NOT_CORRECT_STATUS;
				}

				cureStrangeStatus();

				bIsUsedEffect		=	TRUE;
				break;

//	저하계 치료
			case	eIE_RECOVER_ALL_DECLINE_STATUS			://	"저하계 상태 치료":
				if	(isDeclinePowerStatus()	== FALSE)	//	모든 마법 저항 보정
				{
					if	(bIsUsedEffect)
						break;

					if	(i	>=	dGENERATE_ITEM_DATA_COUNT-1)
						return	eUIR_IS_NOT_CORRECT_STATUS;

					if	(lpBasicItem->m_aGenerateData[i+1].m_wEffect	==	0xffff)
						return	eUIR_IS_NOT_CORRECT_STATUS;
				}

				cureDeclinePowerStatus();

				bIsUsedEffect	=	TRUE;
				break;

//	저주계 치료
			case	eIE_RECOVER_ALL_CURSE_STATUS			://	"저주계 상태 치료":
				if	(isCurseStatus()	==	FALSE) //	피흡혈률
				{
					if	(bIsUsedEffect)
						break;

					if (i	>=	dGENERATE_ITEM_DATA_COUNT-1)
						return	eUIR_IS_NOT_CORRECT_STATUS;

					if (lpBasicItem->m_aGenerateData[i+1].m_wEffect	==	0xffff)
						return	eUIR_IS_NOT_CORRECT_STATUS;
				}

				cureCurseStatus();

				bIsUsedEffect	=	TRUE;
				break;

			case	eIE_RECOVER_ALL_BADSTATUS				://	"모든 상태 이상 치료":
				if	(isWrongStatus()	==	FALSE)
				{
					if	(bIsUsedEffect)
						break;

					if	(i	>=	dGENERATE_ITEM_DATA_COUNT-1)
						return	eUIR_IS_NOT_CORRECT_STATUS;

					if	(lpBasicItem->m_aGenerateData[i+1].m_wEffect	==	0xffff)
						return	eUIR_IS_NOT_CORRECT_STATUS;
				}

				cureStrangeStatus();
				cureDeclinePowerStatus();
				cureCurseStatus();

				bIsUsedEffect	=	TRUE;

				break;

			case	eIE_HEAL_POSION							://	"중독 치료":
			{
				m_wPoisonTime	=	0;
				bIsUsedEffect	=	TRUE;
				break;
			}

			case	eIE_SHORT_WHILE_INCREASE_POWER			://	"일시적으로 힘을 상승 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_strength.correctValue(iValue1,iValue2);
				break;
			case	eIE_SHORT_WHILE_INCREASE_POWER_BY_MINUTE://	"일시적으로 힘을 상승 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_strength.correctValue(iValue1,iValue2*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_AGILITY		://	"일시적으로 민첩성을 상승 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_agility.correctValue(iValue1,iValue2);
				break;
			case	eIE_SHORT_WHILE_INCREASE_AGILITY_BY_MINUTE	://	"일시적으로 민첩성을 상승 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_agility.correctValue(iValue1,iValue2*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_CONSTITUTION	://	"일시적으로 건강을 상승 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_constitution.correctValue(iValue1,iValue2);
				break;
			case	eIE_SHORT_WHILE_INCREASE_CONSTITUTION_BY_MINUTE	://	"일시적으로 건강을 상승 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_constitution.correctValue(iValue1,iValue2*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_WISDOM			://	"일시적으로 지혜를 상승 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_wisdom.correctValue(iValue1,iValue2);
				break;
			case	eIE_SHORT_WHILE_INCREASE_WISDOM_BY_MINUTE	://	"일시적으로 지혜를 상승 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_wisdom.correctValue(iValue1,iValue2*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_INTELIGENCE	://	"일시적으로 지식을 상승 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_intelligence.correctValue(iValue1,iValue2);
				break;
			case	eIE_SHORT_WHILE_INCREASE_INTELIGENCE_BY_MINUTE	://	"일시적으로 지식을 상승 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_intelligence.correctValue(iValue1,iValue2*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_CHARISMA		://	"일시적으로 카리스마를 상승 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_charisma.correctValue(iValue1,iValue2);
				break;
			case	eIE_SHORT_WHILE_INCREASE_CHARISMA_BY_MINUTE		://	"일시적으로 카리스마를 상승 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_charisma.correctValue(iValue1,iValue2*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_LUCK			://	"일시적으로 행운을 상승 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_luck.correctValue(iValue1,iValue2);
				break;
			case	eIE_SHORT_WHILE_INCREASE_LUCK_BY_MINUTE			://	"일시적으로 행운을 상승 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_luck.correctValue(iValue1,iValue2*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_LUCKx100_BY_MINUTE	:
				bIsUsedEffect	=	TRUE;
				m_luck.correctValue(iValue1*100,iValue2*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_ATTACK_POWER		://	"일시적으로 공격력을 증가 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_attackPower.correctPercentageValue(iValue2,iValue1);
				break;

			case	eIE_INCREASE_FINAL_DAMAGE	:
				if	(m_wIncreaseFinalDamage	>	m_wIncreaseFinalDamage)
					break;
				bIsUsedEffect				=	TRUE;
				m_wIncreaseFinalDamage		=	iValue2;
				m_wIncreaseFinalDamageTime	=	iValue1*60;
				break;

			case	eIE_SHORT_WHILE_INCREASE_ATTACK_POWER_BY_MINUTE	://	"일시적으로 공격력을 증가 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_attackPower.correctPercentageValue(iValue2,iValue1*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_ATTACK_POWER_BY_MINUTE_10	://	"일시적으로 공격력을 증가 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_attackPower.correctPercentageValue(iValue2*10,iValue1*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER	://	"일시적으로 방어력을 증가 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_defensePower.correctPercentageValue(iValue2,iValue1);
				break;
			case	eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER_BY_MINUTE	://	"일시적으로 방어력을 증가 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_defensePower.correctPercentageValue(iValue2,iValue1*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER_BY_MINUTE_10	://	"일시적으로 방어력을 증가 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_defensePower.correctPercentageValue(iValue2*10,iValue1*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_HP				://	"일시적으로 체력을 증가 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_maxHP.correctPercentageValue(iValue2,iValue1);
				break;
			case	eIE_SHORT_WHILE_INCREASE_HP_BY_MINUTE			://	"일시적으로 체력을 증가 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_maxHP.correctPercentageValue(iValue2,iValue1*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_CP				://	"일시적으로 CP를 증가 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_maxCP.correctPercentageValue(iValue2,iValue1);
				break;
			case	eIE_SHORT_WHILE_INCREASE_CP_BY_MINUTE	://	"일시적으로 CP를 증가 시킨다.":
				bIsUsedEffect	=	TRUE;
				m_maxCP.correctPercentageValue(iValue2,iValue1*60);
				break;
			case	eIE_SHORT_WHILE_SET_MAX_DAMAGE			://	"일시적으로 무기 데미지 맥스로 유지":
				bIsUsedEffect	=	TRUE;
				m_wMaintainMaxWeponDamageTime=	iValue1;
				break;
			case	eIE_SHORT_WHILE_SET_MAX_DAMAGE_BY_MINUTE	://	"일시적으로 무기 데미지 맥스로 유지":
				bIsUsedEffect	=	TRUE;
				m_wMaintainMaxWeponDamageTime=	iValue1*60;
				break;
			case	eIE_SHORT_WHILE_SET_MAX_CP				://	"일시적으로 CP 맥스로 유지":
				bIsUsedEffect	=	TRUE;
				m_wMaintainMaxCPTime		=	iValue1;
				break;
			case	eIE_SHORT_WHILE_SET_MAX_CP_BY_MINUTE	://	"일시적으로 CP 맥스로 유지":
				bIsUsedEffect	=	TRUE;
				m_wMaintainMaxCPTime		=	iValue1*60;
				break;

			case	eIE_ONE_WAY_PORTAL						:
			{
				if	(m_lpField->m_bIsGuildBattleField)
					return	eUIR_CAN_NOT_USE_THIS_FIELD;
				
				if	(m_wCurrentField	==	m_wLastVillage)
					return	eUIR_IS_NOT_CORRECT_STATUS;

				if	(m_uiLastReturnToVillageTime	>	timeGetTime()-1000)
					return	eUIR_IS_NOT_CORRECT_STATUS;
				
				if	(getRestraintTimeByBattle())
				{
					sendEtcWork(eEW_CAN_NOT_RETURN_TO_VILLAGE_DURING_BATTLE,getRestraintTimeByBattle());

					return	eUIR_IS_NOT_CORRECT_STATUS;
				}
				
				m_uiLastReturnToVillageTime	=	timeGetTime();

				bIsUsedEffect	=	TRUE;
				g_pMoveFieldUserManager->booking(this,NULL,m_wLastVillage,0xffff);
				break;
			}
			case	eIE_TRANSFORMATION_HUNTER	://	"[수치]0분 동안 레벨 [수치]1의 사냥꾼으로 변신한다.",
			case	eIE_TRANSFORMATION_THIEF	://	"[수치]0분 동안 레벨 [수치]1의 좀도둑으로 변신한다.",
			case	eIE_TRANSFORMATION_MAGICIAN	://	"[수치]0분 동안 레벨 [수치]1의 마법사로 변신한다.",
				{
					bIsUsedEffect	=	TRUE;
					transToMonster(iEffect, iValue1 * 60, iValue2);
					break;				
				}
			case	eIE_SUMMON_MONSTER	://	"[수치]0 이하의 몬스터를 소환한다."
				{
					bIsUsedEffect	=	TRUE;

					return summonMonster(iValue1);

				}

			case eIE_CARVING_LEVEL:
				{
					break;
				}
			case	eIE_ACL_SOUL_GUARD	://	"초간 방어력으로 최상으로 올린다."
				{

					cParty* lpParty = getParty();
					if(lpParty)
					{
						for(int i=0;i<lpParty->m_wMemberCount; ++i)
						{
							cACTOR	*lpActor	=	lpParty->getActorByMemberIndex(i);
							
							if	(!lpActor	||	!isOnSameField(lpActor)	||	lpActor->isDeath())
								continue;

							lpActor->m_wCarvingOptionSoulGuardTime = dACL_OPTION_SOUL_GUARD(iValue1);
						}
					}
					else
					{
						m_wCarvingOptionSoulGuardTime = dACL_OPTION_SOUL_GUARD(iValue1);
					}
					bIsUsedEffect	=	TRUE;
					break;
				}
			case	eIE_ACL_TRANSFORMATION_REPTILE :
				{
					int iLevel	=	dACL_OPTION_TRANSFORMAION_REPTILE_FORMULA1(iValue1);
					int iTime	=	dACL_OPTION_TRANSFORMAION_REPTILE_FORMULA2(iValue2) * 60;
					transToMonster(eIE_ACL_TRANSFORMATION_REPTILE , iTime, iLevel);
					bIsUsedEffect	=	TRUE;
					break;
				}
			case	eIE_ACL_INCREASE_ATTACK_POWER	://	"일시적으로 공격력을 증가 시킨다.":
				{
					int iTime			=	dACL_OPTION_INCREASE_ATTACK_POWER_FORMULA1(iValue1) * 60;
					int iAttackPower	=	dACL_OPTION_INCREASE_ATTACK_POWER_FORMULA2(iValue2);
					m_attackPower.correctPercentageValue(iAttackPower,iTime);
					bIsUsedEffect	=	TRUE;
					break;
				}
			case	eIE_ACL_HELL_PRISON:
				{

					cAbility ability;
					ability.m_wSkill = dSKILL_HELL_PRISON;
					ability.m_wLevel=	iValue1;
					applySkillExtraEffects(_lpUser,&ability,0,m_pos.x,m_pos.y , FALSE ,FALSE);
					break;
				}
			case	eIE_CHANGE_HP	:
				{
					bIsUsedEffect	=	TRUE;
					int iHP	=	iValue1 * iValue2 * 100 + 10;
					iHP	=	max(100,iHP);
					iHP	=	min(getMaxHP(), iHP);
					
					m_iHP	=	iHP;	
					syncPlayerBasicStatus(TRUE);	
					break;
				}
			case	eIE_MOVE_FIELD	:
			{
				if	(m_lpField->m_bIsGuildBattleField)
					return	eUIR_CAN_NOT_USE_THIS_FIELD;

				if	(m_lpField->m_wIsBlockToWarpField)
					return	eUIR_CAN_NOT_USE_THIS_FIELD;
				
				if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
					return eUIR_FAILED;

				if	(m_wCurrentField	==	lpBasicItem->m_wMoveFieldSerial)
					return	eUIR_IS_NOT_CORRECT_STATUS;

				if	(m_uiLastReturnToVillageTime	>	timeGetTime()-1000)
					return	eUIR_IS_NOT_CORRECT_STATUS;
				
				if	(getRestraintTimeByBattle())
				{
					sendEtcWork(eEW_CAN_NOT_RETURN_TO_VILLAGE_DURING_BATTLE,getRestraintTimeByBattle());

					return	eUIR_IS_NOT_CORRECT_STATUS;
				}
				
				m_uiLastReturnToVillageTime	=	timeGetTime();

				bIsUsedEffect	=	TRUE;
				g_pMoveFieldUserManager->booking(this,NULL,lpBasicItem->m_wMoveFieldSerial,0xffff);
				break;
			}
			default									:	//	아직 구현되지 않았어..
				return	eUIR_NOT_EMBODY_ITEM;
		}

		iEffectCount++;
	}

	return	eUIR_OK;
}

//
//	스탯 향상 아이템 사용
int
cACTOR::useImproveStateItem(cItem *_lpItem)
{
	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();
	BOOL		bIsDeath		=	FALSE;

	for (int i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	0xffff)	break;

		int	iReferenceIndex1	=	lpBasicItem->m_aGenerateData[i].m_aValue[0];
		int	iReferenceIndex2	=	lpBasicItem->m_aGenerateData[i].m_aValue[1];
		int	iValue1				=	_lpItem->m_aOption[iReferenceIndex1];
		int	iValue2				=	_lpItem->m_aOption[iReferenceIndex2];
		int	iFailChance;
		int	iRandomValue		=	random(100);

		switch(lpBasicItem->m_aGenerateData[i].m_wEffect)
		{
			case	eIE_GROW_STRENGTH						://	"힘 증가"
				iFailChance	=	100-iValue2;

				if (iRandomValue		<	iFailChance)	{bIsDeath   =   TRUE;break;}

				m_sStrength			+=	iValue1;
				m_strength.m_sValue	+=	iValue1;
				break;

			case	eIE_GROW_AGILITY						://	"민첩성 증가"
				iFailChance	=	100-iValue2;

				if (iRandomValue		<	iFailChance)	{bIsDeath	=	TRUE;break;}

				m_sAgility				+=	iValue1;
				m_agility.m_sValue		+=	iValue1;
				break;

			case	eIE_GROW_CONSTITUTION					://	"건강 증가"
				iFailChance	=	100-iValue2;

				if (iRandomValue		<	iFailChance)	{bIsDeath	=	TRUE;break;}

				m_sConstitution			+=	iValue1;
				m_constitution.m_sValue	+=	iValue1;
				break;

			case	eIE_GROW_WISDOM							://	"지혜 증가"
				iFailChance	=	100-iValue2;

				if (iRandomValue		<	iFailChance)	{bIsDeath	=	TRUE;break;}

				m_sWisdom				+=	iValue1;
				m_wisdom.m_sValue		+=	iValue1;
				break;

			case	eIE_GROW_INTELLIGENCE					://	"지식 증가"
				iFailChance	=	100-iValue2;

				if (iRandomValue		<	iFailChance)	{bIsDeath	=	TRUE;break;}

				m_sIntelligence			+=	iValue1;
				m_intelligence.m_sValue	+=	iValue1;
				break;

			case	eIE_GROW_CHARISMA						://	"카리스마 증가"
				iFailChance	=	100-iValue2;

				if (iRandomValue		<	iFailChance)	{bIsDeath	=	TRUE;break;}

				m_sCharisma				+=	iValue1;
				m_charisma.m_sValue		+=	iValue1;
				break;

			case	eIE_GROW_LUCK							://	"행운 증가"
				iFailChance	=	100-iValue2;

				if (iRandomValue		<	iFailChance)	{bIsDeath	=	TRUE;break;}

				m_sLuck					+=	iValue1;
				m_luck.m_sValue			+=	iValue1;
				break;

			case	eIE_GROW_HP								://	"체력 증가"
				iFailChance	=	iValue1*iValue2;

				if (iRandomValue		<	iFailChance)	{bIsDeath	=	TRUE;break;}

				m_iMaxHP				+=	iValue1*100;
				m_maxHP.m_iValue		+=	iValue1*100;
				break;

			case	eIE_GROW_CP								://	"CP 증가"
				iFailChance			=	iValue1*iValue2;

				if (iRandomValue	<	iFailChance)	{bIsDeath	=	TRUE;break;}

				m_iMaxCP			+=	iValue1*100;
				m_maxCP.m_iValue	+=	iValue1*100;
				break;

			default											:
				if (i == 0)	return	eUIR_NOT_EMBODY_ITEM;	//	이상한 옵션이 붙어 있다.
				break;
		}
	}

	if (bIsDeath	==	FALSE)	return	eUIR_DEATH_BY_REACTION;

	return	eUIR_OK;
}

//
//	보석 사용
int
cACTOR::useJewel(cItem *_lpItem)
{
	memcpy(&m_jewel,_lpItem,sizeof(cItem));
	m_wJewelTime	=	m_jewel.m_aOption[0]*60;

	buildPower();

	return eUIR_OK;
}

//
//	아이템 사용
int
cACTOR::useItemToCharacter(cItem *_lpItem,cACTOR *_lpTarget,BOOL _isBeltSlotItem)
{

	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();

	if	(!lpBasicItem)
		return	eUIR_FAILED;

	if	(!lpBasicItem->m_bf1IsUseToEnemy &&  !isOwnForce(_lpTarget))
		return	eUIR_INCORRECT_TARGET;

	if	(_lpItem->isBreedingRecordBook())
	{
		if(_lpTarget->m_wTransformationTime)
		{// 변신 중이당..
			if(!lpBasicItem->m_bf1IsUseItemAfterTransformation)
				return eUIR_FAILED;
		}
		if	(unsealPet(_lpItem))
		{
			arrangeInventory();

			return	eUIR_OK;
		}

		return	eUIR_FAILED;
	}
	
	if	(_lpItem->checkFirstEffect(eIE_MINIPET_POUCH))
	{
		if	(iMiniPet()->unsealMiniPet(_lpItem))
		{
			arrangeInventory();

			return	eUIR_OK;
		}

		return	eUIR_FAILED;
	}

	if	(!isUseAbleItem(_lpItem))
		return	eUIR_LACK_ABILITY;
	if	(!lpBasicItem)
		return	eUIR_FAILED;
	if	(_lpItem->m_bCount		<	1)
		return	eUIR_FAILED;

	if	(checkItemCoolTime(lpBasicItem)	==	FALSE)
	{
		int	iSlot	=	lpBasicItem->m_bCoolTimeIndex;

		sendEtcWork(eEW_SET_ITEM_COOL_TIME,iSlot,m_awItemCoolTime[iSlot]);

		return	eUIR_FAILED_BY_ITEM_COOLTIME;
	}

	if (lpBasicItem->m_attr.isDurabilityToUseCount		)	//	내구력을 아이템 카운트로 쓰는 경우
	{
		if (_lpItem->m_bDurability	==	0)
		{
			_lpItem->decreaseCount(1,_isBeltSlotItem);

			return	eUIR_FAILED;
		}
	}

	if	(lpBasicItem->m_attr.isCanNotUseableItem && _lpItem->getExpandInventorySize() == 0			)
		return	eUIR_IS_NOT_CONSUME_ITEM;		//	소비형 아이템이 아니다.

	int		iResult;

	if	(lpBasicItem->m_wKind	==	eIK_JEWEL			)
		iResult	=	_lpTarget->useJewel(_lpItem);	//	보석이다.
	else
	if	(lpBasicItem->m_wKind	==	eIK_IMPROVE_STATE	)
		iResult	=	_lpTarget->useImproveStateItem(_lpItem);	//	능력 향상제다.
	else
		iResult	=	_lpTarget->eatItem(_lpItem, 100, this);

	if	(iResult		>	eUIR_DECREASE_ITEM_COUNT_BORDER	)
		return	iResult;

	if	( !lpBasicItem->m_bf1IsNotConsume && iResult		!=	eUIR_ROTTERY_TICKET_OK)		//  소모되지않음 체크가 되어있으면 감소시키지않는다.
		decreaseItemCount(_lpItem,_isBeltSlotItem);

	setLastUseItem(lpBasicItem->m_iSerial);

	arrangeInventory();

	return	iResult;
}


BOOL
cACTOR::useMemoryStone(int _iWaypointSlot,int _iPieceCount)
{
	cItem	*lpValidSlot		=	getValidInventorySlot();

	if	(!lpValidSlot)
		return	FALSE;

	m_aWayPoint[_iWaypointSlot].m_bf12Field	=	m_lpField->m_iUniqueSerial;
	m_aWayPoint[_iWaypointSlot].m_bf10XPos	=	m_posLastMoved.x/dTILE_XS;
	m_aWayPoint[_iWaypointSlot].m_bf10YPos	=	m_posLastMoved.y/dTILE_YS;

	int		iValidInventorySlot	=	getValidInventorySlotIndex();

	g_premiumItemManager.generateTeleportStone(lpValidSlot,_iWaypointSlot);

	lpValidSlot->m_bCount=	_iPieceCount;

	sendUpdateItemData(iValidInventorySlot,TRUE);
	sendUpdateWayPointInfo(m_lpField->m_iUniqueSerial,_iWaypointSlot,m_aWayPoint[_iWaypointSlot].m_bf10XPos,m_aWayPoint[_iWaypointSlot].m_bf10YPos);

	return	TRUE;
}

BOOL
cACTOR::useTeleportStone(int _iWaypointSlot)
{
	if	(getRestraintTimeByBattle())
	{
		sendEtcWork(eEW_CAN_NOT_TELEPORT_DURING_BATTLE,getRestraintTimeByBattle());

		return	FALSE;
	}

	int	iTargetField	=	m_aWayPoint[_iWaypointSlot].m_bf12Field;
	int	iTargetPosX		=	m_aWayPoint[_iWaypointSlot].m_bf10XPos;
	int	iTargetPosY		=	m_aWayPoint[_iWaypointSlot].m_bf10YPos;

	if	(iTargetField	>=	dINSTANCE_FIELD_FLAG || g_game.isCloseMap(iTargetField))
		return	FALSE;

	teleportByInregularMethod(iTargetField,iTargetPosX*dTILE_XS,iTargetPosY*dTILE_YS);

	return	TRUE;
}

BOOL
cACTOR::usePortalStone(int _iWaypointSlot)
{
	int	iTargetField	=	m_aWayPoint[_iWaypointSlot].m_bf12Field;
	int	iTargetPosX		=	m_aWayPoint[_iWaypointSlot].m_bf10XPos;
	int	iTargetPosY		=	m_aWayPoint[_iWaypointSlot].m_bf10YPos;

	if	(iTargetField	>=	dINSTANCE_FIELD_FLAG || g_game.isCloseMap(iTargetField))
		return	FALSE;

	if	(!m_lpField->addPortal(this,iTargetField,c_iUpkeepPortalStoneTime,iTargetPosX,iTargetPosY))
		return	FALSE;

	return	TRUE;
}





BOOL
cACTOR::useCoupleRingSet(cItem *_lpItem)
{
	if	(getRemainInventorySlotCount()	<	2)
	{
		sendRegistServerMessage(eRSM_TO_USE_COUPLERING_SET_REQUIRE_TWO_VALID_INVENTORY_SLOT);
		return	FALSE;
	}
	if(!_lpItem)
		return FALSE;

	cItem				*lpValidSlot	=	getValidInventorySlot();
	cCoupleRingDefine	*lpCoupleRing1	=	(cCoupleRingDefine	*)lpValidSlot;
	int					iSlot1			=	getValidInventorySlotIndex();

	lpCoupleRing1->m_dwSerial			=	largeRandom(0x7fffffff)+100;
	lpCoupleRing1->m_wBaseItem			=	g_im.m_wPresentCoupleRing;
	lpCoupleRing1->m_bf1IsCanNotTradeItem=	FALSE;

	lpCoupleRing1->setCoupleName(m_strName);



	lpValidSlot							=	getValidInventorySlot();
	int					iSlot2			=	getValidInventorySlotIndex();
	cCoupleRingDefine	*lpCoupleRing2	=	(cCoupleRingDefine	*)lpValidSlot;

	lpCoupleRing2->m_dwSerial			=	largeRandom(0x7fffffff)+100;
	lpCoupleRing2->m_wBaseItem			=	g_im.m_wNotDefineCoupleCoupleRing;
	lpCoupleRing2->m_bf1IsCanNotTradeItem=	TRUE;
	strcpy(lpCoupleRing2->m_strCoupleName,"");

	if(_lpItem->m_year)
	{
		CTimeInfo			timeInfo;
		
		timeInfo.reset();
		timeInfo.m_wYear	=	_lpItem->m_year;
		timeInfo.m_wMonth	=	_lpItem->m_month;
		timeInfo.m_wDay		=	_lpItem->m_day;

		timeInfo.increaseDay(1);

		lpCoupleRing1->m_year	=	timeInfo.m_wYear;	
		lpCoupleRing1->m_month	=	timeInfo.m_wMonth;	
		lpCoupleRing1->m_day	=	timeInfo.m_wDay;	

		lpCoupleRing2->m_year	=	timeInfo.m_wYear;	
		lpCoupleRing2->m_month	=	timeInfo.m_wMonth;	
		lpCoupleRing2->m_day	=	timeInfo.m_wDay;
	}


	sendUpdateItemData(iSlot1);
	sendUpdateItemData(iSlot2,TRUE);

	return	TRUE;
}

BOOL
cACTOR::useTreasureMap()
{
	cPOINT	pos;

	pos.x	=	m_pos.x;
	pos.y	=	m_pos.y;

	if	(!m_lpField->getTreasureMapPoint(&pos))
	{
		sendEtcWork(eEW_TREASURE_MAP_DESTROYED_WHEN_USE);

		return	FALSE;
	}

	SG_ADD_ARCA	packet;

	packet.base.set(sizeof(packet),dSG_ADD_ARCA);

	packet.arcaInfo.m_wSerial			=	0x7fff;
	packet.arcaInfo.m_wX				=	pos.x;
	packet.arcaInfo.m_wY				=	pos.y;
	packet.arcaInfo.m_bf8SecretLevel	=	0;
	packet.arcaInfo.m_bf1IsDestroyAble	=	0;
	packet.arcaInfo.m_bf1IsLocked		=	FALSE;

	m_wTreasureBoxXPos					=	pos.x;
	m_wTreasureBoxYPos					=	pos.y;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

BOOL
cACTOR::useGuildMarkPack(int _iPack)
{
	CGuild	*lpGuild	=	getGuild();

	if	(!lpGuild)
		return	FALSE;

	if	(isGuildMaster()	==	FALSE)
	{
		sendRegistServerMessage(eRSM_GUILD_ONLY_GUILD_MASTER_CAN_OPEN_GUILD_MARK_COMPOSER);

		return	FALSE;
	}

	CGuildMarkInfo	info;

	memcpy(&info,&lpGuild->m_guildMarkInfo,sizeof(CGuildMarkInfo));

	switch(_iPack)
	{
		case	eIE_GS_EXPAND_PACK				://길드 마크 확장 팩
			info.m_ownedMarkItem.m_bf1IsOwnedExpandPack		=	TRUE;
			break;
		case	eIE_GS_SPECIAL_PACK				://길드 마크 스페셜 팩
			info.m_ownedMarkItem.m_bf1IsOwnedSpecialPack	=	TRUE;
			break;
		case	eIE_GS_LUXURY_PACK				://길드 마크 럭셔리 팩
			info.m_ownedMarkItem.m_bf1IsOwnedLuxuryPack		=	TRUE;
			break;
		case	eIE_GS_HANGUL_PACK				://길드 마크 한글 팩
			info.m_ownedMarkItem.m_bf1IsOwnedHangulPack		=	TRUE;
			break;
		case	eIE_GS_ENGLISH_PACK				://길드 마크 영문 팩
			info.m_ownedMarkItem.m_bf1IsOwnedEnglishPack	=	TRUE;
			break;
		case	eIE_GS_HANMUN_PACK				://길드 마크 한문
			info.m_ownedMarkItem.m_bf1IsOwnedHanmunPack		=	TRUE;
			break;
		case	eIE_GS_ITEM_PACK				://길드 마크 아이템 팩
			info.m_ownedMarkItem.m_bf1IsOwnedItemPack		=	TRUE;
			break;
		case	eIE_GS_ANIMAL_PACK				://길드 마크 동물 팩
			info.m_ownedMarkItem.m_bf1IsOwnedAnimalPack		=	TRUE;
			break;
		case	eIE_GS_SIGNAL_PACK				://길드 마크 한문
			info.m_ownedMarkItem.m_bf1IsOwnedSignalPack		=	TRUE;
			break;
		case	eIE_GS_UNIQUE1_PACK				://길드 마크 아이템 팩
			info.m_ownedMarkItem.m_bf1IsOwnedUnique1Pack		=	TRUE;
			break;
		case	eIE_GS_UNIQUE2_PACK				://길드 마크 동물 팩
			info.m_ownedMarkItem.m_bf1IsOwnedUnique2Pack		=	TRUE;
			break;
	}

	g_game.sendWPSelectGuildMark(lpGuild->m_wSerial,&info,TRUE);

	return	TRUE;
}