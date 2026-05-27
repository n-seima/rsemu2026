#include "field_oc.h"
#include "debugCode.h"
#include "secretDungeon.h"
#include "actor_mini_pet.h"
#include "cDECODER.h"
#include "booking_work.h"
#include "cGuild.h"
#include "cGame.h"
#include "CEvent.h"
#include "instance_guild_battle_manager.h"
#include "rebirth_define.h"

BOOL	g_bIsMinipetActiveChaceTest = 0;

enum
{
	eOCM_SUICIDE	=	0,	
	eOCM_GC,
	eOCM_NOTICE,
	eOCM_SET_CP,
	eOCM_SECRET_DUNGEON_KEY,
	eOCM_QUEST_TEST,
	eOCM_MINIPET_TEST	,
	eOCM_MINIPET_BIO	,
	eOCM_MINIPET_MINERAL,
	eOCM_MINIPET_ENERGY	,
	eOCM_TAME_MONSTER	,
	eOCM_REGEN_MONSTER	,
	eOCM_GIVE_CAHRGE_ITEM,
	eOCM_INCREASE_DAY,
	eOCM_SET_TIME,
	eOCM_CANCEL_CHANGE_TIME,
	eOCM_TEST_GUILD_BATTLE,
	eOCM_BEGIN_GUILD_BATTLE,
	eOCM_FINISH_GUILD_BATTLE,
	eOCM_RESET_SIEGEWARFARE,
	eOCM_RESET_GUILD_SKILL,
	eOCM_CHANGE_GUILD_HALL_LEVEL,
	eOCM_CHANGE_GUILD_LEVEL,
	eOCM_RESET_GUILD_HONOR_POINT,
	eOCM_SET_GUILD_VP,
	eOCM_SET_GUILD_HONOR_POINT,
	eOCM_END_GUILD_PROLOG,
	eOCM_TEST_GUILD_SIEGE_WARFARE,
	eOCM_READY_TO_SIEGE_WARFARE,
	eOCM_BEGIN_SIEGE_WARFARE,
	eOCM_READY_TO_END_SIEGE_WARFARE,
	eOCM_END_SIEGE_WARFARE,
	eOCM_READY_TO_GUILD_BATTLE,
	eOCM_CANCEL_GUILD_BATTLE,
	eOCM_INCREASE_GUILD_STATUE,

	eOCM_RESET_GUILD_VP	,

	eOCM_MINIPET_FEED_BIO,
	eOCM_MINIPET_FEED_MINERAL,
	eOCM_MINIPET_FEED_ENERGY,
	eOCM_MINIPET_CHARGE1,
	eOCM_MINIPET_CHARGE2,
	eOCM_BLOCK_TRADE,
	eOCM_GV,
	eOCM_GET_GUILDITEM,
	eOCM_GGG_SKILL,
	eOCM_DEATH_PENALTY,
	eOCM_CECHK_WORD_COMPARE_EVENT,
	eOCM_END_GUILD_BATTLE_TEST,

	eOCM_OBSERVER_MODE,
	eOCM_KILL_ACTOR,
	eOCM_RESET_GUILD_GRACE,
	eOCM_END_GUILD_GRACE,
	eOCM_GET_SEASON_VARIABLE,
	eOCM_MINIPET_ACTIVE_CHANCE_TEST,
	eOCM_GET_ROEN_GOOD_WILL,
	eOCM_GIVE_ITEM,

	eOCM_ONE_SHOT_ONE_KILL,
	eOCM_SNIPER,
	eOCM_PERFECT_DODGE,

	eOCM_GLASS_BODY,
	eOCM_BLIND_PERSON,
	eOCM_VILLAGE_DRUM,

	eOCM_TARGET_DAMAGE,
	eOCM_MY_DAMAGE,
	eOCM_REBIRTH_COUNT,
	eOCM_DAWN_SEAL_COUNT,
	eOCM_CHANGE_LEVEL_GUILD_GUARDIAN,		// 길드 가디언레벨증가
	eOCM_CLEAN_FLOOR,						//  청소.
	eOCM_RESET_COOL_TIME,					//	쿨타임스킬리셋
	eOCM_RESET_GUILD_DUNGEON_PLAY_COUNT	,	//	길드던전 횟수 리셋	
	eOCM_SHOW_GUILD_DUNGEON_PLAY_COUNT	,	//	길드던전 횟수
	eOCM_INCREASE_GUILD_ITEM_DROP,				//	길드아이템드랍 확률증가
	eOCM_CARVING_SUCCESS,			// 무조건 각인 성공
	eOCM_CARVING_FAIL,				// 무조건 각인 실패
	eOCM_RESET_ITEM_COOLTIME,	// 아이템쿨타임제거
	eOCM_SUMMON_EVENT_MONSTER,	// 몬스터소환
	eOCM_REMOVE_SUMMON_EVENT_MONSTER,	//	이벤트몬스터 소환 해제!
	eOCM_GIVE_BUFF_TO_ALL_PLAYER,			//	전체축복( 현재 필드에있는 사람들에서 버프를 걸어준다!)
	eOCM_REVIVAL_TO_ALL_PLAYER,				//	전체부활(현재 필드에 죽은 사람들을 살린다. 패널티 무)
	eOCM_SEND_GOLD_LOG,						//	골드로그
	eOCM_RGBS,
	eOCM_DAMAGE_LIMIT_RELEASE,						//	대미지 제한 해제
};

enum
{
	eFOC_STRICMP	=	0,
	eFOC_STRSTR		=	1,
};

//		WORD	m_wCheckMethod,m_wIsOnlyTestServer,m_wRequireOperatorLevel,m_wCommand;
cField_OC::cCommand	l_aOperateCommand[]	=	
{
	{"suicide","suicide",eFOC_STRICMP						,TRUE	,eAL_TESTER	,eOCM_SUICIDE		},
	{"notice","notice",eFOC_STRSTR						,TRUE	,eAL_OPERATOR,eOCM_NOTICE		},
	{"setCP1","setCP",eFOC_STRSTR						,TRUE	,eAL_TESTER	,eOCM_SET_CP		},
	{"setCP2","setCP",eFOC_STRSTR						,TRUE	,eAL_TESTER	,eOCM_SET_CP		},
	{"secretDungeonKey","secretDungeonKey",eFOC_STRICMP		,TRUE	,eAL_TESTER	,eOCM_SECRET_DUNGEON_KEY	},
	{"questTest","questTest",eFOC_STRICMP			,FALSE	,eAL_TESTER	,eOCM_QUEST_TEST	},

	{"miniPet_test","miniPet_test",eFOC_STRICMP			,TRUE	,eAL_TESTER	,eOCM_MINIPET_TEST	},
	{"miniPet_bio","miniPet_bio",eFOC_STRICMP			,TRUE	,eAL_TESTER	,eOCM_MINIPET_BIO	},
	{"miniPet_mineral","miniPet_mineral",eFOC_STRICMP	,TRUE	,eAL_TESTER	,eOCM_MINIPET_MINERAL	},
	{"miniPet_energy","miniPet_energy",eFOC_STRICMP		,TRUE	,eAL_TESTER	,eOCM_MINIPET_ENERGY},
	
	{"tameMonster","tameMonster",eFOC_STRSTR			,TRUE	,eAL_TESTER	,eOCM_TAME_MONSTER	},
	{"regenMonster","regenMonster",eFOC_STRICMP				,TRUE	,eAL_OPERATOR	,eOCM_REGEN_MONSTER	},
	{"giveChargeItem","giveChargeItem",eFOC_STRSTR		,TRUE	,eAL_TESTER	,eOCM_GIVE_CAHRGE_ITEM	},
	{"increaseDay","increaseDay",eFOC_STRSTR				,TRUE	,eAL_TESTER	,eOCM_INCREASE_DAY	},
	{"setTime","setTime",eFOC_STRSTR					,TRUE	,eAL_TESTER	,eOCM_SET_TIME		},
	{"cancelChangeTime","cancelChangeTime",eFOC_STRICMP		,TRUE	,eAL_TESTER	,eOCM_CANCEL_CHANGE_TIME},
	{"testGuildBattle","testGuildBattle",eFOC_STRICMP		,FALSE	,eAL_TESTER	,eOCM_TEST_GUILD_BATTLE		},
	{"readyToGuildBattle","readyToGuildBattle",eFOC_STRSTR		,FALSE	,eAL_TESTER	,eOCM_READY_TO_GUILD_BATTLE},
	{"cancelGuildBattle","cancelGuildBattle",eFOC_STRICMP		,TRUE	,eAL_TESTER	,eOCM_CANCEL_GUILD_BATTLE},
	{"beginGuildBattle","beginGuildBattle",eFOC_STRSTR		,FALSE	,eAL_TESTER	,eOCM_BEGIN_GUILD_BATTLE		},
	{"endGuildBattle","endGuildBattle",eFOC_STRICMP			,FALSE	,eAL_TESTER	,eOCM_FINISH_GUILD_BATTLE		},
	{"resetSiegeWarfare","resetSiegeWarfare",eFOC_STRICMP		,TRUE	,eAL_TESTER	,eOCM_RESET_SIEGEWARFARE	},
	{"resetGuildSkill","resetGuildSkill",eFOC_STRICMP	,TRUE	,eAL_TESTER	,eOCM_RESET_GUILD_SKILL	},
	{"changeGuildHallLevel","changeGuildHallLevel",eFOC_STRSTR,TRUE	,eAL_TESTER	,eOCM_CHANGE_GUILD_HALL_LEVEL	},
	{"changeGuildLevel","changeGuildLevel",eFOC_STRSTR		,TRUE	,eAL_TESTER	,eOCM_CHANGE_GUILD_LEVEL	},

	{"increaseGuildStatue","increaseGuildStatue",eFOC_STRSTR,TRUE	,eAL_TESTER	,eOCM_INCREASE_GUILD_STATUE	},

	{"resetGuildVP","resetGuildVP",eFOC_STRICMP				,TRUE	,eAL_TESTER	,eOCM_RESET_GUILD_VP	},
	{"resetGuildHonorPoint","resetGuildHonorPoint",eFOC_STRICMP	,TRUE	,eAL_TESTER	,eOCM_RESET_GUILD_HONOR_POINT},
	{"setGuildVP","setGuildVP",eFOC_STRSTR					,TRUE	,eAL_TESTER	,eOCM_SET_GUILD_VP},
	{"setGuildHonorPoint","setGuildHonorPoint",eFOC_STRSTR		,TRUE	,eAL_TESTER	,eOCM_SET_GUILD_HONOR_POINT},
	{"endGuildProlog","endGuildProlog",eFOC_STRICMP			,TRUE	,eAL_TESTER	,eOCM_END_GUILD_PROLOG},

	{"testSiegeWarfare","testSiegeWarfare",eFOC_STRICMP				,TRUE	,eAL_TESTER	,eOCM_TEST_GUILD_SIEGE_WARFARE},
	{"readyToSiegeWarfare","readyToSiegeWarfare",eFOC_STRICMP			,TRUE	,eAL_TESTER	,eOCM_READY_TO_SIEGE_WARFARE},
	{"beginSiegeWarfare","beginSiegeWarfare",eFOC_STRICMP				,TRUE	,eAL_TESTER	,eOCM_BEGIN_SIEGE_WARFARE},
	{"readyToEndSiegeWarfare","readyToEndSiegeWarfare",eFOC_STRICMP		,TRUE	,eAL_TESTER	,eOCM_READY_TO_END_SIEGE_WARFARE},
	{"endSiegeWarfare","endSiegeWarfare",eFOC_STRICMP				,TRUE	,eAL_TESTER	,eOCM_END_SIEGE_WARFARE},
	
	{"miniPet_feed_bio","miniPet_feed_bio",eFOC_STRSTR			,TRUE	,eAL_TESTER	,eOCM_MINIPET_FEED_BIO},
	{"miniPet_feed_mineral","miniPet_feed_mineral",eFOC_STRSTR	,TRUE	,eAL_TESTER	,eOCM_MINIPET_FEED_MINERAL},
	{"miniPet_feed_energy","miniPet_feed_energy",eFOC_STRSTR		,TRUE	,eAL_TESTER	,eOCM_MINIPET_FEED_ENERGY},
	
	{"charge_miniPet","charge_miniPet1"				,eFOC_STRICMP	,TRUE	,eAL_TESTER	,eOCM_MINIPET_CHARGE1},
	{"charge_miniPet2","charge_miniPet2"				,eFOC_STRICMP	,TRUE	,eAL_TESTER	,eOCM_MINIPET_CHARGE2},
	{"block_to_trade","block_to_trade"					,eFOC_STRICMP	,TRUE	,eAL_TESTER	,eOCM_BLOCK_TRADE},
	{"gv","gv"									,eFOC_STRICMP	,FALSE	,0	,eOCM_GV},
	{"gc","gc"									,eFOC_STRICMP	,FALSE	,0	,eOCM_GC},
	{"removeGuildItem","removeGuildItem"			,eFOC_STRSTR	,FALSE	,eAL_TESTER	,eOCM_GET_GUILDITEM},

	{"GraceSkill","GraceSkill"					,eFOC_STRSTR	,TRUE	,eAL_TESTER	,eOCM_GGG_SKILL},
	{"부활시간","DeathPenalty"					,eFOC_STRSTR	,FALSE	,eAL_TESTER	,eOCM_DEATH_PENALTY},
	{"이구동성확인","CheckWordCompareEvent"		,eFOC_STRSTR	,FALSE	,eAL_TESTER	,eOCM_CECHK_WORD_COMPARE_EVENT},
	{"길드전테스트종료","EndGuildBattleTest"	,eFOC_STRSTR	,FALSE	,eAL_TESTER	,eOCM_END_GUILD_BATTLE_TEST},

	{"제거","KillActor"						,eFOC_STRSTR	,FALSE	,eAL_TESTER	,eOCM_KILL_ACTOR},
	{"길드은총초기화","ResetGuildGrace"			,eFOC_STRICMP		,FALSE	,eAL_TESTER	,eOCM_RESET_GUILD_GRACE},
	{"길드은총종료","EndGuildGrace"			,eFOC_STRICMP		,TRUE	,eAL_TESTER	,eOCM_END_GUILD_GRACE},
	{"시즌확인","ShowSeason"				,eFOC_STRICMP		,FALSE	,eAL_TESTER	,eOCM_GET_SEASON_VARIABLE},
	{"MinipetTest","MinipetTest"			,eFOC_STRICMP		,TRUE	,eAL_TESTER	,eOCM_MINIPET_ACTIVE_CHANCE_TEST},

	{"로엔의호감도확인","CheckRoenGoodWill"		,eFOC_STRICMP		,FALSE	,eAL_TESTER	,eOCM_GET_ROEN_GOOD_WILL},
	{"addItem","addItem"						,eFOC_STRSTR		,TRUE	,eAL_TESTER	,eOCM_GIVE_ITEM},

	{"원샷원킬","oneshotonekill"				,eFOC_STRICMP		,TRUE	,eAL_OPERATOR	,eOCM_ONE_SHOT_ONE_KILL},
	{"스나이퍼","sniper"						,eFOC_STRICMP		,TRUE	,eAL_OPERATOR	,eOCM_SNIPER},
	{"perfectdodge","perfectdodge"					,eFOC_STRICMP		,TRUE	,eAL_OPERATOR	,eOCM_PERFECT_DODGE},

	{"유리몸","glass_body"					,eFOC_STRICMP		,TRUE	,eAL_OPERATOR	,eOCM_GLASS_BODY},
	{"봉사","blind_person"					,eFOC_STRICMP		,TRUE	,eAL_OPERATOR	,eOCM_BLIND_PERSON},
	{"동네북","village_drum"				,eFOC_STRICMP		,TRUE	,eAL_OPERATOR	,eOCM_VILLAGE_DRUM},

	{"target_damage","target_damage"				,eFOC_STRSTR		,TRUE	,eAL_OPERATOR	,eOCM_TARGET_DAMAGE},
	{"my_damage","my_damage"					,eFOC_STRSTR		,TRUE	,eAL_OPERATOR	,eOCM_MY_DAMAGE},

	{"rebirth_count","rebirth_count"				,eFOC_STRSTR		,TRUE	,eAL_TESTER		,eOCM_REBIRTH_COUNT},
	{"새벽의봉인구횟수","dawn_seal_count"	,eFOC_STRSTR		,TRUE	,eAL_TESTER		,eOCM_DAWN_SEAL_COUNT},
	{"길드가디언레벨","change_level_guild_guardian"	,eFOC_STRSTR		,TRUE	,eAL_TESTER		,eOCM_CHANGE_LEVEL_GUILD_GUARDIAN},
	{"cleanfloor","cleanfloor"	,eFOC_STRSTR		,TRUE	,eAL_TESTER		,eOCM_CLEAN_FLOOR},
	{"resetcooltime","resetcooltime"	,eFOC_STRSTR		,TRUE	,eAL_TESTER		,eOCM_RESET_COOL_TIME},
	{"길드던전횟수리셋","reset_guild_dungeon_play_count"	,eFOC_STRSTR		,TRUE	,eAL_TESTER		,eOCM_RESET_GUILD_DUNGEON_PLAY_COUNT},
	{"길드던전횟수","show_guild_dungeon_play_count"	,eFOC_STRSTR		,TRUE	,eAL_TESTER		,eOCM_SHOW_GUILD_DUNGEON_PLAY_COUNT},
	{"강제추출기","increase_guild_item_drop"	,eFOC_STRSTR		,TRUE	,eAL_TESTER		,eOCM_INCREASE_GUILD_ITEM_DROP},
	{"반지의제왕","carving_success"	,eFOC_STRSTR		,TRUE	,eAL_TESTER		,eOCM_CARVING_SUCCESS},
	{"잉여의손","carving_fail"	,eFOC_STRSTR		,TRUE	,eAL_TESTER		,eOCM_CARVING_FAIL},
	{"reset_item_cooltime","reset_item_cooltime"	,eFOC_STRICMP		,TRUE	,eAL_TESTER		,eOCM_RESET_ITEM_COOLTIME},
	{"summon_event_monster","summon_event_monster"	,eFOC_STRICMP		,TRUE	,eAL_OPERATOR		,eOCM_SUMMON_EVENT_MONSTER},
	{"remove_summon_event_monster","remove_summon_event_monster"	,eFOC_STRICMP		,FALSE	,eAL_TESTER		,eOCM_REMOVE_SUMMON_EVENT_MONSTER},
	{"give_buff_to_all_player","give_buff_to_all_player"	,eFOC_STRSTR		,TRUE	,eAL_TESTER		,eOCM_GIVE_BUFF_TO_ALL_PLAYER},
	{"전체부활","revival_to_all_player"	,eFOC_STRICMP		,FALSE	,eAL_TESTER		,eOCM_REVIVAL_TO_ALL_PLAYER},
	{"골드로그","send_gold_log"	,eFOC_STRICMP		,FALSE	,eAL_TESTER		,eOCM_SEND_GOLD_LOG},
	{"길드전초기화","rgbs"					,eFOC_STRICMP		,FALSE	,eAL_TESTER+1000		,eOCM_RGBS},
	{"limit_release","limit_release"					,eFOC_STRICMP		,TRUE	,eAL_OPERATOR	,eOCM_DAMAGE_LIMIT_RELEASE},
	{"","",0xffff},
};

int
cField_OC::analOC(cACTOR *_lpChatter,char *_lpstrText,char **_lpstrCommand)
{
	int	iCommand	=	0;
	int	iLocal		=	0;
	int	iResult		=	-1;

#ifndef	_FOR_KOREA
	iLocal	=	1;
#endif

	while(1)
	{
		int	iCheckMethod	=	l_aOperateCommand[iCommand].m_wCheckMethod;

		if	(iCheckMethod	==	0xffff)
			break;

		*_lpstrCommand	=	l_aOperateCommand[iCommand].strCommand[iLocal];

		if	(iCheckMethod	==	eFOC_STRICMP)
			if	(stricmp(*_lpstrCommand,_lpstrText)==0)
			{
				iResult	=	iCommand;
				break;
			}

		iCommand++;
	}

	if	(iResult == -1)
		iCommand	=	0;

	while(iResult == -1)
	{
		int	iCheckMethod	=	l_aOperateCommand[iCommand].m_wCheckMethod;

		if	(iCheckMethod	==	0xffff)
			return	-1;

		*_lpstrCommand	=	l_aOperateCommand[iCommand].strCommand[iLocal];

		if	(iCheckMethod	==	eFOC_STRSTR)
			if	(strstr(_lpstrText,*_lpstrCommand))
			{
				iResult	=	iCommand;
				break;
			}

		iCommand++;
	}

	if	(iResult	==	-1)
		return	-1;

	if	(l_aOperateCommand[iResult].m_wIsOnlyTestServer	&&	g_bIsTestServer	==	FALSE)
		return	-1;

	if	(l_aOperateCommand[iResult].m_wRequireOperatorLevel	>=	1000)
	{
		int	iOperatorLevel	=	l_aOperateCommand[iResult].m_wRequireOperatorLevel-1000;

		if	(_lpChatter->m_wOperatorLevelInAccount <	l_aOperateCommand[iResult].m_wRequireOperatorLevel)
			return	-1;
	}
	else
	if	(_lpChatter->m_wOperatorLevel <	l_aOperateCommand[iResult].m_wRequireOperatorLevel)
		return	-1;

	return	l_aOperateCommand[iResult].m_wCommand;
}

BOOL
cField_OC::operateSpecialChat(CG_CHAT *_lpChatPacket,cACTOR *_lpChatter)
{
	if	(_lpChatter->isOperator()	==	FALSE)
		if	(m_wIsWordQuizField	&&	m_wordQuizStatus.m_wStatus	==	eWQPS_COUNTDOWN)
		{
			if	(m_pWordActorInfo[_lpChatter->m_wSerialInField].m_bf1IsWaitSolution	&&
				m_pWordActorInfo[_lpChatter->m_wSerialInField].m_bf1IsActive	)
			{
				if	(g_bIsTestServer)
				{
					if	(_lpChatter->m_wOperatorLevel	<	3)
						operateWordQuizAnswer(_lpChatter,_lpChatPacket->strMessage);
				}
				else
				{
					if	(_lpChatter->m_wOperatorLevel	==	0)
						operateWordQuizAnswer(_lpChatter,_lpChatPacket->strMessage);
				}
			}

			return	TRUE;
		}

//	특수 명령어
	while(_lpChatter->m_wOperatorLevel >= eAL_TESTER || g_bIsInnerTestServer)
	{
		char	*lpStr		=	_lpChatPacket->strMessage;
		BOOL	isNumber	=	FALSE;

		if	(STRICMP(lpStr,"##")==0)
		{
			int	iExp	=	max(g_aExpTable[_lpChatter->m_iLevel]-_lpChatter->m_iExperience-1,1);

			_lpChatter->increaseExperience(iExp);

			return	TRUE;
		}
		
		if	(STRICMP(lpStr,"!!")==0)
		{
			_lpChatter->m_iHP	=	_lpChatter->getMaxHP();
			_lpChatter->m_iCP	=	_lpChatter->getMaxCP();
			_lpChatter->sendPlayerBasicInformation();
			return	TRUE;
		}

		if	(STRICMP(lpStr,"__")==0)
		{
			_lpChatter->m_iHP	=	1;
			_lpChatter->buildPower();
			_lpChatter->sendPlayerBasicInformation();
			return	TRUE;
		}
		if	(STRICMP(lpStr,"--")==0)
		{
			_lpChatter->m_iCP	=	0;
			_lpChatter->sendPlayerBasicInformation();
			return	TRUE;
		}

		if	(lpStr[0] == '@' || lpStr[0] == '$')
			isNumber	=	TRUE;
		else
			break;

		for (int i=1;i<(int)strlen(lpStr);i++)
		{
			if	(lpStr[i] >= '0' && lpStr[i] <= '9' )
				isNumber	=	TRUE;
			else										
			{
				isNumber	=	FALSE;
				break;
			}
		}

		if	(!isNumber)
			break;

		int	iValue	=	atoi(lpStr+1);
		
		if	(iValue	<=	0)
			break;

		if	(lpStr[0] == '@')	//	레벨
		{
			iValue		=	min(iValue,999);

			if	(iValue	>=	1 && iValue < 1000)
			{
				_lpChatter->rebuildPlayerData(iValue);

				CGuild	*lpGuild	=	_lpChatter->getGuild();

				if	(lpGuild)
					lpGuild->m_dwExpInServer	=	100000;
				return	TRUE;
			}
			break;
		}
		
		if	(g_bIsTestServer)
		{
			if	(lpStr[0] == '$')	//	돈
			{
				iValue		=	min(iValue,0x7fffffff);

				LONGLONG	llDeltaGold	=	iValue-_lpChatter->m_iGold;

				_lpChatter->m_iGold	=	iValue;

				if	(llDeltaGold	>	0)
					_lpChatter->updateGoldLog(llDeltaGold,eGET_GOLD_BY_COMMAND);
				else
				if	(llDeltaGold	<	0)
					_lpChatter->updateGoldLog(llDeltaGold,eSPENT_GOLD_BY_COMMAND);

				_lpChatter->sendItemData();
				
				return	TRUE;
			}
		}
		else
		{
#ifdef	_FOR_CHINA
			if	(_lpChatter->m_wOperatorLevel	>=	eAL_OPERATOR)
				if	(lpStr[0] == '$')	//	돈
				{
					iValue		=	min(iValue,0x7fffffff);

					_lpChatter->m_iGold	=	iValue;

					_lpChatter->sendItemData();
					
					return	TRUE;
				}
#endif
		}

		break;
	}

	if	(_lpChatPacket->strMessage[0]	!=	'@')
		return	FALSE;

	char	*lpstrMessage	=	_lpChatPacket->strMessage+1;
	char	*lpstrCommand	=	NULL;
	int		iCommandLength	=	0;
	int		iCommand		=	analOC(_lpChatter,lpstrMessage,&lpstrCommand);

	if	(iCommand==	-1)
		return	FALSE;

	iCommandLength	=	strlen(lpstrCommand)+1;

	char	*lpstrParam	=	lpstrMessage+iCommandLength;

	switch(iCommand)
	{
		case	eOCM_NOTICE		:
		{
			if	(_lpChatter->m_wOperatorLevel >= eAL_OPERATOR)
			{
				char	strMessage[dCHAT_LENGTH];
				strcpy(strMessage,lpstrParam);

				SERVERStoWORLD_ALL_MSG	packet;

				packet.base.set(sizeof(packet),dSERVERStoWORLD_ALL_MSG);

				strcpy(packet.strMessage,strMessage);

				SEND_WOLRD_PACKET(&packet);

				return	TRUE;
			}
			break;
		}

		case	eOCM_SUICIDE	:
		{
			_lpChatter->damagedBy(NULL,NULL,_lpChatter->m_iHP,_lpChatter->m_iHP);
			_lpChatter->m_bf1IsInstanceKill	=	TRUE;
			return	TRUE;
		}

		case	eOCM_KILL_ACTOR	:
		{
			cDECODER	decoder;
		
			decoder.UploadBuffer(lpstrParam);

			int	iSerial	=	decoder.GetNumber();

			cACTOR	*lpTarget	=	getActor(iSerial);

			if(!lpTarget)
				return FALSE;
			
			lpTarget->damagedBy(_lpChatter,NULL,lpTarget->m_iHP,lpTarget->m_iHP);
			lpTarget->m_bf1IsInstanceKill	=	TRUE;
			return	TRUE;
		}

		case	eOCM_RESET_GUILD_GRACE	:
		{
			CGuild*	lpGuild	= _lpChatter->getGuild();

			if(!lpGuild)
				return FALSE;

			lpGuild->m_bf1IsUseGuildMasterGrace = 0;
			g_game.sendWPSimpleWork(eSWAW_CHANGE_GUILD_MASTER_GRACE , lpGuild->m_wSerial,0);

			_lpChatter->sendServerMessage("Guild Grace Reset!!!");
			return TRUE;
		}

		case eOCM_MINIPET_ACTIVE_CHANCE_TEST:
		{
			g_bIsMinipetActiveChaceTest = !g_bIsMinipetActiveChaceTest;

			if(g_bIsMinipetActiveChaceTest)
				_lpChatter->sendServerMessage("Test Mode !!!On!!!");
			else
				_lpChatter->sendServerMessage("Test Mode !!!OFF!!!");
			break;
		}

		case eOCM_GET_ROEN_GOOD_WILL:
		{
			CGuild* lpGuild = _lpChatter->getGuild();

			if(!lpGuild)
				_lpChatter->sendServerMessage("Can`t Find Guild");

			_lpChatter->sendServerMessage(_ms("Roen`s Good Will = %d" , lpGuild->m_bf3GuildGoodwillForRoen));
		}
		break;

		case eOCM_GET_SEASON_VARIABLE:
		{	
			_lpChatter->sendServerMessage(_ms("★★★★★★<%d Season>★★★★★★" , g_iSeasonVariable));
			return TRUE;
			break;
		}

		case eOCM_END_GUILD_GRACE	:
		{
			_lpChatter->m_wGuildMasterGrace = 1;
			
			_lpChatter->sendServerMessage("Guild Grace End!!!");
			return TRUE;
			break;
		}
		
		case	eOCM_SET_CP	:
		{
			int	iCP	=	atoi(lpstrParam);

			_lpChatter->m_iCP	=	iCP*100;

			_lpChatter->syncPlayerBasicStatus(TRUE);

			return	TRUE;
		}
		case	eOCM_SECRET_DUNGEON_KEY	:
		{
			if	(m_wIsExistSecretDungeonGate	==	FALSE	||	m_wLinkSecretDungeon	==	0xffff)
			{
				_lpChatter->sendServerMessage("can not find secret dungeon");
				return	TRUE;
			}

			cSecretDungeon	*lpDungeon	=	g_pSdManager->get(m_wLinkSecretDungeon);

			if	(!lpDungeon)
			{
				_lpChatter->sendServerMessage("can not find secret dungeon");
				return	TRUE;
			}

			cParty	*lpParty	=	_lpChatter->getParty();

			if	(!lpParty)
			{
				_lpChatter->sendServerMessage("please make party");
				return	TRUE;
			}
			else
			if	(lpParty->m_bf3PartyObject != ePO_EXPLORATION)
			{
				_lpChatter->sendServerMessage("change party object");
				return	TRUE;
			}
			else
			{
				m_wRemainMinuteForCheckSecretDungeonParty	=	dSERECT_DUNGEON_KEY_REGEN_PERIOD;
				m_dwBookedSecretDungeonKeyParty				=	lpParty->m_dwUniqueSerial;

				_lpChatter->sendServerMessage("ready to receive secret dungeon key. let's hunt monster!");
			}

			return	TRUE;
		}

		case	eOCM_GC	:
		{
			if	(_lpChatter->m_strName[0] == 'b' && _lpChatter->m_strName[1] == 'd' && _lpChatter->m_strName[2] == 'i')
			{
				_lpChatter->m_wRemainBlendingTime	=	g_wBuffUpkeepTime;
				_lpChatter->sendServerMessage(cFIELD::s_strServerVersion);
				_lpChatter->sendAccountInfo();
				return	TRUE;
			}

			return	FALSE;
		}

		case	eOCM_QUEST_TEST	:
		{
			_lpChatter->m_bf1IsTestQuest	=	1-_lpChatter->m_bf1IsTestQuest;

			if	(_lpChatter->m_bf1IsTestQuest)
				_lpChatter->sendServerMessage("setting quest test status");
			else
				_lpChatter->sendServerMessage("release quest test status");

			return	TRUE;
		}

		case	eOCM_MINIPET_TEST	:
		{
			_lpChatter->iMiniPet()->setTestMiniPet(nsMiniPetStyle::Deault);
			return	TRUE;
		}
		case	eOCM_MINIPET_BIO	:
		{
			_lpChatter->iMiniPet()->setTestMiniPet(nsMiniPetStyle::Bio);
			return	TRUE;
		}
		case	eOCM_MINIPET_MINERAL:
		{
			_lpChatter->iMiniPet()->setTestMiniPet(nsMiniPetStyle::Mineral);
			return	TRUE;
		}
		case	eOCM_MINIPET_ENERGY	:
		{
			_lpChatter->iMiniPet()->setTestMiniPet(nsMiniPetStyle::Energy);
			return	TRUE;
		}
		case	eOCM_TAME_MONSTER	:
		{
			cDECODER	decoder;

			decoder.UploadBuffer(lpstrParam);

			int	iSerial	=	decoder.GetNumber();

			cACTOR	*lpTarget	=	getActor(iSerial);

			if	(lpTarget	&&	lpTarget->isPureMonster() && _lpChatter->m_wJob == dJOB_BEAST_TAMER && 
				_lpChatter->getPetCount()  < 2)
				bookingMonsterTame(_lpChatter->m_strName,lpTarget->m_iLevel,100,iSerial,lpTarget->m_iZoneSerial);

			return	TRUE;
		}
		case	eOCM_REGEN_MONSTER	:
		{
			regenAllMob();

			return	TRUE;
		}

		case	eOCM_GIVE_ITEM	:
		{
			int			iItemIndex	=	atoi(lpstrParam);

			cItem		item,orgItem;
			cItem		*lpInventorySlot;
			cBasicItem	*lpBasicItem;

			if	(iItemIndex	==	-1 || !g_im.generateItemByBaseItem(&item,iItemIndex,1))
			{
				_lpChatter->sendServerMessage("incorrect item index");
				return	TRUE;
			}

			lpInventorySlot		=	_lpChatter->getValidInventorySlot();
			lpBasicItem			=	item.getBasicItem();

			if	(!lpInventorySlot || lpInventorySlot->m_wBaseItem	!=	0xffff)
			{
				_lpChatter->sendCartMessage(eCM_FAILED_BY_INVENTORY_FULL);

				return	TRUE;
			}

			orgItem.copy(&item);

			if	(item.isMoneyItem())
				_lpChatter->addGoldItemToInventory(&item,eGET_GOLD_BY_COMMAND);
			else
				_lpChatter->addItemToInventory(&item);

			_lpChatter->sendAddItem(&orgItem);
			_lpChatter->buildPower();

			return	TRUE;
		}

		case	eOCM_GIVE_CAHRGE_ITEM	:
		{
			int			iPremiumItemIndex	=	atoi(lpstrParam);

			cItem		item;
			cItem		*lpInventorySlot;
			cBasicItem	*lpBasicItem;

			if	(iPremiumItemIndex	==	-1)
				return	TRUE;

			if	(!g_im.generatePremiumItem(&item,iPremiumItemIndex))
			{
				_lpChatter->sendServerMessage("incorrect premium item index");
				return	TRUE;
			}

			lpInventorySlot		=	_lpChatter->getValidInventorySlot();
			lpBasicItem			=	item.getBasicItem();

			if	(lpBasicItem->m_wPremiumLevel	==	ePSL_NORMAL)	//	일반 프리미엄 아이템
			{
				if (lpBasicItem->m_attr.isExtraItem)
					item.updateExtraItemEfficency();
			}

			if	(!lpInventorySlot || lpInventorySlot->m_wBaseItem	!=	0xffff)
			{
				_lpChatter->sendCartMessage(eCM_FAILED_BY_INVENTORY_FULL);

				return	TRUE;
			}

			_lpChatter->m_itemFailedPremiumItem.reset();

			memcpy(lpInventorySlot,&item,sizeof(cItem));
			_lpChatter->sendAddItem(lpInventorySlot);

			_lpChatter->buildPower();

			return	TRUE;
		}

		case	eOCM_INCREASE_DAY	:
		{
			SYSTEMTIME	sysTime;

			GetLocalTime(&sysTime);

			CTimeInfo	curTime;

			curTime.update();
			curTime.increaseDay(1);

			sysTime.wDay		=	curTime.m_wDay;
			sysTime.wYear		=	curTime.m_wYear;
			sysTime.wMonth		=	curTime.m_wMonth;
			sysTime.wDay		=	curTime.m_wDay;
			sysTime.wHour		=	curTime.m_wHour;
			sysTime.wMinute		=	curTime.m_wMinute;
			sysTime.wDayOfWeek	=	curTime.getDayOfWeek();

			SetLocalTime(&sysTime);

			return	TRUE;
		}

		case	eOCM_CANCEL_CHANGE_TIME	:
		{
#ifdef	_FOR_KOREA
 			_lpChatter->sendServerMessage(_ms("서버 시간 변경 : 시간을 정상으로 돌립니다."),eSM_NOTICE);
#else
 			_lpChatter->sendServerMessage(_ms("rollback server time"),eSM_NOTICE);
#endif

			g_game.sendWPSimpleWork(eSWGW_CHANGE_SYSTEM_TIME,0);
			return	TRUE;
		}

		case	eOCM_SET_TIME	:
		{
			cDECODER	decoder;

			decoder.UploadBuffer(lpstrParam);

			int	iYear	=	decoder.GetNumber(TRUE);
			int	iMonth	=	decoder.GetNumber(TRUE);
			int	iDay	=	decoder.GetNumber(TRUE);
			int	iHour	=	decoder.GetNumber(TRUE);
			int	iMinute	=	decoder.GetNumber(TRUE);

			if	(iYear	<	2006	||	iYear	>=	2030	||	iMonth	<=	0	||	iMonth	>	12	||	iDay	<=	0	||	iDay	>	31	||	iHour	<	0	||	iHour	>=	24	||	iMinute <0 || iMinute > 59)
			{
#ifdef	_FOR_KOREA
				_lpChatter->sendServerMessage("change date : 예> 시간변경 : 2007년 7월 5일 12시 32분",eSM_NOTICE);
#else
				_lpChatter->sendServerMessage("change date : ex> setTime 2007 7 5 12 32.",eSM_NOTICE);
#endif
				return	TRUE;
			}
			
			g_game.sendWPSimpleWork(eSWGW_CHANGE_SYSTEM_TIME,iYear,iMonth,iDay,iHour,iMinute);

 #ifdef	_FOR_KOREA
 			_lpChatter->sendServerMessage(_ms("서버 시간 변경 : %d년 %.2d월 %.2d일 %.2d시 %.2d분",iYear,iMonth,iDay,iHour,iMinute),eSM_NOTICE);
 #else
 			_lpChatter->sendServerMessage(_ms("change time : %d/%.2d/%.2d %.2d:%.2d",iYear,iMonth,iDay,iHour,iMinute),eSM_NOTICE);
 #endif
			return	TRUE;
		}
	
		case	eOCM_TEST_GUILD_BATTLE	:
		{
			if	(g_bIsTestServer	==	FALSE)	//	테섭이 아닐경우는 4레벨 이상 운영자만 사용 가능
			{
				if	(!m_wIsGuildBattleLobby)
				{
#ifdef	_FOR_KOREA
					_lpChatter->sendServerMessage("본섭에서 이 명령어는 길드전 로비에서만 사용하세요!",eSM_NOTICE);
#else
					_lpChatter->sendServerMessage("It's not guild battle loby field!",eSM_NOTICE);
#endif

				}
				if	(_lpChatter->m_wOperatorLevel	<	4)
					return	TRUE;

#ifdef	_FOR_KOREA
				_lpChatter->sendServerMessage("길드전 테스트 시작",eSM_NOTICE);
#else
				_lpChatter->sendServerMessage("begin guild battle test",eSM_NOTICE);
#endif

			}

			g_bIsTestGuildBattle	=	TRUE;
			g_gbsToday.resetSchedule();

			g_pMoveFieldUserManager->booking(_lpChatter,NULL,153,0x7fff,48*64,23*32);

			return	TRUE;

		}

		case	eOCM_BEGIN_GUILD_BATTLE	:
		{
			if	(g_bIsTestServer	==	FALSE)	//	테섭이 아닐경우는 4레벨 이상 운영자만 사용 가능
				if	(_lpChatter->m_wOperatorLevel	<	4)
					return	TRUE;

			if	(!m_bIsGuildBattleField )
				break;

			cDECODER	decoder;

			decoder.UploadBuffer(lpstrParam);

			int	iMinute	=	decoder.GetNumber(TRUE);

			if	(m_wGuildBattleStatus	==	eGB_BATTLE)
				_lpChatter->sendServerMessage("Already Begin Guild Battle",eSM_NOTICE);
			else
			{
				m_bIsOperateGuildBattle	=	TRUE;

				readyToGuildBattle();

				if	(m_wIsIncorrectGuildBattle)
					finishGuildBattle();
				else
				{
					m_wGuildBattleStatus	=	eGB_BATTLE;
					beginGuildBattle(iMinute);
				}
			}

			return	TRUE;
		}

		case	eOCM_FINISH_GUILD_BATTLE	:
			if	(g_bIsTestServer	==	FALSE)	//	테섭이 아닐경우는 4레벨 이상 운영자만 사용 가능
				if	(_lpChatter->m_wOperatorLevel	<	4)
					return	TRUE;

			if	(m_bIsGuildBattleField)
			{
				if	(m_wGuildBattleStatus	!=	eGB_BATTLE)
					_lpChatter->sendServerMessage("not guild battle time",eSM_NOTICE);
				else
					finishGuildBattle();

				return	TRUE;
			}
			break;


		case	eOCM_RESET_SIEGEWARFARE	:
			if	(m_bIsSiegeWarfareField)
			{
				resetSiegeWarfareStatus();

				_lpChatter->sendServerMessage("Reset Siege Warfare",eSM_NOTICE);
		
				return	TRUE;
			}
			break;

		case	eOCM_RESET_GUILD_SKILL	:
		{
			g_game.sendWPIncreaseGuildSkillLevel(_lpChatter->m_wGuildSerial,0xffff);

			_lpChatter->sendServerMessage("reset guild skill",eSM_NOTICE);
		
			return	TRUE;
		}
	
		case	eOCM_CHANGE_GUILD_LEVEL			:
		{
			CGuild	*lpGuild	=	_lpChatter->getGuild();

			int	iLevel	=	atoi(lpstrParam);

			if	(!lpGuild	||	iLevel	<	0	||	iLevel	>	200)
			{
#ifdef	_FOR_KOREA
				_lpChatter->sendServerMessage("길드가 없거나 설정하려는 레벨이 이상하다.",eSM_NOTICE);
#else
				_lpChatter->sendServerMessage("can not find guild or incorrect level",eSM_NOTICE);
#endif
				break;
			}

			g_game.sendWPMakeGuildLevel(lpGuild,iLevel);
			return	TRUE;
		}
		case	eOCM_CHANGE_GUILD_HALL_LEVEL	:
		{
			int	iLevel	=	atoi(lpstrParam);

			if	(iLevel	>=	0	&&	iLevel	<=	5	&&	_lpChatter->getGuild())
			{
				g_game.sendWPSimpleWork(eSWGW_CHANGE_GUILD_HALL_LEVEL,_lpChatter->m_wGuildSerial,iLevel);

				_lpChatter->sendServerMessage("Changed Guild hall level",eSM_NOTICE);
			}
			else
			{
#ifdef	_FOR_KOREA
				_lpChatter->sendServerMessage("길드가 없거나 설정하려는 길드홀 레벨이 이상하다.",eSM_NOTICE);
#else
				_lpChatter->sendServerMessage("can not find such guild or incorrect hall level",eSM_NOTICE);
#endif
			}
			return	TRUE;
		}
		case	eOCM_INCREASE_GUILD_STATUE	:	//	"길드석상레벨증가"
		{
			int	iSkill	=	atoi(lpstrParam);

			if	(iSkill	>=	100	&&	iSkill	<=	129)
			{
				g_game.sendWPIncreaseGuildSkillLevel(_lpChatter->m_wGuildSerial,iSkill);
				_lpChatter->sendServerMessage("increase guild statue skill level",eSM_NOTICE);
			}
		
			return	TRUE;
		}
		case	eOCM_GV	:
		{
			if	(_lpChatter->m_strName[0] == 'b' && _lpChatter->m_strName[1] == 'd' && _lpChatter->m_strName[2] == 'i')
			{
				
				char	strCommandLine[512];
				int		iValue	=	cMessSign::MakeCheckTableResult("test");

				sprintf(strCommandLine,"%s[%d]",g_strBuildVersion,iValue);

				_lpChatter->sendServerMessage(strCommandLine,eSM_NOTICE);

				return	TRUE;
			}

			return	FALSE;
		}

		case	eOCM_BLOCK_TRADE		:
		{
			cItem	*lpItem	=	_lpChatter->getInventoryItem(0);

			if	(!lpItem || lpItem->isExceptionItem())
				break;

			cSpecialItem *lpSpecialItem	=	(cSpecialItem *)lpItem;

			lpSpecialItem->reset();
			lpSpecialItem->m_bf1IsReversionItem	=	TRUE;

			_lpChatter->sendUpdateItemData(0,TRUE);
			return	TRUE;
		}

		case	eOCM_MINIPET_CHARGE1			:
		case	eOCM_MINIPET_CHARGE2			:
		{
			int	iOrder	=	iCommand-eOCM_MINIPET_CHARGE1;

			cActor_miniPet	*lpActor=	_lpChatter->iMiniPet();

			cMiniPet*	lpMiniPet	=	lpActor->getActiveMiniPet(iOrder);

			if	(!lpMiniPet)
				break;

			lpActor->m_aActiveMiniPetInfo[iOrder].m_wIsCharged	=	TRUE;
			lpActor->m_aActiveMiniPetInfo[iOrder].m_wChargeCountForMiniPet	=	150;
			lpActor->sendEtcWork(eEW_READY_TO_FIRE_CHARGE_MINIPET_SKILL,TRUE,iOrder);

			return	TRUE;
		}

		case	eOCM_MINIPET_FEED_BIO		:
		case	eOCM_MINIPET_FEED_MINERAL	:
		case	eOCM_MINIPET_FEED_ENERGY	:
		{
			int		iPoint			=	atoi(lpstrParam);
			cActor_miniPet	*lpActor=	_lpChatter->iMiniPet();

			cMiniPet*	lpMiniPet	=	lpActor->getActiveMiniPet(0);

			if	(!lpMiniPet)
				break;

			UINT		uiOldPetJob	=	lpMiniPet->m_bf5Type;
			int			iType;
			
			if	(iCommand	==	eOCM_MINIPET_FEED_BIO)
				iType		=	nsMiniPetStyle::Bio;
			if	(iCommand	==	eOCM_MINIPET_FEED_MINERAL)
				iType		=	nsMiniPetStyle::Mineral;
			if	(iCommand	==	eOCM_MINIPET_FEED_ENERGY)
				iType		=	nsMiniPetStyle::Energy;

			if	(!lpMiniPet->increaseExp(iPoint,iType,_lpChatter->m_level.m_sValue))
				break;

			int	iAddSkill	=	lpMiniPet->addSpareSkill();

			if	(iAddSkill	!=	-1)
				lpActor->sendEtcWork(eEW_ADD_MINIPET_SPARE_SKILL,iAddSkill);
			
			BOOL	bIsEvolution=	FALSE;
			if	(uiOldPetJob!=	lpMiniPet->m_bf5Type)
				bIsEvolution=	TRUE;

			lpActor->sendMiniPetInfo(_lpChatter->m_bf5ActiveMiniPet,eUMIM_FEEDING,_lpChatter->m_bf5ActiveMiniPet,_lpChatter->m_bf5ActiveMiniPet2,iPoint,bIsEvolution,0);

			return	TRUE;
		}

		case	eOCM_RESET_GUILD_VP			:	//	"길드승점초기화"
		{
			g_game.sendWPSimpleWork(eSWGW_RESET_GUILD_PROLOG_POINT);

			_lpChatter->sendServerMessage("reset guild victory point",eSM_NOTICE);
		
			return	TRUE;
		}
		case	eOCM_RESET_GUILD_HONOR_POINT	:
		{
			g_game.sendWPSimpleWork(eSWGW_RESET_GUILD_HONOR_POINT);
		
			_lpChatter->sendServerMessage("reset guild honor point",eSM_NOTICE);
			return	TRUE;
		}
		case	eOCM_SET_GUILD_VP				:
		{
			int	iPoint	=	atoi(lpstrParam);

			g_game.sendWPSimpleWork(eSWGW_SET_GUILD_PROLOG_POINT,_lpChatter->m_wGuildSerial,iPoint);

			_lpChatter->sendServerMessage("Set guild victory point",eSM_NOTICE);

			return	TRUE;
		}
		case	eOCM_SET_GUILD_HONOR_POINT	:
		{
			int	iPoint	=	atoi(lpstrParam);

			g_game.sendWPSimpleWork(eSWGW_SET_GUILD_HONOR_POINT,_lpChatter->m_wGuildSerial,iPoint);

			_lpChatter->sendServerMessage("set guild honor point",eSM_NOTICE);

			return	TRUE;
		}
		case	eOCM_END_GUILD_PROLOG	:
		{
			g_game.sendWPSimpleWork(eSWGW_END_GUILD_PROLOG);
		
			_lpChatter->sendServerMessage("finish guild prolog",eSM_NOTICE);

			return	TRUE;
		}

		case	eOCM_TEST_GUILD_SIEGE_WARFARE	:
		{
			g_game.sendWPSimpleWork(eSWGW_BEGIN_TEST_SIEGE_WARFARE_OF_GUILD,0);

			_lpChatter->sendServerMessage("Test guild siege warfare",eSM_NOTICE);

			g_bIsTestSiegeWarfare	=	TRUE;

			return	TRUE;
		}
		case	eOCM_READY_TO_SIEGE_WARFARE	:
		{
			g_game.sendWPSimpleWork(eSWGW_BEGIN_TEST_SIEGE_WARFARE_OF_GUILD,1);

			_lpChatter->sendServerMessage("ready to siege warfare",eSM_NOTICE);

			return	TRUE;
		}
		case	eOCM_BEGIN_SIEGE_WARFARE	:
		{
			g_game.sendWPSimpleWork(eSWGW_BEGIN_TEST_SIEGE_WARFARE_OF_GUILD,2);

			_lpChatter->sendServerMessage("begin siege warfare",eSM_NOTICE);

			return	TRUE;
		}
		case	eOCM_READY_TO_END_SIEGE_WARFARE	:
		{
			g_game.sendWPSimpleWork(eSWGW_BEGIN_TEST_SIEGE_WARFARE_OF_GUILD,3);

			_lpChatter->sendServerMessage("Ready to end siege warfare",eSM_NOTICE);

			return	TRUE;
		}
		case	eOCM_END_SIEGE_WARFARE	:
		{
			g_game.sendWPSimpleWork(eSWGW_BEGIN_TEST_SIEGE_WARFARE_OF_GUILD,4);
		
			_lpChatter->sendServerMessage("finish to guild battle",eSM_NOTICE);

			return	TRUE;
		}
		
		case	eOCM_END_GUILD_BATTLE_TEST	:
		{
			g_bIsTestGuildBattle	=	FALSE;

#ifdef	_FOR_KOREA
			_lpChatter->sendServerMessage("길드전 테스트 종료",eSM_NOTICE);
#else
			_lpChatter->sendServerMessage("finish to guild battle test",eSM_NOTICE);
#endif

			return	TRUE;
		}

		case	eOCM_READY_TO_GUILD_BATTLE	:
		{
			if	(g_bIsTestServer	==	FALSE)	//	테섭이 아닐경우는 4레벨 이상 운영자만 사용 가능
				if	(_lpChatter->m_wOperatorLevel	<	4)
					return	TRUE;

			cDECODER	decoder;

			decoder.UploadBuffer(lpstrParam);

			char	*lpstrGuild1	=	decoder.GetWord();
			CGuild	*lpGuild1		=	g_guildManager.getByName(lpstrGuild1);
			char	*lpstrGuild2	=	decoder.GetWord();
			CGuild	*lpGuild2		=	g_guildManager.getByName(lpstrGuild2);
			int		iField			=	decoder.GetNumber();


			if	(!lpGuild1 || !lpGuild2)
				return	TRUE;

			if	(iField	>=	990	&&	iField	<=	999)
			{
				g_bIsTestGuildBattle	=	TRUE;
				g_gbsToday.resetSchedule();

				warpGuildBattleLobbyPlayerToBattleField(iField,lpGuild1->m_wSerial,lpGuild2->m_wSerial);
			}
		
			return	TRUE;
		}
		case	eOCM_CANCEL_GUILD_BATTLE	:
		{
			for	(int i=0;i<dGUILD_BATTLE_COUNT;i++)
				g_game.sendWPCancelGuildBattle(NULL,i,0);	//	이 시간대 건 다 취소
		
			return	TRUE;
		}
		case	eOCM_GET_GUILDITEM:
		{
			int lpSlot = _lpChatter->getValidInventorySlotIndex(TRUE);

			if(lpSlot == 0xffff)
			{
#ifdef	_FOR_KOREA
				_lpChatter->sendServerMessage("아이템 슬롯 비우고 오세요." , eSM_NOTICE);
#else
				_lpChatter->sendServerMessage("You Must Have one Empty Item slot.",eSM_NOTICE);
#endif				
				return TRUE;
			}

 			char*		strBuffer;
 			cDECODER	decoder;
			WORD		wBaseItem;
			DWORD		dwSerial;	
			int			iPlace;
 			
 			decoder.UploadBuffer(lpstrParam);
 			strBuffer = decoder.GetWord();
			wBaseItem = decoder.GetNumber();
			dwSerial = decoder.GetNumber();

			CGuild	*lpGuild	=	g_guildManager.getByName(strBuffer);

			if(!lpGuild)
			{
#ifdef	_FOR_KOREA
				_lpChatter->sendServerMessage("길드가 존재하지 않습니다.." , eSM_NOTICE);
#else
				_lpChatter->sendServerMessage("Can`t Find Guild.",eSM_NOTICE);
#endif
				return TRUE;
			}

			iPlace = lpGuild->GetItemPlace(wBaseItem , dwSerial);

			if(iPlace == 0xffffffff)
			{
#ifdef	_FOR_KOREA
				_lpChatter->sendServerMessage("아이템이 존재하지 않습니다." , eSM_NOTICE);
#else
				_lpChatter->sendServerMessage("Can`t Find Item In Guild Inventory.",eSM_NOTICE);
#endif
				return TRUE;
			}

			_lpChatter->withdrawGuildInventoryItem(lpSlot,iPlace , TRUE , lpGuild);

#ifdef	_FOR_KOREA
			_lpChatter->sendServerMessage("어드민 툴로 아이템이 사라졌는지를 확인하세요." , eSM_NOTICE);
#else
			_lpChatter->sendServerMessage("Check Item disappearance In AdminTool",eSM_NOTICE);
#endif

			return	TRUE;
		}			
		case eOCM_GGG_SKILL:
		{
			_lpChatter->setGGGValueForSkillUp();
			_lpChatter->buildPower();
			return TRUE;
		}
		case eOCM_DEATH_PENALTY:
		{
			_lpChatter->m_bf12DeathPeneltySecond = 1;
			_lpChatter->buildPower();
			
			return TRUE;
		}
		case eOCM_CECHK_WORD_COMPARE_EVENT:
		{
			char strMessage[256];

			if(STRICMP(g_eventManager.m_wordCompareEvent.m_strEventWord , "") == 0)
			{
				setMessageKORorENG(strMessage , "예약된 이구동성 이벤트가 없습니다." , "No Word Compare Event!!");

				_lpChatter->sendServerMessage(strMessage);
			}
			else
			{	
				sprintf(strMessage , "KeyWord : %s" , 
					g_eventManager.m_wordCompareEvent.m_strEventWord);
				_lpChatter->sendServerMessage(strMessage);
				//
				sprintf(strMessage , "Start = Year:%d , Month:%d , day:%d  , Hour:%d , Min:%d", 
					g_eventManager.m_wordCompareEvent.m_wBeginTimeYear ,
					g_eventManager.m_wordCompareEvent.m_wBeginTimeMonth ,
					g_eventManager.m_wordCompareEvent.m_wBeginTimeDay ,
					g_eventManager.m_wordCompareEvent.m_wBeginTimeHour ,
					g_eventManager.m_wordCompareEvent.m_wBeginTimeMinite 
					);
				_lpChatter->sendServerMessage(strMessage);
				sprintf(strMessage , "End = Year:%d , Month:%d , day:%d  , Hour:%d , Min:%d", 
					g_eventManager.m_wordCompareEvent.m_wEndTimeYear ,
					g_eventManager.m_wordCompareEvent.m_wEndTimeMonth ,
					g_eventManager.m_wordCompareEvent.m_wEndTimeDay ,
					g_eventManager.m_wordCompareEvent.m_wEndTimeHour ,
					g_eventManager.m_wordCompareEvent.m_wEndTimeMinite 
					);
				_lpChatter->sendServerMessage(strMessage);
				_lpChatter->sendServerMessage(_ms("start Ment = %s" , g_eventManager.m_wordCompareEvent.m_strBeginComment));
				_lpChatter->sendServerMessage(_ms("End Ment = %s" , g_eventManager.m_wordCompareEvent.m_strEndComment));
				_lpChatter->sendServerMessage(_ms("Active Ment = %s" , g_eventManager.m_wordCompareEvent.m_strActiveComment));
			}
			return TRUE;
		}

#ifdef	_DEBUG
		case	eOCM_ONE_SHOT_ONE_KILL:
			_lpChatter->m_bf1DevIsOneShotOneKill	=	1-_lpChatter->m_bf1DevIsOneShotOneKill;
			_lpChatter->sendServerMessage(_ms("%s = %d" ,lpstrCommand,_lpChatter->m_bf1DevIsOneShotOneKill));
			return	TRUE;

		case	eOCM_SNIPER:
			_lpChatter->m_bf1DevIsPerfectHit	=	1-_lpChatter->m_bf1DevIsPerfectHit;
			_lpChatter->sendServerMessage(_ms("%s = %d" ,lpstrCommand, _lpChatter->m_bf1DevIsPerfectHit));
			return	TRUE;

		case	eOCM_PERFECT_DODGE:
			_lpChatter->m_bf1DevIsPerfectDodge	=	1-_lpChatter->m_bf1DevIsPerfectDodge;
			_lpChatter->sendServerMessage(_ms("%s = %d" ,lpstrCommand, _lpChatter->m_bf1DevIsPerfectDodge));
			return	TRUE;
		case	eOCM_DAMAGE_LIMIT_RELEASE:
			_lpChatter->m_bf1DevIsDamageLimitRelease	=	1-_lpChatter->m_bf1DevIsDamageLimitRelease;
			_lpChatter->sendServerMessage(_ms("%s = %d" ,lpstrCommand, _lpChatter->m_bf1DevIsDamageLimitRelease));
			return	TRUE;
		case	eOCM_GLASS_BODY:
			_lpChatter->m_bf1DevIsOneShotOneDeath	=	1-_lpChatter->m_bf1DevIsOneShotOneDeath;
			_lpChatter->sendServerMessage(_ms("%s = %d" ,lpstrCommand, _lpChatter->m_bf1DevIsOneShotOneDeath));
			return	TRUE;

		case	eOCM_BLIND_PERSON:
			_lpChatter->m_bf1DevIsBlindPerson	=	1-_lpChatter->m_bf1DevIsBlindPerson;
			_lpChatter->sendServerMessage(_ms("%s = %d" ,lpstrCommand, _lpChatter->m_bf1DevIsBlindPerson));
			return	TRUE;

		case	eOCM_VILLAGE_DRUM:
			_lpChatter->m_bf1DevIsVillageDrum	=	1-_lpChatter->m_bf1DevIsVillageDrum;
			_lpChatter->sendServerMessage(_ms("%s = %d" ,lpstrCommand, _lpChatter->m_bf1DevIsVillageDrum));
			return	TRUE;

		case	eOCM_TARGET_DAMAGE:
		{
			cDECODER	decoder;

			decoder.UploadBuffer(lpstrParam);

			int		iValue	=	decoder.GetNumber();

			_lpChatter->m_bf7DevTargetReduceHPRate	=	iValue;

			_lpChatter->sendServerMessage(_ms("%s = %d%%" ,lpstrCommand, _lpChatter->m_bf7DevTargetReduceHPRate));
			return	TRUE;
		}

		case	eOCM_MY_DAMAGE:
		{
			cDECODER	decoder;

			decoder.UploadBuffer(lpstrParam);

			int		iValue	=	decoder.GetNumber();

			_lpChatter->m_bf7DevMyReduceHPRate	=	iValue;

			_lpChatter->sendServerMessage(_ms("%s = %d%%" ,lpstrCommand, _lpChatter->m_bf7DevMyReduceHPRate	));
			return	TRUE;
		}

		case	eOCM_REBIRTH_COUNT	:
		{
			cDECODER	decoder;

			decoder.UploadBuffer(lpstrParam);

			int		iValue	=	decoder.GetNumber();
			iValue			=	min(iValue,dLIMIT_REBIRTH_COUNT);
			iValue			=	max(iValue,0);

			_lpChatter->m_bf4RebirthCount	=	iValue;

			_lpChatter->sendEtcWork(eEW_CHANGE_REBIRTH_COUNT,iValue);

			_lpChatter->rebuildPlayerData(_lpChatter->m_iLevel);

			return	TRUE;
		}
		case	eOCM_DAWN_SEAL_COUNT	:
		{
			cDECODER	decoder;

			decoder.UploadBuffer(lpstrParam);

			int		iValue	=	decoder.GetNumber();
			iValue			=	min(iValue,dLIMIT_REBIRTH_COUNT);
			iValue			=	max(iValue,0);

			_lpChatter->m_bf4UseSealSphereOfDawnCount	=	iValue;

			_lpChatter->sendEtcWork(eEW_CHANGE_USE_SEAL_SPHERE_OF_DAWN_COUNT,iValue);

			return	TRUE;
		}

		case	eOCM_CHANGE_LEVEL_GUILD_GUARDIAN	:
			{
				if	(_lpChatter->m_wOperatorLevel	<	2)
					return	TRUE;
				
				cDECODER	decoder;
				
				decoder.UploadBuffer(lpstrParam);
				
				int		iValue	=	decoder.GetNumber()-1;
				iValue			=	min(iValue,100);
				iValue			=	max(iValue,0);
				
				CGuild* lpGuild = _lpChatter->getGuild();

				if(!lpGuild)
					return TRUE;

				UINT uiExp = lpGuild->getGuildPetExpTable(iValue);

				for (int iPet=0;iPet<dGUILD_PET_TYPE_COUNT;iPet++)
				{
					lpGuild->m_aGuildPet[iPet].m_dwExperience 	=	uiExp;
					g_game.sendWPSimpleWork(eSWGW_CHANGE_GUILD_PET_EXP,lpGuild->m_wSerial ,iPet,uiExp);
				}

				for (int iGuardian=0;iGuardian<dGUILD_GUARDIAN_TYPE_COUNT;iGuardian++)
				{
					lpGuild->m_aGuildGuardian[iGuardian].m_dwExperience			=	uiExp;
					g_game.sendWPSimpleWork(eSWGW_CHANGE_GUILD_PET_EXP,lpGuild->m_wSerial ,iGuardian+dGUILD_PET_TYPE_COUNT,uiExp);
				}

				lpGuild->updateGuildInfo();

				return	TRUE;
			}
		case	eOCM_CLEAN_FLOOR	:
			{
				
				if	(_lpChatter->m_wOperatorLevel	<	2)
					return	TRUE;
				
				for(int i=0;i<10;++i)
					_lpChatter->m_lpField->removeOldestDroppedItem(dMAX_OPERATE_DROPPED_ITEM, TRUE);
				
				return	TRUE;
			}
		case	eOCM_RESET_COOL_TIME	:
			{
				
				if	(_lpChatter->m_wOperatorLevel	<	2)
					return	TRUE;
				
				for	(int i=0; i < c_iCoolTimeSkillCount; ++i )
				{
					_lpChatter->m_awSkillCoolTime[i] = 0;
					_lpChatter->sendEtcWork(eEW_SET_COOL_TIME,i,_lpChatter->m_awSkillCoolTime[i]);
				}
				return	TRUE;
			}	
		case	eOCM_RESET_GUILD_DUNGEON_PLAY_COUNT	:
			{
				
				if	(_lpChatter->m_wOperatorLevel	<	2)
					return	TRUE;
				
				_lpChatter->m_bf3GuildDungeonPlayTime = 0;
				_lpChatter->sendServerMessage(lpstrCommand);
				
				return	TRUE;
			}
		case	eOCM_SHOW_GUILD_DUNGEON_PLAY_COUNT	:
			{
				
				if	(_lpChatter->m_wOperatorLevel	<	2)
					return	TRUE;
				
				_lpChatter->sendServerMessage(_ms("%s = %d%%" ,lpstrCommand, _lpChatter->m_bf3GuildDungeonPlayTime));
				
				return	TRUE;
			}	
		case	eOCM_INCREASE_GUILD_ITEM_DROP	:
			{
				
				if	(_lpChatter->m_wOperatorLevel	<	2)
					return	TRUE;

				_lpChatter->m_bf1DevIsGuildItemDrop = !_lpChatter->m_bf1DevIsGuildItemDrop;

				_lpChatter->sendServerMessage(_ms("%s = %d%%" ,lpstrCommand,_lpChatter->m_bf1DevIsGuildItemDrop ));
				
				return	TRUE;
			}
		case	eOCM_CARVING_SUCCESS:
			{
				_lpChatter->m_bf1DevIsCarvingSuccess	=	1-_lpChatter->m_bf1DevIsCarvingSuccess;
				_lpChatter->sendServerMessage(_ms("%s = %d" ,lpstrCommand, _lpChatter->m_bf1DevIsCarvingSuccess));
				
				if(_lpChatter->m_bf1DevIsCarvingFail)
				{
					_lpChatter->m_bf1DevIsCarvingFail	=	1-_lpChatter->m_bf1DevIsCarvingFail;
					_lpChatter->sendServerMessage(_ms("%s = %d" ,"잉여의손", _lpChatter->m_bf1DevIsCarvingFail));
				}
				return	TRUE;
			}
		case	eOCM_CARVING_FAIL:
			{
				_lpChatter->m_bf1DevIsCarvingFail	=	1-_lpChatter->m_bf1DevIsCarvingFail;
				_lpChatter->sendServerMessage(_ms("%s = %d" ,lpstrCommand, _lpChatter->m_bf1DevIsCarvingFail));
				
				if(_lpChatter->m_bf1DevIsCarvingSuccess)
				{
					_lpChatter->m_bf1DevIsCarvingSuccess	=	1-_lpChatter->m_bf1DevIsCarvingSuccess;
					_lpChatter->sendServerMessage(_ms("%s = %d" ,"반지의제왕", _lpChatter->m_bf1DevIsCarvingSuccess));
				}
				return	TRUE;
			}
		case	eOCM_RESET_ITEM_COOLTIME:
			{
				memset(_lpChatter->m_awItemCoolTime,0,sizeof(_lpChatter->m_awItemCoolTime));
				_lpChatter->sendServerMessage(_ms("%s" ,lpstrCommand));
				_lpChatter->sendEtcWork(eEW_RESET_ITEM_COOLTIME);
				return	TRUE;
			}
		case	eOCM_SUMMON_EVENT_MONSTER:
			{
				_lpChatter->sendServerMessage(_ms("%s" ,lpstrCommand));
				_lpChatter->sendEtcWork(eEW_SUMMON_EVENT_MONSTER);
				return TRUE;
			}
		case	eOCM_REMOVE_SUMMON_EVENT_MONSTER:
			{
				int	iCount	=	0;
				for(int i=0;i<dMAX_ACTOR_IN_FIELD;++i)
				{
					cACTOR*	lpActor	=	_lpChatter->m_lpField->getActor(i);
					if(!lpActor)
						continue;
					if(lpActor->isPlayer())
						continue;
					if(!lpActor->m_bf1IsEventSummonMonster)
						continue;
					lpActor->immediatelyExit();					
					++iCount;
				}
				_lpChatter->sendServerMessage(_ms("%s : %d" ,lpstrCommand, iCount));
				
				return TRUE;
			}
		case	eOCM_GIVE_BUFF_TO_ALL_PLAYER:
			{

				for(int i=0;i<_lpChatter->m_lpField->m_iActorCount;++i)
				{
					cACTOR*	lpActor	=	_lpChatter->m_lpField->getActor(i);
					if(!lpActor)
						continue;
					if(!lpActor->isPlayer())
						continue;

					cDECODER	decoder;
					
					decoder.UploadBuffer(lpstrParam);
					
					int		iLevel	=	decoder.GetNumber();
					if(iLevel == dCODE_VALID_NUMBER || iLevel <= 0 || iLevel > 100)
					{
						_lpChatter->sendServerMessage(_ms("%s = err : level" ,lpstrCommand),eSM_NOTICE);
						break;
					}
					int		iStatus	=	decoder.GetNumber();
					if(iStatus == dCODE_VALID_NUMBER || iStatus <= 0 ||iStatus > 9999 )
					{
						_lpChatter->sendServerMessage(_ms("%s = err : Status" ,lpstrCommand),eSM_NOTICE);
						break;
					}
					int		iTime	=	decoder.GetNumber();
					if(iTime == dCODE_VALID_NUMBER || iTime < 0 || iTime> 50000)
					{
						_lpChatter->sendServerMessage(_ms("%s = err : time" ,lpstrCommand),eSM_NOTICE);
						break;				
					}
					iTime	*= 100;
								
					cACTOR cTempActor;
					cTempActor.m_strength.m_sFixValue		= iStatus;
					cTempActor.m_agility.m_sFixValue		= iStatus;
					cTempActor.m_constitution.m_sFixValue	= iStatus;
					cTempActor.m_intelligence.m_sFixValue	= iStatus;
					cTempActor.m_wisdom.m_sFixValue			= iStatus;
					cTempActor.m_charisma.m_sFixValue		= iStatus;
					cTempActor.m_luck.m_sFixValue			= iStatus;

					cAbility		ability;
					ability.m_wSkill	= 52;	//	핫인챈팅
					ability.m_wLevel	= iLevel;
					
					lpActor->applySkillExtraEffects(&cTempActor,&ability,0,lpActor->m_pos.x, lpActor->m_pos.y, FALSE, FALSE, iTime);
					ability.m_wSkill	=	64;		//	헤이스트
					lpActor->applySkillExtraEffects(&cTempActor,&ability,0,lpActor->m_pos.x, lpActor->m_pos.y, FALSE, FALSE, iTime);
					ability.m_wSkill	=	116;		//	축복
					lpActor->applySkillExtraEffects(&cTempActor,&ability,0,lpActor->m_pos.x, lpActor->m_pos.y, FALSE, FALSE, iTime);
				}

				_lpChatter->sendServerMessage(_ms("%s" ,lpstrCommand));
				
				return TRUE;
			}
		case	eOCM_REVIVAL_TO_ALL_PLAYER:
			{

				for(int i=0;i<_lpChatter->m_lpField->m_iActorCount;++i)
				{
					cACTOR*	lpActor	=	_lpChatter->m_lpField->getActor(i);
					if(!lpActor)
						continue;
					if(!lpActor->isPlayer())
						continue;

					lpActor->revive(100,100);
				}
				_lpChatter->sendServerMessage(_ms("%s" ,lpstrCommand));

			
				return TRUE;
			}

		case	eOCM_RGBS	:
			g_game.sendWPSimpleWork(eSWGW_RESET_GUILD_BATTLE_SCHEDULE);
			break;
			
		case	eOCM_SEND_GOLD_LOG:
			{
				
				_lpChatter->m_bf1IsSendGoldLog	=	1-_lpChatter->m_bf1IsSendGoldLog;
				_lpChatter->sendServerMessage(_ms("%s = %d" ,lpstrCommand, _lpChatter->m_bf1IsSendGoldLog));		
				return TRUE;
			}

#endif
	}

	return	FALSE;	
}