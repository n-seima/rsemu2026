#include	<math.h>
#include	"CSkill.H"
#include	"cJOB.H"
#include	"CActor.H"
#include	"CImageData.H"
#include	"cSHAKE.H"
#include	"CFieldItem.H"
#include	"CSound.H"
#include	"CLoadTextCode.h"
#include	"miniPet.h"
#include "mess_sign.H"

//	short			m_sHitChance,m_sHitChancePerLevel,m_sFixHitCahnce;	//	명중률 보정,레벨당 명중률 보정,명중률

int		l_iEnableJob	=	0;

#ifdef	_IS_DEV_CLIENT
CDecoderKeyword	l_aSkillKeyword[]	=
{
	{	"end"				,dCODE_END}
};


CDecoderKeyword	l_aRequireEquipment[]	=	
{
	{	"없음"		,	eREQUIRE_EQUIPMENT_FOR_SKILL_NONE	},
	{	"무기"		,	eREQUIRE_EQUIPMENT_FOR_SKILL_WEAPON	},
	{	"방패"		,	eREQUIRE_EQUIPMENT_FOR_SKILL_SHIELD	},
	{	"탄환"		,	eREQUIRE_EQUIPMENT_FOR_SKILL_BULLET	},
	{	"무기2"		,	eREQUIRE_EQUIPMENT_FOR_SKILL_WEAPON2},
	{	"펫"		,	eREQUIRE_EQUIPMENT_FOR_SKILL_PET	},
	{	"소환수"	,	eREQUIRE_EQUIPMENT_FOR_SKILL_SUMMON_BEAST},
	{	"펫또는소환수",	eREQUIRE_EQUIPMENT_FOR_SKILL_PET_OR_BEAST},
	{	"물약"		,	eREQUIRE_EQUIPMENT_FOR_SKILL_POTION},
	{	"치료약"	,	eREQUIRE_EQUIPMENT_FOR_SKILL_DRUG},
	{	"꽃"		,	eREQUIRE_EQUIPMENT_FOR_SKILL_FLOWER},
	{	"사탕"		,	eREQUIRE_EQUIPMENT_FOR_SKILL_CANDY},
	
	{	"end",dCODE_END							}
};

//
//	기술 종류
CDecoderKeyword	l_aAttribute[]	=
{
	{	"물리"	,	eDAMAGE_TYPE_PHYSICAL		},
	{	"불"	,	eDAMAGE_TYPE_FIRE			},
	{	"물"	,	eDAMAGE_TYPE_WATER			},
	{	"바람"	,	eDAMAGE_TYPE_WIND			},
	{	"대지"	,	eDAMAGE_TYPE_EARTH			},
	{	"빛"	,	eDAMAGE_TYPE_LIGHT			},
	{	"어둠"	,	eDAMAGE_TYPE_DARK			},
};

CDecoderKeyword	l_aMonsterType[]	=
{
	{	"언데드형"	,	eJT_UNDEAD},
	{	"인간형"	,	eJT_HUMAN},
	{	"악마형"	,	eJT_DEVIL},
	{	"동물형"	,	eJT_ANIMAL},
	{	"신수형"	,	eJT_HOLY_BEAST},
	{	"모두"		,	eJT_ALL},
};

//	패시브 부가효과
CDecoderKeyword	l_aSkillPassiveEffect[]	=
{
	{	"집중력증가"			,eSKILL_PEE_INCREASE_CONCENTRATION			},	
	{	"강타확률증가"			,eSKILL_PEE_INCREASE_HARD_BLOW			},	
	{	"최대체력증가"			,eSKILL_PEE_INCREASE_MAX_HP			},	
	{	"상태이상저항증가"		,eSKILL_PEE_INCREASE_STRANGE_STATUS_RESISTANCE			},	
	{	"사정거리증가"			,eSKILL_PEE_INCREASE_RANGE			},	
	{	"마법저항증가"			,eSKILL_PEE_INCREASE_MAGIC_RESISTANCE			},	
	{	"빛최대대미지로최소대미지보정"					,eSKILL_PEE_CORRECT_LIGHT_MIN_DAMAGE			},	
	{	"비트부착확률증가"										,eSKILL_PEE_ADD_BIT_CHANCE			},	
	{	"맞을때상대방비트부착확률증가"							,eSKILL_PEE_ADD_BIT_CHANCE_WHEN_BE_HIT			},	
	{	"타격범위증가"							,eSKILL_PEE_INCREASE_HIT_RANGE			},	
	{	""						,dCODE_END									}
};
//
//	기술 종류
CDecoderKeyword	l_aSkillType[]	=
{
	{	"일반타격"				,eSKILL_TYPE_NORMAL_HIT						},
	{	"임팩트"				,eSKILL_TYPE_IMPACT							},
	{	"캐릭터에의한연속타격"	,eSKILL_TYPE_CONTINUOUS_HIT_BY_CHARACTER	},
	{	"수치에의한연속타격"	,eSKILL_TYPE_CONTINUOUS_HIT_VALUE			},
	{	"잔상에의한연속타격"	,eSKILL_TYPE_CONTINUOUS_HIT_BY_ILLUSION		},
	{	"분신공격"				,eSKILL_TYPE_ATTACK_BY_BUNSHIN				},
	{	"시미터커팅"			,eSKILL_TYPE_SCIMITAR_CUTTING				},
	{	"라운딩"				,eSKILL_TYPE_WHIRLWIND						},
	{	"러쉬"					,eSKILL_TYPE_RUSH							},
	{	"돌려찌르기"			,eSKILL_TYPE_WHIRLTHRUSTING					},
	{	"서프라이징레이드"		,eSKILL_TYPE_SURPRISING_RAID				},
	{	"휠러닝"				,eSKILL_TYPE_WHIRL_RUNNING					},
	{	"바운싱리니어"			,eSKILL_TYPE_BOUNCING_LINEAR				},

	{	"아이시스탈라그마이트"	,eSKILL_TYPE_ICY_STALAGMITE					},

	{	"미사일"				,eSKILL_TYPE_MISSILE						},
	{	"연사"					,eSKILL_TYPE_CONTINUOUS_SHOOT				},
	{	"폭발미사일"			,eSKILL_TYPE_EXPLOSION_MISSILE				},
	{	"관통미사일"			,eSKILL_TYPE_PIERCING_MISSILE				},
	{	"환상미사일"			,eSKILL_TYPE_ILLUSION_MISSILE				},
	{	"스프레드애로우"		,eSKILL_TYPE_SPREAD_ARROW					},
	{	"비트글라이더"			,eSKILL_TYPE_BIT_GLIDER						},
	{	"부메랑"				,eSKILL_TYPE_BOOMERANG						},

	{	"파이어볼"				,eSKILL_TYPE_FIRE_BALL						},
	{	"레이저"				,eSKILL_TYPE_LASER							},
	{	"폭발형"				,eSKILL_TYPE_EXPLOSION						},
	{	"이미지의존폭발형"		,eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE		},
	{	"직선형연쇄폭발"		,eSKILL_TYPE_LINER_CHAIN_EXPLOSION			},
	{	"랜드마커"				,eSKILL_TYPE_LAND_MARKER					},
	{	"인터벌슈터"			,eSKILL_TYPE_INTERVAL_SHOOTER				},
	{	"워터폴"				,eSKILL_TYPE_WATER_FALL						},
	{	"가디언포스트"			,eSKILL_TYPE_GUARDIAN_POST					},
	{	"레이디얼아크"			,eSKILL_TYPE_RADIAL_ARC						},
	{	"오우섬포트리스"		,eSKILL_TYPE_AWESOME_FORTRESS				},
	{	"바이트행잉"			,eSKILL_TYPE_BITE_HANGING					},

	{	"토네이도"				,eSKILL_TYPE_TORNADO						},
	{	"낙하"					,eSKILL_TYPE_DROP_ON_HEAD					},
	{	"충전"					,eSKILL_TYPE_CHARGE							},
	{	"치료"					,eSKILL_TYPE_HEAL							},
	{	"지속형리액션"			,eSKILL_TYPE_PASSIVE_REACTION				},
	{	"발동형리액션"			,eSKILL_TYPE_ACTIVITY_REACTION				},
	{	"댄싱블로커"			,eSKILL_TYPE_DANCING_BLOCKER				},
	{	"마법실드"				,eSKILL_TYPE_MAGIC_SHIELD					},
	{	"향상된블록킹"			,eSKILL_TYPE_ADVANCED_BLOCKING				},
	{	"오라"					,eSKILL_TYPE_AURA							},
	{	"머신건"				,eSKILL_TYPE_MACHINE_GUN					},
	{	"막기"					,eSKILL_TYPE_BLOCKING						},
	{	"회피"					,eSKILL_TYPE_DODGE							},
	{	"카운터"				,eSKILL_TYPE_COUNTER						},

	{	"텔레포트"				,eSKILL_TYPE_TELEPORT						},

	{	"보조마법"				,eSKILL_TYPE_AID_MAGIC						},
	{	"컴플리트프로텍션"		,eSKILL_TYPE_COMPLETE_PROTECTION			},
	{	"시머링실드"			,eSKILL_TYPE_SHIMMERING_SHIELD				},
	{	"테일체이서"			,eSKILL_TYPE_TAIL_CHASER					},
	{	"미러타워"				,eSKILL_TYPE_MIRROR_TOWER					},	//	
	{	"디스플레이스먼트"		,eSKILL_TYPE_DISPLACEMENT					},	//	

	{	"돌려일어나기"			,eSKILL_TYPE_SPIN_ARISING					},	//	
	{	"아머디스어셈블러"		,eSKILL_TYPE_ARMOR_DISASSEMBLER				},	//	
	{	"점프"					,eSKILL_TYPE_JUMP							},	//	
	{	"점프공격"				,eSKILL_TYPE_JUMP_ATTACK					},	//	
	{	"올터링히터"			,eSKILL_TYPE_ALTERING_HITTER				},	//	
	{	"와일드스탬프"			,eSKILL_TYPE_WILD_STAMP						},
	{	"보조공격"				,eSKILL_TYPE_AID_ATTACK						},
	{	"펫명령"				,eSKILL_TYPE_PET_COMMAND					},
	{	"더블타겟"				,eSKILL_TYPE_DOUBLE_TARGET					},
	{	"가스"					,eSKILL_TYPE_GAS							},
	{	"플로렌스스페셜"		,eSKILL_TYPE_FLORENCE_SPECIAL				},

	{	"데굴데굴통나무"		,eSKILL_TYPE_ROLLING_LOG					},
	{	"거대탄환"				,eSKILL_TYPE_BIG_BULLET						},
	{	"아이템던지기"			,eSKILL_TYPE_THROW_ITEM						},
	{	"라이트닝와인더"		,eSKILL_TYPE_LIGHTNING_WINDER				},

	{	"미러이미지"			,eSKILL_TYPE_MIRROR_IMAGE					},
	{	"체인라이트닝"			,eSKILL_TYPE_CHAIN_LIGHTNING				},
	{	"파이어월"				,eSKILL_TYPE_FIRE_WALL						},
	{	"기가라이트닝"			,eSKILL_TYPE_GIGA_LIGHTNING					},
	{	"한입"					,eSKILL_TYPE_ONE_BITE						},
	{	"미니펫조합"			,eSKILL_TYPE_MINIPET_COMBI					},
	{	"지속형라운딩"			,eSKILL_TYPE_CONTINUOUS_WHIRLWIND			},		// 지속형라운딩.	
	{	"멀티미사일"			,eSKILL_TYPE_MULTI_MISSILE			},		//멀티미사일.	
	{	"분신연속공격"			,eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER			},		//분신연속공격	
	{	"패시브"				,eSKILL_TYPE_PASSIVE			},		//패시브	
	{	"비트그룹연타"				,eSKILL_TYPE_GROUP_CONTINUOUS_BY_BIT			},		
	{	"바운싱미사일"				,eSKILL_TYPE_BOUNCING_MISSILE			},		
	{	"범위미사일"				,eSKILL_TYPE_WIDE_MISSILE			},	
	{	"end"					,dCODE_END									}
};

CDecoderKeyword	l_aCureEffect[]	=
{
	{	"이상계통상태이상아군치료"	,eCE_STRANGE_STATUS_CURE},		
	{	"저하계통상태이상아군치료"	,eCE_DECLINE_POWER_STATUS_CURE},	
	{	"저주계통상태이상아군치료"	,eCE_CURSE_STATUS_CURE},			
	{	"모든상태이상아군치료"		,eCE_ALL_STRANGE_STATUS_CURE},	
	{	"행동불능아군회복"			,eCE_RESURRECTION},				
	{	"end",dCODE_END}
};

CDecoderKeyword	l_aCastType[]	=
{
	{	"지속형리액션스킬"			,eST_PASSIVE	},
	{	"액션스킬"					,eST_NORMAL		},
	{	"발동형리액션스킬"			,eST_REACTION	},
	{	"서포트스킬"				,eST_SUPPORT	},
	{	"end"						,dCODE_END}
};

CDecoderKeyword	l_aGetCPTiming[]	=
{
	{"히트순간",eGET_CP_HIT},
	{"첫번째히트",eGET_CP_FIRST_HIT},
	{"막는순간",eGET_CP_DEFENSE},
	{"발동순간",eGET_ACTION},
	{"카운트순간",eGET_CP_COUNTER},
	{"행동종료",eGET_ACTION_FINISH},
/*
	
	{"착지순간",eGET_LAND},
	{"매번히트시",eGET_EVERY_HIT},
	{"사정권 내 적이 들어온 순간",eGET_ENNEMY_IN_ATTACK_RANGE},
	{"막는 순간,회피 순간",eGET_DEFENSE_DODGE},
	{"주위 아군 상태이상,마법 공격 받았을 시",eGET_PARTY_ATTACKED},
	{"적 하나 발견 할 때 마다",eGET_FIND_ENEMY},
	{"적 하나 파멸 할 때 마다",eGET_ENEMY_DEATH},*/
	{"end",dCODE_END}
};


CDecoderKeyword l_aNeedState[] = 
{
	{"교감", eNEED_STATE_CONSENSUS},
	{"투지", eNEED_STATE_FIGHTING_SPIRIT},
	{"end",	dCODE_END},
	

};

//	기술을 사용하는 방법
CDecoderKeyword	l_aSkillUseTo[]	=
{
	{	"적"		,	eSKILL_CAST_AT_ENEMY		},
	{	"아군"		,	eSKILL_CAST_AT_PLAYER		},
	{	"죽은아군"	,	eSKILL_CAST_AT_DEATH_PLAYER	},
	{	"목적지"	,	eSKILL_CAST_AT_GROUND		},
	{	"퀵"		,	eSKILL_CAST_QUICK			},
	{	"자신"		,	eSKILL_CAST_AT_CASTER		},
	{	"펫"		,	eSKILL_CAST_ON_PET			},
	{	"파티원"	,	eSKILL_CAST_ON_PARTY_MEMBER	},
	{	"유저파티원",	eSKILL_CAST_ON_USER_PARTY_MEMBER	},
	{	"적펫"		,	eSKILL_CAST_ON_ENEMY_PET	},
	{	"죽은펫"	,	eSKILL_CAST_ON_DEATH_PET	},
	{	"소환수"	,	eSKILL_CAST_ON_SUMMON_BEAST	},
	{	"켈비"		,	eSKILL_CAST_ON_KELBY		},
	{	"죽은소환수",	eSKILL_CAST_ON_DEATH_SUMMON_BEAST},

	{	"함정"		,	eSKILL_CAST_ON_TRAP},
	{	"보물상자"	,	eSKILL_CAST_ON_ARCA},
	{	"문"		,	eSKILL_CAST_ON_DOOR},
	{	"아군펫"	,	eSKILL_CAST_ON_PET},
	
	{	"end"		,	dCODE_END					}
};

CDecoderKeyword	l_aSplashEffect[]	=
{
	{	"red"		,	dPLT_EFFECT_RED		},
	{	"greed"		,	dPLT_EFFECT_GREEN	},
	{	"blue"		,	dPLT_EFFECT_BLUE	},
	{	"white"		,	dPLT_EFFECT_BRIGHT	},
	{	"end"		,	dCODE_END	}
};

CDecoderKeyword	l_aFlyType[]	=
{
	{	"liner"				,	dSA_LINER			},
	{	"homming"			,	dSA_HOMMING			},
	{	"bring_to_target"	,	dSA_BRING_TO_TARGET	},
	{	"부메랑"			,	dSA_BOOMERANG		},
	{	"end"				,	dCODE_END			}
};

CDecoderKeyword	l_aAfterImageType[]	=
{
	{	"없음"				,	eSKILL_AFTER_IMAGE_TYPE_NONE},
	{	"자기복제"			,	eSKILL_AFTER_IMAGE_SELF_COPY},
	{	"다음에니메이션"	,	eSKILL_AFTER_IMAGE_NEXT_ANIMATION},
	{	"프레임증가"		,	eSKILL_AFTER_IMAGE_INCREASE_FRAME},
	{	"워터캐논스타일"	,	eSKILL_AFTER_IMAGE_WATER_CANON_STYLE},
	{	"end"				,	dCODE_END			}
};

CDecoderKeyword	l_aSoundType[]	=
{
	{	"시전효과음"	,	eSSD_CASTING	},
	{	"액션효과음"	,	eSSD_ACTION		},
	{	"타격효과음"	,	eSSD_HIT		},
	{	"생성효과음"	,	eSSD_CREATE		},
	{	"폭발효과음"	,	eSSD_EXPLOSION	},
	{	"실패효과음"	,	eSSD_MISS		},
	{	"end"			,	dCODE_END		}
};


//int l_a
//0,0,0,1,1,1,2,2,2,3,4,4,4,5,5,6,6,6,8,8,8,9,9,9,10,10,10,12,12,12,13,13,13,14,14,14,15,15,15		
//	기술 효과
CDecoderKeyword	l_aSkillAction[][40]	=
{
	//	견습기사
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"수직 베기"			,8},
		{"수평 베기"			,9},
		{"사선 베기"			,10},
		{"찌르기"				,11},
		{"방패 공격"			,12},
		{"막기"					,13},
		{"러쉬"					,14},
		{"와일드 댄스"			,15},
		{"시미터 커팅"			,16},
		{"크라잉 넛"			,17},
		{"챠징"					,18},
		{"워크라이"				,19},
		{"end"					,dCODE_END	}
	},
	//	전사
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"내려치기"				,8},
		{"수평 베기"			,9},
		{"찌르기"				,10},
		{"점프 공격"			,11},
		{"드래곤 트위스트"		,12},
		{"딜레이 크러싱"		,13},
		{"이단 내려치기"		,14},
		{"라운딩 붐"			,15},
		{"점프"					,16},
		{"막기"					,17},
		{"점프공격"				,18},
		{"카운터"				,19},
		{"end"					,dCODE_END	}
	},
	//	마법사
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"휘두르기"				,8},
		{"치켜들기"				,9},
		{"차징"					,10},
		{"파이어볼 발사"		,11},
		{"end"					,dCODE_END	}
	},
	//	늑대인간
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"후려치기"				,8},
		{"물어뜯기"				,9},
		{"회전공격"				,10},
		{"울부짖기"				,11},
		{"플래쉬 태클"			,12},
		{"버닝 오브"			,13},
		{"end"					,dCODE_END	}
	},
	//	사제
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"후려치기"				,8},
		{"치켜들기"				,9},
		{"기도"					,10},
		{"안수"					,11},
		{"막기"					,12},
		{"가즈 핸드"			,13},
		{"end"					,dCODE_END	}
	},
	//	타락천사
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"공격"					,8},
		{"날개짓"				,9},
		{"축도"					,10},
		{"구원"					,11},
		{"회상"					,12},
		{"1초"					,13},
		{"0.8초"				,14},
		{"end"					,dCODE_END	}
	},
	//	도둑
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},
		
		{"단도 던지기"			,8},
		{"왼손 단도"			,9},
		{"함정 설치"			,10},
		{"치료"					,11},
		{"백 스탭"				,12},
		{"왼손 펀치"			,13},
		{"오른쪽 팔꿈치"		,14},

		{"이단 앞 차기"			,15},
		{"오른발 돌려차기"		,16},
		{"발걸어 후리기"		,17},

		{"날아 찍기"			,18},
		{"내려찍기"				,19},
		{"앞차기"				,20},
		{"삼단 돌려차기"		,21},
		{"반격 턱 때리기"		,22},
		{"화살잡기"				,23},
		{"충전"					,24},
		{"장풍"					,25},
		{"분신"					,26},
		{"다팅 피버"			,27},
		{"함정 해체"			,28},
		{"캐스팅"				,29},

		{"end"					,dCODE_END	}
	},
	//	무도가
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"단도 던지기"			,8},
		{"왼손 단도"			,9},
		{"함정 설치"			,10},
		{"치료"					,11},
		{"백 스탭"				,12},
		{"왼손 펀치"			,13},
		{"오른쪽 팔꿈치"		,14},

		{"이단 앞 차기"			,15},
		{"오른발 돌려차기"		,16},
		{"발걸어 후리기"		,17},

		{"날아 찍기"			,18},
		{"내려찍기"				,19},
		{"앞차기"				,20},
		{"삼단 돌려차기"		,21},
		{"반격 턱 때리기"		,22},
		{"화살잡기"				,23},
		{"충전"					,24},
		{"장풍"					,25},
		{"분신"					,26},
		{"다팅 피버"			,27},
		{"함정 해체"			,28},
		{"캐스팅"				,29},

		{"end"					,dCODE_END	}
	},
	//	마법 창검사
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"찌르기"				,8},
		{"투창"					,9},
		{"창 돌리기"			,10},
		{"창 땅에 박기"			,11},
		{"돌리며 달리기"		,12},
		{"충전"					,13},
		{"막기"					,14},
		{"카운터"				,15},
		{"돌려 찌르기"			,16},
		{"end"					,dCODE_END	}
	},
	//	마법 궁수
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"화살 쏘기"			,8},
		{"활 올려치기"			,9},
		{"수직 화살쏘기"		,10},
		{"충전"					,11},
		{"스나이프"				,12},
		{"연사"					,13},
		{"수직 연사"			,14},
		{"end"					,dCODE_END	}
	},
	//	조련사
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"피리 불기"			,8},
		{"명령"					,9},
		{"소환"					,10},
		{"충전"					,11},
		{"end"					,dCODE_END	}
	},
	//	소환사
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"피리 불기"			,8},
		{"명령"					,9},
		{"소환"					,10},
		{"충전"					,11},
		{"end"					,dCODE_END	}
	},
	//	공주
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},
		{"슬링 던지기"			,8},
		{"슬링 따귀"			,9},
		{"슬링 돌리기"			,10},
		{"울기"					,11},
		{"깡총거리기"			,12},
		{"우는 척 하기"			,13},
		{"수다 떨기"			,14},
		{"거대 탄환"			,15},
		{"거대 탄환 캐스팅"		,16},
		{"수다 충전"			,17},

		{"end"					,dCODE_END	}
	},
	//	마법소녀
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"휘두르기"				,8},
		{"노래 부르기"			,9},
		{"그림 그리기"			,10},
		{"위로 들기"			,11},
		{"가슴 내밀기"			,12},
		{"기간틱 완드"			,13},
		{"변신"					,14},
		{"변신x변신"			,15},
		{"걸스 파라다이스"		,16},
		{"라이트닝 와인더"		,17},
		{"매지컬 드로잉"		,18},

		{"end"					,dCODE_END	}
	},
	//	강신술사
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"공격"					,8},
		{"마법"					,9},
		{"욕설"					,10},
		{"end"					,dCODE_END	}
	},
	//	악마
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"채찍 휘두르기"		,8},
		{"채찍 감기"			,9},
		{"주문"					,10},
		{"키스"					,11},
		{"꿈틀거림"				,12},

		{"end"					,dCODE_END	}
	},

	//	영술사
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"충전1"			,8},
		{"충전2"			,9},
		{"베기1"			,10},
		{"낫소환"			,11},
		{"낫던지기"			,12},
		{"회전베기"			,13},
		{"end"					,dCODE_END	}
	},
	//	투사
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"베기1"				,8},
		{"베기2"			,9},
		{"투기"				,10},
		{"발차기"			,11},
		{"충전"					,12},
		{"점프러쉬"			,	13},
		{"end"					,dCODE_END	}
	},

	{ // 광학사
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"기본공격"				,8},
		{"특수공격01"			,9},
		{"특수공격02"				,10},
		{"특수공격03"			,11},
		{"차징"					,12},
		{"end"					,dCODE_END	}
	},	//	18
	{ // 수인
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"기본공격"				,8},
		{"특수공격01"			,9},
		{"특수공격02"				,10},
		{"특수공격03"			,11},
		{"차징"					,12},
		{"end"					,dCODE_END	}
	},	//	19
	{	{"",0xffff},	},	//	20
	{	{"",0xffff},	},	//	21
	{	{"",0xffff},	},	//	22
	{	{"",0xffff},	},	//	23
	{	{"",0xffff},	},	//	24
	{	{"",0xffff},	},	//	25
	{	{"",0xffff},	},	//	26
	{	{"",0xffff},	},	//	27
	{	{"",0xffff},	},	//	28
	{	{"",0xffff},	},	//	29
	{	{"",0xffff},	},	//	28
	{	{"",0xffff},	},	//	30
	{	{"",0xffff},	},	//	31

	//	몬스터
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"액션1"				,8},
		{"액션2"				,9},
		{"액션3"				,10},
		{"액션4"				,11},
		{"액션5"				,12},
		{"액션6"				,13},
		{"액션7"				,14},
		{"액션8"				,15},
		{"액션9"				,16},
		{"end"					,dCODE_END	}
	},

	//	소환수
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"액션1"				,8},
		{"액션2"				,9},
		{"액션3"				,10},
		{"액션4"				,11},
		{"액션5"				,12},
		{"액션6"				,13},
		{"액션7"				,14},
		{"액션8"				,15},
		{"액션9"				,16},
		{"end"					,dCODE_END	}
	},
	//	모두
	{
		{"없음"					,0xffff},
		{"걷기"					,0},
		{"뛰기"					,1},
		{"정지"					,2},
		{"맞기"					,3},
		{"변신1"				,4},
		{"변신2"				,5},
		{"죽음"					,6},
		{"앉기"					,7},

		{"액션1"				,8},
		{"액션2"				,9},
		{"액션3"				,10},
		{"액션4"				,11},
		{"액션5"				,12},
		{"액션6"				,13},
		{"액션7"				,14},
		{"액션8"				,15},
		{"액션9"				,16},
		{"end"					,dCODE_END	}
	},
};

CDecoderKeyword	l_aJobNameKeyword[128]	=
{
	{	"기사"			,dJOB_KNIGHT		},	{	"전사"			,dJOB_WARRIOR		},
	{	"마법사"		,dJOB_WIZARD		},	{	"늑대인간"		,dJOB_WEREWOLF		},
	{	"사제"			,dJOB_PRIEST		},	{	"타락천사"		,dJOB_FALLEN_ANGEL	},
	{	"도둑"			,dJOB_ROGUE			},	{	"무도가"		,dJOB_FIGHTER		},
	{	"마법창병"		,dJOB_MAGIC_LANCER	},	{	"마법궁수"		,dJOB_MAGIC_ARCHER	},
	{	"조련사"		,dJOB_BEAST_TAMER	},	{	"소환사"		,dJOB_SUMMONER		},
	{	"공주"			,dJOB_PRINCESS		},	{	"변신 소녀"		,dJOB_MAGICAL_GIRL	},
	{	"네크로멘서"	,dJOB_NECROMANCER	},	{	"악마"			,dJOB_DEVIL			},
	{	"영술사"		,dJOB_SOUL_BRINGER	},	{	"투사"			,dJOB_CHAMPION	},
	{	"없음"			,dJOB_VALID			},	{	"몬스터"		,dJOB_CODE_MONSTER	},
	{	"광학사"		,dJOB_OPTICALIST	},	{	"수인"			,dJOB_BEAST_MAN	},
	{	"소환수"		,dJOB_CODE_SUMMON_BEAST},	{	"모두"			,dJOB_CODE_ALL	},
	{	""				,dCODE_END			}
};

CDecoderKeyword	l_aBookColor[10]	=
{
	{	"붉은색"		,eSage_book_color_red			},
	{	"하얀색_푸른색"	,eSage_book_color_white_blue	},
	{	"푸른색"		,eSage_book_color_blue			},
	{	""				,dCODE_END						}
};

//
//	스킬 부가효과
CDecoderKeyword	l_aSkillExtraEffect[]	=
{
	{	"혼란"				,eSKILL_EE_CONFUSE							},	//	혼란
	{	"갑옷파괴"			,eSKILL_EE_BREAK_ARMOR						},	//	갑옷파괴
	{	"무기파괴"			,eSKILL_EE_BREAK_WEAPON						},	//	무기파괴
	{	"경직"				,eSKILL_EE_STUN								},	//	경직
	{	"프리즈"			,eSKILL_EE_FREEZE							},	//	프리즈
	{	"콜드"				,eSKILL_EE_COLD								},	//	콜드

	{	"장님"				,eSKILL_EE_BLIND							},	//	장님
	{	"장님by빛"			,eSKILL_EE_BLIND_BY_LIGHT					},	//	장님

	{	"명중률저하"		,eSKILL_EE_DECLINE_HIT_CHANCE				},	//	명중률저하
	{	"회피율저하"		,eSKILL_EE_DECLINE_DODGE_CHANCE				},	//	회피율저하
	{	"공격력저하"		,eSKILL_EE_DECLINE_ATTACK_POWER				},	//	공격력 저하
	{	"공격력저하퍼센티지",eSKILL_EE_DECLINE_ATTACK_POWER_PERCENTAGE	},	//	공격력 저하
	{	"명중률저하by빛"	,eSKILL_EE_DECLINE_HIT_CHANCE_BY_LIGHT		},	//	명중률저하
	{	"회피율저하by빛"	,eSKILL_EE_DECLINE_DODGE_CHANCE_BY_LIGHT	},	//	회피율저하

	{	"빛공격부가효과"	,eSKILL_EE_LIGHT_ATTACK_ADDITIONAL_EFFECT	},	//	회피율저하
	{	"저주증폭"			,eSKILL_EE_CURSE_AMPLIFICATION				},	//	회피율저하

	{	"더미생성"			,eSKILL_EE_CREATE_DUMMY						},	//	더미 생성
	{	"이동속도저하"		,eSKILL_EE_DECLINE_MOVE_SPEED				},	//	이동 속도 저하
	{	"공격속도저하"		,eSKILL_EE_DECLINE_ATTACK_SPEED				},	//	이동 속도 저하

	{	"명중률증가"		,eSKILL_EE_INCLINE_HIT_CHANCE				},	//	명중률저하
	{	"회피율증가"		,eSKILL_EE_INCLINE_DODGE_CHANCE				},	//	회피율증가
	{	"공격력증가"		,eSKILL_EE_INCLINE_ATTACK_POWER				},	//	공격력증가
	{	"공격력증가퍼센티지",eSKILL_EE_INCLINE_ATTACK_POWER_PERCENTAGE	},	//	공격력증가
	{	"이동속도증가"		,eSKILL_EE_INCLINE_MOVE_SPEED				},	//	이동 속도 증가
	{	"공격속도증가"		,eSKILL_EE_INCLINE_ATTACK_SPEED				},	//	공격 속도 증가
	{	"공중부양"			,eSKILL_EE_LEVITATE							},	//	공중부양

	{	"이상계저항증가"	,eSKILL_EE_INCLINE_STRANGE_STATUS_RESISTANCE},	//	이상상태 저항 증가
	{	"저하계저항증가"	,eSKILL_EE_INCLINE_DECLINE_POWER_RESISTANCE	},	//	능력치 저하 저항 증가
	{	"저주계저항증가"	,eSKILL_EE_INCLINE_CURSE_RESISTANCE			},	//	저주 저항 증가

	{	"이상계저항감소"	,eSKILL_EE_INCLINE_STRANGE_STATUS_RESISTANCE},	//	이상상태 저항 증가
	{	"저하계저항감소"	,eSKILL_EE_INCLINE_DECLINE_POWER_RESISTANCE	},	//	능력치 저하 저항 증가
	{	"저주계저항감소"	,eSKILL_EE_INCLINE_CURSE_RESISTANCE			},	//	저주 저항 증가

	{	"마법저항감소"		,eSKILL_EE_DECLINE_MAGIC_RESISTANCE			},	//	마법 저항 감소
	{	"타겟마법저항감소"	,eSKILL_EE_DECLINE_TARGET_MAGIC_RESISTANCE	},	//	타겟마법저항감소
	{	"불저항감소"		,eSKILL_EE_DECLINE_FIRE_RESISTANCE			},	//	불저항 감소
	{	"물저항감소"		,eSKILL_EE_DECLINE_WATER_RESISTANCE			},	//	물저항 감소
	{	"대지저항감소"		,eSKILL_EE_DECLINE_EARTH_RESISTANCE			},	//	대지저항 감소
	{	"바람저항감소"		,eSKILL_EE_DECLINE_WIND_RESISTANCE			},	//	바람저항 감소
	{	"빛저항감소"		,eSKILL_EE_DECLINE_LIGHT_RESISTANCE			},	//	빛저항 감소
	{	"어둠저항감소"		,eSKILL_EE_DECLINE_DARK_RESISTANCE			},	//	어둠저항 감소

	{	"마법저항증가"		,eSKILL_EE_INCLINE_MAGIC_RESISTANCE			},	//	마법 저항 증가
	{	"불저항증가"		,eSKILL_EE_INCLINE_FIRE_RESISTANCE			},	//	불저항 증가
	{	"물저항증가"		,eSKILL_EE_INCLINE_WATER_RESISTANCE			},	//	물저항 증가
	{	"대지저항증가"		,eSKILL_EE_INCLINE_EARTH_RESISTANCE			},	//	대지저항 증가
	{	"바람저항증가"		,eSKILL_EE_INCLINE_WIND_RESISTANCE			},	//	바람저항 증가
	{	"빛저항증가"		,eSKILL_EE_INCLINE_LIGHT_RESISTANCE			},	//	빛저항 증가
	{	"어둠저항증가"		,eSKILL_EE_INCLINE_DARK_RESISTANCE			},	//	어둠저항 증가
	{	"4대원소저항증가"	,eSKILL_EE_INCLINE_4ELEMENT_RESISTANCE		},	//	어둠저항 증가

	{	"집중력증가"		,eSKILL_EE_INCLINE_CONCENTRATION			},	//	집중력 증가
	{	"집중력감소"		,eSKILL_EE_DECLINE_CONCENTRATION			},	//	집중력 감소
	
	{	"집중력증가by스텔라피어"	,eSKILL_EE_INCLINE_CONCENTRATION_BY_STELLAR_PEER	},	//	집중력 증가

	{	"힘증가"			,eSKILL_EE_INCLINE_POWER					},	//	힘증가"	
	{	"민첩성증가"		,eSKILL_EE_INCLINE_AGILITY					},	//	민첩성증가"
	{	"건강증가"			,eSKILL_EE_INCLINE_CONSTITUTION				},	//	건강증가"	
	{	"카리스마증가"		,eSKILL_EE_INCLINE_CHARISMA					},	//	카리스마증가"	
	{	"지식증가"			,eSKILL_EE_INCLINE_INTELIGENCE				},	//	지식증가"	
	{	"지혜증가"			,eSKILL_EE_INCLINE_WISDOM					},	//	지혜증가"	
	{	"운증가"			,eSKILL_EE_INCLINE_LUCK						},	//	운증가"	

	{	"최대CP증가"		,eSKILL_EE_INCLINE_MAX_CP					},	//	CP증가"	
	{	"최대체력증가"		,eSKILL_EE_INCLINE_MAX_HP					},	//	체력증가"	
	{	"최대체력증가퍼센티지"	,eSKILL_EE_INCLINE_MAX_HP_PERCENTAGE		},	//	체력증가"	
	
	{	"체력자동회복"		,eSKILL_EE_REGEN_HP							},	//	체력자동회복"	
	
	{	"녹백"				,eSKILL_EE_NOCK_BACK						},	//	집중력 증가

	{	"최종방어력감소퍼센티지"	,eSKILL_EE_DECREASE_FINAL_DEFENSE_POWER_PERCENTAGE	},	//	최종방어력감소퍼센티지

	{	"방어력감소"		,eSKILL_EE_DECREASE_DEFENSE_POWER			},	//	방어력 감소
	{	"방어력감소퍼센티지",eSKILL_EE_DECREASE_DEFENSE_POWER_PERCENTAGE},	//	집중력 증가

	{	"방어력증가"		,eSKILL_EE_INCREASE_DEFENSE_POWER			},	//	방어력 증가
	{	"방어력증가퍼센티지",eSKILL_EE_INCREASE_DEFENSE_POWER_PERCENTAGE},	//	집중력 증가

	{	"이동불가by대지"	,eSKILL_EE_CAN_NOT_MOVE_BY_EARTH			},	//	이동불가 by 대지
	{	"이동불가"			,eSKILL_EE_CAN_NOT_MOVE						},	//	이동불가
	{	"이동속도저하by대지",eSKILL_EE_DECLINE_MOVE_SPEED_BY_EARTH		},	//	이동속도 저하 by 대지
	{	"경직by대지"		,eSKILL_EE_STUN_BY_EARTH					},	//	경직 by 대지
	{	"석화by대지"		,eSKILL_EE_STONE_BY_EARTH					},	//	석화 by 대지

	{	"추가불대미지"		,eSKILL_EE_ADDITIONAL_FIRE_DAMAGE			},	//	석화 by 대지
	{	"추가물대미지"		,eSKILL_EE_ADDITIONAL_WATER_DAMAGE			},	//	석화 by 대지
	{	"추가바람대미지"	,eSKILL_EE_ADDITIONAL_WIND_DAMAGE			},	//	석화 by 대지
	{	"추가대지대미지"	,eSKILL_EE_ADDITIONAL_EARTH_DAMAGE			},	//	석화 by 대지
	{	"추가빛대미지"		,eSKILL_EE_ADDITIONAL_LIGHT_DAMAGE			},	//	석화 by 대지
	{	"추가어둠대미지"	,eSKILL_EE_ADDITIONAL_DARK_DAMAGE			},	//	석화 by 대지

	{	"불대미지"			,eSKILL_EE_FIRE_DAMAGE						},	//	
	{	"물대미지"			,eSKILL_EE_WATER_DAMAGE						},	//	
	{	"바람대미지"		,eSKILL_EE_WIND_DAMAGE						},	//	
	{	"대지대미지"		,eSKILL_EE_EARTH_DAMAGE						},	//	
	{	"빛대미지"			,eSKILL_EE_LIGHT_DAMAGE						},	//	
	{	"어둠대미지"		,eSKILL_EE_DARK_DAMAGE						},	//	
	
	{	"불대미지실드"		,eSKILL_EE_FIRE_DAMAGE_SHIELD				},
	{	"물대미지실드"		,eSKILL_EE_WATER_DAMAGE_SHIELD				},
	{	"바람대미지실드"	,eSKILL_EE_WIND_DAMAGE_SHIELD				},
	{	"대지대미지실드"	,eSKILL_EE_EARTH_DAMAGE_SHIELD				},
	{	"빛대미지실드"		,eSKILL_EE_LIGHT_DAMAGE_SHIELD				},
	{	"어둠대미지실드"	,eSKILL_EE_DARK_DAMAGE_SHIELD				},

	{	"도망by턴언데드"	,eSKILL_EE_FLEE_BY_TURNUNDEAD				},
	{	"마비by턴언데드"	,eSKILL_EE_STUN_BY_TURNUNDEAD				},
	{	"즉사by턴언데드"	,eSKILL_EE_INSTANCE_KILL_TURNUNDEAD			},
	{	"디스트로잉언데드"	,eSKILL_EE_DESTROYING_UNDEAD				},
	{	"디스트로잉언홀리"	,eSKILL_EE_DESTROYING_UNHOLY				},

	{	"중독"				,eSKILL_EE_POISON							},
	{	"중독치료"			,eSKILL_EE_CURE_POISON						},
	
	{	"동료에의한공격력상승",eSKILL_EE_INCLINE_BY_OWNER_FORCE			},
	{	"성향변경"			,eSKILL_EE_CHANGE_ALLIGNMENT				},
	{	"체력흡수"			,eSKILL_EE_LIFE_DRAIN						},
	{	"체력회복"			,eSKILL_EE_RECOVER_HP						},
	{	"잠"				,eSKILL_EE_SLEEP							},

	{	"이상계통상태이상아군치료"	,eSKILL_EE_CURE_STRANGE_STATUS		},
	{	"저주계통상태이상아군치료"	,eSKILL_EE_CURE_CURSE				},
	{	"저하계통상태이상아군치료"	,eSKILL_EE_CURE_DECLINE_POWER_STATUS	},
	{	"이상계통상태이상지속시간감소"	,eSKILL_EE_REDUCE_STRANGE_STATUS_TIME		},
	{	"저주계통상태이상지속시간감소"	,eSKILL_EE_REDUCE_CURSE_TIME				},
	{	"저하계통상태이상지속시간감소"	,eSKILL_EE_REDUCE_DECLINE_POWER_STATUS_TIME	},

	{	"빙의"				,eSKILL_EE_DISPLACEMENT						},
	{	"디텍팅"			,eSKILL_EE_DETECTING						},
	{	"디텍팅이블"		,eSKILL_EE_DETECTING_EVIL					},

	{	"디스펠링"			,eSKILL_EE_DISPELLING						},
	{	"CP감소"			,eSKILL_EE_REDUCE_CP						},
	{	"CP감소퍼센티지"	,eSKILL_EE_REDUCE_CP_PERCENTAGE				},
	{	"유혹"				,eSKILL_EE_CHARMING							},
	{	"생츄어리"			,eSKILL_EE_SANCTUARY						},
	{	"가까운마을로이동"	,eSKILL_EE_WARP_NEAREST_VILLAGE				},
	{	"콜링"				,eSKILL_EE_CALLING							},
	{	"타운포탈"			,eSKILL_EE_CREATE_TOWN_PORTAL				},
	{	"레벨드레인"		,eSKILL_EE_LEVEL_DRAIN						},
	{	"블럭률저하"		,eSKILL_EE_DECREASE_BLOCKING_CHANCE			},

	{	"유혹저항"			,eSKILL_EE_CHARMING_RESISTANCE				},
	{	"조련"				,eSKILL_EE_TAME								},
	{	"도망"				,eSKILL_EE_FLEE								},
	{	"타겟제외"			,eSKILL_EE_NONE_TARGET						},	//	타겟 제외 상태
	{	"타겟재설정"		,eSKILL_EE_REAIM_TARGET						},	//	타겟 재 설정
	{	"응급처치"			,eSKILL_EE_FIRST_AID						},	//	응급 처치
	{	"레벨증가"			,eSKILL_EE_INCREASE_LEVEL					},	//	레벨 증가
	{	"사육기록"			,eSKILL_EE_BREEDING_RECORD					},	//	사육기록
	{	"경험치보너스"		,eSKILL_EE_BONUS_EXP						},	//	경험치보너서
	{	"펫능력치버스트"	,eSKILL_EE_PET_BOOST						},	//	경험치보너서

	{	"불공격력강화"		,eSKILL_EE_STRENGTHENING_FIRE_DAMAGE		},
	{	"물공격력강화"		,eSKILL_EE_STRENGTHENING_WATER_DAMAGE		},
	{	"바람공격력강화"	,eSKILL_EE_STRENGTHENING_WIND_DAMAGE		},
	{	"대지공격력강화"	,eSKILL_EE_STRENGTHENING_EARTH_DAMAGE		},
	{	"빛공격력강화"		,eSKILL_EE_STRENGTHENING_LIGHT_DAMAGE		},
	{	"어둠공격력강화"	,eSKILL_EE_STRENGTHENING_DARK_DAMAGE		},
	{	"마법공격력강화"	,eSKILL_EE_STRENGTHENING_MAGIC_DAMAGE		},

	{	"공격력증가퍼센티지for펫커맨드",eSKILL_EE_INCLINE_ATTACK_POWER_PERCENTAGE_FOR_PET_COMMAND},
	{	"방어력증가퍼센티지for펫커맨드",eSKILL_EE_INCLINE_DEFENSE_POWER_PERCENTAGE_FOR_PET_COMMAND},
	{	"펫커맨드공방초기화",eSKILL_EE_RESET_ATTACK_DEFENSE_POWER_FOR_PET_COMMAND},

	{	"유혹저항력저하"	,eSKILL_EE_DECLINE_CHARMING_RESISTANCE		},
	{	"모이기"			,eSKILL_EE_RALLY							},
	{	"펫유혹"			,eSKILL_EE_CHARMING_PET						},

	{	"소환수소환"		,eSKILL_EE_CALL_SUMMON_BEAST				},
	{	"소환수파워업"		,eSKILL_EE_POWER_UP_SUMMON_BEAST			},
	{	"개타기"			,eSKILL_EE_RIDING_DOG						},
	{	"스킬레벨증가"		,eSKILL_EE_INCREASE_SKILL_LEVEL				},
	{	"함정설치"			,eSKILL_EE_SET_TRAP							},
	{	"가시돋힘"			,eSKILL_EE_ADD_THORN						},
	
	{	"치명타확률상승"	,eSKILL_EE_INCLINE_CRITICAL_CHANCE			},
	{	"남은체력비례대미지",eSKILL_EE_PHYSICAL_DAMAGE_BASED_REMAIN_HP	},
	{	"보물찾기"			,eSKILL_EE_FINDING_TEASURE					},
	{	"아이템컬렉터"		,eSKILL_EE_ITEM_COLLECTOR					},
	{	"골드홀더"			,eSKILL_EE_GOLD_HOLDER						},

	{	"무기공격력퍼센티지감소",eSKILL_EE_DECLINE_WEAPON_ATTACK_POWER_PERCENTAGE},
	{	"무기공격력증가"	,eSKILL_EE_INCLINE_WEAPON_ATTACK_POWER		},
	{	"방어구방어력증가"	,eSKILL_EE_INCLINE_ARMOR_DEFENSE_POWER		},
	{	"타운트"			,eSKILL_EE_TAUNT							},
	{	"펫타운트"			,eSKILL_EE_PET_TAUNT						},

	{	"마비저항증가"		,eSKILL_EE_INCREASE_STUN_RESISTANCE			},
	{	"물리대미지감소"	,eSKILL_EE_DECREASE_PHYSICAL_DAMAGE			},
	{	"무적"				,eSKILL_EE_INVINCIBLE						},
	{	"회피1회"			,eSKILL_EE_DODGE_THIS_TIME_ATTACK			},
	{	"분신"				,eSKILL_EE_MAKE_THE_OTHER_SELF				},
	
	{	"녹백저항"			,eSKILL_EE_RESISTANCE_KNOCK_BACK			},
	{	"녹아웃저항"		,eSKILL_EE_RESISTANCE_KNOCK_OUT				},
	{	"오른손스킬레벨증가",eSKILL_EE_INCREASE_RIGHT_PUNCH_SKILL_LEVEL	},
	{	"왼손스킬레벨증가"	,eSKILL_EE_INCREASE_LEFT_PUNCH_SKILL_LEVEL	},
	{	"오른발스킬레벨증가",eSKILL_EE_INCREASE_RIGHT_KICK_SKILL_LEVEL	},
	{	"왼발스킬레벨증가"	,eSKILL_EE_INCREASE_LEFT_KICK_SKILL_LEVEL	},
	{	"아래차기스킬레벨증가",eSKILL_EE_INCREASE_DOWN_KICK_SKILL_LEVEL	},

	{	"독중화"			,eSKILL_EE_NEUTRALIZE_POISON				},
	{	"마비얼음결정중화"	,eSKILL_EE_NEUTRALIZE_FREEZE_STUN			},

	{	"디텍팅몬스터"		,eSKILL_EE_DETECTING_MONSTER				},
	{	"인비저블"			,eSKILL_EE_INVISIBLE						},

	{	"즉사"				,eSKILL_EE_INSTANCE_KILL					},

	{	"함정해체"			,eSKILL_EE_DISARM_TRAP						},
	{	"함정탐지"			,eSKILL_EE_DETECT_TRAP						},
	{	"비밀오브젝트탐지"	,eSKILL_EE_DETECT_SECRET_OBJECT				},
	{	"잠긴상자열기"		,eSKILL_EE_UNLOCK_ARCA						},
	{	"잠긴문열기"		,eSKILL_EE_UNLOCK_DOOR						},
	{	"소매치기"			,eSKILL_EE_PICKPOCKET						},
	{	"아이템강탈"		,eSKILL_EE_ROBBERY							},
	{	"죽음의예고장"		,eSKILL_NOTIFICATION_OF_DEATH				},
	{	"돈훔치기"			,eSKILL_EE_STEAL_MONEY						},
	{	"이번공격대미지최소",eSKILL_EE_MIN_DAMAGE_FOR_THIS_TIME_ATTACK	},
	{	"에니메이트파트너"	,eSKILL_EE_ANIMATE_PARTNER					},
	{	"고문"				,eSKILL_EE_TORTURE							},
	{	"스킬영역생성"		,eSKILL_EE_MAKE_SKILL_ZONE					},
	{	"족쇄생성"			,eSKILL_EE_MAKE_SHAKLE						},
	{	"잠으로인한어둠대미지",eSKILL_EE_PERIODICAL_DARK_DAMAGE_BY_SLEEP},
	{	"얼티밋배리어"		,eSKILL_EE_ULTIMATE_BARRIER					},
	{	"사랑의전서구"		,eSKILL_EE_PIGEON_POST						},
	{	"버서크"			,eSKILL_EE_BERSERK							},
	{	"춤추기"			,eSKILL_EE_DANCE							},
	{	"매직박스"			,eSKILL_EE_MAGIC_BOX						},
	{	"복제"				,eSKILL_EE_COPY								},

	{	"개구리변신"		,eSKILL_EE_TRANS_FROG						},
	{	"개구리변신2"		,eSKILL_EE_TRANS_FROG2						},
	{	"비만증"			,eSKILL_EE_TRANS_FAT_GIRL					},
	{	"무기변신"			,eSKILL_EE_TRANS_TO_WEAPON					},
	{	"토끼변신"			,eSKILL_EE_TRANS_TO_RABBIT					},
	{	"깡총거리기"		,eSKILL_EE_JUMP_JUMP						},
	{	"죽은척하기"		,eSKILL_EE_FEIGN_DEATH						},
	{	"죽은척하면서공격하기",eSKILL_EE_ATTACK_UNDER_FEIGN_DATH		},
	{	"튕기기"			,eSKILL_EE_BOUNCE							},
	{	"모든저항력퍼센트저하"	,eSKILL_EE_DECREASE_ALL_RESISTANCE		},
	{	"병터지기"			,eSKILL_EE_EXPLOSION_BOTTLE					},
	{	"CP충전"			,eSKILL_EE_CHARGE_CP						},	//	CP충전"	
	{	"마리오네트"		,eSKILL_EE_MARIONETTE						},	//	마리오네트

	{	"능력치변환"		,eSKILL_EE_TRANS_POWER						},	//	능력치변환"	
	{	"환영생성"			,eSKILL_EE_MAKE_ILLUSION					},	
	{	"대미지풀"			,eSKILL_EE_DAMAGE_POOL						},	
	{	"충동"				,eSKILL_EE_IMPULSE							},	
	{	"피흡혈"			,eSKILL_EE_BLOOD_DRAIN						},	
	{	"자책감"			,eSKILL_EE_SELF_REPROOF						},	
	{	"악화"				,eSKILL_EE_DEGENERATE						},	
	{	"환상의충격"		,eSKILL_EE_PHANTOM_IMPULSE					},	
	{	"가위"				,eSKILL_EE_TERRIBLE_DREAMS					},	
	{	"화병"				,eSKILL_EE_HWABYUNG							},	
	{	"공격명령"			,eSKILL_EE_ATTACK_COMMAND					},	
	{	"음모의그림자"		,eSKILL_EE_PLOT_OF_SHADOW					},	
	{	"죽음의향기"		,eSKILL_EE_SMELL_OF_DEATH					},	

	{	"끌어오기"			,eSKILL_EE_DRAW_BODY						},	
	{	"연쇄물리대미지"	,eSKILL_EE_CHAIN_DAMAGE						},	

	{	"연쇄불대미지"		,eSKILL_EE_CHAIN_FIRE_DAMAGE				},	
	{	"연쇄물대미지"		,eSKILL_EE_CHAIN_WATER_DAMAGE				},	
	{	"연쇄바람대미지"	,eSKILL_EE_CHAIN_WIND_DAMAGE				},	
	{	"연쇄대지대미지"	,eSKILL_EE_CHAIN_EARTH_DAMAGE				},	
	{	"연쇄빛대미지"		,eSKILL_EE_CHAIN_LIGHT_DAMAGE				},	
	{	"연쇄어둠대미지"	,eSKILL_EE_CHAIN_DARK_DAMAGE				},	

	{	"회복률저하"		,eSKILL_EE_DECLINE_RECOVER_HP_EFFICIENT		},
	{	"부활방해"			,eSKILL_EE_INTERRUPTING_REVIVE				},
	{	"블러디크로스"		,eSKILL_EE_BLOODY_CROSS						},
	{	"불공정계약"		,eSKILL_EE_UNFAIR_COVENANT					},
	{	"계약"				,eSKILL_EE_CONTRACT							},

	{	"혼의서약"			,eSKILL_EE_SOUL_OATH						},
	{	"피의맹약"			,eSKILL_EE_BLOOD_COMPACT					},
	{	"계약파기"			,eSKILL_EE_ANNULMENT_CONTRACT				},

	{	"상태이상면역"		,eSKILL_EE_IMMUNE_STRANGE_STAUS				},
	{	"물리대미지면역"	,eSKILL_EE_IMMUNE_PHYSICAL_ATTACK			},
	{	"마법대미지면역"	,eSKILL_EE_IMMUNE_MAGIC_ATTACK				},

	{	"최종물리공격력증가",eSKILL_EE_INCREASE_FINAL_PHYSICAL_ATTACK_POWER},

	{	"마법회피실드"		,eSKILL_EE_DODGE_MAGIC_DAMAGE	},
	{	"비행"				,eSKILL_EE_FLY},
	{	"몬스터리젠"		,eSKILL_EE_REGEN_MONSTER},
	{	"몬스터폭탄"		,eSKILL_EE_MONSTER_BOMB},
	{	"삼키기"			,eSKILL_EE_SWALLOW},
	{	"당기기"			,eSKILL_EE_INHALE_ENEMY},
	{	"삼킨시체수감소"	,eSKILL_EE_DECREASE_SWALLOW_CORPES},
	{	"컨버젼_아이스"		,eSKILL_EE_CONVERSION_ICE},
	{	"포그폼"			,eSKILL_EE_FOG_FORM},
	{	"전의상실"			,eSKILL_EE_NO_MORE_FIGHT},
	{	"좀비폭탄"			,eSKILL_EE_ZOMBIE_BOMB},
	{	"마법아이템드랍확률증가"	,eSKILL_EE_INCREASE_MAGIC_ITEM_DROP_CHANCE},
	{	"골드드랍확률증가"	,eSKILL_EE_INCREASE_GOLD_DROP_CHANCE},
	{	"필드불저항페널티감소",eSKILL_EE_CORRECT_FIELD_FIRE_PENELTY		},
	{	"필드물저항페널티감소",eSKILL_EE_CORRECT_FIELD_WATER_PENELTY	},
	{	"필드바람저항페널티감소",eSKILL_EE_CORRECT_FIELD_WIND_PENELTY		},
	{	"필드대지저항페널티감소",eSKILL_EE_CORRECT_FIELD_EARTH_PENELTY	},
	{	"필드빛저항페널티감소",eSKILL_EE_CORRECT_FIELD_LIGHT_PENELTY	},
	{	"필드어둠저항페널티감소",eSKILL_EE_CORRECT_FIELD_DARK_PENELTY		},
	{	"장비착용레벨보정"	,eSKILL_EE_CORRECT_ITEM_EQUIP_LEVEL	},
	{	"무기해제"			,eSKILL_EE_INTERRUPTING_WEAPON},		// 무기해제	
	{	"최종공격력감소퍼센티지"	,eSKILL_EE_DECREASE_FINAL_ATTACK_POWER_PERCENTAGE	},	//	최종공격력감소퍼센티지
	{	"발동형리액션사용금지"			,eSKILL_EE_NOT_USE_ACTIVITY_REACTION_SKILL},	// 발동형리액션사용금지
	{	"교감"							,eSKILL_EE_CONSENSUS},				// 교감
	{	"모든능력치증가"				,eSKILL_EE_INCREASE_ALL_STATUS	},	// 모든능력치증가.
	{	"광란"							,eSKILL_EE_MADNESS				}, // 광란
	{	"치명타대미지증가퍼센티지"	,eSKILL_EE_INCLINE_CRITICAL_DAMAGE_PERCENTAGE},	// 치명타대미지증가퍼센티지.
	{	"CP흡수"					,eSKILL_EE_DRAIN_CP},					//  CP 흡수.
	{	"소울컨버전"					,eSKILL_EE_SOUL_CONVERSION},					//  소울컨버전.
	{	"소울번"					,eSKILL_EE_SOUL_BURN},					//  소울번.
	{	"타오르는영혼"		,eSKILL_EE_RAGE_SOUL			},	// 타오르는영혼
	{	"언어구속"		,eSKILL_EE_BINDING_WORDS			},	// 언어구속
	{	"투기"		,eSKILL_EE_FIGHT_ENERGY			},	// 투기..
	{	"돌진"		,eSKILL_EE_RUSH			},	// 돌진
	{	"투지"		,eSKILL_EE_FIGHTING_SPIRIT			},	// 투지 상태
	{	"필요상태무시"		,eSKILL_EE_IGNORE_NEED_STATE			},	// 필요상태무시
	{	"블럭킹무시확률증가"		,eSKILL_EE_IGNORE_BLOCKING			},	// 블럭킹무시확률증가.
	{	"미사일대미지리턴"		,eSKILL_EE_RETURN_MISSILE_DAMAGE			},	// 미사일대미지리턴
	{	"강타확률증가"		,eSKILL_EE_INCREASE_HARD_BLOW			},	// 강타확률증가
	{	"점프"				,eSKILL_EE_JUMP			},	// 점프
	{	"분신공격"				,eSKILL_EE_ATTACK_DOPPELGANGER			},	// 분신공격
	{	"비트추가"				,eSKILL_EE_ADD_BIT			},	
	{	"비트제거"				,eSKILL_EE_REMOVE_BIT			},	
	{	"출혈"					,eSKILL_EE_BLEEDING			},	// 출혈
	{	"감전"					,eSKILL_EE_ELECTRIC_SHOCK			},	
	{	"즉시힐효과"			,eSKILL_EE_INSTANCE_HEAL_EFFECT			},	
	{	"언데드화"				,eSKILL_EE_MAKE_TYPE_UNDEAD				},	
	{	"헌혈"				,eSKILL_EE_BLOOD_DONATION				},
	{	""					,dCODE_END									}
};

//
//	스킬 부가효과 요소
enum
{
eSKILL_EE_ACTIVE_CAHCNE				,	//	발동확률
eSKILL_EE_ACTIVE_CAHCNE_PER_LEVEL	,	//	레벨당 발동확률
eSKILL_EE_ACTIVE_CAHCNE_INVERSE_PROPOTION_BY_REMAIN_HP,
eSKILL_EE_ACTIVE_CAHCNE_INVERSE_PROPOTION_BY_REMAIN_HP_X_MONSTER_GRADE_1,
eSKILL_EE_LIMIT_ACTIVE_CHANCE		,
eSKILL_EE_UPKEEP_TIME				,	//	지속시간
eSKILL_EE_UPKEEP_TIME_PER_LEVEL		,	//	레벨당 지속시간
eSKILL_EE_UPKEEP_TIME_BY_FRAME		,	//	레벨당 지속시간
eSKILL_EE_VALUE1					,
eSKILL_EE_VALUE2					,
eSKILL_EE_VALUE3					,
eSKILL_EE_VALUE4					,
eSKILL_EE_VALUE5					,
eSKILL_EE_VALUE6					,
eSKILL_EE_VALUE7					,
eSKILL_EE_VALUE8					,
eSKILL_EE_VALUE9					,
eSKILL_EE_VALUE10					,
eSKILL_EE_RANGE						,
eSKILL_EE_RANGE_PER_LEVEL			,
eSKILL_EE_ADAPTIVE_TARGET			,
eSKILL_EE_ADAPTIVE_TARGET_TYPE		,
eSKILL_EE_STOP						,
};

//
//	스킬 부가효과
CDecoderKeyword	l_aSkillExtraEffectElement[]	=
{
	{	"발동확률"			,eSKILL_EE_ACTIVE_CAHCNE			},
	{	"발동확률per레벨"	,eSKILL_EE_ACTIVE_CAHCNE_PER_LEVEL	},
	{	"발동확률남은체력에반비례"	,eSKILL_EE_ACTIVE_CAHCNE_INVERSE_PROPOTION_BY_REMAIN_HP	},
	{	"발동확률남은체력x등급반비례1"	,eSKILL_EE_ACTIVE_CAHCNE_INVERSE_PROPOTION_BY_REMAIN_HP_X_MONSTER_GRADE_1	},
	{	"발동확률한계"		,eSKILL_EE_LIMIT_ACTIVE_CHANCE		},
	{	"지속시간"			,eSKILL_EE_UPKEEP_TIME				},
	{	"지속시간per레벨"	,eSKILL_EE_UPKEEP_TIME_PER_LEVEL	},
	{	"지속시간Frame"		,eSKILL_EE_UPKEEP_TIME_BY_FRAME		},
	{	"수치1"				,eSKILL_EE_VALUE1					},
	{	"수치2"				,eSKILL_EE_VALUE2					},
	{	"수치3"				,eSKILL_EE_VALUE3					},
	{	"수치4"				,eSKILL_EE_VALUE4					},
	{	"수치5"				,eSKILL_EE_VALUE5					},
	{	"수치6"				,eSKILL_EE_VALUE6					},
	{	"수치7"				,eSKILL_EE_VALUE7					},
	{	"수치8"				,eSKILL_EE_VALUE8					},
	{	"수치9"				,eSKILL_EE_VALUE9					},
	{	"수치10"			,eSKILL_EE_VALUE10					},
	{	"거리"				,eSKILL_EE_RANGE					},
	{	"거리per레벨"		,eSKILL_EE_RANGE_PER_LEVEL			},
	{	"적용대상"			,eSKILL_EE_ADAPTIVE_TARGET			},
	{	"적용대상타입"		,eSKILL_EE_ADAPTIVE_TARGET_TYPE		},
	{	"정지"				,eSKILL_EE_STOP						},

	{	""					,dCODE_END							}
};	//	CDecoderKeyword	l_aSkillExtraEffect[]

//
//	스킬 부가효과
CDecoderKeyword	l_aHitEffectOuptuPart[]	=
{
	{	"타격영역"			,eHEOP_HIT_ZONE						},	//	혼란
	{	"발"				,eHEOP_FOOT							},	//	혼란
	{	"어깨"				,eHEOP_SHOULDER						},	//	어깨
	{	"머리위"			,eHEOP_ON_THE_HEAD					},	//	어깨
	{	""					,dCODE_END							}
};	//	CDecoderKeyword	l_aSkillExtraEffect[]

CDecoderKeyword	l_aOutputEffect[]	=
{
	{	"불꽃"		,	dPUT_SOFT_DODGE			},	//	불효과
	{	"중독"		,	dPUT_DIFFERENT			},	//	중독
	{	"그을림"	,	dPUT_DODGE_BURN			},	//	불에 그을린 효과
	{	"연기"		,	dPUT_SOFT				},	//	연기..
	{	"빛마법"	,	dPUT_DODGE				},	//	빛계열 마법
	{	"라이트"	,	dPUT_LIGHT				},	//	다중 투영효과..
	{	"라이트EX"	,	dPUT_EXTREAM_LIGHT		},	//	무쟈게 밝게..
	{	"보통"		,	dPUT_NORMAL				},	//	걍.. 찍는다.
	{	"반투명"	,	dPUT_HALF_BLENDING		},	//	반투명
	{	""				,dCODE_END				}
};	//	CDecoderKeyword	l_aSkillExtraEffect[]

#endif

BOOL
CSkillManager::initForDev()
{
#ifdef	_IS_DEV_CLIENT
	if	(!g_bIsDevPC)
		return	FALSE;

	CDecoder		reader;

	if	(!reader.Upload("dev data/scripter/fighterData.txt",NULL))
		return ERRMSG("error!!","can not find 'fighterData.txt' file");

	g_fighterAttackInfo.m_iCount	=	0;

	while(1)
	{
		int	iLevel1			=	reader.getNumber();
		int	iLevel2			=	reader.getNumber();
		int	iMinAttackPower	=	reader.getNumber();
		int	iMaxAttackPower	=	reader.getNumber();
		int	iAttackSpeed	=	reader.getNumber();
		int	iAttackRange	=	reader.getNumber();

		if (iLevel1	==	-1)
			break;

		cFighterAttackInfoE	*lpInfo	=	&g_fighterAttackInfo.m_aData[g_fighterAttackInfo.m_iCount];

		lpInfo->m_wLevel1			=	iLevel1			;
		lpInfo->m_wLevel2			=	iLevel2			;
		lpInfo->m_wMinAttackPower	=	iMinAttackPower	;
		lpInfo->m_wMaxAttackPower	=	iMaxAttackPower	;
		lpInfo->m_wAttackSpeed		=	iAttackSpeed	;
		lpInfo->m_wAttackRange		=	iAttackRange	;

		g_fighterAttackInfo.m_iCount++;
	}

	cFILE	file;

	file.Open("data/fighterData.dat","wb");
	file.Write(&g_fighterAttackInfo,sizeof(g_fighterAttackInfo));
	file.Close();
#endif

	return	TRUE;
}

BOOL
CSkillManager::initFighterData()
{
	cFILE	file;

	if	(!file.Open("data/fighterData.dat","rb"))
		return ERRMSG("error!!","can not find 'data/fighterData.dat' file");

	file.Read(&g_fighterAttackInfo,sizeof(g_fighterAttackInfo));
	file.Close();

	return	TRUE;
}

//
//	기술을 초기화 한다.
BOOL
CSkillManager::init(char *fn,BOOL _bIsWantMakeSkill2)
{
	if (_bIsWantMakeSkill2	&&	g_bIsDevPC	==	FALSE)
		return	TRUE;

	m_bIsCorrectSkillForAnotherSkillBalance	=	_bIsWantMakeSkill2;

	char	strDataFN[50]	=	"data/skill.dat";
	
#ifndef	_FOR_KOREA
	strcpy(strDataFN,"data/skill2.dat");
#endif	

	if (_bIsWantMakeSkill2)
		strcpy(strDataFN,"data/skill2.dat");

#ifdef	_IS_DEV_CLIENT
	SetCurrentDirectory("dev data/scripter"); 

	if (!decodeScripter(fn) || !decodeScripterForRequireSkill(fn))
	{
#ifdef	_FOR_KOREA
		if (g_bIsDevPC)
			cMSG::Put("얼래!!!","디버그 모드인데 스크립트 데이터가 없잖유!!!!");
#endif	

		SetLogFolder();

		return	loadData(strDataFN);
	}

	SetLogFolder();
	saveData(strDataFN);

	return	TRUE;

#else
	// 10.02.25 기획자 클라이언트에서 createskilldata.dat 파일이 있을시 스킬데이터 생성
#ifdef	_DEBUG
	SetCurrentDirectory("dev data"); 
	cFILE file;
	
	if(file.Open("createskilldata.nfo","rb"))
	{
		SetLogFolder();
		SetCurrentDirectory("dev data/scripter"); 
		if (!decodeScripter(fn) || !decodeScripterForRequireSkill(fn))
		{
#ifdef	_FOR_KOREA
			if (g_bIsDevPC)
				cMSG::Put("얼래!!!","디버그 모드인데 스크립트 데이터가 없잖유!!!!");
#endif	
			
			SetLogFolder();
			return	loadData(strDataFN);
		}
		
		file.Close();
	}
	else
	{
		SetLogFolder();
		return	loadData(strDataFN);	
	}
	
	SetLogFolder();
	saveData(strDataFN);
	
	return	TRUE;
#else
	return	loadData(strDataFN);


#endif

#endif
}
//
//	스크립트 화일을 읽어서 기술들을 초기화 한다.
BOOL
CSkillManager::decodeScripterForRequireSkill(char *fn)
{
#ifdef	_IS_DEV_CLIENT

	CDecoder		reader;
	DWORD			dwCode		=	0;
	CSkill			*lpSkill;
	char			*str;
	int				iRequireSkillCount,iValue;

	if (!reader.Upload(fn,l_aSkillKeyword))
		return FALSE;

	while(dwCode	!=	dCODE_END)
	{
		char *keyword	=	reader.getWord(TRUE);
		dwCode			=	reader.analCode(keyword);

		switch(dwCode)
		{
			case	dCODE_CLOSE		:
			case	dCODE_END		:
			case	dCODE_FINISH	:
			case	dCODE_OPEN		:
				continue;
		}

		if (STRICMP("include",keyword)==0)
		{
			str	=	reader.getStream();

			decodeScripterForRequireSkill(str);
			continue;
		}

		if (STRICMP("correctSkill",keyword)==0)
		{
			if (m_bIsCorrectSkillForAnotherSkillBalance	==	FALSE)
				break;

			continue;
		}

		if (STRICMP("직업제한",keyword)==0)
		{	
			str					=	reader.getStream();
			l_iEnableJob		=	CDecoder::AnalCode(l_aJobNameKeyword,str);

			if	(l_iEnableJob	==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","화일 '%s'에 직업제한의 직업 '%s'를 정의 할 수 없습니다.'",fn,str);

			continue;
		}
		if (STRICMP("스킬",keyword)==0)
		{
			iRequireSkillCount	=	0;

			str		=	reader.getStream();

			if	(STRICMP("흑진주 품기",str)==0)
				str	=	str;
			
			iValue	=	getIndex(str,l_iEnableJob);
			lpSkill	=	&g_aSkill[iValue];

			reader.findOpenCode();
			continue;
		}
		if (STRICMP("필요스킬",keyword)==0)
		{
			str		=	reader.getStream();
			iValue	=	CDecoder::AnalCode(l_aJobNameKeyword,str);

			if (iValue==dCODE_UNDEFINE)	
				iValue	=	l_iEnableJob;
			else						
				str		=	reader.getStream();

			if	(lpSkill->m_wSerial	==	1040)
				lpSkill->m_wSerial	=	1040;

			int	iLevel	=	reader.getNumber();
			int	iSkill	=	getIndex(str,iValue);

			if (iSkill	==	0xffff)	
				EXIT("error in CSkillManager::decodeScripterForRequireSkill(char *fn)","필요스킬이 이상합니다.\n\n%s-%s[%s]",fn,lpSkill->m_strName,str);

			lpSkill->m_aRequireSkill[iRequireSkillCount].m_wSkill	=	iSkill;
			lpSkill->m_aRequireSkill[iRequireSkillCount].m_wLevel	=	iLevel;
			iRequireSkillCount++;
			continue;
		}
		if (STRICMP("명령스킬",keyword)==0)
		{
			str			=	reader.getStream();
			int	iSkill	=	getIndex(str,dJOB_CODE_MONSTER,TRUE);

			if (iSkill	==	0xffff)	
				EXIT("error in CSkillManager::decodeScripterForRequireSkill(char *fn)","명령스킬이 이상합니다.\n\n%s-%s",fn,lpSkill->m_strName);

			lpSkill->m_wCommandSkill			=	iSkill;

			continue;
		}		
	}

	return	TRUE;
#endif

	return	TRUE;
}

//
//	스크립트 화일을 읽어서 기술들을 초기화 한다.
BOOL
CSkillManager::decodeScripter(char *fn)
{
#ifdef	_IS_DEV_CLIENT
	CDecoder		reader;

	DWORD			dwCode		=	0;
	BOOL			isGroupCode	=	FALSE,bIsSkillCopy	=	FALSE;
	CSkill			skill;
	char			*str;
	cSKILL_GROUP	*lpSkillGroup;
	int				iRequireSkillCount,iValue,iCh;
	WORD			wExtraEffectAdaptiveTarget=0,wExtraEffectCount=0;

#ifndef	_FOR_KOREA
	return	FALSE;
#endif
	
	if (!g_bIsDevPC)
		return	FALSE;

	if (!reader.Upload(fn,l_aSkillKeyword))
		return FALSE;

	while(dwCode	!=	dCODE_END)
	{
		char *keyword	=	reader.getWord();
		dwCode			=	reader.analCode(keyword);

		switch(dwCode)
		{
			case	dCODE_CLOSE		:
			{
				if (isGroupCode)
				{	
					m_iSkillGroupCount++;
					lpSkillGroup->m_wJob=	l_iEnableJob;
				}
				else
				{
					if	(l_iEnableJob	==	dJOB_CODE_ALL)
						skill.m_wEnableJob	=	0xffff;
					else
						skill.m_wEnableJob	=	l_iEnableJob;

					if	(bIsSkillCopy	==	FALSE)
					{
						if	(skill.m_aExtraEffect[0].m_wEffect	!=	0xffff)
						{
							if	(wExtraEffectAdaptiveTarget	==	0)
								EXIT("error in CSkillManager::decodeScripter(char *fn)","부가효과 적용 대상이 없습니다.\n\n%s-%s",fn,skill.m_strName);

							for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
							{
								if (skill.m_aExtraEffect[i].m_bf4TargetMethod	==	0)
									skill.m_aExtraEffect[i].m_bf4TargetMethod	=	wExtraEffectAdaptiveTarget;

								if (skill.m_wTargetTypeLimit == 0xffff)
									continue;

								skill.m_aExtraEffect[i].m_bf1TargetUndead	=	FALSE;
								skill.m_aExtraEffect[i].m_bf1TargetHuman	=	FALSE;
								skill.m_aExtraEffect[i].m_bf1TargetDemon	=	FALSE;
								skill.m_aExtraEffect[i].m_bf1TargetAnimal	=	FALSE;
								skill.m_aExtraEffect[i].m_bf1TargetHolyBeast=	FALSE;

								for (int iMask=0;iMask<5;iMask++)
								{
									int	iValue	=	1<<iMask;

									if (iValue & skill.m_wTargetTypeLimit)
									{
										switch(iMask)
										{
											case	eJT_UNDEAD	:
												skill.m_aExtraEffect[i].m_bf1TargetUndead		=	TRUE;
												break;
											case	eJT_HUMAN	:
												skill.m_aExtraEffect[i].m_bf1TargetHuman		=	TRUE;
												break;
											case	eJT_DEVIL	:
												skill.m_aExtraEffect[i].m_bf1TargetDemon		=	TRUE;
												break;
											case	eJT_ANIMAL	:
												skill.m_aExtraEffect[i].m_bf1TargetAnimal		=	TRUE;
												break;
											case	eJT_HOLY_BEAST	:
												skill.m_aExtraEffect[i].m_bf1TargetHolyBeast	=	TRUE;
												break;
										}
									}
								}
							}
						}
					}

					addSkill(&skill);
					skill.reset();
					iRequireSkillCount			=	0;
					wExtraEffectAdaptiveTarget	=	0;
					wExtraEffectCount			=	0;
					bIsSkillCopy				=	FALSE;
				}

				isGroupCode			=	FALSE;
				continue;
			}

			case	dCODE_END		:
			case	dCODE_FINISH	:
			case	dCODE_OPEN		:
				continue;
		}

		if (STRICMP("스킬AI",keyword)==0)
		{
			int		iCount	=	0;

			cSkillAiPatternInfo	*lpInfo	=	&skill.m_aAi[0];

			while(1)
			{
				if	(lpInfo->m_wTrigger	!=	0xffff)
				{
					iCount++;
					lpInfo	=	&skill.m_aAi[iCount];
				}

				str			=	reader.getWord();

				if (STRICMP(str,dDC_WORD_OPEN)==0 || STRICMP(str,dDC_WORD_FINISH)==0 )
					continue;
				if (STRICMP(str,dDC_WORD_CLOSE)==0)
					break;

			
				if	(STRICMP(str,"반격스킬")==0)
				{
					skill.m_bf1IsMonsterCounter		=	TRUE;
					continue;
				}
				if (STRICMP(str,"에니메이트파트너횟수체크")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_CHECK_ANIMATE_PARTNER_COUNT;
					continue;
				}

				if (STRICMP(str,"AI대상")==0)
				{
					str			=	reader.getWord();

					if (STRICMP(str,"적")==0)
						skill.m_wAI_Target	=	eSAT_ENEMY;
					else
					if (STRICMP(str,"아군")==0)
						skill.m_wAI_Target	=	eSAT_OWN_FORCE;
					else
					if (STRICMP(str,"자신")==0)
						skill.m_wAI_Target	=	eSAT_SELF;
					else
					if (STRICMP(str,"아군시체")==0)
						skill.m_wAI_Target	=	eSAT_OWN_FORCE_CORPSE;
					else
					if (STRICMP(str,"같은계열직업아군시체")==0)
						skill.m_wAI_Target	=	eSAT_OWN_FORCE_SAME_ORDER_JOB_CORPSE;
					else
						EXIT("error in CSkillManager::decodeScripter(char *fn)","스킬AI에 AI대상이 이상합니다.\n\n%s-%s",fn,skill.m_strName);
					continue;
				}

				if (STRICMP(str,"우선순위")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_ORDERING;
					lpInfo->m_asValue[0]=	reader.getNumber();
					continue;
				}
				if (STRICMP(str,"시야내아군의HP")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_IN_SIGHT_OWNER_FORCE_HP;
					lpInfo->m_asValue[0]=	reader.getNumber();
					continue;
				}
				if (STRICMP(str,"타겟과의거리")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_RANGE_TO_TARGET;
					lpInfo->m_asValue[0]=	reader.getNumber();
					continue;
				}
				if (STRICMP(str,"삼킨시체수체크")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_CHECK_SWALLOW_CORPES_COUNT;
					lpInfo->m_asValue[0]=	reader.getNumber();
					continue;
				}
				
				if (STRICMP(str,"버프체크_본인")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_BUFF_ON_OWNER;

					str			=	reader.getWord();

					if (STRICMP(str,"TRUE")==0)
						lpInfo->m_asValue[0]=	1;
					else
					if (STRICMP(str,"FALSE")==0)
						lpInfo->m_asValue[0]=	0;
					else
						EXIT("error in CSkillManager::decodeScripter(char *fn)","스킬AI에 버프체크_본인이 이상합니다.\n\n%s-%s",fn,skill.m_strName);
					continue;
				}
				if (STRICMP(str,"버프체크_타겟")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_BUFF_ON_TARGET;

					str			=	reader.getWord();

					if (STRICMP(str,"TRUE")==0)
						lpInfo->m_asValue[0]=	1;
					else
					if (STRICMP(str,"FALSE")==0)
						lpInfo->m_asValue[0]=	0;
					else
						EXIT("error in CSkillManager::decodeScripter(char *fn)","스킬AI에 버프체크_타겟이 이상합니다.\n\n%s-%s",fn,skill.m_strName);
					continue;
				}
				if (STRICMP(str,"동료의수")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_OWNER_FORCE_COUNT_IN_SIGHT;
					lpInfo->m_asValue[0]=	reader.getNumber();
					lpInfo->m_asValue[1]=	reader.getNumber();
					continue;
				}
				if (STRICMP(str,"적의수")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_ENEMY_COUNT_IN_SIGHT;
					lpInfo->m_asValue[0]=	reader.getNumber();
					lpInfo->m_asValue[1]=	reader.getNumber();	//	0 이면 기본 시야
					continue;
				}
				if (STRICMP(str,"타겟주위에적의수")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_ENEMY_COUNT_IN_TARGET_AROUND;
					lpInfo->m_asValue[0]=	reader.getNumber();
					lpInfo->m_asValue[1]=	reader.getNumber();
					continue;
				}
				if (STRICMP(str,"시야내아군의죽음")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_DEATH_OWNER_FORCE_IN_SIGHT;
					str			=	reader.getWord();

					if (STRICMP(str,"TRUE")==0)
						lpInfo->m_asValue[0]=	1;
					else
					if (STRICMP(str,"FALSE")==0)
						lpInfo->m_asValue[0]=	0;
					else
						EXIT("error in CSkillManager::decodeScripter(char *fn)","스킬AI에 시야내아군의죽음이 이상합니다.\n\n%s-%s",fn,skill.m_strName);
					continue;
				}
				if (STRICMP(str,"전투상황")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_ON_BATTLE;
					str			=	reader.getWord();

					if (STRICMP(str,"시작")==0)
						lpInfo->m_asValue[0]=	1;
					else
					if (STRICMP(str,"진행중")==0)
						lpInfo->m_asValue[0]=	0;
					else
						EXIT("error in CSkillManager::decodeScripter(char *fn)","스킬AI에 전투상황이 이상합니다.\n\n%s-%s",fn,skill.m_strName);
					continue;
				}
				if (STRICMP(str,"저주로증폭가능한상태")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_ENABLE_CURSE_AMPLIFY;
					str			=	reader.getWord();

					if (STRICMP(str,"TRUE")==0)
						lpInfo->m_asValue[0]=	1;
					else
					if (STRICMP(str,"FALSE")==0)
						lpInfo->m_asValue[0]=	0;
					else
						EXIT("error in CSkillManager::decodeScripter(char *fn)","스킬AI에 저주로증폭가능이 이상합니다.\n\n%s-%s",fn,skill.m_strName);
					continue;
				}
				if (STRICMP(str,"타겟이펫이다")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_TARGET_IS_PET;
					str			=	reader.getWord();

					if (STRICMP(str,"TRUE")==0)
						lpInfo->m_asValue[0]=	1;
					else
					if (STRICMP(str,"FALSE")==0)
						lpInfo->m_asValue[0]=	0;
					else
						EXIT("error in CSkillManager::decodeScripter(char *fn)","스킬AI에 타겟이펫이다가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
					continue;
				}
				if (STRICMP(str,"발동확률")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_ACTIVE_CHANCE;
					lpInfo->m_asValue[0]=	reader.getNumber();
					continue;
				}
				if (STRICMP(str,"현재HP")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_CURRENT_HP;
					lpInfo->m_asValue[0]=	reader.getNumber();
					continue;
				}
				if	(STRICMP(str,"상태체크")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_CHECK_STATUS;
					str			=	reader.getWord();

					if (STRICMP(str,"인비저블")==0)
						lpInfo->m_asValue[0]=	eSTATUS_INVISIBLE;
					else
						EXIT("error in CSkillManager::decodeScripter(char *fn)","스킬AI에 상태체크가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
					continue;
				}
				if	(STRICMP(str,"함정개수체크")==0)
				{
					lpInfo->m_wTrigger	=	eSACS_CHECK_TRAP_COUNT;
					lpInfo->m_asValue[0]=	reader.getNumber();	//	개수
					lpInfo->m_asValue[1]=	reader.getNumber();	//	함정 스킬

					continue;
				}
				EXIT("error in CSkillManager::decodeScripter(char *fn)","스킬AI에 판단 할 수 없는 단어가 있습니다. [%s]\n\n%s-%s",str,fn,skill.m_strName);
			}
			continue;
		}
		if (STRICMP("공격속도옵션무시",keyword)==0)
		{
			skill.m_bf1IsIgnoreOptionAttackSpeed	=	TRUE;
			continue;
		}	

		if (STRICMP("기가라이트닝_함정설치",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","기가라이트닝_함정설치 스킬 번호가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[0]	=	iValue;
			continue;
		}

		if	(STRICMP("기가라이트닝_시전시간",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","기가라이트닝_서치시간이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[1]	=	iValue;
			continue;
		}

		if	(STRICMP("기가라이트닝_서치시간",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","기가라이트닝_서치시간이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[2]	=	iValue;
			continue;
		}

		if	(STRICMP("기가라이트닝_경고시간",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","기가라이트닝_경고시간이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[3]	=	iValue;
			continue;
		}

		if	(STRICMP("기가라이트닝_액션",keyword)==0)
		{
			str			=	reader.getStream();

			if	(strcmp(str,"기본 공격")==0)
				skill.m_aValue[4]	=	dACT_DEFAULT_ATTACK;
			else
			if	(strcmp(str,"기본 마법")==0)
				skill.m_aValue[4]	=	dACT_DEFAULT_MAGIC;
			else
			{
				int	iAction	=	CDecoder::AnalCode(l_aSkillAction[l_iEnableJob],str);

				if	(iAction >= 0	&&	iAction	!=	dCODE_UNDEFINE)
					skill.m_aValue[4]	=	iAction;
				else
					EXIT("error in CSkillManager::decodeScripter(char *fn)","액션이 이상합니다.\n\n%s-%s",skill.m_strName,str);
			}
			continue;
		}

		if	(STRICMP("기가라이트닝_책색깔",keyword)==0)
		{
			char	*lpstrColor	=	reader.getWord();
			int		iColor		=	CDecoder::AnalCode(l_aBookColor,lpstrColor);

			if	(iColor	==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","파이어와인더_책색깔이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[5]			=	iColor;
			continue;
		}

		if	(STRICMP("기가라이트닝_범위",keyword)==0)
		{
			int		iTime		=	reader.getNumber();

			skill.m_aValue[6]	=	iTime;
			continue;
		}

		if	(STRICMP("파이어월_작은불꽃_지속시간",keyword)==0)
		{
			int		iTime		=	reader.getNumber();

			skill.m_aValue[0]	=	iTime;
			continue;
		}

		if	(STRICMP("미러이미지_인챈트스킬",keyword)==0)
		{
			int		iSkill		=	reader.getNumber();

			skill.m_aValue[3]	=	iSkill;
			continue;
		}
		
		if	(STRICMP("미러이미지_딜레이시간",keyword)==0)
		{
			int		iPeriod		=	reader.getNumber();

			skill.m_aValue[2]	=	iPeriod;
			continue;
		}

		if	(STRICMP("미러이미지_생성주기",keyword)==0)
		{
			int		iPeriod		=	reader.getNumber();

			skill.m_aValue[1]	=	iPeriod;
			continue;
		}

		if	(STRICMP("미러이미지_생성직업",keyword)==0)
		{
			char		*lpstrItemName	=	reader.getStream();
			CJobInfo	*lpJob			=	g_jm.getJob(lpstrItemName);

			if	(!lpJob)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","''%s'라는 직업을 찾을 수 없습니다.\n\n%s-%s",lpstrItemName,fn,skill.m_strName);
			skill.m_aValue[0]			=	lpJob->m_wSerial-dJOB_BORDER;
			continue;
		}

		if	(STRICMP("체인라이트닝_중계직업",keyword)==0)
		{
			char		*lpstrItemName	=	reader.getStream();
			CJobInfo	*lpJob			=	g_jm.getJob(lpstrItemName);

			if	(!lpJob)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","''%s'라는 직업을 찾을 수 없습니다.\n\n%s-%s",lpstrItemName,fn,skill.m_strName);
			skill.m_aValue[0]			=	lpJob->m_wSerial-dJOB_BORDER;
			continue;
		}

		if	(STRICMP("체인라이트닝_책색깔",keyword)==0)
		{
			char	*lpstrColor	=	reader.getWord();
			int		iColor		=	CDecoder::AnalCode(l_aBookColor,lpstrColor);

			if	(iColor	==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","파이어와인더_책색깔이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[1]			=	iColor;
			continue;
		}

		if	(STRICMP(keyword,"펫스킬대미지수식")==0)
		{
			int	iType	=	reader.getNumber();
			skill.m_bf4PetSkillDamageExpressionType	=	iType;
			continue;
		}

		if	(STRICMP(keyword,"펫스킬대미지수식For씨앗")==0)
		{
			int	iType	=	reader.getNumber();
			skill.m_bf4PetSkillDamageExpressionTypeForAwaken	=	iType;
			continue;
		}

		if	(STRICMP(keyword,"차지카운트")==0)
		{
			int	iCount	=	reader.getNumber();
			skill.m_aValue[0]	=	iCount;
			continue;
		}

		if	(STRICMP(keyword,"미니펫_조합배열")==0)
		{
			for	(int i=0;i<16;i++)
			{
				str	=	reader.getStream();

				if	(stricmp(str,"fire")	==	0)
					skill.m_aValue[i]	=	100;
				else
				if	(stricmp(str,"water")	==	0)
					skill.m_aValue[i]	=	101;
				else
				if	(stricmp(str,"wind")	==	0)
					skill.m_aValue[i]	=	102;
				else
				if	(stricmp(str,"earth")	==	0)
					skill.m_aValue[i]	=	103;
				else
				if	(stricmp(str,"light")	==	0)
					skill.m_aValue[i]	=	104;
				else
				if	(stricmp(str,"dark")	==	0)
					skill.m_aValue[i]	=	105;
				else
				if	(stricmp(str,"any")		==	0)
					skill.m_aValue[i]	=	-1;
				else
				{
					int	iType	=	GetMiniPetTypeByName(str);

					if	(iType	==	-1)
						EXIT("error in CSkillManager::decodeScripter(char *fn)","미니펫_조합배열 항목이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

					skill.m_aValue[i]	=	iType;
				}
			}

			continue;
		}

		if	(STRICMP(keyword,"미니펫_조합효과")==0)
		{
			char	*lpstrKeyWord[]	=	
			{
				"eIncrease_MiniPet_FireDamage",
				"eIncrease_MiniPet_WaterDamage",
				"eIncrease_MiniPet_WindDamage",
				"eIncrease_MiniPet_EarthDamage",
				"eIncrease_MiniPet_LightDamage",
				"eIncrease_MiniPet_DarkDamage",
				"eIncrease_MiniPet_MagicDamage",
				"eCorrect_MiniPet_SkillActiveChance",
				"eIncrease_MiniPet_BasicSkillLevel",
				"eIncrease_MiniPet_SkillLevel",
				"eIncrease_MiniPet_MagicDamage_vsBoss",
				"end"
			};

			str	=	reader.getStream();

			for	(int i=0;;i++)
			{
				if	(stricmp(lpstrKeyWord[i],"end")	==	0)
					EXIT("error in CSkillManager::decodeScripter(char *fn)","미니펫_조합효과 항목이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

				if	(stricmp(lpstrKeyWord[i],str)	==	0)
				{
					skill.m_aValue[16]	=	i;
					break;
				}
			}

			int	iValue	=	reader.getNumber();
			skill.m_aValue[17]	=	iValue;
			continue;
		}
		

		if	(STRICMP(keyword,"파워업대상미니펫스킬")==0)
		{
			str			=	reader.getStream();
			int	iSkill	=	getIndex(str,0xffff);

			if	(iSkill	==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","파워업대상미니펫스킬이 이상합니다.%s\n\n%s-%s",str,fn,skill.m_strName);

			skill.m_aValue[0]	=	iSkill;
			continue;

		}

		if	(STRICMP(keyword,"미니펫타겟_보스타입")==0)
		{
			str			=	reader.getWord();

			iValue		=	CDecoder::AnalCode(l_aMonsterType,str);

			if	(iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","미니펫타겟_보스타입이 이상합니다.%s\n\n%s-%s",str,fn,skill.m_strName);

			skill.m_aValue[2]	=	iValue;
			continue;

		}

		if	(STRICMP(keyword,"펫스킬타입")==0)
		{
			char	*str	=	reader.getWord();

			if	(STRICMP(str,"오브젝트파괴")==0)
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::AttackTheObject;
			else
			if	(STRICMP(str,"맞을때발동")==0)
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::ActionWhenDamaged;
			else
			if	(STRICMP(str,"전투대상공격")==0)
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::AttackUnderBattleEnemy;
			else
			if	(STRICMP(str,"주인파워업")	==	0)		//	주인파워업
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::LordPowerUp;
			else
			if	(STRICMP(str,"주인돕기")	==	0)		//	주인돕기
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::AidToLord;
			else
			if	(STRICMP(str,"파워업_보스전")	==	0)	//	파워업_보스전
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::PowerUpUnderBattleToBoss;
			else
			if	(STRICMP(str,"미니펫스킬파워업")	==	0)	//	미니펫버프
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::MiniPetSkillPowerUp;
			else
			if	(STRICMP(str,"전투중주인보조")	==	0)	//	주인보조
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::AidToUnderBattleLord;
			else
			if	(STRICMP(str,"아이템줍기")	==	0)	//	아이템줍기
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::PickItem;
			else
			if	(STRICMP(str,"함정회피")	==	0)	//	함정회피
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::AvoidTrap;
			else
			if	(STRICMP(str,"골드획득")	==	0)	//	골드획득
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::PickGold;
			else
			if	(STRICMP(str,"부활페널티감소")	==	0)	//	부활페널티감소
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::DecreaseResurrectionPenelty;
			else
			if	(STRICMP(str,"부활")	==	0)	//	부활
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::Resurrection;
			else
			if	(STRICMP(str,"잠금해체레벨증가")	==	0)	//	잠금해체레벨증가
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::IncreaeUnlockLevel;
			else
			if	(STRICMP(str,"포션효율증가")	==	0)	//	포션효율증가
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::UseHealPotion;
			else
			if	(STRICMP(str,"CP포션효율증가")	==	0)	//	CP포션효율증가
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::UseChargePotion;
			else
			if	(STRICMP(str,"사용CP회복")	==	0)	//	사용CP회복
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::RecoverUseCP;
			else
			if	(STRICMP(str,"힐스킬회복체력증가")	==	0)	//	힐스킬회복체력증가
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::HealBonusByHealSkill;
			else
			if	(STRICMP(str,"아이템드랍률증가")	==	0)	//	아이템드랍률증가
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::IncreaseDropItemChance;
			else
			if	(STRICMP(str,"골드드랍률증가")	==	0)	//	골드드랍률증가
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::IncreaseDropGoldChance;
			else
			if	(STRICMP(str,"타겟마법저항무시_보스전")	==	0)	//	타겟마법저항무시
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::IgnoreTargetMagicResistanceUnderBossBattle;
			else
			if	(STRICMP(str,"스킬누적공격")	==	0)	//	스킬누적공격
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::ChargeAndFire;
			else
			if	(STRICMP(str,"스킬누적공격_누적회수감소")	==	0)	//	스킬누적공격_누적회수감소
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::DecreaseChargeCount;
			else
			if	(STRICMP(str,"발동확률증가_보스전")	==	0)	//	스킬누적공격_누적회수감소
				skill.m_bf5MiniPetSkillType	=	nsMiniPetST::IncreaseActiveChanceUnderBossBattle;
			else
				EXIT("error in CSkillManager::decodeScripter(char *fn)","펫스킬타입에 [%s]라는건 없습니다.\n\n%s-%s",str,fn,skill.m_strName);

			continue;
		}

		if	(STRICMP(keyword,"발동확률x펫레벨")==0)
		{
			skill.m_bf1IsXMiniPetLevel	=	TRUE;
			
			continue;
		}
		
		if	(STRICMP(keyword,"미니펫기본스킬")==0)
		{
			skill.m_bf1IsDefaultMiniPetSkill	=	TRUE;
			
			continue;
		}
		if	(STRICMP(keyword,"미니펫일반스킬")==0)
		{
			skill.m_bf1IsNormalMiniPetSkill	=	TRUE;
			
			continue;
		}
		
		if	(STRICMP(keyword,"사용미니펫")==0)
		{
			char	*lpstrMiniPet	=	reader.getStream();

			int	iType	=	GetMiniPetTypeByName(lpstrMiniPet);

			if	(iType	==	-1)
			{
				if	(stricmp(lpstrMiniPet,"스페어")	==	0)
					iType	=	c_iMiniPetSpareSkillType;

				if	(iType	==	-1)
					EXIT("error in CSkillManager::decodeScripter(char *fn)","잘못된 사용미니펫 이름 입니다.[%s]\n\n%s-%s",lpstrMiniPet,fn,skill.m_strName);
			}

			skill.m_bf5MiniPetType	=	iType;
			
			continue;
		}

		if	(STRICMP("파이어와인더_책색깔",keyword)==0)
		{
			char	*lpstrColor	=	reader.getWord();
			int		iColor		=	CDecoder::AnalCode(l_aBookColor,lpstrColor);

			if	(iColor	==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","파이어와인더_책색깔이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[0]			=	iColor;
			continue;
		}

		if	(STRICMP("단계별대미지",keyword)==0)
		{
			str		=	reader.getWord();
			if (STRICMP(str	,"시전자")==0)
				skill.m_wWADDamageMethod	=	eWAD_METHOD_CASTER;
			else
			if (STRICMP(str	,"최대폭발범위")==0)
				skill.m_wWADDamageMethod	=	eWAD_MAX_EXPLOSION_RANGE;
			else
			if (STRICMP(str	,"폭발범위")==0)
				skill.m_wWADDamageMethod	=	eWAD_EXPLOSION_RANGE;
			else
			if (STRICMP(str	,"최대사정거리")==0)
				skill.m_wWADDamageMethod	=	eWAD_SHOOT_RANGE;
			else
			if (STRICMP(str	,"타겟비타겟")==0)
				skill.m_wWADDamageMethod	=	eWAD_TARGET_NON_TARGET;
			else
				EXIT("error in CSkillManager::decodeScripter(char *fn)","단계별대미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_wWADDamageGradeCount=	reader.getNumber();
			skill.m_wWADMaxDamage		=	reader.getNumber();
			skill.m_wWADDamageValue		=	reader.getNumber();
			skill.m_wWADMinDamage		=	reader.getNumber();
			continue;
		}

		if (STRICMP("동시시전스킬수제한",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","동시시전스킬수제한이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wLimitActiveCount	=	iValue;
			continue;
		}
		if (STRICMP("분신공격속도",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","분신공격속도가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[0]	=	iValue;
			continue;
		}
		if	(STRICMP("쿨타임",keyword)==0	||	STRICMP("결투쿨타임",keyword)==0)
		{
			if	(STRICMP("결투쿨타임",keyword)==0)
				skill.m_bf1IsDuelCoolTimeSkill	=	TRUE;

			int	iIndex	=	reader.getNumber();

			if	(iIndex	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","쿨타임 인덱스가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			iValue	=	reader.getNumber();

			if	(iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","쿨타임이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wCoolTimeIndex	=	iIndex;
			skill.m_wCoolTime		=	iValue;
			continue;
		}
		if (STRICMP("쿨타임per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","쿨타임per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wCoolTimePerLevel	=	iValue;
			continue;
		}
		if (STRICMP("최소쿨타임",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","최소쿨타임이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wMinimumCoolTime	=	iValue;
			continue;
		}
		if (STRICMP("이동각도",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","이동각도가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wDodgeAngle	=	iValue;
			continue;
		}

		if (STRICMP("타격각도범위",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","타격각도범위가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wHitAngleRange			=	iValue;
			continue;
		}

		if (STRICMP("타격각도범위per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","타격각도범위per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wHitAngleRangePerLevel	=	iValue;
			continue;
		}

		if	(STRICMP("타겟상태체크",keyword)==0	||	STRICMP("시전자상태체크",keyword)==0)
		{
			CSkillDefine::uCheckStatus	*lpStatus;

			if	(STRICMP("타겟상태체크",keyword)==0)
				lpStatus	=	&skill.m_checkTargetStatus;
			if	(STRICMP("시전자상태체크",keyword)==0)
				lpStatus	=	&skill.m_checkCasterStatus;

			while(1)
			{
				str					=	reader.getWord();

				if	(STRICMP(str,"중독")==0)
					lpStatus->m_bf1IsPoison		=	TRUE;
				else
				if	(STRICMP(str,"계약")==0)
					lpStatus->m_bf1IsContract	=	TRUE;
				else
				if	(STRICMP(str,"CP마이너스")==0)
					lpStatus->m_bf1IsMinusCP	=	TRUE;
				else
					EXIT("error in CSkillManager::decodeScripter(char *fn)","타겟상태체크 상태가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

				iCh	=	reader.getNextChar();
				if	(iCh	==	';'	)
					break;
			}
			continue;
		}

		if	(STRICMP("방향바꾸기",keyword)==0)
		{
			str					=	reader.getWord();

			if	(STRICMP(str,"북")==0)
				skill.m_bf4ChangeDirect		=	1;
			else
			if	(STRICMP(str,"북동")==0)
				skill.m_bf4ChangeDirect		=	2;
			else
			if	(STRICMP(str,"동")==0)
				skill.m_bf4ChangeDirect		=	3;
			else
			if	(STRICMP(str,"남동")==0)
				skill.m_bf4ChangeDirect		=	4;
			else
			if	(STRICMP(str,"남")==0)
				skill.m_bf4ChangeDirect		=	5;
			else
			if	(STRICMP(str,"남서")==0)
				skill.m_bf4ChangeDirect		=	6;
			else
			if	(STRICMP(str,"서")==0)
				skill.m_bf4ChangeDirect		=	7;
			else
			if	(STRICMP(str,"북서")==0)
				skill.m_bf4ChangeDirect		=	8;
			else
				EXIT("error in CSkillManager::decodeScripter(char *fn)","'방향바꾸기'가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}

		if (STRICMP("각도",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","각도가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wAngle	=	iValue;
			continue;
		}
		if (STRICMP("회피거리",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","회피거리가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wDodgeDistance	=	iValue;
			continue;
		}

		if (STRICMP("지속시간",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if	(iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","지속시간이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			if	(iValue	<	0)
				iValue	=	iValue;

			skill.m_wUpkeepTime	=	iValue;
			continue;
		}

		if (STRICMP("지속시간per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","지속시간per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wUpkeepTimePerLevel	=	iValue;
			continue;
		}

		if (STRICMP("발동확률",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","발동확률이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sActiveChance	=	iValue;
			continue;
		}
		if (STRICMP("발동확률per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","발동확률per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sActiveChancePerLevel	=	iValue;
			continue;
		}
		if(STRICMP("씨앗보너스발동확률",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","씨앗보너스발동확률.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_wAwakenSkillBonusActiveChance	=	iValue;
			continue;
		}
		if(STRICMP("씨앗보너스발동확률per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","씨앗보너스발동확률per레벨.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_wAwakenSkillBonusActiveChancePerLevel	=	iValue;
			continue;
		}

		if(STRICMP("씨앗보너스데미지",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","씨앗보너스데미지.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_wAwakenSkillBonusDamage	=	iValue;
			continue;
		}
		if(STRICMP("씨앗보너스데미지per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","씨앗보너스데미지per레벨.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_wAwakenSkillBonusDamagePerLevel	=	iValue;
			continue;
		}
		
		if(STRICMP("씨앗보너스데미지For기존공식",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","씨앗보너스데미지For기존공식.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_wAwakenSkillBonusDamageForOrigin			=	iValue;
			continue;
		}
		if(STRICMP("씨앗보너스데미지per레벨For기존공식",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","씨앗보너스데미지per레벨For기존공식.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_wAwakenSkillBonusDamagePerLevelForOrigin	=	iValue;
			continue;
		}
		if(STRICMP("씨앗보너스데미지범위For기존공식",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","씨앗보너스데미지범위For기존공식.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_wAwakenSkillBonusDamageValueRangeForOrigin	=	iValue;
			continue;
		}
		if(STRICMP("씨앗보너스데미지범위per레벨For기존공식",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","씨앗보너스데미지범위per레벨For기존공식.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_wAwakenSkillBonusDamageValueRangePerLevelForOrigin	=	iValue;
			continue;
		}

		if(STRICMP("씨앗보너스보조스킬대미지",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","씨앗보너스보조스킬대미지.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_wAwakenSkillBonusDamageForBoJoSkill	=	iValue;
			continue;
		}

		if(STRICMP("씨앗보너스보조스킬대미지per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","씨앗보너스보조스킬대미지per레벨.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_wAwakenSkillBonusDamageForBoJoSkillPerLevel	=	iValue;
			continue;
		}

		if(STRICMP("씨앗보너스사정거리",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","씨앗보너스사정거리.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_wAwakenSkillBonusRange	=	iValue;
			continue;
		}

		if(STRICMP("씨앗보너스사정거리per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","씨앗보너스사정거리per레벨.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_wAwakenSkillBonusRangePerLevel	=	iValue;
			continue;
		}

		if(STRICMP("씨앗보너스보조스킬발동확률",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","씨앗보너스보조스킬발동확률.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_wAwakenSkillBonusActiveChanceForBoJoSKill	=	iValue;
			continue;
		}

		if(STRICMP("씨앗보너스보조스킬발동확률per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","씨앗보너스보조스킬발동확률per레벨.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_wAwakenSkillBonusActiveChanceForBoJoSKillPerLevel	=	iValue;
			continue;
		}

		if (STRICMP("발동확률한계",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","발동률한계가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wLimitActiveChance	=	iValue;
			continue;
		}
		if (STRICMP("범위내적개수에의한발동확률",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","범위내적개수에의한발동확률이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wActiveChanceByInRangeEnemyCount	=	iValue;
			continue;
		}
		if (STRICMP("입힌대미지를체력으로흡수",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","입힌대미지를체력으로흡수가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wDamageToHP			=	iValue;
			continue;
		}
		if (STRICMP("입힌대미지를체력으로흡수per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","입힌대미지를체력으로흡수per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wDamageToHPPerLevel	=	iValue;
			continue;
		}
		if (STRICMP("적용주기",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","적용주기가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sOccurActionPeriod	=	iValue;
			continue;
		}
		if (STRICMP("오라적용주기",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","오라적용주기가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sAuraActivePeriod	=	iValue;
			continue;
		}
		if (STRICMP("오라적용주기per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","오라적용주기per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sAuraActivePeriodPerLevel	=	iValue;
			continue;
		}
		if (STRICMP("오라적용주기올림계산",keyword)==0)
		{
			skill.m_sAuraActivePeriodDecimalMethod	=	eDECIMAL_METHOD_UP;
			continue;
		}
		if (STRICMP("오라적용주기반올림계산",keyword)==0)
		{
			skill.m_sAuraActivePeriodDecimalMethod	=	eDECIMAL_METHOD_ROUND_OFF;
			continue;
		}

		if (STRICMP("물약회복수치",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","물약회복수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[0]	=	iValue;
			continue;
		}
		if (STRICMP("물약회복수치per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","물약회복수치per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[1]	=	iValue;
			continue;
		}

		if (STRICMP("사용가능물약수",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","사용가능물약수가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[2]	=	iValue;
			continue;
		}
		if (STRICMP("사용가능물약",keyword)==0)
		{
			int		iIndex			=	reader.getNumber();
			int		iRequireLevel	=	reader.getNumber();
			char	*lpstrItemName	=	reader.getStream();

			iIndex					=	3+iIndex*2;

			if	(iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","물약회복수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[iIndex]	=	iRequireLevel;
			skill.m_aValue[iIndex+1]=	cItem::GetItemIndexByName(lpstrItemName);
			continue;
		}

		if (STRICMP("스킬복사",keyword)==0)
		{
			str			=	reader.getStream();
			int	iJob	=	CDecoder::AnalCode(l_aJobNameKeyword,str);

			if (iJob	==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","스킬복사의 직업이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			str			=	reader.getStream();
			int	iSkill	=	getIndex(str,iJob);
			int	iSerial	=	skill.m_wSerial;

			if	(iSkill	==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","스킬복사의 스킬이 이상합니다.\n\n%s-%s",fn,str);

			char	strName[dSKILL_NAME_LENGTH];
			
			strcpy(strName,skill.m_strName);
			memcpy(&skill,&g_aSkill[iSkill],sizeof(CSkill));
			memset(skill.m_aRequireSkill,0xff,sizeof(skill.m_aRequireSkill));
			memset(skill.m_aAi,0xff,sizeof(skill.m_aAi));
			strcpy(skill.m_strName,strName);

			skill.m_wSerial	=	iSerial;
			bIsSkillCopy	=	TRUE;
			continue;
		}

		if (STRICMP("스킬타입",keyword)==0)
		{
			str		=	reader.getWord();

			iValue	=	CDecoder::AnalCode(l_aCastType,str);

			if (iValue		==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","스킬타입이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wCastMethod	=	iValue;
			continue;
		}


		if (STRICMP("시전시간",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","시전시간가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sCastingTime	=	iValue;
			continue;
		}

		if (STRICMP("치유효과",keyword)==0)
		{
			str		=	reader.getWord();

			iValue	=	CDecoder::AnalCode(l_aCureEffect,str);

			if (iValue		==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","치유효과가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wCureEffectField	|=	iValue;
			continue;
		}

		if	(STRICMP("타겟방어력보정",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","타겟방어력보정이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wCorrectTargetDefensivePower	=	iValue;
			continue;
		}

		if (STRICMP("대상레벨제한타입1",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","대상레벨제한타입1이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sTargetLevelLimitType1	=	iValue;
			continue;
		}

		if (STRICMP("동물타입레벨제한타입1",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","동물타입레벨제한타입1이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sAnimalTargetLevelLimitType1	=	iValue;
			continue;
		}
		if (STRICMP("신수타입레벨제한타입1",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","대상레벨제한타입1이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sHolyBeastTargetLevelLimitType1	=	iValue;
			continue;
		}
		if (STRICMP("인간타입레벨제한타입1",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","인간타입레벨제한타입1이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sHumanTargetLevelLimitType1	=	iValue;
			continue;
		}
		if (STRICMP("악마타입레벨제한타입1",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","악마타입레벨제한타입1이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDemonTargetLevelLimitType1	=	iValue;
			continue;
		}
		if (STRICMP("언데드타입레벨제한타입1",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","언데드타입레벨제한타입1이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sUndeadTargetLevelLimitType1	=	iValue;
			continue;
		}

		if	(STRICMP("적용제한HP퍼센트",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","적용제한HP퍼센트가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sApplyLimitHPPercentage			=	iValue;
			continue;
		}
		if	(STRICMP("적용제한HP퍼센트per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","적용제한HP퍼센트per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sApplyLimitHPPercentagePerLevel	=	iValue;
			continue;
		}
		if	(STRICMP("적용제한HP퍼센트한계",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if	(iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","적용제한HP퍼센트한계가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wLimitApplyLimitHPPercentage	=	iValue;
			continue;
		}
		if (STRICMP("현HP기반회복수치퍼센트",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","현HP기반회복수치퍼센트가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sHealPercentageBasedCurrentHP			=	iValue;
			continue;
		}
		if (STRICMP("현HP기반회복수치퍼센트per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","현HP기반회복수치퍼센트per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sHealPercentageBasedCurrentHPperLevel	=	iValue;
			continue;
		}

		if	(STRICMP(keyword,"파티오라")==0)
		{
			skill.m_bf1IsApplyToPartyAura	=	TRUE;
			continue;
		}
		if	(STRICMP("오비티안스킬",keyword)==0)
		{
			skill.m_bf1IsObitianSkill	=	TRUE;
			continue;
		}
		if	(STRICMP("대쉬블레이드스킬",keyword)==0)
		{
			skill.m_bf1IsDashBladeSkill	=	TRUE;
			continue;
		}
		if	(STRICMP("아스트로보우스킬",keyword)==0)
		{
			skill.m_bf1IsAstroBowSkill	=	TRUE;
			continue;
		}
		if	(STRICMP("크리스탈워터스킬",keyword)==0)
		{
			skill.m_bf1IsCristalWaterSkill	=	TRUE;
			continue;
		}

		if	(STRICMP("첫번째효과가실패하면나머지효과는취소",keyword)==0)
		{
			skill.m_bf1IsStopWhenFailedFirstSE	=	TRUE;
			continue;
		}
		if	(STRICMP("미스이미지필요",keyword)	==	0)
		{
			skill.m_bf1IsRequireMissImage	=	TRUE;
			continue;
		}
		if	(STRICMP("결투필드사용불가",keyword)	==	0)
		{
			skill.m_bf1IsBlockInDuelField	=	TRUE;
			continue;
		}

		if	(STRICMP("타격이미지대신회복이미지사용",keyword)==0)
		{
			skill.m_bf1IsUsHealEffectToHitImageForAuraSkill	=	TRUE;
			continue;
		}

		if	(STRICMP("토글스킬",keyword)==0)
		{
			skill.m_bf1IsToggleSkill	=	TRUE;
			continue;
		}

		if	(STRICMP("죽은척하기상태필요",keyword)==0)
		{
			skill.m_bf1IsRequireFeignDeathStatus	=	TRUE;
			continue;
		}

		if	(STRICMP("공격력남은체력에비례",keyword)==0)
		{
			skill.m_bf1IsAttackDamageBasedAttackerRemainHPRate	=	TRUE;
			continue;
		}
		
		if	(STRICMP("저주저항으로녹백저항",keyword)==0)
		{
			skill.m_bf1IsResistKnockBackByCurse	=	TRUE;
			continue;
		}
		if	(STRICMP("환영스킬",keyword)==0)
		{
			skill.m_bf1IsIllusionAttack		=	TRUE;
			continue;
		}
		if	(STRICMP("목적지까지이동후스킬사용",keyword)==0)
		{
			skill.m_bf1IsCastOnDestPosSkill	=	TRUE;
			continue;
		}
		if	(STRICMP("방어력저주저항으로계산",keyword)==0)
		{
			skill.m_bf1DefensePowerByCurseResistance	=	TRUE;
			continue;
		}
		if	(STRICMP("공격력제곱근으로처리",keyword)==0)
		{
			skill.m_bf1RootAttackPower	=	TRUE;
			continue;
		}
		if	(STRICMP("비공격적스킬",keyword)==0)
		{
			skill.m_bf1IsNonAggressiveSkill						=	TRUE;
			continue;
		}
		if	(STRICMP("자폭스킬",keyword)==0)
		{
			skill.m_bf1IsSelfDestructionSkill					=	TRUE;
			continue;
		}

		if	(STRICMP("타격대미지표시",keyword)==0)
		{
			skill.m_bf1IsPutHitDamage							=	TRUE;
			continue;
		}

		if	(STRICMP("회피블럭리액션무시",keyword)==0)
		{
			skill.m_bf1IsIgnoreDodgeBlockReaction	=	TRUE;
			continue;
		}
		
		if	(STRICMP("회복수치퍼센티지로적용",keyword)==0)
		{
			skill.m_bf1IsApplyHealPointByPercentage	=	TRUE;
			continue;
		}
		
		if	(STRICMP("회복수치",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","회복수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wHealPoint	=	iValue;
			continue;
		}
		if	(STRICMP("회복수치per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","회복수치per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wHealPointPerLevel	=	iValue;
			continue;
		}
		if (STRICMP("수치",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			int	iValue2	=	reader.getNumber();

			if (iValue2	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","수치2가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[iValue]	=	iValue2;
			continue;
		}

		if	(STRICMP("폭발이미지머리위에출력",keyword)==0)
		{
			skill.m_aValue[0]		=	1;
			continue;
		}
		if	(STRICMP("바이트행잉해제대미지",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","바이트행잉해제대미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[0]		=	iValue;
			continue;
		}
		if	(STRICMP("바이트행잉유지가능체력",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","바이트행잉유지가능체력이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[1]		=	iValue;
			continue;
		}

		if (STRICMP("캐릭터스킬릴리즈위치사용",keyword)==0)
		{
			skill.m_aValue[0]		=	1;
			continue;
		}
		if (STRICMP("캐릭터와싱크시켜사용",keyword)==0)
		{
			skill.m_aValue[1]		=	1;
			continue;
		}

		if (STRICMP("함정해체스킬",keyword)==0)
		{
			skill.m_wSkillUniqueSpecialFeature	=	eSUSF_IS_DISARM;
			continue;
		}
		if (STRICMP("잠긴문열기스킬",keyword)==0)
		{
			skill.m_wSkillUniqueSpecialFeature	=	eSUSF_IS_UNLOCK_DOOR;
			continue;
		}
		if (STRICMP("잠긴상자열기스킬",keyword)==0)
		{
			skill.m_wSkillUniqueSpecialFeature	=	eSUSF_IS_UNLOCK_ARCA;
			continue;
		}

		if (STRICMP("파티원과전투중인적만공격",keyword)==0)
		{
			skill.m_bf1IsTagetingToBattleWithPartyMember	=	TRUE;
			continue;
		}
		if (STRICMP("이미지기반함정",keyword)==0)
		{
			skill.m_bf1IsDefendOnImageTrap	=	TRUE;
			continue;
		}
		if (STRICMP("즉시적용오라",keyword)==0)
		{
			skill.m_bf1IsInstanceApplyAura	=	TRUE;
			continue;
		}
		if (STRICMP("무기수감소",keyword)==0)
		{
			skill.m_bf1IsDecreaseWeaponCount	=	TRUE;
			continue;
		}
		if (STRICMP("그림자숨기",keyword)==0)
		{
			skill.m_bf1IsShadowHideSkill		=	TRUE;
			continue;
		}
		if (STRICMP("그림자숨기효과적용",keyword)==0)
		{
			skill.m_bf1IsApplyShadowHideEffect		=	TRUE;
			continue;
		}
		if (STRICMP("오른손스킬",keyword)==0)
		{
			skill.m_bf1IsRightPunchSkill	=	TRUE;
			continue;
		}
		if (STRICMP("왼손스킬",keyword)==0)
		{
			skill.m_bf1IsLeftPunchSkill	=	TRUE;
			continue;
		}
		if (STRICMP("오른발스킬",keyword)==0)
		{
			skill.m_bf1IsRightKickSkill	=	TRUE;
			continue;
		}
		if (STRICMP("왼발스킬",keyword)==0)
		{
			skill.m_bf1IsLeftKickSkill	=	TRUE;
			continue;
		}
		if (STRICMP("아래차기스킬",keyword)==0)
		{
			skill.m_bf1IsDownKickSkill	=	TRUE;
			continue;
		}
		if (STRICMP("그라운드미사일",keyword)==0)
		{
			skill.m_bf1IsGroundMissile	=	TRUE;
			continue;
		}
		if (STRICMP("미사일공격만블럭",keyword)==0)
		{
			skill.m_bf1IsBlockOnlyMissilAttack	=	TRUE;
			continue;
		}
		if (STRICMP("미스이미지없음",keyword)==0)
		{
			skill.m_bf1IsHideMissImage	=	TRUE;
			continue;
		}
		if (STRICMP("인스턴스힐",keyword)==0)
		{
			skill.m_bf1IsInstanceHeal	=	TRUE;
			continue;
		}
		if (STRICMP("함정",keyword)==0)
		{
			skill.m_bf1IsTrap	=	TRUE;
			continue;
		}
		if (STRICMP("평평한함정",keyword)==0)
		{
			skill.m_bf1IsFlatTrap	=	TRUE;
			continue;
		}
		if (STRICMP("소환수명령",keyword)==0)
		{
			skill.m_bf1IsSummonBeastCommand		=	TRUE;
			continue;
		}
		if (STRICMP("결투장효율페널티",keyword)==0)
		{
			iValue	=	reader.getNumber();

			skill.m_bf7DuelServerpenaltyDiv = iValue;
			continue;
		}
		if (STRICMP("실행소환수",keyword)==0)
		{
			str		=	reader.getStream();
			iValue	=	reader.getNumber();

			skill.m_bf2RequireSummonBeastGrade	=	iValue;

			if (STRICMP(str,"켈비")==0)
				skill.m_bf2OperateSummonBeast	=	dSBT_KELBY;
			if (STRICMP(str,"스웰퍼")==0)
				skill.m_bf2OperateSummonBeast	=	dSBT_SWELLFER;
			if (STRICMP(str,"윈디")==0)
				skill.m_bf2OperateSummonBeast	=	dSBT_WINDY;
			if (STRICMP(str,"헷저")==0)
				skill.m_bf2OperateSummonBeast	=	dSBT_HEDGER;
			continue;
		}
		if (STRICMP("액션효과음",keyword)==0)
		{
			str						=	reader.getStream();
			if (strlen(str) >= dSKILL_SOUND_FILE_LENGTH -1)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","액션효과음 화일명이 너무 깁니다.\n\n%s-%s",fn,skill.m_strName);

			strcpy(skill.m_sound.m_strAction,str);
			continue;
		}
		if (STRICMP("폭발효과음",keyword)==0)
		{
			str						=	reader.getStream();
			if (strlen(str) >= dSKILL_SOUND_FILE_LENGTH -1)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","폭발효과음 화일명이 너무 깁니다.\n\n%s-%s",fn,skill.m_strName);

			strcpy(skill.m_sound.m_strExplosion,str);
			continue;
		}
		if (STRICMP("타격효과음",keyword)==0)
		{
			str						=	reader.getStream();
			if (strlen(str) >= dSKILL_SOUND_FILE_LENGTH -1)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","타격효과음 화일명이 너무 깁니다.\n\n%s-%s",fn,skill.m_strName);

			strcpy(skill.m_sound.m_strHit,str);
			continue;
		}
		if (STRICMP("시전효과음",keyword)==0)
		{
			str						=	reader.getStream();
			if (strlen(str) >= dSKILL_SOUND_FILE_LENGTH -1)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","캐스트효과음 화일명이 너무 깁니다.\n\n%s-%s",fn,skill.m_strName);

			strcpy(skill.m_sound.m_strCasting,str);
			continue;
		}

		if (STRICMP("생성효과음",keyword)==0)
		{
			str						=	reader.getStream();
			if (strlen(str) >= dSKILL_SOUND_FILE_LENGTH -1)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","생성효과음 화일명이 너무 깁니다.\n\n%s-%s",fn,skill.m_strName);

			strcpy(skill.m_sound.m_strCreate,str);
			continue;
		}

		if (STRICMP("실패효과음",keyword)==0)
		{
			str						=	reader.getStream();
			if	(strlen(str) >= dSKILL_SOUND_FILE_LENGTH -1)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","실패효과음 화일명이 너무 깁니다.\n\n%s-%s",fn,skill.m_strName);

			strcpy(skill.m_sound.m_strMiss,str);
			continue;
		}

		if	(STRICMP("부가효과성공타격효과음",keyword)==0)
		{
			str					=	reader.getWord();
			int	iSound			=	CDecoder::AnalCode(l_aSoundType,str);

			if	(iSound			==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","화일 '%s'에 직업제한의 '부가효과성공타격효과음'을 정의 할 수 없습니다.'",fn,str);

			skill.m_sound.m_wSEE_Success	=	iSound;
			continue;
		}
		if	(STRICMP("부가효과실패타격효과음",keyword)==0)
		{
			str					=	reader.getWord();
			int	iSound			=	CDecoder::AnalCode(l_aSoundType,str);

			if	(iSound			==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","화일 '%s'에 직업제한의 '부가효과실패타격효과음'을 정의 할 수 없습니다.'",fn,str);

			skill.m_sound.m_wSEE_Miss		=	iSound;
			continue;
		}
		if (STRICMP("직업제한",keyword)==0)
		{
			str					=	reader.getStream();
			l_iEnableJob		=	CDecoder::AnalCode(l_aJobNameKeyword,str);

			if	(l_iEnableJob	==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","화일 '%s'에 직업제한의 직업 '%s'를 정의 할 수 없습니다.'",fn,str);
			continue;
		}

		if (STRICMP("스킬",keyword)==0)
		{
			skill.reset();
			iRequireSkillCount	=	0;

			str	=	reader.getStream();
			strcpy(skill.m_strName,str);

			reader.findOpenCode();

			if (STRICMP(str,"플레임 링")==0)
				continue;

			continue;
		}

		if (STRICMP("유형",keyword)==0)
		{
			str		=	reader.getWord();
			iValue	=	CDecoder::AnalCode(l_aSkillType,str);

			if (iValue==dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","유형이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wType	=	iValue;
			continue;
		}

		if (STRICMP("블럭률",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","블럭률이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sBlockingChance=	iValue;
			continue;
		}

		if (STRICMP("블럭률per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","블럭률per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sBlockingChancePerLevel=	iValue;
			continue;
		}
		if (STRICMP("한계블럭률",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","한계블럭률이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sMaximumBlockingChance	=	iValue;
			continue;
		}

		if (STRICMP("블럭률고정",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","블럭률고정이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sFixBlockingChance=	iValue;
			continue;
		}

		if (STRICMP("회피율",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","회피율이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDodgeChance	=	iValue;
			
			continue;
		}
		if (STRICMP("회피율per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","회피율per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDodgeChancePerLevel	=	iValue;
			continue;
		}

		if (STRICMP("범위공격대미지퍼센트",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","범위공격대미지퍼센트가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wWideAreaAttackDamagePercent	=	iValue;
			
			continue;
		}

		if (STRICMP("범위공격대미지퍼센트per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","범위공격대미지퍼센트per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wWideAreaAttackDamagePercentPerLevel	=	iValue;
			
			continue;
		}

		
		if	(STRICMP("당기기사정거리",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","당기기사정거리가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_aValue[1]	=	iValue;
			
			continue;
		}
		if	(STRICMP("당기기사정거리per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","당기기사정거리per레벨가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_aValue[5]	=	iValue;
			
			continue;
		}
		if	(STRICMP("당기기주기",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","당기기주기가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_aValue[2]	=	iValue;
			
			continue;
		}
		if	(STRICMP("당기는거리",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","당기는거리가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_aValue[3]	=	iValue;
			
			continue;
		}
		if	(STRICMP("당기기최소거리",keyword)==0)
		{
			iValue	=	reader.getNumber();
			
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","당기기최소거리가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_aValue[4]	=	iValue;
			
			continue;
		}
		if(STRICMP("중복타겟금지",keyword)==0)
		{
			skill.m_aValue[0] = TRUE;
			continue;
	
		}

		if (STRICMP("비스트버서커",keyword)==0)
		{
			skill.m_bf1BeastBerserker	=	TRUE;
			continue;
		}

		if (STRICMP("시전과동시에버프",keyword)==0)
		{
			skill.m_bf1IsCastSelfBuff	=	TRUE;
			continue;
		}
		
		
		if (STRICMP("소환수스킬",keyword)==0)
		{
			skill.m_bf1IsSummonedBeastSkill	=	TRUE;
			continue;
		}

		if	(STRICMP("블랙블루스수치",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if	(iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","블랙블루스수치 수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wBlackBluesFactor	=	iValue;
			
			continue;
		}
		if	(STRICMP("블랙블루스수치per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if	(iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","블랙블루스수치per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wBlackBluesFactorPerLevel	=	iValue;
			
			continue;
		}

		if	(STRICMP("블랙블루스수치한계",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if	(iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","블랙블루스per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wBlackBluesFactorLimit	=	iValue;
			
			continue;
		}

		if	(STRICMP("공격력",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if	(iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","공격력이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sAttackPoint=	iValue;
			
			continue;
		}
		if (STRICMP("공격력per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","공격력per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sAttackPointPerLevel=	iValue;
			continue;
		}
		if (STRICMP("공격력per자신레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","공격력per자신레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sAttackPointPerActorLevel=	iValue;
			continue;
		}
		if (STRICMP("추가대미지퍼센트",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","추가대미지퍼센트가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sAddDamagePercent=	iValue;
			continue;
		}
		if (STRICMP("마법대미지주사위굴림횟수",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","마법대미지주사위굴림횟수가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_bf5MagicDamageDiceCount=	iValue;
			continue;
		}
		if (STRICMP("마법대미지주사위굴림횟수per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","마법대미지주사위굴림횟수per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sMagicDamageDiceCountPerLevel=	iValue;
			continue;
		}
		if (STRICMP("마법대미지주사위굴림횟수한계",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","마법대미지주사위굴림횟수한계가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_bf5MagicDamageDiceCountLimit=	iValue;
			continue;
		}
		if (STRICMP("무기변신중에도사용가능",keyword)==0)
		{
			skill.bf1IsAbleSkillWhenTransToWeapon=	TRUE;
			continue;
		}
		if (STRICMP("공격력범위per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","공격력범위per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sAttackPointRangePerLevel=	iValue;
			continue;
		}
		if (STRICMP("공격력퍼센트",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","공격력퍼센트가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sAttackPercentage=iValue;
			continue;
		}
		if (STRICMP("공격력퍼센트per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","공격력퍼센트per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sAttackPercentagePerLevel=	iValue;
			continue;
		}

		if (STRICMP("이상계저항",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","이상계저항이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sStrangeStatusResistance			=	iValue;
			continue;
		}

		if (STRICMP("이상계저항per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","이상계저항per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sStrangeStatusResistancePerLevel	=	iValue;
			continue;
		}

		if (STRICMP("마비저항",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","마비저항이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sStunResistance	=	iValue;
			continue;
		}

		if (STRICMP("마비저항per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","마비저항per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sStunResistancePerLevel	=	iValue;
			continue;
		}

		if (STRICMP("관통확률",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","관통확률이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sPiercingChance	=	iValue;
			continue;
		}
		if (STRICMP("관통확률per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","관통확률per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sPiercingChancePerLevel	=	iValue;
			continue;
		}

		if (STRICMP("방어력",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","방어력이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDefensePoint=	iValue;
			continue;
		}
		if (STRICMP("방어력per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","방어력per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDefensePointPerLevel=	iValue;
			continue;
		}
		if (STRICMP("방어력퍼센트",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","방어력퍼센트가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDefensePercentage=	iValue;
			continue;
		}
		if (STRICMP("방어력퍼센트per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","방어력퍼센트per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDefensePercentagePerLevel=	iValue;
			continue;
		}


		if (STRICMP("최소사정거리",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","최소사정거리가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wMinimumShootRange	=	iValue;
			continue;
		}
		if (STRICMP("사정거리보정",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","사정거리보정이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wShootRangeCorrect	=	iValue;
			continue;
		}

		if (STRICMP("사정거리",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","사정거리가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wShootRange=	iValue;
			continue;
		}
		if (STRICMP("사정거리per레벨",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","사정거리per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wShootRangePerLevel=iValue;
			continue;
		}
		if (STRICMP("무기사정거리보정",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","무기사정거리보정이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wWeaponShootRangeCorrect	=	iValue;
			continue;
		}
		if (STRICMP("무기사정거리보정per레벨",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","무기사정거리보정per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wWeaponShootRangeCorrectPerLevel	=	iValue;
			continue;
		}
		if (STRICMP("무기타격범위보정",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","무기타격범위보정이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wWeaponHitRangeCorrect	=	iValue;
			continue;
		}

		if (STRICMP("캐릭터잔상",keyword)==0)
		{
			str			=	reader.getWord();

			if (strcmp(str,"딜레이")==0)
			{
				skill.m_bCharacterAfterImageType	=	eCAIT_DELAY;

				int	iValue	=	reader.getNumber();

				if (iValue	==	dCODE_VALID_NUMBER)
					EXIT("error in CSkillManager::decodeScripter(char *fn)","캐릭터잔상딜레이타임이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

				skill.m_bCharacterAfterImageDelayTime	=	iValue;
			}
			else	EXIT("error in CSkillManager::decodeScripter(char *fn)","캐릭터잔상이 이상합니다.\n\n%s-%s",skill.m_strName,str);

			continue;
		}

		if (STRICMP("오버랩액션",keyword)==0)
		{
			str			=	reader.getStream();

					if (strcmp(str,"기본 공격")==0)		skill.m_wAction		=	dACT_DEFAULT_ATTACK;
			else	if (strcmp(str,"기본 마법")==0)		skill.m_wAction		=	dACT_DEFAULT_MAGIC;
			else
			{
				int	iAction	=	CDecoder::AnalCode(l_aSkillAction[l_iEnableJob],str);

				if (iAction >= 0	&&	iAction	!=	dCODE_UNDEFINE)	skill.m_wOverlapAction	=	iAction;
				else	EXIT("error in CSkillManager::decodeScripter(char *fn)","오버랩액션이 이상합니다.\n\n%s-%s",skill.m_strName,str);
			}
			continue;
		}
		if (STRICMP("오버랩액션2",keyword)==0)
		{
			str			=	reader.getStream();

					if (strcmp(str,"기본 공격")==0)		skill.m_wAction		=	dACT_DEFAULT_ATTACK;
			else	if (strcmp(str,"기본 마법")==0)		skill.m_wAction		=	dACT_DEFAULT_MAGIC;
			else
			{
				int	iAction	=	CDecoder::AnalCode(l_aSkillAction[l_iEnableJob],str);

				if (iAction >= 0	&&	iAction	!=	dCODE_UNDEFINE)	skill.m_wOverlapAction2	=	iAction;
				else	EXIT("error in CSkillManager::decodeScripter(char *fn)","오버랩액션이 이상합니다.\n\n%s-%s",skill.m_strName,str);
			}
			continue;
		}

		if (STRICMP("오버랩액션과싱크된중첩공격횟수",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","오버랩액션과싱크된중첩공격횟수 수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wReiterationDamageCountSyncWithOverlapAction	=	iValue;

			continue;
		}

		if (STRICMP("액션",keyword)==0)
		{
			str			=	reader.getStream();

					if (strcmp(str,"기본 공격")==0)		skill.m_wAction		=	dACT_DEFAULT_ATTACK;
			else	if (strcmp(str,"기본 마법")==0)		skill.m_wAction		=	dACT_DEFAULT_MAGIC;
			else
			{
				int	iAction	=	CDecoder::AnalCode(l_aSkillAction[l_iEnableJob],str);

				if (iAction >= 0	&&	iAction	!=	dCODE_UNDEFINE)	skill.m_wAction		=	iAction;
				else	EXIT("error in CSkillManager::decodeScripter(char *fn)","액션이 이상합니다.\n\n%s-%s",skill.m_strName,str);
					
			}
			continue;
		}
		if	(STRICMP("액션2",keyword)==0)
		{
			str			=	reader.getStream();

			if	(strcmp(str,"기본 공격")==0)
				skill.m_wAction2	=	dACT_DEFAULT_ATTACK;
			else
			if	(strcmp(str,"기본 마법")==0)
				skill.m_wAction2	=	dACT_DEFAULT_MAGIC;
			else
			{
				int	iAction	=	CDecoder::AnalCode(l_aSkillAction[l_iEnableJob],str);

				if	(iAction >= 0	&&	iAction	!=	dCODE_UNDEFINE)
					skill.m_wAction2	=	iAction;
				else
					EXIT("error in CSkillManager::decodeScripter(char *fn)","액션2가 이상합니다.\n\n%s-%s",skill.m_strName,str);
			}
			continue;
		}
		if	(STRICMP("사용대상",keyword)==0)
		{
			while(1)
			{
				str					=	reader.getWord();
				int	iDest			=	CDecoder::AnalCode(l_aSkillUseTo,str);

				if	(iDest	==	dCODE_VALID_NUMBER)
					EXIT("error in CSkillManager::decodeScripter(char *fn)","사용대상이 이상합니다.\n\n%s-%s",fn,skill.m_strName);
				skill.m_dwTargetMethod	+=	iDest;

				iCh	=	reader.getNextChar();
				if	(iCh	==	';'	)
					break;
				if	(iCh	==	0	)
					EXIT("error in CSkillManager::decodeScripter(char *fn)","사용대상이 이상합니다.\n\n'%s-%s'",fn,skill.m_strName);
			}
		
			continue;
		}

		if (STRICMP("탐지레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","탐지레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDetectLevel		=	iValue;
			continue;
		}
		if (STRICMP("탐지레벨per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","탐지레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDetectLevelPerLevel=	iValue;
			continue;
		}
		if (STRICMP("해체레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","해체레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDisarmLevel		=	iValue;
			continue;
		}
		if (STRICMP("해체레벨per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","해체레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDisarmLevelPerLevel=	iValue;
			continue;
		}
		if (STRICMP("소모HP퍼센트",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","소모HP퍼센트가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wSpentHPPercentage	=	iValue;
			continue;
		}
		if (STRICMP("소모HP퍼센트based블러디윙",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","소모HP퍼센트based블러디윙이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wSpentHPPercentageBasedBloodWing	=	iValue;
			continue;
		}

		if (STRICMP("소모CP",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","소모CP가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wSpentCP	=	iValue;
			continue;
		}
		if (STRICMP("소모CPper레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","소모CPper레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wSpentCPPerLevel	=	iValue;
			continue;
		}
		if (STRICMP("획득CP",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","획득CP가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wGetCP		=	iValue;
			continue;
		}
		if (STRICMP("획득CPper레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","획득CPper레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wGetCPPerLevel	=	iValue;
			continue;
		}
		if (STRICMP("소모비트",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","소모비트가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_bf4SpendBit	=	iValue;
			continue;
		}
		if (STRICMP("비트부착중인적만공격",keyword)==0)
		{
			skill.m_bf1IsTargetingToActorStickedBit	=	TRUE;
			continue;
		}
		if (STRICMP("빛관련상태이상일때크리티컬효과적용",keyword)==0)
		{
			skill.m_bf1IsApplyCriticalEffectWhenBeLightEffect	=	TRUE;
			continue;
		}
		if (STRICMP("빛관련상태이상아닐때최소대미지",keyword)==0)
		{
			skill.m_bf1IsApplyMinDamageWhenBeNotLightEffect	=	TRUE;
			continue;
		}
		if (STRICMP("비트부착중인적히트시폭발공격",keyword)==0)
		{
			skill.m_bf1IsApplyExplosionWhenHitTargetStickedBit	=	TRUE;
			continue;
		}
		if (STRICMP("중복타겟적용",keyword)==0)
		{
			skill.m_bf1IsApplySameTarget	=	TRUE;
			continue;
		}
		if (STRICMP("포물선높이",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","포물선높이가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			
			skill.m_bf11ArcHeight	=	iValue;

			continue;
		}
		if (STRICMP("FPS보정",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","FPS보정이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wCorrectFPS	=	iValue;
			continue;
		}
		if (STRICMP("속도",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","속도가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wSpeed=	iValue;
			continue;
		}
		if (STRICMP("미사일속도",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","미사일속도가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wSpeed=	iValue;
			continue;
		}
		if (STRICMP("공격속도",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","공격속도가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sAttackSpeed	=	iValue;
			continue;
		}
		if (STRICMP("공격속도per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","공격속도per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sAttackSpeedPerLevel	=	iValue;
			continue;
		}
		if (STRICMP("공격한계속도",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","공격한계속도가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sLimitAttackSpeed	=	iValue;
			continue;
		}
		if (STRICMP("공격속도보정",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","공격속도보정이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sCorrectAttackSpeed			=	iValue;
			continue;
		}
		if (STRICMP("공격속도보정per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","공격속도보정per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sCorrectAttackSpeedPerLevel	=	iValue;
			continue;
		}
		if (STRICMP("캐스팅즉시지진발생",keyword)==0)
		{
			skill.m_wShakeTiming	=	eST_CAST;
			continue;
		}
		if (STRICMP("지진강도",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","지진강도가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wShakeIntensity			=	iValue;
			continue;
		}
		if (STRICMP("지진시간",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","지진시간이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wShakeTime				=	iValue;
			continue;
		}
		if (STRICMP("이동속도보정",keyword)==0)
		{
			continue;
		}
		if (STRICMP("이동속도per레벨",keyword)==0)
		{
			continue;
		}

		if (STRICMP("소환가능한펫의수",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","소환가능한펫의수가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_bf3MaxPetCount	=	iValue;
			continue;
		}

		if (STRICMP("필요장비",keyword)==0)
		{
			while(1)
			{
				int	iEquipment;
				str							=	reader.getWord();
				iEquipment					=	CDecoder::AnalCode(l_aRequireEquipment,str);
				if (iEquipment	==	dCODE_VALID_NUMBER)
					EXIT("error in CSkillManager::decodeScripter(char *fn)","필요장비가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
				skill.m_wRequireEquipment	|=	iEquipment;

				iCh	=	reader.getNextChar();
				if (iCh	==	';'	)	break;
				if (iCh	==	0	)	EXIT("error in CSkillManager::decodeScripter(char *fn)","필요장비가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			}
			continue;
		}

		if (STRICMP("펫명령",keyword)==0)
		{
			keyword	=	reader.getWord();

			if (STRICMP("공격명령",keyword)==0)
				skill.m_wPetCommand	=	ePC_ATTACK;
			else
			if (STRICMP("방어명령",keyword)==0)
				skill.m_wPetCommand	=	ePC_DEFENSE;
			else
			if (STRICMP("순찰명령",keyword)==0)
				skill.m_wPetCommand	=	ePC_PATROL;
			else
			if (STRICMP("특기사용",keyword)==0)
				skill.m_wPetCommand	=	ePC_SPECIAL_ACTION;
			else
			if (STRICMP("스킬사용",keyword)==0)
				skill.m_wPetCommand	=	ePC_USE_SKILL;
			else
				EXIT("error in CSkillManager::decodeScripter(char *fn)","펫명령이 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}
		if (STRICMP("명령스킬",keyword)==0)
		{
			reader.getStream();
			continue;
		}
		if (STRICMP("무기감추기",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","무기감추기가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sHideWeaponTime	=	iValue;
			continue;
		}

		if (STRICMP("무기감추기per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","무기감추기가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sHideWeaponTimePerLevel=	iValue;
			continue;
		}

		if (STRICMP("명중률",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","명중률이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sHitChance=	iValue;
			continue;
		}
		if (STRICMP("명중률per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","명중률per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sHitChancePerLevel=	iValue;
			continue;
		}
		if (STRICMP("명중률고정",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","명중률고정이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sFixHitChance=	iValue;
			continue;
		}
		if (STRICMP("잔상",keyword)==0)
		{
			str		=	reader.getWord();

			iValue	=	CDecoder::AnalCode(l_aAfterImageType,str);

			if (iValue		==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","잔상이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wAfterImageType=	iValue;

			continue;
		}
		if (STRICMP("잔상간격",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","잔상간격이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wAfterImageGap	=	iValue;
			continue;
		}
		if (STRICMP("첫번째잔상농도",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","첫번째잔상농도가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wAfterImageFirstImageAlphaDepth	=	iValue;
			continue;
		}
		if (STRICMP("첫번째잔상위치",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","첫번째잔상위치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wAfterImageFirstImageDistance	=	iValue;
			continue;
		}
		if (STRICMP("잔상농도감소값",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","잔상농도감소값이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sAfterImageDecreaseAlphaDepthValue	=	iValue;
			continue;
		}
		if (STRICMP("잔상갯수",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","잔상갯수가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wAfterImageCount	=	iValue;
			continue;
		}
		if (STRICMP("이미지반지름",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","이미지반지름이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wImageRadius	=	iValue;
			continue;
		}

		if (STRICMP("이미지스케일",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","이미지스케일이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wImageScale	=	iValue;
			continue;
		}

		if (STRICMP("스윙이미지",keyword)==0)
		{
			str						=	reader.getStream();
			skill.m_wSwingImage		=	g_im.getEffectIndex(str);

			if (skill.m_wSwingImage	==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","스윙이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}

		if (STRICMP("머신이미지",keyword)==0)
		{
			str						=	reader.getStream();
			skill.m_wMachineImage	=	g_im.getEffectIndex(str);

			if (skill.m_wMachineImage==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","머신이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}
		if (STRICMP("시전이미지",keyword)==0)
		{
			str						=	reader.getStream();
			skill.m_wCastImage		=	g_im.getEffectIndex(str);

			if (skill.m_wCastImage	==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","시전이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}
		if (STRICMP("시전이미지위치",keyword)==0)
		{
			str		=	reader.getWord();

			iValue	=	CDecoder::AnalCode(l_aHitEffectOuptuPart,str);

			if (iValue		==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","시전이미지위치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wCastImageOutputPart	=	iValue;
			continue;
		}
		if (STRICMP("시전자타격이미지",keyword)==0)
		{
			str						=	reader.getStream();
			skill.m_wCasterHitImage	=	g_im.getEffectIndex(str);

			if (skill.m_wCasterHitImage	==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","시전자타격이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}
		if (STRICMP("시전자타격이미지위치",keyword)==0)
		{
			str		=	reader.getWord();

			iValue	=	CDecoder::AnalCode(l_aHitEffectOuptuPart,str);

			if (iValue		==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","시전자타격이미지위치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wCasterHitImageOutputPart	=	iValue;
			continue;
		}

		if (STRICMP("스킬이미지",keyword)==0)
		{
			str						=	reader.getStream();
			skill.m_wSkillImage		=	g_im.getEffectIndex(str);

			if (skill.m_wSkillImage	==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","스킬이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}
		if (STRICMP("스킬이미지위치",keyword)==0)
		{
			str		=	reader.getWord();

			iValue	=	CDecoder::AnalCode(l_aHitEffectOuptuPart,str);

			if (iValue		==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","스킬이미지위치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wSkillImageOutputPart	=	iValue;
			continue;
		}
		
		if (STRICMP("미사일머리이미지",keyword)==0)
		{
			str						=	reader.getStream();
			skill.m_wMissileHeadImage		=	g_im.getEffectIndex(str);

			if (skill.m_wMissileHeadImage	==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","미사일머리이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}
		
		if (STRICMP("타겟표적이미지",keyword)==0)
		{
			str						=	reader.getStream();
			skill.m_wTargetMarkImage=	g_im.getEffectIndex(str);

			if (skill.m_wTargetMarkImage	==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","타겟표적이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}
		if (STRICMP("미사일연기이미지",keyword)==0)
		{
			str						=	reader.getStream();
			skill.m_wMissileFollowImage=	g_im.getEffectIndex(str);

			if (skill.m_wShootImage	==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","미사일연기이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}
		if (STRICMP("미사일이미지",keyword)==0)
		{
			str						=	reader.getStream();
			skill.m_wShootImage		=	g_im.getEffectIndex(str);

			if (skill.m_wShootImage	==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","미사일이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}
		if (STRICMP("연기이미지위치",keyword)==0)
		{
			iValue						=	reader.getNumber();
			skill.m_wMissileDustRange	=	iValue;

			if (iValue==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","연기이미지위치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}
		if (STRICMP("연기이미지폭",keyword)==0)
		{
			iValue						=	reader.getNumber();
			skill.m_wDustImageRange		=	iValue;

			if (iValue==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","연기이미지폭이 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}
		if	(STRICMP("시전위치에폭발",keyword)==0)
		{
			skill.m_bf1IsExplosionAtCastPos	=	TRUE;
			continue;
		}
		if	(STRICMP("연기이미지랜덤프레임",keyword)==0)
		{
			skill.m_bf1IsRandomDustImageFrame		=	TRUE;
			continue;
		}
		if (STRICMP("인챈트이미지",keyword)==0)
		{
			str						=	reader.getStream();
			skill.m_wEnchantedImage	=	g_im.getEffectIndex(str);

			if (skill.m_wEnchantedImage	==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","인챈트이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			CEnchantedImageInfo	*lpImage=	g_enchantedImage.get(str);

			if (!lpImage)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","인챈트이미지 정보를 찾을 수 없어효!!\n\n%s-%s-%s",fn,skill.m_strName,str);

			lpImage->m_wImage				=	skill.m_wEnchantedImage;
			skill.m_dwEnchantedEffectMask	=	lpImage->m_dwMask;
			continue;
		}

		if (STRICMP("인챈트마스크",keyword)==0)
		{
			str							=	reader.getStream();
			CEnchantedImageInfo	*lpImage=	g_enchantedImage.get(str);

			if (!lpImage)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","인챈트마스크 정보를 찾을 수 없어효!!\n\n%s-%s-%s",fn,skill.m_strName,str);

			skill.m_dwEnchantedEffectMask	=	lpImage->m_dwMask;
			continue;
		}
		if (STRICMP("인챈트이미지2",keyword)==0)
		{
			str							=	reader.getStream();

			if (STRICMP(str,"플레임 링")==0)
				skill.m_enchantedImage.m_bf1EIFlameRing	=	TRUE;
			else
			if (STRICMP(str,"인시너레이트")==0)
				skill.m_enchantedImage.m_bf1Incinerate	=	TRUE;
			else
			if (STRICMP(str,"베이큠 포인트")==0)
				skill.m_enchantedImage.m_bf1VaccumPoint	=	TRUE;
			else
			if (STRICMP(str,"핫 스킨")==0)
				skill.m_enchantedImage.m_bf1HotSkin		=	TRUE;
			else
			if (STRICMP(str,"다크 웨폰")==0)
				skill.m_enchantedImage.m_bf1DarkWeapon	=	TRUE;
			else
			if (STRICMP(str,"캔서 홀")==0)
				skill.m_enchantedImage.m_bf1CancerHall	=	TRUE;
			else
			if (STRICMP(str,"얼티밋 배리어")==0)
				skill.m_enchantedImage.m_bf1UltimateBarrier=	TRUE;
			else
			if (STRICMP(str,"래빗 러쉬")==0)
				skill.m_enchantedImage.m_bf1IsRabbitRush	=	TRUE;
			else
			if (STRICMP(str,"라이트닝 배리어")==0)
				skill.m_enchantedImage.m_bf1IsLightningBarrier	=	TRUE;
			else
			if (STRICMP(str,"빛의 장막")==0)
				skill.m_enchantedImage.m_bf1IsGreamTant	=	TRUE;
			else
			if (STRICMP(str,"활성화의 권능")==0)
				skill.m_enchantedImage.m_bf1IsPowerOfVitalization	=	TRUE;
			else
			if (STRICMP(str,"해방")==0)
				skill.m_enchantedImage.m_bf1IsIncreaseAllStatus			=	TRUE;
			else
				EXIT("error in CSkillManager::decodeScripter(char *fn)","인챈트이미지2 정보를 찾을 수 없어효!!\n\n%s-%s-%s",fn,skill.m_strName,str);

			continue;
		}

		if (STRICMP("발동조건",keyword)==0)
		{
			str							=	reader.getWord();

			if (STRICMP("블럭킹",keyword)==0)
				skill.m_wActivateTrigger	=	eRSAT_BLOCKING;
			else
			if (STRICMP("때릴때",keyword)==0)
				skill.m_wActivateTrigger	=	eRSAT_STRIKE;
			else
			if (STRICMP("맞을때",keyword)==0)
				skill.m_wActivateTrigger	=	eRSAT_HIT;
			else
			if (STRICMP("대미지입을때",keyword)==0)
				skill.m_wActivateTrigger	=	eRSAT_DAMAGED;
			else
			if (STRICMP("탐지",keyword)==0)	
				skill.m_wActivateTrigger	=	eRSAT_DETECTING;
			else
			if (STRICMP("마법대미지입는순간펫이반응",keyword)==0)
				skill.m_wActivateTrigger	=	eRSAT_DAMAGE_MAGIC_SKILL_SB;
			else
			if (STRICMP("물리공격맞을때",keyword)==0)
				skill.m_wActivateTrigger	=	eRSAT_HIT_DAMAGE_PHYSICAL;
			else
			if (STRICMP("물리대미지입을때",keyword)==0)
				skill.m_wActivateTrigger	=	eRSAT_PHYSICAL_DAMAGED;
			else
			if (STRICMP("물리공격맞을때카운터",keyword)==0)
				skill.m_wActivateTrigger	=	eRSAT_HIT_PHYSICAL_DAMAGE_TIME_COUNTER;
			else
			if (STRICMP("근접공격맞을때카운터",keyword)==0)
				skill.m_wActivateTrigger	=	eRSAT_HIT_CLOSE_RANGE_ATTACK_COUNTER;
			else
			if (STRICMP("근접공격맞을때회피카운터",keyword)==0)
				skill.m_wActivateTrigger	=	eRSAT_HIT_CLOSE_RANGE_ATTACK_DODGE_COUNTER;
			else
			if (STRICMP("원거리공격맞을때",keyword)==0)
				skill.m_wActivateTrigger	=	eRSAT_HIT_MISSILE_ATTACK;
			else
			if (STRICMP("정지상태",keyword)==0)
				skill.m_wActivateTrigger	=	eRSAT_REST;
			else
			if	(STRICMP("맞았다고판정났을때",keyword)==0)
				skill.m_wActivateTrigger	=	eRSAT_JUDGED_BEATEN_TIMING;
			else
			if	(STRICMP("상태이상공격을당할때",keyword)==0)
				skill.m_wActivateTrigger	=	eRSAT_HIT_STRANGE_STATUS_ATTACK;

			continue;
		}

		if (STRICMP("바닥이미지",keyword)==0)
		{
			str						=	reader.getStream();
			skill.m_wBottomImage	=	g_im.getEffectIndex(str);

			if (skill.m_wBottomImage==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","바닥이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}
		
		if (STRICMP("회복이미지",keyword)==0)
		{
			str						=	reader.getStream();
			skill.m_wHealImage		=	g_im.getEffectIndex(str);

			if (skill.m_wHealImage	==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","회복이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}
		if (STRICMP("회복이미지위치",keyword)==0)
		{
			str		=	reader.getWord();
			iValue	=	CDecoder::AnalCode(l_aHitEffectOuptuPart,str);

			if (iValue		==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","회복이미지위치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wHealImageOutputPart	=	iValue;
			continue;
		}
		if (STRICMP("보조공격이미지",keyword)==0)
		{
			str						=	reader.getStream();

			skill.m_wAidAttackImage	=	g_im.getEffectIndex(str);

			if (skill.m_wAidAttackImage	==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","보조공격이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}
		if (STRICMP("보조공격이미지위치",keyword)==0)
		{
			str		=	reader.getWord();
			iValue	=	CDecoder::AnalCode(l_aHitEffectOuptuPart,str);

			if (iValue		==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","보조공격이미지위치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wAidAttackImageOutputPart	=	iValue;
			continue;
		}
		if (STRICMP("타격이미지",keyword)==0)
		{
			str						=	reader.getStream();

			skill.m_wHitImage		=	g_im.getEffectIndex(str);

			if (skill.m_wHitImage	==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","타격이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}
		if (STRICMP("타격이미지효과",keyword)==0)
		{
			str						=	reader.getStream();
			skill.m_wHitImageEffect	=	CDecoder::AnalCode(l_aOutputEffect,str);

			if	(skill.m_wHitImageEffect	==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","타격이미지효과가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}
		if	(STRICMP("타격이미지위치",keyword)==0)
		{
			str		=	reader.getWord();

			iValue	=	CDecoder::AnalCode(l_aHitEffectOuptuPart,str);

			if (iValue		==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","타격이미지위치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wHitImageOutputPart	=	iValue;
			continue;
		}
		if (STRICMP("추가타격이미지",keyword)==0)
		{
			str						=	reader.getStream();
			skill.m_wAddHitImage	=	g_im.getEffectIndex(str);

			if (skill.m_wAddHitImage	==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","추가타격이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}

		if (STRICMP("추가타격이미지위치",keyword)==0)
		{
			str		=	reader.getWord();
			
			iValue	=	CDecoder::AnalCode(l_aHitEffectOuptuPart,str);

			if (iValue		==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","추가타격이미지위치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wAddHitImageOutputPart	=	iValue;
			continue;
		}
		
		if (STRICMP("보조마법캐스팅이미지",keyword)==0)
		{
			str								=	reader.getStream();
			skill.m_wAidSkillCastingImage	=	g_im.getEffectIndex(str);

			if (skill.m_wAidSkillCastingImage==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","보조마법캐스팅이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}
		if (STRICMP("보조마법캐스팅이미지위치",keyword)==0)
		{
			str		=	reader.getWord();

			iValue	=	CDecoder::AnalCode(l_aHitEffectOuptuPart,str);

			if (iValue		==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","보조마법캐스팅이미지위치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wAidSkillCastingImageOutputPart	=	iValue;
			continue;
		}
		if (STRICMP("폭발이미지",keyword)==0)
		{
			str						=	reader.getStream();
			skill.m_wExplosionImage	=	g_im.getEffectIndex(str);

			if (skill.m_wExplosionImage	==	0xffff)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","폭발이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			continue;
		}

		if( STRICMP("필요상태",keyword) == 0 )
		{
			
			str = reader.getWord();
			
			for(int i=0;; ++i)
			{

				if(STRICMP(l_aNeedState[i].m_strKeyword, "end") == 0)
				{
					EXIT("error in CSkillManager::decodeScripter(char *fn)","필요상태가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
					break;
				}

				if(STRICMP(l_aNeedState[i].m_strKeyword, str) == 0)
				{
					skill.m_bf6NeedState = l_aNeedState[i].m_dwCode;		
					break;
				}
			}
			continue;
		}
		if	(STRICMP("플레이어부가효과확률적용",keyword)==0)
		{
			skill.m_bf1IsApplyEffectChanceToPlayer	=	TRUE;
			continue;
		}
		if	(STRICMP("플레이어부가효과확률",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","플레이어부가효과확률이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wApplyEffectChanceToPlayer	=	iValue;
			continue;
		}
		if	(STRICMP("플레이어부가효과확률per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","플레이어부가효과확률per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wApplyEffectChancePerLevelToPlayer	=	iValue;
			continue;
		}
		if	(STRICMP("플레이어부가효과확률제한",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","플레이어부가효과확률제한이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wApplyEffectChanceLimitToPlayer	=	iValue;
			continue;
		}
		if(STRICMP("필요상태해제", keyword) == 0)
		{
			skill.m_bf1IsCancelNeedState = TRUE;
			continue;
		}

		if (STRICMP("필요스킬",keyword)==0)
		{
			str		=	reader.getStream();
			iValue	=	CDecoder::AnalCode(l_aJobNameKeyword,str);

			if	(iValue==dCODE_UNDEFINE)	
				iValue	=	l_iEnableJob;
			else
				str		=	reader.getStream();

			reader.getNumber();
			continue;
		}

		if (STRICMP("스킬인덱스",keyword)==0)
		{
			int	iIndex	=	reader.getNumber();

			if (iIndex	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","스킬인덱스가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wSerial	=	iIndex;
			continue;
		}

		if (STRICMP("아이콘인덱스",keyword)==0)
		{
			int	iIndex	=	reader.getNumber();

			if (iIndex	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","아이콘인덱스가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wIconIndex	=	iIndex;
			continue;
		}

		if (STRICMP("마법실드액션주기",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","마법실드액션주기가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[0]	=	iValue;
			continue;
		}
		if (STRICMP("미사일도착후유지시간frame",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","미사일도착후유지시간frame이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[0]	=	iValue;
			continue;
		}

		if (STRICMP("마법실드액션주기per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","마법실드액션주기per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[1]	=	iValue;
			continue;
		}
		if (STRICMP("마법실드액션주기반올림계산",keyword)==0)
		{
			skill.m_aValue[2]	=	1;
			continue;
		}

		if (STRICMP("타격주기",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","타격주기가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wStrikePeriod	=	iValue;
			continue;
		}
		if (STRICMP("타격주기per레벨",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","타격주기per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sStrikePeriodPerLevel	=	iValue;
			continue;
		}
		if (STRICMP("최소타격주기",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","최소타격주기가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wMinimumStrikePeriod	=	iValue;
			continue;
		}

		if (STRICMP("탄착점간격",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","탄착점간격이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[0]	=	iValue;
			continue;
		}

		if (STRICMP("최소타격범위",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","최소타격범위가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wMinimumHitRange	=	iValue;
			continue;
		}
		if (STRICMP("타격범위",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","타격범위가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wHitRange	=	iValue;
			continue;
		}
		if (STRICMP("타격범위per레벨",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","타격범위per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wHitRangePerLevel	=	iValue;
			continue;
		}
		if (STRICMP("CP획득타이밍",keyword)==0)
		{
			str			=	reader.getWord();
			iValue		=	CDecoder::AnalCode(l_aGetCPTiming,str);
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","CP획득타이밍이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wGetCPTiming	=	iValue;
			continue;
		}
		if (STRICMP("기술난이도",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","기술난이도가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wDifficultyLevel	=	iValue;
			continue;
		}

		if (STRICMP("속성",keyword)==0)
		{
			while(1)
			{
				str		=	reader.getWord();
				iValue	=	CDecoder::AnalCode(l_aAttribute,str);
				if (iValue	==	dCODE_VALID_NUMBER)
					EXIT("error in CSkillManager::decodeScripter(char *fn)","속성이 이상합니다.\n\n%s-%s",fn,skill.m_strName);
				skill.m_wDamageAttribute	+=	iValue;

				iCh	=	reader.getNextChar();
				if (iCh	==	';'	)	break;
				if (iCh	==	0	)	EXIT("error in CSkillManager::decodeScripter(char *fn)","속성이 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			}
			continue;
		}

		if (STRICMP("대상타입제한",keyword)==0)
		{
			skill.m_wTargetTypeLimit	=	0;

			while(1)
			{
				str		=	reader.getWord();
				iValue	=	CDecoder::AnalCode(l_aMonsterType,str);
				if (iValue	==	dCODE_VALID_NUMBER)
					EXIT("error in CSkillManager::decodeScripter(char *fn)","대상타입제한이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

				skill.m_wTargetTypeLimit	+=	1<<iValue;

				iCh	=	reader.getNextChar();
				if (iCh	==	';'	)	break;
				if (iCh	==	0	)	EXIT("error in CSkillManager::decodeScripter(char *fn)","대상타입제한이 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			}
			continue;
		}

		if (STRICMP("마법대미지based남은체력",keyword)==0)
		{
			skill.m_bf1IsMagicDamageBasedRemainHP	=	TRUE;
			continue;
		}
		
		if (STRICMP("인스턴스워터폴",keyword)==0)
		{
			skill.m_bf1InsInstanceWaterFall	=	TRUE;
			continue;
		}
		
		if	(STRICMP("출력효과",keyword)==0)
		{
			str			=	reader.getWord();

			if	(STRICMP(str,"연기")	==	0)
				skill.m_wOutputEffect	=	dPUT_SOFT;
			else
			if	(STRICMP(str,"중독")	==	0)
				skill.m_wOutputEffect	=	dPUT_DIFFERENT;
			else
			if	(STRICMP(str,"그을림")	==	0)
				skill.m_wOutputEffect	=	dPUT_DODGE_BURN;
			else
			if	(STRICMP(str,"불꽃")	==	0)
				skill.m_wOutputEffect	=	dPUT_SOFT_DODGE;
			else
				EXIT("error in CSkillManager::decodeScripter(char *fn)","출력효과가 이상합니다.\n\n%s-%s : %s",fn,skill.m_strName,str);

			continue;
		}

		if (STRICMP("팔레트인덱스",keyword)==0)
		{
			str			=	reader.getWord();

			if	(STRICMP(str,"회색")	==	0)
				skill.m_wPaletteIndex	=	0;
			else
			if	(STRICMP(str,"빨강")	==	0)
				skill.m_wPaletteIndex	=	1;
			else
			if	(STRICMP(str,"주황")	==	0)
				skill.m_wPaletteIndex	=	2;
			else
			if	(STRICMP(str,"노랑")	==	0)
				skill.m_wPaletteIndex	=	3;
			else
			if	(STRICMP(str,"연두")	==	0)
				skill.m_wPaletteIndex	=	4;
			else
			if	(STRICMP(str,"초록")	==	0)
				skill.m_wPaletteIndex	=	5;
			else
			if	(STRICMP(str,"하늘")	==	0)
				skill.m_wPaletteIndex	=	6;
			else
			if	(STRICMP(str,"밝은파랑")	==	0)
				skill.m_wPaletteIndex	=	7;
			else
			if	(STRICMP(str,"파랑")	==	0)
				skill.m_wPaletteIndex	=	8;
			else
			if	(STRICMP(str,"보라")	==	0)
				skill.m_wPaletteIndex	=	9;
			else
			if	(STRICMP(str,"분홍")	==	0)
				skill.m_wPaletteIndex	=	10;
			else
				EXIT("error in CSkillManager::decodeScripter(char *fn)","팔레트인덱스가 이상합니다.\n\n%s-%s : %s",fn,skill.m_strName,str);

			continue;
		}

		if (STRICMP("불대미지",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","불대미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sFireDamage	=	iValue;
			continue;
		}
		if (STRICMP("불대미지per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","불대미지per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sFireDamagePerLevel	=	iValue;
			continue;
		}
		if (STRICMP("불대미지범위",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","불대미지범위이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sFireDamageRange			=	iValue;
			continue;
		}
		if (STRICMP("불대미지범위per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","불대미지범위per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sFireDamageRangePerLevel	=	iValue;
			continue;
		}
		if (STRICMP("불대미지퍼센티지",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","불대미지퍼센티지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sFireDamagePercentage	=	iValue;
			continue;
		}
		if (STRICMP("불대미지퍼센티지한계",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","불대미지퍼센티지한계가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sFireDamagePercentageLimit	=	iValue;
			continue;
		}
		if (STRICMP("불대미지퍼센티지per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","불대미지퍼센티지per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sFireDamagePercentagePerLevel	=	iValue;
			continue;
		}
		if (STRICMP("물대미지",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","물대미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sWaterDamage	=	iValue;
			continue;
		}
		if (STRICMP("물대미지per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","물대미지per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sWaterDamagePerLevel	=	iValue;
			continue;
		}
		if (STRICMP("물대미지범위",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","물대미지범위가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sWaterDamageRange	=	iValue;
			continue;
		}
		if (STRICMP("물대미지범위per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","물대미지범위per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sWaterDamageRangePerLevel=	iValue;
			continue;
		}
		if (STRICMP("물대미지퍼센티지",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","물대미지퍼센티지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sWaterDamagePercentage=	iValue;
			continue;
		}
		if (STRICMP("물대미지퍼센티지한계",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","물대미지퍼센티지한계가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sWaterDamagePercentageLimit	=	iValue;
			continue;
		}
		if (STRICMP("물대미지퍼센티지per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","물대미지퍼센티지per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sWaterDamagePercentagePerLevel=	iValue;
			continue;
		}
		if (STRICMP("바람대미지",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","바람대미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sWindDamage=	iValue;
			continue;
		}
		if (STRICMP("바람대미지per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","바람대미지per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sWindDamagePerLevel=	iValue;
			continue;
		}
		if (STRICMP("바람대미지범위",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","바람대미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sWindDamageRange=	iValue;
			continue;
		}
		if (STRICMP("바람대미지범위per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","바람대미지범위per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sWindDamageRangePerLevel=	iValue;
			continue;
		}
		if (STRICMP("바람대미지퍼센티지",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","바람대미지퍼센티지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sWindDamagePercentage=	iValue;
			continue;
		}
		if (STRICMP("바람대미지퍼센티지한계",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","바람대미지퍼센티지한계가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sWindDamagePercentageLimit	=	iValue;
			continue;
		}
		if (STRICMP("바람대미지퍼센티지per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","바람대미지퍼센티지per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sWindDamagePercentagePerLevel=	iValue;
			continue;
		}
		if (STRICMP("대지대미지",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","대지대미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sEarthDamage=	iValue;
			continue;
		}
		if (STRICMP("대지대미지per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","대지대미지per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sEarthDamagePerLevel=	iValue;
			continue;
		}
		if (STRICMP("대지대미지범위",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","대지대미지범위가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sEarthDamageRange	=	iValue;
			continue;
		}
		if (STRICMP("대지대미지범위per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","대지대미지범위per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sEarthDamageRangePerLevel=	iValue;
			continue;
		}
		if (STRICMP("대지대미지퍼센티지",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","대지대미지퍼센티지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sEarthDamagePercentage=	iValue;
			continue;
		}
		if (STRICMP("대지대미지퍼센티지한계",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","대지대미지퍼센티지한계가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sEarthDamagePercentageLimit	=	iValue;
			continue;
		}
		if (STRICMP("대지대미지퍼센티지per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","대지대미지퍼센티지per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sEarthDamagePercentagePerLevel=	iValue;
			continue;
		}
		if (STRICMP("빛대미지",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","빛대미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sLightDamage=	iValue;
			continue;
		}
		if (STRICMP("빛대미지per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","빛대미지per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sLightDamagePerLevel=	iValue;
			continue;
		}
		if (STRICMP("빛대미지범위",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","빛대미지범위가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sLightDamageRange	=	iValue;
			continue;
		}
		if (STRICMP("빛대미지범위per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","빛대미지범위per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sLightDamageRangePerLevel=	iValue;
			continue;
		}
		if (STRICMP("빛대미지퍼센티지",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","빛대미지퍼센티지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sLightDamagePercentage=	iValue;
			continue;
		}
		if (STRICMP("빛대미지퍼센티지한계",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","빛대미지퍼센티지한계가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sLightDamagePercentageLimit	=	iValue;
			continue;
		}
		if (STRICMP("빛대미지퍼센티지per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","빛대미지퍼센티지per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sLightDamagePercentagePerLevel=	iValue;
			continue;
		}
		if (STRICMP("어둠대미지",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","어둠대미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDarkDamage=	iValue;
			continue;
		}
		if (STRICMP("어둠대미지per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","어둠대미지per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDarkDamagePerLevel=	iValue;
			continue;
		}
		if (STRICMP("어둠대미지범위",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","어둠대미지범위가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDarkDamageRange	=	iValue;
			continue;
		}
		if (STRICMP("어둠대미지범위per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","어둠대미지범위per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDarkDamageRangePerLevel=	iValue;
			continue;
		}
		if (STRICMP("어둠대미지퍼센티지",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","어둠대미지퍼센티지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDarkDamagePercentage=	iValue;
			continue;
		}
		if (STRICMP("어둠대미지퍼센티지한계",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","어둠대미지퍼센티지한계가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDarkDamagePercentageLimit	=	iValue;
			continue;
		}
		if (STRICMP("어둠대미지퍼센티지per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","어둠대미지퍼센티지per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sDarkDamagePercentagePerLevel=	iValue;
			continue;
		}
		if (STRICMP("크리티컬to언데드",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","크리티컬to언데드가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sCriticalToUndead=	iValue;
			continue;
		}
		if (STRICMP("크리티컬to언데드per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","크리티컬to언데드가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sCriticalToUndeadPerLevel=	iValue;
			continue;
		}
		if (STRICMP("크리티컬to악마",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","크리티컬to악마가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sCriticalToDemon=	iValue;
			continue;
		}
		if (STRICMP("크리티컬to악마per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","크리티컬to악마가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sCriticalToDemonPerLevel=	iValue;
			continue;
		}
		if (STRICMP("크리티컬to동물",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","//크리티컬to동물이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sCriticalToAnimal=	iValue;
			continue;
		}
		if (STRICMP("크리티컬to동물per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","//크리티컬to동물이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sCriticalToAnimalPerLevel=	iValue;
			continue;
		}
		if (STRICMP("크리티컬to인간",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","크리티컬to인간이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sCriticalToHuman=	iValue;
			continue;
		}
		if (STRICMP("크리티컬to인간per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","크리티컬to인간이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sCriticalToHumanPerLevel=	iValue;
			continue;
		}
		if (STRICMP("크리티컬to신수",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","크리티컬to신수가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sCriticalToHolyAnimal=	iValue;
			continue;
		}
		if (STRICMP("즉사to언데드",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","즉사to언데드가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sInstanceKillToUndead=	iValue;
			continue;
		}
		if (STRICMP("즉사to언데드per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","즉사to언데드가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sInstanceKillToUndeadPerLevel=	iValue;
			continue;
		}
		if (STRICMP("즉사to악마",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","즉사to악마가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sInstanceKillToDemon=	iValue;
			continue;
		}
		if (STRICMP("즉사to악마per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","즉사to악마가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sInstanceKillToDemonPerLevel=	iValue;
			continue;
		}
		if (STRICMP("즉사to동물",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","//즉사to동물이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sInstanceKillToAnimal=	iValue;
			continue;
		}
		if (STRICMP("즉사to동물per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","//즉사to동물이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sInstanceKillToAnimalPerLevel=	iValue;
			continue;
		}
		if (STRICMP("즉사to인간",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","즉사to인간이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sInstanceKillToHuman=	iValue;
			continue;
		}
		if (STRICMP("즉사to인간per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","즉사to인간이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sInstanceKillToHumanPerLevel=	iValue;
			continue;
		}
		if (STRICMP("즉사to신수",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","즉사to신수가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sInstanceKillToHolyAnimal=	iValue;
			continue;
		}
		if (STRICMP("즉사to신수per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","즉사to신수가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sInstanceKillToHolyAnimalPerLevel=	iValue;
			continue;
		}

		if (STRICMP("공격력보정",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","공격력보정이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sCorrectAttackPoint=	iValue;
			continue;
		}

		if (STRICMP("타격주기",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","타격주기가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_bf5HitDamagePeriod	=	iValue;
			continue;
		}

		if (STRICMP("동시공격탄수",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","동시공격탄수가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sShotCount	=	iValue;
			continue;
		}
		if (STRICMP("동시공격탄수per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","동시공격탄수per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sShotCountPerLevel=	iValue;
			continue;
		}
		if (STRICMP("동시공격탄수제한",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","동시공격탄수제한이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wLimitShotCount	=	iValue;
			continue;
		}
		
		if (STRICMP("동시공격탄수올림계산",keyword)==0)
		{
			skill.m_wShotCountDecimalMethod	=	eDECIMAL_METHOD_ROUND_OFF;
			continue;
		}
		if (STRICMP("동시공격탄수반올림계산",keyword)==0)
		{
			skill.m_wShotCountDecimalMethod	=	eDECIMAL_METHOD_UP;
			continue;
		}

		if (STRICMP("탄수",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","탄수가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sBulletCount	=	iValue;
			continue;
		}
		if (STRICMP("탄수per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","탄수per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sBulletCountPerLevel	=	iValue;
			continue;
		}
		if (STRICMP("탄수제한",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","탄수제한이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sBulletLimitCount	=	iValue;
			continue;
		}
		if (STRICMP("탄수버림계산",keyword)==0)
		{
			skill.m_wBulletDecimalMethod	=	eDECIMAL_METHOD_LEAVE;
			continue;
		}
		if (STRICMP("탄수반올림계산",keyword)==0)
		{
			skill.m_wBulletDecimalMethod	=	eDECIMAL_METHOD_ROUND_OFF;
			continue;
		}
		if (STRICMP("탄수올림계산",keyword)==0)
		{
			skill.m_wBulletDecimalMethod	=	eDECIMAL_METHOD_UP;
			continue;
		}
		if (STRICMP("연사횟수",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","연사횟수가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sContiniousShotCount=	iValue;
			continue;
		}
		if (STRICMP("연사횟수per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","연사횟수per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sContiniousShotCountPerLevel=	iValue;
			continue;
		}
		if (STRICMP("연사제한",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","연사제한이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sContiniousShotMaxCount		=	iValue;
			continue;
		}
		if (STRICMP("연사간격",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","연사간격이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sContiniousShotPeriod=	iValue;
			continue;
		}
		if (STRICMP("연사횟수올림계산",keyword)==0)
		{
			skill.m_wContiniousShotDecimalMethod	=	eDECIMAL_METHOD_UP;
			continue;
		}
		if (STRICMP("연사횟수반올림계산",keyword)==0)
		{
			skill.m_wContiniousShotDecimalMethod	=	eDECIMAL_METHOD_ROUND_OFF;
			continue;
		}

		if (STRICMP("연사횟수버림계산",keyword)==0)
		{
			skill.m_wContiniousShotDecimalMethod	=	eDECIMAL_METHOD_LEAVE;
			continue;
		}
		if (STRICMP("한계물리타격대미지",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","한계물리타격대미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wLimitPhysicalDamage		=	iValue;
			continue;
		}
		if (STRICMP("한계물리타격대미지per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","한계물리타격대미지per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sLimitPhysicalDamagePerLevel=	iValue;
			continue;
		}
		if (STRICMP("한계마법타격대미지",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","한계마법타격대미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wLimitMagicDamage			=	iValue;
			continue;
		}
		if (STRICMP("한계마법타격대미지per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","한계마법타격대미지per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sLimitMagicDamagePerLevel	=	iValue;
			continue;
		}
		if (STRICMP("녹백",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","녹백이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sNockbackDistance=	iValue;
			continue;
		}

		if (STRICMP("녹백per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","녹백per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sNockbackDistancePerLevel=	iValue;
			continue;
		}
		if (STRICMP("액션순차적증가",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","액션순차적증가가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wIncreaseActionAnmCount	=	iValue;
			continue;
		}
		if (STRICMP("테스트중",keyword)==0)
		{
			skill.m_bf1IsTestSkill			=	TRUE;
			continue;
		}

		if (STRICMP("오라발동으로CP증가안함",keyword)==0)
		{
			skill.m_bf1IsNotIncreaseByActiveAura=	TRUE;
			continue;
		}
		if (STRICMP("스킬사용후CP획득하는가스스킬",keyword)==0)
		{
			skill.m_bf1IsGetCPGasSkillByCasting	=	TRUE;
			continue;
		}

		if (STRICMP("시전이미지액션릴리즈위치에출력",keyword)==0)
		{
			skill.m_bf1IsCastImageOnReleasPos	=	TRUE;
			continue;
		}
		if (STRICMP("잡아던지기타입공격력계산",keyword)==0)
		{
			skill.m_bf1IsCatchAndThrowingTypeAttackPower	=	TRUE;
			continue;
		}

		if (STRICMP("입은대미지로어둠공격력계산",keyword)==0)
		{
			skill.m_bf1DarkDamageAttackByDamagePoint	=	TRUE;
			continue;
		}
		if (STRICMP("입은대미지로공격력계산",keyword)==0)
		{
			skill.m_bf1AttackByDamagedDamage			=	TRUE;
			continue;
		}
		if	(STRICMP("효과발동시시전효과음연주",keyword)==0)
		{
			skill.m_bf1PlayCastSoundByEnchantAidSkill	=	TRUE;
			continue;
		}
		
		if (STRICMP("레이저미사일",keyword)==0)
		{
			skill.m_bf1IsLaser	=	TRUE;
			continue;
		}
		if (STRICMP("이미지크기타격범위에맞게조절",keyword)==0)
		{
			skill.m_bf1IsRefitImageSizeByHitRange	=	TRUE;
			continue;
		}
		if (STRICMP("폭발이미지바닥에출력",keyword)==0)
		{
			skill.m_bf1IsRapeExplosionImage=	TRUE;
			continue;
		}

		if (STRICMP("행동금지",keyword)==0)
		{
			skill.m_bf1IsPohibitAction		=	TRUE;
			continue;
		}
		if (STRICMP("사정거리0",keyword)==0)
		{
			skill.m_bf1ShootRangeZeroSkill	=	TRUE;
			continue;
		}
		if (STRICMP("둘러싸서분신공격",keyword)==0)
		{
			skill.m_bf1IsRoundRappedBunshinAttack	=	TRUE;
			continue;
		}
		if (STRICMP("무기시머링실드",keyword)==0)
		{
			skill.m_bf1IsShimmeringShieldByWeapon	=	TRUE;
			continue;
		}
		if (STRICMP("반격안함",keyword)==0)
		{
			skill.m_bf1NotRevenge	=	TRUE;
			continue;
		}
		if (STRICMP("분신농도",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","분신농도가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_bf5BunshineAlphaDepth	=	iValue;
			continue;
		}
		
		if (STRICMP("HP싱크스킬",keyword)==0)
		{
			skill.m_bf1IsHpSyncSkill			=	TRUE;
			continue;
		}
		if (STRICMP("스킬효과스캐일링",keyword)==0)
		{
			skill.m_bf1IsZoomInEffectSkill	=	TRUE;
			continue;
		}
		if (STRICMP("타겟의방패블럭률무시",keyword)==0)
		{
			skill.m_bf1IsIgnoreTargetBlockingChance	=	TRUE;
			continue;
		}
		if (STRICMP("타겟에게만물리대미지적용",keyword)==0)
		{
			skill.m_bf1IsApplyPhysicalDamageOnlyTarget=	TRUE;
			continue;
		}
		if (STRICMP("병렬분신",keyword)==0)
		{
			skill.m_bf1IsParallelAfterImage	=	TRUE;
			continue;
		}
		if (STRICMP("관통러쉬",keyword)==0)
		{
			skill.m_bf1IsPenetrationRush		=	TRUE;
			continue;
		}

		if (STRICMP("루프러쉬에니",keyword)==0)
		{
			skill.m_bf1IsLoopRushAni	=	TRUE;
			continue;
		}

		if (STRICMP("훨러닝스타일러쉬",keyword)==0)
		{
			skill.m_bf1IsWhirlRunningStyleRush	=	TRUE;
			continue;
		}

		if (STRICMP("회전안하는토네이도",keyword)==0)
		{
			skill.m_bf1IsNotWhirlTornado	=	TRUE;
			continue;
		}
		if (STRICMP("트리거다음프레임을러쉬이미지로설정",keyword)==0)
		{
			skill.m_bf1SetRushImageByTriggerNextFrame	=	TRUE;
			continue;
		}
		if (STRICMP("러쉬잔상각도",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","러쉬잔상각도가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_bf9IsParallellRushAngle		=	iValue;
			continue;
		}	
		if (STRICMP("러쉬잔상간격",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","러쉬잔상간격이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_bf8ParallellRushGab			=	iValue;
			continue;
		}	

		if (STRICMP("타겟에게만타격을주는러쉬",keyword)==0)
		{
			skill.m_bf1IsAttackOnlyTargetRush	=	TRUE;
			continue;
		}		
		if (STRICMP("시전자만인챈트",keyword)==0)
		{
			skill.m_bf1IsOnlySelfEnchantSkill	=	TRUE;
			continue;
		}

		if (STRICMP("무기대미지무시",keyword)==0)
		{
			skill.m_bf1IsIgnoreWeaponDamage		=	TRUE;
			continue;
		}
		if (STRICMP("온오프스킬",keyword)==0)
		{
			skill.m_bf1IsOnOffSkill	=	TRUE;
			continue;
		}
		if (STRICMP("노려보기스킬",keyword)==0)
		{
			skill.m_bf1IsGlareSkill	=	TRUE;
			continue;
		}
		
		if (STRICMP("마법대미지based마지막으로입히고입은대미지",keyword)==0)
		{
			skill.m_wIsMagicDamageBasedLastHitDamage	=	TRUE;
			continue;
		}
		if	(STRICMP("CP소모싱크안함",keyword)==0)
		{
			skill.m_wIsNotSyncCP	=	TRUE;
			continue;
		}
		if	(STRICMP("방패모양동기화",keyword)==0)
		{
			skill.m_bf1IsSyncShieldShape	=	TRUE;
			continue;
		}
		if	(STRICMP("타겟회피보정무시",keyword)==0)
		{
			skill.m_bf1IsIgnoreTargetDodgeCorrectValue	=	TRUE;
			continue;
		}

		if(STRICMP("돌진공격",keyword)==0)
		{
			skill.m_bf1IsRushAttack = TRUE;
			continue;
		}
		if(STRICMP("돌진거리",keyword)==0)
		{
			iValue	=	reader.getNumber();
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","부가효과적용횟수가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			skill.m_aValue[0] = iValue;
			continue;
		}

		if	(STRICMP("부가효과클라이언트와싱크",keyword)==0)
		{
			skill.m_bf1IsSyncSkillExtraEffectWithPlayer	=	TRUE;
			continue;
		}
		if	(STRICMP("부가효과적용횟수",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","부가효과적용횟수가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wApplyExtraEffectCount	=	iValue;
			continue;
		}
		if	(STRICMP("부가효과적용대상",keyword)==0)
		{
			str	=	reader.getWord();

			if	(STRICMP(str,"아군")==0)			wExtraEffectAdaptiveTarget	=	eEEAT_OWN_FORCE;
			if	(STRICMP(str,"적")==0)				wExtraEffectAdaptiveTarget	=	eEEAT_ENEMY;
			if	(STRICMP(str,"본인")==0)			wExtraEffectAdaptiveTarget	=	eEEAT_SELF;
			if	(STRICMP(str,"파티원")==0)			wExtraEffectAdaptiveTarget	=	eEEAT_PARTY_MEMBER;
			if	(STRICMP(str,"펫")==0)				wExtraEffectAdaptiveTarget	=	eEEAT_PET;
			if	(STRICMP(str,"펫과소환수")==0)		wExtraEffectAdaptiveTarget	=	eEEAT_PET_AND_BEAST;
			if	(STRICMP(str,"적펫")==0)			wExtraEffectAdaptiveTarget	=	eEEAT_ENEMY_PET;
			if	(STRICMP(str,"소환수")==0)			wExtraEffectAdaptiveTarget	=	eEEAT_SUMMON_BEAST;
			if	(STRICMP(str,"적과본인")==0)		wExtraEffectAdaptiveTarget	=	eEEAT_SELF_AND_ENEMY;
			if	(STRICMP(str,"유저파티원")==0)		wExtraEffectAdaptiveTarget	=	eEEAT_USER_PARTY_MEMBER;
			if	(STRICMP(str,"본인을제외한모두")==0)wExtraEffectAdaptiveTarget	=	eEEAT_ALL_EXCEPT_SELF;
			if	(STRICMP(str,"적몬스터")==0)		wExtraEffectAdaptiveTarget	=	eEEAT_ENEMY_MONSTER;
			if	(STRICMP(str,"스킬발동지역")==0)	wExtraEffectAdaptiveTarget	=	eEEAT_ACTIVE_SKILL_AREA;

			continue;
		}
		if (STRICMP("부가효과",keyword)==0)
		{
			int		iEE;

			CSkillExtraEffect	*lpEE	=	&skill.m_aExtraEffect[wExtraEffectCount];

			str				=	reader.getWord();
			iValue			=	CDecoder::AnalCode(l_aSkillExtraEffect,str);

			if	(iValue		==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","부가효과가 이상합니다.\n\n%s-%s[%s]",fn,skill.m_strName,str);

			lpEE->m_wEffect	=	iValue;

			reader.findOpenCode();	//	오픈코드

			while(1)
			{
				str		=	reader.getWord();
				iEE		=	CDecoder::AnalCode(l_aSkillExtraEffectElement,str);

				if	(iEE	==	dCODE_FINISH)
					continue;
				if	(iEE	==	dCODE_CLOSE)
					break;

				if	(stricmp(str,"방어구착용방해")	==	0)
				{
					int	iSlot				=	reader.getNumber();
					lpEE->m_aiValue[iSlot]	=	0;

					CSkillExtraEffect::cInterruptingArmor	*lpInfo	=	(CSkillExtraEffect::cInterruptingArmor	*)&lpEE->m_aiValue[iSlot];

					lpInfo->m_bf7BeginLevel	=	reader.getNumber();
					lpInfo->m_bf7EndLevel	=	reader.getNumber();

					while(1)
					{
						str					=	reader.getWord();

						if	(STRICMP(str,"귀걸이")	==	0)
							lpInfo->m_bf1InterruptingPartEaring		=	1;
						else
						if	(STRICMP(str,"목걸이")	==	0)
							lpInfo->m_bf1InterruptingPartNecklace	=	1;
						else
						if	(STRICMP(str,"부츠")	==	0)
							lpInfo->m_bf1InterruptingPartBoots		=	1;
						else
						if	(STRICMP(str,"헬멧")	==	0)
							lpInfo->m_bf1InterruptingPartHelm		=	1;
						else
						if	(STRICMP(str,"장갑")	==	0)
							lpInfo->m_bf1InterruptingPartGauntlet	=	1;
						else
						if	(STRICMP(str,"갑옷")	==	0)
							lpInfo->m_bf1InterruptingPartArmor		=	1;
						else
						if	(STRICMP(str,"반지")	==	0)
							lpInfo->m_bf1InterruptingPartRing		=	1;
						else
						if	(STRICMP(str,"방패")	==	0)
							lpInfo->m_bf1InterruptingPartShield		=	1;
						else
							EXIT("error in CSkillManager::decodeScripter(char *fn)","방어구착용방해 대상이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

						iCh	=	reader.getNextChar();

						if	(iCh	==	';'	)
							break;
					}

					continue;
				}

				if	(stricmp(str,"투기효과")	==	0)
				{	
					str					=	reader.getWord();
					
					if	(STRICMP(str,"절대공격력수치증가")	==	0)
						lpEE->m_aiValue[9]		=	eSKILL_FEE_ADD_ABSOLUTE_ATTACK_POWER;
					if	(STRICMP(str,"공격횟수증가")	==	0)
						lpEE->m_aiValue[9]		=	eSKILL_FEE_INCREASE_ATTACK_COUNT;
					if	(STRICMP(str,"획득CP증가")	==	0)
						lpEE->m_aiValue[9]		=	eSKILL_FEE_INCREASE_GET_CP;
					if	(STRICMP(str,"절대크리티컬퍼센트증가")	==	0)
						lpEE->m_aiValue[9]		=	eSKILL_FEE_INCREASE_ABSOLUTE_CRITICAL_PERCENT;
					if	(STRICMP(str,"쿨타임감소")	==	0)
						lpEE->m_aiValue[9]		=	eSKILL_FEE_DECREASE_COOL_TIME;
					continue;
				}

				if (STRICMP("부가효과이미지",keyword)==0)
				{
					str						=	reader.getStream();
					lpEE->m_wExtraEffectImage 	=	g_im.getEffectIndex(str);
					
					if (lpEE->m_wExtraEffectImage	==	0xffff)
						EXIT("error in CSkillManager::decodeScripter(char *fn)","부가효과 이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
					continue;
				}
				if (STRICMP("부가효과이미지위치",keyword)==0)
				{
					str		=	reader.getWord();
					
					iValue	=	CDecoder::AnalCode(l_aHitEffectOuptuPart,str);
					
					if (iValue		==	dCODE_UNDEFINE)
						EXIT("error in CSkillManager::decodeScripter(char *fn)","스킬이미지위치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
					lpEE->m_wExtraEffectOutputPart =	iValue;
					
					continue;
				}					
				if	(stricmp(str,"환영직업")	==	0)
				{
					str					=	reader.getStream();
					lpEE->m_aiValue[4]	=	CDecoder::AnalCode(l_aJobNameKeyword,str);
					
					continue;
				}
				if	(stricmp(str,"지속시간반올림으로계산")	==	0)
				{
					lpEE->m_bf1IsCalcUpkeepTimeRoundOff	=	TRUE;

					continue;
				}
				if	(stricmp(str,"지속시간한계")	==	0)
				{
					iValue						=	reader.getNumber();

					if	(iValue	==	dCODE_VALID_NUMBER)
						EXIT("error in CSkillManager::decodeScripter(char *fn)","지속시간한계 수치가 이상합니다.\n\n%s-%s\n\n%s",fn,skill.m_strName,str);

					lpEE->m_wLimitUpkeepTime	=	iValue;
					continue;
				}

				if	(iEE	==	dCODE_UNDEFINE)
					EXIT("error in CSkillManager::decodeScripter(char *fn)","부가효과의 세부항목이 이상합니다.\n\n%s-%s\n\n%s",fn,skill.m_strName,str);
				if (iEE	==	eSKILL_EE_STOP)	
				{
					lpEE->m_bf1IsStop	=	TRUE;
					continue;//지속시간per레벨
				}
				if (iEE	==	eSKILL_EE_UPKEEP_TIME_BY_FRAME)	
				{
					lpEE->m_bf1IsUpkeepTimeByFrame	=	TRUE;
					continue;//지속시간per레벨
				}
				if (iEE	==	eSKILL_EE_ACTIVE_CAHCNE_INVERSE_PROPOTION_BY_REMAIN_HP)	
				{
					lpEE->m_bf1IsActiveChaceInversePropotionByRemainHP	=	TRUE;
					continue;//지속시간per레벨
				}
				if (iEE	==	eSKILL_EE_ACTIVE_CAHCNE_INVERSE_PROPOTION_BY_REMAIN_HP_X_MONSTER_GRADE_1)	
				{
					lpEE->m_bf1IsActiveChaceInversePropotionByRemainHPxMonsterGrade1	=	TRUE;
					continue;//지속시간per레벨
				}
				if (iEE	==	eSKILL_EE_ADAPTIVE_TARGET)	
				{
					str	=	reader.getWord();

					if	(STRICMP(str,"아군")==0)			lpEE->m_bf4TargetMethod	=	eEEAT_OWN_FORCE;
					if	(STRICMP(str,"적")==0)				lpEE->m_bf4TargetMethod	=	eEEAT_ENEMY;
					if	(STRICMP(str,"본인")==0)			lpEE->m_bf4TargetMethod	=	eEEAT_SELF;
					if	(STRICMP(str,"파티원")==0)			lpEE->m_bf4TargetMethod	=	eEEAT_PARTY_MEMBER;
					if	(STRICMP(str,"펫")==0)				lpEE->m_bf4TargetMethod	=	eEEAT_PET;
					if	(STRICMP(str,"펫과소환수")==0)		lpEE->m_bf4TargetMethod	=	eEEAT_PET_AND_BEAST;
					if	(STRICMP(str,"적펫")==0)			lpEE->m_bf4TargetMethod	=	eEEAT_ENEMY_PET;
					if	(STRICMP(str,"소환수")==0)			lpEE->m_bf4TargetMethod	=	eEEAT_SUMMON_BEAST;
					if	(STRICMP(str,"적과본인")==0)		lpEE->m_bf4TargetMethod	=	eEEAT_SELF_AND_ENEMY;
					if	(STRICMP(str,"유저파티원")==0)		lpEE->m_bf4TargetMethod	=	eEEAT_USER_PARTY_MEMBER;
					if	(STRICMP(str,"본인을제외한모두")==0)lpEE->m_bf4TargetMethod	=	eEEAT_ALL_EXCEPT_SELF;
					if	(STRICMP(str,"적몬스터")==0)		lpEE->m_bf4TargetMethod	=	eEEAT_ENEMY_MONSTER;
					if	(STRICMP(str,"스킬발동지역")==0)	lpEE->m_bf4TargetMethod	=	eEEAT_ACTIVE_SKILL_AREA;

					continue;
				}

				if (iEE	==	eSKILL_EE_ADAPTIVE_TARGET_TYPE)	
				{
					lpEE->m_bf1TargetUndead	=	FALSE;
					lpEE->m_bf1TargetHuman	=	FALSE;
					lpEE->m_bf1TargetDemon	=	FALSE;
					lpEE->m_bf1TargetAnimal	=	FALSE;
					lpEE->m_bf1TargetHolyBeast=	FALSE;

					while(1)
					{
						str			=	reader.getWord();
						int	iDest	=	CDecoder::AnalCode(l_aMonsterType,str);

						if (iDest	==	dCODE_VALID_NUMBER)
							EXIT("error in CSkillManager::decodeScripter(char *fn)","사용대상이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

						switch(iDest)
						{
							case	eJT_UNDEAD:
								lpEE->m_bf1TargetUndead	=	TRUE;
								break;
							case	eJT_HUMAN:
								lpEE->m_bf1TargetHuman	=	TRUE;
								break;
							case	eJT_DEVIL:
								lpEE->m_bf1TargetDemon	=	TRUE;
								break;
							case	eJT_ANIMAL:
								lpEE->m_bf1TargetAnimal	=	TRUE;
								break;
							case	eJT_HOLY_BEAST:
								lpEE->m_bf1TargetHolyBeast=	TRUE;
								break;
						}

						iCh	=	reader.getNextChar();
						
						if	(iCh	==	';'	)
							break;
						if	(iCh	==	0	)
							EXIT("error in CSkillManager::decodeScripter(char *fn)","사용대상이 이상합니다.\n\n'%s-%s'",fn,skill.m_strName);
					}
								
					continue;
				}

				iValue	=	reader.getNumber();

				if	(iValue	==	dCODE_VALID_NUMBER)
					EXIT("error in CSkillManager::decodeScripter(char *fn)","부가효과의 세부항목의 수치가 이상합니다.\n\n%s-%s\n\n%s",fn,skill.m_strName,str);

				switch(iEE)
				{
					case	eSKILL_EE_ACTIVE_CAHCNE				:	lpEE->m_sActiveChance			=	iValue;break;//발동확률
					case	eSKILL_EE_ACTIVE_CAHCNE_PER_LEVEL	:	lpEE->m_sActiveChancePerLevel	=	iValue;break;//발동확률per레벨
					case	eSKILL_EE_LIMIT_ACTIVE_CHANCE		:	lpEE->m_wLimitActiveChance		=	iValue;break;//발동확률per레벨
					case	eSKILL_EE_UPKEEP_TIME				:	lpEE->m_sUpkeepTime				=	iValue;break;//지속시간
					case	eSKILL_EE_UPKEEP_TIME_PER_LEVEL		:	lpEE->m_sUpkeepTimePerLevel		=	iValue;break;//지속시간per레벨
					case	eSKILL_EE_VALUE1					:	
						lpEE->m_aiValue[0]				=	iValue;
						break;//
					case	eSKILL_EE_VALUE2					:	
						lpEE->m_aiValue[1]				=	iValue;
						break;//
					case	eSKILL_EE_VALUE3					:
						lpEE->m_aiValue[2]				=	iValue;
						break;//
					case	eSKILL_EE_VALUE4					:	
						lpEE->m_aiValue[3]				=	iValue;
						break;//
					case	eSKILL_EE_VALUE5					:	
						lpEE->m_aiValue[4]				=	iValue;
						break;//
					case	eSKILL_EE_VALUE6					:	
						lpEE->m_aiValue[5]				=	iValue;
						break;//
					case	eSKILL_EE_VALUE7					:	
						lpEE->m_aiValue[6]				=	iValue;
						break;//
					case	eSKILL_EE_VALUE8					:	
						lpEE->m_aiValue[7]				=	iValue;
						break;//
					case	eSKILL_EE_VALUE9					:	
						lpEE->m_aiValue[8]				=	iValue;
						break;//
					case	eSKILL_EE_VALUE10					:	
						lpEE->m_aiValue[9]				=	iValue;
						break;//
					case	eSKILL_EE_RANGE						:	
						lpEE->m_aiValue[0]				=	iValue;
						break;//
					case	eSKILL_EE_RANGE_PER_LEVEL			:	
						lpEE->m_aiValue[1]				=	iValue;
						break;//
				}
			}

			wExtraEffectCount++;
			continue;
		}

		if (STRICMP("녹아웃방어",keyword)==0)
		{
			skill.m_wSpecialEffect	|=	eSKILL_SE_DEFENSE_KNOCKOUT;
			continue;
		}
		if (STRICMP("적공격무시",keyword)==0)
		{
			skill.m_wSpecialEffect	|=	eSKILL_SE_NOT_INTERRUPT;
			continue;
		}
		if (STRICMP("스나이프샷",keyword)==0)
		{
			skill.m_wSpecialEffect	|=	eSKILL_SE_SNIPE_SHOT;
			continue;
		}
		if (STRICMP("스컬샷",keyword)==0)
		{
			skill.m_wSpecialEffect	|=	eSKILL_SE_SKULL_SHOT;
			continue;
		}
		if (STRICMP("무기던지기",keyword)==0)
		{
			skill.m_wSpecialEffect	|=	eSKILL_SE_THROW_WEAPON;
			continue;
		}
		if (STRICMP("다른직업무기던지기",keyword)==0)
		{
			skill.m_wSpecialEffect	|=	eSKILL_SE_THROW_OTHER_JOB_WEAPON;
			continue;
		}
		if (STRICMP("방패숨기기",keyword)==0)
		{
			skill.m_wSpecialEffect	|=	eSKILL_SE_HIDE_SHIELD;
			continue;
		}
		if (STRICMP("미사일무기제거",keyword)==0)
		{
			skill.m_wSpecialEffect	|=	eSKILL_SE_REMOVE_MISSLE;
			continue;
		}
		if (STRICMP("변신",keyword)==0)
		{
			skill.m_wSpecialEffect	|=	eSKILL_SE_CHANGE_JOB;
			continue;
		}
		if (STRICMP("변신이후에니메이션만표시",keyword)==0)
		{
			skill.m_bf1PutTrasAfterAnm	=	TRUE;
			continue;
		}

		if (STRICMP("펫능력치증가per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)	EXIT("error in CSkillManager::decodeScripter(char *fn)","펫능력치증가per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sIncreasePetPowerPerLevel	=	iValue;
			continue;
		}
		if (STRICMP("치명타",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)	EXIT("error in CSkillManager::decodeScripter(char *fn)","치명타가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sCriticalChance=	iValue;
			continue;
		}
		if (STRICMP("치명타per레벨",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","치명타per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sCriticalChancePerLevel	=	iValue;
			continue;
		}

		if (STRICMP("치명타고정",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)	EXIT("error in CSkillManager::decodeScripter(char *fn)","치명타고정이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sFixCriticalChance=	iValue;
			continue;
		}
			
		if (STRICMP("결정타",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)	EXIT("error in CSkillManager::decodeScripter(char *fn)","결정타가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sCrushChance=	iValue;
			continue;
		}
	
		if (STRICMP("결정타per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)	EXIT("error in CSkillManager::decodeScripter(char *fn)","결정타per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sCrushChancePerLevel=	iValue;
			continue;
		}
		if (STRICMP("결정타고정",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)	EXIT("error in CSkillManager::decodeScripter(char *fn)","결정타고정이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sFixCrushChance=	iValue;
			continue;
		}
		if (STRICMP("결정타한계",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","결정타한계가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wLimitCrushChance	=	iValue;
			continue;
		}
		if	(STRICMP("투지효과적용", keyword) == 0)
		{
			skill.m_bf1IsApplyFightingSpirit	=	TRUE;
			continue;
		}
		if	(STRICMP("동일버프적용불가적용", keyword) == 0)
		{	//	  이부분은 필드이동시 유지되는 일부 버프만 적용이된다.
			skill.m_bf1IsCanNotApplySameEffect	=	TRUE;
			continue;
		}

		if (STRICMP("세컨잡머신",keyword)==0)
		{
			skill.m_bf1IsSecondJobMachine	=	TRUE;
			continue;
		}
		if (STRICMP("마지막프레임루프",keyword)==0)
		{
			skill.m_bf1IsLoopLastFrame	=	TRUE;
			continue;
		}
		if (STRICMP("독점액션",keyword)==0)
		{
			skill.m_bf1IsExclusiveAction	=	TRUE;
			continue;
		}
		if (STRICMP("비독점액션",keyword)==0)
		{
			skill.m_bf1IsExclusiveAction	=	FALSE;
			continue;
		}
		if (STRICMP("회전반경",keyword)==0)
		{
			int	iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","회전반경이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wCircleRange	=	iValue;
			continue;
		}


		if (STRICMP("즉사적용방법",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)	EXIT("error in CSkillManager::decodeScripter(char *fn)","즉사적용방법이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wInstanceKillMethod	=	iValue;
			continue;
		}

		if (STRICMP("즉사확률",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)	EXIT("error in CSkillManager::decodeScripter(char *fn)","즉사확률이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sInstanceKillChance	=	iValue;
			continue;
		}

		if (STRICMP("즉사확률per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)	EXIT("error in CSkillManager::decodeScripter(char *fn)","즉사확률per레벨이 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_sInstanceKillChancePerLevel=	iValue;
			continue;
		}

		if (STRICMP("스킬그룹",keyword)==0)
		{
			char	*strName	=	reader.getStream();

			lpSkillGroup			=	&m_aSkillGroup[m_iSkillGroupCount];
			lpSkillGroup->m_wSerial	=	m_iSkillGroupCount;
				
			strcpy(lpSkillGroup->m_strName,strName);
			isGroupCode				=	TRUE;
			reader.findOpenCode();
			continue;
		}

		if (STRICMP("멤버스킬",keyword)==0)
		{
			char	*strSkill	=	reader.getStream();
			WORD	wSkill		=	getIndex(strSkill,l_iEnableJob);
			int		iCount		=	lpSkillGroup->m_wSkilCount;

			if (wSkill	==	0xffff)	
				EXIT("error in CSkillManager::decodeScripter(char *fn)","valid skill name in skill group '%s''s '%s'",lpSkillGroup->m_strName,strSkill);

			lpSkillGroup->m_aSkill[iCount]	=	wSkill;
			lpSkillGroup->m_wSkilCount++;
			continue;
		}

		if (STRICMP("include",keyword)==0)
		{
			str	=	reader.getStream();

			decodeScripter(str);
			continue;
		}

		if (STRICMP("correctSkill",keyword)==0)
		{
			if (m_bIsCorrectSkillForAnotherSkillBalance	==	FALSE)
				break;

			continue;
		}

		if (STRICMP("설명",keyword)==0)
		{
			str		=	reader.getStream();
			strcpy(skill.m_strComment,str);
			continue;
		}

		if (STRICMP("파워업형태",keyword)==0)
		{
			str	=	reader.getStream();
			strcpy(skill.m_strPowerup,str);
			continue;
		}
		if (STRICMP("리액션발동수제한",keyword)==0)
		{
			skill.m_bf1IsReactionLimitCount	= TRUE;
			continue;
		}
		if (STRICMP("운무시",keyword)==0)
		{
			skill.m_bf1IsIgnoreLucky	=	TRUE;
			continue;
		}
		if	(STRICMP("패시브효과", keyword) == 0)
		{
			str			= reader.getWord();
			iValue		= CDecoder::AnalCode(l_aSkillPassiveEffect,str);
			if	(iValue		==	dCODE_UNDEFINE)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","패시브효과가 이상합니다.\n\n%s-%s[%s]",fn,skill.m_strName,str);

			skill.m_aValue[0] = iValue;
			continue;
		}
		if (STRICMP("패시브효과수치",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)	EXIT("error in CSkillManager::decodeScripter(char *fn)","패시브효과수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[1]	=	iValue;
			continue;
		}
		if (STRICMP("패시브효과수치per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)	EXIT("error in CSkillManager::decodeScripter(char *fn)","패시브효과수치per레벨가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[2]	=	iValue;
			continue;
		}

		if	(STRICMP("패시브효과수치2", keyword) == 0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)	EXIT("error in CSkillManager::decodeScripter(char *fn)","패시브효과수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);;

			skill.m_aValue[4] = iValue;
			continue;
		}
		if (STRICMP("패시브효과수치per레벨2",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)	EXIT("error in CSkillManager::decodeScripter(char *fn)","패시브효과수치2가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[5]	=	iValue;
			continue;
		}
		if (STRICMP("패시브효과제한수치2",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)	EXIT("error in CSkillManager::decodeScripter(char *fn)","패시브효과수치per레벨2가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[6]	=	iValue;
			continue;
		}
		if (STRICMP("공격후딜레이무시",keyword)==0)
		{
			skill.m_bf1IsIgnoreDelayAfterAttack = TRUE;
			continue;
		}
		if	(STRICMP("모으기범위",keyword)==0)
		{
			iValue	=	reader.getNumber();
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","모으기범위 수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			skill.m_aValue[10]		=	iValue;
			continue;
		}
		if	(STRICMP("모으기범위per레벨",keyword)==0)
		{
			iValue	=	reader.getNumber();
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","모으기범위per레벨 수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			skill.m_aValue[11]		=	iValue;
			continue;
		}
		if	(STRICMP("모으기제한범위",keyword)==0)
		{
			iValue	=	reader.getNumber();
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","모으기제한범위 수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			skill.m_aValue[12]		=	iValue;
			continue;
		}

		if (STRICMP("패시브효과제한수치",keyword)==0)
		{
			iValue	=	reader.getNumber();

			if (iValue	==	dCODE_VALID_NUMBER)	EXIT("error in CSkillManager::decodeScripter(char *fn)","패시브효과제한수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_aValue[3]	=	iValue;
			continue;
		}
		if(STRICMP("분신공격형태",keyword)==0)
		{
			str	=	reader.getWord();

			if	(STRICMP(str,"러쉬")==0)				skill.m_aValue[0]	=	eBUNSHIN_RUSH_ATTACK;
			if	(STRICMP(str,"라운드")==0)				skill.m_aValue[0]	=	eBUNSHIN_ROUND_ATTACK;

			continue;
		}
		if	(STRICMP("매직미사일",keyword)==0)
		{
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","미사일공격지점범위랜덤 수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			skill.m_aValue[3]		=	TRUE;
			continue;
		}
		if	(STRICMP("매직미사일휘는최소크기",keyword)==0)
		{
			iValue	=	reader.getNumber();
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","미사일공격지점범위랜덤 수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			skill.m_aValue[4]		=	iValue;
			continue;
		}
		if	(STRICMP("매직미사일휘는최대크기",keyword)==0)
		{
			iValue	=	reader.getNumber();
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","미사일공격지점범위랜덤 수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			skill.m_aValue[5]		=	iValue;
			continue;
		}

		if	(STRICMP("가속도",keyword)==0)
		{
			iValue	=	reader.getNumber();
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","미사일공격지점범위랜덤 수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			skill.m_aValue[6]		=	iValue;
			continue;
		}
		if	(STRICMP("점프러쉬",keyword)==0)
		{
			
			skill.m_aValue[0]		=	TRUE;
			continue;
		}

		if	(STRICMP("점프러쉬높이증가값",keyword)==0)
		{
			iValue	=	reader.getNumber();
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","가속도 수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			skill.m_aValue[1]		=	iValue;
			continue;
		}
		if	(STRICMP("점프러쉬최종액션프레임",keyword)==0)
		{
			iValue	=	reader.getNumber();
			if (iValue	==	dCODE_VALID_NUMBER)
				EXIT("error in CSkillManager::decodeScripter(char *fn)","점프러쉬최종액션프레임 수치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);
			skill.m_aValue[2]		=	iValue;
			continue;
		}
		EXIT("Undefined KEY WORD","%s's '%s'",skill.m_strName,keyword);
		break;
	}

#endif

	return TRUE;
}

//
//	기본 기술 추가
BOOL
CSkillManager::addSkill(CSkill *_lpSkill)
{
#ifdef	_IS_DEV_CLIENT
	if	(m_iSkillCount >= dMAX_SKILL)
		return ERRMSG("Error in CSkillManager::add(CSkill *skill)","too many skill!!");

	if	(_lpSkill->m_wIconIndex	==	0xffff)
		_lpSkill->m_wIconIndex	=	_lpSkill->m_wSerial;

	if	(_lpSkill->m_wSerial		==	0xffff)
	{
		ERRMSG("에러!!","%s - 인덱스 없음",_lpSkill->m_strName);

		return	FALSE;
	}

	if	(g_aSkill[_lpSkill->m_wSerial].m_wSerial	==	0xffff)
		m_iSkillCount++;
	else
	if	(m_bIsCorrectSkillForAnotherSkillBalance == FALSE)
	{
		ERRMSG("에러!!","%s[%d] - 이미 등록되어 있는 스킬 인덱스 입니다.",_lpSkill->m_strName,_lpSkill->m_wSerial);

		return	FALSE;
	}

	if	(_lpSkill->m_aAi[0].m_wTrigger	!=	0xffff)
		if	(_lpSkill->getOrderingInSkillAi()	==	-1)
			return	ERRMSG("에러!!","%s[%d] - 스킬에 AI 우선순위 정보가 없습니다.",_lpSkill->m_strName,_lpSkill->m_wSerial);

	memcpy(&g_aSkill[_lpSkill->m_wSerial],_lpSkill,sizeof(CSkill));

#endif

	return TRUE;
}


//
//	이름으로 기술의 인덱스를 구함
int
CSkillManager::getIndex(char *skillName,int job,BOOL _bIsWantSummonBeastSkill)
{
	for (int i=0;i<dMAX_SKILL;i++)
	{
		if (g_aSkill[i].m_wSerial	==	0xffff)
			continue;

		if (_bIsWantSummonBeastSkill)
			if (g_aSkill[i].m_bf1IsSummonedBeastSkill	==	FALSE)
				continue;

		if (STRICMP(skillName,g_aSkill[i].m_strName)==0 )
			if (g_aSkill[i].m_wEnableJob == job || g_aSkill[i].m_wEnableJob == 0xffff || job == 0xffff)
				return i;
	}

	return 0xffff;
}

//
//	읽어들인 스킬 기본 데이터를 저장한다.
BOOL
CSkillManager::saveData(char *fn)
{
#ifdef	_IS_DEV_CLIENT

	cFILE	file;

	for (int i=0;i<dMAX_SKILL;i++)
	{
		CSkill	*lpSkill	=	&g_aSkill[i];

		if (lpSkill->m_wSerial	==	0xffff	||	lpSkill->m_wCommandSkill == 0xffff)
			continue;

		CSkill	*lpCommandSkill	=	&g_aSkill[g_aSkill[i].m_wCommandSkill];

		strcpy(lpCommandSkill->m_strComment,lpSkill->m_strComment);
		strcpy(lpCommandSkill->m_strPowerup,lpSkill->m_strPowerup);
		memcpy(lpCommandSkill->m_aRequireSkill,lpSkill->m_aRequireSkill,sizeof(lpSkill->m_aRequireSkill));
		lpCommandSkill->m_wDifficultyLevel	=	lpSkill->m_wDifficultyLevel	;
		lpCommandSkill->m_wSpentCP			=	lpSkill->m_wSpentCP			;
		lpCommandSkill->m_wSpentCPPerLevel	=	lpSkill->m_wSpentCPPerLevel	;
		lpCommandSkill->m_wGetCP			=	lpSkill->m_wGetCP			;
		lpCommandSkill->m_wGetCPPerLevel	=	lpSkill->m_wGetCPPerLevel	;
		lpCommandSkill->m_bf1IsTestSkill	=	lpSkill->m_bf1IsTestSkill	;
		lpCommandSkill->m_wIconIndex		=	lpSkill->m_wIconIndex		;
	}

	if (!file.Open(fn,"wb"))
		return	FALSE;

	UINT	uiHeaderSign	=	dSKILL_DATA_HEADER_SIGN;
	int		iVersion		=	eSKILL_DATA_VERSION_CURRENT;

	int		iSkillCount		=	m_iSkillCount;
	int		iNation			=	eNationKorea;

	cMessSign::SetDataEncodeTable(eNationKorea);
	
	cMessSign::EncodeDataBuffer2(&iSkillCount,4);

	file.Write(&uiHeaderSign,4);		//	헤더 사인
	file.Write(&iVersion,4);		//	버젼
	file.Write(&iSkillCount,4);		//	스킬의 수
	file.Write(&iNation,4);		//	스킬의 수

	{
		for (int i=0;i<dMAX_SKILL;i++)
		{
			CSkill	saveData;

			memcpy(&saveData,g_aSkill+i,sizeof(CSkill));
			cMessSign::EncodeDataBuffer2(&saveData,sizeof(CSkill));
			file.Write(&saveData,sizeof(CSkill));
		}
	}

	file.Write(&m_iSkillGroupCount,4);		//	스킬의 수
	file.Write(m_aSkillGroup,m_iSkillGroupCount*sizeof(cSKILL_GROUP));		//	스킬의 수
	file.Close();

	file.Close();
#endif	
	return	TRUE;
}

//
//	스킬 데이터를 데이터 화일에서 읽어 들인다.
BOOL
CSkillManager::loadData(char *fn)
{
	cFILE	file;
	int		iIndex	=	0;

	if	(stricmp(m_strLastLoadFileName,fn)	==	0)
		return	TRUE;

	strcpy(m_strLastLoadFileName,fn);

	char	*lpstrFileName	=	fn;

	if	(!file.Open(lpstrFileName,"rb"))
		return	FALSE;

	UINT	uiHeaderSign;
	int		iVersion		=	-1;

	file.Read(&uiHeaderSign,4);		//	스킬의 수

	if	(uiHeaderSign	==	dSKILL_DATA_HEADER_SIGN)
		file.Read(&iVersion,4);		//	스킬의 수
	else
		file.Seek(0,SEEK_SET);

	file.Read(&m_iSkillCount,4);		//	스킬의 수

	if	(iVersion	>=	eSKILL_DATA_VERSION_ENCRYPT2)
	{
		int		iNation;
		file.Read(&iNation,sizeof(iNation));

#ifdef	_FOR_JAPAN
		iNation	=	eNationJapan;
#endif

		cMessSign::SetDataEncodeTable(iNation);
	}
	else
		cMessSign::SetDataEncodeTable(-1);
	
	file.Read(g_aSkill,dMAX_SKILL*sizeof(CSkill));		//	스킬의 수
	file.Read(&m_iSkillGroupCount,4);		//	스킬의 수
	file.Read(m_aSkillGroup,m_iSkillGroupCount*sizeof(cSKILL_GROUP));		//	스킬의 수
	file.Close();

	cMessSign::DecodeDataBuffer2(&m_iSkillCount,4);
	

	{
		for (int i=0;i<dMAX_SKILL;i++)
			cMessSign::DecodeDataBuffer2(g_aSkill+i,sizeof(CSkill));
	}
	
	m_iMiniPetCombiSkillCount	=	0;

	{
		for (int i=0;i<dMAX_SKILL;i++)
		{
			if	(g_aSkill[i].m_wSerial	==	0xffff)
				continue;

			if	(g_aSkill[i].m_wType==	eSKILL_TYPE_MINIPET_COMBI)
				m_aiMiniPetCombiSkill[m_iMiniPetCombiSkillCount++]	=	i;
		}
	}

	while(1)
	{
		LONGLONG	dwMask	=	g_aEnchantedImageInfo[iIndex].m_dwMask;

		if	(dwMask		==	0xffffffff)
			break;

		for (int i=0;i<dMAX_SKILL;i++)
		{
			if	(g_aSkill[i].m_wSerial	==	0xffff)
				continue;

			if	(g_aSkill[i].m_dwEnchantedEffectMask	&	dwMask)
			{
				g_aEnchantedImageInfo[iIndex].m_wImage	=	g_aSkill[i].m_wEnchantedImage;

				break;
			}
		}

		iIndex++;
	}
	
	return	TRUE;
}