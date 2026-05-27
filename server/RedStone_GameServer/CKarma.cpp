#include "CKarma.h"
#include "cACTOR.h"
#include "CQuest.h"
#include "debugCode.h"
#include "cGame.h"

CKarma	g_ItemEffectKarma[dMAX_ITEM_EFFECT_KARMA];

void
CKarma::resetToFirstStatus()
{
	m_wRemainCount	=	m_wActiveCount;

	if	(m_wActiveCount	==	0)
		m_wRemainCount	=	0xffff;

	m_bf1IsActive	=	1-m_bf1IsDisable;
}

//
//	초기화
void
CKarma::reset()
{
	m_wSerial			=	0xffff;	//	카르마 시리얼
	m_isWantAndCondition=	TRUE;
	m_isWantNotComplete	=	FALSE;
	m_bf1IsDisable		=	FALSE;

	m_wReactionCount	=	0;		//	반응의 수
	m_wTriggerCount		=	0;		//	조건의 수
	m_wActiveCount		=	0;
	m_wActivePeriod		=	0;
	m_dwLastOperateTime	=	0;

	pKILL(m_pTrigger);
	pKILL(m_pReaction);
}

//
//	초기화
void
CKarma::reset(int iTriggerCount,int iReactionCount)
{
	reset();

	m_pTrigger	=	new	CKarmaItem [iTriggerCount];
	m_pReaction	=	new	CKarmaItem[iReactionCount];
}

//
//	복사
void
CKarma::copy(CKarma *_lpKarma)
{
	int	i;

	reset(_lpKarma->m_wTriggerCount,_lpKarma->m_wReactionCount);

	memcpy(this,_lpKarma,sizeof(CKarmaBasicInfo));	//	수치들

	setTitle(_lpKarma->m_pstrTitle);

	for (i=0;i<m_wTriggerCount	;i++)
		m_pTrigger[i]	.copy(&_lpKarma->m_pTrigger[i]);
	for (i=0;i<m_wReactionCount	;i++)
		m_pReaction[i]	.copy(&_lpKarma->m_pReaction[i]);
}

//
//	가지고 있는 트리거 조건이 만족되냐?
BOOL
CKarma::isCompleteTrigger(cACTOR *_lpAsker,cACTOR *_lpOwner,cFIELD *_lpField)
{
	g_debugSign.m_iTempStep	=	11;

	int	i,iCompleteTriggerCount=0;

	if	(m_wTriggerCount	<=	0)
		return	TRUE;

	if (g_bIsWantPrintTriggerCondition)
	{
		char	*strCondition[]	=	{"or","and"};
		char	*strNot[]		=	{"만족","불만족"};

		printf("        트리거 체크 시작 - 조건 %s/%s \n",strCondition[m_isWantAndCondition],strNot[m_isWantNotComplete]);
	}

	g_debugSign.m_iTempStep	=	12;

	if (!_lpField)
	{
		if (_lpOwner)
			_lpField	=	_lpOwner->m_lpField;
		else
		if (_lpAsker)
			_lpField	=	_lpAsker->m_lpField;
	}

	int	iRequireCompleteTriggerCount	=	1;

	if	(m_isWantAndCondition)
		iRequireCompleteTriggerCount	=	m_wTriggerCount;
	else
	if	(m_bf7ActivateTriggerCount)
	{
		iRequireCompleteTriggerCount	=	m_bf7ActivateTriggerCount;
		iRequireCompleteTriggerCount	=	min(iRequireCompleteTriggerCount,m_wTriggerCount);
	}

	for (i=0;i<m_wTriggerCount;i++)
	{
		BOOL	isComplete		=	m_pTrigger[i].isCompleteTrigger(_lpAsker,_lpOwner,_lpField);

		if	(isComplete	==	FALSE	&&	(m_pTrigger[i].m_bf1IsAbsolute || m_isWantAndCondition== TRUE ))	//	절대 트리거 체크
		{
			if	(m_isWantNotComplete)	//	실패 해야 성공하는 카르마
				return	TRUE;

			return	FALSE;
		}

		iCompleteTriggerCount	+=	isComplete;
	}

	g_debugSign.m_iTempStep	=	13;

	if	(m_isWantNotComplete)
	{
		if	(iCompleteTriggerCount	>=	iRequireCompleteTriggerCount)
			return	FALSE;

		return	TRUE;
	}

	if	(iCompleteTriggerCount	>=	iRequireCompleteTriggerCount)
		return	TRUE;

	g_debugSign.m_iTempStep	=	14;

	return	FALSE;
}

BOOL
CKarma::_operate(cACTOR *_lpAsker,cACTOR *_lpOwner,BOOL *_lpbIsStopWork,cFIELD *_lpField,BOOL _bIsEnterField,int *_lpiJumpKarma,CKarmaContainer *_lpKC)
{
	if	(m_bf1IsForEnterFieldPlayer	!= _bIsEnterField)
		return	TRUE;

	if	(m_wRemainCount	==	0 || m_bf1IsActive == FALSE)
	{
		if	(g_bIsWantPrintTriggerCondition)
			printf("      남은 횟수 %d,활성화 상태 %d\n",m_wRemainCount,m_bf1IsActive);

		return	TRUE;
	}

	if	(m_wActivePeriod)
	{
		if	(g_iCounterBySecond-m_dwLastOperateTime	<	m_wActivePeriod)
			return	TRUE;

		m_dwLastOperateTime	=	g_iCounterBySecond;
	}

	g_debugSign.m_iTempStep	=	9;

	*_lpbIsStopWork	=	FALSE;

	if	(m_wActivateChance	!=	100)
	{
		int		iRandomValue=	random(100);
		BOOL	bIsOk		=	FALSE;
		int		iActiveChance=	m_wActivateChance;

		if	(_lpAsker	&&	_lpAsker->m_bf1IsTestQuest)
			iActiveChance	=	100;

		if	(iRandomValue	<	iActiveChance	)
			bIsOk		=	TRUE;

		if	(g_bIsWantPrintTriggerCondition)
		{
			char	*strCheck[]	=	{"발동 안함!!","발동!!"};

			printf("      발동확률 %d/%d 결과 %s\n",m_wActivateChance,iRandomValue,strCheck[bIsOk]);
		}

		if	(bIsOk == FALSE)
			return	FALSE;
	}

	g_debugSign.m_iTempStep	=	10;

	if	(!isCompleteTrigger(_lpAsker,_lpOwner,_lpField)	)
	{
		if	(g_bIsWantPrintTriggerCondition)
			printf("        트리거 조건 불만족\n");

		return	FALSE;
	}

	if	(m_wRemainCount	!=	0xffff)
		m_wRemainCount--;

	if	(g_bIsWantPrintTriggerCondition)
	{
		if (m_wTriggerCount)
			printf("        트리거 조건 만족으로 %d개의 리액션을 수행한다.\n",m_wReactionCount);
		else
			printf("        트리거 조건 없음으로 %d개의 리액션을 수행한다.\n",m_wReactionCount);
	}

	g_debugSign.m_iTempStep	=	16;

	return	operateReaction(_lpAsker,_lpOwner,_lpbIsStopWork,0,_lpField,_lpiJumpKarma,_lpKC);
}

//
//	처리
BOOL
CKarma::operate(cACTOR *_lpAsker,cACTOR *_lpOwner,BOOL *_lpbIsStopWork,cFIELD *_lpField,BOOL _bIsEnterField,int *_lpiJumpKarma,CKarmaContainer *_lpKC)
{
	if	(m_bf1IsApplyAllMember && _lpAsker && _lpAsker->getPartySerial() != 0xffff && _lpAsker->getParty())
	{
		cParty	party;

		if	(g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))
		{
			int	iCompleteCount	=	0;

			for (int i=0;i<party.m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

				if	(!lpMember)
					continue;

				if	(!_lpAsker->isOnSameField(lpMember))
					continue;

				iCompleteCount	+=	_operate(lpMember,_lpOwner,_lpbIsStopWork,_lpField,_bIsEnterField,_lpiJumpKarma,_lpKC);
			}

			if	(iCompleteCount)
				return	TRUE;

			return	FALSE;
		}
	}

	return	_operate(_lpAsker,_lpOwner,_lpbIsStopWork,_lpField,_bIsEnterField,_lpiJumpKarma,_lpKC);
}

BOOL
CKarma::changeWeather(CKarmaItem*	_lpReaction, cFIELD* _lpField)
{		// 마지막 수정일 : 09.10.07

	if(!_lpField )
		return FALSE;
	int iSecond = _lpReaction->m_aValue[0];
	BYTE iWeather = _lpReaction->m_aValue[1];
	
	if(_lpField->m_iWeatherSecond)
		return TRUE;
	
	_lpField->m_wWeather = iWeather;
	_lpField->m_iWeatherSecond = iSecond;
	
	for (int i=_lpField->m_aiActorCount[0];i;i--)
	{
		cACTOR	*lpActor	=	_lpField->getTestedActor(_lpField->m_awActorList[0][i-1]);
					
		if	(!lpActor)
			continue;
					
		lpActor->sendEtcWork(eEW_CHANGE_WEATHER, _lpField->m_wWeather);
	}
				

	return TRUE;
}

BOOL
CKarma::operateReactionItem(CKarmaItem	*_lpReaction,int *_lpiReactionIndex,cACTOR *_lpAsker,cACTOR *_lpOwner,BOOL *_lpbIsStopWork,int _iPauseReaction,cFIELD *_lpField,int *_lpiJumpKarma,CKarmaContainer *_lpKC)
{
	BOOL	bIsOK	=	TRUE;

	switch(_lpReaction->m_wItem)
	{
		case	eREACTION_JOIN_GUILD_SECRET_DUNGEONS_WITH_PARTY					:		// 파티원이 있는 길드비전입장.
			bIsOK			=	_lpAsker->retryToGuildDungeon();
			break;

		case	eREACTION_DUEL_INTERFACE				://	결투장 인터페이스
		{
			bIsOK			=	operateDuelWork(_lpReaction,_lpAsker);
			*_lpbIsStopWork	=	TRUE;

			if	(bIsOK	==	FALSE	&&	g_bIsWantPrintTriggerCondition)
				printf("결투장 인터페이스 처리 실패\n");
			break;
		}

//	대화 관련
		case	eREACTION_SELECT_DIALOG					://	" 대화 [speech]0으로 이동한다."
			bIsOK			=	selectSpeech(_lpReaction,_lpAsker,_lpOwner);
			*_lpbIsStopWork	=	TRUE;

			if	(bIsOK	==	FALSE	&&	g_bIsWantPrintTriggerCondition)
				printf("대화선택 실패\n");
			break;

		case	eREACTION_END_DIALOG					://	" 대화를 종료한다."
			bIsOK			=	endDialog(_lpAsker,_lpOwner);
			*_lpbIsStopWork	=	TRUE;
			if	(bIsOK	==	FALSE	&&	g_bIsWantPrintTriggerCondition)
				printf("대화를 종료 실패\n");
			break;

		case	eREACTION_SHOP							://	" 상점 [shop]0의 거래창을 연다."
			bIsOK			=	openShop(_lpReaction,_lpAsker,_lpOwner);
			break;

		case	eREACTION_OPEN_BANK						:	//	은행 열기
			bIsOK			=	openBank(_lpAsker);
			break;
		case	eREACTION_UPGRADE_SHOP							://	강화창을 연다.	09.08.25 
			bIsOK			=	openUpgradeShop(_lpAsker);	
			break;
		case	eREACTION_OPEN_RELAY_STATION	:
			bIsOK			=	openRelayStation(_lpAsker);
			break;

		case	eREACTION_FEED_GUILD_PET_AND_GUARDIAN	:
			bIsOK			=	feedGuildPetAndGuardian(_lpAsker);
			break;
		case	eREACTION_SETTING_ROEN_GOODWILL	:
			bIsOK			=	setGuildHallRoenGoolWill(_lpReaction , _lpAsker);
			break;

		case	eREACTION_OPEN_GUILD_SHOP					:
			bIsOK			=	_lpAsker->openGuildShop();
			break;

		case	eREACTION_SET_GUILD_HALL_LEVEL			://	1 레벨 길드 홀 부여
			bIsOK			=	setGuildHallLevel(_lpAsker,_lpReaction);
			break;

		case	eREACTION_OPEN_RELATED_PLACE_LIST_OF_GUILD	:
			bIsOK			=	_lpAsker->openRelatedPlaceListOfGuild();
			break;
			
		case	eREACTION_OPEN_GUILD_HONOR_POINT_WINDOW		:
			bIsOK			=	_lpAsker->openGuildHonorPointWindow(_lpReaction->m_aValue[0]);
			break;

		case	eREACTION_MOVE_TO_GUILD_HALL			:	//	길드 홀로 이동
			bIsOK			=	moveToGuildHall(_lpAsker,_lpReaction);
			break;

		case	eREACTION_MOVE_TO						:
			bIsOK			=	moveTo(_lpField,_lpAsker,_lpOwner,_lpReaction);
			break;

		case	eREACTION_MAKE_GUILD					:	//	길드 생성
			bIsOK			=	readyToMakeGuild(_lpAsker);
			break;

		case	eREACTION_OPEN_GUILD_BATTLE_SCHEDULER	:	//	길드전 스케쥴
		{
			bIsOK			=	openGuildBattleScheduler(_lpAsker,_lpReaction);
			break;
		}

		case	eREACTION_OPEN_GUILD_MARK_COMPOSER		:	//	길드 마크 조합
			bIsOK			=	openGuildMarkComposer(_lpAsker);
			break;

		case	eREACTION_INCREASE_GUILD_STATUE_SKILL_LEVEL	://	길드 석상 스킬 레벨 증가
			bIsOK			=	increaseGuildStatueSkillLevel(_lpAsker,_lpReaction);
			break;
		case	eREACTION_OPEN_GUILD_INVENTORY				://	길드 아아템 창 열기
			bIsOK			=	openGuildInventory(_lpAsker);
			break;

		case	eREACTION_ADD_GUILD_PLACE_OF_RELATED		:
			bIsOK			=	addRelatedPlaceOfGuild(_lpAsker,_lpReaction);
			break;
			
		case	eREACTION_TRY_GUILD_POINT_BATTLE	:
			bIsOK			=	tryGuildPointBattle(_lpAsker);
			break;

		case	eREACTION_MOVE_TO_SECRET_DUNGEON	:
			bIsOK			=	trySecretDungeon(_lpAsker , _lpReaction);
			break;

		case	eREACTION_ENTER_GUILD_DUNGEN	:
			bIsOK			=	tryGuildDungen(_lpAsker , _lpReaction);
			break;

		case	eREACTION_GIVE_GUILD_HONOR_POINT		:
			bIsOK			=	giveGuildHonorPoint(_lpAsker,_lpReaction);
			break;

		case	eREACTION_END_GUILD_POINT_BATTLE_AND_RECORD_GUILD_HONOR_POINT		:
			bIsOK			=	endGuildPointBattleAndRecordGuildHonorPoint(_lpAsker,_lpReaction);
			break;

		case	eREACTION_BEGIN_SIEGE_WARFARE_OF_GUILD	:
			bIsOK			=	beginSiegeWarfareOfGuild(_lpAsker);
			break;

		case	eREACTION_BREAK_KARMA					:
			*_lpbIsStopWork	=	TRUE;
			break;

		case	eREACTION_JUMP_KARMA					:
			if	(_lpiJumpKarma)
				*_lpiJumpKarma	=	_lpReaction->m_aValue[0];
			return	TRUE;

		case	eREACTION_LAUNCH_RANDOM_KARMA			:
			bIsOK			=	launchRandomKarma(_lpReaction,_lpAsker,_lpOwner,_lpbIsStopWork,_iPauseReaction,_lpField,_lpiJumpKarma,_lpKC);
			
			if	(_lpiJumpKarma && *_lpiJumpKarma != -1)
				return	TRUE;
			break;
			
		case	eREACTION_LAUNCH_RANDOM_REACTION			:
		{
			int		iJumpReaction	=	-1;

			bIsOK	=	launchRandomReaction(_lpReaction,_lpAsker,_lpOwner,_lpbIsStopWork,_iPauseReaction,_lpField,_lpiJumpKarma,&iJumpReaction,_lpKC);

			if	(iJumpReaction	!= -1)
				*_lpiReactionIndex	=	iJumpReaction;
			break;
		}

		case	eREACTION_CALL_MAGIC_CARPET				:	//	
			_lpAsker->callMagicCarpet(-1 , TRUE);
			break;

		case	eREACTION_OPEN_REPAIR_ITEM_WINDOW		:	//	아이템 수리창 열기
			_lpAsker->openRepairItemWindow(_lpOwner);
			break;

		case	eREACTION_OPEN_DISJOINTING_WINDOW		:	//	아이템 분해창 열기
			_lpAsker->openDisjointingItemWindow(_lpOwner);
			break;
			
		case	eREACTION_OPEN_JUDGE_OF_ITEM_WINDOW		:	//	아이템 감별창 오픈
			_lpAsker->sendEtcWork(eEW_OPEN_JUDGE_OF_ITEM_WINDOW,_lpOwner->m_wSerialInField);
			break;
		case	eREACTION_OPEN_MERGE_ITEM_PIECE_WINDOW	:	//	아이템 조각 합체창 오픈
			_lpAsker->sendEtcWork(eEW_OPEN_MERGE_ITEM_PIECE_WINDOW,_lpOwner->m_wSerialInField);
			break;


//	이벤트 관련
		case	eREACTION_SET_QUEST_START				://	퀘스트 시작 상태로 만든다.
			bIsOK			=	setQuestStart(_lpReaction,_lpAsker,eQT_NORMAL);
			break;
		case	eREACTION_SET_QUEST_COMPLETE			://	퀘스트를 완료한다.
			bIsOK			=	setQuestComplete(_lpReaction,_lpAsker,eQT_NORMAL);
			break;
		case	eREACTION_CHECK_QUEST_COMPLETE_CHECKER		:
			bIsOK			=	setQuestCompleteChecker(_lpReaction,_lpAsker,eQT_NORMAL);
			break;
		case	eREACTION_SET_QUEST_PROCESS_LEVEL		://	퀘스트 상태를 설정한다.
			bIsOK			=	setQuestProcessLevel(_lpReaction,_lpAsker,eQT_NORMAL);
			break;
		case	eREACTION_SET_QUEST_VALUE				://퀘스트 관련 수치 설정"		
			bIsOK			=	setQuestValue(_lpReaction,_lpAsker,eQT_NORMAL);
			break;
		case	eREACTION_CONTROL_QUEST_VALUE			://퀘스트 수치 증가/감소"		
			bIsOK			=	controlQuestValue(_lpReaction,_lpAsker,eQT_NORMAL);
			break;
		case	eREACTION_CANCEL_QUEST					://퀘스트 취소
			bIsOK			=	cancelQuest(_lpReaction,_lpAsker,eQT_NORMAL);
			break;
		case	eREACTION_SET_ARBEIT_COMPLETE			://	퀘스트를 완료한다.
			bIsOK			=	setQuestComplete(_lpReaction,_lpAsker,eQT_ARBEIT);
			break;
		case	eREACTION_CHECK_ARBEIT_COMPLETE_CHECKER		://	아르바이트를 완료 상태로 체크한다.
			bIsOK			=	setQuestCompleteChecker(_lpReaction,_lpAsker,eQT_ARBEIT);
			break;

		case	eREACTION_SET_ARBEIT_START				://아르바이트 시작"				
			bIsOK			=	setQuestStart(_lpReaction,_lpAsker,eQT_ARBEIT);
			break;
		case	eREACTION_SET_ARBEIT_PROCESS_LEVEL		://아르바이트 상태 변경"		
			bIsOK			=	setQuestProcessLevel(_lpReaction,_lpAsker,eQT_ARBEIT);
			break;
		case	eREACTION_SET_ARBEIT_VALUE				://아르바이트 관련 수치 설정"	
			bIsOK			=	setQuestValue(_lpReaction,_lpAsker,eQT_ARBEIT);
			break;
		case	eREACTION_CONTROL_ARBEIT_VALUE				://아르바이트 수치 증가/감소"	
			bIsOK			=	controlQuestValue(_lpReaction,_lpAsker,eQT_ARBEIT);
			break;
		case	eREACTION_CANCEL_ARBEIT					://아르바이트 취소
			bIsOK			=	cancelQuest(_lpReaction,_lpAsker,eQT_ARBEIT);
			break;

		case	eREACTION_SET_QUEST_COUNTDOWN_TIMER		:
			bIsOK			=	setQuestCountdownTimer(_lpReaction,_lpAsker,eQT_NORMAL);
			break;

		case	eREACTION_SET_ARBEIT_COUNTDOWN_TIMER	:
			bIsOK			=	setQuestCountdownTimer(_lpReaction,_lpAsker,eQT_ARBEIT);
			break;

//			case	eREACTION_CREATE_CHARACTER				://	캐릭터 생성
//				break;

		case	eREACTION_REMOVE_CHARACTER				://	캐릭터 제거
			bIsOK			=	removeCharacter(_lpReaction,_lpAsker);
			break;

		case	eREACTION_MOVE_FIELD					://	필드 이동
			bIsOK			=	moveField(_lpReaction,_lpAsker);
			break;

		case	eREACTION_CHANGE_BATTLE_INCLINATION		://	전투 성향 설정
			bIsOK			=	changeBattleInclination(_lpReaction,_lpField);
			break;

		case	eREACTION_SYSTEM_NOTICE					:
			bIsOK			=	eventNotice(_lpReaction,_lpAsker,_lpField);
			break;

//	아이템 관련
		case	eREACTION_ADD_GOLD						:
			bIsOK			=	addGold(_lpReaction,_lpAsker);
			break;

		case	eREACTION_ADD_ITEM						:
		case	eREACTION_ADD_SPECIAL_ITEM				:
		case	eREACTION_ADD_QUEST_ITEM				:
			bIsOK			=	addItem(_lpReaction,_lpAsker);
			break;

		case	eREACTION_BUY_GOLD_BAR					:			// 09.08.19
			bIsOK			=	buyGoldBar(_lpReaction,_lpAsker);
			break;

		case	eREACTION_ADD_ITEM_PIECE				:
			bIsOK			=	addItemPiece(_lpReaction,_lpOwner,_lpAsker);
			break;


		case	eREACTION_DROP_GOLD						:
			bIsOK			=	dropGold(_lpReaction,_lpOwner,_lpAsker);
			break;

		case	eREACTION_ADD_SET_ELEMENT_ITEM						:
			bIsOK			=	dropSetItem(_lpReaction,_lpOwner,_lpAsker);
			break;

		case	eREACTION_DROP_ITEM						://	아이템 떨구기
		case	eREACTION_DROP_SPECIAL_ITEM				://	특수 아이템 떨구기
		case	eREACTION_DROP_QUEST_ITEM				://	퀘스트 아이템 떨구기
			bIsOK			=	dropItem(_lpReaction,_lpOwner,_lpAsker);
			break;

		case	eREACTION_MOVE_AREA						://	영역 이동
			bIsOK			=	moveArea(_lpReaction,_lpAsker);
			break;

		case	eREACTION_SELECT_OX_RESULT				://	OX 퀴즈 결과 선택
			bIsOK			=	selectOXQuizResult(_lpReaction,_lpField);
			break;

		case	eREACTION_ADD_SPECIFIC_ITEM				://	특정 아이템 추가
			bIsOK			=	addSpecificItem(_lpReaction,_lpAsker);
			break;
		case	eREACTION_DROP_SPECIFIC_ITEM			://	특정 아이템 떨어뜨리기
			bIsOK			=	dropSpecificItem(_lpReaction,_lpOwner,_lpAsker);
			break;
//	캐릭터 관련
		case	eREACTION_SHOUT							:
			bIsOK			=	shout(_lpReaction,_lpField);
			break;

		case	eREACTION_TALK							:	//	대화
			bIsOK			=	talk(_lpReaction,_lpOwner);
			break;

		case	eREACTION_TALK2							:	//	대화
			bIsOK			=	talk(_lpReaction,_lpField,_lpAsker);
			break;

		case	eREACTION_RECOVER_HP					:	//	체력 설정
			bIsOK			=	healHP(_lpReaction,_lpAsker);
			break;

		case	eREACTION_CARGE_CP						:	//	CP 채워주기
			bIsOK			=	chargeCP(_lpReaction,_lpAsker);
			break;

		case	eREACTION_DAMAGE						:	//	체력 설정
			bIsOK			=	damageToCharacter(_lpReaction,_lpAsker,_lpOwner);
			break;

		case	eREACTION_GET_EXPERIENCE				://경험치 얻기
			bIsOK			=	getExperience(_lpReaction,_lpAsker);
			break;
			
		case	eREACTION_GET_EXPERIENCE2				://경험치 얻기
			bIsOK			=	getExperience2(_lpReaction,_lpAsker);
			break;

		case	eREACTION_RECOVER_DEATH_PENELTY			:
			{
				BOOL bNotNeedCost = _lpReaction->m_aValue[0];
				
				if(bNotNeedCost)	
					bIsOK			=	_lpAsker->clearDeathPenelty();
				else
					bIsOK			=	_lpAsker->sendQuestionForRecoverDeathPenelty();

				break;
			}

		case	eREACTION_GET_TITLE						:
			bIsOK			=	getTitle(_lpReaction,_lpAsker);
			break;
			
		case	eREACTION_REDISTRIBUTE_SKILL			:	//	스킬 재분배
			_lpAsker->resetSkillPoint();
			_lpAsker->sendRedistribute(eREDISTRIBUTE_SKILL);
			break;

		case	eREACTION_REDISTRIBUTE_STATE			:	//	스탯 재분배
			_lpAsker->resetStatePoint();
			_lpAsker->sendRedistribute(eREDISTRIBUTE_STATE);
			break;

		case	eREACTION_RESET_PET_NAME				:
			bIsOK			=	resetPetName(_lpReaction,_lpAsker);
			break;

		case	eREACTION_ADD_PET				:
			bIsOK			=	addPet(_lpReaction,_lpAsker);
			break;
			

		case	eREACTION_RESET_ACTOR_IN_DUEL_SERVER	:
			bIsOK			=	TRUE;
			_lpAsker->resetAllForDuel();
			break;

		case	eREACTION_DELAY							:
			bIsOK			=	delay(_lpReaction,*_lpiReactionIndex,_lpOwner,_lpAsker,_lpField);
			g_debugSign.m_iTempStep	=	20;
			return	TRUE;

		case	eREACTION_SET_COUNTDOWN_TIMER			:
			bIsOK			=	setSecretDungeonCountdownTimer(_lpReaction,_lpField);
			break;

		case	eREACTION_USE_ABILITY					:				// 09.08.14		보유스킬사용
			bIsOK			=	useHaveSkill(_lpReaction,_lpOwner);
			break;
		case	eREACTION_USE_MONSTER_ABILITY			:			// 09.08.14			스킬사용

			bIsOK			=	useMonsterSkill(_lpReaction, _lpOwner);
			break;
		case	eREACTION_CHANGE_SIGHT_RANGE			:
			bIsOK			=	changeSightRange(_lpReaction,_lpField);
			break;

		case	eREACTION_STATE_CHANGE					:
			bIsOK			=	changeState(_lpReaction,_lpAsker , _lpOwner);
			break;
		case	eREACTION_STATE_BLIND_EFFECT			:
			bIsOK			=	giveBlindEffect(_lpReaction,_lpAsker);
			break;
			
		case	eREACTION_REBIRTH					:
			bIsOK			=	_lpAsker->rebirth(FALSE,_lpReaction->m_aValue[0]);
			break;
		case	eREACTION_DISPELL					:
			bIsOK			=	dispellAllBuff(_lpReaction,_lpAsker);
			break;

		case	eREACTION_RELEASE_TRANSFORMATION						:
			bIsOK			=	releaseTransformationMonster(_lpReaction,_lpAsker);
			break;
		case	eREACTION_TRANS_JOB						:
			bIsOK			=	changeJob(_lpReaction,_lpOwner);
			break;

		case	eREACTION_SET_GUIDE						:
			bIsOK			=	setGuide(_lpReaction,_lpAsker,_lpOwner);
			break;

		case	eREACTION_TRACE_GUIDE					:
			bIsOK			=	setTraceGuide(_lpReaction,_lpOwner);
			break;

		case	eREACTION_REGEN_MONSTER					:
			bIsOK			=	regenMonster(_lpOwner,_lpReaction,_lpField);
			break;

		case	eREACTION_CHANGE_TARGET_BY_RANDOM		:
			bIsOK			=	changeTargetByRandom(_lpReaction,_lpOwner);
			break;

		case	eREACTION_TARGET_RESET		:
			bIsOK			=	removeTargeting(_lpReaction,_lpOwner);
			break;

		case	eREACTION_RELEASE_WRONG_STATUS			:
			bIsOK			=	releaseWrongStatus(_lpReaction,_lpField);
			break;

		case	eREACTION_CONTROL_INVINCIBLE_STATUS		:
			bIsOK			=	setInvincibleStatus(_lpReaction,_lpField);
			break;

		case	eREACTION_CONTROL_DUNGEON_FLAG			:
			bIsOK			=	controlDungeonValue(_lpReaction,_lpField);
			break;
		case	eREACTION_CONTROL_FIELD_VALUE	:
			bIsOK			=	controlFieldValue(_lpReaction,_lpField);
			break;
		case	eREACTION_CONTROL_WORLD_VALUE	:
			bIsOK			=	controlWorldValue(_lpReaction);
			break;

		case	eREACTION_SET_ARCA_DOOR_TRAP_STATUS		:
			bIsOK			=	setArcaDoorTrapStatus(_lpReaction,_lpField);
			break;

		case	eREACTION_SET_DOOR_ENABLE_DISABLE		:
			bIsOK			=	setDoorEnableDisable(_lpReaction,_lpField);
			break;

			

		case	eREACTION_BEGIN_EVENT					:
			bIsOK			=	beginEvent(_lpReaction,_lpField);
			break;

		case	eREACTION_OCCUR_SKILL_EFFECT			:
			bIsOK			=	occurSkillEffect(_lpReaction,_lpField);
			break;

		case	eREACTION_MOVE_FOCUS					:
			bIsOK			=	setGroundFocus(_lpReaction,_lpField);
			break;

		case	eREACTION_FOCUS_ON_CHARACTER			:
			bIsOK			=	setCharacterFocus(_lpReaction,_lpField);
			break;

		case	eREACTION_SHAKE_SCREEN					:
			bIsOK			=	shakeScreen(_lpReaction,_lpField);
			break;

		case	eREACTION_PLAY_SOUND					:
			bIsOK			=	playSound(_lpReaction,_lpField);
			break;

		case	eREACTION_STRESS_MINIMAP				:
			bIsOK			=	stressMinimap(_lpReaction,_lpField);
			break;

		case	eREACTION_NPC_ACTION					:
			bIsOK			=	npcAction(_lpReaction,_lpAsker,_lpField);
			break;
			
		case	eREACTION_COMPLETE_SECRET_DUNGEON		:
			bIsOK			=	completeSecretDungeon(_lpReaction,_lpField);
			break;


		case	eREACTION_PLAY_PATTERN					:
			bIsOK			=	playPattern(_lpReaction,_lpOwner);
			break;
			
		case	eREACTION_PLAY_BOSS_EVENT				:
			bIsOK			=	playBossEvent(_lpReaction,_lpField);
			break;
		case	eREACTION_SCREEN_EFFECT					:
			bIsOK			=	giveScreenEvent(_lpReaction,_lpAsker,_lpField);
			break;
		case	eREACTION_FIX_FIELD_RESISTANCE			:
			bIsOK			=	fixFieldResistance(_lpReaction,_lpField);
			break;
			
		case	eREACTION_SET_SCREEN_EVENT				:
			bIsOK			=	playScreenEvent(_lpReaction,_lpAsker);
			break;

		case	eREACTION_EXTEND_SECRET_DUNGEON_UPKEEP_TIME			:
			bIsOK			=	extendSecretDungeonUpkeepTime(_lpReaction,_lpField);
			break;


		case	eREACTION_SET_CHAPTER								:	//	챕터 x 시작/종료
			bIsOK			=	setChapter(_lpReaction,_lpAsker);
			break;

		case	eREACTION_SET_MAIN_QUEST_VALUE						:	//	메인 퀘스트 수치 x 의 값을 y [로 설정한다/증가 시킨다/감소 시킨다.]
			bIsOK			=	setMainQuestValue(_lpReaction,_lpAsker);
			break;
		case	eREACTION_SET_MAIN_QUEST_SWITCH						:	//	메인 퀘스트 스위치 x 번을 [켠다/끈다]
			bIsOK			=	setMainQuestSwitch(_lpReaction,_lpAsker);
			break;

		case	eREACTION_RESET_MAIN_QUEST_VALUE					:	//	메인 퀘스트 수치를 모두 초기화 시킨다.(0이된다.)
			bIsOK			=	resetMainQuestValue(_lpAsker);
			break;

		case	eREACTION_RESET_MAIN_SWITCH							:	//	메인 퀘스트 스위치를 모두 초기화 시킨다.(모두off가 된다.)
			bIsOK			=	resetMainQuestSwitch(_lpAsker);
			break;
		case	eREACTION_GET_GRACE:
			bIsOK			=	addGGGtoTarget(_lpReaction , _lpAsker);
			break;

		case	eREACTION_DISPLAY_MAIN_QUEST_EPILOG					:
			bIsOK			=	displayMainQuestEpilog(_lpReaction,_lpAsker);
			break;

		case	eREACTION_CREATE_BOOK								://	책 아이템 생성
			bIsOK			=	createBook(_lpReaction,_lpAsker);
			break;
		case	eREACTION_WRITE_PAGE								://	책 아이템 생성
			bIsOK			=	writeBookPage(_lpReaction,_lpAsker);
			break;

		case	eREACTION_EXTEND_ITEM_VALIDATE_TIME_TO_NEXT			://	아이템 유효기간 연장
			bIsOK			=	extendItemValidateTimeToNextDay(_lpReaction,_lpAsker);
			break;

		case	eREACTION_EXPLOSION_EVENT_BALLOON					:
			bIsOK			=	explosionEventBalloon(_lpReaction,_lpAsker);
			break;

		case	eREACTION_CREATE_ADD_CATEGORY_TO_BOOK_ITEM			://	책 아이템에 카테고리 추가 아이템 생성
			bIsOK			=	createAddCategoryToBookItem(_lpReaction,_lpAsker);
			break;
		case	eREACTION_CREATE_ADD_PAGE_TO_BOOK_ITEM				://	책 아이템에 페이지 추가 아이템 생성
			bIsOK			=	createAddPageToBookItem(_lpReaction,_lpAsker);
			break;
		case	eREACTION_CREATE_ADD_RANDOM_CATEGORY_TO_BOOK_ITEM	:
			bIsOK			=	createAddRandomCategoryToBookItem(_lpReaction,_lpAsker);
			break;
		case	eREACTION_CREATE_ADD_RANDOM_PAGE_TO_BOOK_ITEM		://	책에 랜덤한 페이지를 추가하는 아이템 생성
			bIsOK			=	createAddRandomPageToBookItem(_lpReaction,_lpAsker);
			break;

		case	eREACTION_CHANGE_NAME								:	//	몬스터 이름 변경
			bIsOK			=	changeName(_lpReaction,_lpOwner);
			break;
		case	eREACTION_SET_EVENT_MONSTER_VALUE					:	//	몬스터 이름 변경
			bIsOK			=	setEventValue(_lpReaction,_lpOwner);
			break;
			
		case	eREACTION_RECEIVE_HEAVEN_REWARD		:						//	천상계 보상
			bIsOK			=	heavenReward(_lpReaction,_lpAsker);
			break;
		case	eREACTION_RECEIVE_HELL_REWARD		:							//	지하계 보상
			bIsOK			=	hellReward(_lpReaction,_lpAsker);
			break;
		case	eREACTION_RECEIVE_RED_DEVIL_REWARD	:						//	붉은 악마 보상
			bIsOK			=	redDevilReward(_lpReaction,_lpAsker);
			break;

		case	eREACTION_TIME_STAMP	:
			bIsOK			=	timeStamp(_lpReaction,_lpAsker);
			break;

		case	eREACTION_EVENT_VALUE	:
			bIsOK			=	increaseEventValue(_lpReaction,_lpAsker);
			break;
			
		case	eREACTION_GIVE_RED_STONE_PIECE	:
			bIsOK			=	giveRedStonePiece(_lpReaction,_lpAsker,_lpOwner);
			break;

		case	eREACTION_CASTING_MAGIC		:
			bIsOK			=	useSkillByCompulsion(_lpReaction,_lpOwner,_lpAsker);
			break;
		case	eREACTION_FIELD_WEATHER					:
			bIsOK			=	changeWeather( _lpReaction,_lpField);	// 09.10.07  기후변화		
			break;
		case	eREACTION_ADD_SET_ITEM_RANDOM					:
			bIsOK			=	addSetItemRandom(_lpReaction,_lpAsker);	// 세트아이템 랜덤지급		
			break;
		case	eREACTION_ADD_ITEM_WITH_OPTION					:		// 아이템추가(옵션)	
			bIsOK			=	addItemWithOption(_lpReaction,_lpAsker);	
			break;
		case	eREACTION_DROP_ITEM_WITH_OPTION					:		// 아이템드롭(옵션)
			bIsOK			=	dropItemWithOption(_lpReaction,_lpOwner,_lpAsker);
			break;
		case	eREACTION_UNOWNED_DROP_ITEM_WITH_OPTION					:		// 주인이 없는 아이템 드롭(옵션)
			bIsOK			=	dropItemWithOption(_lpReaction,_lpOwner,_lpAsker, TRUE);
			break;
		case	eREACTION_INCREASE_INSTANT_EXPERIENCE					:		// 일시적 경험치 증가
			bIsOK			=	setIncreaseInstantExperience(_lpReaction,_lpAsker);
			break;
		case	eREACTION_FIRE_WORK					:		// 불꽃놀이 이팩트 출력
			bIsOK			=	showFireWorkInField(_lpReaction,_lpAsker);
			break;
		case	eREACTION_DRAW_IMAGE					:		// 화면 이미지 출력
			bIsOK			=	drawImageInScreen(_lpReaction,_lpAsker);
			break;
		case	eREACTION_OFF_DRAWING_IMAGE					:		// 화면에 그린 이미지 끄기
			bIsOK			=	offDrawingImageInScreen(_lpReaction,_lpAsker);
			break;
		case	eREACTION_OPEN_INTERFACE					:		// 인터페이스 강제로 열기
			bIsOK			=	openInterface(_lpReaction,_lpAsker);
			break;
		case	eREACTION_OPEN_FAST_PORTAL_WINDOW			:		// 패스트 포탈 창 열기
			bIsOK			=	openFastPortalWindow(_lpReaction,_lpAsker);
			break;
	}

	return	bIsOK;
}

BOOL
CKarma::operateReaction(cACTOR *_lpAsker,cACTOR *_lpOwner,BOOL *_lpbIsStopWork,int _iPauseReaction,cFIELD *_lpField,int *_lpiJumpKarma,CKarmaContainer *_lpKC)
{
	BOOL	bIsSelectDialog	=	FALSE;

	g_debugSign.m_iTempStep	=	17;
	BOOL	bIsDelayReaction=	FALSE;

	for (int iReaction=_iPauseReaction;iReaction<m_wReactionCount;iReaction++)
	{
		CKarmaItem	*lpReaction	=	&m_pReaction[iReaction];
		BOOL		bIsOK		=	TRUE;

		g_debugSign.m_iTempStep	=	0x1000+lpReaction->m_wItem;

		if	(!operateReactionItem(lpReaction,&iReaction,_lpAsker,_lpOwner,_lpbIsStopWork,_iPauseReaction,_lpField,_lpiJumpKarma,_lpKC))
		{
			g_debugSign.m_iTempStep	=	18;

			endDialog(_lpAsker,_lpOwner);

			return	FALSE;
		}

		if	(lpReaction->m_wItem	==	eREACTION_DELAY)
			return	TRUE;

		if	(_lpiJumpKarma && *_lpiJumpKarma	!=	-1)
			return	TRUE;
	}

	g_debugSign.m_iTempStep	=	19;

	return	TRUE;
}