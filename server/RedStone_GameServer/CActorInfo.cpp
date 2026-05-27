#include "cACTOR.H"
#include "cFIELD.H"
#include "debugCode.H"
#include "Client.h"
#include "cPACKET_GAMESERVER.h"
#include "secretDungeon.h"
#include "CGuild.h"
#include "actor_mini_pet.h"
#include "cEVENT.h"
#include "rebirth_define.H"

//	무기의 사정거리 얻기
int
cACTOR::getWeaponAttackRange()
{
	cItem	*lpWeapon	=	getWeapon();

	if (!lpWeapon)
		return	0;

	return	lpWeapon->getBasicItem(TRUE)->m_wRange;
}

//
//	지현씨가 관리하는 클라이언트 구하기
CClient*
cACTOR::getClient()
{
	CClient	*lpClient	=	CM.mGETCLIENT(m_iClientSerial);

	if	(lpClient && lpClient->GetActorSerialInServer() == (DWORD)m_iZoneSerial)
		return	lpClient;

	return	NULL;
}

//	커뮤니티 서버 시리얼 구하기 
int
cACTOR::getBCSerial()
{
	CClient	*lpClient	=	CM.mGETCLIENT(m_iClientSerial);

	if	(lpClient && lpClient->GetActorSerialInServer() == (DWORD)m_iZoneSerial)
		return	lpClient->GetBCSerial();

	return	0xffff;
}

//
//	NPC얻어오기
cNPC*
cACTOR::getNpc()
{
	if	(m_wNpcActorSaveSerial	==	0xffff)	
		return	NULL;

	return	m_lpField->m_apNpcGenerateData[m_wNpcActorSaveSerial];
}

cCHARACTER*
cACTOR::getCharacter()
{
	cNPC*	lpNpc	=	getNpc();

	if	(!lpNpc)
		return	NULL;

	return	&m_lpField->m_aCharacter[lpNpc->m_wCharacter];
}

//
//	방향을 구한다.
int
cACTOR::getDirect(int _iX,int _iY,BOOL _isSetAngle)
{
	int	iAngle;

	if (m_pos.x == _iX	&& m_pos.y == _iY)
		return	m_wDirect;

	iAngle		=	g_protractor.getAngleToTarget(m_pos.x,m_pos.y,_iX,_iY);

	if (_isSetAngle)
		m_wAngle	=	iAngle;

	return	GetDirect(iAngle,dDEFAULT_DIRECT_COUNT);
}

//
//	액터의 정보를 간략화 해서 _lpInfo에 담아준다.
void
cACTOR::getSimplePlayerInfo(CSimplePlayerInfo *_lpInfo)
{
	if	(getMaxHP() == 0)	
		m_maxHP.m_iValue	=	m_iMaxHP;

	LONGLONG	llRestHP	=	m_iHP;
	llRestHP	*=	255;
	llRestHP	/=	getMaxHP();

	strcpy(_lpInfo->strName,m_strName);

	_lpInfo->bf2Kind			=	eAK_PLAYER;
	_lpInfo->bf8RestHp			=	llRestHP;
	_lpInfo->bf16MaxHp			=	getMaxHP()/100;
	_lpInfo->bf10Job			=	m_wTrueJob;
	_lpInfo->bf3Direct			=	m_wDirect;
	_lpInfo->bf1IsEventMob		=	FALSE;
	_lpInfo->bf1IsHide			=	m_bf1IsHide;

	if	(m_wOperatorLevel	>=	eAL_SYSTEM_OPERATOR)
		_lpInfo->bf1IsHide	=	TRUE;

	if	(m_bf10Costume		!=	0)
		_lpInfo->bf10Job	=	m_bf10Costume;

	if	(m_wGuildSerial		==	0xffff)
		_lpInfo->bf12GuildSerial=	4001;
	else
		_lpInfo->bf12GuildSerial=	m_wGuildSerial;

	_lpInfo->bf5MiniPetType					=	nsMiniPetType::Count;
	_lpInfo->bf5MiniPetType2				=	nsMiniPetType::Count;
	_lpInfo->bf4RebirthCount				=	m_bf4RebirthCount;
	
	if	(g_eventManager.m_wIsDisplayNameWithSoundOfLeadersBell && g_iBoostExpByLeadersSoundofBell > 100)
		_lpInfo->bf1IsSoundOfLedersBellsLeader	=	m_bf1IsSoundOfLedersBellsLeader;
	else
		_lpInfo->bf1IsSoundOfLedersBellsLeader	=	FALSE;

	if	(m_bf5ActiveMiniPet	<	c_iMiniPetCount)
		if	(m_aMiniPet[m_bf5ActiveMiniPet].m_bf7Level)
			_lpInfo->bf5MiniPetType		=	m_aMiniPet[m_bf5ActiveMiniPet].m_bf5Type;

	if	(m_bf5ActiveMiniPet2	<	c_iMiniPetCount)
		if	(m_aMiniPet[m_bf5ActiveMiniPet2].m_bf7Level)
			_lpInfo->bf5MiniPetType2		=	m_aMiniPet[m_bf5ActiveMiniPet2].m_bf5Type;

	_lpInfo->bf2Team			=	m_wTeam;
	_lpInfo->bf11SerialInField	=	m_wSerialInField;
	_lpInfo->bf5Scale			=	(BYTE)m_wScaleX/10;
	_lpInfo->bf3OperatorLevel	=	m_wOperatorLevel;

	_lpInfo->enchantedImage.copy(&m_enchantedImage);

	_lpInfo->wItemEffect		=	m_wItemEffect;
	_lpInfo->dwEnchantedMask	=	m_dwEnchantedMask;
	_lpInfo->wX					=	m_pos.x;
	_lpInfo->wY					=	m_pos.y;
	_lpInfo->bf14FirstAidHP		=	m_iFirstAidHP/100;
	_lpInfo->bf2RideDog			=	0;
	_lpInfo->bf10Level			=	getLevel();
	_lpInfo->bf5MagicCarpetShape=	m_bf5MagicCarpetShape;
	_lpInfo->bf3GuildPetType	=	m_wGuildPetType;

	cParty	*lpParty	=	getParty();

	if(lpParty)	
	{
		if(getParty()->m_wUnionPartyserial != 0xffff)
			_lpInfo->bf1IsUnionPartyUser	=	TRUE;
	}
	else
		_lpInfo->bf1IsUnionPartyUser	=	FALSE;

	if	(isSitdown())
		_lpInfo->dwEnchantedMask|=	dAMEM_IS_SITDOWN;

	if	(m_wPitchmanShopSerial	!=	0xffff)
	{
		_lpInfo->bf3Direct		=	dDIRECT_S;
		_lpInfo->dwEnchantedMask|=	dAMEM_IS_SITDOWN;
	}

	if	(m_bf1IsEvilSpritOfChristmas)
	{
		_lpInfo->bf1IsEventMob	=	TRUE;
		_lpInfo->bf5Scale		=	200/10;
	}

	if	(m_wRidingDogTime)
	{
		cACTOR	*lpKelby		=	getKelby();

		if (lpKelby)
			_lpInfo->bf2RideDog	=	lpKelby->m_wSummonBeastGrade+1;
	}
	
	if	(m_iHP)	
		_lpInfo->bf8RestHp	=	max(_lpInfo->bf8RestHp,1);

	cItem	*lpWeapon		=	getWeapon();
	cItem	*lpShield		=	getShield();
	cItem	*lpArmor		=	getEquipment(dEQUIP_ARMOR);

	_lpInfo->bf4Weapon		=	dVAILD_EQUIPMENT_VALUE_FOR_SAI;
	_lpInfo->bf4Shield		=	dVAILD_EQUIPMENT_VALUE_FOR_SAI;
	_lpInfo->bf2Armor		=	dVAILD_EQUIPMENT_VALUE_FOR_ARMOR;
	_lpInfo->bf8WeaponEffect=	0;
	_lpInfo->bf3ArmorPalette=	0;
	_lpInfo->bf4BlockerShape=	m_bf4BlockerShape;
	_lpInfo->bf1IsCopyActor	=	isCopyActor();

	if	(m_wMarionetteTime)
		_lpInfo->bf1IsMarionette=	TRUE;
	else
		_lpInfo->bf1IsMarionette=	FALSE;

	if	(lpWeapon)
	{
		_lpInfo->bf4Weapon		=	lpWeapon->getBasicItem(TRUE)->m_wEquippedShape;
		_lpInfo->bf8WeaponEffect=	getWeaponColorizeEffect();
	}

	if	(lpShield)
	{
		if (m_wCastBlocker		==	0xffff)
			_lpInfo->bf4Shield	=	lpShield->getBasicItem()->m_wEquippedShape;
	}

	if	(lpArmor)
	{
		_lpInfo->bf2Armor		=	lpArmor->getBasicItem()->m_wEquippedShape;
		_lpInfo->bf3ArmorPalette=	lpArmor->getBasicItem()->m_wPaletteIndex;
	}

	if (m_wSlaveActor != 0xffff)
	{
		_lpInfo->wX				=	m_posFeignDeath.x;
		_lpInfo->wY				=	m_posFeignDeath.y;
	}

	switch(m_bf4Skin+eNS_OPERATOR_MALE-1)
	{
		case	eNS_OPERATOR_MALE	:		//	남자 천사
		case	eNS_OPERATOR_FEMALE	:		//	여자 천사
			_lpInfo->bf10Job	=	m_bf4Skin-1+dJOB_OPERATOR_ANGEL_MALE;
			break;
	}
}
int
cACTOR::getBitCount(WORD _wSerialInField, DWORD	_dwNameHashCode)
{
	return	m_bitInfoManager.getCount(_wSerialInField,_dwNameHashCode);
}

//	액터의 정보를 간략화 해서 _lpInfo에 담아준다.
void
cACTOR::getVerySimplePlayerInfo(CVerySimplePlayerInfo *_lpInfo)
{
	_lpInfo->bf2Kind		= eAK_PLAYER;
	_lpInfo->bf10Job		= m_wTrueJob;
	_lpInfo->bf3Direct		= m_wDirect;

	if (m_bf10Costume		!=	0)
		_lpInfo->bf10Job	=	m_bf10Costume;
		
	cItem	*lpWeapon		=	getWeapon();
	cItem	*lpShield		=	getShield();
	cItem	*lpArmor		=	getEquipment(dEQUIP_ARMOR);

	_lpInfo->bf5MiniPetType		=	nsMiniPetType::Count;
	_lpInfo->bf5MiniPetType2	=	nsMiniPetType::Count;
	_lpInfo->bf4RebirthCount	=	m_bf4RebirthCount;

	if	(g_eventManager.m_wIsDisplayNameWithSoundOfLeadersBell && g_iBoostExpByLeadersSoundofBell > 100)
		_lpInfo->bf1IsSoundOfLedersBellsLeader	=	m_bf1IsSoundOfLedersBellsLeader;
	else
		_lpInfo->bf1IsSoundOfLedersBellsLeader	=	FALSE;

	if	(m_bf5ActiveMiniPet	<	c_iMiniPetCount)
		if	(m_aMiniPet[m_bf5ActiveMiniPet].m_bf7Level)
			_lpInfo->bf5MiniPetType		=	m_aMiniPet[m_bf5ActiveMiniPet].m_bf5Type;
	if	(m_bf5ActiveMiniPet2	<	c_iMiniPetCount)
		if	(m_aMiniPet[m_bf5ActiveMiniPet2].m_bf7Level)
			_lpInfo->bf5MiniPetType2		=	m_aMiniPet[m_bf5ActiveMiniPet2].m_bf5Type;

	_lpInfo->bf4Weapon			=	dVAILD_EQUIPMENT_VALUE_FOR_SAI;
	_lpInfo->bf4Shield			=	dVAILD_EQUIPMENT_VALUE_FOR_SAI;
	_lpInfo->bf2Armor			=	dVAILD_EQUIPMENT_VALUE_FOR_ARMOR;
	_lpInfo->bf8WeaponEffect	=	0;
	_lpInfo->bf3ArmorPalette	=	0;
	_lpInfo->bf2RideDog			=	0;
	_lpInfo->bf1IsDeath			=	isDeath();
	_lpInfo->bf1IsEventMob		=	FALSE;
	_lpInfo->bf4MagicCarpetShape=	m_bf5MagicCarpetShape;
	_lpInfo->bf4BlockerShape	=	m_bf4BlockerShape;
	_lpInfo->bf1IsHide			=	m_bf1IsHide;
	_lpInfo->bf1IsCopyActor		=	isCopyActor();
	_lpInfo->bf3GuildPetType	=	m_wGuildPetType;

	cParty*	lpParty =	getParty();

	if (lpParty)
	{
		if(getParty()->m_wUnionPartyserial != 0xffff)
			_lpInfo->bf1IsUnionPartyUser	=	TRUE;
	}
	else
		_lpInfo->bf1IsUnionPartyUser	=	FALSE;

	if	(m_wOperatorLevel	>=	eAL_SYSTEM_OPERATOR)
		_lpInfo->bf1IsHide	=	TRUE;

	if	(m_wMarionetteTime)
		_lpInfo->bf1IsMarionette=	TRUE;
	else
		_lpInfo->bf1IsMarionette=	FALSE;

	if	(isSitdown())
		_lpInfo->dwEnchantedMask|=	dAMEM_IS_SITDOWN;

	if	(m_wPitchmanShopSerial	!=	0xffff)
	{
		_lpInfo->bf3Direct		=	dDIRECT_S;
		_lpInfo->dwEnchantedMask|=	dAMEM_IS_SITDOWN;
	}

	if	(m_wRidingDogTime)
	{
		cACTOR	*lpKelby	=	getKelby();

		if (lpKelby)
			_lpInfo->bf2RideDog	=	lpKelby->m_wSummonBeastGrade+1;
	}

	if	(lpWeapon)
	{
		_lpInfo->bf4Weapon		=	lpWeapon->getBasicItem(TRUE)->m_wEquippedShape;
		_lpInfo->bf8WeaponEffect=	getWeaponColorizeEffect();
	}
	if	(lpShield)
	{
		if (m_wCastBlocker		==	0xffff)
			_lpInfo->bf4Shield	=	lpShield->getBasicItem()->m_wEquippedShape;
	}

	if	(lpArmor)
	{
		_lpInfo->bf2Armor		=	lpArmor->getBasicItem()->m_wEquippedShape;
		_lpInfo->bf3ArmorPalette=	lpArmor->getBasicItem()->m_wPaletteIndex;
	}

	_lpInfo->bf2Team			= m_wTeam;
	_lpInfo->bf11SerialInField	= m_wSerialInField;
	_lpInfo->bf5Scale			= (BYTE)m_wScaleX/10;
	_lpInfo->bf3OperatorLevel=	m_wOperatorLevel;

	if (m_bf1IsEvilSpritOfChristmas)
	{
		_lpInfo->bf1IsEventMob	=	TRUE;
		_lpInfo->bf5Scale		=	200/10;
	}

	_lpInfo->enchantedImage.copy(&m_enchantedImage);
	_lpInfo->wItemEffect		= m_wItemEffect;
	_lpInfo->dwEnchantedMask	= m_dwEnchantedMask;
	_lpInfo->wX					= m_pos.x;
	_lpInfo->wY					= m_pos.y;

	if	(m_wSlaveActor != 0xffff)
	{
		_lpInfo->wX				=	m_posFeignDeath.x;
		_lpInfo->wY				=	m_posFeignDeath.y;
	}

	switch(m_bf4Skin+eNS_OPERATOR_MALE-1)
	{
		case	eNS_OPERATOR_MALE	:		//	남자 천사
		case	eNS_OPERATOR_FEMALE	:		//	여자 천사
			_lpInfo->bf10Job	=	m_bf4Skin-1+dJOB_OPERATOR_ANGEL_MALE;
			break;
	}
}

//
//	액터의 정보를 간략화 해서 _lpInfo에 담아준다.
void
cACTOR::getSimpleMonsterInfo(CSimpleMonsterInfo *_lpInfo)
{
	if (getMaxHP() == 0)
		m_maxHP.m_iValue	=	m_iMaxHP;

	LONGLONG	llRestHP	= m_iHP;
	llRestHP	*=	255;
	llRestHP	/=	getMaxHP();

	strcpy(_lpInfo->strName,m_strName);

	_lpInfo->bf2Kind			= eAK_MONSTER;
	_lpInfo->bf8Scale			= m_wScaleX;
	_lpInfo->bf10Level			= getLevel();
	_lpInfo->bf11SerialInField	= m_wSerialInField;
	_lpInfo->bf1IsPet			= isPet();
	_lpInfo->bf1IsRegen			= FALSE;
	_lpInfo->bf11Tamer			= m_wTamer;
	_lpInfo->bf1IsBoss			= m_wIsBoss;

	_lpInfo->bf8RestHp			= llRestHP;
	_lpInfo->bf24MaxHp			= getMaxHP()/100;

	_lpInfo->bf11Job			=	m_wTrueJob;
	_lpInfo->bf3Direct			=	m_wDirect;
	_lpInfo->bf2Team			=	m_wTeam;
	_lpInfo->bf1IsCopyActor		=	isCopyActor();

	cNPC		*lpNpc			=	getNpc();

	if	(lpNpc)
		_lpInfo->bf1IsHideName	=	lpNpc->m_bf1IsHideName;
	else
		_lpInfo->bf1IsHideName	=	FALSE;

	if	(m_wMarionetteTime)
		_lpInfo->bf1IsMarionette=	TRUE;
	else
		_lpInfo->bf1IsMarionette=	FALSE;

	_lpInfo->wX					= m_pos.x;
	_lpInfo->wY					= m_pos.y;
	_lpInfo->dwEnchantedMask	= m_dwEnchantedMask;
	_lpInfo->enchantedImage.copy(&m_enchantedImage);

	{
		llRestHP				=	m_iFirstAidHP;	
		llRestHP				=	llRestHP*getMaxHP()/1000;
		_lpInfo->bf10FirstAidHP	=	llRestHP;
	}

	if	(m_wRidingDogTime)
		_lpInfo->bf1RideTamer	=	TRUE;
	else
		_lpInfo->bf1RideTamer	=	FALSE;

	if (m_iHP)
		_lpInfo->bf8RestHp	=	max(_lpInfo->bf8RestHp,1);
	
	m_iLastSyncHP			=	m_iHP;
	m_iLastSyncMaxHP		=	getMaxHP()/100;
}

//
//	액터의 정보를 간략화 해서 _lpInfo에 담아준다.
void
cACTOR::getVerySimpleMonsterInfo(CVerySimpleMonsterInfo *_lpInfo)
{
	_lpInfo->bf2Kind			=	eAK_MONSTER;
	_lpInfo->bf8Scale			=	m_wScaleX;
	_lpInfo->bf11SerialInField	=	m_wSerialInField;
	_lpInfo->bf11Job			=	m_wTrueJob;
	_lpInfo->bf1IsPet			=	isPet();

	_lpInfo->bf3Direct			=	m_wDirect;
	_lpInfo->bf2Team			=	m_wTeam;
	_lpInfo->bf1IsDeath			=	isDeath();
	_lpInfo->bf1IsCopyActor		=	isCopyActor();
	_lpInfo->bf1IsBoss			=	m_wIsBoss;

	cNPC		*lpNpc			=	getNpc();

	if	(lpNpc)
		_lpInfo->bf1IsHideName	=	lpNpc->m_bf1IsHideName;
	else
		_lpInfo->bf1IsHideName	=	FALSE;

	if	(m_wMarionetteTime)
		_lpInfo->bf1IsMarionette=	TRUE;
	else
		_lpInfo->bf1IsMarionette=	FALSE;

	_lpInfo->wX					=	m_pos.x;
	_lpInfo->wY					=	m_pos.y;
	_lpInfo->dwEnchantedMask	=	m_dwEnchantedMask;
	_lpInfo->bf1IsMove			=	m_bIsMoving;
	_lpInfo->enchantedImage.copy(&m_enchantedImage);

	if	(m_wRidingDogTime)
		_lpInfo->bf1RideTamer	=	TRUE;
	else
		_lpInfo->bf1RideTamer	=	FALSE;
}

//
//	액터의 정보를 간략화 해서 _lpInfo에 담아준다.
void
cACTOR::getSimpleNpcInfo(CSimpleNpcInfo *_lpInfo)
{
	if	(getMaxHP() == 0)
		m_maxHP.m_iValue	=	m_iMaxHP;

	LONGLONG	llRestHP	= m_iHP;
	llRestHP	*=	255;
	llRestHP	/=	getMaxHP();

	strcpy(_lpInfo->strName,m_strName);

	_lpInfo->bf2Kind			=	eAK_NPC;
	_lpInfo->bf8Scale			=	(BYTE)m_wScaleX;
	_lpInfo->bf11SerialInField	=	m_wSerialInField;

	_lpInfo->bf11Job			=	m_wTrueJob;
	_lpInfo->bf3Direct			=	m_wDirect;
	_lpInfo->bf6ActorKind		=	m_wActorKind;
	_lpInfo->bf2Team			=	m_wTeam;

	_lpInfo->wX					=	m_pos.x;
	_lpInfo->wY					=	m_pos.y;
	_lpInfo->bf1IsTalker		=	TRUE;

	cCHARACTER	*lpCharacter	=	getCharacter();
	cNPC		*lpNpc			=	getNpc();

	if	(lpNpc)
	{
		if	(!lpNpc->m_pSpeech)	
			_lpInfo->bf1IsTalker	=	FALSE;

		_lpInfo->bf1IsHideName		=	lpNpc->m_bf1IsHideName;
	}

	if	(lpCharacter	&&	lpCharacter->m_bf8Skin)
	{
		switch(lpCharacter->m_bf8Skin)
		{
			case	eNS_KNIGHT			:		//	견습 기사
			case	eNS_WARRIOR			:		//	전사
			case	eNS_WIZARD			:		//	마법사
			case	eNS_WEREWOLF		:		//	늑대인간
			case	eNS_PRIEST			:		//	사제
			case	eNS_FALLEN_ANGEL	:		//	타락천사
			case	eNS_ROGUE			:		//	도둑
			case	eNS_FIGHTER			:		//	무도가
			case	eNS_MAGIC_LANCER	:		//	마법 창검사
			case	eNS_MAGIC_ARCHER	:		//	마법 궁수
			case	eNS_BEAST_TAMER		:		//	조련사
			case	eNS_SUMMONER		:		//	소환사
			case	eNS_PRINCESS		:		//	공주
			case	eNS_MAGICAL_GIRL	:		//	마법 소녀
			case	eNS_NECROMANCER		:		//	네크로맨서
			case	eNS_DEVIL			:		//	악마
				_lpInfo->bf11Job	=	lpCharacter->m_bf8Skin-1;
				break;

			case	eNS_OPERATOR_MALE	:		//	남자 천사
			case	eNS_OPERATOR_FEMALE	:		//	여자 천사
				_lpInfo->bf11Job	=	lpCharacter->m_bf8Skin-eNS_OPERATOR_MALE+dJOB_OPERATOR_ANGEL_MALE;
				break;
			case	eNS_SOUL_BRINGER :
			case	eNS_CHAMPION :	// 투사
				_lpInfo->bf11Job	=	lpCharacter->m_bf8Skin - 3;
				break;
		}
	}
}

//
//	액터의 정보를 간략화 해서 _lpInfo에 담아준다.
void
cACTOR::getVerySimpleNpcInfo(CVerySimpleNpcInfo *_lpInfo)
{
	_lpInfo->bf2Kind			= eAK_NPC;
	_lpInfo->bf8Scale			= (BYTE)m_wScaleX;
	_lpInfo->bf11SerialInField	= m_wSerialInField;
	_lpInfo->bf11Job			= m_wTrueJob;
	
	_lpInfo->bf3Direct			= m_wDirect;
	_lpInfo->bf6ActorKind		= m_wActorKind;

	_lpInfo->wX					=	m_pos.x;
	_lpInfo->wY					=	m_pos.y;
	_lpInfo->bf1IsTalker		=	TRUE;

	cCHARACTER	*lpCharacter	=	getCharacter();
	cNPC		*lpNpc			=	getNpc();

	if	(lpNpc)
	{
		if	(!lpNpc->m_pSpeech)	
			_lpInfo->bf1IsTalker	=	FALSE;
		_lpInfo->bf1IsHideName		=	lpNpc->m_bf1IsHideName;
	}

	if	(lpCharacter	&&	lpCharacter->m_bf8Skin)
	{
		switch(lpCharacter->m_bf8Skin)
		{
			case	eNS_KNIGHT			:		//	견습 기사
			case	eNS_WARRIOR			:		//	전사
			case	eNS_WIZARD			:		//	마법사
			case	eNS_WEREWOLF		:		//	늑대인간
			case	eNS_PRIEST			:		//	사제
			case	eNS_FALLEN_ANGEL	:		//	타락천사
			case	eNS_ROGUE			:		//	도둑
			case	eNS_FIGHTER			:		//	무도가
			case	eNS_MAGIC_LANCER	:		//	마법 창검사
			case	eNS_MAGIC_ARCHER	:		//	마법 궁수
			case	eNS_BEAST_TAMER		:		//	조련사
			case	eNS_SUMMONER		:		//	소환사
			case	eNS_PRINCESS		:		//	공주
			case	eNS_MAGICAL_GIRL	:		//	마법 소녀
			case	eNS_NECROMANCER		:		//	네크로맨서
			case	eNS_DEVIL			:		//	악마
				_lpInfo->bf11Job	=	lpCharacter->m_bf8Skin-1;
				break;

			case	eNS_OPERATOR_MALE	:		//	남자 천사
			case	eNS_OPERATOR_FEMALE	:		//	여자 천사
				_lpInfo->bf11Job	=	lpCharacter->m_bf8Skin-eNS_OPERATOR_MALE+dJOB_OPERATOR_ANGEL_MALE;
				break;
			case	eNS_SOUL_BRINGER :	// 영술사
			case	eNS_CHAMPION :	// 투사
				_lpInfo->bf11Job	=	lpCharacter->m_bf8Skin - 3;
				break;
		}
	}
}

//
//	액터의 정보를 간략화 해서 _lpInfo에 담아준다.
void
cACTOR::getMinumumActorInfo(CMinimumActorInfo *_lpInfo)
{
	_lpInfo->bf11SerialInField	=	m_wSerialInField;
	_lpInfo->bf2Team			=	m_wTeam;
	_lpInfo->wX					=	m_pos.x;
	_lpInfo->wY					=	m_pos.y;
	
	_lpInfo->bf1IsNpc			=	isNpc();
	_lpInfo->wNpcActorSerial	=	m_wNpcActorSaveSerial;

	if	(m_wSlaveActor != 0xffff)
	{
		_lpInfo->wX				=	m_posFeignDeath.x;
		_lpInfo->wY				=	m_posFeignDeath.y;
	}
}

//
//	액터의 정보를 간략화 해서 _lpInfo에 담아준다.
void
cACTOR::getEventNpcInfo(cEventNpcInfo *_lpInfo)
{
	_lpInfo->bf11Serial	=	m_wSerialInField;
	_lpInfo->bf7Type	=	m_wActorKind;
	_lpInfo->wX			=	m_pos.x;
	_lpInfo->wY			=	m_pos.y;
}

//
//	액터의 정보를 간략화 해서 _lpInfo에 담아준다.
void
cACTOR::getActorInfoForOuterActor(CActorInfoForOuterActor *_lpInfo)
{
	_lpInfo->wSerialInField	= m_wSerialInField;
	_lpInfo->wX				= m_pos.x;
	_lpInfo->wY				= m_pos.y;
	_lpInfo->dwEnchantedMask= m_dwEnchantedMask;

	if (m_wSlaveActor != 0xffff)
	{
		_lpInfo->wX				=	m_posFeignDeath.x;
		_lpInfo->wY				=	m_posFeignDeath.y;
	}
}

//
//	현재 사용중인 스킬
CActiveSkill*
cACTOR::getActiveSkill()
{
	if	(m_wUseSkill	==	0xffff || !m_lpField)
		return	NULL;

	return	m_lpField->m_skillManager.getActiveSkill(m_wUseSkill);
}

//
//	천적 플레이어
BOOL
cACTOR::isNEPlayer(cACTOR *_lpPlayer)
{
	if (!_lpPlayer)
		return	FALSE;

	if	(isAbsoluteEvil() && _lpPlayer->isAbsoluteGood())
		return	TRUE;

	if	(_lpPlayer->isAbsoluteEvil() && isAbsoluteGood())
		return	TRUE;

	return	FALSE;
}

//
//	제대로 된넘?
BOOL
cACTOR::isCorrectClient(CClient *_lpClient)
{
	if (!_lpClient)
	{
		_lpClient	=	getClient();

		if	(!_lpClient)	return	FALSE;
	}

	if (m_iClientSerial		!=	_lpClient->GetSerial()				)
		return	FALSE;
	if ((DWORD)m_iZoneSerial!=	_lpClient->GetActorSerialInServer()	)
		return	FALSE;

	if (STRCMP(m_strName,_lpClient->GetName())	!=	0			)
		return	FALSE;
	if (STRICMP(m_strId,_lpClient->GetID())		!=	0			)
		return	FALSE;

	return	TRUE;
}


//
//	체크섬을 구한다.
int
cACTOR::getCheckSumValueForSaveData()
{
	int		i,j,iCheckSum;

	iCheckSum	=	m_wItemCount+m_iGold+m_iLevel+m_iExperience+m_dwSkillExperience+m_wLevelPoint+m_sAllignment;

	for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
	{
		iCheckSum	+=	m_aEquip[i].m_wBaseItem;

		if (i	==	dEQUIP_SHIELD_BULLET)	
				iCheckSum	-=	1;
		else	iCheckSum	-=	m_aEquip[i].m_bCount;

		iCheckSum	+=	m_aEquip[i].m_bDurability;
		iCheckSum	-=	m_aEquip[i].m_aOption[0];
		iCheckSum	+=	m_aEquip[i].m_aOption[1];

		for (j=0;j<dITEM_PREFIX_COUNT;j++)
		{
			iCheckSum	-=	m_aEquip[i].m_aPrefix[j].m_wPrefix;	//	추가효과 3개
			iCheckSum	+=	m_aEquip[i].m_aPrefix[j].m_aValue[0];//	추가효과 3개
			iCheckSum	-=	m_aEquip[i].m_aPrefix[j].m_aValue[1];	//	추가효과 3개
		}
	}

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		iCheckSum	+=	m_aItems[i].m_wBaseItem;
		iCheckSum	-=	m_aItems[i].m_bCount;
		iCheckSum	+=	m_aItems[i].m_bDurability;
		iCheckSum	-=	m_aItems[i].m_aOption[0];
		iCheckSum	+=	m_aItems[i].m_aOption[1];

		for (j=0;j<dITEM_PREFIX_COUNT;j++)
		{
			iCheckSum	-=	m_aItems[i].m_aPrefix[j].m_wPrefix;	//	추가효과 3개
			iCheckSum	+=	m_aItems[i].m_aPrefix[j].m_aValue[0];//	추가효과 3개
			iCheckSum	-=	m_aItems[i].m_aPrefix[j].m_aValue[1];	//	추가효과 3개
		}
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];

		iCheckSum	+=	lpItem->m_wBaseItem;
		iCheckSum	-=	lpItem->m_bCount;
		iCheckSum	+=	lpItem->m_bDurability;
		iCheckSum	-=	lpItem->m_aOption[0];
		iCheckSum	+=	lpItem->m_aOption[1];

		for (j=0;j<dITEM_PREFIX_COUNT;j++)
		{
			iCheckSum	-=	lpItem->m_aPrefix[j].m_wPrefix;	//	추가효과 3개
			iCheckSum	+=	lpItem->m_aPrefix[j].m_aValue[0];//	추가효과 3개
			iCheckSum	-=	lpItem->m_aPrefix[j].m_aValue[1];	//	추가효과 3개
		}
	}

	return	iCheckSum;
}

//
//	기본 능력치 
int
cACTOR::getBasicPowerValue(int _iType)
{
	switch(_iType)
	{
		case	eABP_LEVEL				:
			return	getLevel();
		case	eABP_STRENGTH			:
			return	getStrength();
		case	eABP_DEXTERITY			:
			return	getAgility();
		case	eABP_CONSTITUTION		:
			return	getConstitution();
		case	eABP_WISDOM				:
			return	getWisdom();
		case	eABP_INTELLIGENCE		:
			return	getIntelligence();
		case	eABP_CHARISMA			:
			return	getCharisma();
		case	eABP_LUCK				:
			return	getLuck();
		case	eABP_ATTACK				:
			return	getPhysicalAttackPower(NULL,0,FALSE,FALSE,NULL,FALSE,NULL);
		case	eABP_DEFENSE			:
			return	getDefensivePower();
		case	eABP_FIRE_RESIST		:
			return	getFireResistance();
		case	eABP_WATER_RESIST		:
			return	getWaterResistance();
		case	eABP_WIND_RESIST		:
			return	getWindResistance();
		case	eABP_EARTH_RESIST		:
			return	getEarthResistance();
		case	eABP_LIGHT_RESIST		:
			return	getLightResistance();
		case	eABP_DARK_RESIST		:
			return	getDarkResistance();
		case	eABP_MAGIC_RESIST		:
			return	getAllMagicResistance();
	}

	return	0;
}

//
//	지금 변신 가능하냐?
int
cACTOR::isAbleToTransformation(BOOL _bIgnoreExclusive)
{
	if	(m_wJob	>=	dPLAYER_JOB_COUNT	)
		return	eTRANSFORMATION_FAILED_BY_NOT_AVAIL_JOB;
	if	(!c_aFreeJobChangeJob[m_wJob]	&&	getCPLevel()<dLOW_CP_LEVEL_FOR_TRANSFORMATIION)
		return	eTRANSFORMATION_FAILED_BY_LOW_CP_LEVEL;

	int		iAnotherJob				=	getAnotherJob();
	BOOL	bIsExistAnotherJobSkill	=	FALSE;

	for (int i=0;i<dHERO_ABILITY_COUNT;i++)
		if	(m_aAbility[i].isEnableJob(iAnotherJob))
		{
			bIsExistAnotherJobSkill	=	TRUE;
			break;
		}

	if	(!bIsExistAnotherJobSkill)
		return	eTRANSFORMATION_FAILED_BY_SECOND_JOB_SKILL_NOT_FOUND;

	if	(_bIgnoreExclusive	==	FALSE	&&	m_isExclusiveAction		)
		return	eTRANSFORMATION_FAILED_BY_YET_NOT_COMPLETE_ACTION;

	return	eTRANSFORMATION_SUCCESS;
}

int
cACTOR::getFixedLevelDeathPeneltySecondByGuild(int _iRecverTime)
{
	CGuild*	lpGuild	=	getGuild();
	
	if(lpGuild)
	{
		if(m_lpField->m_wIsVillage)
			return	_iRecverTime;

		//길드 홀 마다 부활 시간을 줄여준다.
		switch(lpGuild->m_wHallLevel)
		{
		case 1:
			_iRecverTime	+= _iRecverTime * 0.2f;
			break;
		case 2:
			_iRecverTime	+= _iRecverTime * 0.5f;
			break;
		case 3:
			_iRecverTime	+= _iRecverTime * 0.6f;
			break;
		case 4:
			_iRecverTime	+= _iRecverTime * 0.7f;
			break;
		case 5:
			_iRecverTime	+= _iRecverTime * 0.8f;
			break;
		}
	}

	return	_iRecverTime;
}

int
cACTOR::getCurrentLevelDeathPeneltySecond()
{
	for (int iIndex=0;iIndex<20;iIndex++)
	{
		if (m_iLevel >= g_aPlayerDeathPeneltyInfo[iIndex].m_iBeginLevel &&
			m_iLevel <= g_aPlayerDeathPeneltyInfo[iIndex].m_iEndLevel)
		{
			int	iValue			=	g_aPlayerDeathPeneltyInfo[iIndex].m_iPeneltySecond;
			int	iCorrectPenelty	=	m_wReduceDeathPeneltyValueByTitle;

			CGuild	*lpGuild	=	getGuild();

			if	(lpGuild)
			{
				iCorrectPenelty	+=	lpGuild->m_wDecreaseDeathPenelty;
			}

			iCorrectPenelty		=	max(iCorrectPenelty,0);

			iValue				-=	iCorrectPenelty*iValue/100;
			iCorrectPenelty		=	iMiniPet()->getDecreaseDeathPeneltyValue();
			iValue				-=	iCorrectPenelty*iValue/100;

			return	iValue;
		}
	}

	return	0;
}

int
cACTOR::getCurrentLevelDeathPeneltyValue()
{
	if	(m_bf12DeathPeneltySecond == 0	||	m_wAnimatePartnerTime)
		return 0;

	for (int iIndex=0;iIndex<20;iIndex++)
	{
		if (m_iLevel >= g_aPlayerDeathPeneltyInfo[iIndex].m_iBeginLevel &&
			m_iLevel <= g_aPlayerDeathPeneltyInfo[iIndex].m_iEndLevel)
		{
			int	iValue			=	g_aPlayerDeathPeneltyInfo[iIndex].m_iPeneltyValue;
			int	iCorrectPenelty	=	m_wReduceDeathPeneltyValueByTitle;

			CGuild	*lpGuild	=	getGuild();

			if	(lpGuild)
			{
				iCorrectPenelty	+=	lpGuild->m_wDecreaseDeathPenelty;
			}

			iCorrectPenelty		=	max(iCorrectPenelty,0);

			iValue				-=	iCorrectPenelty*iValue/100;
			iCorrectPenelty		=	iMiniPet()->getDecreaseDeathPeneltyValue();
			iValue				-=	iCorrectPenelty*iValue/100;

			return	iValue;
		}
	}

	return	0;
}

int
cACTOR::getCurrentLevelDeathPeneltyRecoverPrice()
{
	if (m_bf12DeathPeneltySecond == 0)
		return 0;

	float fGuildBouns = 100;

	CGuild*	lpGuild = getGuild();

	cFIELD*	lpfield = m_lpField;

	if(!lpfield)
		return 0;

	if(lpGuild && !lpfield->isVillage())
	{
		switch(lpGuild->m_wHallLevel)
		{
		case 1:		fGuildBouns = 80;	break;
		case 2:		fGuildBouns = 50;	break;
		case 3:		fGuildBouns = 40;	break;
		case 4:		fGuildBouns = 30;	break;
		case 5:		fGuildBouns = 20;	break;
		}
	}

	for (int iIndex=0;iIndex<20;iIndex++)
	{
		if (m_iLevel >= g_aPlayerDeathPeneltyInfo[iIndex].m_iBeginLevel &&
			m_iLevel <= g_aPlayerDeathPeneltyInfo[iIndex].m_iEndLevel)
			return	m_iLevel*m_iLevel*g_aPlayerDeathPeneltyInfo[iIndex].m_iRecoverPriceValue/10 * fGuildBouns/100;
	}

	return	0;
}

BOOL
cACTOR::isPureMonster()
{
	if	(!isMonster())
		return	FALSE;

	if	(isPet() || isSummonBeast() || m_wCharmedTime || m_wLordActor != 0xffff || m_wLordOperator != 0xffff)
		return	FALSE;

	return	TRUE;
}


BOOL
cACTOR::isDetector()
{
	return	FALSE;
}

DWORD
cACTOR::getCheckSumForClientData()
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
	
	for (i=0;i<(int)strlen(m_strId);i++)
		dwCheckSum+=m_strId[i];
	for (i=0;i<(int)strlen(m_strName);i++)
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
cACTOR::getDetectingLevel()
{
	return	getLevel()+m_detecting.getCorrectedValue();
}

int
cACTOR::getHidingLevel()
{
	return	getLevel()+m_wHidingLevel;
}

BOOL
cACTOR::isEventMob()
{
	if (m_bf1IsEvilSpritOfChristmas)
		return	TRUE;

	return	FALSE;
}

int
cACTOR::getRestraintTimeByBattle()
{
	return	m_wRestraintTimeByBattle;
}

void
cACTOR::setRestraintTimeByBattle()
{
	cACTOR	*lpTamer	=	getTamer();

	if	(lpTamer)
		lpTamer->m_wRestraintTimeByBattle	=	dBATTLE_STATUS_TIMER;

	m_wRestraintTimeByBattle	=	dBATTLE_STATUS_TIMER;
}

void
cACTOR::resetRestraintTimeByBattle()
{
	m_wRestraintTimeByBattle	=	0;
}

int
cACTOR::getCorrectSkillLevel(cAbility	*_lpAbility)
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

int
cACTOR::getInRangeEnemyCount(int _iRange)
{
	if	(!m_lpField)
		return	-1;

	return	m_lpField->getInRangeEnemyCount(this,_iRange);
}

BOOL
cACTOR::isSealdAction(int _iSkill)
{
	if	(m_wUnsealAction	==	_iSkill)
		return	FALSE;

	
	BOOL bIsNotAbleSkillWhenTransToWeapon = FALSE;

	if(m_wTransToWeaponTime)
	{
		bIsNotAbleSkillWhenTransToWeapon  = TRUE;
		
		cAbility ability;
		ability.m_wSkill = _iSkill;
		ability.m_wLevel = 1;
		
		CSkill* lpSkill = ability.getSkill();
		
		if(lpSkill && lpSkill->bf1IsAbleSkillWhenTransToWeapon)
			bIsNotAbleSkillWhenTransToWeapon  = FALSE;
	}
	if	(m_wSleepTime || m_wNightMareTime || m_wParalysisTime || bIsNotAbleSkillWhenTransToWeapon	||	m_wTransToFatGirlTime || m_wTransToFrogTime || m_wJumppingSkippingTime ||
		m_wFeignDeathTime || m_wRabbitTime || m_wHwabyungTime	||	m_wSealActionTime || m_wZombieBombPower)
		return TRUE;

	if	(m_wShakleUpkeepTime	&&	m_bf1IsFreezeShakle)
		return	TRUE;

	return	FALSE;
}
BOOL
cACTOR::isSealdMove()
{
	if	(m_iShakleHP)
	{
		CSkillExtraEffect	*lpSEE	=	g_aSkill[m_bf10ShakleSkill].getExtraEffectByEffect(eSKILL_EE_MAKE_SHAKLE);

		if	(!lpSEE)
			return	TRUE;
		
		if	(lpSEE->m_aiValue[7]	==	FALSE)
			return	TRUE;
	}

	if	(m_wSleepTime || m_wNightMareTime || m_wParalysisTime || m_wFreezeTime	||	m_moveSpeed.m_sCorrectValue	==	-100	||	m_wTransToWeaponTime ||
		m_wFeignDeathTime	||	m_wInMagicBoxTime)
		return TRUE;

	return	FALSE;
}

BOOL
cACTOR::isInArea(int _iArea)
{
	cAreaInfo	*lpArea	=	m_lpField->getArea(_iArea);

	if (!lpArea)
		return	FALSE;

	return	lpArea->isIn(m_pos.x,m_pos.y);
}

BOOL
cACTOR::isUsedSkill(int _iArea,int _iSkill)
{
	if (getLastUseSkill()	!=	_iSkill)
		return	FALSE;

	cAreaInfo	*lpArea	=	m_lpField->getArea(_iArea);

	if (!lpArea)
		return	FALSE;

	return	lpArea->isIn(m_wUseSkillPosX,m_wUseSkillPosY);
}

BOOL
cACTOR::isUsedItem(int _iItem)
{
	if	(_iItem	==	-1)
	{
		if	(getLastUseItem()	!=	-1)
			return	TRUE;

		return	FALSE;
	}

	if (getLastUseItem()	==	_iItem)
		return	TRUE;

	return	FALSE;
}

BOOL
cACTOR::isBattleWithTargetParty(cACTOR *_lpActor)
{
	if	(m_lpTarget	==	NULL)
		return	FALSE;

	return	m_lpTarget->isPartyMember(_lpActor);
}

BOOL
cACTOR::isOnSameField(cACTOR *_lpActor)
{
	if	(!_lpActor)
		return	FALSE;
	if	(m_wCurrentField	!=	_lpActor->m_wCurrentField)
		return	FALSE;

	if	(m_wCurrentField	<	dINSTANCE_FIELD_FLAG)
		return	TRUE;

	if (m_bf8InstanceField	!=	_lpActor->m_bf8InstanceField)
		return	FALSE;

	if (m_bf4InstanceFieldFloor	!=	_lpActor->m_bf4InstanceFieldFloor)
		return	FALSE;

	return	TRUE;
}

BOOL
cACTOR::isOnSameField(int _iFieldUniqueSerial,int _iGateField)
{
	if (m_wCurrentField <	dINSTANCE_FIELD_FLAG)
	{
		if (m_wCurrentField	!=	_iFieldUniqueSerial)
			return	FALSE;

		return	TRUE;
	}

	if (m_wCurrentField -	dINSTANCE_FIELD_FLAG	!=	_iGateField)
		return	FALSE;

	if (m_bf8InstanceField		!=	(DWORD)(_iFieldUniqueSerial/dINSTANCE_FIELD_FLAG))
		return	FALSE;

	if (m_bf4InstanceFieldFloor	!=	(DWORD)(_iFieldUniqueSerial%dINSTANCE_FIELD_FLAG))
		return	FALSE;

	return	TRUE;
}

int
cACTOR::getStrength(BOOL _bIsAskByReversal)
{
	if(m_wTransformationTime)
		return  m_wTransformationLevel;

	if	(m_wReversalPowerTime	&&	_bIsAskByReversal	==	FALSE)
		return	getIntelligence(TRUE);

	int	iValue;

	if	(m_wJob	==	dJOB_WEREWOLF)
	{
		iValue	=	m_intelligence.getCorrectedValue();

		if	(m_intelligence.isFixedValue())
			return	max(iValue,1);
	}
	else
	{
		iValue	=	m_strength.getCorrectedValue();

		if	(m_strength.isFixedValue())
			return	max(iValue,1);
	}

	iValue	=	iValue*m_wBoostPower/100;
	
	if	(m_wMarionetteTime)
		iValue	+=	m_wCorrectMarionetteStrength;

	int	iDeathPenelty	=	m_wCurrentLevelDeathPenelty;

	if	(m_wAnimatePartnerTime)
		iDeathPenelty	=	0;
	if	(m_wJob	==	dJOB_WEREWOLF)
		iValue		+=	m_bf4EatLightMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;
	else
		iValue		+=	m_bf4EatFireMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;


	return	max(iValue*(100-iDeathPenelty)/100,1);
}

int
cACTOR::getConstitution(BOOL _bIsAskByReversal)
{
	if(m_wTransformationTime)
		return  m_wTransformationLevel;

	int	iValue;

	if	(m_wJob	==	dJOB_WEREWOLF)
	{
		iValue	=	m_charisma.getCorrectedValue();

		if	(m_charisma.isFixedValue())
			return	max(iValue,1);
	}	
	else
	{
		iValue	=	m_constitution.getCorrectedValue();

		if	(m_constitution.isFixedValue())
			return	max(iValue,1);
	}
	
	iValue	=	iValue*m_wBoostPower/100;

	if	(m_wMarionetteTime)
		iValue	+=	m_wCorrectMarionetteConstitution;

	int	iDeathPenelty	=	m_wCurrentLevelDeathPenelty;

	if	(m_wAnimatePartnerTime)
		iDeathPenelty	=	0;

	if	(m_wJob	==	dJOB_WEREWOLF)
		iValue		+=	m_bf4EatDarkMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;
	else
		iValue		+=	m_bf4EatEarthMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;

	return	max(iValue*(100-iDeathPenelty)/100,1);
}


int
cACTOR::getIntelligence(BOOL _bIsAskByReversal)
{
	if(m_wTransformationTime)
		return  m_wTransformationLevel;

	if	(m_wReversalPowerTime	&&	_bIsAskByReversal	==	FALSE)
		return	getStrength(TRUE);

	int	iValue;

	if	(m_wJob	==	dJOB_WEREWOLF)
	{
		iValue	=	m_strength.getCorrectedValue();

		if	(m_strength.isFixedValue())
			return	max(iValue,1);
	}
	else
	{
		iValue	=	m_intelligence.getCorrectedValue();

		if	(m_intelligence.isFixedValue())
			return	max(iValue,1);
	}

	iValue	=	iValue*m_wBoostPower/100;

	if	(m_wMarionetteTime)
		iValue	+=	m_wCorrectMarionetteIntelligence;

	int	iDeathPenelty	=	m_wCurrentLevelDeathPenelty;

	if	(m_wAnimatePartnerTime)
		iDeathPenelty	=	0;

	if	(m_wJob	==	dJOB_WEREWOLF)
		iValue		+=	m_bf4EatFireMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;
	else
		iValue		+=	m_bf4EatLightMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;

	return	max(iValue*(100-iDeathPenelty)/100,1);
}


int
cACTOR::getCharisma(BOOL _bIsAskByReversal)
{
	if(m_wTransformationTime)
		return  m_wTransformationLevel;

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

	if	(m_wMarionetteTime)
		iValue	+=	m_wCorrectMarionetteCharisma;

	int	iDeathPenelty	=	m_wCurrentLevelDeathPenelty;

	if	(m_wAnimatePartnerTime)
		iDeathPenelty	=	0;

	if	(m_wJob	==	dJOB_WEREWOLF)
		iValue		+=	m_bf4EatEarthMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;
	else
		iValue		+=	m_bf4EatDarkMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;

	return	max(iValue*(100-iDeathPenelty)/100,1);
}


int
cACTOR::getWisdom(BOOL _bIsAskByReversal)
{
	if(m_wTransformationTime)
		return  m_wTransformationLevel;

	if	(m_wReversalPowerTime	&&	_bIsAskByReversal	==	FALSE)
		return	getAgility(TRUE);

	int	iValue;

	if	(m_wJob	==	dJOB_FALLEN_ANGEL)
	{
		iValue	=	m_wisdom.getCorrectedValue()+m_wisdom.getCorrectedValue()*3/100;

		if (m_wisdom.isFixedValue())
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

	if	(m_wMarionetteTime)
		iValue	+=	m_wCorrectMarionetteWisdom;

	int	iDeathPenelty	=	m_wCurrentLevelDeathPenelty;

	if	(m_wAnimatePartnerTime)
		iDeathPenelty	=	0;

	if	(m_wJob	==	dJOB_WEREWOLF)
		iValue		+=	m_bf4EatWindMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;
	else
		iValue		+=	m_bf4EatWaterMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;


	return	max(iValue*(100-iDeathPenelty)/100,1);
}

int
cACTOR::getAgility(BOOL _bIsAskByReversal)		
{
	if(m_wTransformationTime)
		return  m_wTransformationLevel;

	if	(m_wReversalPowerTime	&&	_bIsAskByReversal	==	FALSE)
		return	getWisdom(TRUE);

	int	iValue;

	if	(m_wJob	==	dJOB_WEREWOLF)
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

	if	(m_wMarionetteTime)
		iValue	+=	m_wCorrectMarionetteAgility;

	int	iDeathPenelty	=	m_wCurrentLevelDeathPenelty;

	if	(m_wAnimatePartnerTime)
		iDeathPenelty	=	0;

	if	(m_wJob	==	dJOB_WEREWOLF)
		iValue		+=	m_bf4EatWaterMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;
	else
		iValue		+=	m_bf4EatWindMiniPetCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET;

	return	max(iValue*(100-iDeathPenelty)/100,1);
}

int
cACTOR::getLuck(BOOL _bIsAskByReversal)			
{
	if(m_wTransformationTime)
		return  m_wTransformationLevel;

	int	iValue	=	m_luck.getCorrectedValue();

	if	(m_luck.isFixedValue())
		return	max(iValue,0);
	
	iValue	=	iValue*m_wBoostPower/100;

	if	(m_wMarionetteTime)
		iValue	+=	m_wCorrectMarionetteLuck;

	int	iDeathPenelty	=	m_wCurrentLevelDeathPenelty;

	if	(m_wAnimatePartnerTime)
		iDeathPenelty	=	0;

	return	max(iValue*(100-iDeathPenelty)/100,1);
}

int
cACTOR::getMoveSpeed()
{
	return	max(m_moveSpeed.getCorrectedValue()+m_wCorrectMoveSpeedByRidingDog,-100);
}//이동 속도 보정

void
cACTOR::getReleasePos(cPOINT *pos,int _iAnm,int _iDirect)
{
	if	(_iAnm	==	-1)
		_iAnm	=	m_iAnm;
	
	if	(_iDirect==	-1)
		_iDirect=	m_wDirect;

	if	(!getBody()->m_pANM[_iAnm].m_isRELEASE	||	getBody()->m_pANM[_iAnm].m_pReleasePos	==	NULL)
	{
		pos->x	=	m_pos.x;
		pos->y	=	m_pos.y;

		return;
	}
	
	pos->x	=	m_pos.x	+	getBody()->m_pANM[_iAnm].m_pReleasePos[_iDirect].x*m_wScaleX/100;
	pos->y	=	m_pos.y	+	getBody()->m_pANM[_iAnm].m_pReleasePos[_iDirect].y*m_wScaleY/100;
}

cActor_miniPet*
cACTOR::iMiniPet()
{
	return	(cActor_miniPet*)this;
}


int
cACTOR::getCorrectUnlockLevel()
{
	return	getCorrectUnlockLevelByTitle()+iMiniPet()->getCorrectUnlockLevel();
}

BOOL
cACTOR::isBossMonster()
{
	if	(isPureMonster()	==	FALSE)
		return	FALSE;

	if	(m_wIsBoss)
		return	TRUE;

	cJOB	*lpJob	=	getJob();

	if	(!lpJob)
		return	FALSE;

	if	(lpJob->m_wMonsterLevel	>=	cJOB::eML_BOSS1)
		return	TRUE;

	return	FALSE;
}

BOOL
cACTOR::isOverSemiBossMonster()
{
	if	(isPureMonster()	==	FALSE)
		return	FALSE;
	
	if	(m_wIsBoss)
		return	TRUE;
	
	cJOB	*lpJob	=	getJob();
	
	if	(!lpJob)
		return	FALSE;
	
	if	(lpJob->m_wMonsterLevel	>=	cJOB::eML_SEMI_BOSS1)
		return	TRUE;
	
	return	FALSE;
}


int
cACTOR::getExpBonusByA()
{
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
cACTOR::getLightResistanceBonusByA()
{
	if	(isPlayer()	==	FALSE)
		return	0;

	if	(g_iHeavenRedStoneCount	>	g_iHellRedStoneCount)
	{
		if	(m_sAllignment	>=	25)
		{
			return	m_sAllignment;
		}

		if	(m_sAllignment	<=	-50)
		{
			return	m_sAllignment*g_iRedDevilFactor/100/10;
		}
	}

	if	(g_iHellRedStoneCount	>	g_iHeavenRedStoneCount)
	{
		if	(m_sAllignment	<=	-75)
		{
			return	-m_sAllignment*g_iRedDevilFactor/100/4;
		}

		if	(m_sAllignment	>=	50)
		{
			return	-m_sAllignment*g_iRedDevilFactor/100/10;
		}
	}

	return	0;
}

int
cACTOR::getFireResistanceBonusByA()
{
	if	(isPlayer()	==	FALSE)
		return	0;

	if	(g_iHeavenRedStoneCount	>	g_iHellRedStoneCount)
	{
		if	(m_sAllignment	>=	50)
		{
			return	m_sAllignment/2;
		}
		if	(m_sAllignment	<=	-50)
		{
			return	m_sAllignment*g_iRedDevilFactor/100/10;
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
			return	-m_sAllignment*g_iRedDevilFactor/100/10;
		}
	}

	return	0;
}

int
cACTOR::getWaterResistanceBonusByA()
{
	if	(isPlayer()	==	FALSE)
		return	0;

	if	(g_iHeavenRedStoneCount	>	g_iHellRedStoneCount)
	{
		if	(m_sAllignment	>=	50)
		{
			return	m_sAllignment/2;
		}
		if	(m_sAllignment	<=	-50)
		{
			return	m_sAllignment*g_iRedDevilFactor/100/10;
		}
	}

	if	(g_iHellRedStoneCount	>	g_iHeavenRedStoneCount)
	{
		if	(m_sAllignment	<=	-75)
		{
			return	-m_sAllignment*g_iRedDevilFactor/100/4;
		}
		if	(m_sAllignment	>=	50)
		{
			return	-m_sAllignment*g_iRedDevilFactor/100/10;
		}
	}
	return	0;
}

int
cACTOR::getWindResistanceBonusByA()
{
	if	(isPlayer()	==	FALSE)
		return	0;

	if	(g_iHeavenRedStoneCount	>	g_iHellRedStoneCount)
	{
		if	(m_sAllignment	>=	75)
		{
			return	m_sAllignment*g_iRedDevilFactor/100/4;
		}
		if	(m_sAllignment	<=	-50)
		{
			return	m_sAllignment*g_iRedDevilFactor/100/10;
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
			return	-m_sAllignment*g_iRedDevilFactor/100/10;
		}
	}

	return	0;
}

int
cACTOR::getDarkResistanceBonusByA()
{
	if	(isPlayer()	==	FALSE)
		return	0;

	if	(g_iHeavenRedStoneCount	>	g_iHellRedStoneCount)
	{
		if	(m_sAllignment	>=	75)
		{
			return	m_sAllignment*g_iRedDevilFactor/100/4;
		}
		if	(m_sAllignment	<=	-50)
		{
			return	m_sAllignment*g_iRedDevilFactor/100/10;
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
			return	-m_sAllignment*g_iRedDevilFactor/100/10;
		}
	}

	return	0;
}

int
cACTOR::getEarthResistanceBonusByA()
{
	if	(isPlayer()	==	FALSE)
		return	0;

	if	(g_iHeavenRedStoneCount	>	g_iHellRedStoneCount)
	{
		if	(m_sAllignment	>=	75)
		{
			return	m_sAllignment*g_iRedDevilFactor/100/4;
		}
		if	(m_sAllignment	<=	-50)
		{
			return	m_sAllignment*g_iRedDevilFactor/100/10;
		}
	}

	if	(g_iHellRedStoneCount	>	g_iHeavenRedStoneCount)
	{
		if	(m_sAllignment	<=	-75)
		{
			return	-m_sAllignment*g_iRedDevilFactor/100/4;
		}
		if	(m_sAllignment	>=	50)
		{
			return	-m_sAllignment*g_iRedDevilFactor/100/10;
		}
	}

	return	0;
}

int
cACTOR::getTradeBonusByA()
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
cACTOR::getItemDropChanceBonusByA()
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
cACTOR::getItemDropLevelBonusByA()
{
	if	(g_iHellRedStoneCount	>	g_iHeavenRedStoneCount)
	{
		if	(m_sAllignment	<=	-50)
		{
			double	dblValue	=	-m_sAllignment;

			return	(int)sqrt(dblValue)*g_iRedDevilFactor/100;
		}
	}

	return	0;
}

