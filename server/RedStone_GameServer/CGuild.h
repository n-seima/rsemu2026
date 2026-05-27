#ifndef _classGuild_H
#define _classGuild_H

#include "CGuildDefine.h"
#include "CGuildSkill.h"
#include "cPACKET_GAMESERVER.h"

//
//	Guild Battle
enum
{
	eGB_INIT,
	eGB_ENTERENCE,
	eGB_READY,
	eGB_BATTLE,
	eGB_READY_TO_BATTLE_FINISH,
	eGB_FINISH,
	eGB_END,
	eGB_INCORRECT_BATTLE,	//	한쪽 혹은 양쪽에 인간들이 안들어와서 파토난 게임이다.

	eGB_EARLY_TIME,
	eGB_OFF_TIME,
	eGB_SLEEP_TIME,
};

class	cACTOR;

class	CGuildInfoInGS
{
public:
	DWORD		m_dwExpInServer;
	WORD		m_wIsWaitGuildWorkResult;
	WORD		m_wExistTodayBattleSchedule;
	WORD		m_wExistTomorrowBattleSchedule;

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
	WORD		m_wDashBladeCount;
	WORD		m_wAstralBowCount;
	WORD		m_wCristalWaterCount;
	WORD		m_wSellEnchantItemCount;
	WORD		m_awSellEnchantItem[c_iGuildHallSellEnchantItemCount];
	WORD		m_wDecreaseStructurePrice;
	WORD		m_wDecreaseDungeonMagicPenelty;
	WORD		m_wDecreaseDeathPenelty;

	WORD		m_awGuardianLevel[5];
	WORD		m_awPetLevel[5];

	CGuildInfoInGS*	getInfoInGS()
	{
		return	this;
	}
};

class	CGuild : public CGuildAdvanceInfo,public CGuildInfoInGS
{
public:
	WORD		m_wTryAddRelatedPlaceField;
	UINT		m_bf1IsMoveToPointBattleField	:	1;

	CGuildMan	m_aMembers[dGUILD_MAX_PLAYER];

				CGuild();

	int			getCheckSum();
	int			getCheckSumForClient();
	
	void		reset();
	BOOL		setMember(int _iSlot,CGuildMan *_lpMember);
	BOOL		addMember(int _iSlot,CGuildMan *_lpMember);
	BOOL		removeMember(char *_lpstrName);
	CGuildMan*	getMember(char *_lpstrName);
	inline	CGuildMan*	getMember(int _iIndex)
	{
		if	(_iIndex	<	0	||	_iIndex	>=	dGUILD_MAX_PLAYER)
			return	NULL;

		return	&m_aMembers[_iIndex];
	}
	CGuildMan*	getMaster();
	BOOL		isRecentlyBattleGuild(int _iGuildSerial);

	void		updateExp(int _iLevel,DWORD _dwExp,int _iGuildPoint,int _iExpTaxRate);
	BOOL		setGuildHallLevel(int _iLevel);
	BOOL		makeLevel(int _iLevel);	
	BOOL		changeRank(char *_lpstrName,int _iRank);
	void		resetAllMembersRank(int _iRank);

	void		updateGuildMemberLevel(char *_lpstrMember,int _iLevel,BOOL _bIsSendToWorld=TRUE);
	void		updateSkillLevel(int _iSkill,int _iLevel,int _iRemainSkillPoint);	//	스킬레벨 증가
	void		updateGuildInfo();
	
	void		updateGuildMemberCount();
	void		changeExpTaxRate(int _iTaxRate);
	void		changeNotice(char *_lpstrNotice);

	void		dissolutionGuild();
	void		selectGuildMark(CGuildMarkInfo *_lpMarkInfo=NULL,BOOL _bIsUseGuildMarkPack=FALSE);	//	길드 마크 변경
	BOOL		askSelectGuildMark(CGuildMarkInfo *_lpMarkInfo);	//	월드에 길드 마크 선택할것을 요청한다.
	void		checkRequireMasterGuildSkill(cACTOR *_lpActor);	//	길마가 필요한 스킬들 체크

	int			getOrganizeDayCount(int _iCheckCount = 0x7fffffff);	//	생성한지 몇일이나 됐냐?
	void		getGuildBattleInfoForMember(CGuildBattleInfoForMember *_lpToday);	//	멤버들을 위한 길드전 정보
	
	BOOL		isSellItem(int _iBaseItem);	//	판매 아이템이냐?
	int			getStorageIndexBySlot(int _iInventrySlot);
	int			getGuardianLevel(UINT _uiExp);	//	길드 가디언 레벨 구하기
	int			getGuardianExpTableValue(int _iLevel);
	int			getRentedPetCount(int _iPetType);
	void		buildReservePetInfo();
	UINT		getGuildPetExpTable(int _iLevel);	// 길드펫 경험치 테이블
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CGuildSendPacket.cpp
	void		sendEtcWork(int _iWork,int _iValue1=0,int _iValue2=0,int _iValue3=0,int _iValue4=0,int _iValue5=0,int _iValue6=0);
	void		sendUpdateMemberInfo(CGuildMan *_lpMember);	//	멤버 정보 업데이트
	void		sendRemoveMember(char *_lpstrName);
	void		sendReadyToNextGuildBattleMessageToMember(char *_lpTargetGuildName,int _iHour,int _iMinute);
	BOOL		sendGuildMemberList(cACTOR *_lpActor);

	void		addSendPacketToAllMember(void *_lpPacket);	
//┃CGuildSendPacket.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃길드 스킬 적용 관련 
	
	BOOL		checkLevelUpAbleGuildStatue(int _iLimitLevel);
	BOOL		increaseRandomGuildStatueLevel(int _iLimitLevel);

	BOOL		checkRequireTitle(int _iSkill,cACTOR *_lpMaster);
	BOOL		isUpgradeAbleSkill(int _iSkill,cACTOR *_lpMaster);

	int			getSkillLimitLevel(int _iSkill);	//	이런저런 상황을 고려해 계산한 한계 레벨
	int			getSkillTopLevel(int _iSkill);		//	최고레벨(이 이상 못 올라간다.)
	int			getApplySkillLevel(int _iSkill);

	void		operateGuildSkillData();
	void		operateGuildSkillData(int _iSkill,int _iLevel,int _iEffectLimitCount=dMAX_GUILD_SKILL_EFFECT_COUNT);

	void		operateGSIncreaseGuildVillage(int _iSkill,int _iLevel);
	void		operateGSIncreaseMaxGuildMemberCount(int _iSkill,int _iLevel);	//	최대길드원증가
	void		operateGSIncreaseMaxSubGuildMasterCount(int _iSkill,int _iLevel);	//최대서브마스터수증가
	void		operateGSIncreaseMaxGuildCongressCount(int _iSkill,int _iLevel);	//	최대길드의원인원수증가
	void		operateGSIncreaseGuildInventorySize(int _iSkill,int _iLevel);	//	길드인벤토리사이즈증가

	void		operateGSIncreaseGuardianCount(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);
	void		operateGSIncreaseGuardianSight(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);
	
	void		operateGSDecreaseMagicResistanceFieldPenelty(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);
	void		operateGSIncreasePetCount(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);
	void		operateGSAddSellMagicEnchantItem(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);
	void		operateGSDecreaseStructurePricePerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);
	void		operateGSDecreaseDungeonMagicPeneltyPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);
	void		operateGSDecreaseDeathPeneltyPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);

	void		operateGSIncreaseDamageUnderSiegeWarfare(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);	//	공성전시 대미지 증가
	void		operateGSReduceDamageUnderSiegeWarfare(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);	//	공성전시 대미지 감소
	void		operateGSIncreaseGuildPetSight(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);	//	길드펫 시야 증가
	void		operateGSIncreaseGuildCrestEfficient(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);	//	길드문장효율증가",
	void		operateGSIncreaseGuildFlagEfficient(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);	//	"길드깃발효율증가",
	void		operateGSIncreaseAstroBowDamage(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);	//	"아스트로보우대미지증가",
	void		operateGSIncreaseGuildDoorEfficient(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);	//	"길드문효율증가",
	void		operateGSIncreaseGuildPetAttackSpeed(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);	//	"길드펫액션속도증가",
	void		operateGSDecreaseGuildShopItemPrice(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);	//	"길드상점아이템가격감소",
	void		operateGSIncreaseDashBladeDamage(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect);	//	"대쉬블레이드대미지증가",
//┃길드 스킬 적용 관련 
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
};

class	CGuildManager
{
public:
	CTimeInfo			m_timeLastUpdate;

	int					m_iReceiveGuildCount,m_iReceiveInventoryCount,m_iReceiveMemberCount;
	int					m_iGuildCount,m_iUpdateFrameCounter;
	int					m_iLastCheckGuildInfo,m_iLastCheckWeekDay;
	CGuild				m_aGuild[dGUILD_MAX_COUNT];
	WORD				m_awGuildBattleOrder[6][c_iGuildHallCount];	//	홀 레벨이 0이면 빈거다.
	cGuildHallInfo		m_aGuildHallList[6][c_iGuildHallCount];	//	홀 레벨이 0이면 빈거다.

						CGuildManager();
						~CGuildManager();

	void				buildHallInfo();
	void				reset();
	BOOL				loadGuildMarkItemInfo();	//	길드마크 아이템 정보 로딩

	BOOL				setGuild(int _iIndex,CGuildAdvanceInfo *_lpInfo);
	BOOL				removeGuild(char *_lpstrGuildName);
	BOOL				checkGuildData();			//	길드 데이터 체크
	int					getAvailGuildCount();

	CGuild*				getByName(char *_lpstrName);//	이름으로 길드 검색
	CGuild*				getBySerial(int _iSerial);	//	시리얼로 길드 검색
	CGuild*				getByIndex(int _iIndex);	//	인덱스로 길드 검색

	void				resetGuildHallInfo();
	void				buildSWSOrder();	//	길드 공성전 전투 순서 빌드
	void				updateGuildHallInfo(cGuildHallInfo* _lpList,int _iCount,BOOL _bIsBuildSWS);
	cGuildHallInfo*		getHallInfoByGuild(int _iGuildIndex);	//	인덱스 번째 홀의 정보를 리턴
	cGuildHallInfo*		getHallInfoByInvadeGuild(int _iGuildIndex);	//	//	공격 길드 정보로 길드홀 정보 찾는다.
	cGuildHallInfo*		getHallInfoByBattleFieldSerial(int _iFieldSerial);	//	전투 필드 시리얼로 길드홀 정보 찾는다.
	void				changeGuildMarkTwinkleStatus(int _iGuild,BOOL _bIsTwinkle);

	cGuildHallInfo*		getHall(int _iHallLevel,int _iHall);

	void				update();
	void				resetWeekFightInfo();
	void				checkGuildCheckSum();

	void				resetGuildHonorPoint();
	void				resetGuildPrologPoint();
	void				changeGuildHallOpenStatus(int _iGuild,int _iStatus);
	void				activeGuildMasterGrace(int _iGuild,int _iStatus);
	void				updateGuildPetExp(int _iGuild,int _iPetType,int _iExp,int _iExpDuringWeek);	//	펫 경험치 업데이트
	void				rentGuildPet(int _iGuild,int _iPetType,int _iMember);	//	펫 대여
	void				retrievalGuildPet(int _iGuild,int _iMember);	//	대여 한 펫 회수

	int					getGuildHallFieldSerial(CGuild *_lpGuild);
	int					getGuildHallSlotIndexByHallLevel(int _iHallLevel,int _iGuildIndex);
	int					getGuildHallOwnerGuildByFieldSerial(int _iGuildHallFieldSerial);
	BOOL				openBattleFieldDoor(int _iBattleOrder);	//	길드 공성전 필드 문 열기

	void				getGuildHallInfoByBattleOrder(int _iBattleOrder,int *_lpiGuildHallLevel,int *_lpiHallIndex);	//	길드홀 정보 얻기
	BOOL				noticeBeginSiegeWarfare(int _iBattleOrder);	//	길드 공성전 시작전 10분 공지. 이때부터 길드전 공성 필드 입장이 가능하다.
	BOOL				noticeEndSiegeWarfare(int _iBattleOrder);	//	길드 공성전 끝나기 5분 공지.
	BOOL				beginSiegeWarfare(int _iBattleOrder);	//	길드 공성전 시작. 잠긴 문을 연다.
	BOOL				endSiegeWarfare(int _iBattleOrder);	//	길드 공성전 종료

	void				operateSiegeWarfareResultDefenseHall(int _iHallLevel,int _iSlot);	//	방어 성공
	void				operateSiegeWarfareResultCaptureHall(int _iHallLevel,int _iSlot);	//	공성 성공

	BOOL				getSiegeWarFareInfoInCurrentTime(int _iGuild,cSiegeWarfareInfoOfGuild *_lpInfo,BOOL _bIsWantBattleTime=FALSE);	//	공성전 정보 구하기
	int					getSiegeWarfareOfGuildOrder(int _iHallLevel,int _iIndex);	//	길드전 순서 구하기
	DWORD				getGuildHallCheckSum();

	void				resetGuildSiegewarfareInfo();
	void				reset1DayGuildInfo();
	void				reset1WeekGuildInfo();

};

extern	CGuildManager			g_guildManager;
extern	CGuildMarkItemPackInfo	g_guildMarkItemPackInfo;
extern	BOOL					g_bIsCompleteReceiveGuildDataWork;
extern	int						g_aiHallCountByHallLevel[];
extern	int						g_iTransToGVGPeriod;	//	캐릭터 GVG 서버로 전송 하는 주기

#endif