#include "CActor.H"
#include "CHero.H"
#include "CItemEffect.H"
#include "itemSet.H"
#include "cMessage.H"
#include "pieceItem.H"
#include "cWindowInterface.H"


void
CHero::receiveOtherPlayerItemData(char *_lpstrId,char *_lpstrName)
{
	strcpy(m_strOtherPlayerId,_lpstrId);
	strcpy(m_strOtherPlayerName,_lpstrName);

	m_bf1IsReceiveOtherPlayerItemData	=	TRUE;

	OpenInventoryWindow();
}

void
CHero::resetOtherPlayerItemData()
{
	m_bf1IsReceiveOtherPlayerItemData	=	FALSE;
}

//
//	아이템 사용
BOOL
CHero::useItem(int _iItemSlot,int _iTarget,BOOL _isBeltSlotItem)
{
	cItem		*lpItem;

	if	(_isBeltSlotItem)
		lpItem	=	getBeltItem(_iItemSlot);
	else
		lpItem	=	getInventoryItem(_iItemSlot);

	if	(!lpItem)
	{
		s_agent.sendAskItemData();
		return	FALSE;
	}

	if	(lpItem->checkFirstEffect(eIE_MINIPET_POUCH))
	{
		lpItem->reset();

		arrangeInventory();

		return	FALSE;
	}

	if	(lpItem->isBreedingRecordBook())
	{
		lpItem->reset();

		arrangeInventory();

		return	TRUE;
	}
	cBasicItem	*lpBasicItem=	lpItem->getBasicItem();
	if	(_iTarget	!=	m_iSerial)
	{
		if(!lpBasicItem->m_bf1IsNotConsume )
			decreaseItemCount(lpItem,_isBeltSlotItem);

		return	TRUE;
	}

	if	(lpBasicItem->m_wKind	==	eIK_JEWEL			)
		return	useJewel(lpItem,_isBeltSlotItem);	//	보석이다.

	useNormalItem(lpItem,_isBeltSlotItem);

	return	FALSE;
}

BOOL
CHero::useItemToItem(int _iInventorySlot,int _iTargetItem)
{
	cItem		*lpItem			=	getInventoryItem(_iInventorySlot);
	cItem		*lpTargetItem	=	getInventoryItem(_iTargetItem);

	if	(lpItem)
		lpItem->decreaseCount(1);

	if	(!lpItem || !lpTargetItem)
		return	FALSE;

	g_hero.addTwinkleItem(_iTargetItem);

	return	TRUE;
}


void
CHero::decreaseItemCount(cItem *_lpItem,BOOL _bIsBeltItem)
{
	cBasicItem	*lpBasicItem=	_lpItem->getBasicItem();

	if (_lpItem->m_bCount	>	0)
	{
		if (lpBasicItem->m_attr.isDurabilityToUseCount)
		{
			_lpItem->m_bDurability--;

			if	(_lpItem->m_bDurability	==	0)
				_lpItem->decreaseCount(1,_bIsBeltItem);
		}
		else
			_lpItem->decreaseCount(1,_bIsBeltItem);
	}
}

//
//	효과 처리
void
CHero::operateEquipmentBasicData(int _iEffect,int _iValue1,int _iValue2)
{
	switch(_iEffect)
	{
		case	eIE_CH5_BONUS_HP_AMULET			:
			m_maxHP.m_sCorrectValueByPrefix	+=	_iValue1;
			break;
				
		case	eIE_CH5_BONUS_CP_AMULET			:
			m_maxCP.m_sCorrectValueByPrefix	+=	_iValue1;
			break;

		case	eIE_CH5_OVER_POWER_ITEM_EQUIP	:
			m_wLimitEquipmentLevelOverPointByCH5	+=	_iValue1;
			break;

		case	eIE_PS_SIGNBOARD1						:
			m_bf1IsOwnedPitchmanShopSignBoard1	=	TRUE;
			break;
		case	eIE_PS_SIGNBOARD2						:
			m_bf1IsOwnedPitchmanShopSignBoard2	=	TRUE;
			break;
		case	eIE_PS_SIGNBOARD3						:
			m_bf1IsOwnedPitchmanShopSignBoard3	=	TRUE;
			break;
		case	eIE_PS_SIGNBOARD4						:
			m_bf1IsOwnedPitchmanShopSignBoard4	=	TRUE;
			break;
		case	eIE_PS_SIGNBOARD5						:
			m_bf1IsOwnedPitchmanShopSignBoard5	=	TRUE;
			break;
		case	eIE_PS_LOUD_SPEAKER						:
			m_bf1IsOwnedPitchmanShopLoudSpeaker	=	TRUE;
			break;
		case	eIE_PS_COLOR_TEXT						:
			m_bf1IsOwnedPitchmanColorPen		=	TRUE;
			break;
		case	eIE_PS_BOLD_TEXT						:
			m_bf1IsOwnedPitchmanShopBoldPen		=	TRUE;
			break;
		case	eIE_PS_EXPAND_TEXT_BUFFER				:
			m_bf1IsOwnedPitchmanShopLongPen		=	TRUE;
			break;
		case	eIE_PS_TWINKLE							:
			m_bf1IsOwnedPitchmanShopTwinkle		=	TRUE;
			break;
		case	eIE_PS_PACK								:
			m_bf1IsOwnedPitchmanShopSignBoard1	=	TRUE;
			m_bf1IsOwnedPitchmanShopSignBoard2	=	TRUE;
			m_bf1IsOwnedPitchmanShopSignBoard3	=	TRUE;
			m_bf1IsOwnedPitchmanShopSignBoard4	=	TRUE;
			m_bf1IsOwnedPitchmanShopSignBoard5	=	TRUE;
			m_bf1IsOwnedPitchmanShopLoudSpeaker	=	TRUE;
			m_bf1IsOwnedPitchmanColorPen		=	TRUE;
			m_bf1IsOwnedPitchmanShopBoldPen		=	TRUE;
			m_bf1IsOwnedPitchmanShopLongPen		=	TRUE;
			m_bf1IsOwnedPitchmanShopTwinkle		=	TRUE;
			break;

		//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		//┃특수 능력
		case	eIE_OVER_POWER_ITEM_EQUIP				:
			m_wLimitEquipmentLevelOverPoint	=	max(_iValue1,m_wLimitEquipmentLevelOverPoint);
			break;

		case	eIE_CHANGE_MAGIC_CARPET_SHAPE			:
			if	(m_wMagicCarpetShape == 0)
				m_wMagicCarpetShape		=	1;
			break;
		//┃특수 능력
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

		//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		//┃기본 수치
		case	eIE_ADD_DEFENSE_POWER					:	m_defensePower.m_iValue	+=	_iValue1*100;break;//	"방어력"
		case	eIE_BELT_STACK_COUNT					:	m_wBeltStackCount		+=	_iValue1;break;//	"벨트 스택 아이템수"
		//┃기본 수치
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

		//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		//┃각종 부가 효과
		case	eIE_DECREASE_RELOAD_TIME				:	addCorrectReloadSpeed(_iValue1);break;//	"벨트에 아이템 채우는 시간 감소"
		case	eIE_INCREASE_TRANSFORMATION_SPEED		:	addCorrectTransformationSpeed(_iValue1);break;//	"변신 속도 빨라짐"
		case	eIE_INCREASE_RECOVER_SPEED				:	addCorrectHPRecoverSpeed(_iValue1);break;//	"물약에 의한 회복 속도 증가"
		case	eIE_RETURN_DAMAGE						:	addReturnDamageRate(_iValue1);break;//	"가시돋힘"
		case	eIE_RETURN_DAMAGE_40					:	addReturnDamageRate(40);break;//	"가시돋힘"
		case	eIE_INFINITY_BULLET						:	m_bIsInfinityBullet	=	TRUE;break;//	"탄환 리젠 주기"
		case	eIE_INCREASE_SITDOWN_RECOVER_HP_SPEED	:	m_wIncreaseSitdownHPRecoverSpeed = _iValue1;break;
		//┃각종 부가 효과
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

		//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		//┃각종 수치 증가
		case	eIE_INCREASE_SELECT_POWER				:	//	선택한 능력치 하나를 증가 시킴
		{
			switch(_iValue2)
			{
				case	0	:	//	힘
					m_strength.addValue(_iValue1);
					break;
				case	1	:	//	민첩성
					m_agility.addValue(_iValue1);
					break;
				case	2	:	//	건강
					m_constitution.addValue(_iValue1);
					break;
				case	3	:	//	카리스마
					m_charisma.addValue(_iValue1);
					break;
				case	4	:	//	지식
					m_intelligence.addValue(_iValue1);
					break;
				case	5	:	//	지혜
					m_wisdom.addValue(_iValue1);
					break;
				case	6	:	//	행운
					m_luck.addValue(_iValue1);
					break;
			}
			break;
		}

		case	eIE_INCREASE_MELEE_JOB_ATTACK_POWER		:	if (c_aIsMelee[m_wJob]){m_iCorrectMinimumDamage	+=	_iValue1*100,m_iCorrectMaximumDamage	+=	_iValue2*100;}break;
		case	eIE_INCREASE_POWER						:	m_strength.addValue(_iValue1);break;//	"힘 상승"
		case	eIE_INCREASE_CONSTITUTION				:	m_constitution.addValue(_iValue1);break;//	"건강 상승"
		case	eIE_INCREASE_WISDON						:	m_wisdom.addValue(_iValue1);break;//	"지혜 상승"
		case	eIE_INCREASE_INTELIGENCE				:	m_intelligence.addValue(_iValue1);break;//	"지식 상승"
		case	eIE_INCREASE_AGILITY					:	m_agility.addValue(_iValue1);break;//	"민첩성 상승"
		case	eIE_INCREASE_CHARISMA					:	m_charisma.addValue(_iValue1);break;//	"카리스마 상승"
		case	eIE_INCREASE_LUCK						:	m_luck.addValue(_iValue1);break;//	"행운 상승"
		case	eIE_INCREASE_CRITICAL					:	m_bCorrectCriticalHitChance	+=	_iValue1;break;//	"크리티컬 발생확률"
		case	eIE_INCREASE_CRUSH_SHOT					:	m_bCorrectCrushBlowChance	+=	_iValue1;break;//	"결정타 발생확률"
		case	eIE_INCREASE_HIT_RATE					:	m_hitChance.addValue(_iValue1);break;//	"명중률 증가"
		case	eIE_INCREASE_DODGE_CHANCE				:	m_dodgeChance.addValue(_iValue1);break;//	"명중률 증가"
		case	eIE_INCREASE_CONCENTRATION				:	m_concentration.addValue(_iValue1,dMAXIMUM_CONCENTRATION_VALUE);break;//	"집중력 증가"
		case	eIE_INCREASE_ATTACK_SPEED				:	m_actionSpeed.addValue(_iValue1);break;//	"공격 속도 증가"
		case	eIE_INCREASE_FIX_SELECT_POWER				:	//	선택한 능력치 하나를 증가 시킴(고정)
			{
				switch(_iValue2)
				{
				case	0	:	//  0이면 옵션이 없는 것! 옵션을 선택할 수 있다.
					break;
				case	1	:	//	힘
					m_strength.addValue(_iValue1);
					break;
				case	2	:	//	민첩성
					m_agility.addValue(_iValue1);
					break;
				case	3	:	//	건강
					m_constitution.addValue(_iValue1);
					break;
				case	4	:	//	카리스마
					m_charisma.addValue(_iValue1);
					break;
				case	5	:	//	지식
					m_intelligence.addValue(_iValue1);
					break;
				case	6	:	//	지혜
					m_wisdom.addValue(_iValue1);
					break;
				case	7	:	//	행운
					m_luck.addValue(_iValue1);
					break;
				}
				break;
			}
		//┃각종 수치 증가
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


		//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		//┃저항
		case	eIE_INCREASE_FIRE_RESITANCE				:	m_fireResistance.addValue(_iValue1);break;//	"불저항"
		case	eIE_INCREASE_TERRA_RESITANCE			:	m_earthResistance.addValue(_iValue1);break;//	"대지저항"
		case	eIE_INCREASE_WIND_RESITANCE				:	m_windResistance.addValue(_iValue1);break;//	"바람저항"
		case	eIE_INCREASE_WATER_RESITANCE			:	m_waterResistance.addValue(_iValue1);break;//	"물저항"
		case	eIE_INCREASE_LIGHT_RESITANCE			:	m_lightResistance.addValue(_iValue1);break;//	"빛저항"
		case	eIE_INCREASE_DARK_RESITANCE				:	m_darkResistance.addValue(_iValue1);break;//	"어둠저항"
		case	eIE_INCREASE_MAGIC_RESITANCE			:	m_allMagicResistance.addValue(_iValue1);break;//	"마법 속성 공격에 대한 저항"
		case	eIE_INCREASE_FOUR_ELEMENTAL_RESITANCE	:	
			m_fireResistance.addValue(_iValue1);
			m_waterResistance.addValue(_iValue1);
			m_windResistance.addValue(_iValue1);
			m_earthResistance.addValue(_iValue1);
			break;
		//┃저항
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

		//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		//┃스킬레벨
		case	eIE_INCREASE_MAGIC_LANCER_SKILL			:	if (m_wJob	==	dJOB_MAGIC_LANCER	)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"마법 창병 스킬 레벨 증가"
		case	eIE_INCREASE_MAGIC_ARCHER_SKILL			:	if (m_wJob	==	dJOB_MAGIC_ARCHER	)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"마법 궁수 스킬 레벨 증가"
		case	eIE_INCREASE_KNIGHT_SKILL				:	if (m_wJob	==	dJOB_KNIGHT			)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"기사 스킬 레벨 증가"
		case	eIE_INCREASE_WARRIOR_SKILL				:	if (m_wJob	==	dJOB_WARRIOR		)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"전사 계열 스킬 레벨 증가"
		case	eIE_INCREASE_WIZARD_SKILL				:	if (m_wJob	==	dJOB_WIZARD			)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"마법사 스킬 레벨 증가"
		case	eIE_INCREASE_WEREWOLF_SKILL				:	if (m_wJob	==	dJOB_WEREWOLF		)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"늑대인간 스킬 레벨 증가"
		case	eIE_INCREASE_THIEF_SKILL				:	if (m_wJob	==	dJOB_ROGUE			)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"도둑 스킬 레벨 증가"
		case	eIE_INCREASE_FIGHTER_SKILL				:	if (m_wJob	==	dJOB_FIGHTER		)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"격투가 스킬 레벨 증가"
		case	eIE_INCREASE_BEAST_TAMER_SKILL			:	
			if (m_wJob	==	dJOB_BEAST_TAMER	)	
				m_correctAllSkillLevel.addValue(_iValue1);break;//	"조련사 스킬 레벨 증가"
		case	eIE_INCREASE_SUMMONER_SKILL				:	if (m_wJob	==	dJOB_SUMMONER		)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"소환사 스킬 레벨 증가"
		case	eIE_INCREASE_PRINCESSE_SKILL			:	if (m_wJob	==	dJOB_PRINCESS		)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"공주 스킬 레벨 증가"
		case	eIE_INCREASE_MAGICAL_GIRL_SKILL			:	if (m_wJob	==	dJOB_MAGICAL_GIRL	)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"변신소녀 스킬 레벨 증가"
		case	eIE_INCREASE_PRIEST_SKILL				:	if (m_wJob	==	dJOB_PRIEST			)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"사제 스킬 레벨 증가"
		case	eIE_INCREASE_FALLEN_ANGEL_SKILL			:	if (m_wJob	==	dJOB_FALLEN_ANGEL	)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"타락천사 스킬 레벨 증가"
		case	eIE_INCREASE_NECROMANCER_SKILL			:	if (m_wJob	==	dJOB_NECROMANCER	)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"네크로멘서 스킬 레벨 증가"
		case	eIE_INCREASE_DEMON_SKILL				:	if (m_wJob	==	dJOB_DEVIL			)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"악마 스킬 레벨 증가"
		case	eIE_INCREASE_SOUL_BRINGER_SKILL			:	if (m_wJob	==	dJOB_SOUL_BRINGER	)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"영술사 스킬 레벨 증가"
		case	eIE_INCREASE_FEMALE_SKILL_LEVEL			:	if (!c_aIsMale[m_wJob]				)	m_correctAllSkillLevel.addValue(_iValue1);break;	//	"여성 캐릭터들의 스킬 레벨 증가"
		case	eIE_INCREASE_MALE_SKILL_LEVEL			:	if (c_aIsMale[m_wJob]				)	m_correctAllSkillLevel.addValue(_iValue1);break;	//	"남성 캐릭터들의 스킬 레벨 증가"
		case	eIE_INCREASE_SKILL_LEVEL				:	
			m_correctAllSkillLevel.addValue(_iValue1);break;//	"모든 스킬 레벨 증가"

		//┃스킬레벨
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

		case	eIE_WEAKNING_TARGET_FIRE_RESISTANCE		:
			m_weakningTargetFireResistance.addValue(_iValue1);
			break;
		case	eIE_WEAKNING_TARGET_WATER_RESISTANCE	:
			m_weakningTargetWaterResistance.addValue(_iValue1);
			break;
		case	eIE_WEAKNING_TARGET_WIND_RESISTANCE		:
			m_weakningTargetWindResistance.addValue(_iValue1);
			break;
		case	eIE_WEAKNING_TARGET_EARTH_RESISTANCE	:
			m_weakningTargetEarthResistance.addValue(_iValue1);
			break;
		case	eIE_WEAKNING_TARGET_LIGHT_RESISTANCE	:
			m_weakningTargetLightResistance.addValue(_iValue1);
			break;
		case	eIE_WEAKNING_TARGET_DARK_RESISTANCE		:
			m_weakningTargetDarkResistance.addValue(_iValue1);
			break;
		case	eIE_WEAKNING_TARGET_MAGIC_RESISTANCE	:
			m_weakningTargetMagicResistance.addValue(_iValue1);
			break;
		case	eIE_STRENGTHENING_FIRE_DAMAGE			:
			m_strengtheningFireDamage.addValue(_iValue1,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
			break;
		case	eIE_STRENGTHENING_WATER_DAMAGE			:
			m_strengtheningWaterDamage.addValue(_iValue1,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
			break;
		case	eIE_STRENGTHENING_WIND_DAMAGE			:
			m_strengtheningWindDamage.addValue(_iValue1,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
			break;
		case	eIE_STRENGTHENING_EARTH_DAMAGE			:
			m_strengtheningEarthDamage.addValue(_iValue1,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
			break;
		case	eIE_STRENGTHENING_LIGHT_DAMAGE			:
			m_strengtheningLightDamage.addValue(_iValue1,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
			break;
		case	eIE_STRENGTHENING_DARK_DAMAGE			:
			m_strengtheningDarkDamage.addValue(_iValue1,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
			break;
		case	eIE_STRENGTHENING_MAGIC_DAMAGE			:
			m_strengtheningMagicDamage.addValue(_iValue1,dMAXIMUM_MAGIC_STRENGTHENING_VALUE);
			break;

		case	eIE_ADD_DAMAGE_TO_UNDEAD				:	addAddDamageToUndead(_iValue1);break;//	"언데드 사냥꾼"
		case	eIE_ADD_DAMAGE_TO_DEMON					:	addAddDamageToDemon(_iValue1);break;//	"악마 사냥꾼"
		case	eIE_ADD_DAMAGE_TO_HUMAN					:	addAddDamageToHuman(_iValue1);break;//	"인간 사냥꾼"
		case	eIE_ADD_DAMAGE_TO_ANIMAL				:	addAddDamageToAnimal(_iValue1);break;//	"동물 사냥꾼"
		case	eIE_ADD_DAMAGE_TO_HOLY_ANIMAL			:	addAddDamageToHolyAnimal(_iValue1);break;//	"신수 사냥꾼"
		case eIE_INCREASE_ALL_STATUS					:
			{
				m_strength.addValue(_iValue1);			//	"힘 상승"
				m_constitution.addValue(_iValue1);		//	"건강 상승"
				m_wisdom.addValue(_iValue1);			//	"지혜 상승"
				m_intelligence.addValue(_iValue1);		//	"지식 상승"
				m_agility.addValue(_iValue1);			//	"민첩성 상승"
				m_charisma.addValue(_iValue1);			//	"카리스마 상승"
				m_luck.addValue(_iValue1);				//	"행운 상승"
				break;
			}
		case	eIE_ACL_INCREASE_ALL_STATUS				:
			{
				int iValue = dACL_OPTION_INCREASE_ALL_STATUS(_iValue1);
				m_strength.addValue(iValue);			//	"힘 상승"
				m_constitution.addValue(iValue);		//	"건강 상승"
				m_wisdom.addValue(iValue);			//	"지혜 상승"
				m_intelligence.addValue(iValue);		//	"지식 상승"
				m_agility.addValue(iValue);			//	"민첩성 상승"
				m_charisma.addValue(iValue);			//	"카리스마 상승"
				m_luck.addValue(iValue);				//	"행운 상승"
				break;
			}
		case	eIE_ACL_INCREASE_MAX_HP				:
			{
				m_iCarvingOptionMaxHP		=	dACL_OPTION_INCREASE_MAX_HP_FORMULA1(_iValue1) * 100;
				break;
			}
		case	eIE_ADD_MAX_HPx100 :
			{
				m_maxHP.m_sCorrectValueByPrefix+=	_iValue1*100;
				
				break;
			}
		case	eIE_ADD_MAX_CPx100 :
			{
				m_maxCP.m_sCorrectValueByPrefix+=	_iValue1*100;
				break;
			}
		case	eIE_ADD_DEFENSE_PERCENTAGE :
			{
				m_defensePower.m_sCorrectPercentageValueByPrefix+=	_iValue1;
				break;
			}
		case	eIE_ADD_MAX_HP_PERCENTAGE :
			{
				m_maxHP.m_sCorrectPercentageValueByPrefix+=	_iValue1;
				break;
			}
		case	eIE_ADD_MAX_CP_PERCENTAGE :
			{
				m_maxCP.m_sCorrectPercentageValueByPrefix+=	_iValue1;
				break;
			}
	}
}

void
CHero::operatePrefixData(int _iEffect,int _iValue1,int _iValue2,int _iValue3,BOOL _bIsUnique)
{
	int	iMax_magic_strengthening_value	=	dMAXIMUM_MAGIC_STRENGTHENING_VALUE;

	if	(_bIsUnique)
		iMax_magic_strengthening_value	=	dMAXIMUM_MAGIC_STRENGTHENING_VALUE_FOR_UNIQUE;

	switch(_iEffect)
	{
		case	eIP_CONTROL_SKILL_LEVEL_BY_DIFFICULTY	: // 레벨별 스킬/마법 레벨 조절
			m_aCorrectLevel[_iValue1-1].addValue(_iValue2);
			break;
		case	eIP_INCREASE_SUMMON_BEAST_STATUS		: // 소환수 스탯 증가
			m_correctSummonBeastState.addValue(_iValue1);
			break;
		case	eIP_INCREASE_GAIN_EXP_FOR_PET			: // 펫 경험치 획득 증가
			m_bonusPetExp.addValue(_iValue1);
			break;

		case	eIP_ADD_DAMAGE_TO_UNDEAD				:	addAddDamageToUndead(_iValue1);break;//	"언데드 사냥꾼"
		case	eIP_ADD_DAMAGE_TO_DEMON					:	addAddDamageToDemon(_iValue1);break;//	"악마 사냥꾼"
		case	eIP_ADD_DAMAGE_TO_HUMAN					:	addAddDamageToHuman(_iValue1);break;//	"인간 사냥꾼"
		case	eIP_ADD_DAMAGE_TO_ANIMAL				:	addAddDamageToAnimal(_iValue1);break;//	"동물 사냥꾼"
		case	eIP_ADD_DAMAGE_TO_HOLY_ANIMAL			:	addAddDamageToHolyAnimal(_iValue1);break;//	"신수 사냥꾼"

		case	eIP_DECREASE_STRENGTH					:	m_strength.addValue(-_iValue1);break;//	힘 증감
		case	eIP_DECREASE_AGILITY					:	m_agility.addValue(-_iValue1);break;//	민첩성 증감
		case	eIP_DECREASE_CONSTITUTION				:	m_constitution.addValue(-_iValue1);break;//	건강 증감
		case	eIP_DECREASE_WISDOM						:	m_wisdom.addValue(-_iValue1);break;//	지혜 증감
		case	eIP_DECREASE_INTELIGENCE				:	m_intelligence.addValue(-_iValue1);break;//	지식 증감
		case	eIP_DECREASE_CHARISMA					:	m_charisma.addValue(-_iValue1);break;//	카리스마 증감
		case	eIP_DECREASE_LUCK						:	m_luck.addValue(-_iValue1);break;//	운 증감

		case	eIP_DECLINE_MOVE_SPEED					:	m_moveSpeed.addValue(-_iValue1);break;//	이동 속도
		case	eIP_DECLINE_ACTION_SPEED				:	m_actionSpeed.addValue(-_iValue1);break;//	공격 속도

		case	eIP_ADD_STRENGTH						:	m_strength.addValue(_iValue1);break;//	힘 증감
		case	eIP_ADD_AGILITY							:	m_agility.addValue(_iValue1);break;//	민첩성 증감
		case	eIP_ADD_CONSTITUTION					:	m_constitution.addValue(_iValue1);break;//	건강 증감
		case	eIP_ADD_WISDOM							:	m_wisdom.addValue(_iValue1);break;//	지혜 증감
		case	eIP_ADD_INTELIGENCE						:	m_intelligence.addValue(_iValue1);break;//	지식 증감
		case	eIP_ADD_CHARISMA						:	m_charisma.addValue(_iValue1);break;//	카리스마 증감
		case	eIP_ADD_LUCK							:	m_luck.addValue(_iValue1);break;//	운 증감
		case	eIP_FIXING_STRENGTH						:	m_strength.fixValue(_iValue1);break;//	힘 고정
		case	eIP_FIXING_AGILITY						:	m_agility.fixValue(_iValue1);break;//	민첩성 고정
		case	eIP_FIXING_CONSTITUTION					:	m_constitution.fixValue(_iValue1);break;//	건강 고정
		case	eIP_FIXING_WISDOM						:	m_wisdom.fixValue(_iValue1);break;//	지혜 고정
		case	eIP_FIXING_INTELLIGENCE					:	m_intelligence.fixValue(_iValue1);break;//	지식 고정
		case	eIP_FIXING_CHARISMA						:	m_charisma.fixValue(_iValue1);break;//	카리스마 고정
		case	eIP_FIXING_LUCK							:	m_luck.fixValue(_iValue1);break;//	운 고정
		case	eIP_ADD_STRENGTH_BY_LEVEL				:	m_strength.addValue(_iValue2*(getLevel()/_iValue1));break;//	레벨 비례 힘 증감
		case	eIP_ADD_AGILITY_BY_LEVEL				:	m_agility.addValue(_iValue2*(getLevel()/_iValue1));break;//	레벨 비례 민첩성 증감
		case	eIP_ADD_CONSTITUTION_BY_LEVEL			:	m_constitution.addValue(_iValue2*(getLevel()/_iValue1));break;//	레벨 비례 건강 증감
		case	eIP_ADD_WISDOM_BY_LEVEL					:	m_wisdom.addValue(_iValue2*(getLevel()/_iValue1));break;//	레벨 비례 지혜 증감
		case	eIP_ADD_INTELIGENCE_BY_LEVEL			:	m_intelligence.addValue(_iValue2*(getLevel()/_iValue1));break;//	레벨 비례 지식 증감
		case	eIP_ADD_CHARISMA_BY_LEVEL				:	m_charisma.addValue(_iValue2*(getLevel()/_iValue1));break;//	레벨 비례 카리스마 증감
		case	eIP_ADD_LUCK_BY_LEVEL					:	m_luck.addValue(_iValue2*(getLevel()/_iValue1));break;//	레벨 비례 운 증감

		case	eIP_ADD_MIN_DAMAGE						:	m_iCorrectMinimumDamage	+=	_iValue1*100;break;//	무기 데미지 증감 (최소값)
		case	eIP_ADD_MAX_DAMAGE						:	m_iCorrectMaximumDamage	+=	_iValue1*100;break;//	무기 데미지 증감 (최대값)

		case	eIP_ADD_DAMAGE_PERCENTAGE				:	m_attackPower.m_sCorrectPercentageValueByPrefix	+=	_iValue1;break;//	무기 데미지 증감 (퍼센티지)

		case	eIP_DECLINE_DEFENSE_PERCENTAGE			:	m_defensePower.m_sCorrectPercentageValueByPrefix-=	_iValue1;break;//	방어력 증감 (퍼센티지)
		case	eIP_DECLINE_DEFENSE						:	m_defensePower.m_sCorrectValueByPrefix-=_iValue1;break;//	방어력 증감 (수치)
		case	eIP_ADD_DEFENSE_PERCENTAGE				:	m_defensePower.m_sCorrectPercentageValueByPrefix+=	_iValue1;break;//	방어력 증감 (퍼센티지)
		case	eIP_ADD_DEFENSE							:	m_defensePower.m_sCorrectValueByPrefix+=_iValue1;break;//	방어력 증감 (수치)
		case	eIP_ADD_MAX_HP_PERCENTAGE				:	m_maxHP.m_sCorrectPercentageValueByPrefix+=	_iValue1;break;//	최대 체력 증감 (퍼센티지)
		case	eIP_ADD_MAX_HP							:	m_maxHP.m_sCorrectValueByPrefix+=	_iValue1;break;//	최대 체력 증감 (수치)
		case	eIP_ADD_MAX_CP_PERCENTAGE				:	m_maxCP.m_sCorrectPercentageValueByPrefix+=	_iValue1;break;//	최대 CP 증감 (퍼센티지)
		case	eIP_ADD_MAX_CP							:	m_maxCP.m_sCorrectValueByPrefix+=	_iValue1;break;//	최대 CP 증감 (수치)

		case	eIP_DECLINE_HIT_RATE					:	m_hitChance.addValue(-_iValue1);break;//	공격 성공 확률 (명중률)
		case	eIP_DECLINE_DODGE_RATE					:	m_dodgeChance.addValue(-_iValue1);break;//	공격 성공 확률 (명중률)

		case	eIP_CORRECT_HIT_RATE					:	m_hitChance.addValue(_iValue1);break;//	공격 성공 확률 (명중률)
		case	eIP_CORRECT_DODGE_RATE					:	m_dodgeChance.addValue(_iValue1);break;//	공격을 피할 확률 (회피율)

		case	eIP_LOW_CP								:	m_wBottomCP+=_iValue1;break;//	이동시 감소 한계 CP
		case	eIP_IGNORE_TARGET_DODGE_ABILITY			:	m_isIgnoreTargetCorrectDodgeChance	=	TRUE;break;//	적 방어자의 회피 보정치 무시
		case	eIP_IGNORE_ATTACKER_CORRECT_HIT_VALUE	:	m_isIgnoreTargetCorrectHitChance	=	TRUE;break;//	적 공격자의 명중 보정치 무시
		case	eIP_PERFECT_HIT							:	m_isPerfectHit	=	TRUE;break;//	항상 명중
		case	eIP_BLOCKING_SPEED						:	m_iCorrectRecoverBlocking+=_iValue1;break;//	블러킹 속도

		case	eIP_ADD_CONCENTRATION					:	m_concentration.addValue(_iValue1,dMAXIMUM_CONCENTRATION_VALUE);break;//CP 감소 속도 조절 (집중력)
		case	eIP_MOVE_SPEED							:	m_moveSpeed.addValue(_iValue1);break;//	이동 속도
		case	eIP_ACTION_SPEED						:	m_actionSpeed.addValue(_iValue1);break;//	공격 속도

		case	eIP_INSTANT_KILL						:	addInstanceKillChance(_iValue1);break;//	즉사
		case	eIP_WEAPON_BREAKER						:	addMakeWeaponBreak(_iValue1,_iValue2);break;//	무기 파괴
		case	eIP_ARMOR_BREAKER						:	addMakeArmorBreak(_iValue1,_iValue2);break;//	갑옷 파괴
		case	eIP_MAKE_COLD							:	addMakeCold(_iValue1,_iValue2);break;//	콜드 상태
		case	eIP_MAKE_FREEZE							:	addMakeFreeze(_iValue1,_iValue2);break;//	프리즈 상태
		case	eIP_MAKE_BLIND							:	addMakeBlind(_iValue1,_iValue2);break;//	장님 상태
		case	eIP_MAKE_STUN							:	addMakeStun(_iValue1,_iValue2);break;//스턴 상태
		case	eIP_MAKE_STONE							:	addMakeStone(_iValue1,_iValue2);break;//	석화 상태
		case	eIP_MAKE_CONFUSE						:	addMakeConfuse(_iValue1,_iValue2);break;//	혼란 상태
		case	eIP_MAKE_SLEEP							:	addMakeSleep(_iValue1,_iValue2);break;//	잠 상태
		case	eIP_MAKE_CHARMING						:	addMakeCharmed(_iValue1,_iValue2);break;//	차밍 상태
		case	eIP_MAKE_FIRST_TARGET					:	addMakeFirstTarget(_iValue1,_iValue2);break;//	우선 타겟이 된 상태
		case	eIP_MAKE_AI_DOWN						:	addMakeSimpleton(_iValue1,_iValue2);break;//	AI 저하 상태
		case	eIP_MAKE_BERSERK						:	addMakeBerserk(_iValue1,_iValue2);break;//	버서크 상태
		case	eIP_RESISTANCE_FIRE						:	m_fireResistance.addValue(_iValue1);break;//	"불저항"
		case	eIP_RESISTANCE_TERRA					:	m_earthResistance.addValue(_iValue1);break;//	"대지저항"
		case	eIP_RESISTANCE_WIND						:	m_windResistance.addValue(_iValue1);break;//	"바람저항"
		case	eIP_RESISTANCE_WATER					:	m_waterResistance.addValue(_iValue1);break;//	"물저항"
		case	eIP_RESISTANCE_LIGHT					:	m_lightResistance.addValue(_iValue1);break;//	"빛저항"
		case	eIP_RESISTANCE_DARK						:	m_darkResistance.addValue(_iValue1);break;//	"어둠저항"
		case	eIP_RESISTANCE_MAGIC					:	m_allMagicResistance.addValue(_iValue1);break;//	"마법 속성 공격에 대한 저항"


		case	eIP_ABSORB_FIRE							:	addAbsorbFireDamage(_iValue1);break;//	불 속성 공격 흡수
		case	eIP_ABSORB_WATER						:	addAbsorbWaterDamage(_iValue1);break;//	물 속성 공격 흡수
		case	eIP_ABSORB_WIND							:	addAbsorbWindDamage(_iValue1);break;//	바람 속성 공격 흡수
		case	eIP_ABSORB_TERRA						:	addAbsorbEarthDamage(_iValue1);break;//	대지 속성 공격 흡수
		case	eIP_ABSORB_LIGHT						:	addAbsorbLightDamage(_iValue1);break;//	빛 속성 공격 흡수
		case	eIP_ABSORB_DARK							:	addAbsorbDarkDamage(_iValue1);break;//	어둠 속성 공격 흡수
		case	eIP_ABSORB_MAGIC						:	addAbsorbMagicDamage(_iValue1);break;//	모든 마법 속성 공격 흡수
		case	eIP_RESISTANCE_FREEZE					:	addFreezeResistance(_iValue1);break;//	프리즈 공격에 대한 저항
		case	eIP_RESISTANCE_COLD						:	addColdResistance(_iValue1);break;//	콜드 공격에 대한 저항
		case	eIP_RESISTANCE_STUN						:	addStunResistance(_iValue1);break;//	스턴 공격에 대한 저항
		case	eIP_RESISTANCE_CONFUSE					:	addConfuseResistance(_iValue1);break;//	혼란 공격에 대한 저항
		case	eIP_RESISTANCE_CHARMING					:	addCharmingResistance(_iValue1);break;//	차밍 공격에 대한 저항
		case	eIP_RESISTANCE_STONE					:	addStoneResistance(_iValue1);break;//	석화 공격에 대한 저항
		case	eIP_RESISTANCE_INSTANCE_KILL			:	addInstanceKillResistance(_iValue1);break;//	즉사 공격에 대한 저항
		case	eIP_RESISTANCE_POISON					:	addPoisonResistance(_iValue1);break;//	중독 공격에 대한 저항
		case	eIP_RESISTANCE_SLEEP					:	addSleepResistance(_iValue1);break;//	잠 공격에 대한 저항
		case	eIP_RESISTANCE_CURSE_STATUS				:	addCurseResistance(_iValue1);break;//"저주 상태에 대한 저항"
		case	eIP_RESISTANCE_DECLINE_STATUS			:	addDeclinePowerResistance(_iValue1);break;//"저하 상태에 대한 저항"
		case	eIP_RESISTANCE_STRANGE_STATUS			:	addStrangeStatusResistance(_iValue1);break;//"이상 상태에 대한 저항"
		case	eIP_RESISTANCE_ALL_BAD_STATUS			:	addAllBadStatusResistance(_iValue1);break;//	"모든 상태 이상에 대한 저항"
		case	eIP_RESISTANCE_KNOCK_BACK				:	addKnockoutResistance(_iValue1);break;//	녹백/넘어짐 공격에 대한 저항
		case	eIP_RESISTANCE_CRITICAL_SHOT			:	addCriticalHitResistance(_iValue1);break;//	치명타 공격에 대한 저항
		case	eIP_RESISTANCE_CRUSH_SHOT				:	addCrushBlowResistance(_iValue1);break;//	결정타 공격에 대한 저항
		case	eIP_RETURN_DAMAGE						:	addReturnDamageRate(_iValue1);break;//	받은 데미지 일부 돌려 줌
		case	eIP_DAMAGE_TO_CP						:	addDamageToCPRate(_iValue1);break;//	받은 데미지 CP 변환

		case	eIP_SHIELD_BLOCK_RATE					:	addBlockingChance(_iValue1);break;//	"방패 블럭률"
		case	eIP_CRITICAL_HIT						:	addCorrectCriticalHitChance(_iValue1);break;//	치명타 발동 확률
		case	eIP_CRUSH_SHOT							:	addCorrectCrushBlowChance(_iValue1);break;//	치명타 발동 확률//	결정타 발동 확률
		case	eIP_POTION_HEAL_SPEED					:	addCorrectHPRecoverSpeed(_iValue1+50);break;//	물약 회복 속도 조절
		case	eIP_POTION_HEAL_SPEED2					:	addCorrectHPRecoverSpeed(_iValue1);break;//	물약 회복 속도 조절
		case	eIP_STEAL_HP							:	addLifeStealPecentage(_iValue1);break;//	체력 스틸
		case	eIP_BONUS_CP							:
		{
			if	(g_bIsGuildBattleField)
				_iValue1	=	_iValue1*dPVP_BONUS_CP_FACTOR/100;

			addGetBonusCPPercentage(_iValue1);

			break;//	보너스 CP
		}
		case	eIP_FLEE								:	addTargetFleeChance(_iValue1);break;//	도망
		case	eIP_KNOCK_OUT							:	addCorrectKnockoutAttackChance(_iValue1);break;//	녹아웃
		case	eIP_FIRST_AID							:	addFirstAidPercentage(_iValue1);break;//	타격 응급 처치
		case	eIP_CHANGE_WEAPON_SPEED					:	addCorrectTransformationSpeed(_iValue1);break;//	무기 교환 속도
		case	eIP_REGENERATION_HP						:	addRegenHPPerSecond(_iValue1*10);break;//	체력 리제네레이션
		case	eIP_RESURRECTION_CHANCE					:	addResurrectionChance(_iValue1);break;//	부활 확률
		case	eIP_INCREASE_RARE_ITEM_DROP_CHANCE		:	addCorrectRareItemDropRate(_iValue1);break;//	마법 아이템 확률 증가
		case	eIP_INCREASE_UNIQUE_ITEM_DROP_CHANCE	:	addCorrectUniqueItemDropRate(_iValue1);break;//	유니크 아이템 확률 증가
		case	eIP_DECREASE_RELOAD_TIME				:	addCorrectReloadSpeed(_iValue1);break;//	"벨트에 아이템 채우는 시간 감소"

		case	eIP_MAKE_FREEZE_ATTACKER				:	if (m_bReturnFreezeChance<_iValue1)	m_bReturnFreezeChance=_iValue1,m_wReturnFreezeTime=_iValue2;break;//	공격자 프리즈
		case	eIP_MAKE_COLD_ATTACKER					:	if (m_bReturnColdChance<_iValue1)	m_bReturnColdChance=_iValue1,m_wReturnColdTime=_iValue2;break;//	공격자 콜드
		case	eIP_JOB_SKILL_LEVEL						:	if (m_wJob==_iValue2)	m_correctAllSkillLevel.addValue(_iValue1);break;//	계열 스킬/마법 레벨 조절
		case	eIP_ALL_SKILL_LEVEL						:	
			m_correctAllSkillLevel.addValue(_iValue1);
			break;//	모든 스킬/마법 레벨 조절
	
		case	eIP_INFINITY_BULLET						:	m_bIsInfinityBullet=TRUE;break;//	탄환 생성
		case	eIP_MAGIC_BULLET						:	m_isMagicBullet=TRUE;break;//	마법 탄환
		case	eIP_AUTO_RELOAD							:	m_isAutoReload=TRUE;break;//	벨트 자동 채우기

		case	eIP_BLURRED								:	m_sBlurTime=-1;break;//	흐릿한 상태 (Blurred)
		case	eIP_INVISIBLE							:	m_sInvisivilityTime=-1;break;//	투명 상태 (Invisible)
		case	eIP_LEVITATE							:	m_sFloatTime=-1;break;//	공중 부양
		case	eIP_NOT_AIMED							:	m_sNonTargetTime=-1;break;//	타겟에서 제외

		case	eIP_FIRE_DAMAGE							:	addAddFireDamage(_iValue1,_iValue2);break;//	불 속성 공격
		case	eIP_WATER_DAMAGE						:	addAddWaterDamage(_iValue1,_iValue2);addMakeCold(100,_iValue3/dSYNC_FPS);break;//	물 속성 공격
		case	eIP_WIND_DAMAGE							:	addAddWindDamage(_iValue1,_iValue2);addMakeStun(100,_iValue3);break;//	바람 속성 공격
		case	eIP_TERRA_DAMAGE						:	addAddPoisonDamage(_iValue1,_iValue2,_iValue3);break;//	"중독시킨다."

		case	eIP_LIGHT_DAMAGE						:	//	빛 속성 공격
		{
			addAddLightDamage(_iValue1,_iValue2);

			m_wTargetDeclineHitChanceTime	=	max(m_wTargetDeclineHitChanceTime,_iValue3);
			m_wTargetDeclineDodgeChanceTime	=	max(m_wTargetDeclineDodgeChanceTime,_iValue3);
			break;
		}
		case	eIP_DARK_DAMAGE							:		//	어둠 속성 공격
		{	
			addAddDarkDamage(_iValue1,_iValue2);
			m_wTargetCurseAmplificationTime=	max(m_wTargetDeclineHitChanceTime,_iValue3);
			break;
		}
		// VV  09.08.07 마법추가데미지 ~DX
		case	eIP_FIRE_DAMAGE_DX						:	addAddNaturalDamageDX(eADD_FIRE_DAMAGE_DX,_iValue1,_iValue2);break;//	불 속성 공격
		case	eIP_WATER_DAMAGE_DX						:	addAddNaturalDamageDX(eADD_WATER_DAMAGE_DX,_iValue1,_iValue2);addMakeCold(100,_iValue3/dSYNC_FPS);break;//	물 속성 공격
		case	eIP_WIND_DAMAGE_DX						:	addAddNaturalDamageDX(eADD_WIND_DAMAGE_DX,_iValue1,_iValue2);addMakeStun(100,_iValue3);break;//	바람 속성 공격
		case	eIP_TERRA_DAMAGE_DX						:	addAddPoisonDamage(_iValue1,_iValue2,_iValue3);break;//	"중독시킨다."

		case	eIP_LIGHT_DAMAGE_DX						:	//	빛 속성 공격
		{
			addAddNaturalDamageDX(eADD_LIGHT_DAMAGE_DX,_iValue1,_iValue2);

			m_wTargetDeclineHitChanceTime	=	max(m_wTargetDeclineHitChanceTime,_iValue3);
			m_wTargetDeclineDodgeChanceTime	=	max(m_wTargetDeclineDodgeChanceTime,_iValue3);
			break;
		}
		case	eIP_DARK_DAMAGE_DX							:		//	어둠 속성 공격
		{	
			addAddNaturalDamageDX(eADD_DARK_DAMAGE_DX,_iValue1,_iValue2);
			m_wTargetCurseAmplificationTime=	max(m_wTargetDeclineHitChanceTime,_iValue3);
			break;
		}
	// AA  09.08.07 마법추가데미지 ~DX

		case	eIP_CHARGE_SKILL						:	//	스킬/마법 충전
			break;

		case	eIP_WEAKNING_TARGET_FIRE_RESISTANCE		:
			m_weakningTargetFireResistance.addValue(_iValue1);
			break;
		case	eIP_WEAKNING_TARGET_WATER_RESISTANCE	:
			m_weakningTargetWaterResistance.addValue(_iValue1);
			break;
		case	eIP_WEAKNING_TARGET_WIND_RESISTANCE		:
			m_weakningTargetWindResistance.addValue(_iValue1);
			break;
		case	eIP_WEAKNING_TARGET_EARTH_RESISTANCE	:
			m_weakningTargetEarthResistance.addValue(_iValue1);
			break;
		case	eIP_WEAKNING_TARGET_LIGHT_RESISTANCE	:
			m_weakningTargetLightResistance.addValue(_iValue1);
			break;
		case	eIP_WEAKNING_TARGET_DARK_RESISTANCE		:
			m_weakningTargetDarkResistance.addValue(_iValue1);
			break;
		case	eIP_WEAKNING_TARGET_MAGIC_RESISTANCE	:
			m_weakningTargetMagicResistance.addValue(_iValue1);
			break;
		case	eIP_STRENGTHENING_FIRE_DAMAGE			:
			m_strengtheningFireDamage.addValue(_iValue1,iMax_magic_strengthening_value);
			break;
		case	eIP_STRENGTHENING_WATER_DAMAGE			:
			m_strengtheningWaterDamage.addValue(_iValue1,iMax_magic_strengthening_value);
			break;
		case	eIP_STRENGTHENING_WIND_DAMAGE			:
			m_strengtheningWindDamage.addValue(_iValue1,iMax_magic_strengthening_value);
			break;
		case	eIP_STRENGTHENING_EARTH_DAMAGE			:
			m_strengtheningEarthDamage.addValue(_iValue1,iMax_magic_strengthening_value);
			break;
		case	eIP_STRENGTHENING_LIGHT_DAMAGE			:
			m_strengtheningLightDamage.addValue(_iValue1,iMax_magic_strengthening_value);
			break;
		case	eIP_STRENGTHENING_DARK_DAMAGE			:
			m_strengtheningDarkDamage.addValue(_iValue1,iMax_magic_strengthening_value);
			break;
		case	eIP_STRENGTHENING_MAGIC_DAMAGE			:
			m_strengtheningMagicDamage.addValue(_iValue1,iMax_magic_strengthening_value);
			break;
		case eIP_INCREASE_ALL_STATUS	:
			{	
				m_strength.addValue(_iValue1);			//	힘 증감
				m_agility.addValue(_iValue1);			//	민첩성 증감
				m_constitution.addValue(_iValue1);		//	건강 증감
				m_wisdom.addValue(_iValue1);			//	지혜 증감
				m_intelligence.addValue(_iValue1);		//	지식 증감
				m_charisma.addValue(_iValue1);			//	카리스마 증감
				m_luck.addValue(_iValue1);				//	운 증감
				break;
			}
	}
}

//
//	효과 처리
BOOL
CHero::useNormalItem(cItem *_lpItem,BOOL _bIsBeltItem)
{
	int		i,iReferenceIndex1,iReferenceIndex2,iValue1,iValue2;
	int		iEffectCount	=	0;
	BOOL	bIsUsedEffect	=	FALSE;

	cBasicItem	*lpItem	=	_lpItem->getBasicItem();

	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		int	iEffect	=	lpItem->m_aGenerateData[i].m_wEffect;

		if	(iEffect	==	0xffff)
			break;

		iReferenceIndex1	=	lpItem->m_aGenerateData[i].m_aValue[0];
		iReferenceIndex2	=	lpItem->m_aGenerateData[i].m_aValue[1];
		iValue1				=	_lpItem->m_aOption[iReferenceIndex1];
		iValue2				=	_lpItem->m_aOption[iReferenceIndex2];

		switch(iEffect)
		{
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃특수 효과
			case	eIE_RESET_SKILL_POINT			://	스킬 포인트 리셋
				resetSkillPoint();
				break;

			case	eIE_RESET_STATE_POINT			://	스탯 초기화
				resetStatePoint();
				break;

			case	eIE_RESET_STRENGTH_STATE		:	//	힘 스탯 하나 재 분배
			case	eIE_RESET_AGILITY_STATE			:	//	민첩 스탯 하나 재 분배
			case	eIE_RESET_CONSTITUTION_STATE	:	//	건강 스탯 하나 재 분배
			case	eIE_RESET_INTELLIGENCE_STATE	:	//	지식 스탯 하나 재 분배
			case	eIE_RESET_WISDOM_STATE			:	//	지혜 스탯 하나 재 분배
			case	eIE_RESET_CHARISMA_STATE		:	//	카리스마 스탯 하나 재 분배
			case	eIE_RESET_LUCK_STATE			:	//	운 스탯 하나 재 분배
			{
	 			resetOneStatePoint(iEffect-eIE_RESET_STRENGTH_STATE);
				break;
			}

//┃특수 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

			case	eIE_PERFECT_PARTY_REVIVE	:
			{
				m_maxHP.correctPercentageValue(iValue2,iValue1);
				m_maxCP.correctPercentageValue(iValue2,iValue1);

				break;
			}


			case	eIE_INSTANCE_RECOVER_HP_PERCENTAGE	:
				if	(lpItem->m_wKind	==	eIK_HEAL_POTION)
					iValue1		=	iValue1*m_wDruggistTitleFactor/100;

				iValue1			=	getMaxHP()*iValue1/100;

				m_iHP			+=	iValue1;
				m_iHP			=	min(m_iHP,getMaxHP());
				break;

			case	eIE_INSTANCE_RECOVER_HPx100	:
				iValue1		*=	10000;

				if(lpItem->m_wKind	==	eIK_HEAL_POTION)
					iValue1			=	iValue1*m_wDruggistTitleFactor/100;

				m_iRecoverHP+=	iValue1;
				break;


			case	eIE_RECOVER_HP							://	"체력 회복(수치)":
				if (lpItem->m_attr.isBeginnerItem && m_iLevel > 20)
				{
					iValue1		=	iValue1-(m_iLevel-20)*5;

					if (iValue1	<=	0)
						break;
				}

				iValue1		*=	100;

				if(lpItem->m_wKind	==	eIK_HEAL_POTION)
					iValue1			=	iValue1*m_wDruggistTitleFactor/100;

				m_iRecoverHP+=	iValue1;
				break;

			case	eIE_RECOVER_HP2							://	"체력 회복(수치)":
				if (lpItem->m_attr.isBeginnerItem && m_iLevel > 20)
				{
					iValue1		=	iValue1-(m_iLevel-20)*5;
					
					if (iValue1	<=	0)
						break;
				}
				
				iValue1		*=	10000;
				
				if(lpItem->m_wKind	==	eIK_HEAL_POTION)
					iValue1			=	iValue1*m_wDruggistTitleFactor/100;
				
				m_iRecoverHP+=	iValue1;
				break;
			

			case	eIE_RECOVER_HP_PERCENTAGE				://	"체력 회복(퍼센티지)":
				if(lpItem->m_wKind	==	eIK_HEAL_POTION)
					iValue1			=	iValue1*m_wDruggistTitleFactor/100;

				m_iRecoverHP	+=	getMaxHP()*iValue1/100;
				break;

			case	eIE_RECOVER_CP							://	"CP 충전(수치)":
				if (lpItem->m_attr.isBeginnerItem && m_iLevel > 20)
				{
					iValue1		=	iValue1-(m_iLevel-20);

					if (iValue1	<=	0)
						break;
				}

				iValue1		*=	100;

				if(lpItem->m_wKind	==	eIK_CHARGING_POTION)
					iValue1		=	iValue1*m_wDruggistTitleFactor/100;

				m_iCP		+=	iValue1;
				m_iCP		=	min(getMaxCP(),m_iCP);
				break;

			case	eIE_RECOVER_CP2							://	"CP 충전(수치)":
				if (lpItem->m_attr.isBeginnerItem && m_iLevel > 20)
				{
					iValue1		=	iValue1-(m_iLevel-20);
					
					if (iValue1	<=	0)
						break;
				}
				
				iValue1		*=	100;
				
				if(lpItem->m_wKind	==	eIK_CHARGING_POTION)
					iValue1		=	iValue1*m_wDruggistTitleFactor/100;
				
				m_iCP		+=	iValue1;
				m_iCP		=	min(getMaxCP(),m_iCP);
				break;

			case	eIE_RECOVER_CP_PERCENTAGE				://	"CP 충전(퍼센티지)":
				if (iValue1		==	100)
				{
					m_iCP		=	getMaxCP();
					break;
				}

				if(lpItem->m_wKind	==	eIK_CHARGING_POTION)
					iValue1			=	iValue1*m_wDruggistTitleFactor/100;

				m_iCP		+=	getMaxCP()*iValue1/100;
				m_iCP		=	min(getMaxCP(),m_iCP);
				break;

			case	eIE_RECOVER_HP_CP						://	"HP,CP를 회복(수치)",
				iValue1		*=	100;

				if(lpItem->m_wKind	==	eIK_CHARGING_POTION)
					iValue1		=	iValue1*m_wDruggistTitleFactor/100;

				m_iRecoverHP+=	iValue1;
				m_iCP		+=	iValue1;
				m_iCP		=	min(getMaxCP(),m_iCP);
				break;
			case	eIE_DECREASE_INSTANT_HP_PERCENTAGE	:
				bIsUsedEffect	=	TRUE;

				iValue1			=	m_iHP*iValue1/100;
				
				m_iHP -= iValue1;
				m_iHP  = max(100, m_iHP);
							
				break;

			case	eIE_RECOVER_HP_CP_PERCENTAGE			://	"HP,CP를 회복(퍼센티지)",
				if(lpItem->m_wKind	==	eIK_CHARGING_POTION)
					iValue1		=	iValue1*m_wDruggistTitleFactor/100;
				m_iRecoverHP+=	getMaxHP()*iValue1/100;
				m_iCP		+=	getMaxCP()*iValue1/100;
				m_iCP		=	min(getMaxCP(),m_iCP);

				if (iValue1		>=	100)
				{
					m_iCP		=	getMaxCP();
					m_iRecoverHP=	getMaxHP()-m_iHP;
				}
				break;

			case	eIE_RESURRECTION						://	"타인 부활"
//				if (!isDeath())	return	FALSE;
				break;

			case	eIE_SELF_RESURRECTION					://	"본인 부활":
//				if (!isDeath())	return	FALSE;	//	안 뒈졌-_-는 뎁셔
//	부활~~
				break;
			case	eIE_HEAL_STRANGE_STATUS					://	"중화":
				cureStrangeStatus(iValue1);
				bIsUsedEffect	=	TRUE;
				break;

//	이상계 치료
			case	eIE_RECOVER_ALL_STRANGE_STATUS			://	"이상계 상태 치료":
				cureStrangeStatus();
				bIsUsedEffect		=	TRUE;
				break;

//	저하계 치료
			case	eIE_RECOVER_ALL_DECLINE_STATUS			://	"저하계 상태 치료":
				cureDeclinePowerStatus();
				bIsUsedEffect	=	TRUE;
				break;

//	저주계 치료
			case	eIE_RECOVER_ALL_CURSE_STATUS			://	"저주계 상태 치료":
				cureCurseStatus();
				bIsUsedEffect	=	TRUE;
				break;

			case	eIE_RECOVER_ALL_BADSTATUS				://	"모든 상태 이상 치료":
			{
				cureStrangeStatus();
				cureDeclinePowerStatus();
				cureCurseStatus();
				bIsUsedEffect	=	TRUE;
				break;
			}

			case	eIE_HEAL_POSION							://	"중독 치료":
			{
				m_wPoisonTime	=	0;
				break;
			}

			case	eIE_SHORT_WHILE_INCREASE_POWER			://	"일시적으로 힘을 상승 시킨다.":
				m_strength.correctValue(iValue1,iValue2);
				break;
			case	eIE_SHORT_WHILE_INCREASE_POWER_BY_MINUTE://	"일시적으로 힘을 상승 시킨다.":
				m_strength.correctValue(iValue1,iValue2*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_AGILITY		://	"일시적으로 민첩성을 상승 시킨다.":
				m_agility.correctValue(iValue1,iValue2);
				break;
			case	eIE_SHORT_WHILE_INCREASE_AGILITY_BY_MINUTE		://	"일시적으로 민첩성을 상승 시킨다.":
				m_agility.correctValue(iValue1,iValue2*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_CONSTITUTION	://	"일시적으로 건강을 상승 시킨다.":
				m_constitution.correctValue(iValue1,iValue2);
				break;
			case	eIE_SHORT_WHILE_INCREASE_CONSTITUTION_BY_MINUTE	://	"일시적으로 건강을 상승 시킨다.":
				m_constitution.correctValue(iValue1,iValue2*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_WISDOM			://	"일시적으로 지혜를 상승 시킨다.":
				m_wisdom.correctValue(iValue1,iValue2);
				break;
			case	eIE_SHORT_WHILE_INCREASE_WISDOM_BY_MINUTE	://	"일시적으로 지혜를 상승 시킨다.":
				m_wisdom.correctValue(iValue1,iValue2*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_INTELIGENCE	://	"일시적으로 지식을 상승 시킨다.":
				m_intelligence.correctValue(iValue1,iValue2);
				break;
			case	eIE_SHORT_WHILE_INCREASE_INTELIGENCE_BY_MINUTE	://	"일시적으로 지식을 상승 시킨다.":
				m_intelligence.correctValue(iValue1,iValue2*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_CHARISMA		://	"일시적으로 카리스마를 상승 시킨다.":
				m_charisma.correctValue(iValue1,iValue2);
				break;
			case	eIE_SHORT_WHILE_INCREASE_CHARISMA_BY_MINUTE	://	"일시적으로 카리스마를 상승 시킨다.":
				m_charisma.correctValue(iValue1,iValue2*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_LUCK			://	"일시적으로 행운을 상승 시킨다.":
				m_luck.correctValue(iValue1,iValue2);
				break;
			case	eIE_SHORT_WHILE_INCREASE_LUCK_BY_MINUTE	://	"일시적으로 행운을 상승 시킨다.":
				m_luck.correctValue(iValue1,iValue2*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_LUCKx100_BY_MINUTE	:
				m_luck.correctValue(iValue1*100,iValue2*60);
				break;

			case	eIE_SHORT_WHILE_INCREASE_ATTACK_POWER	://	"일시적으로 공격력을 증가 시킨다.":
				m_attackPower.correctPercentageValue(iValue2,iValue1);
				break;
			case	eIE_SHORT_WHILE_INCREASE_ATTACK_POWER_BY_MINUTE	://	"일시적으로 공격력을 증가 시킨다.":
				m_attackPower.correctPercentageValue(iValue2,iValue1*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_ATTACK_POWER_BY_MINUTE_10	://	"일시적으로 공격력을 증가 시킨다.":
				m_attackPower.correctPercentageValue(iValue2*10,iValue1*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER	://	"일시적으로 방어력을 증가 시킨다.":
				m_defensePower.correctPercentageValue(iValue2,iValue1);
				break;
			case	eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER_BY_MINUTE	://	"일시적으로 방어력을 증가 시킨다.":
				m_defensePower.correctPercentageValue(iValue2,iValue1*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_DEFENSE_POWER_BY_MINUTE_10	://	"일시적으로 방어력을 증가 시킨다.":
				m_defensePower.correctPercentageValue(iValue2*10,iValue1*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_HP				://	"일시적으로 체력을 증가 시킨다.":
				m_maxHP.correctPercentageValue(iValue2,iValue1);
				break;
			case	eIE_SHORT_WHILE_INCREASE_HP_BY_MINUTE	://	"일시적으로 체력을 증가 시킨다.":
				m_maxHP.correctPercentageValue(iValue2,iValue1*60);
				break;
			case	eIE_SHORT_WHILE_INCREASE_CP				://	"일시적으로 CP를 증가 시킨다.":
				m_maxCP.correctPercentageValue(iValue2,iValue1);
				break;
			case	eIE_SHORT_WHILE_INCREASE_CP_BY_MINUTE	://	"일시적으로 CP를 증가 시킨다.":
				m_maxCP.correctPercentageValue(iValue2,iValue1*60);
				break;
			case	eIE_SHORT_WHILE_SET_MAX_DAMAGE			://	"일시적으로 무기 대미지 맥스로 유지":
				m_wMaintainMaxWeponDamageTime=	iValue1;
				break;
			case	eIE_SHORT_WHILE_SET_MAX_DAMAGE_BY_MINUTE	://	"일시적으로 무기 대미지 맥스로 유지":
				m_wMaintainMaxWeponDamageTime=	iValue1*60;
				break;
			case	eIE_SHORT_WHILE_SET_MAX_CP				://	"일시적으로 CP 맥스로 유지":
				m_wMaintainMaxCPTime		=	iValue1;
				break;
			case	eIE_SHORT_WHILE_SET_MAX_CP_BY_MINUTE	://	"일시적으로 CP 맥스로 유지":
				m_wMaintainMaxCPTime		=	iValue1*60;
				break;

			case	eIE_PERFECT_RESSURECTION				:	//	완전부활
			{
				if (!g_lpHero->isDeath())
					break;;	//	"경험치를 <c:LTGREEN>90％<n>복구하고 <c:LTGREEN>30초<n>간 최대 체력을 [0]％로 만든다.",
				g_lpHero->revive();
				revive();
				m_maxHP.correctPercentageValue(iValue2,iValue1);
				m_iHP			=	getMaxHP();
				g_lpHero->m_iHP	=	m_iHP;
				break;
			}
			case	eIE_CASTING_MAGIC						:	//

				break;

			case	eIE_TRANSFORMATION_HUNTER	://	"[수치]0분 동안 레벨 [수치]1의 사냥꾼으로 변신한다.",
			case	eIE_TRANSFORMATION_THIEF	://	"[수치]0분 동안 레벨 [수치]1의 좀도둑으로 변신한다.",				
			case	eIE_TRANSFORMATION_MAGICIAN	://	"[수치]0분 동안 레벨 [수치]1의 마법사로 변신한다.",

				transToMonster(iEffect, iValue1* 60, iValue2);
				break;
			case	eIE_ACL_TRANSFORMATION_REPTILE :
				{
					int iLevel	=	dACL_OPTION_TRANSFORMAION_REPTILE_FORMULA1(iValue1);
					int iTime	=	dACL_OPTION_TRANSFORMAION_REPTILE_FORMULA2(iValue2) * 60;
					transToMonster(eIE_ACL_TRANSFORMATION_REPTILE , iTime, iLevel);
					break;
				}
			case	eIE_ACL_INCREASE_ATTACK_POWER	://	"일시적으로 공격력을 증가 시킨다.":
				{
					int iTime			=	dACL_OPTION_INCREASE_ATTACK_POWER_FORMULA1(iValue1) * 60;
					int iAttackPower	=	dACL_OPTION_INCREASE_ATTACK_POWER_FORMULA2(iValue2);
					m_attackPower.correctPercentageValue(iAttackPower,iTime);
					break;
				}
		}
	}

	if(!lpItem->m_bf1IsNotConsume)		// 사용시 소모되지 않음.
		decreaseItemCount(_lpItem,_bIsBeltItem);

	return	TRUE;
}

void
CHero::transToMonster(WORD	_wEffect, WORD _wTime,	WORD _wLevel)
{
	switch(_wEffect)
	{
	case eIE_TRANSFORMATION_HUNTER:
		m_wTransformationSkin = dJOB_MONSTER_DARK_ELF;
		m_wTransformationSkill	=	dSKILL_TRANSFORMATION_BOW_ATTACK;		// 변신 스킬
		break;
		
	case eIE_TRANSFORMATION_THIEF:
		m_wTransformationSkin = dJOB_MONSTER_ASSASSIN;
		m_wTransformationSkill	=	dSKILL_TRANSFORMATION_SWORD_ATTACK;		// 변신 스킬
		break;
	case eIE_TRANSFORMATION_MAGICIAN:
		m_wTransformationSkin = dJOB_MONSTER_FALLEN_WIZARD;
		m_wTransformationSkill	=	dSKILL_TRANSFORMATION_FIRE_BOLT;		// 변신 스킬
		break;
	case	eIE_ACL_TRANSFORMATION_REPTILE:

		if(m_wTransformationSkin == dJOB_MONSTER_REPTILE)
		{
			m_wTransformationTime				=	0;
			m_wTransformationLevel				=	0;
			m_wTransformationSkin				=	0xffff;
			m_wTransformationSkill				=	0xffff;
			buildPower();
			return;
		}

		m_wTransformationSkin = dJOB_MONSTER_REPTILE;
		m_wTransformationSkill	=	dSKILL_TRANSFORMATION_SWORD_ATTACK;		// 변신 스킬
		break;
	}
	m_GGG.ResetGGG();
	m_wTransformationTime	= _wTime;
	m_wTransformationLevel	= _wLevel;
 	m_bf5MagicCarpetShape = 0;
	m_defensePower.dispell();
	setReactionAllOff();
	buildPower();
	m_iHP = getMaxHP();
}
//
//	보석 사용
BOOL
CHero::useJewel(cItem *_lpItem,BOOL _isBeltSlotItem)
{
	memcpy(&m_jewel,_lpItem,sizeof(cItem));
	m_wJewelTime	=	m_jewel.m_aOption[0]*60;

	buildPower();

	decreaseItemCount(_lpItem,_isBeltSlotItem);

	return TRUE;
}	//	CActor::useJewel(cItem *_lpItem,BOOL _isBeltSlotItem)
//
//	모든 보정 데이터를 초기화 시킨다.
void
CHero::rebuildBasicData()
{
	//	마법 속성 공격에 대한 저항력
	m_fireResistance.m_sValue	=	m_sFireResistance;		//	불꽃 저항
	m_waterResistance.m_sValue	=	m_sWaterResistance;		//	물 저항
	m_windResistance.m_sValue	=	m_sWindResistance;		//	바람 저항
	m_earthResistance.m_sValue	=	m_sEarthResistance;		//	대지 저항
	m_lightResistance.m_sValue	=	m_sLightResistance;		//	빛 저항
	m_darkResistance.m_sValue	=	m_sDarkResistance;		//	어둠 저항

	m_bFreezeResistance			=	(BYTE)m_sFreezeResistance;		//	프리즈 저항
	m_bColdResistance			=	(BYTE)m_sColdResistance;		//	콜드 저항
	m_bStunResistance			=	(BYTE)m_sStunResistance;		//	경직 저항
	m_bConfuseResistance		=	(BYTE)m_sConfuseResistance;		//	혼란 저항
	m_bCharmingResistance		=	(BYTE)m_sCharmingResistance;	//	챠밍 저항
	m_bStoneResistance			=	(BYTE)m_sStoneResistance;		//	석화 저항
	m_bPoisonResistance			=	(BYTE)m_sPoisonResistance;		//	중독 저항
	m_bSleepResistance			=	(BYTE)m_sSleepResistance;		//	잠 저항
	m_bBlindResistance			=	(BYTE)m_sBlindResistance;		//	장님 저항

	m_strangeStatusResistance.m_sValue	=	m_sBadStatusResistance;		//	상태 이상 저항
	m_declinePowerResistance.m_sValue	=	m_sDeclinePowerResistance;	//	능력치 저하 저항
	m_curseResistance.m_sValue			=	m_sCurseResistance;			//	저주 저항

	m_maxHP.m_iValue			=	m_iMaxHP;
	m_maxCP.m_iValue			=	m_iMaxCP;

	m_defensePower.resetPrefixData();				//	더하거나 빼야 하는 수치

	m_iWeaponMinDamage			=	m_sMinDamage*100;		//	최소 대미지
	m_iWeaponMaxDamage			=	m_sMaxDamage*100;		//	최대 대미지
	m_wAttackSpeed				=	0;
	m_wAttackRange				=	0;


	if	(!m_wTransformationTime && m_wJob	==	dJOB_FIGHTER)
	{
		m_wAttackRange			=	g_fighterAttackInfo.getAttackRange(getLevel());
		m_wAttackSpeed			=	g_fighterAttackInfo.getAttackSpeed(getLevel())*dSYNC_FPS/100;
		m_iWeaponMinDamage		=	m_sMinDamage*100+g_fighterAttackInfo.getMinAttackPower(getLevel());		//	최소 데미지
		m_iWeaponMaxDamage		=	m_sMaxDamage*100+g_fighterAttackInfo.getMaxAttackPower(getLevel());		//	최대 데미지
	}

	m_defensePower.m_iValue		=	m_sDefensivePower*100;	//	최소 방어력

	m_strength.m_sValue			=	m_sStrength;		//	힘
	m_agility.m_sValue			=	m_sAgility;			//	민첩성
	m_constitution.m_sValue		=	m_sConstitution;	//	건강
	m_intelligence.m_sValue		=	m_sIntelligence;	//	지식
	m_wisdom.m_sValue			=	m_sWisdom;			//	지혜
	m_charisma.m_sValue			=	m_sCharisma;		//	카리스마
	m_luck.m_sValue				=	m_sLuck;			//	운

	m_wItemSlotCount			=	dDEFAULT_ITEM_SLOT_COUNT;
}

//
//	장비 하나의 능력치를 적용시킨다.
BOOL
CHero::applyEquipmentAbility(int _iPart,cItem *_lpItem)
{
	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem(TRUE);
	cItem		item;
	int			iEfficiency		=	100;

	item.copy(_lpItem);

	if	(_lpItem->isExtraItem())
	{
		cExtraItemDefine	*lpExItem			=	(cExtraItemDefine *)_lpItem;

		lpExItem->updateCurrentEfficency();
		iEfficiency	=	lpExItem->m_sCurrentEfficiency;

		if (lpExItem->m_sCurrentEfficiency	<	0)
		{
			cItem::GenerateItemByBaseItem(&item,lpBasicItem->m_wBaseItem,1,100);

			lpBasicItem	=	item.getBasicItem(TRUE);
		}
	}

	if	(m_bIsUseTempBufferForCheckUseableItem	&&	_iPart	<	dEQUIPMENT_PART_COUNT)
	{
		if	(!m_acIsUseAbleItem[_iPart])
			return	FALSE;
	}
	else
	if	(!isUseAbleItem(&item,FALSE))
		return	FALSE;

	if (_iPart	==	dEQUIP_WEAPON || lpBasicItem->m_wKind == eIK_SHIELD)
	{
		m_iWeaponMinDamage		+=	lpBasicItem->m_wMinDamage*100;
		m_iWeaponMaxDamage		+=	lpBasicItem->m_wMaxDamage*100;

		if (lpBasicItem->m_attr.isUseAbleBulletSubstituteArrow)
			m_bf1IsUseAbleBulletSubstituteArrow	=	TRUE;
	}

	if (_iPart == dEQUIP_GLOVE && (m_wJob == dJOB_FIGHTER || m_wJob == dJOB_WEREWOLF))	//	장갑-o-의 데미지는 격투가와 늑대인간 에게만 적용된다.
	{
		m_iWeaponMinDamage	+=	lpBasicItem->m_wMinDamage*100;
		m_iWeaponMaxDamage	+=	lpBasicItem->m_wMaxDamage*100;
	}

	if (_iPart == dEQUIP_SHOES && m_wJob == dJOB_FIGHTER)	//	신발의 데미지는 격투가에게만 적용된다.
	{
		m_iWeaponMinDamage	+=	lpBasicItem->m_wMinDamage*100;
		m_iWeaponMaxDamage	+=	lpBasicItem->m_wMaxDamage*100;
	}

//	기본 데이터
	for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;iGenData++)
	{
		if (lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	0xffff)
			break;

		int	iReferenceIndex1	=	lpBasicItem->m_aGenerateData[iGenData].m_aValue[0];
		int	iReferenceIndex2	=	lpBasicItem->m_aGenerateData[iGenData].m_aValue[1];

		operateEquipmentBasicData(lpBasicItem->m_aGenerateData[iGenData].m_wEffect,item.m_aOption[iReferenceIndex1],item.m_aOption[iReferenceIndex2]);
	}

//	유니크
	if (iEfficiency	>	0)
	{
		for (int iUnique=0;iUnique<dITEM_UNIQUE_DATA_COUNT;iUnique++)
		{
			cUniqueData	*lpInfo	=	&lpBasicItem->m_aUniqueData[iUnique];
			cUniqueData	uniqueData;

			if (lpInfo->m_wEffect	==	0xffff)
				break;

			if (iEfficiency	!=	100)
			{
				memcpy(&uniqueData,lpInfo,sizeof(cUniqueData));

				for (int i=0;i<dITEM_UNIQUE_DATA_VALUE_COUNT;i++)
					uniqueData.m_aValue[i]	=	uniqueData.m_aValue[i]*iEfficiency/100;

				lpInfo	=	&uniqueData;
			}

			operateEquipmentUniqueData(lpInfo);
		}
	}

//	접두사
	if	(item.isExceptionItem() == FALSE)
		for (int iPrefix=0;iPrefix<dITEM_PREFIX_COUNT;iPrefix++)
		{
			if	(item.m_aPrefix[iPrefix].m_wPrefix	==	0xffff)
				break;

			operateEquipmentPrefix(&_lpItem->m_aPrefix[iPrefix]);
		}

	if	(item.isPrincessWeapon())
	{
		cPrincessWeaponDefine	*lpWeapon	=	item.getPrincessWeapon();

		m_attackPower.m_sCorrectPercentageValueByPrefix	+=	lpWeapon->m_wAddDamagePercentage;
	}

	return	TRUE;

/*
	cBasicItem	*lpBasicItem=	_lpItem->getBasicItem();

	if (!isUseAbleItem(_lpItem))
		return	FALSE;

	if (_iPart	==	dEQUIP_WEAPON || lpBasicItem->m_wKind == eIK_SHIELD)
	{
		m_iWeaponMinDamage		+=	lpBasicItem->m_wMinDamage*100;
		m_iWeaponMaxDamage		+=	lpBasicItem->m_wMaxDamage*100;
	}

	if (_iPart == dEQUIP_GLOVE && (m_wJob == dJOB_FIGHTER || m_wJob == dJOB_WEREWOLF))	//	장갑-o-의 대미지는 격투가와 늑대인간 에게만 적용된다.
	{
		m_iWeaponMinDamage	+=	lpBasicItem->m_wMinDamage*100;
		m_iWeaponMaxDamage	+=	lpBasicItem->m_wMaxDamage*100;
	}

	if (_iPart == dEQUIP_SHOES && m_wJob == dJOB_FIGHTER)	//	신발의 대미지는 격투가에게만 적용된다.
	{
		m_iWeaponMinDamage	+=	lpBasicItem->m_wMinDamage*100;
		m_iWeaponMaxDamage	+=	lpBasicItem->m_wMaxDamage*100;
	}

//	기본 데이터
	for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;iGenData++)
	{
		if (lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	0xffff)	break;

		int	iReferenceIndex1	=	lpBasicItem->m_aGenerateData[iGenData].m_aValue[0];
		int	iReferenceIndex2	=	lpBasicItem->m_aGenerateData[iGenData].m_aValue[1];

		operateEquipmentBasicData(lpBasicItem->m_aGenerateData[iGenData].m_wEffect,_lpItem->m_aOption[iReferenceIndex1],_lpItem->m_aOption[iReferenceIndex2]);
	}

//	유니크
	for (int iUnique=0;iUnique<dITEM_UNIQUE_DATA_COUNT;iUnique++)
	{
		if (lpBasicItem->m_aUniqueData[iUnique].m_wEffect==	0xffff)	break;

		operateEquipmentUniqueData(&lpBasicItem->m_aUniqueData[iUnique]);
	}

//	접두사
	for (int iPrefix=0;iPrefix<dITEM_PREFIX_COUNT;iPrefix++)
	{
		if (_lpItem->m_aPrefix[iPrefix].m_wPrefix	==	0xffff)	break;

		operateEquipmentPrefix(&_lpItem->m_aPrefix[iPrefix]);
	}

	return	TRUE;*/
}

void
CHero::operateSpecialItemInInventory()
{
	BOOL	bIsChangedMagicCarpetShape	=	FALSE;
	BOOL	aIsCanNotSupportByLowAbility[dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE];

	memset(aIsCanNotSupportByLowAbility,0,sizeof(aIsCanNotSupportByLowAbility));

	m_wLimitEquipmentLevelOverPoint		=	0;
	m_wPreminumServiceLevel				=	0;
	m_wLimitEquipmentLevelOverPointByCH5=	0;
	m_bf1IsHaveSummonMagicCarpetItem	=	FALSE;


	int		iBadgeCount					=	0,i;

	for (i=0;i<getItemSlotCount();i++)
	{
		cItem	*lpItem	=	&m_aItems[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;

		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem	)	
			continue;

		if	(lpItem->isItemPack())
		{
			int	iPackIndex	=	lpItem->m_aOption[0];

			if	(g_astrItemPackListName[iPackIndex][0]	==	NULL)
				AddAskItemPackName(iPackIndex);
		}

		if	(lpBasicItem->m_attr.isEffectInInventoryItem	==	FALSE)	
			continue;

		if	(lpBasicItem->m_attr.isBadge)
		{
			if (iBadgeCount	>=	dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY)
				continue;

			iBadgeCount++;
		}

		m_wPreminumServiceLevel					=	max((WORD)lpBasicItem->m_wPremiumLevel,m_wPreminumServiceLevel);

		if(lpBasicItem->m_bf1IsCanSummonMagicCarpet)
			m_bf1IsHaveSummonMagicCarpetItem = TRUE;

		if (lpItem->isMagicCarpetEmblem())
		{
			cMagicCarpetEmblemDefine	*lpEmblem	=	(cMagicCarpetEmblemDefine	*)lpItem;

			if (lpEmblem->m_bf1IsSquirrelCarpet)
				m_bf1IsExistSquirrelCarpet	=	TRUE;
			if (lpEmblem->m_bf1IsTurtleCarpet)
				m_bf1IsExistTurttleCarpet	=	TRUE;

			continue;
		}

		if	(!applyEquipmentAbility(0xffff,lpItem))
			aIsCanNotSupportByLowAbility[i]	=	TRUE;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;

		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem	)	
			continue;

		if(lpItem->isItemPack())
		{
			int	iPackIndex	=	lpItem->m_aOption[0];

			if (g_astrItemPackListName[iPackIndex][0]	==	NULL)
				AddAskItemPackName(iPackIndex);
		}

		if	(lpBasicItem->m_attr.isEffectInInventoryItem	==	FALSE)	
			continue;

		if (lpBasicItem->m_attr.isBadge)
		{
			if (iBadgeCount	>=	dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY)
				continue;

			iBadgeCount++;
		}

		m_wPreminumServiceLevel					=	max((WORD)lpBasicItem->m_wPremiumLevel,m_wPreminumServiceLevel);

		if(lpBasicItem->m_bf1IsCanSummonMagicCarpet)
			m_bf1IsHaveSummonMagicCarpetItem = TRUE;

		if (lpItem->isMagicCarpetEmblem())
		{
			cMagicCarpetEmblemDefine	*lpEmblem	=	(cMagicCarpetEmblemDefine	*)lpItem;

			if (lpEmblem->m_bf1IsSquirrelCarpet)
				m_bf1IsExistSquirrelCarpet	=	TRUE;
			if (lpEmblem->m_bf1IsTurtleCarpet)
				m_bf1IsExistTurttleCarpet	=	TRUE;

			continue;
		}

		if (!applyEquipmentAbility(0xffff,lpItem))
			aIsCanNotSupportByLowAbility[i+dOWN_ITEM_COUNT]	=	TRUE;
	}

	while(1)
	{
		BOOL	bIsChange	=	FALSE;

		iBadgeCount	=	0;

		for (i=0;i<getItemSlotCount();i++)
		{
			cItem	*lpItem	=	&m_aItems[i];

			if	(lpItem->m_wBaseItem	==	0xffff)
				continue;

			cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

			if	(!lpBasicItem	)	
				continue;
			if	(lpBasicItem->m_attr.isEffectInInventoryItem	==	FALSE)	
				continue;

			if (lpBasicItem->m_attr.isBadge)
			{
				if (iBadgeCount	>=	dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY)
					continue;

				iBadgeCount++;
			}

			if (aIsCanNotSupportByLowAbility[i])
				if (applyEquipmentAbility(0xffff,lpItem))
				{
					aIsCanNotSupportByLowAbility[i]	=	FALSE;
					bIsChange	=	TRUE;
				}
		}
		
		for (i=0;i<m_bf6ExtraInventorySize;i++)
		{
			cItem	*lpItem	=	&m_aExtraInventory[i];

			if	(lpItem->m_wBaseItem	==	0xffff)
				continue;

			cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

			if	(!lpBasicItem	)	
				continue;
			if	(lpBasicItem->m_attr.isEffectInInventoryItem	==	FALSE)	
				continue;

			if (lpBasicItem->m_attr.isBadge)
			{
				if (iBadgeCount	>=	dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY)
					continue;

				iBadgeCount++;
			}

			if (aIsCanNotSupportByLowAbility[i+dOWN_ITEM_COUNT])
				if (applyEquipmentAbility(0xffff,lpItem))
				{
					aIsCanNotSupportByLowAbility[i+dOWN_ITEM_COUNT]	=	FALSE;
					bIsChange	=	TRUE;
				}
		}

		if (bIsChange	==	FALSE)
			break;
	}

	m_wLimitEquipmentLevelOverPoint		+=	m_wLimitEquipmentLevelOverPointByCH5;

}

//
//	능력치 데이터를 계산한다.

void
CHero::operateEquipmentData(BOOL _bIsRequireCheckBeltItem , BOOL _bIsBySkillPointReset)
{
	int		i;
	BOOL	aIsCanNotSupportByLowAbility[dEQUIP_WEAPON_2];
	memset(aIsCanNotSupportByLowAbility,0,sizeof(aIsCanNotSupportByLowAbility));

 	if	(m_bIsEmulation	==	FALSE)
 		syncHeroApprearance();	//	외관 싱크

//	보석 데이터 처리
	if	(m_wJewelTime)
		for (int iPrefix=0;iPrefix<dITEM_PREFIX_COUNT;iPrefix++)
		{
			if	(m_jewel.m_aPrefix[iPrefix].m_wPrefix	==	0xffff)
				break;

			operateEquipmentPrefix((CItemPrefixInfo*)&m_jewel.m_aPrefix[iPrefix]);
		}

//	무기 대미지 계산
	{
		m_defensePower.m_iValue	=	m_sDefensivePower*100;	//	최소 방어력
		m_iWeaponMinDamage		=	m_sMinDamage*100;		//	최소 대미지
		m_iWeaponMaxDamage		=	m_sMaxDamage*100;		//	최대 대미지

		if	(m_wTransformationTime || m_wJob	==	dJOB_FIGHTER)
		{
			m_iWeaponMinDamage		=	m_sMinDamage*100+g_fighterAttackInfo.getMinAttackPower(getLevel());		//	최소 데미지
			m_iWeaponMaxDamage		=	m_sMaxDamage*100+g_fighterAttackInfo.getMaxAttackPower(getLevel());		//	최대 데미지
		}
	}

//	장비에 데이터 처리
	for (i=dEQUIP_WEAPON;i<=dEQUIP_RING8;i++)
	{
		cItem		*lpItem		=	getEquipment(i);

		if	(!lpItem)
			continue;

		if	(!applyEquipmentAbility(i,lpItem))	
			aIsCanNotSupportByLowAbility[i]	=	TRUE;
	}

	operateItemSetEffect();
	int aiItemSetList[dEQUIPMENT_PART_COUNT] = {0,};
	int aiItemSetCount[dEQUIPMENT_PART_COUNT] = {0,};

	int iItemSetListCount = getOperateItemSetList(aiItemSetList);

	for(i=0;i<iItemSetListCount;++i)
		aiItemSetCount[i] = getEquippedSetItemCount(aiItemSetList[i]);
//	능력치가 딸려서 착용 못했던 아이템들 처리
	while(1)
	{
		BOOL	bIsChange	=	FALSE;
		//	장비에 데이터 처리
		for (int i=dEQUIP_WEAPON;i<=dEQUIP_RING8;i++)
		{
			if	(!aIsCanNotSupportByLowAbility[i])
				continue;

			cItem		*lpItem		=	getEquipment(i);

			if	(!lpItem)
				continue;

			if	(applyEquipmentAbility(i,lpItem))
			{
				aIsCanNotSupportByLowAbility[i]	=	FALSE;
				bIsChange						=	TRUE;
			}
		}

		if	(bIsChange	==	FALSE)
			break;
	}
	removeItemSetEffect(aiItemSetList,aiItemSetCount, iItemSetListCount);

	operateItemSetEffect();
	if (m_iWeaponMinDamage>	m_iWeaponMaxDamage)
	{
		int	iDamage			=	m_iWeaponMinDamage;

		m_iWeaponMinDamage	=	m_iWeaponMaxDamage;		//	최소 대미지
		m_iWeaponMaxDamage	=	iDamage;
	}

	if	(_bIsRequireCheckBeltItem	&&	m_bf1IsWaitSimpleGuildInfo	==	FALSE)
		for (i=dEQUIP_BELT_SLOT1;i<=dEQUIP_BELT_SLOT5;i++)
		{
			if	(m_aEquip[i].m_wBaseItem	==	0xffff)
				continue;

			if	(m_aEquip[i].m_bCount	>	m_wBeltStackCount)
			{
				int	iRemoveItemCount	=	m_aEquip[i].m_bCount-m_wBeltStackCount;
				m_aEquip[i].m_bCount	=	(BYTE)m_wBeltStackCount;

				cItem	item,itemOld;

				item.copy(&m_aEquip[i]);
				itemOld.copy(&m_aEquip[i]);
				m_aEquip[i].reset();

				item.m_bCount	=	iRemoveItemCount;

				addItemToInventory(&item);

				m_aEquip[i].copy(&itemOld);

				if (item.m_bCount)
				{
					if (m_aEquip[i].isCanNotDropItem())
						m_aEquip[i].m_bCount+=	item.m_bCount;
				}

				if (m_aEquip[i].m_bCount	==	0)
					m_aEquip[i].reset();
			}
		}
}

//
//	짐 모리의 인챈트 서비스
int
CHero::getJimMorysEnchantServiceSuccessChance(cItem *_lpEnchantItem)
{
	cBasicItem		*lpBasicItem	=	_lpEnchantItem->getBasicItem();

	if (lpBasicItem->m_aGenerateData[0].m_wEffect	!=	eIE_JIM_MORYS_ENCHANT)
		return	0xffffffff;

	int	iMinSuccessChance	=	lpBasicItem->m_wEnchantMinChance*100;
	int	iMaxSuccessChance	=	lpBasicItem->m_wEnchantMaxChance*100;
	int	iSuccessChance		=	min(iMinSuccessChance+(getLuck()-100)*6,iMaxSuccessChance);

	iSuccessChance			=	max(iSuccessChance,iMinSuccessChance);
	
	return	iSuccessChance;
}

//
//	아이템을 아이템에 사용한다.
int
CHero::getEnchantItemSuccessChanceByUniqueInfo(cItem *_lpItem)
{
	cBasicItem		*lpBasicItem	=	_lpItem->getBasicItem();

	if (lpBasicItem->m_aGenerateData[0].m_wEffect	!=	eIE_ENCHANT_ITEM_BY_UNIQUE_INFO)
		return	0xffffffff;

	int	iMinSuccessChance	=	lpBasicItem->m_wEnchantMinChance*100;
	int	iMaxSuccessChance	=	lpBasicItem->m_wEnchantMaxChance*100;
	int	iSuccessChance		=	min(iMinSuccessChance+ (getLuck() - 100)*6,iMaxSuccessChance);

	iSuccessChance			=	max(iSuccessChance,iMinSuccessChance);

	return	iSuccessChance;
}

int
CHero::getEquippedSetItemCount(int _iItemSet)
{
	int			i,iItemSetCount=0,iPart;

	cItemSet	*lpItemSet	=	g_itemSetManager.get(_iItemSet);

	if	(!lpItemSet)
		return	0;

	for (i=0;i<lpItemSet->getItemCount();i++)
	{
		cSetItem	*lpSetItem	=	lpItemSet->getItem(i);

		if	(!lpSetItem)
			continue;

		for (iPart=dEQUIP_WEAPON;iPart<=dEQUIP_WEAPON_2;iPart++)
		{
			cItem			*lpItem			=	&m_aEquip[iPart];
			cSpecialItem	*lpSpecialItem	=	lpItem->getSpecialItem();

			if	(!lpSpecialItem || lpSpecialItem->m_bf1IsSetItem == FALSE	||	lpSpecialItem->m_bf11SValue != _iItemSet)
				continue;

			if	(lpItem->m_wBaseItem	!=	lpSetItem->m_wBaseItem)
				continue;

			if	(!isUseAbleItem(lpItem,FALSE))
				continue;

			iItemSetCount++;

			break;
		}
	}

	return	iItemSetCount;
}

cItem*
CHero::getEquippedSetItemByBaseItem(int _iItemSet,int _iBaseItem)
{
	int			iPart;
	cItemSet	*lpItemSet	=	g_itemSetManager.get(_iItemSet);

	if	(!lpItemSet)
		return	NULL;

	for (iPart=dEQUIP_WEAPON;iPart<=dEQUIP_RING8;iPart++)
	{
		cItem	*lpItem			=	getEquipment(iPart);

		if	(!lpItem)
			continue;

		cSpecialItem	*lpSpecialItem	=	lpItem->getSpecialItem();

		if	(!lpSpecialItem			||	lpSpecialItem->m_bf1IsSetItem == FALSE	||	lpSpecialItem->m_bf11SValue != _iItemSet)
			continue;

		if	(lpItem->m_wBaseItem	!=	_iBaseItem)
			continue;

		return	lpItem;
	}

	return	NULL;
}

cItem*
CHero::getEquippedSetItem(int _iItemSet,int _iIndex)
{
	int			iPart;
	cItemSet	*lpItemSet	=	g_itemSetManager.get(_iItemSet);

	if	(!lpItemSet)
		return	NULL;

	cSetItem	*lpSetItem	=	lpItemSet->getItem(_iIndex);

	if	(!lpSetItem)
		return	NULL;

	for (iPart=dEQUIP_WEAPON;iPart<=dEQUIP_WEAPON_2;iPart++)
	{
		cItem			*lpItem			=	&m_aEquip[iPart];
		cSpecialItem	*lpSpecialItem	=	lpItem->getSpecialItem();

		if	(!lpSpecialItem			||	lpSpecialItem->m_bf1IsSetItem == FALSE	||	lpSpecialItem->m_bf11SValue != _iItemSet)
			continue;

		if	(lpItem->m_wBaseItem	!=	lpSetItem->m_wBaseItem)
			continue;

		if	(!isUseAbleItem(lpItem,FALSE))
			continue;

		return	lpItem;
	}

	return	NULL;
}

cItem*
CHero::getItemByOption(int _iOption)
{
	int	iIndex	=	getItemIndexByOption(_iOption);

	if	(iIndex	==	-1)
		return	NULL;

	return	getInventoryItem(iIndex);
}

int
CHero::getItemIndexByOption(int _iOption)
{
	int	i;

	for (i=0;i<dBORDER_OF_ITEM_AND_EQUIPMENT;i++)
	{
		cItem	*lpItem				=	getInventoryItem(i);

		if	(!lpItem)
			continue;

		if	(lpItem->isExistSpecificEffect(_iOption))
			return	i;
	}

	return	-1;
}

cItem*
CHero::getSetItemInInventory(int _iItemSet,int _iIndex)
{
	int			iSlot;
	cItemSet	*lpItemSet	=	g_itemSetManager.get(_iItemSet);

	if	(!lpItemSet)
		return	NULL;

	cSetItem	*lpSetItem	=	lpItemSet->getItem(_iIndex);

	if	(!lpSetItem)
		return	NULL;

	for (iSlot=0;iSlot<getItemSlotCount();iSlot++)
	{
		cItem			*lpItem			=	&m_aItems[iSlot];
		cSpecialItem	*lpSpecialItem	=	lpItem->getSpecialItem();

		if	(!lpSpecialItem			||	lpSpecialItem->m_bf1IsSetItem == FALSE	||	lpSpecialItem->m_bf11SValue != _iItemSet)
			continue;

		if	(lpItem->m_wBaseItem	!=	lpSetItem->m_wBaseItem)
			continue;

		return	lpItem;
	}

	for (iSlot=0;iSlot<m_bf6ExtraInventorySize;iSlot++)
	{
		cItem			*lpItem			=	&m_aExtraInventory[iSlot];
		cSpecialItem	*lpSpecialItem	=	lpItem->getSpecialItem();

		if	(!lpSpecialItem			||	lpSpecialItem->m_bf1IsSetItem == FALSE	||	lpSpecialItem->m_bf11SValue != _iItemSet)
			continue;

		if	(lpItem->m_wBaseItem	!=	lpSetItem->m_wBaseItem)
			continue;

		return	lpItem;
	}

	{
		int			iPart;

		for (iPart=dEQUIP_WEAPON;iPart<=dEQUIP_WEAPON_2;iPart++)
		{
			cItem			*lpItem			=	&m_aEquip[iPart];
			cSpecialItem	*lpSpecialItem	=	lpItem->getSpecialItem();

			if	(!lpSpecialItem			||	lpSpecialItem->m_bf1IsSetItem == FALSE	||	lpSpecialItem->m_bf11SValue != _iItemSet)
				continue;

			if	(lpItem->m_wBaseItem	!=	lpSetItem->m_wBaseItem)
				continue;

			if	(!isUseAbleItem(lpItem,FALSE))
				return	lpItem;
		}

		return	NULL;

	}
	return	NULL;
}

void
CHero::operateItemSetEffect(int _iItemSet)
{
	cItemSet	*lpItemSet	=	g_itemSetManager.get(_iItemSet);

	if (!lpItemSet)
		return;

	int	iSetItemCount	=	getEquippedSetItemCount(_iItemSet);

	for (int i=0;i<lpItemSet->getSetEffectCount(iSetItemCount);i++)
	{
		cUniqueData	*lpEffect	=	lpItemSet->getSetEffect(iSetItemCount,i);

		operateEquipmentUniqueData(lpEffect);
	}
}

void
CHero::operateItemSetEffect()
{
	int	aiItemSetList[dEQUIPMENT_PART_COUNT];
	
	
	memset(aiItemSetList,0,sizeof(aiItemSetList));
	
	int	iItemSetCount	=	getOperateItemSetList(aiItemSetList);
	
	for (int i=0;i<iItemSetCount;i++)
		operateItemSetEffect(aiItemSetList[i]);
}

int
CHero::getOperateItemSetList(int* _lpItemSetList)
{
// output	: 세트아이템의 수
	// input	: 저장할 세트아이템리스트 
	int iItemSetCount = 0;
	for (int i=dEQUIP_WEAPON;i<=dEQUIP_RING8;i++)
	{
		cItem			*lpItem			=	getEquipment(i);
		
		if	(!lpItem)
			continue;

		cSpecialItem	*lpSpecialItem	=	lpItem->getSpecialItem();
		
		if	(!lpSpecialItem || lpSpecialItem->m_bf1IsSetItem == FALSE)
			continue;

		for (int j=0;j<iItemSetCount;j++)
		{
			if	(_lpItemSetList[j]	==	(int)lpSpecialItem->m_bf11SValue)
			{
				j	=	-1;
				break;
			}
		}

		if	(j	==	-1)
			continue;

		_lpItemSetList[iItemSetCount++]	=	lpSpecialItem->m_bf11SValue;
	}

	return iItemSetCount;

}

void
CHero::removeItemSetEffect(int* _lpItemSetList,int* _lpItemSetCount, int _iItemSetListCount)
{

	for (int i=0;i<_iItemSetListCount;++i)
	{
		cItemSet	*lpItemSet	=	g_itemSetManager.get(_lpItemSetList[i]);
		
		if (!lpItemSet)
			return;
		
		int	iSetItemCount	=	_lpItemSetCount[i];
		
		for (int j=0;j<lpItemSet->getSetEffectCount(iSetItemCount);++j)
		{
			cUniqueData	*lpEffect	=	lpItemSet->getSetEffect(iSetItemCount,j);
			cUniqueData uniqueData;
			
			if(!lpEffect)
				continue;
			memcpy(&uniqueData, lpEffect, sizeof(cUniqueData));
			for(int k=0;k<dITEM_UNIQUE_DATA_VALUE_COUNT;++k)
			{
				uniqueData.m_aValue[k] = uniqueData.m_aValue[k]*-1;
			}

			operateEquipmentUniqueData(&uniqueData);
		}
	}
}


char*
CHero::getAfterEquipInfo(char *_lpstrString,cItem *_lpItem)
{
	static		cSTRING		s_string;

	cBasicItem*	lpBasicItem	=	_lpItem->getBasicItem(TRUE);
	
	if(	m_dwGeneatedToolTipSerial		== _lpItem->m_dwSerial	&& 
		m_dwGeneatedToolTipHeroCheckSum == getCheckSum()		&&
		m_iGeneatedToolTipItemCheckSum	== _lpItem->getCheckSum())
	{
		if(lpBasicItem && lpBasicItem->m_iSerial == m_iGeneatedToolTipBaseSerial && s_string.Length)
			return s_string.String;
	}

	m_dwGeneatedToolTipSerial			=	_lpItem->m_dwSerial;
	m_iGeneatedToolTipItemCheckSum		=	_lpItem->getCheckSum();
	m_dwGeneatedToolTipHeroCheckSum		=	getCheckSum();

	if(lpBasicItem)
		m_iGeneatedToolTipBaseSerial	= lpBasicItem->m_iSerial;

	cItem	itemOrg,itemFocus;
	int		iItemType		=	_lpItem->getItemType(),i;
	s_string.Set(_lpstrString);

	if	(c_aItemEquipPlace[iItemType] == dITEM_KIND_WASTING ||  c_aItemEquipPlace[iItemType] == 0xffff)	// 변경.
	{
		return	s_string.String;
	}

//	착용가능하다면
	if	(!isUseAbleItem(_lpItem,FALSE,TRUE))
		return	s_string.String;


	memcpy(&itemFocus,_lpItem,sizeof(cItem));

	int	iEquipPlace	=  c_aItemEquipPlace[iItemType];			

	if	(iEquipPlace==	dEQUIP_WEAPON)	//	장비 위치가 무기1 슬록이다.
		if (m_wJob != dJOB_SUMMONER	&& m_wJob != dJOB_BEAST_TAMER)	//	소환사나 조련사가 아니라면
			if (m_wJob % 2)	//	두번째 직업이라면
				iEquipPlace	=	dEQUIP_WEAPON_2;

	if	(iEquipPlace==	dEQUIP_RING)
		for	(i=dEQUIP_RING1;i<=dEQUIP_RING8;i++)
			if	(m_aEquip[i].m_wBaseItem	==	0xffff)
			{
				iEquipPlace	=	i;
				break;
			}

	for	 (i = dEQUIP_WEAPON;i<dEQUIP_WEAPON_2;i++)
		m_acIsUseAbleItem[i]		=	isUseAbleItem(getEquipment(i),FALSE,FALSE);

	m_bIsUseTempBufferForCheckUseableItem	=	TRUE;

	if	(iEquipPlace	==	dEQUIP_WEAPON_2)
		m_acIsUseAbleItem[0]			=	TRUE;
	else
		m_acIsUseAbleItem[iEquipPlace]	=	TRUE;

	//	현재 착용중인 아이템 복사(있건 없건)
	memcpy(&itemOrg,&m_aEquip[iEquipPlace],sizeof(cItem));

	int	aiDiffValue[12];
	int	iDiffCount		=	0;
	memset(aiDiffValue,0,sizeof(aiDiffValue));

	int iOldHp					=	getMaxHP()/100;
	int	iOldCp					=	getMaxCP()/100;
	int iOldAttackPower1		=	getAttackPower(NULL,TRUE)/100;
	int iOldAttackPower2		=	getAttackPower(NULL,FALSE)/100;
	int iOldDefensePower		=	getDefensivePower()/100;
	int	iOldStrength			=	getStrength();
	int	iOldAgility				=	getAgility();
	int	iOldConstitution		=	getConstitution();
	int	iOldCharisma			=	getCharisma();
	int	iOldIntelligence		=	getIntelligence();
	int	iOldWisdom				=	getWisdom();
	int	iOldLuck				=	getLuck();
	int	iOldHP					=	m_iHP;
	int	iOldCP					=	m_iCP;

	memcpy(&m_aEquip[iEquipPlace],&itemFocus,sizeof(cItem));
	buildPower(FALSE);

	aiDiffValue[0]				=	getMaxHP()/100-iOldHp;
	aiDiffValue[1]				=	getMaxCP()/100-iOldCp;
	aiDiffValue[2]				=	getAttackPower(NULL,TRUE)/100-iOldAttackPower1;
	aiDiffValue[3]				=	getAttackPower(NULL,FALSE)/100-iOldAttackPower2;
	aiDiffValue[4]				=	getDefensivePower()/100-iOldDefensePower;
	aiDiffValue[5]				=	getStrength()-iOldStrength;
	aiDiffValue[6]				=	getAgility()-iOldAgility;
	aiDiffValue[7]				=	getConstitution()-iOldConstitution;
	aiDiffValue[8]				=	getCharisma()-iOldCharisma;
	aiDiffValue[9]				=	getIntelligence()-iOldIntelligence;
	aiDiffValue[10]				=	getWisdom()-iOldWisdom;
	aiDiffValue[11]				=	getLuck()-iOldLuck;

	for (i=0;i<12;i++)
		if (aiDiffValue[i])
		{
			s_string.Add("\n<c:LTCYAN><%s><n>\n",dMSG_AFTER_EQUIP_INFO);
			s_string.Add("-");

			for (i=0;i<12;i++)
			{
				if (aiDiffValue[i]	==	0)
					continue;

				if (iDiffCount	%	4	==	3)
					s_string.Add("\n-");

				s_string.Add(" ");

				s_string.Add("%s ",g_pstrCommonMessage[dWM_DIFF_MESSAGE_BEGIN+i]);

				if (aiDiffValue[i]	>	0)
					s_string.Add(dMSG_DIFF_PLUS_FORM,aiDiffValue[i]);
				else
					s_string.Add(dMSG_DIFF_MINUS_FORM,aiDiffValue[i]);

				iDiffCount++;
			}

			break;
		}

	memcpy(&m_aEquip[iEquipPlace],&itemOrg,sizeof(cItem));
	m_bIsUseTempBufferForCheckUseableItem	=	FALSE;

	buildPower(FALSE);

	m_iHP	=	iOldHP;
	m_iCP	=	iOldCP;

	return	s_string.String;
}
