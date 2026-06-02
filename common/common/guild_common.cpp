#include "CGuildDefine.h"

int	g_iGuildBattleCountAtDay	=	dGUILD_BATTLE_COUNT_AT_DAY;
int	g_iGuildBattleCountAtWeek	=	dGUILD_BATTLE_COUNT_AT_WEEK;

int
CGuildBaseInfo::getFullSkillPoint()
{
	int	iBonusPoint	=	max(m_bf6BonusGuildPoint-m_bf6PeneltyBonusGuildPoint,0);

	return	m_wLevel-1+iBonusPoint;
}

void
CGuildBaseInfo::reset()
{
	memset(this,0,sizeof(CGuildBaseInfo));

	m_wSerial						=	0xffff;
	memset(m_strName,0,dGUILD_NAME_LENGTH);
	m_wGuildHallShape				=	0;
	m_wLevel						=	1;
	m_dwExp							=	0;
	m_llGuildGold					=	0;
	m_wHallLevel					=	0;
	m_wMaxMember					=	dGUILD_MIN_PLAYER;
	m_wMemberCount					=	0;
	m_wMaxSubMaster					=	dGUILD_MIN_SUBMASTER;
	m_wSubMasterCount				=	0;
	m_wMaxCongressMan				=	dGUILD_MIN_CONGRESSMAN;
	m_wCongressManCount				=	0;
	m_wIsGetGuildExp				=	0;
	m_wExpTaxRate					=	100;
	memset(m_awGuildVillage,0xff,sizeof(m_awGuildVillage));
	m_awGuildVillage[0]				=	0;
	m_bf3GuildGoodwillForRoen		=	0;

	strcpy(m_strNotice,"");
	m_wNoticeType					=	eGUILDNOTICETYPE_NORMAL;
	memset(m_strMasterName,0,dNAME_LENGTH);
	m_wElderCount					=	0;
	m_wGuildMasterLevel				=	0xffff;
	m_wTargetGuild					=	0xffff;
	m_wInvadeGuild					=	0xffff;
	m_timeLastConnectedMasterTime.m_dwValue =	0;
	m_timeLastConnectTime.m_dwValue	=	0;
	m_wGuildPoint					=	0;
	m_iVictoryPoint					=	dDEFAULT_GUILD_VICTORY_POINT;
	m_wVictoryContinue				=	0;
	m_wVictoryCount					=	0;
	m_wDefeatCount					=	0;
	m_wDrawCount					=	0;
	m_wCancelCount					=	0;
	m_wRelatedPlaceCount			=	c_iMinCountForRelatedPlaceOfGuiild;
	m_iHonorPoint					=	0;					//	뫄曼 포인트

	m_wSetGuildTaxExpSettingDay		=	0;
	m_wGuildInventorySize			=	dDEFAULT_GUILD_INVENTORY_SIZE;
	m_timeBeginGuildMaster.m_dwValue=	0;

	m_llGuildGold					=	0;					//	길�E소유 금액
	m_wBattle_VCC					=	0;
	m_wBattle_VC					=	0;
	m_wBattle_DC					=	0;
	m_wBattle_CC					=	0;	//	연승,승,패,캔슬

	memset(m_aiWithdrawGoldLimitPerDay,0,sizeof(m_aiWithdrawGoldLimitPerDay));			//	직위별 하루에 꺼낼 �E있는 골�E제한
	memset(m_aWithdrawItemCountPerDay,0,sizeof(m_aWithdrawItemCountPerDay));			//	직위별 하루에 꺼낼 �E있는 골�E제한
	memset(m_abInventorySize,0,sizeof(m_abInventorySize));			//	직위별 하루에 꺼낼 �E있는 골�E제한
	memset(m_awGuildInventoryManagerVillage,0xff,sizeof(m_awGuildInventoryManagerVillage));			//	직위별 하루에 꺼낼 �E있는 골�E제한

	m_guildMarkInfo.reset();

	memset(m_wFightGuild,0xff,sizeof(WORD)*dMAX_FIGHT_ATWEEK);
	memset(m_abGuildSkill,0,sizeof(m_abGuildSkill));
	memset(m_awGuildInventoryManagerVillage,0xff,sizeof(m_awGuildInventoryManagerVillage));
}

void
CGuildAdvanceInfo::reset()
{
	int	i;

	for (i=0;i<c_iGuildInventorySize;i++)
		m_aInventory[i].reset();
	for (i=0;i<dGUILD_PET_TYPE_COUNT;i++)
		m_aGuildPet[i].reset();
	for (i=0;i<dGUILD_GUARDIAN_TYPE_COUNT;i++)
		m_aGuildGuardian[i].reset();
	for (i=0;i<dMAX_GUILD_STRUCTURE_COUNT;i++)
		m_aGuildStructure[i].reset();

	CGuildBaseInfo::reset();

	memset(m_aAllienceGuild,0xff,sizeof(WORD)*dMAX_ALLIENCE_GUILD_COUNT);
	memset(m_aEnemyGuild,0xff,sizeof(WORD)*dMAX_ENEMY_GUILD_COUNT);
	memset(m_awGuildBank,0xff,sizeof(m_awGuildBank));
}

BOOL
CGuildAdvanceInfo::storeItem(int _iSlot,CItemDefine *_lpItem)	//	아이템 보�E
{
	if	(m_aInventory[_iSlot].m_wBaseItem	!=	0xffff)
		return	FALSE;

	memcpy(&m_aInventory[_iSlot],_lpItem,sizeof(CItemDefine));

	return	TRUE;
}

BOOL
CGuildAdvanceInfo::removeItem(int _iSlot,CItemDefine *_lpItem)	//	아이템 제거
{
	if	(m_aInventory[_iSlot].m_wBaseItem	!=	_lpItem->m_wBaseItem)
		return	FALSE;
	if	(m_aInventory[_iSlot].m_dwSerial	!=	_lpItem->m_dwSerial)
		return	FALSE;

	m_aInventory[_iSlot].reset();

	return	TRUE;
}

BOOL
CGuildAdvanceInfo::storeGold(int _iGold)	//	골�E보�E
{
	m_llGuildGold	+=	_iGold;

	return	TRUE;
}

BOOL
CGuildAdvanceInfo::removeGold(int _iGold)	//	골�E제거
{
	if	(m_llGuildGold	<	_iGold)
		return	FALSE;

	m_llGuildGold	-=	_iGold;

	return	TRUE;
}

//	연과�E추가
BOOL
CGuildAdvanceInfo::addRelatedPlace(int _iField,int _iPrice)
{
	int	iSlot	=	0xffff;

	for	(int i=0;i<dMAX_GUILD_VILLAGE_COUNT;i++)
	{
		if	(m_awGuildVillage[i]	==	0xffff)
		{
			iSlot	=	i;
			break;
		}
		if	(m_awGuildVillage[i]	==	_iField)
			return	FALSE;
	}

	if	(iSlot	==	0xffff)
		return	FALSE;

	m_awGuildVillage[iSlot]	=	_iField;

	return	TRUE;
}

BOOL
CGuildAdvanceInfo::isRelatedPlace(int _iField)
{
	for	(int i=0;i<dMAX_GUILD_VILLAGE_COUNT;i++)
	{
		if	(m_awGuildVillage[i]	==	_iField)
			return	TRUE;
	}

	return	FALSE;
}

int
CGuildAdvanceInfo::getRelatedPlaceCount()
{
	for	(int i=0;i<dMAX_GUILD_VILLAGE_COUNT;i++)
	{
		if	(m_awGuildVillage[i]	==	0xffff)
			return	i;
	}

	return	dMAX_GUILD_VILLAGE_COUNT;
}

//	가�E�E있는 연과�E개�E
int
CGuildAdvanceInfo::getMaxRelatedPlaceCount()
{
	return	m_wRelatedPlaceCount;
}


//	연과�E제거
BOOL
CGuildAdvanceInfo::removeRelatedPlace(int _iField)
{
	for	(int i=0;i<dMAX_GUILD_VILLAGE_COUNT;i++)
	{
		if	(m_awGuildVillage[i]	==	_iField)
		{
			if	(i	==	0)
				return	FALSE;

			for	(int j=i;j<dMAX_GUILD_VILLAGE_COUNT-1;j++)
				m_awGuildVillage[j]	=	m_awGuildVillage[j+1];

			m_awGuildVillage[dMAX_GUILD_VILLAGE_COUNT-1]	=	0xffff;

			return	TRUE;
		}
	}

	return	FALSE;
}

//JBC	웝滂자 커멘�E추가 �E� 08-09-25
int
CGuildAdvanceInfo::GetItemPlace(WORD _wBaseItem,DWORD _dwSerial)	//	아이템 위치 조�E
{
	for (int i =0  ; i < c_iGuildInventorySize ; i++)
	{
		if(m_aInventory[i].m_wBaseItem	==	0xffff)
			continue;

		if(m_aInventory[i].m_wBaseItem == _wBaseItem && m_aInventory[i].m_dwSerial == _dwSerial)
			return	i;
	}

	return 0xffffffff;
}

BOOL
CGuildAdvanceInfo::changeItemPlace(int _iSlot1,int _iSlot2)	//	아이템 위치 변�E
{
	CItemDefine	item;

	memcpy(&item,&m_aInventory[_iSlot1],sizeof(CItemDefine));
	memcpy(&m_aInventory[_iSlot1],&m_aInventory[_iSlot2],sizeof(CItemDefine));
	memcpy(&m_aInventory[_iSlot2],&item,sizeof(CItemDefine));

	return	TRUE;	
}

int
CGuildAdvanceInfo::getInventoryItemCount()
{
	int	iCount	=	0;

	for (int i=c_iGuildInventorySize;i;)
	{
		--i;

		if	(m_aInventory[i].m_wBaseItem	!=	0xffff)
			iCount++;
	}

	return	iCount;
}

void
CGuildAdvanceInfo::resetInventory()
{
	for (int i=c_iGuildInventorySize;i;)
	{
		--i;

		m_aInventory[i].reset();
	}
}

int
CGuildAdvanceInfo::getCheckSum()
{
	int	iCheckSum	=	0,i;

	iCheckSum	+=	m_wSetGuildTaxExpSettingDay;								//	마지막으로 길�E경험치에 �E� 세팅을 바꾼날(동일한 �E날로 설정되�E있다�E변�E불가능.)
//	iCheckSum	+=	m_wGuildInventorySize;										//	길�E인벤토리 사이�E	
//	iCheckSum	+=	m_wLevel;								//	길�E레벨
//	iCheckSum	+=	m_dwExp;								//	길�E경험치
	iCheckSum	+=	m_iVictoryPoint;						//	길�E승점
	iCheckSum	+=	m_wVictoryContinue;
	iCheckSum	+=	m_wVictoryCount;
	iCheckSum	+=	m_wDefeatCount;
	iCheckSum	+=	m_wDrawCount;
	iCheckSum	+=	m_wCancelCount;

	iCheckSum	+=	(int)m_llGuildGold;								//	길�E소유 금액
	iCheckSum	+=	m_wHallLevel;							//	길드홀 레벨
//	iCheckSum	+=	m_wMaxMember;
	iCheckSum	+=	m_wMemberCount;			//	최�E길드�E�E현�E길드�E�E//	iCheckSum	+=	m_wMaxSubMaster;
	iCheckSum	+=	m_wSubMasterCount;		//	최�E서�E길마 �E현�E서�E길마 �E//	iCheckSum	+=	m_wMaxCongressMan;
	iCheckSum	+=	m_wCongressManCount;	//	최�E길�E의�E�E현�E서길�E의�E�E	iCheckSum	+=	m_wIsGetGuildExp;
	iCheckSum	+=	m_wExpTaxRate;			//	길�E경험치를 걷을것인가? 길�E경험치 세율-_-
	iCheckSum	+=	m_wGuildPoint;
	iCheckSum	+=	m_bf1SubMasterEnableBanish;
	iCheckSum	+=	m_bf1IsPermitVisitor;
	iCheckSum	+=	m_iHonorPoint;
	iCheckSum	+=	m_bf2TryGuildPointBattleCountPerWeek;
	iCheckSum	+=	m_bf5ThisWeekGuildBattleCount;	//	주�E길드�E횟�E제한
	iCheckSum	+=	m_bf3ThisDayGuildBattleCount;	//	일�E길드�E횟�E제한
	iCheckSum	+=	m_bf1IsUseGuildMasterGrace;

	for (i=0;i<dMAX_GUILD_SKILL_COUNT;i++)
		iCheckSum	+=	m_abGuildSkill[i];

	iCheckSum	+=	GetHashCode(m_strName);			//	길�E이름

	return	iCheckSum;
}

void
CGuildAdvanceInfo::getSaveData_Column(CGuildSaveInfoForColumn * _lpData)
{
	_lpData->m_wSerial	= m_wSerial;
	strcpy(_lpData->m_strName,m_strName);
	_lpData->m_wLevel	= m_wLevel;
	_lpData->m_iVictoryPoint = m_iVictoryPoint;
	_lpData->m_iHonorPoint = m_iHonorPoint;
	_lpData->m_wGuildPoint = m_wGuildPoint;
	_lpData->m_wHallLevel = m_wHallLevel;
//	_lpData->m_timeOrganizeDate.m_dwValue = m_timeOrganizeDate.m_dwValue;
	strcpy(_lpData->m_strNotice , m_strNotice);
	_lpData->m_wNoticeType = m_wNoticeType;
	strcpy(_lpData->m_strMasterName , m_strMasterName);
	_lpData->m_timeLastConnectTime.m_dwValue = m_timeLastConnectTime.m_dwValue;
	_lpData->m_wGuildHallShape = m_wGuildHallShape;
	_lpData->m_llGuildGold = m_llGuildGold;
}

void
CGuildAdvanceInfo::getSaveData_Binary(CGuildSaveInfoForBinary * _lpData)
{
	_lpData->m_dwExp = m_dwExp;
	_lpData->m_wVictoryContinue = m_wVictoryContinue;
	_lpData->m_wVictoryCount = m_wVictoryCount;
	_lpData->m_wDefeatCount = m_wDefeatCount;
	_lpData->m_wDrawCount = m_wDrawCount;
	_lpData->m_wCancelCount = m_wCancelCount;
	_lpData->m_wBattle_VCC = m_wBattle_VCC;
	_lpData->m_wBattle_VC = m_wBattle_VC;
	_lpData->m_wBattle_DC = m_wBattle_DC;
	_lpData->m_wBattle_CC = m_wBattle_CC;
	_lpData->m_wMaxMember = m_wMaxMember;
	_lpData->m_wMaxSubMaster = m_wMaxSubMaster;
	_lpData->m_wMaxCongressMan = m_wMaxCongressMan;
	_lpData->m_wIsGetGuildExp = m_wIsGetGuildExp;
	_lpData->m_wExpTaxRate = m_wExpTaxRate;
	memcpy(_lpData->m_awGuildVillage,m_awGuildVillage,sizeof(WORD)*dMAX_GUILD_VILLAGE_COUNT);
	_lpData->m_bf1SubMasterEnableBanish = m_bf1SubMasterEnableBanish;
	_lpData->m_bf1IsPermitVisitor		=	m_bf1IsPermitVisitor;	//	길드홀에 들엉邦는걸 허�E�.
	_lpData->m_bf2TryGuildPointBattleCountPerWeek	=	m_bf2TryGuildPointBattleCountPerWeek;	//	이�E주에 길�E포인트�E� 도�E� 횟�E	_lpData->m_bf1IsTestSiegeWarfareGuild=	m_bf1IsTestSiegeWarfareGuild;	//	이�E주에 길�E포인트�E� 도�E� 횟�E	_lpData->m_bf1IsTwinkle				=	m_bf1IsTwinkle;	//	이�E주에 길�E포인트�E� 도�E� 횟�E
	_lpData->m_bf3GuildGoodwillForRoen	=	m_bf3GuildGoodwillForRoen;
	//_lpData->m_bf3GuildDungeonPlayTime=	m_bf3GuildDungeonPlayTime;
	_lpData->m_bf1IsUseGuildMasterGrace	=	m_bf1IsUseGuildMasterGrace;

	_lpData->m_bf6LastGVGBattleYear		=	m_bf6LastGVGBattleYear;
	_lpData->m_bf4LastGVGBattleMonth	=	m_bf4LastGVGBattleMonth;
	_lpData->m_bf5LastGVGBattleDay		=	m_bf5LastGVGBattleDay;

	_lpData->m_bf5ThisWeekGuildBattleCount	=	m_bf5ThisWeekGuildBattleCount;	//	주�E길드�E횟�E제한
	_lpData->m_bf3ThisDayGuildBattleCount	=	m_bf3ThisDayGuildBattleCount;	//	일�E길드�E횟�E제한

	_lpData->m_bf6BonusGuildPoint			=	m_bf6BonusGuildPoint;
	_lpData->m_bf6PeneltyBonusGuildPoint	=	m_bf6PeneltyBonusGuildPoint;
	
	_lpData->m_wRelatedPlaceCount= m_wRelatedPlaceCount;

	memcpy(_lpData->m_wFightGuild,m_wFightGuild,sizeof(WORD)*dMAX_FIGHT_ATWEEK);
	_lpData->m_wElderCount = m_wElderCount;
	_lpData->m_wTargetGuild = m_wTargetGuild;
	_lpData->m_wInvadeGuild = m_wInvadeGuild;
	_lpData->m_wSetGuildTaxExpSettingDay = m_wSetGuildTaxExpSettingDay;
	_lpData->m_wGuildInventorySize = m_wGuildInventorySize;
	_lpData->m_timeBeginGuildMaster.m_dwValue = m_timeBeginGuildMaster.m_dwValue;
	_lpData->m_timeLastConnectedMasterTime.m_dwValue = m_timeLastConnectedMasterTime.m_dwValue;

	memcpy(_lpData->m_aiWithdrawGoldLimitPerDay,m_aiWithdrawGoldLimitPerDay,sizeof(m_aiWithdrawGoldLimitPerDay));
	memcpy(_lpData->m_aWithdrawItemCountPerDay,m_aWithdrawItemCountPerDay,sizeof(m_aWithdrawItemCountPerDay));
	_lpData->m_bChangeInventorySettingAdmin	=	m_bChangeInventorySettingAdmin;
	memcpy(_lpData->m_abInventorySize,m_abInventorySize,sizeof(m_abInventorySize));
	memcpy(_lpData->m_awGuildInventoryManagerVillage,m_awGuildInventoryManagerVillage,sizeof(m_awGuildInventoryManagerVillage));
}

void
CGuildAdvanceInfo::getSaveData_Bank(CGuildSaveInfoBank * _lpData)
{
	memcpy(_lpData->m_awGuildBank,m_awGuildBank,sizeof(WORD)*c_iGuildBankCount);
}

void
CGuildAdvanceInfo::getSaveData_Mark(CGuildSaveInfoMark * _lpData)
{
	memcpy(&_lpData->m_guildMarkInfo,&m_guildMarkInfo,sizeof(CGuildMarkInfo));
}

void
CGuildAdvanceInfo::getSaveData_RelShip(CGuildSaveInfoRelShip * _lpData)
{
	memcpy(_lpData->m_aAllienceGuild,m_aAllienceGuild,sizeof(WORD)*dMAX_ALLIENCE_GUILD_COUNT);
	memcpy(_lpData->m_aEnemyGuild,m_aEnemyGuild,sizeof(WORD)*dMAX_ENEMY_GUILD_COUNT);
}

void
CGuildAdvanceInfo::getSaveData_Skill(CGuildSaveInfoSkill * _lpData)
{
	for	(int i=dMAX_GUILD_SKILL_COUNT;i;)
	{
		--i;

		_lpData->m_aGuildSkill[i]	=	m_abGuildSkill[i];
	}
}

void
CGuildAdvanceInfo::getSaveData_Pet(CGuildSaveInfoPet * _lpData)
{
	memcpy(_lpData->m_aGuildPet,m_aGuildPet,sizeof(m_aGuildPet));
}

void
CGuildAdvanceInfo::getSaveData_Guardian(CGuildSaveInfoGuardian * _lpData)
{
	memcpy(_lpData->m_aGuildGuardian,m_aGuildGuardian,sizeof(m_aGuildGuardian));
}

void
CGuildAdvanceInfo::getSaveData_Inventory(CGuildSaveInfoInventory * _lpData)
{
	memcpy(_lpData->m_aInventory,m_aInventory,sizeof(CItemDefine)*c_iGuildInventorySize);
}

void
CGuildAdvanceInfo::getSaveData_Structure(CGuildSaveInfoStructure * _lpData)
{
	memcpy(_lpData->m_aGuildStructure,m_aGuildStructure,sizeof(cGuildStructureInfo)*dMAX_GUILD_STRUCTURE_COUNT);
}


void
CGuildAdvanceInfo::setSaveData_Column(CGuildSaveInfoForColumn * _lpData)
{
	m_wSerial = _lpData->m_wSerial;
	strcpy(m_strName,_lpData->m_strName);
	m_wLevel = _lpData->m_wLevel;
	m_iVictoryPoint = _lpData->m_iVictoryPoint;
	m_iHonorPoint = _lpData->m_iHonorPoint;
	m_wGuildPoint = _lpData->m_wGuildPoint;
	m_wHallLevel = _lpData->m_wHallLevel;
	m_timeOrganizeDate.m_dwValue = _lpData->m_timeOrganizeDate.m_dwValue;
	strcpy(m_strNotice , _lpData->m_strNotice);
	m_wNoticeType = _lpData->m_wNoticeType;
	strcpy(m_strMasterName , _lpData->m_strMasterName);
	m_timeLastConnectTime.m_dwValue = _lpData->m_timeLastConnectTime.m_dwValue;
	m_wGuildHallShape = _lpData->m_wGuildHallShape;
	m_llGuildGold = _lpData->m_llGuildGold;
}

void
CGuildAdvanceInfo::setSaveData_Binary(CGuildSaveInfoForBinary * _lpData)
{
	SYSTEMTIME	TimeInfo;
	GetLocalTime(&TimeInfo);


	m_dwExp = _lpData->m_dwExp;
	m_wVictoryContinue = _lpData->m_wVictoryContinue;
	m_wVictoryCount = _lpData->m_wVictoryCount;
	m_wDefeatCount = _lpData->m_wDefeatCount;
	m_wDrawCount = _lpData->m_wDrawCount;
	m_wCancelCount = _lpData->m_wCancelCount;
	m_wBattle_VCC = _lpData->m_wBattle_VCC;
	m_wBattle_VC = _lpData->m_wBattle_VC;
	m_wBattle_DC = _lpData->m_wBattle_DC;
	m_wBattle_CC = _lpData->m_wBattle_CC;
	m_wMaxMember = _lpData->m_wMaxMember;
	m_wMaxSubMaster = _lpData->m_wMaxSubMaster;
	m_wMaxCongressMan = _lpData->m_wMaxCongressMan;
	m_wIsGetGuildExp = _lpData->m_wIsGetGuildExp;
	m_wExpTaxRate = _lpData->m_wExpTaxRate;
	m_bf6BonusGuildPoint		=	_lpData->m_bf6BonusGuildPoint;
	m_bf6PeneltyBonusGuildPoint	=	_lpData->m_bf6PeneltyBonusGuildPoint;
	m_bf3GuildGoodwillForRoen	= _lpData->m_bf3GuildGoodwillForRoen;	
	//m_bf3GuildDungeonPlayTime	= _lpData->m_bf3GuildDungeonPlayTime;
	m_bf1IsUseGuildMasterGrace	= _lpData->m_bf1IsUseGuildMasterGrace;

	m_bf6LastGVGBattleYear		= _lpData->m_bf6LastGVGBattleYear;
	m_bf4LastGVGBattleMonth		= _lpData->m_bf4LastGVGBattleMonth;
	m_bf5LastGVGBattleDay		= _lpData->m_bf5LastGVGBattleDay;
	m_bf5ThisWeekGuildBattleCount=	_lpData->m_bf5ThisWeekGuildBattleCount;	//	주�E길드�E횟�E제한
	m_bf3ThisDayGuildBattleCount=	_lpData->m_bf3ThisDayGuildBattleCount;	//	일�E길드�E횟�E제한

	if(TimeInfo.wYear == 2009 && TimeInfo.wMonth == 4 && TimeInfo.wDay == 29 && TimeInfo.wHour <= 19)
	{
 		m_bf1IsUseGuildMasterGrace	= 0;
		m_bf3GuildGoodwillForRoen	= 0;
 	}

	memcpy(m_awGuildVillage,_lpData->m_awGuildVillage,sizeof(WORD)*dMAX_GUILD_VILLAGE_COUNT);

	m_bf1SubMasterEnableBanish = _lpData->m_bf1SubMasterEnableBanish;
	m_bf1IsPermitVisitor					= _lpData->m_bf1IsPermitVisitor;	//	길드홀에 들엉邦는걸 허�E�.
	m_bf2TryGuildPointBattleCountPerWeek	= _lpData->m_bf2TryGuildPointBattleCountPerWeek;	//	이�E주에 길�E포인트�E� 도�E� 횟�E	m_bf1IsTestSiegeWarfareGuild			= _lpData->m_bf1IsTestSiegeWarfareGuild;	//	이�E주에 길�E포인트�E� 도�E� 횟�E	m_bf1IsTwinkle							= _lpData->m_bf1IsTwinkle;	//	이�E주에 길�E포인트�E� 도�E� 횟�E

	m_wRelatedPlaceCount = _lpData->m_wRelatedPlaceCount;
	memcpy(m_wFightGuild,_lpData->m_wFightGuild,sizeof(WORD)*dMAX_FIGHT_ATWEEK);
	m_wElderCount = _lpData->m_wElderCount;
	m_wTargetGuild = _lpData->m_wTargetGuild;
	m_wInvadeGuild = _lpData->m_wInvadeGuild;
	m_wSetGuildTaxExpSettingDay = _lpData->m_wSetGuildTaxExpSettingDay;
	m_wGuildInventorySize = _lpData->m_wGuildInventorySize;
	m_timeBeginGuildMaster.m_dwValue = _lpData->m_timeBeginGuildMaster.m_dwValue;
	m_timeLastConnectedMasterTime.m_dwValue = _lpData->m_timeLastConnectedMasterTime.m_dwValue;
	//	초기값..
	m_wMemberCount		= 0;
	m_wSubMasterCount	= 0;
	m_wCongressManCount = 0;

	memcpy(m_aiWithdrawGoldLimitPerDay,_lpData->m_aiWithdrawGoldLimitPerDay,sizeof(m_aiWithdrawGoldLimitPerDay));
	memcpy(m_aWithdrawItemCountPerDay,_lpData->m_aWithdrawItemCountPerDay,sizeof(m_aWithdrawItemCountPerDay));
	m_bChangeInventorySettingAdmin	=	_lpData->m_bChangeInventorySettingAdmin;
	memcpy(m_abInventorySize,_lpData->m_abInventorySize,sizeof(m_abInventorySize));
	memcpy(m_awGuildInventoryManagerVillage,_lpData->m_awGuildInventoryManagerVillage,sizeof(m_awGuildInventoryManagerVillage));

}

void
CGuildAdvanceInfo::setSaveData_Bank(CGuildSaveInfoBank * _lpData)
{
	memcpy(m_awGuildBank,_lpData->m_awGuildBank,sizeof(WORD)*c_iGuildBankCount);
}

void
CGuildAdvanceInfo::setSaveData_Mark(CGuildSaveInfoMark * _lpData)
{
	memcpy(&m_guildMarkInfo,&_lpData->m_guildMarkInfo,sizeof(CGuildMarkInfo));
}

void
CGuildAdvanceInfo::setSaveData_RelShip(CGuildSaveInfoRelShip * _lpData)
{
	memcpy(m_aAllienceGuild,_lpData->m_aAllienceGuild,sizeof(WORD)*dMAX_ALLIENCE_GUILD_COUNT);
	memcpy(m_aEnemyGuild,_lpData->m_aEnemyGuild,sizeof(WORD)*dMAX_ENEMY_GUILD_COUNT);
}

void
CGuildAdvanceInfo::setSaveData_Skill(CGuildSaveInfoSkill * _lpData)
{
	for	(int i=dMAX_GUILD_SKILL_COUNT;i;)
	{
		--i;

		m_abGuildSkill[i]	=	(BYTE)_lpData->m_aGuildSkill[i];
	}
}

void
CGuildAdvanceInfo::setSaveData_Pet(CGuildSaveInfoPet * _lpData)
{
	memcpy(m_aGuildPet,_lpData->m_aGuildPet,sizeof(m_aGuildPet));
}

void
CGuildAdvanceInfo::setSaveData_Guardian(CGuildSaveInfoGuardian * _lpData)
{
	memcpy(m_aGuildGuardian,_lpData->m_aGuildGuardian,sizeof(m_aGuildGuardian));
}

void
CGuildAdvanceInfo::setSaveData_Inventory(CGuildSaveInfoInventory * _lpData)
{
	memcpy(m_aInventory,_lpData->m_aInventory,sizeof(CItemDefine)*c_iGuildInventorySize);
}

void
CGuildAdvanceInfo::setSaveData_Structure(CGuildSaveInfoStructure * _lpData)
{
	memcpy(m_aGuildStructure,_lpData->m_aGuildStructure,sizeof(cGuildStructureInfo)*dMAX_GUILD_STRUCTURE_COUNT);
}
// 
// void
// CGuildAdvanceInfo::copyData(CGuildAdvanceInfoOld *_lpOldData)
// {
// 	reset();
// 	
// 	memcpy(m_strName,_lpOldData->m_strName,sizeof(m_strName));	//	길�E이름
// 	
// 	m_wSerial			=	_lpOldData->m_wSerial;					//	길�E과鼈 시리�E// 	m_wLevel			=	_lpOldData->m_wLevel;				//	길�E레벨
// 	m_dwExp				=	_lpOldData->m_dwExp;				//	길�E경험치
// 	m_iVictoryPoint		=	_lpOldData->m_iVictoryPoint;		//	길�E승점
// 	m_wVictoryContinue	=	_lpOldData->m_wVictoryContinue;
// 	m_wVictoryCount		=	_lpOldData->m_wVictoryCount;
// 	m_wDefeatCount		=	_lpOldData->m_wDefeatCount;
// 	m_wDrawCount		=	_lpOldData->m_wDrawCount;
// 	m_wCancelCount		=	_lpOldData->m_wCancelCount;
// 
// 	//	길드홀레벨을 정하는데 �E�. 15일에 한번씩 정리된다.
// 	m_wHallLevel		=	_lpOldData->m_wHallLevel;							//	길드홀 레벨
// 	m_wMaxMember		=	_lpOldData->m_wMaxMember;
// 	m_wMemberCount		=	_lpOldData->m_wMemberCount;			//	최�E길드�E�E현�E길드�E�E// 	m_wMaxSubMaster		=	_lpOldData->m_wMaxSubMaster;
// 	m_wSubMasterCount	=	_lpOldData->m_wSubMasterCount;		//	최�E서�E길마 �E현�E서�E길마 �E// 	m_wMaxCongressMan	=	_lpOldData->m_wMaxCongressMan;
// 	m_wCongressManCount	=	_lpOldData->m_wCongressManCount;	//	최�E길�E의�E�E현�E서길�E의�E�E// 	m_wIsGetGuildExp	=	_lpOldData->m_wIsGetGuildExp;
// 	m_wExpTaxRate		=	_lpOldData->m_wExpTaxRate;			//	길�E경험치를 걷을것인가? 길�E경험치 세율-_-
// 	m_wGuildPoint		=	_lpOldData->m_wGuildPoint;
// 	m_bf1IsTestSiegeWarfareGuild	=	FALSE;
// 	m_bf1IsTwinkle		=	FALSE;	//	이�E주에 길�E포인트�E� 도�E� 횟�E
// 
// 	memset(m_aiWithdrawGoldLimitPerDay,0,sizeof(m_aiWithdrawGoldLimitPerDay));			//	직위별 하루에 꺼낼 �E있는 골�E제한
// 	memset(m_aWithdrawItemCountPerDay,0,sizeof(m_aWithdrawItemCountPerDay));			//	직위별 하루에 꺼낼 �E있는 골�E제한
// 	m_bChangeInventorySettingAdmin;			//	인벤토리 세팅 변�E가능한 따坤
// 	memset(m_abInventorySize,0,sizeof(m_abInventorySize));	//	길�E창�E분점이 있는 마�E// 	memset(m_awGuildInventoryManagerVillage,0xff,sizeof(m_awGuildInventoryManagerVillage));	//	길�E창�E분점이 있는 마�E// 	
// 	memcpy(&m_guildMarkInfo,&_lpOldData->m_guildMarkInfo,sizeof(m_guildMarkInfo));
// 	memcpy(m_strNotice,_lpOldData->m_strNotice,sizeof(m_strNotice));
// 	memcpy(m_strMasterName,_lpOldData->m_strMasterName,sizeof(m_strMasterName));
// 
// 	memset(m_awGuildVillage,0xff,sizeof(m_awGuildVillage));
// 	m_awGuildVillage[0]	=	0;
// 	m_wRelatedPlaceCount=	c_iMinCountForRelatedPlaceOfGuiild;
// 	memcpy(m_wFightGuild,_lpOldData->m_wFightGuild,sizeof(m_wFightGuild));
// 
// 	m_wNoticeType	=	_lpOldData->m_wNoticeType;
// 	m_wElderCount	=	_lpOldData->m_wElderCount;
// 	m_wTargetGuild	=	_lpOldData->m_wTargetGuild;
// 	m_wInvadeGuild	=	_lpOldData->m_wInvadeGuild;	
// 
// 	memset(m_aGuildPet,0,sizeof(m_aGuildPet));
// 	memset(m_aGuildGuardian,0,sizeof(m_aGuildGuardian));
// 
// 	memcpy(&m_timeOrganizeDate,&_lpOldData->m_timeOrganizeDate,sizeof(m_timeOrganizeDate));
// 	memcpy(&m_timeLastConnectTime,&_lpOldData->m_timeLastConnectTime,sizeof(m_timeLastConnectTime));
// 	memcpy(&m_timeLastConnectedMasterTime,&_lpOldData->m_timeLastConnectedMasterTime,sizeof(m_timeLastConnectedMasterTime));
// 	memcpy(&m_timeBeginGuildMaster,&_lpOldData->m_timeBeginGuildMaster,sizeof(m_timeBeginGuildMaster));
// 
// 	for	(int i=dMAX_GUILD_SKILL_COUNT;i;)
// 	{
// 		--i;
// 
// 		m_abGuildSkill[i]		=	(BYTE)_lpOldData->m_aGuildSkill[i];
// 	}
// 
// 	m_wSetGuildTaxExpSettingDay	=	_lpOldData->m_wSetGuildTaxExpSettingDay;
// 	m_wGuildInventorySize		=	_lpOldData->m_wGuildInventorySize;
// 
// 	m_wGuildHallShape			=	0;				//	길드홀 모양[추가됨]
// 	m_wGuildInventorySize		=	0;			//	길�E인벤토리 사이짊w이�EE같음]
// 	m_llGuildGold				=	0;					//	길�E소유 금액[추가됨]
// 	m_iHonorPoint				=	0;
// 
// 	m_wBattle_VCC				=	0;
// 	m_wBattle_VC				=	0;
// 	m_wBattle_DC				=	0;
// 	m_wBattle_CC				=	0;	//	연승,승,패,캔슬[추가됨]
// 	m_wGuildHallShape			=	0xffff;
// 	m_bf2TryGuildPointBattleCountPerWeek	=	0;
// 	m_bf1IsPermitVisitor					=	TRUE;
// 	m_bf1SubMasterEnableBanish				=	TRUE;
// 
// 	for (i=0;i<c_iGuildInventorySize;i++)
// 		m_aInventory[i].reset();
// 
// 	for (i=0;i<dMAX_GUILD_STRUCTURE_COUNT;i++)
// 		m_aGuildStructure[i].reset();
// 
// 	memset(m_aAllienceGuild,0xff,sizeof(WORD)*dMAX_ALLIENCE_GUILD_COUNT);
// 	memset(m_aEnemyGuild,0xff,sizeof(WORD)*dMAX_ENEMY_GUILD_COUNT);
// 
// 	memset(m_awGuildBank,0xff,sizeof(m_awGuildBank));	//	길�E은행[추가됨]
// 	memcpy(&m_timeLastConnectedMasterTime,&_lpOldData->m_timeLastConnectedMasterTime,sizeof(m_timeLastConnectedMasterTime));			//	길�E마스터 �E� 시간
// 	memcpy(&m_timeBeginGuildMaster,&_lpOldData->m_timeBeginGuildMaster,sizeof(m_timeBeginGuildMaster));			//	길�E마스터 �E� 시간
// }

CGuildBattleInfoForMember::CGuildBattleInfoForMember()
{
	reset();
}

void
CGuildBattleInfoForMember::reset()
{
	memset(this,0,sizeof(CGuildBattleInfoForMember));
	m_wIGB_FieldSerial		=	0xffff;
	m_bBSW_Hour				=	0xff;
	m_bIGB_Hour				=	0xff;
}
