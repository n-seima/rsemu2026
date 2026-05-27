#include "cItem.H"
#include "cACTOR.H"
#include "CGame.H"

void
cACTOR::getItemMagicDamage(int _iEffect,int _iValue1,int _iValue2,int *_lpiFireDamage,int *_lpiWaterDamage,int *_lpiWindDamage,int *_lpiEarthDamage,int *_lpiLightDamage,int *_lpiDarkDamage)
{
	switch(_iEffect)
	{
		case	eIE_HIT_FIRE_DAMAGE						:
			*_lpiFireDamage	+=	_iValue1;
			break;//	"불속성 데미지"
		case	eIE_HIT_WATER_DAMAGE					:
			*_lpiWaterDamage	+=	_iValue1;
			break;//	"불속성 데미지"
		case	eIE_HIT_WIND_DAMAGE						:
			*_lpiWindDamage	+=	_iValue1;
			break;//	"불속성 데미지"
		case	eIE_HIT_TERRA_DAMAGE					:
			*_lpiEarthDamage+=	_iValue1;
			break;//	"불속성 데미지"
		case	eIE_HIT_LIGHT_DAMAGE					:
			*_lpiLightDamage	+=	_iValue1;
			break;//	"불속성 데미지"
		case	eIE_HIT_DARK_DAMAGE						:
			*_lpiDarkDamage	+=	_iValue1;
			break;//	"불속성 데미지"
	}
}

void
cACTOR::getItemPrefixMagicDamage(int _iEffect,int _iValue1,int _iValue2,int *_lpiFireDamage,int *_lpiWaterDamage,int *_lpiWindDamage,int *_lpiEarthDamage,int *_lpiLightDamage,int *_lpiDarkDamage)
{
	int	iValue;

	switch(_iEffect)
	{
		case	eIP_FIRE_DAMAGE							:
		{
			iValue	=	max(_iValue1+random(_iValue2-_iValue1),0);
			*_lpiFireDamage	+=	iValue;
			break;
		}
		case	eIP_WATER_DAMAGE						:
		{
			iValue	=	max(_iValue1+random(_iValue2-_iValue1),0);
			*_lpiWaterDamage	+=	iValue;
			break;
		}
		case	eIP_WIND_DAMAGE							:
		{
			iValue	=	max(_iValue1+random(_iValue2-_iValue1),0);
			*_lpiWindDamage	+=	iValue;
			break;
		}
		case	eIP_TERRA_DAMAGE						:
		{
			iValue	=	max(_iValue1+random(_iValue2-_iValue1),0);
			*_lpiEarthDamage	+=	iValue;
			break;
		}

		case	eIP_LIGHT_DAMAGE						:	//	빛 속성 공격
		{
			iValue	=	max(_iValue1+random(_iValue2-_iValue1),0);
			*_lpiLightDamage	+=	iValue;
			break;
		}
		case	eIP_DARK_DAMAGE							:		//	어둠 속성 공격
		{	
			iValue	=	max(_iValue1+random(_iValue2-_iValue1),0);
			*_lpiDarkDamage	+=	iValue;
			break;
		}
	}
}
//
//	효과 처리
void
cACTOR::operateEquipmentBasicData(int _iEffect,int _iValue1,int _iValue2)
{
	switch(_iEffect)
	{
		case	eIE_EQUIP_INCREASE_MOVE_SPEED			:
			m_moveSpeed.addValue(_iValue1);
			break;//	이동 속도

		case	eIE_INCREASE_SECRET_DUNGEON_PLAY_COUNT	:
			m_wCorrectSecretDungeonPlayCount	=	_iValue1;
			break;

		case	eIE_INCREASE_GUILD_DUNGEON_PLAY_COUNT	:
			m_wCorrectGuildDungeonPlayCount	=	_iValue1;
			break;

		case	eIE_INCREASE_GAIN_MINIPET_EXP			:	//	미니펫이 먹이를 먹어서 올라가는 경험치 증가
			m_wCorrectMiniPetGainExpByFeedItem	=	_iValue1;
			break;

		case	eIE_HUNTING_EXP_BONUS			:	//	사냥 경험치 보너스
			m_bIsApplyHuntingExpBonus		=	TRUE;
			break;

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
			m_wMagicCarpetShape				=	eMCS_WILD_CAT;
			break;
		//┃특수 능력
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

		//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		//┃기본 수치
		case	eIE_ADD_DEFENSE_POWER					:	m_defensePower.m_iValue	+=	_iValue1*100;break;//	"방어력"
		case	eIE_BLOCKING_RATE						:	addBlockingChance(_iValue1);break;//	"방패 블럭률"
		case	eIE_BELT_STACK_COUNT					:	m_wBeltStackCount		+=	_iValue1;break;//	"벨트 스택 아이템수"
		//┃기본 수치
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

		//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		//┃공격 부가 효과/부가 데미지
		case	eIE_INSTANCE_KILL						:	addInstanceKillChance(_iValue1);break;//	"즉사"
		case	eIE_MAKE_BERSERKER						:	addMakeBerserk(_iValue1,_iValue2);break;//	"대상을 버서크 상태로 만듦"
		case	eIE_MAKE_ARMOR_BREAK					:	addMakeArmorBreak(_iValue1,_iValue2);break;//	"갑옷 파괴"
		case	eIE_MAKE_WEAPON_BREAK					:	addMakeWeaponBreak(_iValue1,_iValue2);break;//	"무기 파괴"
		case	eIE_MAKE_POISONED						:	addAddPoisonDamage(_iValue1,_iValue2);break;//	"중독시킨다."
		case	eIE_MAKE_CONFUSE						:	addMakeConfuse(_iValue1,_iValue2);break;//	"혼란시킨다."
		case	eIE_MAKE_CHARMING						:	addMakeCharmed(_iValue1,_iValue2);break;//	"유혹"
		case	eIE_MAKE_CHARMING_30					:	addMakeCharmed(_iValue1,30);break;//	"유혹"
		case	eIE_MAKE_STUN							:	addMakeStun(_iValue1,_iValue2);break;//	"스턴"
		case	eIE_MAKE_COLD							:	addMakeCold(_iValue1,_iValue2);break;//	"콜드"
		case	eIE_MAKE_COLD_100						:	addMakeCold(100,_iValue1);break;//	"콜드"

		case	eIE_HIT_FIRE_DAMAGE						:	addAddFireDamage(_iValue1);break;//	"불속성 데미지"
		case	eIE_HIT_WATER_DAMAGE					:	addAddWaterDamage(_iValue1);break;//	"물속성 데미지"
		case	eIE_HIT_WIND_DAMAGE						:	addAddWindDamage(_iValue1);break;//	"바람속성 데미지"
		case	eIE_HIT_TERRA_DAMAGE					:	addAddEarthDamage(_iValue1);break;//	"대지속성 데미지"
		case	eIE_HIT_LIGHT_DAMAGE					:	addAddLightDamage(_iValue1);break;//	"빛속성 데미지"
		case	eIE_HIT_DARK_DAMAGE						:	addAddDarkDamage(_iValue1);break;//	"어둠속성 데미지"

		// VV 09.08.07 마법속성옵션 DX데미지	
		case	eIE_HIT_FIRE_DAMAGE_DX					:	addAddNaturalDamageDX(eADD_FIRE_DAMAGE_DX,_iValue1);break;//	"불속성 데미지"
		case	eIE_HIT_WATER_DAMAGE_DX					:	addAddNaturalDamageDX(eADD_WATER_DAMAGE_DX,_iValue1);break;//	"물속성 데미지"
		case	eIE_HIT_WIND_DAMAGE_DX					:	addAddNaturalDamageDX(eADD_WIND_DAMAGE_DX,_iValue1);break;//	"바람속성 데미지"
		case	eIE_HIT_TERRA_DAMAGE_DX					:	addAddNaturalDamageDX(eADD_EARTH_DAMAGE_DX,_iValue1);break;//	"대지속성 데미지"
		case	eIE_HIT_LIGHT_DAMAGE_DX					:	addAddNaturalDamageDX(eADD_LIGHT_DAMAGE_DX,_iValue1);break;//	"빛속성 데미지"
		case	eIE_HIT_DARK_DAMAGE_DX					:	addAddNaturalDamageDX(eADD_DARK_DAMAGE_DX,_iValue1);break;//	"어둠속성 데미지"
		// AA 09.08.07 마법속성옵션 DX데미지

		case	eIE_ADD_DAMAGE_TO_UNDEAD				:	addAddDamageToUndead(_iValue1);break;//	"언데드 사냥꾼"
		case	eIE_ADD_DAMAGE_TO_DEMON					:	addAddDamageToDemon(_iValue1);break;//	"악마 사냥꾼"
		case	eIE_ADD_DAMAGE_TO_HUMAN					:	addAddDamageToHuman(_iValue1);break;//	"인간 사냥꾼"
		case	eIE_ADD_DAMAGE_TO_ANIMAL				:	addAddDamageToAnimal(_iValue1);break;//	"동물 사냥꾼"
		case	eIE_ADD_DAMAGE_TO_HOLY_ANIMAL			:	addAddDamageToHolyAnimal(_iValue1);break;//	"신수 사냥꾼"
		//┃공격 부가 효과
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
		case	eIE_INCREASE_CRITICAL					:	m_correctCriticalHitChance.addValue(_iValue1);break;//	"크리티컬 발생확률"
		case	eIE_INCREASE_CRUSH_SHOT					:	m_bCorrectCrushBlowChance	+=	_iValue1;break;//	"결정타 발생확률"
		case	eIE_INCREASE_HIT_RATE					:	m_hitChance.addValue(_iValue1);break;//	"명중률 증가"
		case	eIE_INCREASE_DODGE_CHANCE				:	m_dodgeChance.addValue(_iValue1);break;//	"명중률 증가"
		case	eIE_INCREASE_CONCENTRATION				:	m_concentration.addValue(_iValue1,dMAXIMUM_CONCENTRATION_VALUE);break;//	"집중력 증가"
		case	eIE_INCREASE_ATTACK_SPEED				:	m_actionSpeed.addValue(_iValue1);break;//	"공격 속도 증가"
		case	eIE_INCREASE_FIX_SELECT_POWER				:	//	선택한 능력치 하나를 증가 시킴
		{
			switch(_iValue2)
			{
				case	0	:	// 0이면 옵션이 없는 것! 옵션을 선택할 수 있다.
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
		case	eIE_RESISTANCE_CURSE_STATUS				:	addCurseResistance(_iValue1);break;//"저주 상태에 대한 저항"
		case	eIE_RESISTANCE_DECLINE_STATUS			:	addDeclinePowerResistance(_iValue1);break;//"저하 상태에 대한 저항"
		case	eIE_RESISTANCE_STRANGE_STATUS			:	addStrangeStatusResistance(_iValue1);break;//"이상 상태에 대한 저항"
		case	eIE_RESISTANCE_ALL_BAD_STATUS			:	addAllBadStatusResistance(_iValue1);break;//	"모든 상태 이상에 대한 저항"
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

		case	eIE_INCREASE_NOCK_BACK_RESISTANCE		:	addKnockoutResistance(_iValue1);break;	//	"녹백 저항"
		case	eIE_INCREASE_CRITICAL_RESISTANCE		:	addCriticalHitResistance(_iValue1);break;	//	"치명타 저항"
		case	eIE_INCREASE_CRUSH_SHOT_RESISTANCE		:	addCrushBlowResistance(_iValue1);break;	//	"결정타 저항"
		case	eIE_INCREASE_INSTANCE_KILL_RESISTANCE	:	addInstanceKillResistance(_iValue1);break;//	즉사 공격에 대한 저항
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
		case	eIE_INCREASE_BEAST_TAMER_SKILL			:	if (m_wJob	==	dJOB_BEAST_TAMER	)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"조련사 스킬 레벨 증가"
		case	eIE_INCREASE_SUMMONER_SKILL				:	if (m_wJob	==	dJOB_SUMMONER		)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"소환사 스킬 레벨 증가"
		case	eIE_INCREASE_PRINCESSE_SKILL			:	if (m_wJob	==	dJOB_PRINCESS		)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"공주 스킬 레벨 증가"
		case	eIE_INCREASE_MAGICAL_GIRL_SKILL			:	if (m_wJob	==	dJOB_MAGICAL_GIRL	)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"마법소녀 스킬 레벨 증가"
		case	eIE_INCREASE_PRIEST_SKILL				:	if (m_wJob	==	dJOB_PRIEST			)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"사제 스킬 레벨 증가"
		case	eIE_INCREASE_FALLEN_ANGEL_SKILL			:	if (m_wJob	==	dJOB_FALLEN_ANGEL	)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"타락천사 스킬 레벨 증가"
		case	eIE_INCREASE_NECROMANCER_SKILL			:	if (m_wJob	==	dJOB_NECROMANCER	)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"네크로멘서 스킬 레벨 증가"
		case	eIE_INCREASE_DEMON_SKILL				:	if (m_wJob	==	dJOB_DEVIL			)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"악마 스킬 레벨 증가"
		case	eIE_INCREASE_SOUL_BRINGER_SKILL			:	if (m_wJob	==	dJOB_SOUL_BRINGER	)	m_correctAllSkillLevel.addValue(_iValue1);break;//	"영술사 스킬 레벨 증가"
		case	eIE_INCREASE_FEMALE_SKILL_LEVEL			:	if (!c_aIsMale[m_wJob]				)	m_correctAllSkillLevel.addValue(_iValue1);break;	//	"여성 캐릭터들의 스킬 레벨 증가"
		case	eIE_INCREASE_MALE_SKILL_LEVEL			:	if (c_aIsMale[m_wJob]				)	m_correctAllSkillLevel.addValue(_iValue1);break;	//	"남성 캐릭터들의 스킬 레벨 증가"
		case	eIE_INCREASE_SKILL_LEVEL				:	m_correctAllSkillLevel.addValue(_iValue1);break;//	"모든 스킬 레벨 증가"
		//┃스킬레벨
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

		case	eIE_WEAKNING_TARGET_FIRE_RESISTANCE		:
			m_weakningTargetFireResistance.addValue(_iValue1,dMAXIMUM_MAGIC_WEAKNING_VALUE);
			break;
		case	eIE_WEAKNING_TARGET_WATER_RESISTANCE	:
			m_weakningTargetWaterResistance.addValue(_iValue1,dMAXIMUM_MAGIC_WEAKNING_VALUE);
			break;
		case	eIE_WEAKNING_TARGET_WIND_RESISTANCE		:
			m_weakningTargetWindResistance.addValue(_iValue1,dMAXIMUM_MAGIC_WEAKNING_VALUE);
			break;
		case	eIE_WEAKNING_TARGET_EARTH_RESISTANCE	:
			m_weakningTargetEarthResistance.addValue(_iValue1,dMAXIMUM_MAGIC_WEAKNING_VALUE);
			break;
		case	eIE_WEAKNING_TARGET_LIGHT_RESISTANCE	:
			m_weakningTargetLightResistance.addValue(_iValue1,dMAXIMUM_MAGIC_WEAKNING_VALUE);
			break;
		case	eIE_WEAKNING_TARGET_DARK_RESISTANCE		:
			m_weakningTargetDarkResistance.addValue(_iValue1,dMAXIMUM_MAGIC_WEAKNING_VALUE);
			break;
		case	eIE_WEAKNING_TARGET_MAGIC_RESISTANCE	:
			m_weakningTargetMagicResistance.addValue(_iValue1,dMAXIMUM_MAGIC_WEAKNING_VALUE);
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
		case	eIE_INCREASE_ALL_STATUS				:
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

		case	eIE_ACL_SHIELD:
			{
				m_wCarvingOptionShieldChance	=	dACL_OPTION_SHIELD_FORMULA1(_iValue1);
				m_wCarvingOptionShieldValue		=	dACL_OPTION_SHIELD_FORMULA2(_iValue2);
				m_wCarvingOptionShieldValue		=	min(m_wCarvingOptionShieldValue , 100);
				break;
			}
			
			
		case	eIE_ACL_CHARGE:
			{
				m_wCarvingOptionChargeTime		=	dACL_OPTION_CHARGE_FORMULA1(_iValue1);
				m_wCarvingOptionChargeValue		=	dACL_OPTION_CHARGE_FORMULA2(_iValue2);
				break;
			}
		case eIE_ACL_INCREASE_EXP:
			{
				m_wCarvingOptionIncreaseExp		=	dACL_OPTION_INCREASE_EXP_FORMULA(_iValue1);
				break;
			}
		case eIE_ACL_ADD_FIRE_DAMAGE	:
			{
				m_wCarvingOptionAddFireDamageChance	=	_iValue1;
				m_iCarvingOptionAddFireDamage		=	dACL_OPTION_ADD_FIRE_DAMAGE(_iValue2);

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
		case	eIE_ACL_INCREASE_MAX_HP :
			{
				m_iCarvingOptionMaxHP	=	dACL_OPTION_INCREASE_MAX_HP_FORMULA1(_iValue1) * 100;
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
		case	eIE_INCREASE_LEVEL_GETING_RED_STONE :
			{
				m_iLevelGetingRedStone+=	_iValue1;
				break;
			}
	}
}

void
cACTOR::operatePrefixData(int _iEffect,int _iValue1,int _iValue2,int _iValue3,BOOL _bIsUnique)
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

		case	eIP_WEAKNING_TARGET_FIRE_RESISTANCE		:
			m_weakningTargetFireResistance.addValue(_iValue1,dMAXIMUM_MAGIC_WEAKNING_VALUE);
			break;
		case	eIP_WEAKNING_TARGET_WATER_RESISTANCE	:
			m_weakningTargetWaterResistance.addValue(_iValue1,dMAXIMUM_MAGIC_WEAKNING_VALUE);
			break;
		case	eIP_WEAKNING_TARGET_WIND_RESISTANCE		:
			m_weakningTargetWindResistance.addValue(_iValue1,dMAXIMUM_MAGIC_WEAKNING_VALUE);
			break;
		case	eIP_WEAKNING_TARGET_EARTH_RESISTANCE	:
			m_weakningTargetEarthResistance.addValue(_iValue1,dMAXIMUM_MAGIC_WEAKNING_VALUE);
			break;
		case	eIP_WEAKNING_TARGET_LIGHT_RESISTANCE	:
			m_weakningTargetLightResistance.addValue(_iValue1,dMAXIMUM_MAGIC_WEAKNING_VALUE);
			break;
		case	eIP_WEAKNING_TARGET_DARK_RESISTANCE		:
			m_weakningTargetDarkResistance.addValue(_iValue1,dMAXIMUM_MAGIC_WEAKNING_VALUE);
			break;
		case	eIP_WEAKNING_TARGET_MAGIC_RESISTANCE	:
			m_weakningTargetMagicResistance.addValue(_iValue1,dMAXIMUM_MAGIC_WEAKNING_VALUE);
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
		case	eIP_ADD_DEFENSE_PERCENTAGE				:	m_defensePower.m_sCorrectPercentageValueByPrefix+=	_iValue1;break;//	방어력 증감 (퍼센티지)
		case	eIP_ADD_DEFENSE							:	m_defensePower.m_sCorrectValueByPrefix+=_iValue1;break;//	방어력 증감 (수치)

		case	eIP_DECLINE_DEFENSE_PERCENTAGE			:	m_defensePower.m_sCorrectPercentageValueByPrefix-=	_iValue1;break;//	방어력 증감 (퍼센티지)
		case	eIP_DECLINE_DEFENSE						:	m_defensePower.m_sCorrectValueByPrefix-=_iValue1;break;//	방어력 증감 (수치)

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
		case	eIP_DECLINE_MOVE_SPEED					:	m_moveSpeed.addValue(-_iValue1);break;//	이동 속도
		case	eIP_DECLINE_ACTION_SPEED				:	m_actionSpeed.addValue(-_iValue1);break;//	공격 속도

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
			if (m_lpField->m_bIsGuildBattleField)
			{
				_iValue1	=	_iValue1*dPVP_BONUS_CP_FACTOR/100;
			}

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
		case	eIP_ALL_SKILL_LEVEL						:	m_correctAllSkillLevel.addValue(_iValue1);break;//	모든 스킬/마법 레벨 조절

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
		case	eIP_INCREASE_ALL_STATUS				:
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
		case	eIP_INCREASE_HARD_BLOW				:
			{
				m_correcthardBlowHitChance.addValue(_iValue1);			//	강타확률 증가..
				break;
			}
	}
}