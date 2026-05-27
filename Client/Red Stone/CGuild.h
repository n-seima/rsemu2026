#ifndef _classGUILD_H
#define _classGUILD_H

#include "CNemo.h"
#include "CGuildDefine.h"
#include "guild_skill_define.h"
#include "cPACKET_GAMESERVER.h"
#include "cITEM.h"

enum
{
	eGMLSM_BY_RANK,
	eGMLSM_BY_NAME,
	eGMLSM_BY_LEVEL,
	eGMLSM_BY_JOB,
};

class	cGuildBattleInfo
{
public:
	BOOL		m_bIsBeginBattle;
	int			m_iTeam1KillPoint,m_iTeam2KillPoint;
	int			m_iOwnTeam;
	char		m_strTeamName[2][dGUILD_NAME_LENGTH];

	void		reset()
	{
		m_bIsBeginBattle	=	FALSE;
		strcpy(m_strTeamName[0],"x");
		strcpy(m_strTeamName[1],"x");
	}
};

class	CGuild : public CGuildAdvanceInfo
{
public:
	WORD						m_wGuildInventorySize,m_wIsNotReceivedMemberList;
	WORD						m_wMemberSortMethod;
	CSimpleGuildMemberList		m_memberList;
	CSimpleGuildMemberList		m_OriginmemberList;						//ｱ豬・ﾁ､ｺｸﾀﾇ ｼﾒﾆｮ ﾀﾌﾀ・ﾇ ｸｮｽｺﾆｮ 
	CGuildBattleInfoForMember	m_battleInfo;
	char						m_strBookedGuildNotice[512];

	WORD		m_wMaxGuardianCount;
	WORD		m_wIncreaseGuardianSight;

	WORD		m_wIncreaseDamageUnderSiegeWarfare;
	WORD		m_wReduceDamageUnderSiegeWarfare;
	WORD		m_wIncreaeGuildPetSight;
	WORD		m_wIncreaseGuildCrestEfficient;
	WORD		m_wIncreaseGuildDoorEfficient;
	WORD		m_wIncreaseGuildPetAttackSpeed;
	WORD		m_wDecreaseGuildShopItemPrice;
	WORD		m_wIncreaseDashBladeDamage;
	WORD		m_wIncreaseGuildFlagEfficient;
	WORD		m_wIncreaseAstroBowDamage;
	
	WORD		m_wDecreasePeneltyOfFireResistance;
	WORD		m_wDecreasePeneltyOfWaterResistance;
	WORD		m_wDecreasePeneltyOfWindResistance;
	WORD		m_wDecreasePeneltyOfEarthResistance;
	WORD		m_wDecreasePeneltyOfLightResistance;
	WORD		m_wDecreasePeneltyOfDarkResistance;
	WORD		m_wObitianCount;
	WORD		m_wCristalWaterCount;
	WORD		m_wAstralBowCount;
	WORD		m_wDashBladeCount;
	WORD		m_wSellEnchantItemCount;
	WORD		m_awSellEnchantItem[c_iGuildHallSellEnchantItemCount];
	WORD		m_wDecreaseStructurePrice;
	WORD		m_wDecreaseDungeonMagicPenelty;
	WORD		m_wDecreaseDeathPenelty;

	int			m_iGuildHonorPoint;

	WORD		m_wRelatedPlaceOfGuildForNewPlace;

				CGuild();

	void		reset();
	void		resetGBS();

	char*		getSkillComment(int _iSkill);

	BOOL		isBookedBattle();	//	ｳｻﾀﾏ ｱ豬蠡・ﾌ ｿｹｾ犒ﾇｾ・ﾀﾖｴﾙ.
	BOOL		isEngagedBattle();	//	ｳｻﾀﾏ ｱ豬蠡・ﾌ ｿｹｾ犒ﾇｾ・ﾀﾖｴﾙ.
	BOOL		isBookedIGB();
	BOOL		isEngagedIGB();		//	ﾀﾎｽｺﾅﾏｽｺ ｱ豬蠡・ﾌ ｿｹｾ犒ﾇｾ・ﾀﾖｴﾙ.
	void		receiveInventoryInfo(cGuildInventoryItemForPacket	*_lpItemList,int _iCount);

	int			getInventorySlotIndex(int _iStorage,int _iSlot);
	int			getSlotIndexInStorage(int _iSlot);
	bool		isValidInventorySlot(int _iSlot);

	cItem*		getItem(int _iSlot)
	{
		return	(cItem*)&m_aInventory[_iSlot];
	}

	int			getCheckSum();

	int			getPetLevelByExp(UINT	_uiExp);
	int			getRentedPetCount(int _iPetType);

	CGuildPetInfo*	getPet(int _iPetType)	
	{
		return	&m_aGuildPet[_iPetType-1];
	}
	CGuildPetInfo*	getGuardian(int _iGuardianType)	
	{
		return	&m_aGuildGuardian[_iGuardianType-1];
	}

	inline	int	getPetExp(int _iPetType)	
	{
		return	(int)m_aGuildPet[_iPetType-1].m_dwExperience;
	}
	inline	int	getGuardianExp(int _iGuardianType)	
	{
		return	(int)m_aGuildGuardian[_iGuardianType-1].m_dwExperience;
	}
	inline	void	setPetExp(int _iPetType,int _iExp,int _iExpDuringWeek)	
	{
		m_aGuildPet[_iPetType-1].m_dwExperience		=	_iExp;
		m_aGuildPet[_iPetType-1].m_wExpDuringWeek	=	_iExpDuringWeek;
	}
	inline	void	setGuardianExp(int _iGuardianType,int _iExp,int _iExpDuringWeek)	
	{
		m_aGuildGuardian[_iGuardianType-1].m_dwExperience	=	_iExp;
		m_aGuildGuardian[_iGuardianType-1].m_wExpDuringWeek	=	_iExpDuringWeek;
	}

	int			getPetRemainExpForLevelUp(int _iPetType);
	int			getGuardianRemainExpForLevelUp(int _iGuardianType);

	char*		getGuildGuardianToolTip(int _iType);
	char*		getGuildPetToolTip(int _iType);

	int			getPetLevel(int _iPetType);
	int			getGuardianLevel(int _iGuardianType);
	void		feedItemToGuildPet(int _iPetType,int _iItemSlot,int _iFeedExp,int _iExp,int _iExpDuringWeek);

//ｦｮｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬ
//ｦｭｱ豬・ｸ篁・ｰ・ﾃ 
	void		sortMemberList(int _iMethod=-1);
	CSimpleGuildMemberInfo*	getMember(int _iMember);
	CSimpleGuildMemberInfo*	getMember(char *_lpstrName);
	int			getMemberIndex(char *_lpstrName);
	
	void		updateMember(char *_lpstrName,int _iLevel,int _iRank,int _iJob,BOOL _bIsConnected,int _iReserverPet);
	void		removeMember(char *_lpstrName);
//ｦｭｱ豬・ｸ篁・ｰ・ﾃ 
//ｦｱｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬ

//ｦｮｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬ
//ｦｭｱ豬・ｽｺﾅｳ ﾀ釤・ｰ・ﾃ 
	int			getSkillLimitLevel(int _iSkill);	//	ﾀﾌｷｱﾀ弡ｱ ｻｲﾀｻ ｰ昞ﾁﾇﾘ ｰ霆・ﾑ ﾇﾑｰ・ｷｹｺｧ
	int			getSkillTopLevel(int _iSkill);		//	ﾃﾖｰ昞ｹｺｧ(ﾀﾌ ﾀﾌｻ・ｸ・ｿﾃｶ｣ｴﾙ.)
	int			getApplySkillLevel(int _iSkill);

	BOOL		isUpgradeAbleSkill(int _iSkill);	//	ｾﾗｷｹﾀﾌｵ・ｰ｡ｴﾉﾇﾑ ｽｺﾅｳﾀﾌｴﾙ.

	void		operateGuildSkillData();
	void		operateGuildSkillData(int _iSkill,int _iLevel,int _iEffectLimitCount=dMAX_GUILD_SKILL_EFFECT_COUNT);

	int			operateGSIncreaseGuildVillage(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);	//	ｱ豬・ｿｬｰ暿・ｼ・ﾁ｡
	int			operateGSIncreaseMaxGuildMemberCount(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);	//	ﾃﾖｴ・豬蠢｡
	int			operateGSIncreaseMaxSubGuildMasterCount(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);	//ﾃﾖｴ・ｭｺ・ｶｽｺﾅﾍｼ｡
	int			operateGSIncreaseMaxGuildCongressCount(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);	//	ﾃﾖｴ・豬蠡ﾇｿﾎｿ｡
	int			operateGSIncreaseGuildInventorySize(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);	//	ｱ豬蠡ﾎｺ･ﾅ荳ｮｻ鄲ﾌﾁ錝｡


	int			operateGSIncreaseGuardianCount(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);
	int			operateGSIncreaseGuardianSight(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);
	int			operateGSDecreaseMagicResistanceFieldPenelty(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);
	int			operateGSIncreasePetCount(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);
	int			operateGSAddSellMagicEnchantItem(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);
	int			operateGSDecreaseDungeonMagicPeneltyPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);
	int			operateGSDecreaseDeathPeneltyPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);

	int			operateGSIncreaseDamageUnderSiegeWarfare(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);	//	ｰｺﾀ・ﾃ ｴ・ﾌﾁ・ﾁ｡
	int			operateGSReduceDamageUnderSiegeWarfare(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);	//	ｰｺﾀ・ﾃ ｴ・ﾌﾁ・ｰｨｼﾒ
	int			operateGSIncreaseGuildPetSight(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);	//	ｱ豬衄・ｽﾃｾﾟ ﾁ｡
	int			operateGSIncreaseGuildCrestEfficient(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);	//	ｱ豬蟷ｮﾀ衒ｿﾀｲﾁ｡",
	int			operateGSIncreaseGuildFlagEfficient(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);	//	"ｱ豬螻・ﾟﾈｿﾀｲﾁ｡",
	int			operateGSIncreaseAstroBowDamage(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);	//	"ｾﾆｽｺﾆｮｷﾎｺｸｿ・・ﾌﾁ｡",
	int			operateGSIncreaseGuildDoorEfficient(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);	//	"ｱ豬蟷ｮﾈｿﾀｲﾁ｡",
	int			operateGSIncreaseGuildPetAttackSpeed(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);	//	"ｱ豬衄・ﾗｼﾇｼﾓｵｵﾁ｡",
	int			operateGSDecreaseGuildShopItemPrice(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);	//	"ｱ豬蟒｡ｾﾆﾀﾌﾅﾛｰ｡ｰﾝｰｨｼﾒ",
	int			operateGSIncreaseDashBladeDamage(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue=FALSE);	//	"ｴ・ｬｺ昞ｹﾀﾌｵ蟠・ﾌﾁ｡",
//ｦｭｱ豬・ｽｺﾅｳ ﾀ釤・ｰ・ﾃ 
//ｦｱｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬｦｬ
};

class	CGuildMarkPackManager	: public CGuildMarkItemPackInfo
{
public:
	BOOL		init();	//	ｱ豬・ｸｶﾅｩ ｾﾆﾀﾌﾅﾒ ﾁ､ｺｸ ﾃﾊｱ篳ｭ..
	BOOL		save();	//	ｱ豬・ｸｶﾅｩ ｾﾆﾀﾌﾅﾛ ﾁ､ｺｸ 
	BOOL		load();	//	ｱ豬・ｸｶﾅｩ ｾﾆﾀﾌﾅﾛ ﾁ､ｺｸ ｺﾒｷｯｿﾀｱ・	BOOL		initForDev();

};

class	CGuildMarkInfo2	: public CGuildMarkInfo
{
public:
	WORD	m_wSerial;
	WORD	m_bf3HallLevel	:	3;
	WORD	m_bf8Level		:	8;
	WORD	m_bf1IsTwinkle	:	1;
};

class	CGuildManager
{
public:
	CGuildMarkInfo2		m_aGuildMark[dGUILD_MAX_COUNT];
	WORD				m_wPlayLevelForGuildDungen;

	void				init();
	void				setGuildMark(int _iGuild,CGuildMarkInfo *_lpMark,BOOL _bIsTwinkle,int _iGuildLevel,int _iHallLevel);
	CGuildMarkInfo2*	getGuildMark(int _iGuild);
};

extern	CGuild					g_guild;
extern	CGuildMarkPackManager	g_guildMarkPackManager;
extern	CGuildManager			g_guildManager;
extern	cGuildBattleInfo		g_guildBattleInfo;
extern	int						g_iRemainGuildBattleTime;

#endif