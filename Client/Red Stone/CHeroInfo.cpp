#include "CHero.H"
#include "CActor.H"
#include "CGamePlay.H"
#include "CGuild.h"
#include "miniPetWindow.h"
#include "rebirth_define.h"


DWORD	g_aExpTable[1000]=
{
00,250,420,770,1200,1710,2300,3190,4200,5330,6580,7950,9440,11390,13500,15770,18200,20790,23540,26450,29520,33250,
37180,41310,45640,50170,54900,59830,64960,70290,75820,82250,88920,95830,102980,110370,118000,125870,133980,142330,150920,159750,
168820,179070,189600,200410,211500,222870,234520,246450,258660,271150,283920,296970,310300,323910,337800,353190,368900,384930,401280,417950,
434940,452250,469880,487830,506100,524690,543600,562830,582380,602250,622440,644490,666900,689670,712800,736290,760140,784350,808920,833850,
859140,884790,910800,937170,963900,990990,1018440,1046250,1074420,1104850,1135680,1166910,1198540,1230570,1263000,1295830,1329060,1362690,1396720,1431150,
1465980,1501210,1536840,1572870,1609300,1646130,1683360,1720990,1759020,1799750,1840920,1882530,1924580,1967070,2010000,2053370,2097180,2141430,2186120,2231250,
2276820,2322830,2369280,2416170,2463500,2511270,2559480,2608130,2657220,2706750,2756720,2809870,2863500,2917610,2972200,3027270,3082820,3138850,3195360,3252350,
3309820,3367770,3426200,3485110,3544500,3604370,3664720,3725550,3786860,3848650,3910920,3973670,4036900,4100610,4164800,4232690,4301100,4370030,4439480,4509450,
4579940,4650950,4722480,4794530,4867100,4940190,5013800,5087930,5162580,5237750,5313440,5389650,5466380,5543630,5621400,5699690,5778500,5857830,5937680,6018050,
6098940,6184090,6269800,6356070,6442900,6530290,6618240,6706750,6795820,6885450,6975640,7066390,7157700,7249570,7342000,7434990,7528540,7622650,7717320,7812550,
7908340,8004690,8101600,8199070,8297100,8395690,8494840,8594550,8694820,8799950,8905680,9012010,9118940,9226470,9334600,9443330,9552660,9662590,9773120,9884250,
9995980,10108310,10221240,10334770,10448900,10563630,10678960,10794890,10911420,11028550,11146280,11264610,11383540,11503070,11623200,11743930,11865260,11987190,12109720,12237750,
12366420,12495730,12625680,12756270,12887500,13019370,13151880,13285030,13418820,13553250,13688320,13824030,13960380,14097370,14235000,14373270,14512180,14651730,14791920,14932750,
15074220,15216330,15359080,15502470,15646500,15791170,15936480,16082430,16229020,16376250,16524120,16678170,16832900,16988310,17144400,17301170,17458620,17616750,17775560,17935050,
18095220,18256070,18417600,18579810,18742700,18906270,19070520,19235450,19401060,19567350,19734320,19901970,20070300,20239310,20409000,20579370,20750420,20922150,21094560,21267650,
21441420,21615870,21791000,21966810,22143300,22326690,22510800,22695630,22881180,23067450,23254440,23442150,23630580,23819730,24009600,24200190,24391500,24583530,24776280,24969750,
25163940,25358850,25554480,25750830,25947900,26145690,26344200,26543430,26743380,26944050,27145440,27347550,27550380,27753930,27958200,28163190,28368900,28575330,28782480,28990350,
29198940,29415190,29632200,29849970,30068500,30287790,30507840,30728650,30950220,31172550,31395640,31619490,31844100,32069470,32295600,32522490,32750140,32978550,33207720,33437650,
33668340,33899790,34132000,34364970,34598700,34833190,35068440,35304450,35541220,35778750,36017040,36256090,36495900,36736470,36977800,37219890,37462740,37706350,37950720,38203550,
38457180,38711610,38966840,39222870,39479700,39737330,39995760,40254990,40515020,40775850,41037480,41299910,41563140,41827170,42092000,42357630,42624060,42891290,43159320,43428150,
43697780,43968210,44239440,44511470,44784300,45057930,45332360,45607590,45883620,46160450,46438080,46716510,46995740,47275770,47556600,47838230,48120660,48403890,48687920,48981250,
49275420,49570430,49866280,50162970,50460500,50758870,51058080,51358130,51659020,51960750,52263320,52566730,52870980,53176070,53482000,53788770,54096380,54404830,54714120,55024250,
55335220,55647030,55959680,56273170,56587500,56902670,57218680,57535530,57853220,58171750,58491120,58811330,59132380,59454270,59777000,60100570,60424980,60750230,61076320,61403250,
61731020,62068970,62407800,62747510,63088100,63429570,63771920,64115150,64459260,64804250,65150120,65496870,65844500,66193010,66542400,66892670,67243820,67595850,67948760,68302550,
68657220,69012770,69369200,69726510,70084700,70443770,70803720,71164550,71526260,71888850,72252320,72616670,72981900,73348010,73715000,74082870,74451620,74821250,75191760,75563150,
75935420,76308570,76682600,77057510,77433300,77820190,78208000,78596730,78986380,79376950,79768440,80160850,80554180,80948430,81343600,81739690,82136700,82534630,82933480,83333250,
83733940,84135550,84538080,84941530,85345900,85751190,86157400,86564530,86972580,87381550,87791440,88202250,88613980,89026630,89440200,89854690,90270100,90686430,91103680,91521850,
91940940,92360950,92781880,93203730,93626500,94050190,94474800,94900330,95326780,95754150,96182440,96622790,97064100,97506370,97949600,98393790,98838940,99285050,99732120,100180150,
100629140,101079090,101530000,101981870,102434700,102888490,103343240,103798950,104255620,104713250,105171840,105631390,106091900,106553370,107015800,107479190,107943540,108408850,108875120,109342350,
109810540,110279690,110749800,111220870,111692900,112165890,112639840,113114750,113590620,114067450,114545240,115023990,115503700,115984370,116466000,116948590,117432140,117916650,118402120,118900650,
119400180,119900710,120402240,120904770,121408300,121912830,122418360,122924890,123432420,123940950,124450480,124961010,125472540,125985070,126498600,127013130,127528660,128045190,128562720,129081250,
129600780,130121310,130642840,131165370,131688900,132213430,132738960,133265490,133793020,134321550,134851080,135381610,135913140,136445670,136979200,137513730,138049260,138585790,139123320,139661850,
140201380,140741910,141283440,141825970,142369500,142914030,143459560,144006090,144553620,145115250,145677920,146241630,146806380,147372170,147939000,148506870,149075780,149645730,150216720,150788750,
151361820,151935930,152511080,153087270,153664500,154242770,154822080,155402430,155983820,156566250,157149720,157734230,158319780,158906370,159494000,160082670,160672380,161263130,161854920,162447750,
163041620,163636530,164232480,164829470,165427500,166026570,166626680,167227830,167830020,168433250,169037520,169642830,170249180,170856570,171465000,172074470,172684980,173296530,173909120,174522750,
175137420,175767270,176398200,177030210,177663300,178297470,178932720,179569050,180206460,180844950,181484520,182125170,182766900,183409710,184053600,184698570,185344620,185991750,186639960,187289250,
187939620,188591070,189243600,189897210,190551900,191207670,191864520,192522450,193181460,193841550,194502720,195164970,195828300,196492710,197158200,197824770,198492420,199161150,199830960,200501850,
201173820,201846870,202521000,203196210,203872500,204549870,205228320,205907850,206588460,207270150,207952920,208636770,209321700,210007710,210694800,211398190,212102700,212808330,213515080,214222950,
214931940,215642050,216353280,217065630,217779100,218493690,219209400,219926230,220644180,221363250,222083440,222804750,223527180,224250730,224975400,225701190,226428100,227156130,227885280,228615550,
229346940,230079450,230813080,231547830,232283700,233020690,233758800,234498030,235238380,235979850,236722440,237466150,238210980,238956930,239704000,240452190,241201500,241951930,242703480,243456150,
244209940,244964850,245720880,246478030,247236300,247995690,248756200,249517830,250280580,251044450,251809440,252591890,253375500,254160270,254946200,255733290,256521540,257310950,258101520,258893250,
259686140,260480190,261275400,262071770,262869300,263667990,264467840,265268850,266071020,266874350,267678840,268484490,269291300,270099270,270908400,271718690,272530140,273342750,274156520,274971450,
275787540,276604790,277423200,278242770,279063500,279885390,280708440,281532650,282358020,283184550,284012240,284841090,285671100,286502270,287334600,288168090,289002740,289838550,290675520,291513650,
292352940,293193390,294035000,294877770,295721700,296566790,297413040,298260450,299109020,299976250,300844680,301714310,302585140,303457170,304330400,305204830,306080460,306957290,307835320,308714550,
309594980,310476610,311359440,312243470,313128700,314015130,314902760,315791590,316681620,317572850,318465280,319358910,320253740,321149770,322047000,322945430,323845060,324745890,325647920,326551150,
327455580,328361210,329268040,330176070,331085300,331995730,332907360,333820190,334734220,335649450,336565880,337483510,338402340,339322370,340243600,341166030,342089660,343014490,343940520,344867750,
345796180,346725810,347656640,348588670,349521900,350456330,351391960,352328790,353266820,354224750,355183920,356144330,357105980,358068870,359033000,359998370,360964980,361932830,362901920,363872250,
364843820,365816630,366790680,367765970,368742500,369720270,370699280,371679530,372661020,373643750,374627720,375612930,376599380,377587070,378576000,379566170,380557580,381550230,382544120,383539250,
384535620,385533230,386532080,387532170,388533500,389536070,390539880,391544930,392551220,393558750,394567520,395577530,396588780,397601270,398615000,399629970,400646180,401663630,402682320,403702250,
404723420,405745830,406769480,407794370,408820500,409847870,410876480,411906330,412937420,413969750,415003320,416058070,417114100,418171410,419230000,420289870,421351020,422413450,
};

static int
getClientExpTableIndex(int _iLevel)
{
	const int iMaxIndex = sizeof(g_aExpTable)/sizeof(g_aExpTable[0])-1;

	if	(_iLevel < 0)
		return	0;
	if	(_iLevel > iMaxIndex)
		return	iMaxIndex;

	return	_iLevel;
}

static DWORD
getClientLevelupExperience(int _iLevel)
{
	return	g_aExpTable[getClientExpTableIndex(_iLevel)];
}

//
//	레벨 설정
void
CHero::setLevel(int _iLevel)
{
	m_iLevel			=	_iLevel;
	m_level.m_sValue	=	_iLevel;
}

//
//	레벨업 까지 남은 경험치
DWORD
CHero::getRemainExpForLevelUp()
{
	DWORD	dwLevelupExperience	=	getClientLevelupExperience(m_iLevel);

	if	(dwLevelupExperience <= m_iExperience)
		return	0;

	return	dwLevelupExperience	-	m_iExperience;
}

//
//	레벨업에 필요한 경험치
DWORD
CHero::getExpForLevelUp()
{
	return	getClientLevelupExperience(m_iLevel);
}

//
//	현재 레벨의 경험치
DWORD
CHero::getCurrentLevelExp()
{
	return	m_iExperience;
}

//
//	레벨업 까지 남은 경험치
int
CHero::getRemainExpForSkillLevelUp(int _iAbility)
{
	return	m_aAbility[_iAbility].getRemainExpForLevelUp();
}

//
//	레벨업 가능한 어빌러티다.
BOOL
CHero::isLevelUpAbleAbility(cAbility *_lpAbility)
{
	if (!_lpAbility	)
		return	FALSE;
	
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if	(!lpSkill	)
		return	FALSE;	//	이건 문제인데.. --
	if	(_lpAbility->m_wLevel	>=	dLIMIT_SKILL_LEVEL		)
		return	FALSE;
	if	(lpSkill->m_bf1IsTestSkill	&& g_bIsTestServer==FALSE	)
		return	FALSE;
	if	(m_dwSkillExperience		<	(DWORD)_lpAbility->getRemainExpForLevelUp())
		return	FALSE;

	for (int i=0;i<dMAX_REQUIRE_SKILL_COUNT;i++)
	{
		if	(lpSkill->m_aRequireSkill[i].m_wSkill	==	0xffff)
			break;
		
		int	iSkill	=	lpSkill->m_aRequireSkill[i].m_wSkill;

		_lpAbility	=	getAbilityBySkill(iSkill);

		if	(!_lpAbility)
			return	FALSE;
		if	(_lpAbility->m_wLevel	<	lpSkill->m_aRequireSkill[i].m_wLevel)
			return	FALSE;
	}

	return	TRUE;
}

//
//	레벨업 가능한 어빌러티.. 냐?
BOOL
CHero::isLevelUpAbleAbility(int _iAbility)
{
	cAbility	*lpAbility	=	getAbility(_iAbility);

	return	isLevelUpAbleAbility(lpAbility);
}

//
//	리액션 스킬이냐?
BOOL
CHero::isReactionSkill(int _iAbility)
{
	return	m_aAbility[_iAbility].isReactionSkill();
}

//
//	사용 가능한 스킬이냐?


int
CHero::isUseAbleSkill(int _iAbility)
{
	int	iCP			=	m_iCP;

	cAbility	*lpAbility	=	getAbility(_iAbility);
	CSkill		*lpSkill	=	lpAbility->getSkill();
	cAbility	ability;

	ability.copy(lpAbility);
	ability.m_wLevel		+=	getCorrectSkillLevel(lpAbility);

	if	(g_lpHero->m_wHillSkill	!=	0xffff)
	{
		if	(ability.isCanNotActionSkillOnHill()					)
			return	eUSR_CANO_NOT_CLOSE_ATTACK_ON_HILL;
		if	(lpSkill->m_wType	==	eSKILL_TYPE_ICY_STALAGMITE	)
			iCP	=	1000000;
	}

	if	(!lpSkill)
		return	eUSR_VALID_SKILL;
	if(m_wRushTime)		// 돌진
		return eUSR_NOT_NEED_STATE;
	if(m_wBindingWordsTime)
	{	// 언어구속상태.
		if(lpSkill->m_wDifficultyLevel >= m_wBindingWordsOverLimitLevel)
			return eUSR_BINDING_WORDS;
	}
	if(g_map.m_bf1IsNotUseSkillBeforeTrans)
	{
		if(!m_wTransformationTime)
			return eUSR_NOT_NEED_STATE;

	}
	if(g_hero.m_wPrincessWeaponOwner	!=	0xffff)
	{

		if(!lpSkill->bf1IsAbleSkillWhenTransToWeapon)
			return eUSR_NOT_NEED_STATE;
	}

	if(!isUseSkillState(lpSkill))		// 스킬이 사용가능한 상태인가..
		return eUSR_NOT_NEED_STATE;
	if(m_wFleeTime)
	{
		return eUSR_NOT_NEED_STATE;
	}
	if	(lpSkill->m_wType==	eSKILL_TYPE_SHIMMERING_SHIELD)
		if	(g_lpHero->m_wIsCastBlocker)	
			return	eUSR_SUCCESS;

	BOOL	isEquipWeapon=FALSE,isEquipWeapon2=FALSE,isEquipShield=FALSE;
	int		iBulletCount=0;

	cItem	*lpWeapon	=	getWeapon();
	cItem	*lpWeapon2	=	getOtherJobWeapon();
	cItem	*lpShield	=	getShield();
	cItem	*lpBullet	=	getBullet();

	if (lpWeapon && isUseAbleItem(lpWeapon)	)	isEquipWeapon	=	TRUE;
	if (lpWeapon2							)	isEquipWeapon2	=	TRUE;
	if (lpShield && isUseAbleItem(lpShield)	)	isEquipShield	=	TRUE;
	if (lpBullet && isUseAbleItem(lpBullet)	)	
		if (getBullet()->m_bCount	>	0)
		{
			iBulletCount	=	getBullet()->m_bCount;

			if (getBullet()->isInfinityBullet())
				iBulletCount	=	255;
		}

	if (g_lpHero->m_wIsCastBlocker || g_lpHero->m_wIsHideShield)
		isEquipShield	=	FALSE;

	if	(lpSkill->m_wType==	eSKILL_TYPE_COMPLETE_PROTECTION)
	{
		if	(isActiveCompleteProtection())
			return	eUSR_SUCCESS;
		if(!isEquipShield )
			return eUSR_SHIELD_NOT_FOUND;		
	}

	if	(ability.isRequirePotion())
	{
		if	(!getFirstPotion())
			return	eUSR_REQUIRE_POTION;
	}
	if	(ability.isRequireDrug())
	{
		if	(!getFirstDrug())
			return	eUSR_REQUIRE_DURG;
	}
	if	(ability.isRequireFlower())
	{
		if	(!getFirstFlower())
			return	eUSR_REQUIRE_FLOWER;
	}
	if	(ability.isRequireCandy())
	{
		if	(!getFirstCandy())
			return	eUSR_REQUIRE_CANDY;
	}
	if(m_wTransformationTime)
	{
		isEquipWeapon	=	TRUE;
		isEquipWeapon2	=	TRUE;
		isEquipShield	=	FALSE;
		iBulletCount	=	255;
	}

	if(m_isMagicBullet)
		iBulletCount	=	255;

	int	iResult	=	ability.isUseAble(iCP,isEquipWeapon,isEquipWeapon2,isEquipShield,iBulletCount,getPetCount(),g_hero.getSummonBeastCount());
	if(iResult == eUSR_LOWER_CP && m_wMadnessTime)
	{	// 광란상태..
		iResult = eUSR_SUCCESS;
	}
	if	(iResult	!=	eUSR_SUCCESS)
		return	iResult;

	if	(g_hero.getCoolTime(&ability))
		return	eUSR_IS_COOL_TIME;

	return	eUSR_SUCCESS;
}



//
//_iSkill인 어빌러티를 찾는다.
cAbility*
CHero::getAbilityBySkill(int _iSkill)
{
	if (_iSkill	==	0xffff)	return	NULL;

	for (int i=0;i<dABILITY_COUNT;i++)
		if (m_aAbility[i].m_wSkill	==	_iSkill)	return	&m_aAbility[i];

	return	NULL;
}

//
//_iSkill인 어빌러티를 찾는다.
int
CHero::getAbilityIndexBySkill(int _iSkill)
{
	if (_iSkill	==	0xffff)	return	0xffff;

	for (int i=0;i<dABILITY_COUNT;i++)
		if (m_aAbility[i].m_wSkill	==	_iSkill)	return	i;

	return	0xffff;
}

//
//스킬 레벨이 올랐다.
void
CHero::increaseSkillLevel(int _iAbility,int _iLevel,DWORD _dwRemainSkillPoint)
{
	cAbility	*lpAbility	=	&m_aAbility[_iAbility];

	m_dwSkillExperience	=	_dwRemainSkillPoint;
	lpAbility->m_wLevel	=	_iLevel;

	if (lpAbility->m_wLevel	==	dLIMIT_SKILL_LEVEL)
		buildPower();
}

//
//	공격력을 구한다.
int
CHero::getAttackPower(cAbility *_lpAbility,BOOL _isMin)
{
	
	if	(_lpAbility)
	{
		CSkill	*lpSkill	=	_lpAbility->getSkill();

		if	(lpSkill->getExtraEffectByEffect(eSKILL_EE_SET_TRAP))
		{
			int			iDamage					=	0;
			int			iSkillDamage			=	_lpAbility->getAttackPower();
			int			iSkillDamagePercentage	=	_lpAbility->getAttackPowerPercentage();
			int			iLevelCorrectDamagePercentage	=	0;
			
			iDamage		=	iSkillDamage*iSkillDamagePercentage/100;
			iDamage		=	iDamage+iDamage*getStrength()/200;
			
			return	iDamage;
		}
	}
	
	int	iMinSkillDamage=0,iMaxSkillDamage=0,iSkillDamagePercentage=100;
	
	if (_lpAbility)
	{
		CSkill	*lpSkill	=	_lpAbility->getSkill();

		int iAttackPointPerActorLevel = 0;
		
		if(lpSkill)
			iAttackPointPerActorLevel = lpSkill->m_sAttackPointPerActorLevel * getLevel();
		
		iMinSkillDamage			=	_lpAbility->getAttackPower(TRUE) + iAttackPointPerActorLevel;
		iMaxSkillDamage			=	_lpAbility->getAttackPower(FALSE) + iAttackPointPerActorLevel;
		iSkillDamagePercentage	=	_lpAbility->getAttackPowerPercentage();
	}

//	공격력 = (무기의 기본 공격력 * 아이템, 기술에 의한 보정율 총합 + 아이템, 기술에 의한 보정치 총합) * (1 + 힘 * 0.01)
//	소수점 2자리 까지 공격한다. 눈에 보이는 대미지는 /100을 해야 한다.

	int	iMinDamage,iMaxDamage;

	iMinDamage	=	m_iWeaponMinDamage+m_weaponPower.getCorrectedValue();
	iMaxDamage	=	m_iWeaponMaxDamage+m_weaponPower.getCorrectedValue();

	if (_lpAbility)
	{
		if	(_lpAbility->getSkill()->m_bf1IsIgnoreWeaponDamage	==	TRUE)
		{
			iMinDamage	=	0;
			iMaxDamage	=	0;
		}

		if (_lpAbility->isRequireBullet())
		{
			cItem	*lpBullet		=	getBullet();

			if (lpBullet)
			{
				iMinDamage	+=	lpBullet->getBasicItem(TRUE)->m_wMinDamage*100;
				iMaxDamage	+=	lpBullet->getBasicItem(TRUE)->m_wMaxDamage*100;
			}
		}

		if (_lpAbility->isRequireOtherJobWeapon())
		{
			cItem	*lpWeapon	=	getOtherJobWeapon();

			if (lpWeapon)
			{
				iMinDamage	+=	lpWeapon->getBasicItem(TRUE)->m_wMinDamage*100;
				iMaxDamage	+=	lpWeapon->getBasicItem(TRUE)->m_wMaxDamage*100;
			}
		}
	}

	iMinDamage	=	iMinDamage*(m_attackPower.getCorrectPercentageValue()+iSkillDamagePercentage)/100;
	iMinDamage	=	iMinDamage+m_attackPower.getCorrectValue()*100 + iMinSkillDamage;

	iMaxDamage	=	iMaxDamage*(m_attackPower.getCorrectPercentageValue()+iSkillDamagePercentage)/100;
	iMaxDamage	=	iMaxDamage+m_attackPower.getCorrectValue()*100 + iMaxSkillDamage;

	iMinDamage	+=	m_iCorrectMinimumDamage;
	iMaxDamage	+=	m_iCorrectMaximumDamage;

	iMinDamage	=	iMinDamage+iMinDamage*getStrength()/200;
	iMaxDamage	=	iMaxDamage+iMaxDamage*getStrength()/200;

	iMinDamage	+=	iMinDamage*m_wIncreaseFinalPhysicalAttackPower/100;
	iMaxDamage	+=	iMaxDamage*m_wIncreaseFinalPhysicalAttackPower/100;

	if	(_isMin)
		return	min(iMinDamage,iMaxDamage);

	return	max(iMinDamage,iMaxDamage);
}

//	무기 대미지를 구한다.
int
CHero::getWeaponDamage(BOOL _isMin)
{
	int	iDamage;

	if (_isMin)
		iDamage	=	m_iWeaponMinDamage+m_weaponPower.getCorrectedValue();
	else
		iDamage	=	m_iWeaponMaxDamage+m_weaponPower.getCorrectedValue();

	iDamage			+=	iDamage*m_attackPower.getCorrectPercentageValue()/100;

	return	iDamage;
}

//
//	지금 변신 가능하냐?
int
CHero::isAbleToTransformation()
{
	if	(m_wJob		>=	dPLAYER_JOB_COUNT										)
		return	eCTR_IS_NOT_TRANSFORMATION_JOB;

	int iCorrectTransFrameCounter = dSYNC_FPS/2*m_bCorrectTransformationSpeed/100;

	if	(m_iLastTransformationTime	+dSYNC_FPS*2-iCorrectTransFrameCounter	>	s_iFrameCounter	)
		return	eCTR_IS_NOT_TRANSFORMATION_TIME;

	if	(m_iLastActionTime			+dSYNC_FPS/4-iCorrectTransFrameCounter	>	s_iFrameCounter	)
		return	eCTR_IS_NOT_TRANSFORMATION_TIME;

	if	(g_lpHero	&&	g_lpHero->isSitdown()							)
		return	eCTR_CAN_NOT_TRANSFORMATION_IN_SITDOWN_STATUS;

	if	(c_aFreeJobChangeJob[m_wJob]	==	FALSE)
		if	(getCPLevel() < dLOW_CP_LEVEL_FOR_TRANSFORMATIION)
			return	eCTR_IS_REQUIRE_TRANSFORMATION_CP;

	int	iAnotherJob	=	getAnotherJob();

	for (int i=0;i<dHERO_ABILITY_COUNT;i++)	
	{
		if	(m_aAbility[i].m_wLevel	<=	0)
			continue;

		if	(m_aAbility[i].isEnableJob(iAnotherJob))
			return	eCTR_OK;
	}

	return	eCTR_REQUIRE_SECOND_JOB_SKILL;
}

//
//	지금 앉을 수 있냐?
BOOL
CHero::isAbleToSitdown()
{
	if	(m_iLastActionTime	+	dSYNC_FPS	>	s_iFrameCounter)
		return	FALSE;

	return	TRUE;
}

//
//	지금 이동 방법을 바꿀 수 있냐?
BOOL
CHero::isAbleToChangeMoveMethod()
{
	if	(m_iLastActionTime	+	dSYNC_FPS/2	>	s_iFrameCounter)
		return	FALSE;

	return	TRUE;
}	//	CHero::isAbleToSitdown()

//
//	레벨에 의한 스킬 포인트의 합
int
CHero::getSkillPointSumByLevel()
{
	int	iSkillPoint		=	0;
	int	iLevel			=	max(m_iLevel,1);

	for (int i=2;i<=iLevel;i++)
		iSkillPoint	+=	min(i,100);

	DWORD	dwLevelupExperience		=	getClientLevelupExperience(iLevel);
	DWORD	dwNextSkillPoint		=	min(iLevel+1,100);
	LONGLONG	llExp				=	m_iExperience;
	llExp							*=	dwNextSkillPoint;

	if	(dwLevelupExperience	!=	0)
		llExp						/=	dwLevelupExperience;
	else
		llExp						=	0;

	int		iCurrentLevelSkillPoint	=	(int)llExp;

	iSkillPoint						+=	iCurrentLevelSkillPoint;	//	경험치에 의한 보정
	iSkillPoint						+=	m_wBonusSkillPoint;
	iSkillPoint						+=	m_bf4CH5SkillBonusCount*100;
	iSkillPoint						+=	c_aBonusSkillPointForRebirth[m_bf4RebirthCount];

	return	iSkillPoint;
}

//
//	스킬 포인트 합 얻기
int
CHero::getSkillPointSum()
{
	int i,iSkillPoint=m_dwSkillExperience;

	for (i=0;i<dABILITY_COUNT;i++)
	{
		cAbility	*lpAbility	=	&m_aAbility[i];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if (!lpSkill || lpAbility->m_wLevel==0)	continue;

		int	iPoint	=	lpAbility->getSkillPoint();

		iSkillPoint	+=	iPoint;
	}

	return	iSkillPoint-1;
}


BOOL
CHero::isOwnForce(CActor *_lpActor)
{
	if (_lpActor->m_wTeam == m_wTeam)
		return	TRUE;

	return	FALSE;
}


BOOL
CHero::isEnemy(CActor *_lpActor)
{
	if	(_lpActor->isMonster())
		return	TRUE;

	return	FALSE;
}

int
CHero::getFireResistance()		
{
	int	iResistance	=	getValuePassiveSkill(eSKILL_PEE_INCREASE_MAGIC_RESISTANCE)/100 + m_allMagicResistance.getCorrectedValue()+m_fireResistance.getCorrectedValue()+getWisdom()/20;

	if	(m_wJob	==	dJOB_DEVIL)
		iResistance	+=	15;

	if	(g_lpHero->m_wRideDog	!=	0xffff)
	{
		cAbility	*lpRidingDogSkill	=	getRidingDogSkill();

		if	(lpRidingDogSkill)
			iResistance	+=	10+lpRidingDogSkill->m_wLevel+getCorrectSkillLevel(lpRidingDogSkill);
	}

	iResistance	+=	getFireResistanceBonusByA();

	int	iFieldElementalResistance	=	g_map.m_sCorrectFireResistance;
	iFieldElementalResistance		-=	m_wCorrectFieldElementalResistanceByTitle;
	iFieldElementalResistance		-=	m_bCorrectFieldFirePenelty;
	iFieldElementalResistance		=	max(iFieldElementalResistance,0);

	return	min(iResistance-iFieldElementalResistance,90);
}

int
CHero::getWaterResistance()
{
	int	iResistance	=	getValuePassiveSkill(eSKILL_PEE_INCREASE_MAGIC_RESISTANCE)/100 + m_allMagicResistance.getCorrectedValue()+m_waterResistance.getCorrectedValue()+getWisdom()/20;

	iResistance	+=	getWaterResistanceBonusByA();

	int	iFieldElementalResistance	=	g_map.m_sCorrectWaterResistance;
	iFieldElementalResistance		-=	m_wCorrectFieldElementalResistanceByTitle;
	iFieldElementalResistance		-=	m_bCorrectFieldWaterPenelty;
	iFieldElementalResistance		=	max(iFieldElementalResistance,0);

	return	min(iResistance-iFieldElementalResistance,90);
}

int
CHero::getWindResistance()		
{
	int	iResistance	=	getValuePassiveSkill(eSKILL_PEE_INCREASE_MAGIC_RESISTANCE)/100 + m_allMagicResistance.getCorrectedValue()+m_windResistance.getCorrectedValue()+getWisdom()/20;

	iResistance	+=	getWindResistanceBonusByA();

	int	iFieldElementalResistance	=	g_map.m_sCorrectWindResistance;
	iFieldElementalResistance		-=	m_wCorrectFieldElementalResistanceByTitle;
	iFieldElementalResistance		-=	m_bCorrectFieldWindPenelty;
	iFieldElementalResistance		=	max(iFieldElementalResistance,0);

	return	min(iResistance-iFieldElementalResistance,90);
}

int
CHero::getEarthResistance()	
{
	int	iResistance	=	getValuePassiveSkill(eSKILL_PEE_INCREASE_MAGIC_RESISTANCE)/100 + m_allMagicResistance.getCorrectedValue()+m_earthResistance.getCorrectedValue()+getWisdom()/20;

	iResistance	+=	getEarthResistanceBonusByA();

	int	iFieldElementalResistance	=	g_map.m_sCorrectEarthResistance;
	iFieldElementalResistance		-=	m_wCorrectFieldElementalResistanceByTitle;
	iFieldElementalResistance		-=	m_bCorrectFieldEarthPenelty;
	iFieldElementalResistance		=	max(iFieldElementalResistance,0);

	return	min(iResistance-iFieldElementalResistance,90);
}

int
CHero::getLightResistance()	
{
	int	iResistance	=	getValuePassiveSkill(eSKILL_PEE_INCREASE_MAGIC_RESISTANCE)/100 + m_allMagicResistance.getCorrectedValue()+m_lightResistance.getCorrectedValue()+getWisdom()/20;

	iResistance	+=	getLightResistanceBonusByA();

	int	iFieldElementalResistance	=	g_map.m_sCorrectLightResistance;
	iFieldElementalResistance		-=	m_wCorrectFieldElementalResistanceByTitle;
	iFieldElementalResistance		-=	m_bCorrectFieldLightPenelty;
	iFieldElementalResistance		=	max(iFieldElementalResistance,0);

	return	min(iResistance-iFieldElementalResistance,90);
}

int
CHero::getDarkResistance()		
{
	int	iResistance	=	getValuePassiveSkill(eSKILL_PEE_INCREASE_MAGIC_RESISTANCE)/100 + m_allMagicResistance.getCorrectedValue()+m_darkResistance.getCorrectedValue()+getWisdom()/20;

	iResistance	+=	getDarkResistanceBonusByA();

	int	iFieldElementalResistance	=	g_map.m_sCorrectDarkResistance;
	iFieldElementalResistance		-=	m_wCorrectFieldElementalResistanceByTitle;
	iFieldElementalResistance		-=	m_bCorrectFieldDarkPenelty;
	iFieldElementalResistance		=	max(iFieldElementalResistance,0);

	return	min(iResistance-iFieldElementalResistance,90);
}

int
CHero::getCurrentLevelDeathPeneltyValue()
{
	if	(m_wDeathPeneltySecond == 0	||	m_wAnimatePartnerTime)
		return 0;

	for (int iIndex=0;iIndex<20;iIndex++)
	{
		if (m_iLevel >= g_aPlayerDeathPeneltyInfo[iIndex].m_iBeginLevel &&
			m_iLevel <= g_aPlayerDeathPeneltyInfo[iIndex].m_iEndLevel)
		{
			int	iValue	=	g_aPlayerDeathPeneltyInfo[iIndex].m_iPeneltyValue;

			iValue		-=	m_wReduceDeathPeneltyValueByTitle*iValue/100;

			return	iValue;
		}
	}

	return	0;
}

int
CHero::getCurrentLevelDeathPeneltyRecoverPrice()
{
	if (m_wDeathPeneltySecond == 0)
		return 0;

	for (int iIndex=0;iIndex<20;iIndex++)
	{
		if (m_iLevel >= g_aPlayerDeathPeneltyInfo[iIndex].m_iBeginLevel &&
			m_iLevel <= g_aPlayerDeathPeneltyInfo[iIndex].m_iEndLevel)
			return	m_iLevel*m_iLevel*g_aPlayerDeathPeneltyInfo[iIndex].m_iRecoverPriceValue/10;
	}

	return	0;
}

class	CCorrectIntelligenceFactor
{
public:
	int		m_iLevel;
	float	m_fValue;
};

CCorrectIntelligenceFactor	l_aCorrectIntelligenceFactor[]	=	
{
	0		,0.005f,
	100		,0.0051f,
	132		,0.0052f,
	174		,0.0053f,
	230		,0.0054f,
	304		,0.0055f,
	402		,0.0056f,
	531		,0.0057f,
	702		,0.0058f,
	928		,0.0059f,
	1227	,0.006f,
	1577	,0.0061f,
	1927	,0.0062f,
	2277	,0.0063f,
	2627	,0.0064f,
	2977	,0.0065f,
	3327	,0.0066f,
	3677	,0.0067f,
	4027	,0.0068f,
	4377	,0.0069f,
	4727	,0.007f,
	5127	,0.0071f,
	5527	,0.0072f,
	5927	,0.0073f,
	6327	,0.0074f,
	6727	,0.0075f,
	7127	,0.0076f,
	7527	,0.0077f,
	7927	,0.0078f,
	8327	,0.0079f,
	8727	,0.008f,
	100000	,0.008f,
	-1,
};


float
CHero::getCorrectIntelligenceFactorForMagicDamage(CSkill *_lpSkill)
{
	if	(_lpSkill && _lpSkill->isPhysicalMagicDamageSkill())
		return	0.005f;

	int	iIntelligence=	getIntelligence();

	for (int iIndex=0;;iIndex++)
	{
		if (iIntelligence >= l_aCorrectIntelligenceFactor[iIndex].m_iLevel && iIntelligence	< l_aCorrectIntelligenceFactor[iIndex+1].m_iLevel)
			return	l_aCorrectIntelligenceFactor[iIndex].m_fValue;
	}

	return	0.005f;
}

int
CHero::getStrengtheningFireDamage(int _iCorrectByTitle, BOOL _bIsPureMagicDamageSkill)
{// 마지막 수정일 : 09.09.28
	return	_iCorrectByTitle+m_strengtheningFireDamage.getCorrectedValue()+m_strengtheningMagicDamage.getCorrectedValue()*_bIsPureMagicDamageSkill;
}

int
CHero::getStrengtheningWaterDamage(int _iCorrectByTitle, BOOL _bIsPureMagicDamageSkill)
{// 마지막 수정일 : 09.09.28
	return	_iCorrectByTitle+m_strengtheningWaterDamage.getCorrectedValue()+m_strengtheningMagicDamage.getCorrectedValue()*_bIsPureMagicDamageSkill;
}

int
CHero::getStrengtheningWindDamage(int _iCorrectByTitle, BOOL _bIsPureMagicDamageSkill)
{// 마지막 수정일 : 09.09.28
	return	_iCorrectByTitle+m_strengtheningWindDamage.getCorrectedValue()+m_strengtheningMagicDamage.getCorrectedValue()*_bIsPureMagicDamageSkill;
}

int
CHero::getStrengtheningEarthDamage(int _iCorrectByTitle, BOOL _bIsPureMagicDamageSkill)
{// 마지막 수정일 : 09.09.28
	return	_iCorrectByTitle+m_strengtheningEarthDamage.getCorrectedValue()+m_strengtheningMagicDamage.getCorrectedValue()*_bIsPureMagicDamageSkill;
}

int
CHero::getStrengtheningLightDamage(int _iCorrectByTitle, BOOL _bIsPureMagicDamageSkill)
{// 마지막 수정일 : 09.09.28
	return	_iCorrectByTitle+m_strengtheningLightDamage.getCorrectedValue()+m_strengtheningMagicDamage.getCorrectedValue()*_bIsPureMagicDamageSkill;
}

int
CHero::getStrengtheningDarkDamage(int _iCorrectByTitle, BOOL _bIsPureMagicDamageSkill)
{// 마지막 수정일 : 09.09.28
	return	_iCorrectByTitle+m_strengtheningDarkDamage.getCorrectedValue()+m_strengtheningMagicDamage.getCorrectedValue()*_bIsPureMagicDamageSkill;
}
int
CHero::getWeakningTargetFireResistance()
{
	return	m_weakningTargetFireResistance.getCorrectedValue()+m_weakningTargetMagicResistance.getCorrectedValue();
}

int
CHero::getWeakningTargetWaterResistance()
{
	return	m_weakningTargetWaterResistance.getCorrectedValue()+m_weakningTargetMagicResistance.getCorrectedValue();
}

int
CHero::getWeakningTargetWindResistance()
{
	return	m_weakningTargetWindResistance.getCorrectedValue()+m_weakningTargetMagicResistance.getCorrectedValue();
}

int
CHero::getWeakningTargetEarthResistance()
{
	return	m_weakningTargetEarthResistance.getCorrectedValue()+m_weakningTargetMagicResistance.getCorrectedValue();
}

int
CHero::getWeakningTargetLightResistance()
{
	return	m_weakningTargetLightResistance.getCorrectedValue()+m_weakningTargetMagicResistance.getCorrectedValue();
}

int
CHero::getWeakningTargetDarkResistance()
{
	return	m_weakningTargetDarkResistance.getCorrectedValue()+m_weakningTargetMagicResistance.getCorrectedValue();
}

BOOL
CHero::isFixedStrength()
{
	if (m_wJob	==	dJOB_WEREWOLF)
	{
		if (m_intelligence.isFixedValue())
			return	TRUE;
	}
	else
	{
		if (m_strength.isFixedValue())
			return	TRUE;
	}

	return	FALSE;
}


BOOL
CHero::isFixedIntelligence()
{
	if (m_wJob	==	dJOB_WEREWOLF)
	{
		if (m_strength.isFixedValue())
			return	TRUE;
	}
	else
	{
		if (m_intelligence.isFixedValue())
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CHero::isFixedAgility()
{
	if (m_wJob	==	dJOB_WEREWOLF)
	{
		if (m_wisdom.isFixedValue())
			return	TRUE;
	}
	else
	{
		if (m_agility.isFixedValue())
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CHero::isFixedWisdom()
{
	if (m_wJob	==	dJOB_WEREWOLF)
	{
		if (m_agility.isFixedValue())
			return	TRUE;
	}
	else
	{
		if (m_wisdom.isFixedValue())
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CHero::isFixedConstitution()
{
	if (m_wJob	==	dJOB_WEREWOLF)
	{
		if (m_charisma.isFixedValue())
			return	TRUE;
	}
	else
	{
		if (m_constitution.isFixedValue())
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CHero::isFixedCharisma()
{
	if (m_wJob	==	dJOB_WEREWOLF)
	{
		if (m_constitution.isFixedValue())
			return	TRUE;
	}
	else
	{
		if (m_charisma.isFixedValue())
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CHero::isFixedLuck()
{
	if (m_luck.isFixedValue())
		return	TRUE;

	return	FALSE;
}

int
CHero::getLevel(BOOL _bIsWantBaseValue)
{
	if	(_bIsWantBaseValue)
		return	m_iLevel;

	m_level.m_sValue	=	m_iLevel;

	int iValue	=	m_level.getCorrectedValue();
	if(m_wTransformationTime)
	{
		iValue = m_wTransformationLevel;
	}

	return	min(max(iValue,1),999);
}



int	
CHero::getGuildInfOriginArryPlace(char* _strName)
{
	for (int i = 0 ; i < g_guild.m_memberList.m_wCount ; i++)
	{
		if(STRICMP(g_guild.m_OriginmemberList.m_aMembers[i].getName() , _strName) == 0)
			return i;
	}

	return 0xffff;
}

int	
CHero::getGuildDisplayIndex()
{
	if(g_guild.m_memberList.m_wCount == 0)
		return 0xffff;

	for (int i = 0 ; i < g_guild.m_memberList.m_wCount ; i++)
	{
		if(STRICMP(g_guild.m_OriginmemberList.m_aMembers[i].getName() , g_lpHero->m_strName) == 0)
			return i;
	}

	return 0xffff;
}

//
//JBC	배지와 넷카페 특전을 포함한 레벨 조사. 08-09-08
int
CHero::getLevelIncludeBadge()
{
	int iMax = 0 ;
	int iBadgeCount	=	0;

	for (int i=0;i<getItemSlotCount();i++)
	{
		cItem	*lpItem	=	&m_aItems[i];
		
		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;
		
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem	)
			continue;

		if (lpBasicItem->m_attr.isBadge)
		{
			for(int j=0;j<dGENERATE_ITEM_DATA_COUNT;++j)
			{
				if(lpBasicItem->m_aGenerateData[j].m_wEffect	==	eIE_OVER_POWER_ITEM_EQUIP)	// 장비 배지이면..
				{
					
					if((int)lpBasicItem->m_aValue[0][1] > iMax)
						iMax = (int)lpBasicItem->m_aValue[0][1];
					
					if (iBadgeCount	>=	dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY)
						continue;
					
					iBadgeCount++;				
					
					break;
				}
			}
		}
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];
		
		if	(lpItem->m_wBaseItem	==	0xffff	||	lpItem->m_bCount	==	0)
			continue;
		
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem(TRUE);
		
		if	(!lpBasicItem	)
			continue;
		
		if (lpBasicItem->m_attr.isBadge)
		{
			if((int)lpBasicItem->m_aValue[0][1] > iMax)
				iMax = (int)lpBasicItem->m_aValue[0][1];
			
			if (iBadgeCount	>=	dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY)
				continue;
			
			iBadgeCount++;
		}
	}

#ifdef _FOR_JAPAN						//넷카페 유저라면 레벨제한 40감소!! 툴팁에서.
	if(g_hero.m_bf1IsNetCafeUser)
		iMax	+=	40;
#endif

	return min(iMax + m_iLevel , 999);
}


int
CHero::getMinipetAwakenType(int _iSlot)
{
	if(_iSlot == -1)
		_iSlot = g_gwMiniPetStatus.m_iSelectMiniPet;

	if(m_aMiniPet[_iSlot].m_bf1MinipetAwaken100)
		return dAWAKENTYPE_100;

	if(m_aMiniPet[_iSlot].m_bf1MinipetAwaken50)
		return dAWAKENTYPE_50;

	return	dAWAKENTYPE_NONE;
}


int
CHero::getStrength(BOOL _bIsWantBaseValue,BOOL _bIsAskByReversal)
{

	if(m_wTransformationTime)
		return m_wTransformationLevel;

	if	(m_wWantSummonedBeastIndex	!=	0xffff)
	{
		if	(m_wWantSummonedBeastIndex	>=	dOWN_SUMMON_BEAST_COUNT)
			return	0;

		return	max(m_aSummonBeast[m_wWantSummonedBeastIndex].m_wStrength,1);
	}

	if (_bIsWantBaseValue)
	{
		if	(m_wJob	==	dJOB_WEREWOLF)
			return	max(m_sIntelligence,1);

		return	max(m_sStrength,1);
	}

	if	(m_wReversalPowerTime	&&	_bIsAskByReversal	==	FALSE)
		return	getIntelligence(_bIsWantBaseValue,TRUE);

	int	iValue;

	if (m_wJob	==	dJOB_WEREWOLF)
	{
		iValue	=	m_intelligence.getCorrectedValue();

		if (m_intelligence.isFixedValue())
			return	max(iValue,1);
	}
	else
	{
		iValue	=	m_strength.getCorrectedValue();

		if	(m_strength.isFixedValue())
			return	max(iValue,1);
	}

	iValue	=	iValue*m_wBoostPower/100;

	int	iDeathPenelty	=	m_wDeathPenelty;

	if	(m_wAnimatePartnerTime)
		iDeathPenelty	=	0;

	if(m_wJob	==	dJOB_WEREWOLF)
		iValue		+=	m_bf4EatLightMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;
	else
		iValue		+=	m_bf4EatFireMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;

	return	max(iValue*(100-iDeathPenelty)/100,1);
}


int
CHero::getConstitution(BOOL _bIsWantBaseValue,BOOL _bIsAskByReversal)
{

	if(m_wTransformationTime)
		return m_wTransformationLevel;

	if (_bIsWantBaseValue)
	{
		if (m_wJob	==	dJOB_WEREWOLF)
			return	m_sCharisma;

		return	m_sConstitution;
	}

	int	iValue;

	if (m_wJob	==	dJOB_WEREWOLF)
	{
		iValue	=	m_charisma.getCorrectedValue();

		if (m_charisma.isFixedValue())
			return	max(iValue,1);
	}	
	else
	{
		iValue	=	m_constitution.getCorrectedValue();

		if (m_constitution.isFixedValue())
			return	max(iValue,1);
	}
	
	iValue	=	iValue*m_wBoostPower/100;

	int	iDeathPenelty	=	m_wDeathPenelty;

	if	(m_wAnimatePartnerTime)
		iDeathPenelty	=	0;

	if	(m_wJob	==	dJOB_WEREWOLF)
		iValue		+=	m_bf4EatDarkMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;
	else
		iValue		+=	m_bf4EatEarthMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;


	return	max(iValue*(100-iDeathPenelty)/100,1);
}


int
CHero::getIntelligence(BOOL _bIsWantBaseValue,BOOL _bIsAskByReversal)
{

	if(m_wTransformationTime)
		return m_wTransformationLevel;

	if	(m_wWantSummonedBeastIndex	!=	0xffff)
	{
		if	(m_wWantSummonedBeastIndex	>=	dOWN_SUMMON_BEAST_COUNT)
			return	0;

		return	m_aSummonBeast[m_wWantSummonedBeastIndex].m_wIntelligence;
	}

	if (_bIsWantBaseValue)
	{
		if	(m_wJob	==	dJOB_WEREWOLF)
			return	m_sStrength;

		return	m_sIntelligence;
	}

	if	(m_wReversalPowerTime	&&	_bIsAskByReversal	==	FALSE)
		return	getStrength(_bIsWantBaseValue,TRUE);

	int	iValue;

	if (m_wJob	==	dJOB_WEREWOLF)
	{
		iValue	=	m_strength.getCorrectedValue();

		if (m_strength.isFixedValue())
			return	max(iValue,1);
	}
	else
	{
		iValue	=	m_intelligence.getCorrectedValue();

		if (m_intelligence.isFixedValue())
			return	max(iValue,1);
	}

	iValue	=	iValue*m_wBoostPower/100;

	int	iDeathPenelty	=	m_wDeathPenelty;

	if	(m_wAnimatePartnerTime)
		iDeathPenelty	=	0;

	if(m_wJob	==	dJOB_WEREWOLF)
		iValue		+=	m_bf4EatFireMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;
	else
		iValue		+=	m_bf4EatLightMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;

	return	max(iValue*(100-iDeathPenelty)/100,1);
}


int
CHero::getCharisma(BOOL _bIsWantBaseValue,BOOL _bIsAskByReversal)
{

	if(m_wTransformationTime)
		return m_wTransformationLevel;

	if	(_bIsWantBaseValue)
	{
		if (m_wJob	==	dJOB_FALLEN_ANGEL)
			return	m_sCharisma+m_sCharisma/100;
		
		if (m_wJob	==	dJOB_WEREWOLF)
			return	m_sConstitution;

		return	m_sCharisma;
	}

	int	iValue;

	if	(m_wJob	==	dJOB_MAGICAL_GIRL)
	{
		iValue	=	m_charisma.getCorrectedValue();

		if	(m_charisma.isFixedValue())
			return	max(iValue,1);

		iValue	+=	m_luck.getCorrectedValue()/4;
	}
	else
	if	(m_wJob	==	dJOB_FALLEN_ANGEL)
	{
		iValue	=	m_charisma.getCorrectedValue()+m_charisma.getCorrectedValue()/100;

		if	(m_charisma.isFixedValue())
			return	max(iValue,1);
	}
	else
	if	(m_wJob	==	dJOB_WEREWOLF)
	{
		iValue	=	m_constitution.getCorrectedValue();

		if	(m_constitution.isFixedValue())
			return	max(iValue,1);
	}
	else
	{
		iValue	=	m_charisma.getCorrectedValue();

		if	(m_charisma.isFixedValue())
			return	max(iValue,1);
	}

	iValue	=	iValue*m_wBoostPower/100;

	int	iDeathPenelty	=	m_wDeathPenelty;

	if	(m_wAnimatePartnerTime)
		iDeathPenelty	=	0;


	if	(m_wJob	==	dJOB_WEREWOLF)
		iValue		+=	m_bf4EatEarthMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;
	else
		iValue		+=	m_bf4EatDarkMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;

	return	max(iValue*(100-iDeathPenelty)/100,1);
}


int
CHero::getWisdom(BOOL _bIsWantBaseValue,BOOL _bIsAskByReversal)
{

	if(m_wTransformationTime)
		return m_wTransformationLevel;

	if	(_bIsWantBaseValue)
	{
		if	(m_wJob	==	dJOB_FALLEN_ANGEL)
			return	m_sWisdom+m_sWisdom*3/100;
		
		if	(m_wJob	==	dJOB_WEREWOLF)
			return	m_sAgility;

		return	m_sWisdom;
	}

	if	(m_wReversalPowerTime	&&	_bIsAskByReversal	==	FALSE)
		return	getAgility(_bIsWantBaseValue,TRUE);

	int	iValue;

	if	(m_wJob	==	dJOB_FALLEN_ANGEL)
	{
		iValue	=	m_wisdom.getCorrectedValue()+m_wisdom.getCorrectedValue()*3/100;

		if	(m_wisdom.isFixedValue())
			return	max(iValue,1);
	}
	else
	if	(m_wJob	==	dJOB_WEREWOLF	)
	{
		iValue	=	m_agility.getCorrectedValue();

		if	(m_agility.isFixedValue())
			return	max(iValue,1);
	}
	else
	{
		iValue	=	m_wisdom.getCorrectedValue();

		if	(m_wisdom.isFixedValue())
			return	max(iValue,1);
	}

	iValue	=	iValue*m_wBoostPower/100;

	int	iDeathPenelty	=	m_wDeathPenelty;

	if	(m_wAnimatePartnerTime)
		iDeathPenelty	=	0;

	if	(m_wJob	==	dJOB_WEREWOLF)
		iValue		+=	m_bf4EatWindMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;
	else
		iValue		+=	m_bf4EatWaterMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;

	return	max(iValue*(100-iDeathPenelty)/100,1);
}

int
CHero::getAgility(BOOL _bIsWantBaseValue,BOOL _bIsAskByReversal)
{

	if(m_wTransformationTime)
		return m_wTransformationLevel;

	if (_bIsWantBaseValue)
	{
		if	(m_wJob	==	dJOB_WEREWOLF)
			return	m_sWisdom;

		return	m_sAgility;
	}

	if	(m_wReversalPowerTime	&&	_bIsAskByReversal	==	FALSE)
		return	getWisdom(_bIsWantBaseValue,TRUE);

	int	iValue;

	if (m_wJob	==	dJOB_WEREWOLF)
	{
		iValue	=	m_wisdom.getCorrectedValue();

		if	(m_wisdom.isFixedValue())
			return	max(iValue,1);
	}
	else
	{
		iValue	=	m_agility.getCorrectedValue();

		if	(m_agility.isFixedValue())
			return	max(iValue,1);
	}

	iValue	=	iValue*m_wBoostPower/100;

	int	iDeathPenelty	=	m_wDeathPenelty;

	if	(m_wAnimatePartnerTime)
		iDeathPenelty	=	0;

	if	(m_wJob	==	dJOB_WEREWOLF)
		iValue		+=	m_bf4EatWaterMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;
	else
		iValue		+=	m_bf4EatWindMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;

	return	max(iValue*(100-iDeathPenelty)/100,1);
}

int
CHero::getLuck(BOOL _bIsWantBaseValue,BOOL _bIsAskByReversal)
{


	if(m_wTransformationTime)
		return m_wTransformationLevel;

	if	(_bIsWantBaseValue)
		return	m_sLuck;

	int	iValue	=	m_luck.getCorrectedValue();

	if	(m_luck.isFixedValue())
		return	max(iValue,0);

	iValue	=	iValue*m_wBoostPower/100;

	int	iDeathPenelty	=	m_wDeathPenelty;

	if	(m_wAnimatePartnerTime)
		iDeathPenelty	=	0;

	return	max(iValue*(100-iDeathPenelty)/100,1);
}


int
CHero::getMaxCP()	//	최대 CP
{
	int	iAddCPByCH5	=	max(m_bf4CH5CPBonusCount-m_bf4CH5PeneltyCPBonusCount,0);
	iAddCPByCH5		=	iAddCPByCH5*c_iCH5BonusCPPerLevel;

	int	iMaxCP	=	m_maxCP.m_iValue+getCharisma()*100+getLevel()*100;
	iMaxCP		=	iMaxCP*(m_maxCP.getCorrectPercentageValue()+100+iAddCPByCH5)/100;
	iMaxCP		+=	m_maxCP.getCorrectValue()*100;

	iMaxCP		+=	iMaxCP*m_wBardTitleValue/100;

	return	max(iMaxCP,100);
}

int
CHero::getMaxHP()	//	최대 체력
{
	int	iAddHPByCH5	=	max(m_bf4CH5HPBonusCount-m_bf4CH5PeneltyHPBonusCount,0);
	iAddHPByCH5		=	iAddHPByCH5*c_iCH5BonusHPPerLevel;

	int	iMaxHP	=	m_maxHP.m_iValue+getConstitution()*m_wCorrectMaxHPFactor+getLevel()*m_wCorrectMaxHPConstitutionFactor;
	iMaxHP		=	iMaxHP*(m_maxHP.getCorrectPercentageValue()+100+iAddHPByCH5)/100;
	iMaxHP		+=	m_maxHP.getCorrectValue()*100;

	if	(m_wTeam	==	0)
	{
		int	iCorrectHPByGuildStategyPointPerFlag=	5;
		int	iCorrectHPByGuildStategyPoint		=	s_iRemainGuildStategyPoint*iCorrectHPByGuildStategyPointPerFlag;

		iMaxHP	+=	iMaxHP*iCorrectHPByGuildStategyPoint/100;
	}

	iMaxHP		+=	iMaxHP*m_wBardTitleValue/100;

	iMaxHP		+=	getValuePassiveSkill(eSKILL_PEE_INCREASE_MAX_HP);
	iMaxHP		+=	m_iCarvingOptionMaxHP;

	return	max(iMaxHP,100);
}

void
CHero::operatePassiveSkill()		// 패시브스킬들저장..
{
	int	iSlot = (m_wJob%2)*25;
	int iCount = 0;

	memset(m_aPassiveSkill,0xffff,sizeof(m_aPassiveSkill));		// 패시브스킬들.

	for(int i=iSlot;i<iSlot+25;++i)
	{
		cAbility	*lpAbility	=	&m_aAbility[i];
		if(!lpAbility)
			continue;
		CSkill* lpSkill = lpAbility->getSkill();
		if(!lpSkill)
			continue;
		if(lpSkill->m_wType == eSKILL_TYPE_PASSIVE)
		{
			this->m_aPassiveSkill[iCount++] = i;
		}

		if(iCount>=dPASSIVE_SKILL_COUNT)
			break;
	}	
	
}

int
CHero::getValuePassiveSkill(int _iKind, int _iValueIndex)
{
	int	iValue = 0;

	for(int i=0;i<dPASSIVE_SKILL_COUNT;++i)
	{
		if(m_aPassiveSkill[i] == 0xffff)
			break;
		if(m_aPassiveSkill[i] >= dABILITY_COUNT)
			continue;
		cAbility	*lpAbility	=	&m_aAbility[m_aPassiveSkill[i]];
		CSkill		*lpSkill	=	lpAbility->getSkill();
		
		if(!lpSkill)
			continue;
		if (lpSkill->m_aValue[0]	!=	_iKind)
			continue;
		if(lpAbility->m_wLevel == 0)
			break;

		int iLevel	= lpAbility->m_wLevel + getCorrectSkillLevel(lpAbility);	

		if(_iValueIndex == 1)
		{
			iValue += lpSkill->m_aValue[1] + lpSkill->m_aValue[2] * iLevel;
			if(lpSkill->m_aValue[3] != 0)
				iValue = min(iValue, lpSkill->m_aValue[3]);
		}
		else if(_iValueIndex == 2)
		{
			iValue += lpSkill->m_aValue[4] + lpSkill->m_aValue[5] * iLevel;
			if(lpSkill->m_aValue[6] != 0)
				iValue = min(iValue, lpSkill->m_aValue[6]);
		}
		break;
	}
	
	return	iValue;
}

int
CHero::getAfterRebirthLevel()
{
	if (m_iLevel < 600)
		return 0xffff;		
	
	__int64 i64SumExp = 0;      // 현재레벨 까지의 합산 경험치량
	__int64 i64SumExp600 = 0;	// 600 레벨 까지의 합산 경험치량

	for(int i=1 ; i<m_iLevel ; i++)
	{
		if (i < 600)
			i64SumExp600 += g_aExpTable[i];
		i64SumExp += g_aExpTable[i];
	}
	
	i64SumExp += m_iExperience;
	__int64 i64RebirthExp = i64SumExp - i64SumExp600;

	i64SumExp = 0;
	
	for(i=1 ; i<1000 ; i++)
	{
		i64SumExp += g_aExpTable[i];
		if (i64SumExp > i64RebirthExp)
			return i;
	}
	
	return 0xffff;
}


int
CHero::getCorrectSkillLevel(cAbility *_lpAbility)
{
	int	iLevel			=	m_correctAllSkillLevel.getCorrectedValue();

	if (_lpAbility)
	{
		CSkill	*lpSkill	=	_lpAbility->getSkill();

		if (lpSkill)
		{
			for (int i=4;i>=lpSkill->m_wDifficultyLevel-1;i--)
				iLevel	+=	m_aCorrectLevel[i].getCorrectedValue();

			if (lpSkill->m_bf1IsRightPunchSkill	)
				iLevel	+=	m_correctRightPunchSkillLevel.getCorrectedValue();
			if (lpSkill->m_bf1IsLeftPunchSkill	)
				iLevel	+=	m_correctLeftPunchSkillLevel.getCorrectedValue();
			if (lpSkill->m_bf1IsRightKickSkill	)
				iLevel	+=	m_correctRightKickSkillLevel.getCorrectedValue();
			if (lpSkill->m_bf1IsLeftKickSkill	)
				iLevel	+=	m_correctLeftKickSkillLevel.getCorrectedValue();
			if (lpSkill->m_bf1IsDownKickSkill	)
				iLevel	+=	m_correctDownKickSkillLevel.getCorrectedValue();
		}
	}

	return	iLevel;
}

DWORD
CHero::getCheckSum()
{
	DWORD	dwCheckSum	=	0;
	int		i;

	dwCheckSum+=m_iLevel;							//	레벨
	dwCheckSum+=m_iExperience;						//	경험치
	dwCheckSum+=m_dwSkillExperience;				//	스킬 경험치

	dwCheckSum+=m_wCorrectMaxHPFactor;				//	최대 HP 계산식에 필요한 HP 인자
	dwCheckSum+=m_wCorrectMaxHPConstitutionFactor;	//	최대 HP 계산식에 필요한 건강 인자

	dwCheckSum+=m_sStrength;						//	힘			5 1,000
	dwCheckSum+=m_sAgility;							//	민첩성		5 1,000
	dwCheckSum+=m_sConstitution;					//	건강		5 1,000
	dwCheckSum+=m_sWisdom;							//	지혜		5 1,000
	dwCheckSum+=m_sIntelligence;					//	지식		5 1,000
	dwCheckSum+=m_sCharisma;						//	카리스마	5 1,000
	dwCheckSum+=m_sLuck;							//	운			5 1,000
	dwCheckSum+=m_sSight;							//	시야

	dwCheckSum+=m_sMinDamage;						//	최소 데미지
	dwCheckSum+=m_sMaxDamage;						//	최대 데미지
	dwCheckSum+=m_sDefensivePower;					//	방어력

	dwCheckSum+=m_sAllignment;						//	성향 -100.00~100.00(absoulte evil,evil,neutral,good,absoulte good)

	dwCheckSum+=m_sFireResistance;					//	불 저항력 0% 100.00%  
	dwCheckSum+=m_sWaterResistance;					//	물 저항력 0% 100.00%  
	dwCheckSum+=m_sWindResistance;					//	바람 저항력 0% 100.00%  
	dwCheckSum+=m_sEarthResistance;					//	대지 저항력 0% 100.00%  
	dwCheckSum+=m_sLightResistance;					//	빛 저항력 0% 100.00%
	dwCheckSum+=m_sDarkResistance;					//	어둠 저항력 0% 100.00%

	dwCheckSum+=m_sBlindResistance;					//	장님 저항
	dwCheckSum+=m_sPoisonResistance;				//	중독 저항
	dwCheckSum+=m_sSleepResistance;					//	잠 저항
	dwCheckSum+=m_sColdResistance;					//	콜드 저항
	dwCheckSum+=m_sFreezeResistance;				//	프리즈 저항
	dwCheckSum+=m_sStunResistance;					//	경직 저항
	dwCheckSum+=m_sStoneResistance;					//	석화 저항
	dwCheckSum+=m_sConfuseResistance;				//	혼란 저항
	dwCheckSum+=m_sCharmingResistance;				//	챠밍 저항

	dwCheckSum+=m_sBadStatusResistance;				//	상태 이상공격에 대한 저항 0% 100.00%
	dwCheckSum+=m_sDeclinePowerResistance;			//	능력치 저하계에 대한 저항 0% 100.00%
	dwCheckSum+=m_sCurseResistance;					//	저주에 대한 저항 0% 100.00%

	dwCheckSum+=m_wProcessChapterIndex;
	dwCheckSum+=m_wProcessHighQuestIndex;

	for (i=0;i<strlen(m_strId);i++)
		dwCheckSum+=m_strId[i];
	for (i=0;i<strlen(m_strName);i++)
		dwCheckSum+=m_strName[i];

	dwCheckSum+=m_wJob;				//	현재 직업
	dwCheckSum+=m_iGold;			//	돈
	dwCheckSum+=m_wLevelPoint;		//	레벨업 포인트

	dwCheckSum+=m_wGuildSerial;
	dwCheckSum+=m_wGuildRank;
	dwCheckSum+=m_wBonusSkillPoint;

	dwCheckSum+=m_bf6ExtraInventorySize;

	for (i=0;i<dARBEIT_BUFFER_SIZE;i++)
	{
		dwCheckSum+=m_aCompleteArbeit[i];
		dwCheckSum+=m_aCompleteArbeit2[i];
	}
	for (i=0;i<dQUEST_BUFFER_SIZE;i++)
		dwCheckSum+=m_aCompleteQuest[i];

	dwCheckSum+=m_wBonusStatePoint;

	for (i=0;i<dABILITY_COUNT;i++)
	{
		dwCheckSum+=m_aAbility[i].m_wSkill;
		dwCheckSum+=m_aAbility[i].m_wLevel;
	}
	for (i=0;i<dMAX_PLAYER_TITLE_COUNT;i++)
	{
		dwCheckSum+=m_aTitle[i].m_bTitle;
		dwCheckSum+=m_aTitle[i].m_bLevel;
	}
	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		dwCheckSum+=m_aProcessQuest[i].isActive;
		dwCheckSum+=m_aProcessQuest[i].questKind;
		dwCheckSum+=m_aProcessQuest[i].questProcessLevel;
		dwCheckSum+=m_aProcessQuest[i].questIndex;
		dwCheckSum+=m_aProcessQuest[i].questValue;
		dwCheckSum+=m_aProcessQuest[i].isComplete;
	}
	for (i=0;i<dMAX_PLAYER_WAY_POINT_COUNT;i++)
	{
		dwCheckSum+=m_aWayPoint[i].m_bf10XPos;
		dwCheckSum+=m_aWayPoint[i].m_bf10YPos;
		dwCheckSum+=m_aWayPoint[i].m_bf12Field;
	}

	return	dwCheckSum;
}

int
CHero::getItemCountInInventory(int _iItem)
{
	int	i,iCount=0;

	for (i=0;i<m_wItemSlotCount;i++)
	{
		if	(m_aItems[i].m_wBaseItem	==	_iItem)
			iCount	+=	m_aItems[i].m_bCount;
	}
	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		if	(m_aExtraInventory[i].m_wBaseItem	==	_iItem)
			iCount	+=	m_aExtraInventory[i].m_bCount;
	}

	return	iCount;
}

int
CHero::getResetAbleStatePoint(int _iPower,int *_lpiCurrentPoint)
{
	CPlayerSaveData	player;

	int	iJob	=	m_wJob;

	if	(iJob == dJOB_WEREWOLF	||	iJob == dJOB_FALLEN_ANGEL	||	iJob	== dJOB_DEVIL	||	iJob	== dJOB_MAGICAL_GIRL)
		iJob--;

	cFILE	file;

	file.Open("data/defaultJob.dat","rb");
	file.Seek(iJob*sizeof(CPlayerSaveDataForServerDefine),SEEK_SET);
	file.Read(&player,sizeof(player));

	file.Close();

	CPlayerSaveData		*lpJob	=	&player;

	iJob	/=	2;

	int	iStrength		=	lpJob->m_sStrength;		//	힘			5 1,000
	int	iAgility		=	lpJob->m_sAgility;		//	민첩성		5 1,000
	int	iConstitution	=	lpJob->m_sConstitution;	//	건강		5 1,000
	int	iWisdom			=	lpJob->m_sWisdom;		//	지혜		5 1,000
	int	iIntelligence	=	lpJob->m_sIntelligence;	//	지식		5 1,000
	int	iCharisma		=	lpJob->m_sCharisma;		//	카리스마	5 1,000
	int	iLuck			=	lpJob->m_sLuck;			//	운			5 1,000

	for (int i=2;i<=m_iLevel;i++)
	{
//		힘	민첩성	건강	지식	지혜	카리스마	운	사용자
		iStrength		+=	g_aiLevelUpValue[iJob][0];
		iAgility		+=	g_aiLevelUpValue[iJob][1];
		iConstitution	+=	g_aiLevelUpValue[iJob][2];
		iIntelligence	+=	g_aiLevelUpValue[iJob][3];
		iWisdom			+=	g_aiLevelUpValue[iJob][4];
		iCharisma		+=	g_aiLevelUpValue[iJob][5];
		iLuck			+=	g_aiLevelUpValue[iJob][6];
	}

	switch(_iPower)
	{
		case	0	:	//"힘",
			*_lpiCurrentPoint	=	m_sStrength;
			return	m_sStrength-iStrength;
		case	1	:	//"민첩성",
			*_lpiCurrentPoint	=	m_sAgility;
			return	m_sAgility-iAgility;
		case	2	:	//"건강",
			*_lpiCurrentPoint	=	m_sConstitution;
			return	m_sConstitution-iConstitution;
		case	3	:	//"카리스마",
			*_lpiCurrentPoint	=	m_sCharisma;
			return	m_sCharisma-iCharisma;
		case	4	:	//"지식",
			*_lpiCurrentPoint	=	m_sIntelligence;
			return	m_sIntelligence-iIntelligence;
		case	5	:	//"지혜",
			*_lpiCurrentPoint	=	m_sWisdom;
			return	m_sWisdom-iWisdom;
		case	6	:	//"행운",
			*_lpiCurrentPoint	=	m_sLuck;
			return	m_sLuck-iLuck;
	}

	return	-1;
}


int
CHero::getResetAbleSkillPoint(int _iAbility,int *_lpiAbleLevel)
{
	CPlayerSaveData	player;

	int	iJob	=	m_wJob;

	if	(iJob == dJOB_WEREWOLF	||	iJob == dJOB_FALLEN_ANGEL	||	iJob	== dJOB_DEVIL	||	iJob	== dJOB_MAGICAL_GIRL)
		iJob--;

	cFILE	file;

	file.Open("data/defaultJob.dat","rb");
	file.Seek(iJob*sizeof(CPlayerSaveDataForServerDefine),SEEK_SET);
	file.Read(&player,sizeof(player));

	file.Close();


	int			iRequireLevel	=	0;
	cAbility	*lpAbility		=	&m_aAbility[_iAbility];
	cAbility	ability;
	CSkill		*lpSrcSkill		=	lpAbility->getSkill();

	if	(lpSrcSkill	==	NULL)
		return	0;

	memcpy(&ability,lpAbility,sizeof(cAbility));	

	for (int i=0;i<dABILITY_COUNT;i++)
	{
		if	(m_aAbility[i].m_wLevel	==	0)
			continue;

		CSkill	*lpSkill	=	m_aAbility[i].getSkill();

		if	(!lpSkill)
			continue;

		for (int j=0;j<dMAX_REQUIRE_SKILL_COUNT;j++)
		{
			if	(lpSkill->m_aRequireSkill[j].m_wSkill	==	lpSrcSkill->m_wSerial)
				iRequireLevel	=	max(iRequireLevel,lpSkill->m_aRequireSkill[j].m_wLevel);
		}
	}

	iRequireLevel	=	max(iRequireLevel,player.m_aAbility[_iAbility].m_wLevel);

	*_lpiAbleLevel	=	iRequireLevel;

	int	iDeltaLevel	=	ability.m_wLevel-iRequireLevel;
	int	iResetPoint	=	0;

	for (;iDeltaLevel;)
	{
		iDeltaLevel--;
		ability.m_wLevel--;
		iResetPoint	+=	ability.getRemainExpForLevelUp();
	}

	return	iResetPoint;
}

int
CHero::getExpBonusByA()
{
	int	iSlugger	=	-1;

	if	(g_iHeavenRedStoneCount	>	g_iHellRedStoneCount)
	{
		if	(m_sAllignment	>=	25)
		{
			return	m_sAllignment*100/20;
		}

		if	(m_sAllignment	<=	-75)
		{
			return	m_sAllignment*100/50;
		}
	}

	if	(g_iHellRedStoneCount	>	g_iHeavenRedStoneCount)
	{
		if	(m_sAllignment	>=	75)
		{
			return	-m_sAllignment*100/50;
		}
	}

	return	0;
}

int
CHero::getLightResistanceBonusByA()
{
	if	(g_iHeavenRedStoneCount	>	g_iHellRedStoneCount)
	{
		if	(m_sAllignment	>=	25)
		{
			return	m_sAllignment;
		}

		if	(m_sAllignment	<=	-50)
		{
			return	m_sAllignment*getRedDevilFactor()/100/10;
		}
	}

	if	(g_iHellRedStoneCount	>	g_iHeavenRedStoneCount)
	{
		if	(m_sAllignment	<=	-75)
		{
			return	-m_sAllignment*getRedDevilFactor()/100/4;
		}

		if	(m_sAllignment	>=	50)
		{
			return	-m_sAllignment*getRedDevilFactor()/100/10;
		}
	}

	return	0;
}

int
CHero::getFireResistanceBonusByA()
{
	if	(g_iHeavenRedStoneCount	>	g_iHellRedStoneCount)
	{
		if	(m_sAllignment	>=	50)
		{
			return	m_sAllignment/2;
		}
		if	(m_sAllignment	<=	-50)
		{
			return	m_sAllignment*getRedDevilFactor()/100/10;
		}
	}

	if	(g_iHellRedStoneCount	>	g_iHeavenRedStoneCount)
	{
		if	(m_sAllignment	<=	-50)
		{
			return	-m_sAllignment/2;
		}
		if	(m_sAllignment	>=	50)
		{
			return	-m_sAllignment*getRedDevilFactor()/100/10;
		}
	}

	return	0;
}

int
CHero::getWaterResistanceBonusByA()
{
	if	(g_iHeavenRedStoneCount	>	g_iHellRedStoneCount)
	{
		if	(m_sAllignment	>=	50)
		{
			return	m_sAllignment/2;
		}
		if	(m_sAllignment	<=	-50)
		{
			return	m_sAllignment*getRedDevilFactor()/100/10;
		}
	}

	if	(g_iHellRedStoneCount	>	g_iHeavenRedStoneCount)
	{
		if	(m_sAllignment	<=	-75)
		{
			return	-m_sAllignment*getRedDevilFactor()/100/4;
		}
		if	(m_sAllignment	>=	50)
		{
			return	-m_sAllignment*getRedDevilFactor()/100/10;
		}
	}
	return	0;
}

int
CHero::getWindResistanceBonusByA()
{
	if	(g_iHeavenRedStoneCount	>	g_iHellRedStoneCount)
	{
		if	(m_sAllignment	>=	75)
		{
			return	m_sAllignment*getRedDevilFactor()/100/4;
		}
		if	(m_sAllignment	<=	-50)
		{
			return	m_sAllignment*getRedDevilFactor()/100/10;
		}
	}

	if	(g_iHellRedStoneCount	>	g_iHeavenRedStoneCount)
	{
		if	(m_sAllignment	<=	-50)
		{
			return	-m_sAllignment/2;
		}
		if	(m_sAllignment	>=	50)
		{
			return	-m_sAllignment*getRedDevilFactor()/100/10;
		}
	}

	return	0;
}

int
CHero::getDarkResistanceBonusByA()
{
	if	(g_iHeavenRedStoneCount	>	g_iHellRedStoneCount)
	{
		if	(m_sAllignment	>=	75)
		{
			return	m_sAllignment*getRedDevilFactor()/100/4;
		}
		if	(m_sAllignment	<=	-50)
		{
			return	m_sAllignment*getRedDevilFactor()/100/10;
		}
	}

	if	(g_iHellRedStoneCount	>	g_iHeavenRedStoneCount)
	{
		if	(m_sAllignment	<=	-25)
		{
			return	-m_sAllignment;
		}
		if	(m_sAllignment	>=	50)
		{
			return	-m_sAllignment*getRedDevilFactor()/100/10;
		}
	}

	return	0;
}

int
CHero::getEarthResistanceBonusByA()
{
	if	(g_iHeavenRedStoneCount	>	g_iHellRedStoneCount)
	{
		if	(m_sAllignment	>=	75)
		{
			return	m_sAllignment*getRedDevilFactor()/100/4;
		}
		if	(m_sAllignment	<=	-50)
		{
			return	m_sAllignment*getRedDevilFactor()/100/10;
		}
	}

	if	(g_iHellRedStoneCount	>	g_iHeavenRedStoneCount)
	{
		if	(m_sAllignment	<=	-75)
		{
			return	-m_sAllignment*getRedDevilFactor()/100/4;
		}
		if	(m_sAllignment	>=	50)
		{
			return	-m_sAllignment*getRedDevilFactor()/100/10;
		}
	}

	return	0;
}

int
CHero::getResistanceBonusByA(int _iAttr)
{
	switch(_iAttr)
	{
		case	0	:
			return	getFireResistanceBonusByA();
		case	1	:
			return	getWaterResistanceBonusByA();
		case	2	:
			return	getWindResistanceBonusByA();
		case	3	:
			return	getEarthResistanceBonusByA();
		case	4	:
			return	getLightResistanceBonusByA();
		case	5	:
			return	getDarkResistanceBonusByA();
	}

	return	0;
}

int
CHero::getTradeBonusByA()
{
	if	(g_iHeavenRedStoneCount	>	g_iHellRedStoneCount)
	{
		if	(m_sAllignment	>=	50)
		{
			return	-10;
		}
	}

	if	(g_iHellRedStoneCount	>	g_iHeavenRedStoneCount)
	{
		if	(m_sAllignment	>=	50)
		{
			return	10;
		}
	}

	return	0;
}

int
CHero::getItemDropChanceBonusByA()
{
	if	(g_iHellRedStoneCount	>	g_iHeavenRedStoneCount)
	{
		if	(m_sAllignment	<=	-25)
		{
			return	-m_sAllignment;
		}
	}

	return	0;
}

int
CHero::getItemDropLevelBonusByA()
{
	if	(g_iHellRedStoneCount	>	g_iHeavenRedStoneCount)
	{
		if	(m_sAllignment	<=	-50)
		{
			double	dblValue	=	-m_sAllignment;

			return	(int)sqrt(dblValue)*getRedDevilFactor()/100;
		}
	}

	return	0;
}

int
CHero::getRedDevilFactor()
{
	int	iAverage	=	(g_iHeavenRedStoneCount	+	g_iHellRedStoneCount)/2;

	if	(g_iRedDevilRedStoneCount	==	0	||	g_iRedDevilRedStoneCount	<=	iAverage)
		return	100;

	if	(g_iRedDevilRedStoneCount <= iAverage+ iAverage*10/100)
	{
		return	110;
	}
	if	(g_iRedDevilRedStoneCount <= iAverage+ iAverage*25/100)
	{
		return	120;
	}
	if	(g_iRedDevilRedStoneCount <= iAverage+ iAverage*50/100)
	{
		return	140;
	}
	if	(g_iRedDevilRedStoneCount <= iAverage+ iAverage*75/100)
	{
		return	170;
	}
	if	(g_iRedDevilRedStoneCount <= iAverage+ iAverage)
	{
		return	200;
	}

	return	250;
}
