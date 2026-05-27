//	개     요 : 트리거 시스템에 이은 카르마 시스템
//	
//	작성 날짜 : 2000.12.04

#ifndef _classKARMA_H
#define	_classKARMA_H

#include "SFC.H"
#include "CKarmaDefine.H"
//#include "cACTOR.h"

#define	dINRANGE_PARTY_MEMBER_DISTANCE_FOR_TRIGGER	1600

class	cACTOR;
class	cFIELD;
class	CKarmaContainer;

class	CKarmaItemBasicInfo
{
public:
	WORD		m_wItem;
	int			m_aValue[6];
	WORD		m_wStringSize	:	15;
	WORD		m_bf1IsAbsolute	:	1;
};

//	카르마의 반응
class CKarmaItem : public CKarmaItemBasicInfo
{
public:
	char		*m_strString;

				CKarmaItem()
				{	m_wStringSize	=	0;
					m_strString		=	NULL;
					m_wItem			=	0xffff;
					memset(m_aValue,0,sizeof(m_aValue));
				}

				~CKarmaItem()
				{
					pKILL(m_strString);
				}

	void		reset()
				{
					m_wStringSize	=	0;
					m_wItem			=	0xffff;
					memset(m_aValue,0,sizeof(m_aValue));

					pKILL(m_strString);
				}

	void		setString(char *str)
				{	pKILL(m_strString);

					if (str == NULL)
					{	m_wStringSize	=	0;
						return;
					}

					m_wStringSize	=	strlen(str)+1;
					m_strString		=	new char [m_wStringSize];
					strcpy(m_strString,str);
				}

	void		copy(CKarmaItem *reaction)
				{
					m_wItem		=	reaction->m_wItem;
					m_aValue[0]	=	reaction->m_aValue[0];
					m_aValue[1]	=	reaction->m_aValue[1];
					m_aValue[2]	=	reaction->m_aValue[2];
					m_aValue[3]	=	reaction->m_aValue[3];
					m_aValue[4]	=	reaction->m_aValue[4];
					m_aValue[5]	=	reaction->m_aValue[5];

					setString(reaction->m_strString);
				}

	BOOL					isCompleteTrigger(cACTOR *_lpAsker,cACTOR *_lpOwner,cFIELD *_lpField);	//	트리거 조건 만족?

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃트리거 
//┃CKarmaTrigger.cpp
	BOOL					checkLevel(cACTOR *_lpAsker);			//	레벨 체크
	BOOL					checkIsAssignedAnyGuild(cACTOR *_lpAsker);	//	어딘가의 길드에 속해있다.
	BOOL					checkRankInGuild(cACTOR *_lpAsker);		//	길드 안에 직위 체크
	BOOL					checkItem(cACTOR *_lpAsker);			//	아이템 체크
	BOOL					checkRemainItemSlotCount(cACTOR *_lpAsker);	//	남아 있는 빈 아이템 슬롯의 수
	BOOL					checkEquippedItem(cACTOR *_lpAsker);	//	특정 아이템을 착용하고 있냐?
	BOOL					checkGold(cACTOR *_lpAsker);			//	돈 체크
	BOOL					checkSkill(cACTOR *_lpAsker);			//	스킬 체크
	BOOL					checkMasterSkillCount(cACTOR *_lpAsker);//	마스터한 스킬의 갯수 체크
	BOOL					checkTitle(cACTOR *_lpAsker);			//	타이틀 체크
	BOOL					checkJob(cACTOR *_lpAsker);				//	직업 체크
	BOOL					checkAllignment(cACTOR *_lpAsker);		//	성향 체크
	BOOL					checkPlayTime(cACTOR *_lpAsker);		//	플래이 시간 체크
	BOOL					checkBasicPower(cACTOR *_lpAsker);		//	기본 능력치 체크
	BOOL					checkBookCompletePercentage(cACTOR *_lpAsker);		//	기본 능력치 체크
	BOOL					checkExistBook(cACTOR *_lpAsker);		//	기본 능력치 체크
	BOOL					checkExistPage(cACTOR *_lpAsker);		//	기본 능력치 체크
	BOOL					checkExistAddBookPageItem(cACTOR *_lpAsker);	//	책에 페이지 추가하는 아이템 체크
	BOOL					checkCurrentStatus(cACTOR *_lpAsker);	//	현재 상태 체크
	BOOL					checkPartyMemberCount(cACTOR *_lpAsker);//	파티 멤버 카운트
	BOOL					checkIsPartyLeader(cACTOR *_lpAsker);	//	파티 리더?

	BOOL					checkPartyMemberLevel(cACTOR *_lpAsker);//	파티 멤버 레벨
	BOOL					checkPartyMembersAssignedGuild(cACTOR *_lpAsker);//	파티 멤버들의 길드 가입 여부
	BOOL					checkAssignedGuildPartyMember(cACTOR *_lpAsker);//	길드에 가입해 있는 파티원
	BOOL					checkAllPartyMembersAssignedSameGuild(cACTOR *_lpAsker);//	파티원이 모두 같은 길드에 가입해 있냐?
	BOOL					checkGuildStatueSkillLevel(cACTOR *_lpAsker);//	길드 석상 스킬 레벨 체크
	
	BOOL					checkCheckGuildPlaceOfRelated(cACTOR *_lpAsker);	//	길드 연고지 체크
	BOOL					checkIsOwnGuildHall(cACTOR *_lpAsker);	//	소유 길드홀인지 체크
	BOOL					checkGuildHallLevel(cACTOR *_lpAsker);	//	길드홀 레벨 체크
	BOOL					checkRoenGoodWill(cACTOR *_lpAsker);	//	로엔의 호감도 체크.
	BOOL					checkRebirthCount(cACTOR *_lpAsker);	//	전생 횟수 체크.

	BOOL					checkAllPartyMembersOwnItem(cACTOR *_lpAsker);//	파티원이 소지 하고 있는 아이템 체크
	BOOL					checkAllPartyMembersNotOwnItem(cACTOR *_lpAsker);//	파티원 모두가 특정 아이템을 소지하고 있지 않다.
	BOOL					checkPartyOwnItem(cACTOR *_lpAsker);//	파티원 모두가 특정 아이템을 소지하고 있지 않다.
	BOOL					checkGoldSumOfPartyMembers(cACTOR *_lpAsker);//	파티원 모두의 골드를 체크한다.
	BOOL					checkGoldOfAllPartyMembers(cACTOR *_lpAsker);//	모든 파티원의 골드 소지량 검사
	BOOL					checkOwnGoldOfPartyMember(cACTOR *_lpAsker);//	파티원의 골드 소지량 검사
	BOOL					checkPartyMembersSkillLevel(cACTOR *_lpAsker);//파티원들의 스킬 체크
	BOOL					checkAllPartyMembersSkill(cACTOR *_lpAsker);//모든 파티멤버의 스킬 체크
	BOOL					checkPartyMembersAllignment(cACTOR *_lpAsker);//파티멤버의 성향 체크
	BOOL					checkTitleInPartyMember(cACTOR *_lpAsker);//파티 멤버중에 타이틀 체크
	BOOL					checkAllPartyMembersTitle(cACTOR *_lpAsker);
	BOOL					checkPartyMemberJob(cACTOR *_lpAsker);	//파티 멤버의 직업 체크
	BOOL					checkAllPartyMembersJob(cACTOR *_lpAsker);	//	모든 파티 멤버의 직업 체크
	BOOL					checkDeahtAllPartyMember(cFIELD *_lpField);	//	파티 멤버 전멸 체크
	BOOL					checkAllPartyMembersAllignment(cACTOR *_lpAsker);//모든 파티멤버의 성향 체크
	BOOL					checkPartyMembersBasicPower(cACTOR *_lpAsker);//파티멤버의 기본 능력치 체크
	BOOL					checkAllPartyMembersBasicPower(cACTOR *_lpAsker);//모든 파티멤버의 기본 능력치 체크
	BOOL					checkPartyMembersLocation(cACTOR *_lpAsker);//모든 파티멤버의 위치 체크
	BOOL					checkHaveMasterSkillPartyMemberCount(cACTOR *_lpAsker);	//	마스터한 스킬수 체크
	BOOL					checkAllPartyMembersMasterSkillCount(cACTOR *_lpAsker);	//	모든 파티 멤버의 마스터한 스킬 수 체크

	BOOL					checkCompleteQuest(cACTOR *_lpAsker,int _iQuestType);				//	완료한 퀘스트 체크
	BOOL					checkProcessQuest(cACTOR *_lpAsker,int _iQuestType);	//	소유하고 있는 퀘스트 체크
	BOOL					checkQuestProcessLevel(cACTOR *_lpAsker,int _iQuestType);//	진행중인 퀘스트의 상태
	BOOL					checkQuestQuestValue(cACTOR *_lpAsker,int _iQuestType);//	진행중인 퀘스트의 수치
	BOOL					isCheckedQuestCompleteChecker(cACTOR *_lpAsker,int _iQuestType);//	진행중인 퀘스트의 수치
	BOOL					checkQuestCountdownTimer(cACTOR *_lpAsker,int _iQuestType);//	진행중인 퀘스트의 수치
	BOOL					checkProcessAbleQuest(cACTOR *_lpAsker,int _iQuestType);	//	수행 가능한 퀘스트냐?

	BOOL					checkProcessQuestCount(cACTOR *_lpAsker);		//	진행중인 퀘스트의 수 체크
	BOOL					checkRemainProcessQuestSlot(cACTOR *_lpAsker);	//	퀘스트 추가 가능 여부?

	BOOL					checkOXQuizStartTime();//	OX 퀴즈 시작 시간 체크
	BOOL					checkWordQuizStartTime();
	BOOL					checkNotObjectMacro(cACTOR *_lpAsker);
	BOOL					checkCurrentTime(BOOL _isType3 = FALSE);	//	현재 시간 체크 
	BOOL					checkCurrentTime2(BOOL _isType4 = FALSE);
	BOOL					checkCurrentWeekDay();						//	요일 체크.
	BOOL					checkTodayTime();
	BOOL					checkCurrentMinute();	// 분체크..
	BOOL					checkPrizePlayer(cACTOR* _lpActor);

	BOOL					checkIsFullGuild();
	BOOL					checkIsTestServer();

	BOOL					checkEnemyCountInRange(cACTOR *_lpAsker);
	BOOL					checkRemainHP(cACTOR *_lpAsker,cACTOR *_lpOwner);
	BOOL					checkArriveArea(cACTOR *_lpAsker);
	BOOL					checkTargetFlee(cACTOR *_lpAsker);
	BOOL					checkUsedSkill(cACTOR *_lpAsker);
	BOOL					checkUsedItem(cACTOR *_lpAsker);
	BOOL					checkGuide(cACTOR *_lpAsker);

	BOOL					checkFieldValue(cFIELD *_lpField);
	BOOL					checkWorldValue();

	BOOL					checkDungeonValue(cFIELD *_lpField);
	BOOL					checkSecretDungeonPlayTime(cFIELD *_lpField);
	BOOL					checkSecretDungeonCountdownTimer(cFIELD *_lpField);

	BOOL					checkMainQuestValue(cACTOR *_lpAsker);
	BOOL					checkMainQuestSwitch(cACTOR *_lpAsker);

	BOOL					checkSex(cACTOR *_lpAsker);
	BOOL					checkAttendance(cACTOR *_lpAsker);
	BOOL					checkCheckOwnItemBooleanOption(cACTOR *_lpAsker);
	BOOL					checkCheckOwnItemBooleanOption(cACTOR *_lpAsker,int _iCategory,int _iExist_NotExist);
	BOOL					checkAttr(cACTOR *_lpAsker);
	BOOL					checkCanReceiveCH5RewardTitleBonus(cACTOR *_lpAsker);
	BOOL					checkEnableIncreaseBonusGuildPoint(cACTOR *_lpAsker);
	BOOL					checkGuildBonusSkillPointPenelty(cACTOR *_lpAsker);
	BOOL					checkCheckEnableIncreaseGuildStatueLevel(cACTOR *_lpAsker);
	BOOL					checkEventTime();


	BOOL					checkLastLevelWhatReceivedCH5EquipLevelBonus(cACTOR *_lpAsker);
	BOOL					checkSameLevelIncreaeHPAmulet(cACTOR *_lpAsker);
	BOOL					checkSameLevelIncreaeCPAmulet(cACTOR *_lpAsker);
	BOOL					checkTimeStamp(cACTOR *_lpAsker);

	BOOL					checkCH5HeavenBonusCount(cACTOR *_lpAsker);
	BOOL					checkMemberSex(cACTOR *_lpAsker);
	BOOL					checkPartyLevelRange(cACTOR *_lpAsker);
	BOOL					checkTargetMonsterLevel(cACTOR *_lpAsker,cACTOR *_lpOwner);
	BOOL					checkProbability();
	BOOL					checkProbability2();

	BOOL					checkAvailAddPet(cACTOR *_lpAsker);	//펫 추가 가능한지 체크
	BOOL					checkCurrentPetCount(cACTOR *_lpAsker);	//	현재 펫의 수 체크

	BOOL					checkLocal();			//	지역 체크
	BOOL					checkMonsterEvent(cACTOR *_lpOwner);	//	몬스터 이벤트 체크
	BOOL					checkDuelRanking(cACTOR *_lpOwner);	//	결투 랭킹 체크
	BOOL					checkSeasonVariable();
	BOOL					checkEventValue();
	BOOL					checkIshaveGrace(cACTOR *_lpAsker);
	BOOL					checkIshaveBlindEffect(cACTOR *_lpAsker);
	BOOL					checkTileSet(cFIELD *_lpField);	// 타일셋 체크
	BOOL					checkPartyMemberCountInCurrentField(cACTOR *_lpAsker);//	현재 필드에 있는 파티 멤버 카운트
	BOOL					checkTransformationMonster(cACTOR *_lpAsker);		// 변신중이다.
	BOOL					checkEventCountTheHours();			// 이벤트 남은 시간 체크
	BOOL					checkPlayerAction(cACTOR *_lpAsker);			// 플레이어 액션
	BOOL					checkSecretDungeonPlayCount(cACTOR *_lpAsker);//	비밀던전 횟수 체크.
	BOOL					checkEventTimeBeforeAfter();			// 이벤트 시간 전후 체크

//┃CKarmaTrigger.cpp
//┃트리거 
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
};

#define	dKARMA_NAME_LENGTH	32

class CKarmaBasicInfo
{
public:
	WORD				m_wSerial;			//	카르마 시리얼
	WORD				m_isWantAndCondition		: 1;	
	WORD				m_isWantNotComplete			: 1;
	WORD				m_bf1IsDisable				: 1;
	WORD				m_bf1IsForEnterFieldPlayer	: 1;
	WORD				m_bf7ActivateTriggerCount	: 7;
	WORD				m_bf1IsApplyAllMember		: 1;
	WORD	:	4;

	WORD				m_wReactionCount,m_wTriggerCount;		//	조건의 수,반응의 수
	WORD				m_wTitleLength;
	WORD				m_wActivateChance;
	
	WORD				m_wActiveCount;
	WORD				m_wActivePeriod;
};

//	카르마 시스템
class CKarma	: public CKarmaBasicInfo
{
	BOOL				_operate(cACTOR *_lpAsker,cACTOR *_lpOwner,BOOL *_lpbIsStopWork,cFIELD *_lpField,BOOL _bIsEnterField,int *_lpiJumpKarma,CKarmaContainer *_lpKC);

public:
	char				*m_pstrTitle;
	CKarmaItem			*m_pReaction,*m_pTrigger;

	WORD				m_wRemainCount;
	DWORD				m_dwLastOperateTime;
	WORD				m_bf1IsActive	:	1;

	//cACTOR				m_DummyActor;		//기술 강제 사용시 잠시 데이터를 세팅할 엑터.

						CKarma()
						{	
							m_pTrigger	=	NULL;
							m_pReaction	=	NULL;
							reset();

							m_pstrTitle	=	new char [dKARMA_NAME_LENGTH];
							strcpy(m_pstrTitle,"nameless Karma");
						}

						~CKarma()
						{
							pKILL(m_pTrigger);
							pKILL(m_pReaction);
							pKILL(m_pstrTitle);
						}

	void				resetToFirstStatus();
	void				reset();
	void				reset(int iTriggerCount,int iReactionCount);
	void				copy(CKarma *lpKarma);
	char*				getTitle(){return m_pstrTitle;}
	void				allocBuffer()
	{
		pKILL(m_pTrigger);
		pKILL(m_pReaction);
		pKILL(m_pstrTitle);

		if (m_wTitleLength	>	0)	m_pstrTitle	=	new char		[m_wTitleLength];
		if (m_wTriggerCount	>	0)	m_pTrigger	=	new	CKarmaItem	[m_wTriggerCount];
		if (m_wReactionCount>	0)	m_pReaction	=	new	CKarmaItem	[m_wReactionCount];
	}

	void				setTitle(char *_lpstr)
	{	
		pKILL(m_pstrTitle);

		if (_lpstr	== NULL)
		{	m_wTitleLength=	0;
			return;
		}

		m_wTitleLength	=	strlen(_lpstr)+1;
		m_pstrTitle		=	new char [m_wTitleLength];
		strcpy(m_pstrTitle,_lpstr);
	}

	BOOL				isCompleteTrigger(cACTOR *_lpAsker,cACTOR *_lpOwner,cFIELD *_lpField);
	BOOL				operateReaction(cACTOR *_lpAsker,cACTOR *_lpOwner,BOOL *_lpbIsStopWork,int _iPauseReaction=0,cFIELD *_lpField=NULL,int *_lpiJumpKarma=NULL,CKarmaContainer *_lpKC=NULL);
	BOOL				operateReactionItem(CKarmaItem	*_lpReaction,int *_lpiReactionIndex,cACTOR *_lpAsker,cACTOR *_lpOwner,
											BOOL *_lpbIsStopWork,int _iPauseReaction,cFIELD *_lpField,int *_lpiJumpKarma,CKarmaContainer *_lpKC);

	BOOL				operate(cACTOR *_lpAsker,cACTOR *_lpOwner,BOOL *_lpbIsStopWork,cFIELD *_lpField,BOOL _bIsEnterField,int *_lpiJumpKarma,CKarmaContainer *_lpKC);

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CKarmaReaction.cpp
//	리액션 돌리기
	BOOL				operateDuelWork(CKarmaItem *_lpReaction,cACTOR *_lpAsker);
	BOOL				selectSpeech(CKarmaItem *_lpReaction,cACTOR *_lpAsker,cACTOR *_lpOwner);
	BOOL				endDialog(cACTOR *_lpAsker,cACTOR *_lpOwner);
	BOOL				openShop(CKarmaItem *_lpReaction,cACTOR *_lpAsker,cACTOR *_lpOwner);
	BOOL				openBank(cACTOR *_lpAsker);
	BOOL				openUpgradeShop(cACTOR *_lpAsker);		// 강화창을 연다 09.08.25
	BOOL				openRelayStation(cACTOR *_lpAsker);

	BOOL				moveTo(cFIELD *_lpField,cACTOR *_lpAsker,cACTOR *_lpOwner,CKarmaItem *lpReaction);
	BOOL				moveToGuildHall(cACTOR *_lpAsker,CKarmaItem *_lpReaction);
	BOOL				setGuildHallLevel(cACTOR *_lpAsker,CKarmaItem *_lpReaction);
	BOOL				setGuildHallRoenGoolWill(CKarmaItem *_lpReaction , cACTOR *_lpAsker);
	BOOL				feedGuildPetAndGuardian(cACTOR *_lpAsker);

	BOOL				readyToMakeGuild(cACTOR *_lpAsker);
	BOOL				openGuildBattleScheduler(cACTOR *_lpAsker,CKarmaItem *_lpReaction);
	BOOL				openGuildMarkComposer(cACTOR *_lpAsker);	//	길드 마크 조합 인터페이스 오픈
	BOOL				increaseGuildStatueSkillLevel(cACTOR *_lpAsker,CKarmaItem *_lpReaction);
	BOOL				openGuildInventory(cACTOR *_lpAsker);
	BOOL				addRelatedPlaceOfGuild(cACTOR *_lpAsker,CKarmaItem *_lpReaction);
	BOOL				tryGuildPointBattle(cACTOR *_lpAsker);
	BOOL				tryGuildDungen(cACTOR *_lpAsker ,CKarmaItem* _lpReaction);
	BOOL				trySecretDungeon(cACTOR *_lpAsker ,CKarmaItem* _lpReaction);
	BOOL				giveGuildHonorPoint(cACTOR *_lpAsker,CKarmaItem *_lpReaction);
	BOOL				beginSiegeWarfareOfGuild(cACTOR *_lpAsker);
	BOOL				endGuildPointBattleAndRecordGuildHonorPoint(cACTOR *_lpAsker,CKarmaItem *lpReaction);

	BOOL				openRepairItemWindow(cACTOR *_lpOwner,cACTOR *_lpTarget);

	BOOL				setQuestStart(CKarmaItem *_lpReaction,cACTOR *_lpAsker,int _iType);
	BOOL				setQuestComplete(CKarmaItem *_lpReaction,cACTOR *_lpAsker,int _iType);
	BOOL				setQuestCompleteChecker(CKarmaItem *_lpReaction,cACTOR *_lpAsker,int _iType);

	BOOL				setQuestCountdownTimer(CKarmaItem *_lpReaction,cACTOR *_lpAsker,int _iType);

	BOOL				setQuestProcessLevel(CKarmaItem *_lpReaction,cACTOR *_lpAsker,int _iType);
	BOOL				setQuestValue(CKarmaItem *_lpReaction,cACTOR *_lpTarget,int _iType);
	BOOL				controlQuestValue(CKarmaItem *_lpReaction,cACTOR *_lpTarget,int _iType);
	BOOL				cancelQuest(CKarmaItem *_lpReaction,cACTOR *_lpTarget,int _iType);

	BOOL				shout(CKarmaItem *_lpReaction,cFIELD *_lpField);
	BOOL				talk(CKarmaItem *_lpReaction,cACTOR *_lpOwner);
	BOOL				talk(CKarmaItem *_lpReaction,cFIELD *_lpField,cACTOR *_lpAsker);
	BOOL				addItem(CKarmaItem *_lpReaction,cACTOR *_lpTarget);//	아이템 추가
	BOOL				addItemWithOption(CKarmaItem *_lpReaction,cACTOR *_lpTarget);	// 아이템추가(옵션)
	BOOL				addSetItemRandom(CKarmaItem *_lpReaction,cACTOR *_lpTarget);//	세트아이템 랜덤 추가
	BOOL				addItemPiece(CKarmaItem *_lpReaction,cACTOR *_lpOwner,cACTOR *_lpTarget);	//	아이템 조각 추가
	BOOL				addItemByKaram(cACTOR *_lpTarget,void *_lpItem,int _iTarget);
	BOOL				addItemByKaram(cACTOR *_lpTarget,void *_lpItem);
	BOOL				addGoldByKarma(cACTOR *_lpTarget,int _iGold,int _iReason);
	BOOL				addGoldByKarmaMulti(cACTOR *_lpTarget,int _iGold,int _iTarget,int _iReason);
	BOOL				removeGoldByKarma(cACTOR *_lpTarget,int _iGold,int _iReason);
	BOOL				removeGoldByKarmaMulti(cACTOR *_lpTarget,int _iGold,int _iTarget,int _iReason);

	
	BOOL				removeItemWithTarget(cACTOR *_lpTarget,int _iItem,int _iCount,int _iTarget,int _iIgnoreIncludeEquipment);

	BOOL				addGGGtoTarget(CKarmaItem *_lpReaction,cACTOR *_lpTarget);
	
	BOOL				dropGold(CKarmaItem *_lpReaction,cACTOR *_lpOwner,cACTOR *_lpTarget);//	골드 드랍
	BOOL				dropItem(CKarmaItem *_lpReaction,cACTOR *_lpOwner,cACTOR *_lpTarget);//	아이템 드랍
	BOOL				dropItemWithOption(CKarmaItem *_lpReaction,cACTOR *_lpOwner,cACTOR *_lpTarget, BOOL _bIsUnowned = FALSE);//	아이템 드랍(옵션)
	BOOL				addGold(CKarmaItem *_lpReaction,cACTOR *_lpAsker);//	돈 추가
	BOOL				addSpecificItem(CKarmaItem *_lpReaction,cACTOR *_lpTarget);
	BOOL				dropSpecificItem(CKarmaItem *_lpReaction,cACTOR *_lpOwner,cACTOR *_lpTarget);
	BOOL				dropSetItem(CKarmaItem *_lpReaction,cACTOR *_lpOwner,cACTOR *_lpTarget);	//	세트 아이템 드랍
	BOOL				buyGoldBar(CKarmaItem *_lpReaction,cACTOR *_lpTarget);		/// 09.08.19 금괴추가~~!~!~!
//------------------------------------
	BOOL				moveArea(CKarmaItem *_lpReaction,cACTOR *_lpAsker);//	영역 이동
	BOOL				selectOXQuizResult(CKarmaItem *_lpReaction,cFIELD *_lpField);//	OX 퀴즈 결과 설정
//------------------------------------

	BOOL				changeBattleInclination(CKarmaItem *_lpReaction,cFIELD *_lpField);
	BOOL				eventNotice(CKarmaItem *_lpReaction,cACTOR *_lpTarget,cFIELD *_lpField);
	BOOL				damageToCharacter(CKarmaItem *_lpReaction,cACTOR *_lpTarget,cACTOR *_lpOwner);//	대미지 입히기
	BOOL				getExperience(CKarmaItem *_lpReaction,cACTOR *_lpTarget);//	경험치 얻기
	BOOL				getExperience2(CKarmaItem *_lpReaction,cACTOR *_lpTarget);
	BOOL				getTitle(CKarmaItem *_lpReaction,cACTOR *_lpTarget);//	칭호 획득
	BOOL				resetPetName(CKarmaItem *_lpReaction,cACTOR *_lpTarget);//	펫 이름 초기화
	BOOL				addPet(CKarmaItem *_lpReaction,cACTOR *_lpTarget);//	펫 추가
	BOOL				delay(CKarmaItem *_lpReaction,int iReactionIndex,cACTOR *_lpOwner,cACTOR *_lpTarget,cFIELD *_lpField);//	딜레이
	BOOL				launchRandomKarma(CKarmaItem *_lpReaction,cACTOR *_lpAsker,cACTOR *_lpOwner,BOOL *_lpbIsStopWork,int _iPauseReaction,cFIELD *_lpField,int *_lpiJumpKarma,CKarmaContainer *_lpKC);
	BOOL				launchRandomReaction(CKarmaItem *_lpReaction,cACTOR *_lpAsker,cACTOR *_lpOwner,BOOL *_lpbIsStopWork,int _iPauseReaction,cFIELD *_lpField,int *_lpiJumpKarma,int *_lpiJumpReaction,CKarmaContainer *_lpKC);

	BOOL				chargeCP(CKarmaItem *_lpReaction,cACTOR *_lpTarget);//	챠징
	BOOL				healHP(CKarmaItem *_lpReaction,cACTOR *_lpTarget);
	BOOL				moveField(CKarmaItem *_lpReaction,cACTOR *_lpTarget);	//	필드 이동
	BOOL				removeCharacter(CKarmaItem *_lpReaction,cACTOR *_lpTarget);

	BOOL				setSecretDungeonCountdownTimer(CKarmaItem *_lpReaction,cFIELD *_lpField);
	BOOL				useHaveSkill(CKarmaItem *_lpReaction,cACTOR *_lpOwner);						// 몬스터 보유스킬 사용 09.08.14
	BOOL				useMonsterSkill(CKarmaItem *_lpReaction,cACTOR *_lpOwner);					// 몬스터 스킬사용	09.08.14
	BOOL				useSkill(int _iSkill, int _iLevel, cACTOR *_lpOwner);	
	BOOL				useSkillByCompulsion(CKarmaItem *_lpReaction,cACTOR *_lpOwner,cACTOR *_lpTarget);	//스킬을 강제적으로 발동 한다.
	BOOL				changeSightRange(CKarmaItem *_lpReaction,cFIELD *_lpField);
	BOOL				changeState(CKarmaItem *_lpReaction,cACTOR *_lpAsker,cACTOR *_lpOwner);
	BOOL				giveBlindEffect(CKarmaItem *_lpReaction,cACTOR *_lpAsker);
	void				giveState(cACTOR* _lpAsker , int _iState , int _iSecond,cACTOR *_lpOwner);
	BOOL				changeJob(CKarmaItem *_lpReaction,cACTOR *_lpOwner);
	BOOL				setGuide(CKarmaItem *_lpReaction,cACTOR *_lpAsker,cACTOR *_lpOwner);
	BOOL				setTraceGuide(CKarmaItem *_lpReaction,cACTOR *_lpOwner);
	BOOL				playPattern(CKarmaItem *_lpReaction,cACTOR *_lpOwner);
	BOOL				playBossEvent(CKarmaItem *_lpReaction,cFIELD	*_lpField);
	BOOL				giveScreenEvent(CKarmaItem *_lpReaction,cACTOR *_lpAsker,cFIELD	*_lpField);
	BOOL				fixFieldResistance(CKarmaItem *_lpReaction,cFIELD	*_lpField);
	BOOL				playScreenEvent(CKarmaItem *_lpReaction,cACTOR	*_lpAsker);
	BOOL				regenMonster(cACTOR *_lpOwner,CKarmaItem *_lpReaction,cFIELD *_lpField);
	BOOL				changeTargetByRandom(CKarmaItem *_lpReaction,cACTOR *_lpOwner);
	BOOL				removeTargeting(CKarmaItem *_lpReaction,cACTOR *_lpOwner);
	BOOL				releaseWrongStatus(CKarmaItem *_lpReaction,cFIELD *_lpField);
	BOOL				setInvincibleStatus(CKarmaItem *_lpReaction,cFIELD	*_lpField);
	BOOL				controlDungeonValue(CKarmaItem *_lpReaction,cFIELD	*_lpField);
	BOOL				controlFieldValue(CKarmaItem *_lpReaction,cFIELD	*_lpField);
	BOOL				controlWorldValue(CKarmaItem *_lpReaction);

	BOOL				setArcaStatus(CKarmaItem *_lpReaction,cFIELD *_lpField);
	BOOL				setDoorStatus(CKarmaItem *_lpReaction,cFIELD *_lpField);
	BOOL				setTrapStatus(CKarmaItem *_lpReaction,cFIELD *_lpField);
	BOOL				setArcaDoorTrapStatus(CKarmaItem *_lpReaction,cFIELD *_lpField);
	BOOL				setDoorEnableDisable(CKarmaItem *_lpReaction,cFIELD *_lpField);

	BOOL				beginEvent(CKarmaItem *_lpReaction,cFIELD *_lpField);
	BOOL				occurSkillEffect(CKarmaItem *_lpReaction,cFIELD *_lpField);
	BOOL				setGroundFocus(CKarmaItem *_lpReaction,cFIELD *_lpField);
	BOOL				setCharacterFocus(CKarmaItem *_lpReaction,cFIELD *_lpField);

	BOOL				extendSecretDungeonUpkeepTime(CKarmaItem *_lpReaction,cFIELD *_lpField);
	BOOL				shakeScreen(CKarmaItem *_lpReaction,cFIELD *_lpField);
	BOOL				playSound(CKarmaItem *_lpReaction,cFIELD *_lpField);
	BOOL				stressMinimap(CKarmaItem *_lpReaction,cFIELD *_lpField);
	BOOL				npcAction(CKarmaItem *_lpReaction,cACTOR *_lpAsker,cFIELD *_lpField);
	BOOL				completeSecretDungeon(CKarmaItem *_lpReaction,cFIELD *_lpField);
	
	BOOL				setChapter(CKarmaItem *_lpReaction,cACTOR *_lpAsker);
	BOOL				setMainQuestValue(CKarmaItem *_lpReaction,cACTOR *_lpAsker);
	BOOL				setMainQuestSwitch(CKarmaItem *_lpReaction,cACTOR *_lpAsker);
	BOOL				resetMainQuestValue(cACTOR *_lpAsker);
	BOOL				resetMainQuestSwitch(cACTOR *_lpAsker);
	BOOL				displayMainQuestEpilog(CKarmaItem *_lpReaction,cACTOR *_lpAsker);
	BOOL				writeBookPage(CKarmaItem *_lpReaction,cACTOR *_lpAsker);
	BOOL				extendItemValidateTimeToNextDay(CKarmaItem *_lpReaction,cACTOR *_lpAsker);
	BOOL				explosionEventBalloon(CKarmaItem *_lpReaction,cACTOR *_lpAsker);
	BOOL				createBook(CKarmaItem *_lpReaction,cACTOR *_lpAsker);
	BOOL				createAddCategoryToBookItem(CKarmaItem *_lpReaction,cACTOR *_lpAsker);
	BOOL				createAddPageToBookItem(CKarmaItem *_lpReaction,cACTOR *_lpAsker);
	BOOL				createAddRandomPageToBookItem(CKarmaItem *_lpReaction,cACTOR *_lpAsker);
	BOOL				createAddRandomCategoryToBookItem(CKarmaItem *_lpReaction,cACTOR *_lpAsker);

	BOOL				changeName(CKarmaItem *_lpReaction,cACTOR * _lpOwner);
	BOOL				setEventValue(CKarmaItem *_lpReaction,cACTOR *_lpOwner);

	BOOL				increaseEventValue(CKarmaItem *_lpReaction,cACTOR *_lpTarget);
	BOOL				giveRedStonePiece(CKarmaItem *_lpReaction,cACTOR *_lpTarget,cACTOR *_lpOwner);
	//	붉은 보석 조각 주기, 전율스런 막 코드다. - -
	

	BOOL				timeStamp(CKarmaItem *_lpReaction,cACTOR *_lpTarget);
	BOOL				heavenReward(CKarmaItem *_lpReaction,cACTOR *_lpTarget);
	BOOL				hellReward(CKarmaItem *_lpReaction,cACTOR *_lpTarget);
	BOOL				redDevilReward(CKarmaItem *_lpReaction,cACTOR *_lpTarget);
	BOOL				changeWeather(CKarmaItem*	_lpReaction, cFIELD* _lpField);	// 09.10.07 기후변화
	BOOL				dispellAllBuff(CKarmaItem *_lpReaction,cACTOR *_lpAsker);
	BOOL				releaseTransformationMonster(CKarmaItem *_lpReaction,cACTOR *_lpAsker);
	BOOL				setIncreaseInstantExperience(CKarmaItem *_lpReaction,cACTOR *_lpAsker);	// 일시적 경험치 증가
	BOOL				showFireWorkInField(CKarmaItem *_lpReaction,cACTOR *_lpAsker);	// 불꽃놀이 이팩트 필드에 출력
	BOOL				drawImageInScreen(CKarmaItem *_lpReaction,cACTOR *_lpAsker);	// 화면에 이미지 출력
	BOOL				offDrawingImageInScreen(CKarmaItem *_lpReaction,cACTOR *_lpAsker);	// 화면에 그린 이미지 끄기
	BOOL				openInterface(CKarmaItem *_lpReaction,cACTOR *_lpAsker);	//  인터페이스 열기
	BOOL				openFastPortalWindow(CKarmaItem *_lpReaction,cACTOR *_lpAsker);	//  패스트 포탈 창 열기
//┃CKarmaReaction.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

};	//	class CKarma

class	cDelayKarmaInfo
{
public:
	WORD				m_wPauseReaction;
	CKarma				*m_lpKarma;
	DWORD				m_dwReleaseDelayTime;
	DWORD				m_dwOwnerNameHashCode,m_dwTargetNameHashCode;
	cACTOR				*m_lpOwner,*m_lpTarget;
	cFIELD				*m_lpField;

						cDelayKarmaInfo()
						{
							reset();
						}

	void				reset();
	BOOL				isIncorrect();
};

#define	dMAX_DELAY_KARMA_COUNT	1024

class	cDelayKarmaManager
{
public:
	cDelayKarmaInfo		m_aDelayKarma[dMAX_DELAY_KARMA_COUNT];
	BOOL				addDelayKarma(CKarma *_lpKarma,int _iPauseReaction,DWORD _dwReleaseDelayTime,cACTOR *_lpOwner,cACTOR *_lpTarget,cFIELD *_lpField);
	void				updateDelayKarma();
};

extern	cDelayKarmaManager	g_delayKarmaManager;

class	CKarmaContainerBasicElement
{
public:
	WORD				m_wSerial;	//	대사 길이
	WORD				m_wTitleLength;	//	대사 길이
	WORD				m_wKarmaCount;	//	이 대화가 처음에 시작될 조건 수
};

class	CKarmaContainer : public CKarmaContainerBasicElement
{
public:
	char				*m_pstrTitle;	//	선택문
	CKarma				*m_pKarma;				//	선택 항목

						CKarmaContainer()
						{
							m_pstrTitle	=	NULL;	//	선택문
							m_pKarma	=	NULL;				//	선택 항목

							reset();
						}

						~CKarmaContainer()
						{
							reset();
						}

	void				setTitle(char *_lpstr)
	{	
		pKILL(m_pstrTitle);

		if (_lpstr	== NULL)
		{	m_wTitleLength=	0;
			return;
		}

		m_wTitleLength	=	strlen(_lpstr)+1;
		m_pstrTitle		=	new char [m_wTitleLength];
		strcpy(m_pstrTitle,_lpstr);
	}

	int					getKarmaCount()			{	return	m_wKarmaCount;}

	void				allocBuffer(int _iKarmaCount);
	void				reset()
	{
		pKILL(m_pstrTitle);
		pKILL(m_pKarma);
		m_wSerial		=	0xffff;	//	대사 길이
		m_wTitleLength	=	0;		//	대사 길이
		m_wKarmaCount	=	0;		//	이 대화가 처음에 시작될 조건 수
	}

	void				copy(CKarmaContainer *_lpKarma);
	void				resetToFirstStatus();

	int					add(CKarma *_lpKarma);
	BOOL				remove(int _iIndex);

	void				operate(cACTOR *_lpAsker,cACTOR *_lpOwner,cFIELD *_lpField,BOOL _bIsEnterField=FALSE);	//	작동!!
};	//	class	CKarmaContainer

//
//	Area Door Trap Work by Karma
enum
{
	eADTWK_REVEAL	,
	eADTWK_HIDE		,
	eADTWK_REGEN	,
	eADTWK_REMOVE	,
	eADTWK_LOCK		,
	eADTWK_UNLOCK	,
	eADTWK_CLOSE	,
	eADTWK_OPEN		,
	eADTWK_ENABLE_DOOR	,
	eADTWK_DISABLE_DOOR	,
};

//
//	Character Direct by Karma
enum
{
	eCDK_CURRENT,
	eCDK_SPEECH_TARGET,
	eCDK_GUIDE,
	eCDK_PARTY_LEADER,
	eCDK_N,
	eCDK_NE,
	eCDK_E,
	eCDK_SE,
	eCDK_S,
	eCDK_SW,
	eCDK_W,
	eCDK_NW,
};

#define dMAX_ITEM_EFFECT_KARMA				1024

extern CKarma g_ItemEffectKarma[dMAX_ITEM_EFFECT_KARMA];

#endif