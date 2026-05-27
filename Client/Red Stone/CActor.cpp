#include <MATH.H>
#include "CActor.H"
#include "cMAP.H"
#include "CHitEffect.H"
#include "effectManager.H"
#include "CFieldItem.H"
#include "cSHAKE.H"
#include "cMESSAGE.H"
#include "CSound.H"
#include "arca.H"
#include "ast_chain_lightning.H"
#include "hero_mini_pet.H"

CActor		g_aActor[dMAX_ACTOR];		//	액터들..
CActor		*g_lpHero	=	NULL,*g_lpControlActor=NULL;
BOOL		g_isSmoothFocusing	=	FALSE;	//	주인공에 포커스를 맞출때 스무스~ 하게 맞춘다.
BOOL		g_isHeroFocusOn		=	TRUE;	//	포커스 맞추기 on

#define	dACT_ARCH_DEVIL_HOVER		(dACT_ACTION_1+2)
#define	dACT_ARCH_DEVIL_FLY			(dACT_ACTION_1+1)
#define	dACT_ARCH_DEVIL_CRAW		(dACT_ACTION_1+4)
#define	dACT_ARCH_DEVIL_THROW_STAFF	(dACT_ACTION_1+5)



CActor::CActor()
{
	reset();
}

//
//	플레이어 데이터로 데이터를 설정한다.
void
CActor::setDataByHero(CHero *_lpPlayer)
{
	cItem	*lpWeapon	=	_lpPlayer->getWeapon();
	cItem	*lpShield	=	_lpPlayer->getShield();
	cItem	*lpArmor	=	_lpPlayer->getEquipment(dEQUIP_ARMOR);

	if	(lpWeapon)
	{
		cItem	item;

		item.copy(lpWeapon);

		if	(lpWeapon->isPrincessWeapon())
			item.m_wBaseItem=	lpWeapon->getPrincessWeapon()->m_wBasicItem;

		m_itemWeapon.copy(&item,0);
	}
	else
		m_itemWeapon.reset();
	if	(lpShield)
		m_itemShield.copy(lpShield);
	else
		m_itemShield.reset();
	if	(lpArmor)
		m_itemArmor.copy(lpArmor);
	else
		m_itemArmor.reset();

	strcpy(m_strName,_lpPlayer->m_strName);

	m_wJob		=	_lpPlayer->m_wJob;
	m_wBody		=	getBodyIndex();
	m_wTeam		=	_lpPlayer->m_wTeam;
	m_wActorKind=	eAK_PLAYER;
	m_iHP		=	_lpPlayer->m_iHP;
	m_iMaxHP	=	max(_lpPlayer->getMaxHP(),100);

	m_pos.x		=	_lpPlayer->m_iXPos;
	m_pos.y		=	_lpPlayer->m_iYPos;
	m_pos.mx	=	m_pos.x>>	dTILE_XSIZE_SHIFT;
	m_pos.my	=	m_pos.y>>	dTILE_YSIZE_SHIFT;
	m_wSerial	=	_lpPlayer->m_iSerial;

	if (_lpPlayer->m_bf10Costume	!=	0)
	{
		m_wBody	=	_lpPlayer->m_bf10Costume;
		m_wJob	=	_lpPlayer->m_bf10Costume;
	}

	if (g_bIsHalfSize)
	{
		m_wVerticalScale=	50;
		m_wHorizonScale	=	50;
	}

	changeSyncWithServerStatus(eSSWS_COMPLETE);

	m_wIsReceivedBasicData	=	TRUE;

	setAnm(dACT_READY);

	setDirect(dDIRECT_S);

	checkEquipmentShape();
}

//
//	대기
void
CActor::buildByMinimumActorInfo(CMinimumActorInfo* _lpInfo)
{
	if	(m_wSerial	!=	0xffff)
		reset();	//	초기화가 안된 상태이다.

	strcpy(m_strName,"");

	if	(_lpInfo->bf1IsNpc)
		m_wNpcActorSerial	=	_lpInfo->wNpcActorSerial;
	else
		m_wNpcActorSerial	=	0xffff;

	m_wSerial	=	_lpInfo->bf11SerialInField;
	m_wTeam		=	_lpInfo->bf2Team;
	m_pos.x		=	_lpInfo->wX;
	m_pos.y		=	_lpInfo->wY;
	m_pos.mx	=	m_pos.x>>dTILE_XSIZE_SHIFT;
	m_pos.my	=	m_pos.y>>dTILE_YSIZE_SHIFT;

	changeSyncWithServerStatus(eSSWS_VALID_ACTOR);
}

void
CActor::changeMinipetType(int _iType1,int _iType2)
{
	{
		BOOL	bIsCreate	=	FALSE;

		//m_wMiniPetType2	=	_iType2;

		if	(_iType2	<	nsMiniPetType::Count	&&	_iType2	!=	m_wMiniPetType2	&&	_iType2	!=	m_wMiniPetType)
			bIsCreate	=	TRUE;

		m_wMiniPetType2	=	_iType2;

		if	(bIsCreate)
			addHitEffect(40,-50,g_im.m_wMiniPetSummon);

		setMiniPetAnm(dMINIPET_ACT_READY,1);
	}

	{
		BOOL	bIsCreate	=	FALSE;

		if	(_iType1	<	nsMiniPetType::Count	&&	m_wMiniPetType	!=	_iType1)
			bIsCreate	=	TRUE;

		m_wMiniPetType	=	_iType1;

		if	(bIsCreate)
			addHitEffect(-40,-50,g_im.m_wMiniPetSummon);

		setMiniPetAnm(dMINIPET_ACT_READY,0);
	}
}
BOOL
CActor::operateBouncingMissileSkill(SG_WIDE_AREA_DAMAGE *_lpPacket)
{
	cAbility	ability;

	ability.set(_lpPacket->bf12Skill,_lpPacket->bf8Level);
	m_attackInfo.m_ability.set(ability.m_wSkill,ability.m_wLevel);
	m_abilityUse.copy(&ability);
	WORD	awTargetList[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	memset(awTargetList, 0xffff, sizeof(awTargetList));
	for (int i=0;i<_lpPacket->bf6Count;i++)
	{
		cWIDE_AREA_DAMAGE_E	*lpInfo		=	&_lpPacket->aData[i];
		CHitInfo		*lpHitInfo		=	&m_attackInfo.m_aInfo[i];

		lpHitInfo->set(lpInfo->wPhysicalDamage *100,lpInfo->wMagicDamage *100,lpInfo->dwResultField);
		awTargetList[i]				=	lpInfo->wActor;
	}
	CActor*	lpTarget	=	NULL;
	if(_lpPacket->bf6Count)
		lpTarget	=	g_am.getTestedActor(awTargetList[0]);	

	if(!lpTarget)
		return FALSE;

	m_wUseSkill		=	g_sm.castBouncingMissileSkill(this,lpTarget,&ability,awTargetList,_lpPacket->bf6Count);
	
	if (isHero())
	{
		g_hero.decreaseBullet(&ability);
		g_hero.m_iCP	=	(int)_lpPacket->sRemainCP*100;	//	CP 증가
	}
	else
	{
		if (m_wUseSkill	==	0xffff)	
			g_am.addObscurityActor(m_wSerial);
	}
	m_iFPS		=	_lpPacket->bf7FPS;
	return	TRUE;
}

BOOL
CActor::operateWideMissileSkill(SG_WIDE_AREA_DAMAGE *_lpPacket)
{
	cAbility	ability;

	ability.set(_lpPacket->bf12Skill,_lpPacket->bf8Level);
	m_attackInfo.m_ability.set(ability.m_wSkill,ability.m_wLevel);
	m_abilityUse.copy(&ability);
	WORD	awTargetList[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	memset(awTargetList, 0xffff, sizeof(awTargetList));
	for (int i=0;i<_lpPacket->bf6Count;i++)
	{
		cWIDE_AREA_DAMAGE_E	*lpInfo		=	&_lpPacket->aData[i];
		CHitInfo		*lpHitInfo		=	&m_attackInfo.m_aInfo[i];

		lpHitInfo->set(lpInfo->wPhysicalDamage *100,lpInfo->wMagicDamage *100,lpInfo->dwResultField);
		memcpy(&m_attackInfo.m_aInfo[i],lpHitInfo, sizeof(CHitInfo)); 
		awTargetList[i]				=	lpInfo->wActor;
	}
	CActor*	lpTarget	=	NULL;
	if(_lpPacket->bf6Count)
		lpTarget	=	g_am.getTestedActor(awTargetList[0]);	

	if(!lpTarget)
		return FALSE;

	m_wUseSkill		=	g_sm.castWideMissileSkill(this,lpTarget,&ability,awTargetList,_lpPacket->bf6Count);
	
	if (isHero())
	{
		g_hero.decreaseBullet(&ability);
		g_hero.m_iCP	=	(int)_lpPacket->sRemainCP*100;	//	CP 증가
	}
	else
	{
		if (m_wUseSkill	==	0xffff)	
			g_am.addObscurityActor(m_wSerial);
	}
	m_iFPS		=	_lpPacket->bf7FPS;
	return	TRUE;
}


void
CActor::buildByVerySimplePlayerInfo(CVerySimplePlayerInfo *_lpInfo,BOOL _bIsAskActorInfoResult)
{
	//
	//로딩중이면 싱크를 마추지 않는다.으
	//변신시 단축키 변경되지 않는 버그수정을 위해서
	if(isHero() && m_wJob != _lpInfo->bf10Job && CGamePlay::s_iPreparationsForGameStatus == dPREPARATION_FOR_GAME_STATUS_INIT)
	{
		transformation();
	}

	m_wJob			=	_lpInfo->bf10Job;
	m_wTrueJob		=	_lpInfo->bf10Job;
	m_bf1IsHide		=	_lpInfo->bf1IsHide;
	m_bf1IsCopyActor=	_lpInfo->bf1IsCopyActor;
	m_wGuildPetType	=	_lpInfo->bf3GuildPetType;
	m_bf1IsSoundOfLedersBellsLeader	=	_lpInfo->bf1IsSoundOfLedersBellsLeader;	//	인도자의 종소리 울린 사람
	m_bf4RebirthCount				=	_lpInfo->bf4RebirthCount;	//	전생 횟수

// 	if	(_lpInfo->bf1IsUnionPartyUser && isPlayer())
// 	{
// 		s_unionPartyInfo.m_waWantupdateActor[s_unionPartyInfo.getFreeActorListIndex()] = m_wSerial;
// 	}
		
	changeMinipetType(_lpInfo->bf5MiniPetType,_lpInfo->bf5MiniPetType2);

	if	(isHero())
		if	(g_bIsDuelServer	==	FALSE && g_hero.m_wOperatorLevel < 2)
			m_bf1IsHide	=	FALSE;

	{
		if (_lpInfo->bf4Weapon	!=	dVAILD_EQUIPMENT_VALUE_FOR_SAI)
			m_itemWeapon.m_wBaseItem=	cBasicItem::GetItemByEquippedShape(_lpInfo->bf4Weapon,_lpInfo->bf10Job,dEQUIP_WEAPON);
		else
			m_itemWeapon.m_wBaseItem=	0xffff;

		if (_lpInfo->bf4Shield	!=	dVAILD_EQUIPMENT_VALUE_FOR_SAI)
			m_itemShield.m_wBaseItem=	cBasicItem::GetItemByEquippedShape(_lpInfo->bf4Shield,_lpInfo->bf10Job,dEQUIP_SHIELD_BULLET);
		else
			m_itemShield.m_wBaseItem=	0xffff;

		if (_lpInfo->bf2Armor	!=	dVAILD_EQUIPMENT_VALUE_FOR_ARMOR)
			m_itemArmor.m_wBaseItem	=	cBasicItem::GetItemByEquippedShape(_lpInfo->bf2Armor,_lpInfo->bf10Job,dEQUIP_ARMOR);
		else
			m_itemArmor.m_wBaseItem	=	0xffff;

		if (_lpInfo->bf8WeaponEffect	!=	0xff)
			m_itemWeapon.m_wColorizeEffect	=	_lpInfo->bf8WeaponEffect;
		else
			m_itemWeapon.m_wColorizeEffect	=	0xffff;

		m_itemArmor.m_wColorizeEffect	=	_lpInfo->bf3ArmorPalette;
	}

	m_wTeam				=	_lpInfo->bf2Team;
	m_wSerial			=	_lpInfo->bf11SerialInField;
	m_wVerticalScale	=	(WORD)_lpInfo->bf5Scale*10;
	m_wHorizonScale		=	(WORD)_lpInfo->bf5Scale*10;
	m_wOperatorLevel	=	_lpInfo->bf3OperatorLevel;
	m_dwEnchantedMask	=	_lpInfo->dwEnchantedMask;
	m_enchantedImage.copy(&_lpInfo->enchantedImage);
	m_wItemEffect		=	_lpInfo->wItemEffect;
	m_wIsEventMob		=	_lpInfo->bf1IsEventMob;
	m_wActorKind		=	eAK_PLAYER;
	m_wMagicCarpetType	=	_lpInfo->bf4MagicCarpetShape;

	if	(m_dwEnchantedMask	&	dAMEM_SHIMMERING_SHIELD)
	{
		m_wBlockerShape			=	_lpInfo->bf4BlockerShape;
		m_wBlockerAngle			=	random(360);
		m_wBlockerRigidityTime	=	0;
	}
	if	(m_dwEnchantedMask	&	dAMEM_SPEAR_BLOCKER)
	{
		m_wSpearBlockerShape	=	_lpInfo->bf4BlockerShape;
		m_wBlockerAngle			=	random(360);
		m_wBlockerRigidityTime	=	0;
	}

	if	(g_bIsHalfSize)
	{
		m_wVerticalScale/=	2;
		m_wHorizonScale	/=	2;
	}

	if	(m_wIsEventMob && isHero() == FALSE)
	{
		m_wTeam		=	1;
		m_wActorKind=	eAK_MONSTER;
	}

	if	(m_iMaxHP<=	0)
		m_iMaxHP=	25500;

	if	(_lpInfo->bf1IsDeath)
		m_iHP		=	0;
	else
	{
		if (isDeath())
		{
			m_wCorpseTime	=	0;
			setAnm(dACT_READY);
		}

		if (m_iHP	==	0)
			m_iHP	=	25500;
	}

	m_wBody			=	getBodyIndex();

	changeSyncWithServerStatus(eSSWS_COMPLETE);

	m_wIsReceivedBasicData	=	TRUE;

	if (m_iHP	<=	0)
	{
		kill(TRUE);
		setPos(_lpInfo->wX,_lpInfo->wY);
	}
	else
	{
		if (_bIsAskActorInfoResult)
		{
			//	좌표차이가 크지 않다면 좌표는 재설정하지 않는다.
			if (m_iAnm != dACT_READY && m_iFPS != 0)
			{
				if (GetOvalRange(_lpInfo->wX,_lpInfo->wY,m_pos.x,m_pos.y)	>=	dRESET_RANGE_BY_MOVE_PACKET)	
					setPos(_lpInfo->wX,_lpInfo->wY);
			}
			else
			{
				setAnm(dACT_READY,TRUE);
	///			setDirect(_lpInfo->bf3Direct);
				setPos(_lpInfo->wX,_lpInfo->wY);
			}
		}
		else
		{
			setPos(_lpInfo->wX,_lpInfo->wY);

			setAnm(dACT_READY,TRUE);

			setDirect(_lpInfo->bf3Direct);
		}

		if (_lpInfo->dwEnchantedMask&dAMEM_IS_SITDOWN)
		{
			sitDown(_lpInfo->wX,_lpInfo->wY,TRUE);
			setDirect(_lpInfo->bf3Direct);
		}
	}

	checkEquipmentShape();

	if (_lpInfo->bf2RideDog)
	{
		m_wRideDog	=	(WORD)(_lpInfo->bf2RideDog-1);

		setAnm(dACT_READY,TRUE);
	}
	else
		m_wRideDog	=	0xffff;

	m_wNpcActorSerial	=	0xffff;
}

void
CActor::buildByVerySimpleMonsterInfo(CVerySimpleMonsterInfo *_lpInfo,BOOL _bIsAskActorInfoResult)
{
	m_wVerticalScale=	(WORD)_lpInfo->bf8Scale;
	m_wHorizonScale	=	(WORD)_lpInfo->bf8Scale;
	m_wSerial		=	_lpInfo->bf11SerialInField;
	m_wJob			=	_lpInfo->bf11Job;
	m_wTrueJob		=	_lpInfo->bf11Job;
	m_wIsPet		=	_lpInfo->bf1IsPet;
	m_wIsRideTamer	=	_lpInfo->bf1RideTamer;
	m_bf1IsCopyActor=	_lpInfo->bf1IsCopyActor;
	m_bf1IsBoss		=	_lpInfo->bf1IsBoss;
	m_bf1IsHideName	=	_lpInfo->bf1IsHideName;
	m_wGuildPetType	=	0;

	if	(g_bIsHalfSize)
	{
		m_wVerticalScale/=	2;
		m_wHorizonScale	/=	2;
	}

	if (m_wJob		>=	dJOB_BORDER)
		m_wJob		=	g_jm.m_aJob[m_wJob].m_iBody;

	m_wActorKind	=	eAK_MONSTER;
	m_wTeam			=	_lpInfo->bf2Team;
	m_wOperatorLevel=	0;
	m_dwEnchantedMask=	_lpInfo->dwEnchantedMask;
	m_enchantedImage.copy(&_lpInfo->enchantedImage);

	m_wBody			=	getBodyIndex();

	changeSyncWithServerStatus(eSSWS_COMPLETE);
	m_wIsReceivedBasicData	=	TRUE;

	if (m_iMaxHP==	0)
		m_iMaxHP=	25500;

	if (_lpInfo->bf1IsDeath)
		m_iHP		=	0;
	else
	{
		if (m_iHP	==	0)
			m_iHP	=	25500;
	}

	if (m_iHP	<=	0)
	{
		setPos(_lpInfo->wX,_lpInfo->wY);
		kill(TRUE);
	}
	else
	if (_bIsAskActorInfoResult)
	{
		//	좌표차이가 크지 않다면 좌표는 재설정하지 않는다.
		if (m_iAnm != dACT_READY && m_iFPS != 0)
		{
			if (GetOvalRange(_lpInfo->wX,_lpInfo->wY,m_pos.x,m_pos.y)	>=	dRESET_RANGE_BY_MOVE_PACKET)	
				setPos(_lpInfo->wX,_lpInfo->wY);
		}
		else
		{
			setAnm(dACT_READY,TRUE);
//			setDirect(_lpInfo->bf3Direct);
			setPos(_lpInfo->wX,_lpInfo->wY);
		}
	}
	else
	{
		setPos(_lpInfo->wX,_lpInfo->wY);

		setAnm(dACT_READY,TRUE);
		setDirect(_lpInfo->bf3Direct);
	}

	checkEquipmentShape();
	m_wNpcActorSerial	=	0xffff;
}

void
CActor::buildByVerySimpleNpcInfo(CVerySimpleNpcInfo *_lpInfo,BOOL _bIsAskActorInfoResult)
{
	m_wVerticalScale=	(WORD)_lpInfo->bf8Scale;
	m_wHorizonScale	=	(WORD)_lpInfo->bf8Scale;
	m_wSerial		=	_lpInfo->bf11SerialInField;
	m_wJob			=	_lpInfo->bf11Job;
	m_wTrueJob		=	_lpInfo->bf11Job;
	m_bf1IsTalker	=	_lpInfo->bf1IsTalker;
	m_bf1IsCopyActor=	FALSE;
	m_wGuildPetType	=	0;
	m_bf1IsHideName	=	_lpInfo->bf1IsHideName;

	if (g_bIsHalfSize)
	{
		m_wVerticalScale/=	2;
		m_wHorizonScale	/=	2;
	}

	if (m_wJob		>=	dJOB_BORDER)
		m_wJob		=	g_jm.m_aJob[m_wJob].m_iBody;

	m_wActorKind	=	_lpInfo->bf6ActorKind;
	m_wTeam			=	_lpInfo->bf2Team;
	
	m_wOperatorLevel=	0;
	m_dwEnchantedMask=	0;
	m_enchantedImage.reset();

	m_iMaxHP		=	100;
	m_iHP			=	100;
	m_wBody			=	getBodyIndex();

	changeSyncWithServerStatus(eSSWS_COMPLETE);

	m_wIsReceivedBasicData	=	TRUE;

	if (_bIsAskActorInfoResult)
	{
		//	좌표차이가 크지 않다면 좌표는 재설정하지 않는다.
		if (m_iAnm != dACT_READY && m_iFPS != 0)
		{
			if (GetOvalRange(_lpInfo->wX,_lpInfo->wY,m_pos.x,m_pos.y)	>=	dRESET_RANGE_BY_MOVE_PACKET)	
				setPos(_lpInfo->wX,_lpInfo->wY);
		}
		else
		{
			setAnm(dACT_READY,TRUE);
			setDirect(_lpInfo->bf3Direct);
			setPos(_lpInfo->wX,_lpInfo->wY);
		}
	}
	else
	{
		setPos(_lpInfo->wX,_lpInfo->wY);

		setAnm(dACT_READY,TRUE);

		setDirect(_lpInfo->bf3Direct);
	}
	checkEquipmentShape();
}

void
CActor::buildBySimplePlayerInfo(CSimplePlayerInfo *_lpInfo)
{
	m_iMaxHP			=	_lpInfo->bf16MaxHp*100;
	LONGLONG	llHP	=	m_iMaxHP;
	llHP				*=	_lpInfo->bf8RestHp;
	llHP				/=	255;
	m_iHP				=	(int )llHP;
	m_wJob				=	_lpInfo->bf10Job;
	m_wTrueJob			=	_lpInfo->bf10Job;
	m_wGuildSerial		=	_lpInfo->bf12GuildSerial;
	m_iFirstAidHP		=	_lpInfo->bf14FirstAidHP*100;
	m_wMagicCarpetType	=	_lpInfo->bf5MagicCarpetShape;
	m_bf1IsHide			=	_lpInfo->bf1IsHide;
	m_bf1IsCopyActor	=	_lpInfo->bf1IsCopyActor;
	m_wGuildPetType		=	_lpInfo->bf3GuildPetType;
	m_bf1IsSoundOfLedersBellsLeader	=	_lpInfo->bf1IsSoundOfLedersBellsLeader;	//	인도자의 종소리 울린 사람
	m_bf4RebirthCount				=	_lpInfo->bf4RebirthCount;	//	전생 횟수

// 	if (_lpInfo->bf1IsUnionPartyUser && isPlayer() )
// 	{
// 		s_unionPartyInfo.m_waWantupdateActor[s_unionPartyInfo.getFreeActorListIndex()] = m_wSerial;
// 	}

	changeMinipetType(_lpInfo->bf5MiniPetType,_lpInfo->bf5MiniPetType2);

	if	(isHero())
		if	(g_bIsDuelServer	==	FALSE && g_hero.m_wOperatorLevel < 2)
			m_bf1IsHide	=	FALSE;

	if	(m_wGuildSerial	>	4000)
		m_wGuildSerial	=	0xffff;

	{
		if (_lpInfo->bf4Weapon	!=	dVAILD_EQUIPMENT_VALUE_FOR_SAI)
			m_itemWeapon.m_wBaseItem=	cBasicItem::GetItemByEquippedShape(_lpInfo->bf4Weapon,_lpInfo->bf10Job,dEQUIP_WEAPON);
		else
			m_itemWeapon.m_wBaseItem=	0xffff;

		if (_lpInfo->bf4Shield	!=	dVAILD_EQUIPMENT_VALUE_FOR_SAI)
			m_itemShield.m_wBaseItem=	cBasicItem::GetItemByEquippedShape(_lpInfo->bf4Shield,_lpInfo->bf10Job,dEQUIP_SHIELD_BULLET);
		else
			m_itemShield.m_wBaseItem=	0xffff;

		if (_lpInfo->bf2Armor	!=	dVAILD_EQUIPMENT_VALUE_FOR_ARMOR)
			m_itemArmor.m_wBaseItem	=	cBasicItem::GetItemByEquippedShape(_lpInfo->bf2Armor,_lpInfo->bf10Job,dEQUIP_ARMOR);
		else
			m_itemArmor.m_wBaseItem	=	0xffff;

		if (_lpInfo->bf8WeaponEffect	!=	0xff)
				m_itemWeapon.m_wColorizeEffect	=	_lpInfo->bf8WeaponEffect;
		else	m_itemWeapon.m_wColorizeEffect	=	0xffff;

		m_itemArmor.m_wColorizeEffect	=	_lpInfo->bf3ArmorPalette;
	}

	m_wTeam			=	_lpInfo->bf2Team;
	m_wSerial		=	_lpInfo->bf11SerialInField;
	m_wVerticalScale=	(WORD)_lpInfo->bf5Scale*10;
	m_wHorizonScale	=	(WORD)_lpInfo->bf5Scale*10;
	m_wOperatorLevel=	_lpInfo->bf3OperatorLevel;
	m_wLevel		=	_lpInfo->bf10Level;
	m_wIsEventMob	=	_lpInfo->bf1IsEventMob;
	m_wActorKind	=	eAK_PLAYER;

	if (g_bIsHalfSize)
	{
		m_wVerticalScale/=	2;
		m_wHorizonScale	/=	2;
	}

	if (m_wIsEventMob && isHero() == FALSE)
	{
		m_wTeam		=	1;
		m_wActorKind=	eAK_MONSTER;
	}

	m_dwEnchantedMask=	_lpInfo->dwEnchantedMask;
	m_enchantedImage.copy(&_lpInfo->enchantedImage);
	m_wItemEffect	=	_lpInfo->wItemEffect;

	if	(m_dwEnchantedMask	&	dAMEM_SHIMMERING_SHIELD)
	{
		m_wBlockerShape			=	_lpInfo->bf4BlockerShape;
		m_wBlockerAngle			=	random(360);
		m_wBlockerRigidityTime	=	0;
	}
	if	(m_dwEnchantedMask	&	dAMEM_SPEAR_BLOCKER)
	{
		m_wSpearBlockerShape	=	_lpInfo->bf4BlockerShape;
		m_wBlockerAngle			=	random(360);
		m_wBlockerRigidityTime	=	0;
	}

	m_wBody			=	getBodyIndex();

	changeSyncWithServerStatus(eSSWS_COMPLETE);
	m_wIsReceivedBasicData	=	TRUE;

	if	(m_iHP	<=	0)	
	{
		kill(TRUE);
		setPos(_lpInfo->wX,_lpInfo->wY);
	}
	else
	{
		if (m_iAnm != dACT_READY && m_iFPS != 0)
		{
			if (GetOvalRange(_lpInfo->wX,_lpInfo->wY,m_pos.x,m_pos.y)	>=	dRESET_RANGE_BY_MOVE_PACKET)	
				setPos(_lpInfo->wX,_lpInfo->wY);
		}
		else
		{
			setAnm(dACT_READY,TRUE);
			setPos(_lpInfo->wX,_lpInfo->wY);
		}

		if (_lpInfo->dwEnchantedMask&dAMEM_IS_SITDOWN)
		{
			sitDown(_lpInfo->wX,_lpInfo->wY,TRUE);
			setDirect(_lpInfo->bf3Direct);
		}

		if	(_lpInfo->bf2RideDog)
		{
			m_wRideDog	=	(WORD)(_lpInfo->bf2RideDog-1);
			setAnm(dACT_READY,TRUE);
		}
		else
			m_wRideDog	=	0xffff;
	}

	strcpy(m_strName,_lpInfo->strName);
	checkEquipmentShape();

	if	(isHero())
		g_hero.m_iHP	=	m_iHP;
	m_wNpcActorSerial	=	0xffff;
}

void
CActor::buildBySimpleMonsterInfo(CSimpleMonsterInfo *_lpInfo)
{
	m_wVerticalScale=	(WORD)_lpInfo->bf8Scale;
	m_wHorizonScale	=	(WORD)_lpInfo->bf8Scale;
	m_wSerial		=	_lpInfo->bf11SerialInField;
	m_wLevel		=	_lpInfo->bf10Level;
	m_wGuildPetType	=	0;
	m_bf1IsHideName	=	_lpInfo->bf1IsHideName;

	if (g_bIsHalfSize)
	{
		m_wVerticalScale/=	2;
		m_wHorizonScale	/=	2;
	}

	m_wJob			=	_lpInfo->bf11Job;
	m_wTrueJob		=	_lpInfo->bf11Job;
	m_wIsPet		=	_lpInfo->bf1IsPet;
	m_wIsRideTamer	=	_lpInfo->bf1RideTamer;
	m_wTamer		=	_lpInfo->bf11Tamer;
	m_bf1IsCopyActor=	_lpInfo->bf1IsCopyActor;
	m_bf1IsBoss		=	_lpInfo->bf1IsBoss;

	if	(m_wJob		>=	dJOB_BORDER)
		m_wJob		=	g_jm.m_aJob[m_wJob].m_iBody;

	m_wActorKind	=	eAK_MONSTER;
	m_wTeam			=	_lpInfo->bf2Team;
	
	m_wOperatorLevel=	0;

	m_iMaxHP		=	_lpInfo->bf24MaxHp*100;
	LONGLONG	llHP=	m_iMaxHP;
	llHP			*=	_lpInfo->bf8RestHp;
	llHP			/=	255;
	m_iHP			=	(int )llHP;
	m_dwEnchantedMask=	_lpInfo->dwEnchantedMask;
	m_enchantedImage.copy(&_lpInfo->enchantedImage);

	{
		LONGLONG	llHP=	m_iMaxHP;
		m_iFirstAidHP	=	llHP*_lpInfo->bf10FirstAidHP/1000;
	}

	m_wBody			=	getBodyIndex();

	changeSyncWithServerStatus(eSSWS_COMPLETE);
	m_wIsReceivedBasicData	=	TRUE;

	if (m_iHP	<=	0)	
	{
		setDirect(_lpInfo->bf3Direct);
		kill(TRUE);
		setPos(_lpInfo->wX,_lpInfo->wY);
	}
	else
	{
		if (m_iAnm != dACT_READY && m_iFPS != 0)
		{
			if (GetOvalRange(_lpInfo->wX,_lpInfo->wY,m_pos.x,m_pos.y)	>=	dRESET_RANGE_BY_MOVE_PACKET)	
				setPos(_lpInfo->wX,_lpInfo->wY);
		}
		else
		{
			setAnm(dACT_READY,TRUE);
			setPos(_lpInfo->wX,_lpInfo->wY);
		}
	}

	if (isSummonBeast())
	{
		if (_lpInfo->bf1IsRegen)	//	이건... 소환수가 파워업
		{
			m_wWhiteFadeStep=	1;

			int	iGrade		=	(m_wJob-dJOB_KELBY_1)%dSUMMON_BEAST_GRADE_COUNT-1;

			iGrade			=	max(iGrade,0);
			iGrade			=	min(iGrade,1);

			addHitEffect(0,0,g_im.m_wSummonBeastPowerUp1+iGrade);
		}
		strcpy(m_strName,getJob()->m_strName);
	}
	else
		strcpy(m_strName,_lpInfo->strName);

	checkEquipmentShape();
	m_wNpcActorSerial	=	0xffff;
}

void
CActor::buildBySimpleNpcInfo(CSimpleNpcInfo *_lpInfo)
{
	m_wVerticalScale=	(WORD)_lpInfo->bf8Scale;
	m_wHorizonScale	=	(WORD)_lpInfo->bf8Scale;
	m_wSerial		=	_lpInfo->bf11SerialInField;
	m_bf1IsTalker	=	_lpInfo->bf1IsTalker;
	m_bf1IsCopyActor=	FALSE;
	m_wGuildPetType	=	0;
	m_bf1IsHideName	=	_lpInfo->bf1IsHideName;

	if	(g_bIsHalfSize)
	{
		m_wVerticalScale/=	2;
		m_wHorizonScale	/=	2;
	}

	m_wJob			=	_lpInfo->bf11Job;
	m_wTrueJob		=	_lpInfo->bf11Job;

	if (m_wJob		>=	dJOB_BORDER)
		m_wJob		=	g_jm.m_aJob[m_wJob].m_iBody;

	m_wActorKind	=	_lpInfo->bf6ActorKind;
	m_wTeam			=	_lpInfo->bf2Team;

	m_wOperatorLevel=	0;

	m_iMaxHP		=	100;
	m_iHP			=	100;
	m_dwEnchantedMask=	0;
	m_enchantedImage.reset();

	m_wBody			=	getBodyIndex();

	changeSyncWithServerStatus(eSSWS_COMPLETE);
	m_wIsReceivedBasicData	=	TRUE;

	if (m_iHP	<=	0)	
	{
		kill();
		setPos(_lpInfo->wX,_lpInfo->wY);
	}
	else
	{
		if (m_iAnm != dACT_READY && m_iFPS != 0)
		{
			if (GetOvalRange(_lpInfo->wX,_lpInfo->wY,m_pos.x,m_pos.y)	>=	dRESET_RANGE_BY_MOVE_PACKET)	
				setPos(_lpInfo->wX,_lpInfo->wY);
		}
		else
		{
			setAnm(dACT_READY,TRUE);
			setDirect(_lpInfo->bf3Direct);
			setPos(_lpInfo->wX,_lpInfo->wY);
		}
	}

	strcpy(m_strName,_lpInfo->strName);
	checkEquipmentShape();
}

//
//아바타 정보로 액터 데이터 설정.. -_-a
BOOL
CActor::setDataByAvatar(cAVATAR_INFO *_lpAvatar)
{
	if	(_lpAvatar->m_wJob	>	dPLAYER_JOB_COUNT)
		return	FALSE;

	if	(_lpAvatar->m_wAvatarIndex	>=	dMAX_AVATAR_COUNT)
		return	FALSE;

	if	(strlen(_lpAvatar->m_strName)>	dNAME_LENGTH-1)
		return	FALSE;

	if	(_lpAvatar->m_wLevel	>	1000	)
		return	FALSE;

	if	(_lpAvatar->m_wWeapon	!=	0xffff	&&	_lpAvatar->m_wWeapon	>=	dBASIC_ITEM_COUNT)
		return	FALSE;
	if	(_lpAvatar->m_wShield	!=	0xffff	&&	_lpAvatar->m_wShield	>=	dBASIC_ITEM_COUNT)
		return	FALSE;
	if	(_lpAvatar->m_wArmor	!=	0xffff	&&	_lpAvatar->m_wArmor		>=	dBASIC_ITEM_COUNT)
		return	FALSE;

	m_itemWeapon.m_wBaseItem	=	_lpAvatar->m_wWeapon;
	m_itemShield.m_wBaseItem	=	_lpAvatar->m_wShield;
	m_itemArmor.m_wBaseItem		=	_lpAvatar->m_wArmor;

	m_wSerial	=	0;
	m_wJob		=	_lpAvatar->m_wJob;
	m_wBody		=	getBodyIndex();

	if	(m_wBody	>=	dBODY_HERO_JOB_END)
		return	FALSE;

	setAnm(dACT_READY);
	setDirect(dDIRECT_S);

	return	TRUE;
}

//
//	캐릭터 데이터 초기화
void
CActor::reset()
{
	//	액터에 관한 정보들 리셋
	{
		cACTOR_DATA::reset();
	}	//	액터에 관한 정보들 리셋

	memset(m_aHitEffect,0xff,sizeof(m_aHitEffect));

	m_wAuraImage			=	0xffff;
	m_wMoveSpeed			=	100;
	m_wActionSpeed			=	100;
	m_wMiniPetType			=	nsMiniPetType::Count;
	m_wMiniPetType2			=	nsMiniPetType::Count;
	m_wBlockerAngle			=	0xffff;
	m_wIsCastBlocker		=	FALSE;
	m_wMachineSkill			=	0xffff;
	m_wMachineAngle			=	0xffff;
	m_wIntervalShooter		=	0xffff;

	m_iDirect				=	0;
	m_wUseSkill				=	0xffff;

	m_wHillSkill			=	0xffff;		//	언덕 높이
	m_wLordActor			=	0xffff;		//	쥔장
	m_wSlaveActor			=	0xffff;
	m_wGuildSerial			=	0xffff;
	m_wBlockerShape			=	0xffff;
	m_wSpearBlockerShape	=	0xffff;

	m_abilityUse.reset();

//	m_flying_bow.set_current_parent_pos(400,200 );
	m_flying_bow.set_mode(2);
	m_flying_bow.set_mass( 180 );
	m_flying_bow.set_friction( 0.45f );
	m_flying_bow.set_elasticity( 80.0f );
	m_flying_bow.set_rotate_mass ( 5.0f );
	m_flying_bow.set_rotate_friction ( 0.3f );
	m_flying_bow.set_rotate_elasticity ( 1.5f );
}

//
//	시야 밖에 있는 액터를 위한 초기화
BOOL
CActor::resetForOutOfSightActor()
{
	if (m_wCorpseTime)
	{
		m_wCorpseTime	=	0;

		if (!isPlayer())
		{
			g_am.removeActor(m_wSerial);

			return TRUE;
		}
	}

	for (int i=0;i<dHIT_EFFECT_COUNT_ON_CHARACTER;i++)
	{
		if (m_aHitEffect[i]	==	0xffff)
			continue;

		g_hem.remove(m_aHitEffect[i]);

		m_aHitEffect[i]		=	0xffff;
	}

	stop();

	m_attackInfo.m_wTarget	=	0xffff;
	m_wStopDirect			=	0xffff;
	m_wChargeImage			=	0xffff;
	m_wAuraImage			=	0xffff;
	m_wOverlapAnm			=	0xffff;
	m_wOverlapAnm2			=	0xffff;
	m_wIsCastBlocker		=	FALSE;
	m_wBlockerAngle			=	0xffff;

	m_wMoveSpeed			=	100;
	m_wActionSpeed			=	100;

	m_wMachineSkill			=	0xffff;
	m_wMachineAngle			=	0xffff;
	m_wIntervalShooter		=	0xffff;
	m_wTargetMark			=	0xffff;

	if (m_wHillSkill		!=	0xffff)
	{
		g_sm.remove(m_wHillSkill);

		m_wHillSkill		=	0xffff;
	}

	m_abilityUse.reset();

	changeSyncWithServerStatus(eSSWS_OUT_OF_SIGHT);

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	데이터 초기화,설정,추가,삭제
//////////////////////////////////////////////////////////////////////////////////////////
//
//	죽인다.
void
CActor::kill(BOOL _bIsCorpse,BOOL _bIsReleaseSummon)
{
	{
		cPet	*lpPet	=	g_hero.getPet(g_hero.getPetIndex(m_wSerial));

		if (lpPet)
			lpPet->m_wEngageTarget	=	0xffff;
	}
	{
		cSummonBeast	*lpSummonBeast	=	g_hero.getSummonBeast(g_hero.getSummonBeastIndex(m_wSerial));

		if (lpSummonBeast)
			lpSummonBeast->m_wEngageTarget	=	0xffff;
	}
	
	m_wPetWaitCommand	=	FALSE;

	m_attackInfo.reset();

	if	(isDeath())
		return;

	if	(isDummy())
	{
		m_wDummyStatus	=	dDISAPEAR_DUMMY;

		return;
	}

	if (m_wRideDog	!=	0xffff)
	{
		CActor	*lpKelby	=	g_am.getActor(m_wRideDog);

		if	(lpKelby)
			lpKelby->m_wIsRideTamer	=	FALSE;

		m_wRideDog			=	0xffff;
	}

	{
		m_iHP				=	0;
		m_wMachineSkill		=	0xffff;
		m_wMachineAngle		=	0xffff;
		m_wIsHideShield		=	FALSE;
		m_dwEnchantedMask	=	0;
		m_enchantedImage.reset();
		m_wHillSkill		=	0xffff;
		m_wTargetMark		=	0xffff;

		m_wIsHideShield		=	FALSE;
		m_wMagicCarpetType	=	0;
	}

	if (isHero())
	{
		g_hero.m_iHP					=	0;
		g_hero.m_iRecoverHP				=	0;
		g_hero.m_iForcedRecoverHP		=	0;
		g_hero.m_iWaitMoveResultTime	=	0;
		g_hero.m_bf1IsHideWeapon1		=	FALSE;
		g_hero.m_bf1IsHideWeapon2		=	FALSE;
		g_hero.m_wMaintainMaxCPTime		=	0;
		g_hero.m_wCPFloatTime			=	0;
		g_hero.m_isWalking				=	FALSE;

		g_hero.resetTemporaryMagicEffect();
	}

	stop();

	if	(isNpc())
	{
		m_wCorpseTime	=	dSYNC_FPS*2;
		return;
	}
	setAnm(dACT_DEAD);

	if (_bIsCorpse)
		m_iFrame	=	m_iFrameCount-1;

	setExclusiveAction(TRUE);

	if	(isMonster() && m_wBody >= dJOB_MONSTER_START && m_wBody <= dJOB_MONSTER_WHITE_SHADOW)
	{
		if	(g_rectViewPort.isIn(m_pos.x,m_pos.y))
			g_esm.playMonsterDeathSound(m_wBody-dJOB_MONSTER_START);
	}
	
	if	(m_wHillSkill		!=	0xffff)
	{
		g_sm.remove(m_wHillSkill);

		m_wHillSkill		=	0xffff;
	}

	m_wCorpseTime			=	0xffff;
	m_wIsReleaseSummonBeast	=	_bIsReleaseSummon;

	m_wBlockerShape			=	0xffff;
	m_wSpearBlockerShape	=	0xffff;
}

//
//	캐릭터를 제거한다.
BOOL
CActor::remove()
{
	for (int i=0;i<dHIT_EFFECT_COUNT_ON_CHARACTER;i++)
	{
		if (m_aHitEffect[i]	==	0xffff)	continue;

		g_hem.remove(m_aHitEffect[i]);
	}

	reset();

	return TRUE;
}

//
//	잔상들을 초기화 시킨다.
void
CActor::resetAfterImage()
{
/*	for (int i=0;i<dAFTER_IMAGE_COUNT;i++)
	{	m_aPreImage[i][0]		=	0xff;
		m_aAfterImageAlpha[i]	=	0;
	}

	m_iAfterImage		=	dCLOSE_AFTER_IMAGE;
	*/
}

//
//	더미를 업데이트
BOOL
CActor::updateDummy()
{
	if (m_wDummyStatus	&&	m_wDummyStatus	!=	dALIVE_DUMMY)
	{
		m_wDummyStatus--;

		if	(m_wDummyStatus	==	0)
		{
			g_am.removeActor(m_wSerial);

			return	FALSE;
		}
	}

	return TRUE;
}

void
CActor::updateActionStatus()
{
	m_isDontUpdate	=	FALSE;

	if	(m_wIsFeignDeath			)
		m_isDontUpdate	=	TRUE;
	if	(m_isSkillControl			)
		m_isDontUpdate	=	TRUE;
}

//
//	특정 프레임이 루프 된다.
inline	void
CActor::updateLoopingAction()
{
	if (m_wLoopState	==	dLOOP_NONE)	return;

	if (m_wLoopState	==	dLOOP_INCREASE)			//	 프레임 증가
	{
		if (m_iFrame	>	getLoopEndFrame())
		{	m_iFrame	-=	2;	//	루프다.
			m_wLoopState=	dLOOP_DECREASE;
		}
	}

	if (m_wLoopState	==	dLOOP_DECREASE)			//	프레임 감소
	{	m_iFrame	-=	2;

		if (m_iFrame	<	getLoopBeginFrame())
		{	m_iFrame	+=	2;	//	루프다.
			m_wLoopState=	dLOOP_INCREASE;
		}
	}
}

//
//	시체를 서서히 사라지게 한다.
inline	BOOL
CActor::reduceCorpseTime()
{
	if	(m_wCorpseTime)	
	{
		if	(m_wCorpseTime	==	0xffff	)
			return	FALSE;

		m_wCorpseTime--;

		if	(m_wCorpseTime	==	0		)
			g_am.removeActor(m_wSerial);

		return	TRUE;
	}
	
	return	FALSE;
}

//
//	타격 액션 취함
void
CActor::hitAction()
{
	if	(isDeath()				)
		return;
	if	(m_iAnm	==	dACT_CHANGE1)
		return;

	CSkill	*lpSkill	=	m_abilityUse.getSkill();

	if	(lpSkill)
		if	(lpSkill->isNotInterrupt())
			return;

	stop();

	setExclusiveAction(TRUE);
	setAnm(dACT_HIT);				//	맞았을때 에니메이션
}

//
//	블럭킹 액션 취하기
void
CActor::blockingAction(CActor *_lpAttacker,int _iRigidityTime)
{
	if	(isDeath())
		return;

	if	(m_iAnm	==	dACT_CHANGE1)
		return;

	if	(m_wUseSpecialActionSkill	!=	eRS_NONE)
		m_iHillHP	=	1;
	else
		m_iHillHP	=	0;

	stop();

	if	(_lpAttacker==	NULL)
		_lpAttacker	=	this;

	if	(isPlayer())
		action(_lpAttacker->m_pos.x,_lpAttacker->m_pos.y,ct_aBlockingAction[m_wJob]);
	else
		action(_lpAttacker->m_pos.x,_lpAttacker->m_pos.y,dACT_MONSTER_BLOCKING);

	setExclusiveAction(TRUE);
	setBlockingRigidityTime(_iRigidityTime);
	setShakeTime(_iRigidityTime);
}

//
//	블럭킹 경직 시간 업데이트
inline	BOOL
CActor::updateBlockingRigidityTime()
{	
	if (m_wBlockingRigidityTime)	
	{
		if	(s_isUpdateTimingByUpdatePeriod)
			m_wBlockingRigidityTime--;

		return	TRUE;
	}

	return	FALSE;
}



//
//	예약된 타격-_- 실행
inline	void
CActor::operateBookedHit()
{
	if (!m_isDamageFrame)
		return;

	CSkill	*lpSkill	=	m_attackInfo.m_ability.getSkill();
	CActor	*lpTarget	=	&g_aActor[m_attackInfo.m_wTarget];
	int		iEffect = 0 ;

	if (m_wUseSkill	>=	dSKILL_BASIC_ATTACK_TO_ARCA	&&	m_wUseSkill	<=	dSKILL_BORDER_OF_ATTACK_TO_OBJECT)
	{
		g_esm.playActionSound(lpSkill);
		strikeBasicAttackDamageToObject();
	}

	if (m_wUseSkill	==	dSKILL_BASIC_ATTACK				)
	{
		g_esm.playActionSound(lpSkill);
		strikeBasicAttackDamage();
	}
	if (m_wUseSkill	==	dSKILL_ILLUSION_ATTACK			)
	{
		g_esm.playActionSound(lpSkill);
		CActor		*lpTarget	=	&g_aActor[m_attackInfo.m_wTarget];
		CHitInfo	*lpInfo		=	m_attackInfo.getInfo(m_bContinuousHitCounter);

		strike(lpTarget,&m_attackInfo.m_ability,lpInfo,0,FALSE);

		if (isHero())
			if (lpInfo->isHit() && !m_isGetCPByContinuousHitFirstAttack)
			{
				m_isGetCPByContinuousHitFirstAttack	=	TRUE;

				g_hero.increaseCP(m_attackInfo.m_ability.getGetCP());
			}

		m_bContinuousHitCounter++;

		if (m_bContinuousHitCounter	>=	m_attackInfo.m_ability.getContinuousShotCount())
			m_wUseSkill				=	0xffff;
	}

	if (m_wUseSkill	==	dSKILL_CONTINUOUS_HIT_ATTACK	)
	{
		g_esm.playActionSound(lpSkill);
		if (m_attackInfo.m_wTarget	!=	0xffff)	//	타겟이 사라져 버렸다.
		{
			CActor		*lpTarget	=	&g_aActor[m_attackInfo.m_wTarget];
			CHitInfo	*lpInfo		=	m_attackInfo.getInfo(m_bContinuousHitCounter);

			strike(lpTarget,&m_attackInfo.m_ability,lpInfo,0,FALSE);

			if (isHero())
				if (lpInfo->isHit() && !m_isGetCPByContinuousHitFirstAttack)
				{
					m_isGetCPByContinuousHitFirstAttack	=	TRUE;

					g_hero.increaseCP(m_attackInfo.m_ability.getGetCP());
				}
		}

		m_bContinuousHitCounter++;
	}

	if(m_attackInfo.m_wTarget == 0xffff)
		return;

	//전투 스킬 버프 스킬포인트 시간이 있을때 버프.
	if (lpTarget->m_bf1IsGGGPointHit)
	{
		lpTarget->m_bf1IsGGGPointHit = FALSE;
		
		switch (lpTarget->m_bf3HitGGGType)
		{
		case 1:		//FIRE 
			iEffect = 544;	//hit_inferno_bite.sad 인페르노 바이트
			break;
		case 2:		//WATER
			iEffect = 37;	//hit_chilling_touch.sad 칠링 터치.
			break;
		case 3:		//WIND
			iEffect = 206;	//charge_yelling.sad 소리치기
			break;
		case 4:		//EARTH 
			iEffect = 271;	//hit_branch_fist.sad 브랜치 피스트.
			break;
		}
	}
	
	if (iEffect)
		lpTarget->addHitEffect(0,0,iEffect);
	
	iEffect = 0 ;
	
	
}

//
//	갱신 - 각종 정보나 이동 어쩌구 저쩌구 암튼 이게 기본이다.
BOOL
CActor::updateForModel()
{
//	각종 타이머 관련 업데이트
////////////////////////////////////////////////////////////
//	프레임 갱신
////////////////////////////////////////////////////////////
	if (m_iFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
	{
		m_iFrameCounter	-=	dSYNC_FPS;
		m_iFrame++;

		if (m_iFrame	>=	m_iFrameCount)		//	한 에니메이션이 끝났다.
		{
			if (isExclusiveAction())		//	단발성 에니메이션이었다면 정지한다
			{
				setExclusiveAction(FALSE);

				stop();
			}

			m_iFrame	=	0;
		}
	}

	m_iFrameCounter	+=	m_iFPS;

	return TRUE;
}

//
//	잘못된 액터 정보 리스트에 추가
void
CActor::addToAskInfoActorList()
{
	if (m_wSyncStatusWithServer	==	eSSWS_WAIT_ACTOR_INFO)
		return;

	if (!g_am.addToAskInfoActorList(m_wSerial))
		return;	//	데이터 요청 리스트에 추가 시킨다.

	changeSyncWithServerStatus(eSSWS_WAIT_ACTOR_INFO);
	m_wWaitActorInfoTime	=	dWAIT_ACTOR_INFO_PERIOD;

	m_dwEnchantedMask		=	0;
	m_enchantedImage.reset();
}

//
//	매우 간단한 캐릭터 정보를 얻기위한 리스트에 추가
void
CActor::addToObscurityActorList()
{
	if (m_wSyncStatusWithServer	==	eSSWS_WAIT_ACTOR_INFO)
		return;

	if (m_wSyncStatusWithServer	==	eSSWS_WAIT_VERY_SIMPLE_ACTOR_INFO)
		return;

	if (!g_am.addObscurityActor(m_wSerial))
		return;

	changeSyncWithServerStatus(eSSWS_WAIT_VERY_SIMPLE_ACTOR_INFO);

	m_wWaitActorInfoTime	=	dWAIT_ACTOR_INFO_PERIOD;

	m_dwEnchantedMask		=	0;
	m_enchantedImage.reset();
}

void
CActor::addToMisplacedActorList()
{
	if (m_wSyncStatusWithServer	==	eSSWS_WAIT_ACTOR_INFO)
		return;

	if (m_wSyncStatusWithServer	==	eSSWS_WAIT_VERY_SIMPLE_ACTOR_INFO)
		return;

	if (m_wSyncStatusWithServer	==	eSSWS_WAIT_MINIMUM_ACTOR_INFO)
		return;

	if (!g_am.addMisplacedActor(m_wSerial))
		return;	//	시야 안에 있는 시야 밖에 있는걸로 되어 있다.

	changeSyncWithServerStatus(eSSWS_WAIT_MINIMUM_ACTOR_INFO);

	m_wWaitActorInfoTime		=	dWAIT_ACTOR_INFO_PERIOD;

	m_dwEnchantedMask		=	0;
	m_enchantedImage.reset();
}

//
//	서버와 위치를 싱크 시킨다.
inline int
CActor::syncInfoWithServer()
{
	if	(m_wSyncStatusWithServer >= eSSWS_WAIT_ACTOR_INFO && m_wSyncStatusWithServer <= eSSWS_WAIT_VERY_SIMPLE_ACTOR_INFO)	//	데이터를 요청했구 기다리는 중이다.
	{
		m_wWaitActorInfoTime--;

		if (m_wWaitActorInfoTime	==	0)
			changeSyncWithServerStatus(m_wOldSyncStatusWithServer);

		return	eSIS_SLEEP;
	}

	if	(m_wSyncStatusWithServer == eSSWS_VALID_ACTOR || m_wSyncStatusWithServer == eSSWS_WANT_SYNC)//	정보가 없거나 싱크를 원하는 상태다.
	{
		if	(g_hero.m_rectSight.isIn(m_pos.x,m_pos.y))
			addToObscurityActorList();	//	시야 안으로 들어왔으니 정보 요청

		return eSIS_SLEEP;	//	시야 밖에 있으니 무시한다.
	}

	if	(s_bIsSecretDungeon || s_bIsBossZone || m_wIsFixedActor || isPartyMember() || g_hero.isPet(m_wSerial) || g_hero.isSummonBeastActor(m_wSerial))
		return	eSIS_ACTIVE_ACTOR;	//	위치가 고정된 녀석은 이동을 안하니까 위치정보가 필요없다. 파티 멤버는 알아서 업데이트 된다.

	//JBC	플레이어의 시야 영역을 강제로 세팅 (에니메이션을 위하여.)	08-07-16
	if(g_bObserverMode)
	{
		g_hero.m_rectSight.set(	g_am.GetObserverPos().x - dVIEWPORT_MARGIN,
			g_am.GetObserverPos().y - dVIEWPORT_MARGIN,
			g_am.GetObserverPos().x + cDRAW::WIDTH	+ dVIEWPORT_MARGIN,
			g_am.GetObserverPos().y + cDRAW::HEIGHT + dVIEWPORT_MARGIN);
	}

	if	(g_hero.m_rectSight.isIn(m_pos.x,m_pos.y))			//	시야 안에 있다.
	{
		if	(m_wSyncStatusWithServer == eSSWS_OUT_OF_SIGHT)	//	시야 밖에 있는 상태이다.
		{
			addToObscurityActorList();

			return	eSIS_SLEEP;
		}

		return	eSIS_ACTIVE_ACTOR;
	}

	if (m_wSyncStatusWithServer	!=	eSSWS_OUT_OF_SIGHT)
		if (resetForOutOfSightActor())					//	 시체의 경우 시야 밖으로 나가면 바로 제거해 버린다. -_-;;
			return	eSIS_REMOVED;

	return	eSIS_SLEEP;
}

//
//	충전효과 업데이트
inline	void
CActor::updateChargeEffect()
{
	if	(m_wChargeImage		==	0xffff)
		return;

	if	(m_isMove)
		cancelChargeEffect();

	cANM	*lpEffect		=	g_im.getEffect(m_wChargeImage);

	int	iAnm				=	m_bChargeAnm;

	if (lpEffect->m_iAnmCount < 3)
		iAnm				=	0;

	int	iFrameCount			=	lpEffect->GetFrameCount(iAnm);
	int	iFPS				=	lpEffect->GetFPS(iAnm);

	m_wChargeFrameCounter	+=	iFPS;

	if (m_wChargeFrameCounter>=	dSYNC_FPS)
	{
		m_wChargeFrameCounter-=	dSYNC_FPS;
		m_bChargeFrame++;

		if (m_bChargeFrame	>=	iFrameCount)
			increaseChargeEffectAnimation();
	}
}

void
CActor::setLastFrame()
{
	m_iFrameCounter	=	0;
	m_iFrame		=	m_iFrameCount-1;
}

void
CActor::updateShakleFrame()
{
	m_bf1IsFreezeShakle	=	FALSE;

	if	(m_wShakleSkill	==	0xffff)
		return;

	cANM	*lpShakle	=	g_im.getEffect(g_aSkill[m_wShakleSkill].m_wSkillImage);

	if	(!lpShakle)
		return;

	{
		CSkillExtraEffect	*lpSEE	=	g_aSkill[m_wShakleSkill].getExtraEffectByEffect(eSKILL_EE_MAKE_SHAKLE);
		
		if	(lpSEE	&&	lpSEE->m_aiValue[4])
			m_bf1IsFreezeShakle	=	TRUE;
	}

	if	(m_wShakleShakeTime)
		m_wShakleShakeTime--;

	int	iFrameCount		=	lpShakle->GetFrameCount(m_wShakleAnm);
	int	iFPS			=	lpShakle->GetFPS(m_wShakleAnm);
	int	iAnmCount		=	lpShakle->m_iAnmCount;

	if	(m_wShakleFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
	{
		m_wShakleFrameCounter	-=	dSYNC_FPS;

		m_wShakleFrame++;

		if	(m_wShakleFrame	>=	iFrameCount)
		{
			m_wShakleAnm			=	min(1,iAnmCount-1);
			m_wShakleFrame			=	0;
			m_wShakleFrameCounter	=	0;
		}
	}

	m_wShakleFrameCounter	+=	iFPS;
}

void
CActor::updateMiniPetFrame()
{
	if	(m_wMiniPetType	<	nsMiniPetType::Count)
	{
		if	(m_isMove				&&	m_iMiniPetAnm	==	dMINIPET_ACT_READY)
			setMiniPetAnm(dMINIPET_ACT_MOVE,0);

		if	(m_isMove	==	FALSE	&&	m_iMiniPetAnm	==	dMINIPET_ACT_MOVE)
			setMiniPetAnm(dMINIPET_ACT_READY,0);

		if	(m_iMiniPetFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
		{
			m_iMiniPetFrameCounter	-=	dSYNC_FPS;

			m_iMiniPetFrame++;

			if	(m_iMiniPetFrame	>=	m_iMiniPetFrameCount)
			{
				if	(m_isMove)
					setMiniPetAnm(dMINIPET_ACT_MOVE,0);
				else
					setMiniPetAnm(dMINIPET_ACT_READY,0);
			}
		}

		m_iMiniPetFrameCounter	+=	m_iMiniPetFPS;

	}

	if	(m_wMiniPetType2	<	nsMiniPetType::Count)
	{
		if	(m_isMove				&&	m_iMiniPetAnm2	==	dMINIPET_ACT_READY)
			setMiniPetAnm(dMINIPET_ACT_MOVE,1);

		if	(m_isMove	==	FALSE	&&	m_iMiniPetAnm2	==	dMINIPET_ACT_MOVE)
			setMiniPetAnm(dMINIPET_ACT_READY,1);

		if	(m_iMiniPetFrameCounter2	>=	dSYNC_FPS)		//	프레임 갱신
		{
			m_iMiniPetFrameCounter2	-=	dSYNC_FPS;

			m_iMiniPetFrame2++;

			if	(m_iMiniPetFrame2	>=	m_iMiniPetFrameCount2)
			{
				if	(m_isMove)
					setMiniPetAnm(dMINIPET_ACT_MOVE,1);
				else
					setMiniPetAnm(dMINIPET_ACT_READY,1);
			}
		}

		m_iMiniPetFrameCounter2	+=	m_iMiniPetFPS;

	}

}

void
CActor::updateMiniPetPos(CPos* _lpPos, int _iAngle)
{
	
	cANM	*lpBody	=	getBody();
	
	if	(!lpBody	||	m_iAnm	>=	lpBody->m_iAnmCount	)
		return;
	
	CPos destPos;
	destPos.x = m_pos.x ;
	destPos.y = m_pos.y - getHeight(TRUE);

	int iAngle	=	_iAngle;

	GetOvalAnglePos(&destPos,iAngle,40);

	if(_lpPos->x == 0xffff || _lpPos->x == 0xffff)
	{
		_lpPos->x = destPos.x;
		_lpPos->y = destPos.y;				
	}
	else
	{
		
		int iRange = sqrt(GetOvalRange(_lpPos->x,_lpPos->y,destPos.x,destPos.y));

		if(iRange> g_iScreenHeight)
		{
			_lpPos->x = destPos.x;
			_lpPos->y = destPos.y;	
		}else
		if(iRange > 40)
		{

			int iNewAngle = GetOvalAngleToTarget(_lpPos->x,_lpPos->y,destPos.x,destPos.y);
			CPos	pos;
			pos.x = _lpPos->x;
			pos.y = _lpPos->y;
			float fSpeed = m_iPPS;

			if(fSpeed <=0 )
				fSpeed = 1;
			fSpeed = fSpeed/80+ iRange/50.0;
			--fSpeed;
			float fRound = fSpeed - (int)fSpeed/1 ;
			if(fRound >=0.5 )
			{
				fSpeed -=fRound;
				++fSpeed;
			}
			GetOvalAnglePos(&pos,iNewAngle,(int)fSpeed);
			int iRange1 = sqrt(GetOvalRange(pos.x,pos.y,destPos.x,destPos.y));
			if(iRange>iRange1)
			{
				_lpPos->x = pos.x;
				_lpPos->y = pos.y;

			}
		}
	}
}



BOOL
CActor::updateFrame()
{
	updateMiniPetFrame();

	if	(m_wAwesomeFortressAction != 0xffff)
	{
		if (m_awesomeFortressActionInfo.updateFrame())
		{
			m_wAwesomeFortressAction	=	0xffff;
			m_wParallelBunshineCount	=	0;
		}
	}
	for(int i=0;i<dAFTER_IMAGE_COUNT;++i)
	{
		if	(m_wContinuousHitOfDoppelgangerAction[i] != 0xffff)
		{	
			if (m_continuousHitOfDoppelgangerActionInfo[i].updateFrame())
			{
				m_wContinuousHitOfDoppelgangerAction[i]=	0xffff;
				m_wDoppelgangerCount	=	0;
			}
		}
	}
	if	(m_iFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
	{
		m_iFrameCounter	-=	dSYNC_FPS;

		m_iFrame++;

		if	(m_wIsFreezeAtTriggerFrame	&&	getBody()->IsTrigger(m_iAnm,m_iFrame-1))
			m_iFrame--;

		if	(m_wUseSkill == dSKILL_ILLUSION_ATTACK)	//	딜레이 어택중이다.
		{
			m_bIllusionAttackFrame++;

			if	(m_iFrame	>=	m_iFrameCount)
				m_iFrame	=	m_iFrameCount-1;

			for (int i=m_bIllusionAttackFrame,iCount=0;i>=0 && iCount<m_attackInfo.getContinuousShotCount();i-=m_attackInfo.getContinuousShotPeriod())
			{
				m_isDamageFrame		=	getBody()->IsDamage(m_iAnm,i);

				if	(m_isDamageFrame)
					break;
			}
		}
		else
		{
			BOOL			bIsRunExceptHandle	=	FALSE;
			CActiveSkill	*lpActiveSkill		=	getActiveSkill();

			{
				if	(m_iFrame	>=	m_iFrameCount)
				{
					if	(lpActiveSkill	&&	lpActiveSkill->m_lpSkill)
					{
						if	(lpActiveSkill->m_lpSkill->m_wType	==	eSKILL_TYPE_GIGA_LIGHTNING)
						{
							cAS_GigaLightning	*lpGL	=	(cAS_GigaLightning	*)lpActiveSkill;

							bIsRunExceptHandle	=	lpGL->casterActionFinished();
						}
					}
					else
					if	(m_wSageBookColor)
						changeSageBookColor(eSage_book_color_normal);
				}
			}
			
			if	(bIsRunExceptHandle	==	FALSE	&&	m_iFrame	>=	m_iFrameCount)		//	한 에니메이션이 끝났다.
			{
				if	(isExclusiveAction())		//	단발성 에니메이션이었다면 정지한다
				{
					if	(isDeath())				//	죽었건 죽은척 한 녀석이건..
					{
						if	(m_bf1IsCopyActor)
							m_wCorpseTime	=	dREMAIN_CORPSE_TIME;	//	복제 캐릭이라면 10초동안 서서히 사라진다.
						else
						if	(!isPlayer() && !isPet())
							m_wCorpseTime	=	dREMAIN_CORPSE_TIME;	//	플레이어가 아니라면.. 10초동안 서서히 사라진다.

						if	(isSummonBeast())
						{
							if	(m_wIsReleaseSummonBeast)
								m_wCorpseTime	=	dSYNC_FPS*2;
							else
								m_wCorpseTime	=	dREMAIN_CORPSE_TIME_FOR_SUMMON_BEAST;
						}

						m_iFrame--;

						return	TRUE;
					}

					setExclusiveAction(FALSE);
					setActionFinish(TRUE);
					m_bContinuousHitCounter	=	0;
					m_wIsDontHitAction		=	FALSE;

					if	(m_iAnm	==	dACT_CHANGE1)
						transformation();
					else
					{
						BOOL			isUsedLoopActionSkill	=	FALSE;

						if (lpActiveSkill	&&	lpActiveSkill->isLoopActionSkill())
							isUsedLoopActionSkill	=	TRUE;

						if (!isUsedLoopActionSkill)
							stop();
					}
				}

				m_iFrame	=	0;

				if(m_wRemainReplayCount)
				{
					m_wRemainReplayCount--;

					if(m_wRemainReplayCount == 0)
						stop();
				}
			}

			m_isDamageFrame		=	getBody()->IsDamage(m_iAnm,m_iFrame);
			m_isTriggerFrame	=	getBody()->IsTrigger(m_iAnm,m_iFrame);

			if	(m_isTriggerFrame)
			{
				if	(m_enchantedImage.m_bf1IsRabbit)
					g_esm.play("Rabbit Run.wav");
				if	(m_enchantedImage.m_bf1IsPlump)
					g_esm.play("pumb.wav");
				if	(m_enchantedImage.m_bf1IsFrog)
				{
					if	(random(100)	<	15)
						g_esm.play("Kiss of Princess_hit.wav");
				}
			}
		}
	}

	if	(getBody()->m_wIsOccasionallyRestAction)
	{
		if	(m_iFrame==0 && m_iFrameCounter == 0	&& m_iAnm == dACT_READY)
		{
			if	(random(1000)==0)
				m_iFrameCounter	+=	m_iFPS;
		}
		else
			m_iFrameCounter	+=	m_iFPS;
	}
	else
		m_iFrameCounter	+=	m_iFPS;

	return	TRUE;
}

void
CActor::syncAppearance()
{
	if	(m_enchantedImage.m_bf1IsExistBiter)
	{
		if	(m_wBiter	==	0xffff)
			g_am.addAskActorEnchantEffectInfo(m_wSerial,eAEI_BITE_HANGER);
	}
	else
		m_wBiter	=	0xffff;

	if	(m_enchantedImage.m_bf1IsExistTheOtherSelf)
	{
		if	(m_wTheOtherSelfCount	==	0)
			g_am.addAskActorEnchantEffectInfo(m_wSerial,eAEI_THE_OTHER_SELF_COUNT);
	}
	else
		m_wTheOtherSelfCount	=	0;

	if	(m_enchantedImage.m_bf1IsBindByShakle)
	{
		if	(m_wShakleSkill			==	0xffff)
			g_am.addAskActorEnchantEffectInfo(m_wSerial,eAEI_SHAKLE_SKILL);
	}
	else
		m_wShakleSkill	=	0xffff;
}
void
CActor::operateDoppellgangerAttack()
{// 부가효과 분신 공격 처리..
	
	if(!m_wAttackDoppelgangerCount)
		return;
	if(isDeath())
	{
		m_fAttackDoppelgangerFrame = 0;
		m_bIsHitAttackDoppelganger = FALSE;
		return;
	}

	cANM* lpAnm = getBody();
	int iMaxFrame = 0;

	if(lpAnm)
		iMaxFrame = lpAnm->GetFrameCount(m_wAttackDoppelgangerAnm);


	if(iMaxFrame == 0)
	{	
		m_wAttackDoppelgangerCount = 0;
		m_fAttackDoppelgangerFrame = 0;
		m_bIsHitAttackDoppelganger = FALSE;
		return;
	}

		
	m_fAttackDoppelgangerFrame += (float)iMaxFrame/dSYNC_FPS* 1000.0 / m_wActionPerionAttackDoppelganger;

	cANM_BASE* lpAnmBase =	g_apBody[m_wBody];
	int iDamageFrame = 0xffff;
	if(lpAnmBase && lpAnmBase->m_pAnmData )
		iDamageFrame = lpAnmBase->getDamageFrame(m_wAttackDoppelgangerAnm);

	if(iDamageFrame == 0xffff)
		iDamageFrame	=	iMaxFrame*0.6;

	if (iDamageFrame <= m_fAttackDoppelgangerFrame&& m_bIsHitAttackDoppelganger == FALSE)
	{
		cAbility ability;
		ability.set(dSKILL_TRANSFORMATION_SWORD_ATTACK,1);		// 타격은 일반 스킬로 설정했다...
		CHitInfo hitInfo;
		hitInfo.reset(TRUE);
		hitInfo.m_dwPhysicalDamage	=	m_iAttackDoppelgangerDamage;		//	얼마나? 아파? ;;
		hitInfo.m_dwMagicDamage		=	0;		//
		hitInfo.setHit();
		m_bIsHitAttackDoppelganger = TRUE;
		hit(NULL,&ability,&hitInfo);


	}

	if(m_fAttackDoppelgangerFrame >= iMaxFrame)
	{
		m_fAttackDoppelgangerFrame	=	0;
		--m_wAttackDoppelgangerCount;
		m_bIsHitAttackDoppelganger = FALSE;

	}

	if(m_wAttackDoppelgangerCount == 0)
	{
		m_fAttackDoppelgangerFrame = 0;
		m_bIsHitAttackDoppelganger = FALSE;
	}

}

//
//	갱신 - 각종 정보나 이동 어쩌구 저쩌구 암튼 이게 기본이다.

void			
CActor::addBit(WORD _wSerialInField, DWORD	_dwNameHashCode)
{

	CPos	pos = m_pos;
	pos.y		-=	getBodyHeight();
	int iWidth	=	getBodyWidth()/2;
	int iAngle	=	random(360);
	GetOvalAnglePos(&pos,(m_iAngle + iAngle) % 360,iWidth);
	m_bitInfoManager.addBit(_wSerialInField,_dwNameHashCode,pos,iAngle);
}

void			
CActor::updateBit()
{
	for(int i=0;i<m_bitInfoManager.m_iCount;++i)
	{
		
		int iWidth	=	getBodyWidth()/2;

		CPos	posDest = m_pos;	
		posDest.y		-=	getBodyHeight();
		GetOvalAnglePos(&posDest,(m_iAngle + m_bitInfoManager.m_bitInfo[i].m_wAngle) % 360,iWidth);

		int iRange	=	sqrt(GetOvalRange(posDest.x,posDest.y,m_bitInfoManager.m_bitInfo[i].m_pos.x,m_bitInfoManager.m_bitInfo[i].m_pos.y));

		if(iRange > g_iScreenHeight)
		{
			m_bitInfoManager.m_bitInfo[i].m_pos = posDest;
		}
		else
		if(iWidth < iRange)
		{	
			int iPPS	=	2;
			int iLength	=	iRange - iWidth;
			iPPS		+=	iLength / 10;	

			int iAngle	=	GetOvalAngleToTarget( m_bitInfoManager.m_bitInfo[i].m_pos.x, m_bitInfoManager.m_bitInfo[i].m_pos.y,posDest.x,posDest.y);
			GetOvalAnglePos(&m_bitInfoManager.m_bitInfo[i].m_pos,iAngle,iPPS);
		}



	}
}

void			
CActor::removeBit(WORD _wSerialInField, DWORD _dwNameHashCode, int _iCount)
{
	m_bitInfoManager.removeBit(_wSerialInField,_dwNameHashCode, _iCount);
}


BOOL
CActor::update()
{
	if	(m_enchantedImage.m_bf1IsPlump || m_enchantedImage.m_bf1IsRabbit || m_enchantedImage.m_bf1IsFrog || m_enchantedImage.m_bf1IsZombieBomb)
		m_bIsTransOther	=	TRUE;
	else
		m_bIsTransOther	=	FALSE;

	if	(m_wBody	==	dJOB_MONSTER_TIAMATH)
	{
		if	(m_iAnm	==	dACT_ACTION_1)
			g_am.m_iIsHideTimathFeelerTime	=	2;
	}

	if	(s_iFrameCounter	%2	==	0)
		m_flying_bow.update();

	syncAppearance();

	updateChainedDamage();
	updateBit();
	if	(m_wMiniPetType	<	nsMiniPetType::Count)
		updateMiniPetPos(&m_posPet, (m_iAngle+ 90) % 360);

	if	(m_wMiniPetType2	<	nsMiniPetType::Count)
		updateMiniPetPos(&m_posPet2, (m_iAngle+ 270) % 360);
	
	if	(m_wIsRideTamer)
	{
		updateEffect();
		return	TRUE;
	}

	if	(m_wRevealTime)
		m_wRevealTime--;

	if	(m_wBody	==	dJOB_MONSTER_ARCH_DEVIL && !isEnchantedLevitate())
	{
		if	(m_iAnm	==	dACT_ARCH_DEVIL_HOVER)
			setAnm(dACT_READY);
	}

	if	(m_wJob	==	dJOB_PRINCESS)
	{
		if	(m_enchantedImage.m_bf1IsJumppingSkipping)
		{
			if	(m_iAnm	!=	dACT_PRINCE_JUMPPING_SKIPPING)
				setAnm(dACT_PRINCE_JUMPPING_SKIPPING);
		}
		else
		{
			if	(m_iAnm	==	dACT_PRINCE_JUMPPING_SKIPPING)
				setAnm(dACT_READY);
		}

		if	(m_enchantedImage.m_bf1IsPlump)
		{
			if	(m_iAnm	!=	dACT_READY)
				setAnm(dACT_READY);
		}
	}

	if	(m_wReleaseMagicBoxAlpha	!=	0xffff)
	{
		m_wReleaseMagicBoxAlpha--;

		if	(m_wReleaseMagicBoxAlpha	==	0)
		{
			m_wReleaseMagicBoxAlpha		=	0xffff;
			m_wCrushMagicBoxFrame		=	0xffff;
		}
	}

	if	(m_wCrushMagicBoxFrame	!=	0xffff)	//	매직 박스 이미지 업데이트
	{
		m_wCrushMagicBoxFrame++;
		
		cANM*	lpEffect	=	g_im.getEffect(g_im.m_wShutInMagicBox);

		if	(lpEffect)
		{
			int	iMaxFrame	=	lpEffect->GetFrameCount(2);
			int	iFPS		=	lpEffect->GetFPS(0);

			int	iFrame		=	(m_wCrushMagicBoxFrame*iFPS/dSYNC_FPS);

			if	(iFrame		>=	iMaxFrame)
			{
				m_wCrushMagicBoxFrame	=	0xffff;
				crushMagicBox(FALSE);
			}
		}
	}

	if	(isDeath()	&&	isNpc()	==	FALSE)
	{
		if	(m_iAnm	!=	dACT_DEAD)
		{
			setAnm(dACT_DEAD,TRUE);
			setExclusiveAction(TRUE);
		}
	}

	if (m_wRideDog	!=	0xffff)
	{
		cSummonBeast	*lpSummonBeast	=	g_hero.getKelby();

		if (lpSummonBeast && isHero())
		{
			CActor		*lpKelby		=	g_am.getTestedActor(lpSummonBeast->m_wBodySerial);

			if (lpKelby)
				lpKelby->stop(m_pos.x,m_pos.y);
		}
	}

	m_wIsAddPaint	=	FALSE;
	m_wIsPutName	=	FALSE;

	if(m_enchantedImage.m_bf1IsImmunePhysicalDamage)
	{
		if	(s_iFrameCounter	%10	==	0)
		{
			
			for(int i=0;i<dAFTER_IMAGE_COUNT;++i)
			{
				if(m_aAfterImageList[i].m_bAlphaDepth	== 0)
				{
					m_aAfterImageList[i].m_bAnm		=	m_iAnm;
					m_aAfterImageList[i].m_bDirect	=	m_iDirect;
					m_aAfterImageList[i].m_bFrame	=	m_iFrame;
					m_aAfterImageList[i].m_pos		=	m_pos;
					m_aAfterImageList[i].m_bAlphaDepth	=	32;
					break;
				}
			}
		}
	}

	if	(m_enchantedImage.m_bf1IsDance)
	{
		int	iCheckPeriod	=	dSYNC_FPS/16;	

		if	((s_iFrameCounter%iCheckPeriod)	==	0)
		{
			cANM	*lpBody	=	getBody();

			if	(lpBody)
			{
				int	iNextDirect=	m_iDirect+1;

				if	(iNextDirect	>=	lpBody->GetDirectCount(m_iAnm))
					iNextDirect	=	0;

				setDirect(iNextDirect);
			}
		}
	}
	if(m_bIsEffectJump)
	{
		int iCosValue = m_wJumpFrameCount;
		float fCos = dCOS_TABLE[iCosValue]/ 256.0;
		m_iHeightEffectJump 	=	fCos * m_wIncreaseHeightEffectJump;
		m_wJumpFrameCount+=m_wEffectJumpSpeed;
		if(m_wJumpFrameCount >= 180)
		{
			m_wJumpFrameCount	= 0;
			m_bIsEffectJump		= FALSE;
			m_iHeightEffectJump	= 0;
			m_wEffectJumpSpeed	= 0;
		}
	}
	if (g_hero.isSummonBeastActor(m_wSerial) + g_hero.isPet(m_wSerial))
	{
		cPet	*lpPet	=	g_hero.getPet(g_hero.getPetIndex(m_wSerial));

		if (lpPet)
		{
			if (lpPet->m_wEngageTarget	!=	0xffff)
			{
				CActor	*lpTarget	=	g_am.getTestedActor(lpPet->m_wEngageTarget,FALSE);

				if (!lpTarget	||	lpTarget->isDeath()	||	m_isMove)
				{
					lpPet->m_wEngageTarget	=	0xffff;
					m_attackInfo.reset();
				}
			}

			if (m_attackInfo.m_wTarget	!=	0xffff)
				lpPet->m_wEngageTarget	=	m_attackInfo.m_wTarget;
		}

		cSummonBeast	*lpSummonBeast	=	g_hero.getSummonBeast(g_hero.getSummonBeastIndex(m_wSerial));

		if (lpSummonBeast)
		{
			if (lpSummonBeast->m_wEngageTarget	!=	0xffff)
			{
				CActor	*lpTarget	=	g_am.getTestedActor(lpSummonBeast->m_wEngageTarget,FALSE);

				if (!lpTarget	||	lpTarget->isDeath()	||	m_isMove)
				{
					lpSummonBeast->m_wEngageTarget	=	0xffff;
					m_attackInfo.reset();
				}
			}

			if (m_attackInfo.m_wTarget	!=	0xffff)
				lpSummonBeast->m_wEngageTarget	=	m_attackInfo.m_wTarget;
		}
	}

	if	(m_wDummyStatus		!=	0xffff	)	
		if (!updateDummy())	
			return	FALSE;

	if	(isSlave())
		g_lpHero->setPos(m_pos.x,m_pos.y);

	if	(!isHero())
	{
		int	iResult		=	syncInfoWithServer();

		if	(iResult	!=	eSIS_ACTIVE_ACTOR)
		{
			if (iResult	==	eSIS_REMOVED)	
				return	FALSE;

			return	TRUE;
		}
	}

	if	(m_isSkillControl			)
		return	TRUE;
	if	(m_wTalkTime				)
		m_wTalkTime--;
	if	(m_wShakeTime				)
		m_wShakeTime--;

	{
		int	iFrameCount		=	getBody()->GetFrameCount(m_iAnm);

		if (m_iFrameCount	!=	iFrameCount	)
			m_iFrameCount	=	iFrameCount;

		if (m_iFrameCount	==	0			)
			setAnm(dACT_READY);
	}

	updateChargeEffect();

	if	(m_wUseSpecialActionSkill	!=	eSAS_NONE)
	{
		if	(m_wUseSpecialActionSkill == eSAS_RUSH)
		{
			updateRushSkill();
			return	TRUE;
		}
		if	(m_wUseSpecialActionSkill == eSAS_SCIMATAR_CUTTING	)
		{
			updateScimitarCuttingSkill();
			return	TRUE;
		}
		if	(m_wUseSpecialActionSkill == eSAS_JUMP)
		{
			updateJumpSkill();
			return	TRUE;
		}
		if	(m_wUseSpecialActionSkill == eSAS_BOUNCING_LINEAR)
		{
			updateBouncingLinear();
			return	TRUE;
		}
		if	(m_wUseSpecialActionSkill == eSAS_BIG_BULLET)
		{
			updateBigBullet();
			return	TRUE;
		}
	}

//	if	(m_isDontUpdate				)	return	TRUE;
//	각종 타이머 관련 업데이트
	if (reduceCorpseTime())
	{
		updateEffect();

		if	(m_wSerial	==	0xffff)	
			return	FALSE;	//	죽어서.. 제거 되었다. -_-;;

		return	TRUE;
	}

	if	(updateBlockingRigidityTime())
		return	TRUE;

////////////////////////////////////////////////////////////
//	프레임 갱신
////////////////////////////////////////////////////////////
	m_isActionFinished	=	FALSE;
	m_isTriggerFrame	=	FALSE;
	m_isDamageFrame		=	FALSE;

	if	(isPlayer())
		m_isDamageFrame		=	FALSE;

	if	(isStunedStatus(TRUE) == FALSE && m_wBiteTarget == 0xffff)
		updateFrame();
	
	operateDoppellgangerAttack();

	updateShakleFrame();

	operateBookedHit();	//	예약된 히트 처리
	updateAfterImage();	//	잔상효과 업데이트
	updateEffect();

	if	(isHero())
	{
		if	(g_lpControlActor)
			setPos(g_lpControlActor->m_pos.x,g_lpControlActor->m_pos.y);
	}

	if (m_isMove)
	{
		if (isBlockToMove())
			stop();
		else
			moveUpdate();		//	캐릭터 이동 업데이트
	}

	return TRUE;
}
//
//	방향 설정
void
CActor::setDirect(int _iDirect,BOOL _bIsIgnoreStunStatus)
{
	if	(isHero())
		_bIsIgnoreStunStatus	=	_bIsIgnoreStunStatus;
	if	(_bIsIgnoreStunStatus == FALSE && isStunedStatus())
		return;

	cANM	*lpBody	=	getBody();

	if	(!lpBody)
		return;

	int	iDirectCount	=	lpBody->GetDirectCount(m_iAnm);

	if	(iDirectCount	<=	0	||	_iDirect		>=	iDirectCount)
		return;

	const	int	constTable8DirectToAngle[8]	=	{90,45,0,315,270,225,180,135};

	m_iDirect	=	_iDirect;
	m_iAngle	=	constTable8DirectToAngle[m_iDirect];
}

void
CActor::bookingDirect(int _iDirect)
{
	m_wBookingDirect	=	_iDirect;
}

//
//	무기 속도에 맞춰서 속도를 보정한다.
void
CActor::setFpsByWeaponSpeed(int _iCorrectSpeed)
{
/*	if (iWeapon	==	0xffff)
			m_iFPS	=	getFPS(m_iAnm)*_iCorrectSpeed/100;
	else	setFpsByTPA(g_aBasicItem[iWeapon].m_wSpeed,_iCorrectSpeed);

	_iCorrectSpeed		+=	getActionSpeed();

	int	iWeapon	=	m_itemWeapon.m_wBaseItem;

	if (iWeapon	!=	0xffff)	setFpsByTPA(g_aBasicItem[iWeapon].m_wSpeed,_iCorrectSpeed);
	else
	{
		m_wActionSpeed	=	dSYNC_FPS;
		_iCorrectSpeed	=	(200-_iCorrectSpeed);
		m_wActionSpeed	=	m_wActionSpeed*_iCorrectSpeed/100;
	}
*/
}	//	CActor::setFpsByWeaponSpeed(int _correctSpeed)

//
//	TPA(Tme Per Action)무기 속도에 맞춰서 속도를 보정한다.
void
CActor::setFpsByTPA(int _iTPA,int _iCorrectSpeed)
{
	m_iFPS	=	m_iFrameCount*100/_iTPA;
	m_iFPS	=	m_iFPS*_iCorrectSpeed/100;
}

void
CActor::changeShakleAnm(int anm)
{
	if	(m_wShakleSkill	==	0xffff)
		return;

	cANM	*lpShakle	=	g_im.getEffect(g_aSkill[m_wShakleSkill].m_wSkillImage);

	if	(!lpShakle)
		return;

	m_wShakleAnm		=	min(anm,lpShakle->m_iAnmCount);
	m_wShakleFrame		=	0;
	m_wShakleFrameCounter=	0;
}

void
CActor::setMiniPetAnm(int anm,int _iIndex)
{
	if	(_iIndex	==	0)
	{
		if	(m_wMiniPetType	>=	nsMiniPetType::Count)
			return;

		cANM	*lpMiniPet		=	&g_aMiniPetBody[m_wMiniPetType];

		if	(anm	<	0	||	anm	>=	lpMiniPet->m_iAnmCount)
			return;

		m_iMiniPetAnm			=	anm;
		m_iMiniPetFrameCount	=	lpMiniPet->GetFrameCount(anm);
		m_iMiniPetFPS			=	lpMiniPet->GetFPS(anm);

		m_iMiniPetFrame			=	0;
		m_iMiniPetFrameCounter	=	0;
	}
	else
	{
		if	(m_wMiniPetType2	>=	nsMiniPetType::Count)
			return;

		cANM	*lpMiniPet		=	&g_aMiniPetBody[m_wMiniPetType2];

		if	(anm	<	0	||	anm	>=	lpMiniPet->m_iAnmCount)
			return;

		m_iMiniPetAnm2			=	anm;
		m_iMiniPetFrameCount2	=	lpMiniPet->GetFrameCount(anm);
		m_iMiniPetFPS2			=	lpMiniPet->GetFPS(anm);

		m_iMiniPetFrame2			=	0;
		m_iMiniPetFrameCounter2	=	0;
	}
}

void
CActor::setReplayCount(int _ReplayCount)
{
	m_wRemainReplayCount = _ReplayCount;
}

//
//	에니메이션 설정
//	int		anm		-	설정할 에니메이션
void
CActor::setAnm(int anm,BOOL _bIsForeced)
{
	cANM*	lpBody	=	getBody();

	if	(!lpBody	||	anm	>=	lpBody->m_iAnmCount)
		return;

	if	(m_enchantedImage.m_bf1IsJumppingSkipping)
		anm				=	dACT_PRINCE_JUMPPING_SKIPPING;

	if	(m_wBody==	dJOB_MONSTER_ARCH_DEVIL	&&	isEnchantedLevitate())
	{
		if	(anm	==	dACT_READY	||	anm	==	dACT_WALK	||	anm	==	dACT_RUN)
			anm		=	dACT_ARCH_DEVIL_HOVER;
	}

	int	iOrgAnm	=	anm;

	if	(m_bIsTransOther)
	{
		if	(anm	!=	dACT_WALK	&&	anm	!=	dACT_RUN	&&	anm	!=	dACT_READY)
			anm		=	dACT_READY;
	}
	
	BOOL	bIsMoveAction	=	FALSE;

	if	(m_wRideDog	!=	0xffff)
	{
		if (anm	==	dACT_WALK || anm	==	dACT_RUN)
		{
			bIsMoveAction	=	TRUE;
			anm	=	dACT_RIDE_DOG_MOVE+m_wRideDog*dACT_RIDE_DOG_ACTION_COUNT;
		}
		else
		if (anm	==	dACT_READY)
			anm	=	dACT_RIDE_DOG_READY+m_wRideDog*dACT_RIDE_DOG_ACTION_COUNT;
		else
		if (anm	>=	dACT_ACTION_1)
			anm	=	dACT_RIDE_DOG_ATTACK+m_wRideDog*dACT_RIDE_DOG_ACTION_COUNT;
		else
			return;
	}


	if	(isStunedStatus())
		return;

	m_iFPS			=	getFPS(anm);

	if(m_enchantedImage.m_bf1IsRushTime)		// 돌진상태이면.. 애니매이션속도를 높인다.
		m_iFPS*=2;

	if	(m_abilityUse.m_wSkill	!=	0xffff)
		m_abilityUse.reset();

	if	(m_wBody==	dJOB_MONSTER_ARCH_DEVIL	&&	isEnchantedLevitate())
	{
		if	(anm	==	dACT_ARCH_DEVIL_HOVER)
			bIsMoveAction	=	TRUE;
	}
	else
	if	(anm <= 1 && m_iAnm <=	1)
		bIsMoveAction	=	TRUE;

	if	(m_enchantedImage.m_bf1IsJumppingSkipping && m_wJob == dJOB_PRINCESS && anm == dACT_PRINCE_JUMPPING_SKIPPING)
		bIsMoveAction	=	TRUE;

	if	(anm == m_iAnm)
	{
		if	(bIsMoveAction)	//	걷는건 0,뛰는건 1
		{
			m_iDirectCount	=	getDirectCount(m_iAnm);
			m_iDirect		=	GetDirect(m_iAngle,m_iDirectCount);
		}

		m_iFrameCount	=	getFrameCount(m_iAnm);

		return;
	}

//
//	설정하려는 에니메이션이 걷기나 뛰기이고, 현재 에니메이션이 걷기나 뛰기이면
//	비슷한 프레임으로 맞춰준다.
//
	if	((bIsMoveAction && m_iFrameCount >= 2) || m_enchantedImage.m_bf1IsPlump || m_enchantedImage.m_bf1IsJumppingSkipping)	//	걷는건 0,뛰는건 1 그러니.. 더하면 무조건 1이 나온다.
	{
		int	iDestFrameCount	=	getFrameCount(anm);
		int	iCurFrameCount	=	max(m_iFrameCount,1);
		int	iRate			=	m_iFrame*100/iCurFrameCount;	//	프레임이 어느정도 진행되었냐?

		if	(iCurFrameCount	!=	iDestFrameCount)
			m_iFrame		=	min(iDestFrameCount*iRate/100,iDestFrameCount-1);
	}
	else
		m_iFrame		=	0;

	m_iAnm			=	anm;
	m_iDirectCount	=	getDirectCount(m_iAnm);
	m_iFrameCount	=	getFrameCount(m_iAnm);

	static	BOOL	bIsError	=	FALSE;

	if	(m_iDirectCount	==	0)
	{
		if	(bIsError	==	FALSE)
			s_agent.sendDebugMessage(_ms("%s/%s - %d,%d,%d,%d",g_map.m_strFileName,m_strName,m_wJob,m_iAnm,m_iDirectCount,m_iFrameCount));

		m_iAnm		=	2;
		stop();
		bIsError	=	TRUE;

		return;
	}

	m_iDirect		=	GetDirect(m_iAngle,m_iDirectCount);

	if	(lpBody->IsTrigger(m_iAnm,m_iFrame))
		m_isTriggerFrame	=	TRUE;

	if	(lpBody->IsDamage(m_iAnm,m_iFrame))
		m_isDamageFrame		=	TRUE;

	if	(m_wBookingDirect)
	{
		setDirect(m_wBookingDirect-1);

		m_wBookingDirect	=	0;
	}
}


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//	행동관련
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

void
CActor::miniPetAction(int _iIndex)
{
	if	(isHero())
	{
		int	iPetIndex	=	g_hero.m_wActiveMiniPet;

		if	(_iIndex)
			iPetIndex	=	g_hero.m_wActiveMiniPet2;
		
		g_hero.iMiniPet()->playFX(eMPFX_USE_SKILL,iPetIndex);
	}

	setMiniPetAnm(dMINIPET_ACT_ACTION,_iIndex);
}

//
//	동작을 취한다.
void
CActor::action(int _iX,int _iY,int _iAction,int _iAngle)
{
	if	(isStunedStatus())
		return;

	if	(m_iAnm == dACT_CHANGE1 && _iAction != dACT_CHANGE2)	
		return;

	if	(_iAction	==	0xffff				)
		return;

	if	(_iAction	>=	dACT_ACTION_1)
	{
		if	(m_wLordActor	==	g_hero.m_iSerial	&&	m_bf1IsMarionette)
			g_hero.m_dwLastReceiveCommandTime	=	timeGetTime();

		if	(g_hero.isPet(m_wSerial) || g_hero.isSummonBeastActor(m_wSerial))
			g_hero.m_dwLastReceiveCommandTime	=	timeGetTime();
	}

	if	(_iAction	==	dACT_DEFAULT_ATTACK	)
		_iAction	=	getBody()->m_iDefaultAttack;
	if	(_iAction	==	dACT_DEFAULT_MAGIC	)
		_iAction	=	getBody()->m_iDefaultMagic;

	setAnm(_iAction);

	if (m_wJob/2	==	dJOB_SUMMONER/2)	//	조련사나 소환사가 아니다.
	{
		if (_iAction==	dACT_RIDE_DOG_ATTACK	||	(_iAction	>=	dACT_ACTION_1 && _iAction	<	dACT_RIDE_DOG_MOVE))	//	액션 1
			m_wRevealTime=	15*dSYNC_FPS;
	}
	else
	{
		if (_iAction	>=	dACT_ACTION_1)	//	액션 1
			m_wRevealTime=	15*dSYNC_FPS;
	}

	m_iDirect			=	getDirect(m_pos.x,m_pos.y,_iX,_iY,TRUE);

	if	(_iAngle	!=	0xffff)
		m_iAngle	=	_iAngle;

	m_isDamageFrame		=	getBody()->IsDamage(m_iAnm,m_iFrame);
	m_isTriggerFrame	=	getBody()->IsTrigger(m_iAnm,m_iFrame);

	if (isMonster() && m_wBody >= dJOB_MONSTER_START && m_wBody <= dJOB_MONSTER_WHITE_SHADOW)
	{
		if (_iAction	>=	dACT_ACTION_1)	//	액션 1
			if (g_rectViewPort.isIn(m_pos.x,m_pos.y))
				g_esm.playMonsterAttackSound(m_wBody-dJOB_MONSTER_START);
	}

}

//
//	앉자
void
CActor::sitDown(int _iX,int _iY,BOOL _isSitDown)
{
	stop(_iX,_iY);

	if (_isSitDown)	action(m_pos.x,m_pos.y,dACT_SITDOWN);
	else			action(m_pos.x,m_pos.y,dACT_READY);

	if (isHero())
		if (!_isSitDown)
			g_hero.m_iLastActionTime	=	s_iFrameCounter;
}

//
//	변신 준비 --a
void
CActor::readyTransformation(SG_TRANSFORMATION *_lpPacket)
{
	int		iJob		=	_lpPacket->bf8TransformationJob;
	int		iSpentCP	=	_lpPacket->wSpentCP;
	int		iPosX		=	_lpPacket->wPosX;
	int		iPosY		=	_lpPacket->wPosY;

	m_wTransformationJob=	iJob;

	if	(isHero())
		g_hero.readyTransformation(iJob,iSpentCP);

	if	(_lpPacket->bWeapon	!=	0xff)
		m_itemTransformationWeapon.m_wBaseItem	=	cBasicItem::GetItemByEquippedShape(_lpPacket->bWeapon,m_wTransformationJob,dEQUIP_WEAPON);
	else
		m_itemTransformationWeapon.m_wBaseItem	=	0xffff;

	if	(_lpPacket->bShield	!=	0xff)
		m_itemTransformationShield.m_wBaseItem	=	cBasicItem::GetItemByEquippedShape(_lpPacket->bShield,m_wTransformationJob,dEQUIP_SHIELD_BULLET);
	else
		m_itemTransformationShield.m_wBaseItem	=	0xffff;

	if	(_lpPacket->bArmor	!=	0xff)
		m_itemTransformationArmor.m_wBaseItem	=	cBasicItem::GetItemByEquippedShape(_lpPacket->bArmor,m_wTransformationJob,dEQUIP_ARMOR);
	else
		m_itemTransformationArmor.m_wBaseItem	=	0xffff;

	if	(_lpPacket->bWeaponEffect	!=	0xff)
		m_itemTransformationWeapon.m_wColorizeEffect	=	_lpPacket->bWeaponEffect;
	else
		m_itemTransformationWeapon.m_wColorizeEffect	=	0xffff;

	stop(iPosX,iPosY);

	if	(isStunedStatus())
	{
		transformation();

		return;
	}

	if(isDeath() && g_bIsDuelServer)
	{
		transformation();
	}
	else if	(_lpPacket->bf1IsRequreOnlyAfterMotion	==	FALSE	&&	getFrameCount(dACT_CHANGE1)	>	0)
	{
		action(iPosX,iPosY,dACT_CHANGE1);
		setExclusiveAction(TRUE);
	}
	else
	{
		transformation();
		return;
	}

	if(_lpPacket->bf6FPS)
		m_iFPS =	_lpPacket->bf6FPS;
}

//
//	장비 모양 체크
BOOL
CActor::checkEquipmentShape()
{
	cANM	*lpBody	=	getBody();

	if	(!lpBody)
		return	FALSE;

	if	(m_iAnm	>=	lpBody->m_iAnmCount)
		setAnm(dACT_READY);

	if	(m_wJob	>=	dPLAYER_JOB_COUNT)
		return	TRUE;

	cBasicItem	*lpArmor	=	m_itemArmor.getBasicItem();
	cBasicItem	*lpWeapon	=	m_itemWeapon.getBasicItem();
	cBasicItem	*lpShield	=	m_itemShield.getBasicItem();

	if	(lpArmor)
		if	(!lpArmor->isEnableJob(m_wJob) || c_aItemEquipPlace[lpArmor->m_wKind]	!=	dEQUIP_ARMOR)
		{
			m_wIsReceivedBasicData	=	FALSE;
			g_am.addObscurityActor(m_wSerial);

			m_iAnm	=	0;
			stop();

			return FALSE;
		}

	if	(lpWeapon)
		if	(!lpWeapon->isEnableJob(m_wJob)	||	c_aItemEquipPlace[lpWeapon->m_wKind]	!=	dEQUIP_WEAPON)
		{
			m_wIsReceivedBasicData	=	FALSE;
			g_am.addObscurityActor(m_wSerial);

			m_iAnm	=	0;
			stop();

			return FALSE;
		}

	if	(lpShield)
		if	(!lpShield->isEnableJob(m_wJob)	||	c_aItemEquipPlace[lpShield->m_wKind]	!=	dEQUIP_SHIELD_BULLET)
		{
			m_wIsReceivedBasicData	=	FALSE;
			g_am.addObscurityActor(m_wSerial);

			m_iAnm	=	0;
			stop();

			return FALSE;
		}

	return	TRUE;
}

//
//	변신
void
CActor::transformation()
{
	m_wJob		=	m_wTransformationJob;
	m_wTrueJob	=	m_wTransformationJob;

	memcpy(&m_itemArmor,&m_itemTransformationArmor,sizeof(m_itemArmor));
	memcpy(&m_itemWeapon,&m_itemTransformationWeapon,sizeof(m_itemArmor));
	memcpy(&m_itemShield,&m_itemTransformationShield,sizeof(m_itemArmor));		//	장비하고 있는 무기,장비하고 있는 방패

	m_wBody			=	getBodyIndex();

	if	(isHero())
		m_wIsReceivedBasicData	=	TRUE;
	
	checkEquipmentShape();

	if	(isHero())
		g_hero.transformation();

	if	(!g_am.checkDeathStatus(this))
		return;

	if	(getFrameCount(dACT_CHANGE2)	>	0)
	{
		action(m_pos.x,m_pos.y,dACT_CHANGE2);
		setExclusiveAction(TRUE);
	}
	else
	{
		stop();
		return;
	}

}

//
//	임의의 방향으로 바꾼다.
void
CActor::randomDirect()
{
	m_iAngle	+=	360;
	if (random(2)==	0)	m_iAngle	+=	45;
	else				m_iAngle	-=	45;

	m_iAngle	%=	360;

	m_iDirect	=	GetDirect(m_iAngle,m_iDirectCount);
}

//
//	target을 공격할만한 적당한 어빌러티를 찾는다.
int
CActor::findSuitableAbility(int target,int range)
{

	return 0;
}

//
//	특정 캐릭터를 공격하라.
BOOL
CActor::attackToObject(int _iTarget,int _iTargetType,cAbility *_lpAbility)
{
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	stop();		//	정지!!

	CPos	posTarget;

	switch(_iTargetType)
	{
		case	eATTACK_OBJECT_ARCA	:
		{
			cArcaInfo	*lpArca		=	g_arcaManager.getArca(_iTarget);

			if (!lpArca)
			{
				AddAskArcaInfo(_iTarget);
				return	FALSE;
			}

			posTarget.x	=	lpArca->m_wX;
			posTarget.y	=	lpArca->m_wY;

			m_wUseSkill	=	dSKILL_BASIC_ATTACK_TO_ARCA;

			break;
		}

		case	eATTACK_OBJECT_DOOR	:
		{
			m_wUseSkill	=	dSKILL_BASIC_ATTACK_TO_DOOR;

			cAreaInfo	*lpArea	=	g_map.m_area.getArea(_iTarget);

			if (lpArea->m_wSerial	==	0xffff)
				break;

			lpArea->getCenter(&posTarget);

			m_wUseSkill	=	dSKILL_BASIC_ATTACK_TO_DOOR;

			break;
		}
	}

	action(posTarget.x,posTarget.y,lpSkill->m_wAction);
	setExclusiveAction(TRUE);	//	기술 사용하는건 에니메이션 하나로 끝난다. 일단은.. --;;
	g_esm.playCastingSound(lpSkill);

	m_wIsDontHitAction	=	lpSkill->isNotInterrupt();

	m_abilityUse.copy(_lpAbility);

	if (isHero())
		g_hero.decreaseBullet(_lpAbility);
	else
	{
		if (m_wUseSkill	==	0xffff)	
			g_am.addObscurityActor(m_wSerial);
	}

	return TRUE;
}

//
//	특정 캐릭터를 공격하라.
BOOL
CActor::attackToActor(CActor *_lpTarget,cAbility *_lpAbility,int _iRange)
{
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	stop();		//	정지!!

	m_wUseSkill		=	g_sm.castAtActor(this,_lpTarget,_lpAbility,_iRange);

	m_abilityUse.copy(_lpAbility);

	if (isHero())
		g_hero.decreaseBullet(_lpAbility);
	else
	{
		if (m_wUseSkill	==	0xffff)	
			g_am.addObscurityActor(m_wSerial);
	}

	return TRUE;
}

//
//	특정 캐릭터를 공격하라.
BOOL
CActor::attackToGroup(CActor *_lpTarget,cAbility *_lpAbility,int _iRange,WORD *_lpwGroup,int _iTargetCount,int _iContinuousShotCount,int _iCriticalCount,int _iMissCount,int _iDoubleCriticalCount, int _iHardBlowCount)
{

	CSkill	*lpSkill	=	_lpAbility->getSkill();

	stop();		//	정지!!

	if	(lpSkill->m_wType==	eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER)
	{
		m_wUseSkill		=	g_sm.castContinuousHitOfDoppelgangerSkill(this,_lpTarget,_lpAbility,_iRange,_lpwGroup,_iTargetCount,_iContinuousShotCount,_iCriticalCount,_iMissCount,_iDoubleCriticalCount, _iHardBlowCount);
		
	}
	else
	{
		m_wUseSkill		=	g_sm.castAtGroup(this,_lpTarget,_lpAbility,_iRange,_lpwGroup,_iTargetCount,_iContinuousShotCount,_iCriticalCount,_iMissCount,_iDoubleCriticalCount, _iHardBlowCount);
	}
	m_abilityUse.copy(_lpAbility);

	if (isHero())
		g_hero.decreaseBullet(_lpAbility);
	else
	{
		if (m_wUseSkill	==	0xffff)	
			g_am.addObscurityActor(m_wSerial);
	}

	return TRUE;
}

//
//	특정 캐릭터를 공격하라.
BOOL
CActor::attackToActorByContinuousAttack(SG_CONTINUOUS_HIT_BY_VALUE	*_lpPacket)
{
	CActor		*lpTarget	=	g_am.getTestedActor(_lpPacket->bf11TargetSerial);

	cAbility	ability;
	CSkill		*lpSkill;

	ability.set(_lpPacket->bf11Skill,_lpPacket->bf7Level);

	lpSkill	=	ability.getSkill();

	if (!isHero())	
		stop(_lpPacket->wPosX,_lpPacket->wPosY);
	else
	if (!lpSkill->m_wIsNotSyncCP)	
		g_hero.reduceCP(ability.getSpentCP());	//	CP 감소

	stop();		//	정지!!

	m_attackInfo.m_wTarget	=	_lpPacket->bf11TargetSerial;
	m_attackInfo.m_ability.set(_lpPacket->bf11Skill,_lpPacket->bf7Level);

	setContinuousAttackDamage(_lpPacket->iPhysicalDamage*100,_lpPacket->iMagicDamage*100,_lpPacket->bf5AttackCount,_lpPacket->bf4MissCount,_lpPacket->bf4CriticalCount,_lpPacket->bf4DoubleCriticalCount,_lpPacket->bf1IsAttackToShakle,
		_lpPacket->wMinPhysicalDamage*100,_lpPacket->wMinMagicDamage*100,_lpPacket->bf4BlockingCount,_lpPacket->bf4DancingBlockCount,_lpPacket->bf4NoActionBlockingCount, NULL, _lpPacket->bf4HardBlowCount);

	if (lpSkill->m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_VALUE	||
		lpSkill->m_wType	==	eSKILL_TYPE_CONTINUOUS_SHOOT		||
		lpSkill->m_wType	==	eSKILL_TYPE_BIT_GLIDER				)
		m_wUseSkill		=	g_sm.castContinuousAttackSkill(this,lpTarget,&ability,_lpPacket->bf5AttackCount,_lpPacket->bf6FPS);

	if (lpSkill->m_wType	==	eSKILL_TYPE_ALTERING_HITTER)
		m_wUseSkill		=	g_sm.castAlteringHitter(this,lpTarget,&ability,_lpPacket->bf5AttackCount,_lpPacket->bf6FPS);

	m_abilityUse.copy(&ability);

	if	(isHero())
		g_hero.decreaseBullet(&ability);
	else
	{
		if	(m_wUseSkill	==	0xffff)	
			g_am.addObscurityActor(m_wSerial);
	}

	return TRUE;
}

//
//	랜드 마커 공격
BOOL
CActor::landMarkerAttack(SG_LAND_MARKER	*_lpPacket)
{
	cAbility	ability;

	ability.set(_lpPacket->wSkill,_lpPacket->wLevel);

	if (!ability.isEnableJob(m_wJob))
	{
		addToObscurityActorList();
		return FALSE;
	}

	if (!isHero())	stop(_lpPacket->wPosX,_lpPacket->wPosY);
	else
	if (!ability.getSkill()->m_wIsNotSyncCP)	g_hero.reduceCP(ability.getSpentCP());	//	CP 감소

	stop();		//	정지!!

	m_attackInfo.m_ability.set(_lpPacket->wSkill,_lpPacket->wLevel);

	m_wUseSkill		=	g_sm.castLandMarkerAttackSkill(this,_lpPacket->wTargetPosX,_lpPacket->wTargetPosY,&ability,_lpPacket->bShotCount,_lpPacket->bFPS);

	m_abilityUse.copy(&ability);

	if (isHero())	g_hero.decreaseBullet(&ability);
	else
	{
		if (m_wUseSkill	==	0xffff)	
			g_am.addObscurityActor(m_wSerial);
	}

	return TRUE;
}

//
//	인터벌 슈터
BOOL
CActor::castIntervalShooter(SG_CAST_INTERVAL_SHOOTER *_lpPacket)
{
	cAbility	ability;

	ability.set(_lpPacket->wSkill,_lpPacket->wLevel);

	if (!ability.isEnableJob(m_wJob))
	{
		addToObscurityActorList();
		return FALSE;
	}

	if (!isHero())	stop(_lpPacket->wPosX,_lpPacket->wPosY);
	else
	if (!ability.getSkill()->m_wIsNotSyncCP)	g_hero.reduceCP(ability.getSpentCP());	//	CP 감소

	stop();		//	정지!!

	m_attackInfo.m_ability.set(_lpPacket->wSkill,_lpPacket->wLevel);

	m_wUseSkill		=	g_sm.castIntervalShooter(this,&ability,_lpPacket->bFPS,_lpPacket->bShotCount);

	m_abilityUse.copy(&ability);

	if (isHero())	g_hero.decreaseBullet(&ability);
	else
	{
		if (m_wUseSkill	==	0xffff)	
			g_am.addObscurityActor(m_wSerial);
	}

	return TRUE;
}

//	체인 라이트닝
BOOL
CActor::operateChainLightningSkill(SG_WIDE_AREA_DAMAGE *_lpPacket)
{
	cAbility	ability;
	CSkill		*lpSkill;
	BOOL		isGetCP		=	TRUE,bIsPlaySound	=	TRUE;

	ability.set(_lpPacket->bf12Skill,_lpPacket->bf8Level);
	lpSkill		=	ability.getSkill();

	WORD		awTargetList[100];

	{
		CActor	*lpTarget	=	g_am.getTestedActor(_lpPacket->aData[0].wActor);

		if	(lpTarget)
			lpTarget->addEffect(lpSkill->m_wSkillImageOutputPart,lpSkill->m_wSkillImage,0xffff,lpSkill->m_wImageScale,lpSkill->m_wImageScale);

		awTargetList[0]		=	_lpPacket->aData[0].wActor;
	}

	for (int i=1;i<_lpPacket->bf6Count;i++)
	{
		if	(i	>=	2)
			bIsPlaySound	=	FALSE;

		cWIDE_AREA_DAMAGE_E	*lpInfo	=	&_lpPacket->aData[i];
		CHitInfo			hitInfo;

		hitInfo.m_dwPhysicalDamage	=	lpInfo->wPhysicalDamage*100;		//	얼마나? 아파? ;;
		hitInfo.m_dwMagicDamage		=	lpInfo->wMagicDamage*100;		//	얼마나? 아파? ;;
		hitInfo.m_dwResultField		=	lpInfo->dwResultField;	//	1 히트,2 크리티컬,4 크러쉬 블로우,8 블럭,16 히트 액션

		awTargetList[i]				=	lpInfo->wActor;

		CActor	*lpTarget			=	g_am.getTestedActor(lpInfo->wActor);

		if	(!lpTarget)
			continue;

		lpTarget->hit(this,&ability,&hitInfo,0,bIsPlaySound);
	}

	CActiveSkill	*lpAS			=		g_sm.getActiveSkill(ability.m_wSkill,m_wSerial);

	if	(!lpAS)
		return	FALSE;

	cAS_ChainLightning	*lpCL		=	(cAS_ChainLightning	*)lpAS;

	lpCL->casting(awTargetList,_lpPacket->bf6Count);

	return	TRUE;
}


//	오우섬 포트리스
BOOL
CActor::operateAwesomeFortressAttack(SG_WIDE_AREA_DAMAGE *_lpPacket)
{
	int			i,iIndex=0;
	cAbility	ability;

	ability.set(_lpPacket->bf12Skill,_lpPacket->bf8Level);

	m_wParallelBunshineCount	=	_lpPacket->bf6Count;

	for (i=0;i<_lpPacket->bf6Count;i++)
	{
		CActor	*lpTarget		=	g_am.getTestedActor(_lpPacket->aData[i].wActor);

		if (!lpTarget)
		{
			g_am.addObscurityActor(_lpPacket->aData[i].wActor);

			continue;
		}

		CPos	pos;
		int		iAngle	=	random(180)+180;
		int		iRange	=	lpTarget->getBodySize()+getBodySize()+40;

		pos.x	=	lpTarget->m_pos.x;
		pos.y	=	lpTarget->m_pos.y;

		GetOvalAnglePos(&pos,iAngle,iRange);

		m_aAfterImageList[iIndex].m_pos.x		=	pos.x;
		m_aAfterImageList[iIndex].m_pos.y		=	pos.y;
		m_aAfterImageList[iIndex].m_bDirect		=	GetDirect((iAngle+180)%360,m_iDirectCount);

		iIndex++;
	}

	m_attackInfo.m_ability.set(ability.m_wSkill,ability.m_wLevel);

	m_abilityUse.copy(&ability);

	m_wUseSkill		=	g_sm.castAwesomeFortressSkill(this,&ability,(void *)_lpPacket->aData,_lpPacket->bf6Count);

	if	(isHero())
	{
		g_hero.decreaseBullet(&ability);
		g_hero.m_iCP	=	(int)_lpPacket->sRemainCP*100;	//	CP 증가
	}
	else
	{
		if (m_wUseSkill	==	0xffff)	
			g_am.addObscurityActor(m_wSerial);
	}
		

	return TRUE;
}

//	바운싱 리니어
BOOL
CActor::operateBouncingLinear(SG_WIDE_AREA_DAMAGE *_lpPacket)
{
	cAbility	ability;

	ability.set(_lpPacket->bf12Skill,_lpPacket->bf8Level);
	m_attackInfo.m_ability.set(ability.m_wSkill,ability.m_wLevel);
	m_abilityUse.copy(&ability);

	m_wUseSkill		=	g_sm.castBouncingLinearSkill(this,&ability,(void *)_lpPacket->aData,_lpPacket->bf6Count);

	if (isHero())
	{
		g_hero.decreaseBullet(&ability);
		g_hero.m_iCP	=	(int)_lpPacket->sRemainCP*100;	//	CP 증가
	}
	else
	{
		if (m_wUseSkill	==	0xffff)	
			g_am.addObscurityActor(m_wSerial);
	}

	return TRUE;
}

BOOL
CActor::operateBigBullet(SG_WIDE_AREA_DAMAGE *_lpPacket)
{
	cAbility	ability;

	ability.set(_lpPacket->bf12Skill,_lpPacket->bf8Level);
	m_attackInfo.m_ability.set(ability.m_wSkill,ability.m_wLevel);
	m_abilityUse.copy(&ability);

	CActiveSkill	*lpAS	=	g_sm.get(m_wUseSkill);

	if	(!lpAS)
		return	FALSE;

	cAS_BigBullet	*lpBB	=	(cAS_BigBullet	*)lpAS;

	lpBB->throwTarget((void *)_lpPacket->aData,_lpPacket->bf6Count);

	if	(lpBB->m_lpBullet)
		action(lpBB->m_lpBullet->m_pos.x,lpBB->m_lpBullet->m_pos.y,ability.getSkill()->m_wAction2);

	return TRUE;
}

//	광역 대미지 공격
BOOL
CActor::operateWideAreaDamage(SG_WIDE_AREA_DAMAGE *_lpPacket)
{
	cAbility	ability;
	CSkill		*lpSkill;
	BOOL		isGetCP		=	TRUE,bIsPlaySound	=	TRUE;

	ability.set(_lpPacket->bf12Skill,_lpPacket->bf8Level);
	lpSkill		=	ability.getSkill();

	for (int i=0;i<_lpPacket->bf6Count;i++)
	{
		if	(i	>=	2)
			bIsPlaySound	=	FALSE;

		cWIDE_AREA_DAMAGE_E	*lpInfo	=	&_lpPacket->aData[i];
		CHitInfo			hitInfo;

		hitInfo.m_dwPhysicalDamage	=	lpInfo->wPhysicalDamage*100;		//	얼마나? 아파? ;;
		hitInfo.m_dwMagicDamage		=	lpInfo->wMagicDamage*100;		//	얼마나? 아파? ;;
		hitInfo.m_dwResultField		=	lpInfo->dwResultField;	//	1 히트,2 크리티컬,4 크러쉬 블로우,8 블럭,16 히트 액션

		CActor	*lpTarget			=	g_am.getTestedActor(lpInfo->wActor);

		if	(!lpTarget)
			continue;

		lpTarget->hit(this,&ability,&hitInfo,0,bIsPlaySound);

		if	(lpTarget->isDeath())
			continue;
	}

	if	(_lpPacket->bf1IsDamagePool	==	FALSE)
		if	(_lpPacket->bf12Caster	==	(WORD)g_hero.m_iSerial && _lpPacket->sRemainCP	!=	0x7fff)
			g_hero.m_iCP	=	(int)_lpPacket->sRemainCP*100;	//	CP 증가

	return	TRUE;
}


//
//	특정 캐릭터를 공격하라.
BOOL
CActor::attackToActorByBunshine(SG_BUNSHINE_ATTACK *_lpPacket)
{
	CActor		*lpTarget	=	g_am.getTestedActor(_lpPacket->bf11TargetSerial);

	cAbility	ability;

	ability.set(_lpPacket->bf11Skill,_lpPacket->bf7Level);

	if (!isHero())
		stop(_lpPacket->wPosX,_lpPacket->wPosY);
	else
	if (!ability.getSkill()->m_wIsNotSyncCP)
		g_hero.reduceCP(ability.getSpentCP());	//	CP 감소

	stop();		//	정지!!

	m_attackInfo.m_wTarget	=	_lpPacket->bf11TargetSerial;
	m_attackInfo.m_ability.set(_lpPacket->bf11Skill,_lpPacket->bf7Level);

	setContinuousAttackDamage(_lpPacket->iPhysicalDamage,_lpPacket->iMagicDamage,_lpPacket->bf5AttackCount,_lpPacket->bf4MissCount,_lpPacket->bf4CriticalCount,_lpPacket->bf4DoubleCriticalCount,_lpPacket->bf1IsAttackToShakle,
							_lpPacket->wMinPhysicalDamage*100,_lpPacket->wMinMagicDamage*100,_lpPacket->bf4BlockingCount,_lpPacket->bf4DancingBlockCount,_lpPacket->bf4NoActionBlockingCount, NULL, _lpPacket->bf4HardBlowCount);

	m_wUseSkill		=	g_sm.castBunshineAttackSkill(this,lpTarget,&ability,_lpPacket->bf5AttackCount);

	m_abilityUse.copy(&ability);

	if (isHero())
		g_hero.decreaseBullet(&ability);
	else
	{
		if (m_wUseSkill	==	0xffff)	
			g_am.addObscurityActor(m_wSerial);
	}

	return TRUE;
}

//
//	특정 캐릭터를 공격하라.
BOOL
CActor::attackToActorByScimitarCutting(SG_SCIMITAR_CUTTING *_lpPacket)
{
	CActor		*lpTarget	=	g_am.getTestedActor(_lpPacket->bf11TargetSerial);
	cAbility	ability;

	ability.set(_lpPacket->bf11Skill,_lpPacket->bf7Level);

	if	(!isHero())
		stop(_lpPacket->wPosX,_lpPacket->wPosY);
	else
	if	(!ability.getSkill()->m_wIsNotSyncCP)
		g_hero.reduceCP(ability.getSpentCP());	//	CP 감소

	stop();		//	정지!!

	m_attackInfo.m_wTarget	=	_lpPacket->bf11TargetSerial;
	m_attackInfo.m_ability.set(_lpPacket->bf11Skill,_lpPacket->bf7Level);

	setContinuousAttackDamage(_lpPacket->iPhysicalDamage,_lpPacket->iMagicDamage,_lpPacket->bf5AttackCount,_lpPacket->bf4MissCount,_lpPacket->bf4CriticalCount,_lpPacket->bf4DoubleCriticalCount,_lpPacket->bf1IsAttackToShakle,
								_lpPacket->wMinPhysicalDamage*100,_lpPacket->wMinMagicDamage*100,_lpPacket->bf4BlockingCount,_lpPacket->bf4DancingBlockCount,_lpPacket->bf4NoActionBlockingCount, NULL, _lpPacket->bf4HardBlowCount);

	m_wUseSkill				=	g_sm.castScimitarCuttingSkill(this,lpTarget,&ability,_lpPacket->bf5AttackCount,_lpPacket->wRange);

	m_abilityUse.copy(&ability);

	if	(isHero())
		g_hero.decreaseBullet(&ability);
	else
	{
		if (m_wUseSkill	==	0xffff)	
			g_am.addObscurityActor(m_wSerial);
	}

	return TRUE;
}

void
CActor::setContinuousAttackDamage(DWORD _dwPhysicalDamage,DWORD _dwMagicDamage,int _iCount,int _iMissCount,int _iCriticalCount,int _iDoubleCriticalCount,BOOL _bIsAttackToShakle,
								  int _iMinPhysicalDamage,int _iMinMagicDamage,int _iBlockingCount,int _iDancingBlockingCount,int _iNoActionBlockingCount,CHitInfo *_lpHitInfoList, int _iHardBlowCount)
{
	if	(_lpHitInfoList	==	NULL)
		_lpHitInfoList	=	m_attackInfo.m_aInfo;


	int		i;
	BYTE	abIsAttackInfo[100];
	int		iMissCount				=	_iMissCount;
	int		iCiriticalCount			=	_iCriticalCount;
	int		iDoubleCriticalCount	=	_iDoubleCriticalCount;
	int		iBlockingCount			=	_iBlockingCount;
	int		iDancingBlockingCount	=	_iDancingBlockingCount;
	int		iNoActionBlockingCount	=	_iNoActionBlockingCount;
	int		iHardBlowCount			=	_iHardBlowCount;

	BOOL	bIsRedColor						=	FALSE;
	int		iRealHitCount					=	_iCount-_iMissCount-_iBlockingCount;
	int		iIsIncreaseCriticalDamage		=	0;
	int		iIsIncreaseCriticalDamagePerHit	=	0;
	int		iPhysicalDamagePerHit			=	0;
	int		iMagicDamagePerHit				=	0;
	int		iNormalHit						=	0;
	int		iShareCount						=	0;
	memset(abIsAttackInfo,0,sizeof(abIsAttackInfo));

	if	(iRealHitCount	<	0	||	_iCount	>=	100)
		return;
	iNormalHit			=	iRealHitCount - _iCriticalCount - _iDoubleCriticalCount - _iHardBlowCount;
	iShareCount			=	iNormalHit+_iCriticalCount*2+_iDoubleCriticalCount*4 + _iHardBlowCount*3;
	
	if(_dwPhysicalDamage && iShareCount)
		iPhysicalDamagePerHit	=	_dwPhysicalDamage/iShareCount;
	
	if(iPhysicalDamagePerHit >= dMAX_PHYSICAL_DAMAGE)
	{
		iIsIncreaseCriticalDamage	=	iPhysicalDamagePerHit%dMAX_PHYSICAL_DAMAGE;
		if(iIsIncreaseCriticalDamage)
		{
			int iCiriticalCount = _iCriticalCount*2+_iDoubleCriticalCount*4;
			if(iCiriticalCount)
				iIsIncreaseCriticalDamagePerHit = iIsIncreaseCriticalDamage/iCiriticalCount;
		}
		iPhysicalDamagePerHit		=	dMAX_PHYSICAL_DAMAGE;
		
	}
	if(_dwMagicDamage && iRealHitCount)
		iMagicDamagePerHit		=	_dwMagicDamage/iRealHitCount;

	if	(_iMissCount	>	_iCount)
	{
		s_agent.sendDebugMessage(_ms("setContinuousAttackDamage 1 c %d,m %d,c %d,d %d",_iCount,_iMissCount,_iCriticalCount,_iDoubleCriticalCount));
		return;
	}
	if	(_iCriticalCount+_iDoubleCriticalCount + _iHardBlowCount	>	iRealHitCount)
	{
		s_agent.sendDebugMessage(_ms("setContinuousAttackDamage 2 c %d,m %d,c %d,d %d",_iCount,_iMissCount,_iCriticalCount,_iDoubleCriticalCount));
		return;
	}

	int iCountRange = _iCount;
	std::vector<BYTE> vecAttackInfo;
	int j =0;
	for(j=0;j<_iCount;++j)
		vecAttackInfo.push_back(j);

	for(j=0;j<_iCount;++j)
	{
		if(iCountRange <= 0)
			break;

		int		iAttackInfoIndex	= random(iCountRange);
		int		iIndex				= vecAttackInfo[iAttackInfoIndex];

		if(iMissCount)
		{
			abIsAttackInfo[iIndex] = eATTACK_RESULT_MISS;
			--iMissCount;
		}
		else if(iCiriticalCount)
		{
			abIsAttackInfo[iIndex] = eATTACK_RESULT_CIRITICAL;
			--iCiriticalCount;
		}
		else if(iDoubleCriticalCount)
		{
			abIsAttackInfo[iIndex] = eATTACK_RESULT_LUCKY_DOUBLE_CIRITICAL;
			--iDoubleCriticalCount;
		}
		else if(iBlockingCount)
		{
			abIsAttackInfo[iIndex] = eATTACK_RESULT_BLOCKING;
			--iBlockingCount;
		}
		else if(iDancingBlockingCount)
		{
			abIsAttackInfo[iIndex] = eATTACK_RESULT_NO_ACTION_BLOCKING;
			--iDancingBlockingCount;
		}
		else if(iNoActionBlockingCount)
		{
			abIsAttackInfo[iIndex] = eATTACK_RESULT_DANCING_BLOCKING;
			--iNoActionBlockingCount;
		}
		else if(iHardBlowCount)
		{
			abIsAttackInfo[iIndex] = eATTACK_RESULT_HARD_BLOW;
			--iHardBlowCount;
		}
		else
		{
			abIsAttackInfo[iIndex] = eATTACK_RESULT_HIT;
		}

		--iCountRange;

		vecAttackInfo[iAttackInfoIndex] = vecAttackInfo[iCountRange];
		vecAttackInfo[iCountRange]		= iIndex;
	}

	vecAttackInfo.clear();

	for (i=0;i<_iCount;i++)
	{
		_lpHitInfoList[i].reset();
		_lpHitInfoList[i].m_dwResultField		=	0;

		if	(abIsAttackInfo[i]	==	eATTACK_RESULT_MISS)
			continue;
		int iPhysicalDamage	=	0;
		int iMagicDamage	=	0;

		int		iRandRange		=	11;
		float	fStartRange		=	95.0f;
		if(iPhysicalDamagePerHit<dMAX_PHYSICAL_DAMAGE)
		{
			iPhysicalDamage			=	iPhysicalDamagePerHit	* ((fStartRange + random(iRandRange))/100);
			iMagicDamage			=	iMagicDamagePerHit		* ((fStartRange + random(iRandRange))/100);
		}
		else
		{
			iPhysicalDamage			=	iPhysicalDamagePerHit;
		}
		
		if(iPhysicalDamagePerHit)
		{
			iPhysicalDamage		=	max(iPhysicalDamage,1);	
			iPhysicalDamage		=	min(_dwPhysicalDamage,iPhysicalDamage);
			iPhysicalDamage		=	min(iPhysicalDamage, dMAX_PHYSICAL_DAMAGE);
		}
		if(iMagicDamagePerHit)
		{
			iMagicDamage		=	max(iMagicDamage,1);
			iMagicDamage		=	min(_dwMagicDamage,iMagicDamage);
			iMagicDamage		=	min(iMagicDamage, dMAX_MAGICAL_DAMAGE);
		}


		if	(abIsAttackInfo[i] == eATTACK_RESULT_CIRITICAL)
		{
			iPhysicalDamage	=	2 * (iPhysicalDamage + iIsIncreaseCriticalDamagePerHit);

		}
		else if	(abIsAttackInfo[i] == eATTACK_RESULT_LUCKY_DOUBLE_CIRITICAL)
		{
			iPhysicalDamage	=	4 * (iPhysicalDamage + iIsIncreaseCriticalDamagePerHit);

		}
		
		else if	(abIsAttackInfo[i] == eATTACK_RESULT_HARD_BLOW)
		{
			iPhysicalDamage	=	3 * iPhysicalDamage;
		}

		_lpHitInfoList[i].m_dwPhysicalDamage		=	iPhysicalDamage;
		_lpHitInfoList[i].m_dwMagicDamage			=	iMagicDamage;



		_lpHitInfoList[i].m_dwPhysicalDamage		=	max(_lpHitInfoList[i].m_dwPhysicalDamage,_iMinPhysicalDamage);
		_lpHitInfoList[i].m_dwMagicDamage			=	max(_lpHitInfoList[i].m_dwMagicDamage,_iMinMagicDamage);	

		if	(iPhysicalDamage+iMagicDamage > 0)
		{
			_lpHitInfoList[i].setHit();

			if	(abIsAttackInfo[i] == eATTACK_RESULT_LUCKY_DOUBLE_CIRITICAL)
				_lpHitInfoList[i].setLuckyDoubleCritical();
			else
			if	(abIsAttackInfo[i] == eATTACK_RESULT_CIRITICAL)
				_lpHitInfoList[i].setCritical();
			else
			if	(abIsAttackInfo[i] == eATTACK_RESULT_BLOCKING)
				_lpHitInfoList[i].setBlock();
			else
			if	(abIsAttackInfo[i] == eATTACK_RESULT_NO_ACTION_BLOCKING)
				_lpHitInfoList[i].setDancingBlockerBlock();
			else
			if	(abIsAttackInfo[i] == eATTACK_RESULT_DANCING_BLOCKING)
				_lpHitInfoList[i].setNoActionBlock();
			else
			if	(abIsAttackInfo[i] == eATTACK_RESULT_HARD_BLOW)
				_lpHitInfoList[i].setHardBlow();
		}

		if	(_bIsAttackToShakle)
			_lpHitInfoList[i].setAttackToShakle();
	}
}



//
//	특정 장소를 공격하라.
BOOL
CActor::attackToGround(int _iX,int _iY,cAbility *_lpAbility,int _iRange)
{
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	stop();		//	정지!!

	m_wUseSkill		=	g_sm.castAtGround(this,_iX,_iY,_lpAbility,_iRange);

	m_abilityUse.copy(_lpAbility);

	if (isHero())	g_hero.decreaseBullet(_lpAbility);
	else
	{
		if (m_wUseSkill	==	0xffff)	
			g_am.addObscurityActor(m_wSerial);
	}

	return TRUE;
}

//
//	퀵 액션을 사용한다.
BOOL
CActor::quickAction(cAbility *_lpAbility)
{
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	stop();		//	정지!!

	m_wUseSkill		=	g_sm.castQuick(this,_lpAbility);

	m_abilityUse.copy(_lpAbility);

	if (isHero())	g_hero.decreaseBullet(_lpAbility);
	else
	{
		if (m_wUseSkill	==	0xffff)	
			g_am.addObscurityActor(m_wSerial);
	}

	return TRUE;
}

/*
BOOL
CActor::Passive(CActor	*lpAttacker,cAbility *lpAbility)
{
	if (ReduceBlocker())	return TRUE;	//	블로커가 있어서.. 가드를 할 수 있다면 오케~ 오케~~

	if (m_iPostureOfDefense	==	dGUARD_DEFENSE)		//	가드하고 있는 상태다.
	{
		m_iDirect	=	getDirect(m_pos.x,m_pos.y,lpAttacker->m_pos.x,lpAttacker->m_pos.y,TRUE);

		return TRUE;
	}

	cAbility	*lpDA	=	GetPassiveAbility();

	if (isVisibleShield())
	{
		if (m_sFreezeTime			)	return FALSE;
//		if (random(1)		!=	0	)	return FALSE;
//		if (g_aSkill[lpDA->m_wSkill].m_wAction	==	0xffff)	return FALSE;	//	걍.. 맞는 기술이다.. ??
		int defense	=	g_aSkill[lpDA->m_wSkill].PassiveType();

		if (g_aSkill[lpDA->m_wSkill].m_wType	==	dSKILL_TYPE_AVOID)
		{
			m_TargetPos.x	=	m_pos.x;
			m_TargetPos.y	=	m_pos.y;
			setTarget(lpAttacker->m_wSerial);
			if (useSkill(m_wTarget,m_wPassiveAbility))	return TRUE;

			return FALSE;
		}

		switch(defense)
		{
			case	dDEFNESE_SHIELD						:		//	방패로 막는다.
				m_TargetPos.x	=	m_pos.x;
				m_TargetPos.y	=	m_pos.y;
				setTarget(lpAttacker->m_wSerial);
				useSkill(m_wTarget,m_wPassiveAbility);
				return TRUE;
				break;

			case	dDEFNESE_HEAT						:		//	맞으면 상태가 변한다.
				return FALSE;
				break;

			case	dDEFNESE_HIT_AND_COUNTER			:		//	맞으면서.. 카운터가 있음 날린다.
				m_TargetPos.x	=	m_pos.x;
				m_TargetPos.y	=	m_pos.y;

				setTarget(lpAttacker->m_wSerial);

				useSkill(m_wTarget,m_wPassiveAbility);
				return FALSE;
				break;

			case	dDEFNESE_CRITICAL_SHIELD			:		//	크리티컬만 막는다.
				if (!m_isCriticalHit)	return FALSE;

				m_TargetPos.x	=	m_pos.x;
				m_TargetPos.y	=	m_pos.y;
				setTarget(lpAttacker->m_wSerial);
				if (useSkill(m_wTarget,m_wPassiveAbility))	return TRUE;
				break;

			case	dDEFNESE_CRITICAL_HEAT				:		//	크리티컬만 맞으면 상태가 변한다.
				if (!m_isCriticalHit)	return FALSE;
				return FALSE;
				break;

			case	dDEFNESE_CRITICAL_HIT_AND_COUNTER	:		//	크리티컬을 맞으면 발동한다.
				if (!m_isCriticalHit)	return FALSE;

				m_TargetPos.x	=	m_pos.x;
				m_TargetPos.y	=	m_pos.y;

				setTarget(lpAttacker->m_wSerial);

				useSkill(m_wTarget,m_wPassiveAbility);

				return FALSE;
				break;
		}
	}

	return FALSE;
}	//	CActor::Passive(CActor	*lpAttacker,cAbility *lpAbility)
*/

//
//	_iX,_iY로 밀려 났다.
void
CActor::thrust(int _iX,int _iY,int _iSkill,BOOL _bIsBlocked, BOOL	_bIsChangeTargetDirect)
{
	int	iRange	=	GetRange(_iX,_iY,m_pos.x,m_pos.y);
	int	iAngle	=	cANGLE::GetAngleToTarget(_iX,_iY,m_pos.x,m_pos.y);
	int iDirect =	 m_iDirect;

	if(_bIsChangeTargetDirect)
		iDirect = getDirect(_iX,_iY,TRUE);

	if(_bIsChangeTargetDirect)
		setPos(_iX,_iY);
	else
		stop(_iX,_iY);

	addSkillHitEffect(_iSkill);

	if	(isHero())
	{
		g_hero.m_posLast.x	=	_iX;
		g_hero.m_posLast.y	=	_iY;
	}

	iRange		=	sqrt(iRange);

	afterImageOn(dFADE_AFTER_IMAGE,dAFTER_IMAGE_COUNT,32,1,2);


	for (int i=0;i<min(iRange/20,dAFTER_IMAGE_COUNT);i++)
	{
		CPos	pos;

		pos.x	=	m_pos.x;
		pos.y	=	m_pos.y;

		GetAnglePos(&pos,iAngle,i*20);

		m_aAfterImageList[i].m_pos.x		=	pos.x;
		m_aAfterImageList[i].m_pos.y		=	pos.y;
		m_aAfterImageList[i].m_bAnm			=	m_iAnm;
		m_aAfterImageList[i].m_bFrame		=	m_iFrame;

		m_aAfterImageList[i].m_bDirect		=	iDirect;
		m_aAfterImageList[i].m_bAlphaDepth	=	32-i;
	}

	if	(_bIsBlocked)
		g_shaker.add(m_pos.x,m_pos.y,3,500,100);

}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//	정보 구함
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
cANM*
CActor::getBody()
{
	if	(m_bIsTransOther)
		m_bIsTransOther	=	TRUE;
	else
		m_bIsTransOther	=	FALSE;

	cANM	*lpBody	=	NULL;
	
	for	 (int i=2;i;)
	{
		--i;

		if	(m_enchantedImage.m_bf1IsZombieBomb)
			lpBody	=	GetBody(dJOB_MONSTER_ZOMBIE);
		else if	(m_enchantedImage.m_bf1IsRabbit)
			lpBody	=	GetBody(dJOB_NPC_RABBIT);
		else if	(m_enchantedImage.m_bf1IsPlump)
			lpBody	=	GetBody(dJOB_NPC_FAT_GIRL);
		else if	(m_enchantedImage.m_bf1IsFrog)
			lpBody	=	GetBody(dJOB_NPC_FROG);
		else if (m_enchantedImage.m_bf1IsTransformationHunter)
			lpBody	=	GetBody(dJOB_MONSTER_DARK_ELF);
		else if (m_enchantedImage.m_bf1IsTransformationThief)
			lpBody	=	GetBody(dJOB_MONSTER_ASSASSIN);
		else if (m_enchantedImage.m_bf1IsTransformationMagician)
			lpBody	=	GetBody(dJOB_MONSTER_FALLEN_WIZARD);
		else if (m_enchantedImage.m_bf1IsTransformationReptile)
			lpBody	=	GetBody(dJOB_MONSTER_REPTILE);
		else
			lpBody	=	GetBody(m_wBody);

		if	(lpBody == NULL || lpBody->m_pAnmData == NULL)
			g_im.loadIMonsterBody(m_wBody,0);
		else
			break;
	}

	return	lpBody;
}
BOOL
CActor::isFocus(int _iX,int _iY)
{
	if	(m_wBody	==	dJOB_MONSTER_TIAMATH_FEELER	||	m_wBody	==	dJOB_MONSTER_TIAMATH_BOTTOM_FEELER)
	{
		if	(g_am.m_iIsHideTimathFeelerTime)
			return	FALSE;
	}

	if	(m_wBody	>=	dJOB_MONSTER_ShieldCraft	&&	m_wBody	<=	dJOB_MONSTER_HollowKnight)
		return	FALSE;

	if	(m_bf1IsHide	||	m_enchantedImage.m_bf1IsTransToWeapon)
		return	FALSE;

	if	(isNpc() && m_bf1IsTalker == FALSE)
		return	FALSE;

	_iX	-=	m_pos.x;
	_iY	-=	m_pos.y;

	cRECT	rectSelect	=	getBody()->m_rectSelect;

	int	iHorizonScale	=	m_wHorizonScale;
	int	iVerticalScale	=	m_wVerticalScale;

	if	(g_bIsHalfSize)
	{
		iHorizonScale	*=	2;
		iVerticalScale	*=	2;
	}

	if	(g_config.m_bIsDoubleSizeTargetting)
	{
		iHorizonScale	*=	2;
		iVerticalScale	*=	2;
	}

	rectSelect.x1	=	rectSelect.x1*iHorizonScale/100;
	rectSelect.x2	=	rectSelect.x2*iHorizonScale/100;
	rectSelect.y1	=	rectSelect.y1*iVerticalScale/100;
	rectSelect.y2	=	rectSelect.y2*iVerticalScale/100;

	if	(rectSelect.isIn(_iX,_iY))
		return	TRUE;

	int	iHeight	=	getHeight();

	if	(iHeight)
		return	getBody()->m_rectSelect.isIn(_iX,_iY+iHeight);

	return	FALSE;
}

//
//	시야에 보이는 적을 구한다.
int
CActor::getViewEnemy()
{
	return 0;
}

//
//	방향을 구한다.
int
CActor::getDirect(int x,int y,BOOL setAngle)
{
	int	iAngle;

	if (m_pos.x == x && m_pos.y == y)
		return	min(m_iDirect,m_iDirectCount);

	iAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,x,y,2);

	if (setAngle)
		m_iAngle	=	iAngle;

	iAngle		=	GetDirect(iAngle,m_iDirectCount);

	return	iAngle;
}

//
//	방향을 구한다.
int
CActor::getDirect(int x1,int y1,int x2,int y2,BOOL _bIsSetAngle)
{
	int	iAngle;

	if	(x1 == x2 && y1 == y2)
		return	m_iDirect;

	iAngle		=	cANGLE::GetAngleToTarget(x1,y1,x2,y2,2);

	if (_bIsSetAngle)	m_iAngle	=	iAngle;

	return	GetDirect(iAngle,m_iDirectCount);
}

//
//	방향을 구한다.
//	int		angle	-	각도
int
CActor::getDirect(int _iAngle,BOOL _bIsSetAngle)
{
	if (_bIsSetAngle)	m_iAngle	=	_iAngle;

	return	GetDirect(m_iAngle,m_iDirectCount);
}

//
//	타격 포인트.. 미사일 종류가 날아갈때 타겟으로 잡아야할 부분
void
CActor::getHitPoint(CPos *_lpPos)
{
	_lpPos->x	=	m_pos.x+getBody()->m_rectCrash.x1	+	getBody()->m_rectCrash.getWidth()/2;
	_lpPos->y	=	m_pos.y+getBody()->m_rectCrash.y1	+	getBody()->m_rectCrash.getHeight()/2;
}


//
//	충돌 박스를 고려해서 _lpActor이 _iRange 안에 있는지 체크한다.
BOOL
CActor::isTouchAbleTarget(CActor *_lpActor,int _iRange,int _iMinRange)
{
	int				iCurrentRange;

	_iRange			+=	_lpActor->getBodySize();
	_iRange			+=	getBodySize();
	_iRange			*=	_iRange;
	_iMinRange		*=	_iMinRange;

	iCurrentRange	=	GetOvalRange(m_pos.x,m_pos.y,_lpActor->m_pos.x,_lpActor->m_pos.y);

//	이동 중이면 다음 위치를 고려한다.
	if (_lpActor->m_isMove)
	{
		c4POINT			posNext;

		_lpActor->getNextPos(&posNext);

		int	iNextRange	=	GetOvalRange(m_pos.x,m_pos.y,posNext.x,posNext.y);

		if (iNextRange	>	iCurrentRange)	iCurrentRange	=	iNextRange;
	}

	if (iCurrentRange	<	_iMinRange)
		return FALSE;

	if (iCurrentRange	>	_iRange)
		return FALSE;

	return	TRUE;
}

//
//	충돌 박스를 고려해서 x,y까지가 range 안에 있는지 체크한다.
BOOL
CActor::isTouchAblePos(int _iX,int _iY,int _iRange)
{
	_iRange	+=	getBodySize();
	_iRange	*=	_iRange;

	if (GetOvalRange(m_pos.x,m_pos.y,_iX,_iY)	<=	_iRange)	return TRUE;

	return	FALSE;
}

//
//	타겟이 죽었다.
void
CActor::targetDeath()
{

}

//
//	타겟이 제거 되었다.
void
CActor::targetRemove()
{
	m_attackInfo.reset();

	if (m_wUseSkill	==	dSKILL_ILLUSION_ATTACK)	m_wUseSkill	=	0xffff;
}

//
//	잘못된 이름
BOOL
CActor::isIncorrectName()
{
	if (STRICMP(m_strName,"EMPTY")==0	)	return	TRUE;
	if (STRICMP(m_strName,"dummy!!")==0	)	return	TRUE;
	if (strstr(m_strName,"　")			)	return	TRUE;

	return	FALSE;
}


//
//	몸에 발생하는 타격 효과를 업데이트 한다.
void
CActor::updateEffect()
{
//	g_hero.addFriend("고구마",0);

	if	(m_wWhiteFadeStep)
	{
		m_wWhiteFadeStep	+=2	;

		if	(m_wWhiteFadeStep	>=	200)
			m_wWhiteFadeStep	=	0;
	}

	int		i;

	if (m_wBlockerAngle	<	360)	//	블러커 앵글
	{
		if (m_wBlockerRigidityTime	>	0	)	m_wBlockerRigidityTime--;
		else
		{
			m_wBlockerAngle+=2;

			if (m_wBlockerAngle		>=	360	)	m_wBlockerAngle	=	0;
		}
	}

	if (m_wAuraImage	!=	0xffff)	
		if	(!g_hem.update(m_wAuraImage))
		{
			g_hem.remove(m_wAuraImage);
			m_wAuraImage	=	0xffff;
		}

	for (i = 0;i < dHIT_EFFECT_COUNT_ON_CHARACTER; i++)	//	타격 효과 갱신
	{
		if	(m_aHitEffect[i]	==	0xffff)
			continue;

		if	(!g_hem.update(m_aHitEffect[i]))
		{
			if	(i	==	m_wShutInMagicBoxImage)
				m_wShutInMagicBoxImage	=	0xffff;

			g_hem.remove(m_aHitEffect[i]);
			m_aHitEffect[i]			=	0xffff;
		}
	}

	if (m_dwEnchantedMask	&	dAMEM_IS_SITDOWN)
	{
		if (isSitdown()	==	FALSE)
			sitDown(m_pos.x,m_pos.y,TRUE);
	}
	else
	{
		if (isSitdown()	==	TRUE)
			sitDown(m_pos.x,m_pos.y,FALSE);
	}

	if ((m_dwEnchantedMask	&	dAMEM_SHIMMERING_SHIELD)&&	m_wBlockerAngle == 0xffff)
	{
		m_wBlockerShape			=	0;
		m_wBlockerAngle			=	random(360);
		m_wBlockerRigidityTime	=	0;
	}

	if	(m_wBody	!=	dJOB_MONSTER_SAGE)
		if	((m_dwEnchantedMask	&	dAMEM_SPEAR_BLOCKER)	&&	m_wBlockerAngle == 0xffff)
		{
			m_wBlockerShape			=	0;
			m_wBlockerAngle			=	random(360);
			m_wBlockerRigidityTime	=	0;
		}

	m_sLevitateHeight	=	0;

	BOOL	bIsLevitate	=	isEnchantedLevitate();

	if	(bIsLevitate	==	FALSE)
		if	(m_wBody	==	dJOB_MONSTER_ARCH_DEVIL	&&	(m_iAnm	==	dACT_ARCH_DEVIL_CRAW	||	m_iAnm	==	dACT_ARCH_DEVIL_THROW_STAFF))
			bIsLevitate	=	TRUE;

	if	(bIsLevitate	&&	isDeath()	==	FALSE	&&	m_enchantedImage.m_bf1IsFeignDeath	==	FALSE)
	{
		m_sLevitateHeight	=	(20+dSIN_TABLE[(s_iFrameCounter*3)%360]*5/256);

		if	(m_wBody	==	dJOB_MONSTER_ARCH_DEVIL)
		{
			m_sLevitateHeight	=	80;

			if	(m_iAnm	==	dACT_ARCH_DEVIL_FLY)
				m_sLevitateHeight	=	0;
		}
	}
}

//
//	체력 줄어듦
void
CActor::reduceHP(int _iDamage,BOOL _bIsCheckSyncHP,cAbility *_lpAbility)
{
	if	(g_bIsObserverModeForDuel)
		return;

	if (s_bIsGuildBattleField)
	{
		if (_bIsCheckSyncHP && !isPetEngageTarget() && !isHero() && !g_hero.isPet(m_wSerial) && !g_hero.isOwnForce(this))
		{
			if (!_lpAbility					)
				return;

			if (!_lpAbility->isHpSyncSkill())
				return;
		}
	}
	else
	{
		if (_bIsCheckSyncHP && !isPetEngageTarget() && !isHero() && !g_hero.isPet(m_wSerial))
		{
			if (!_lpAbility					)
				return;

			if (!_lpAbility->isHpSyncSkill())
				return;
		}
	}

	if (m_iFirstAidHP	!=	0)
	{
		if (_iDamage	<=	m_iFirstAidHP)
		{
			m_iFirstAidHP-=	_iDamage;
			_iDamage	=	0;
		}
		else
		{
			_iDamage	-=	m_iFirstAidHP;
			m_iFirstAidHP=	0;
		}

		if (m_iFirstAidHP	<=	0)
			m_iFirstAidHP	=	0;
	}

	m_iHP	-=	_iDamage;
	m_iHP	=	max(m_iHP,1);

	if	(isHero())
		g_hero.m_iHP	=	m_iHP;
}

int
CActor::getBodyIndex()	
{
	m_wBodyVariation	=	0;

	if	(m_wJob	<	dPLAYER_JOB_COUNT)
		m_wBody	=	m_wJob*3;	//	플레이어는 몸뚱아리가 세개씩.. (이런식으로 관리하면 안되는디.. -_-)
	else
	if	(m_wJob	>=	dJOB_KELBY_1 && m_wJob	<=	dJOB_HEDGER_3 )	//	소환수
		m_wBody	=	dBODY_KELBY_1+m_wJob-dJOB_KELBY_1;
	else
	if	(m_wJob	>=	dJOB_OPERATOR_ANGEL_MALE && m_wJob	<=	dJOB_OPERATOR_ANGEL_FEMALE )	//	소환수
		m_wBody	=	dBODY_OPERATOR_ANGEL_MALE+m_wJob-dJOB_OPERATOR_ANGEL_MALE;
	else
	{
		m_wBody	=	m_wJob;

		if	(m_wJob	>=	dJOB_MONSTER_Exist_Variation_begin)
			m_wBodyVariation	=	getJob()->m_iVariation;
	}

	if	(m_wBody	<	dBODY_HERO_JOB_END)
	{
		cBasicItem	*lpBasicItem	=	m_itemArmor.getBasicItem();

		if	(lpBasicItem)
			m_wBody	+=	lpBasicItem->m_wEquippedShape;

		if	(m_wBody==	dBODY_BEAST_TAMER2)	
			m_wBody	=	dBODY_BEAST_TAMER1;
		if	(m_wBody==	dBODY_SUMMONER1)	
			m_wBody	=	dBODY_BEAST_TAMER1;
		if	(m_wBody==	dBODY_SUMMONER2)	
			m_wBody	=	dBODY_BEAST_TAMER1;
		if	(m_wBody==	dBODY_SUMMONER3)	
			m_wBody	=	dBODY_BEAST_TAMER3;
		if	(m_wBody==	dBODY_FIGHTER1)		
			m_wBody	=	dBODY_ROGUE1;
		if	(m_wBody==	dBODY_FIGHTER2)		
			m_wBody	=	dBODY_ROGUE2;
		if	(m_wBody==	dBODY_FIGHTER3)		
			m_wBody	=	dBODY_ROGUE3;
	}

	return	m_wBody;
}
