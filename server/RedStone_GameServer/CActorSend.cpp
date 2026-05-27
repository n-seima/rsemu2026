#include "cACTOR.H"
#include "cFIELD.H"
#include "debugCode.H"
#include "instance_guild_battle_manager.H"
#include "CGame.H"
#include "CShoppingCart.H"
#include "itemPackList.H"
#include "packetManager.H"
#include "duel.H"
#include "cGuildManager.h"

void
cACTOR::addSendMoveToParty(int _iX,int _iY,int _iDX,int _iDY)
{
	cParty	*lpParty	=	getParty();

	if	(!lpParty)
		return;

	if	(lpParty->getMemberCountForMovePacket(m_wCurrentField)	<=	1)
		return;

	SG_MOVE		packetMove;

	packetMove.base.set(sizeof(SG_MOVE),dSG_MOVE_FOR_PARTY_MEMBER);

	packetMove.wSerial				=	m_wSerialInField;
	packetMove.wPosX				=	_iX;
	packetMove.wPosY				=	_iY;	//	현재 위치
	packetMove.wDestPosX			=	_iDX;
	packetMove.wDestPosY			=	_iDY;	//	목표 위치

	packetMove.moveInfo.wPPS		=	m_wPPS;
	packetMove.moveInfo.isRun		=	m_isRunning;	//	뛰냐-_-?
	packetMove.moveInfo.isContinue	=	FALSE;	//	연속 이동

	m_lpField->addSendPacket((ALL_MSG*)&packetMove,m_wSerialInField);
}

void
cACTOR::addSendMove(int _iX,int _iY,int _iDX,int _iDY,BOOL _isContinue)
{
	SG_MOVE		packetMove;

	packetMove.base.set(sizeof(SG_MOVE),dSG_MOVE);

	packetMove.wSerial				=	m_wSerialInField;
	packetMove.wPosX				=	_iX;
	packetMove.wPosY				=	_iY;	//	현재 위치
	packetMove.wDestPosX			=	_iDX;
	packetMove.wDestPosY			=	_iDY;	//	목표 위치

	packetMove.moveInfo.wPPS		=	m_wPPS;
	packetMove.moveInfo.isRun		=	m_isRunning;	//	뛰냐-_-?
	packetMove.moveInfo.isContinue	=	_isContinue;	//	연속 이동

	m_lpField->addSendPacket((ALL_MSG*)&packetMove,m_wSerialInField);
}

//
//	이동 메시지 전송
void
cACTOR::addNpcMoveMessage(BOOL _isContinue)
{
	addSendMove(m_pos.x,m_pos.y,m_posDest.x,m_posDest.y,_isContinue);
}

void
cACTOR::sendOpenRepairWindow(int _iNpcSerial)
{
	SG_OPEN_REPAIR_ITEM_WINDOW	packet;

	packet.base.set(sizeof(packet),dSG_OPEN_REPAIR_ITEM_WINDOW);
	packet.wNpcSerial	=	_iNpcSerial;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendOpenDisjointingWindow(int _iNpcSerial)
{
	SG_OPEN_DISJOINTING_ITEM_WINDOW		packet;
	
	packet.base.set(sizeof(packet),dSG_OPEN_DISJOINTING_DATA);
	packet.wNpcSerial	=	_iNpcSerial;
	
	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

BOOL
cACTOR::sendQuestionForRecoverDeathPenelty()
{
	SG_QUESTION_FOR_RECOVER_DEATH_PENELTY	packet;

	packet.base.set(sizeof(packet),dSG_QUESTION_FOR_RECOVER_DEATH_PENELTY);

	packet.wRecoverTime	=	getCurrentLevelDeathPeneltySecond()/2;

	packet.wRecoverTime = getFixedLevelDeathPeneltySecondByGuild(packet.wRecoverTime);
	
	packet.dwPrice		=	getCurrentLevelDeathPeneltyRecoverPrice();

	LONGLONG	llPrice	=	packet.dwPrice;
	llPrice				-=	llPrice*m_wCourtesyTitleValue/10000;
	packet.dwPrice		=	(DWORD)llPrice;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

void
cACTOR::sendPartyMessage(int _iMessage)
{
	SG_PARTY_MESSAGE	packet;

	packet.base.set(sizeof(SG_PARTY_MESSAGE),dSG_PARTY_MESSAGE);

	packet.wMessage	=	_iMessage;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendRemoveActor(int _iSerial,BOOL _bIsHide)
{
	SG_REMOVE_ACTOR	packetRemoveActor;

	packetRemoveActor.base.set(sizeof(SG_REMOVE_ACTOR),dSG_REMOVE_ACTOR);
	packetRemoveActor.bf15Actor	=	_iSerial;
	packetRemoveActor.bf1IsHide	=	_bIsHide;

	g_userPM.add(m_iClientSerial,&packetRemoveActor,packetRemoveActor.base.wSize);
}

void
cACTOR::sendPlayerInfo(cACTOR *_lpActor)
{
	SG_PLAYER_INFO		packet;

	packet.base.set(sizeof(packet),dSG_PLAYER_INFO);

	packet.m_wExtraInventorySize		=	m_bf6ExtraInventorySize;
	packet.m_wBonusStatePoint			=	m_wBonusStatePoint;
	packet.m_wProcessChapterIndex		=	m_wProcessChapterIndex;
	packet.m_wProcessHighQuestIndex		=	m_wProcessHighQuestIndex;
	packet.m_bf5ActiveMiniPet			=	m_bf5ActiveMiniPet;
	packet.m_bf5ActiveMiniPet2			=	m_bf5ActiveMiniPet2;

	packet.m_reversalFrameYear			=	m_reversalFrameYear;
	packet.m_reversalFrameMonth			=	m_reversalFrameMonth;
	packet.m_reversalFrameDay			=	m_reversalFrameDay;
	packet.m_reversalFrameHour			=	m_reversalFrameHour;
	packet.m_reversalFrameMinute		=	m_reversalFrameMinute;	//	25

	packet.m_bf4CH5StateBonusCount			=	m_bf4CH5StateBonusCount;
	packet.m_bf4CH5SkillBonusCount			=	m_bf4CH5SkillBonusCount;
	packet.m_bf4CH5HPBonusCount				=	m_bf4CH5HPBonusCount;
	packet.m_bf4CH5CPBonusCount				=	m_bf4CH5CPBonusCount;

	packet.m_bf4CH5PeneltyHPBonusCount		=	m_bf4CH5PeneltyHPBonusCount;
	packet.m_bf4CH5PeneltyCPBonusCount		=	m_bf4CH5PeneltyCPBonusCount;
	packet.m_bf3GreateGodGrace				=	m_bf3GreateGodGrace;

	packet.m_bf3GuildDungeonPlayTime		=	m_bf3GuildDungeonPlayTime;
	
	packet.m_bf4EatFireMiniPetCount			=	m_bf4EatFireMiniPetCount;
	packet.m_bf4EatWaterMiniPetCount		=	m_bf4EatWaterMiniPetCount;
	packet.m_bf4EatWindMiniPetCount			=	m_bf4EatWindMiniPetCount;
	packet.m_bf4EatEarthMiniPetCount		=	m_bf4EatEarthMiniPetCount;
	packet.m_bf4EatLightMiniPetCount		=	m_bf4EatLightMiniPetCount;
	packet.m_bf4EatDarkMiniPetCount			=	m_bf4EatDarkMiniPetCount;	//	24
	packet.m_bf4RebirthCount				=	m_bf4RebirthCount;	//	전생 횟수
	packet.m_bf4UseSealSphereOfDawnCount	=	m_bf4UseSealSphereOfDawnCount;	//	새벽의 봉인구 사용 횟수
	
	memcpy(packet.m_awItemCoolTime,m_awItemCoolTime,sizeof(m_awItemCoolTime));
	memcpy(packet.m_awQuestCountDownTimer,m_awQuestCountDownTimer,sizeof(m_awQuestCountDownTimer));
	memcpy(packet.m_aCompleteQuest,m_aCompleteQuest,sizeof(m_aCompleteQuest));
	memcpy(packet.m_aCompleteQuest2,m_aCompleteQuest2,sizeof(m_aCompleteQuest2));
	memcpy(packet.m_aCompleteArbeit,m_aCompleteArbeit,sizeof(m_aCompleteArbeit));
	memcpy(packet.m_aCompleteArbeit2,m_aCompleteArbeit2,sizeof(m_aCompleteArbeit2));
	
	memcpy(packet.m_aExtraInventory,m_aExtraInventory,sizeof(m_aExtraInventory));
	memcpy(packet.m_aMiniPet,m_aMiniPet,sizeof(m_aMiniPet));
	
	memcpy(&packet.playerData	,this	,sizeof(CPlayerSaveDataForClientDefine));

	g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendSuccesToJoin()
{
	SG_SUCCESS_TO_JOIN_GAME	packet;

	packet.base.set(sizeof(packet),dSG_SUCCESS_TO_JOIN_GAME);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendRedistribute(int _iWork)
{
	SG_REDISTRIBUTE	packet;

	packet.base.set(sizeof(packet),dSG_REDISTRIBUTE);
	packet.wWork	=	_iWork;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendItemWorkResult(int _iResult)
{
	SG_ITEM_WORK_RESULT	packet;

	packet.base.set(sizeof(packet),dSG_ITEM_WORK_RESULT);
	packet.wResult	=	_iResult;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendEtcWorkToPartyMember(BOOL _bIsSendSameField,int _iWork,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5,int _iValue6)
{
	if	(!isPlayer())
		return;

	cParty	*lpParty	=	getParty();

	if	(!lpParty)
		return;

	SG_ETC_WORK	packet;

	packet.base.set(sizeof(packet),dSG_ETC_WORK);
	packet.wWork		=	_iWork;
	packet.aiValue[0]	=	_iValue1;
	packet.aiValue[1]	=	_iValue2;
	packet.aiValue[2]	=	_iValue3;
	packet.aiValue[3]	=	_iValue4;
	packet.aiValue[4]	=	_iValue5;
	packet.aiValue[5]	=	_iValue6;

	if	(_bIsSendSameField)
		lpParty->sendPacketToSameFieldMember(this,&packet,sizeof(packet));
	else
		lpParty->sendPacketToAllMember(&packet,sizeof(packet));
}
void
cACTOR::sendEtcWork(int _iWork,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5,int _iValue6)
{
	if	(!isPlayer())
		return;

	SG_ETC_WORK	packet;

	packet.base.set(sizeof(packet),dSG_ETC_WORK);
	packet.wWork		=	_iWork;
	packet.aiValue[0]	=	_iValue1;
	packet.aiValue[1]	=	_iValue2;
	packet.aiValue[2]	=	_iValue3;
	packet.aiValue[3]	=	_iValue4;
	packet.aiValue[4]	=	_iValue5;
	packet.aiValue[5]	=	_iValue6;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendEtcWork(int _iWork,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5)
{
	if	(!isPlayer())
		return;

	SG_ETC_WORK	packet;

	packet.base.set(sizeof(packet)-sizeof(int),dSG_ETC_WORK);
	packet.wWork		=	_iWork;
	packet.aiValue[0]	=	_iValue1;
	packet.aiValue[1]	=	_iValue2;
	packet.aiValue[2]	=	_iValue3;
	packet.aiValue[3]	=	_iValue4;
	packet.aiValue[4]	=	_iValue5;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendEtcWork(int _iWork,int _iValue1,int _iValue2,int _iValue3,int _iValue4)
{
	if	(!isPlayer())
		return;

	SG_ETC_WORK	packet;

	packet.base.set(sizeof(packet)-sizeof(int)*2,dSG_ETC_WORK);
	packet.wWork		=	_iWork;
	packet.aiValue[0]	=	_iValue1;
	packet.aiValue[1]	=	_iValue2;
	packet.aiValue[2]	=	_iValue3;
	packet.aiValue[3]	=	_iValue4;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendEtcWork(int _iWork,int _iValue1,int _iValue2,int _iValue3)
{
	if	(!isPlayer())
		return;

	SG_ETC_WORK	packet;

	packet.base.set(sizeof(packet)-sizeof(int)*3,dSG_ETC_WORK);
	packet.wWork		=	_iWork;
	packet.aiValue[0]	=	_iValue1;
	packet.aiValue[1]	=	_iValue2;
	packet.aiValue[2]	=	_iValue3;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendEtcWork(int _iWork,int _iValue1,int _iValue2)
{
	if	(!isPlayer())
		return;

	SG_ETC_WORK	packet;

	packet.base.set(sizeof(packet)-sizeof(int)*4,dSG_ETC_WORK);
	packet.wWork		=	_iWork;
	packet.aiValue[0]	=	_iValue1;
	packet.aiValue[1]	=	_iValue2;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendEtcWork(int _iWork,int _iValue1)
{
	if	(!isPlayer())
		return;

	SG_ETC_WORK	packet;

	packet.base.set(sizeof(packet)-sizeof(int)*5,dSG_ETC_WORK);
	packet.wWork		=	_iWork;
	packet.aiValue[0]	=	_iValue1;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}


void
cACTOR::sendEtcWork(int _iWork)
{
	if	(!isPlayer())
		return;

	SG_ETC_WORK	packet;

	packet.base.set(sizeof(packet)-12,dSG_ETC_WORK);
	packet.wWork		=	_iWork;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendPlaySkillSound(int _iSkill,int _iSound)
{
	SG_PLAY_SKILL_SOUND	packet;

	packet.base.set(sizeof(packet),dSG_PLAY_SKILL_SOUND);

	packet.wSkill	=	_iSkill;
	packet.wSound	=	_iSound;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendCorrectActorPos(int _iX,int _iY,BOOL _bIsInnerTeleport)
{
	SG_CORRECT_PLAYER_POSITION	packet;

	packet.base.set(sizeof(packet),dSG_CORRECT_PLAYER_POSITION);
	packet.bf15Serial			=	m_wSerialInField;
	packet.wPosX				=	_iX;
	packet.wPosY				=	_iY;
	packet.bf1IsInnerTeleport	=	_bIsInnerTeleport;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//	스킬 사용 결과 
void
cACTOR::sendUseSkillResult(int _iResult)
{
	SG_USE_SKILL_RESULT	packet;

	packet.base.set(sizeof(SG_USE_SKILL_RESULT),dSG_USE_SKILL_RESULT);
	packet.wResult	=	_iResult;
	packet.sRemainCP=	m_iCP/100;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}


//	현재 위치를 전송함
void
cACTOR::sendCurrentPosition(BOOL _bIsInnerTeleport)
{
	addPacketStop();
	
	if	(isPlayer())
	{
		SG_CORRECT_PLAYER_POSITION	packet;

		packet.base.set(sizeof(packet),dSG_CORRECT_PLAYER_POSITION);
		packet.bf15Serial			=	m_wSerialInField;
		packet.wPosX				=	m_pos.x;
		packet.wPosY				=	m_pos.y;
		packet.bf1IsInnerTeleport	=	_bIsInnerTeleport;

		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
	}
}

void
cACTOR::sendRidingDog(BOOL _bIsRide,int _iGrade,int _iX,int _iY)
{
	SG_RIDING_DOG	packet;

	packet.base.set(sizeof(packet),dSG_RIDING_DOG);
	packet.bf1IsRiding	=	_bIsRide;
	packet.bf2DogGrade	=	_iGrade;
	packet.wPosX		=	_iX;
	packet.wPosY		=	_iY;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendStopBattle(BOOL _bIsReleaseFocus)
{
	SG_STOP_BATTLE	packet;

	packet.base.set(sizeof(packet),dSG_STOP_BATTLE);
	packet.bf1IsReleaseFocus	=	_bIsReleaseFocus;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//	가디언 포스트를 심는다.
void
cACTOR::sendPlantGuardianPost(int _iRegSerial,int _iX,int _iY,int _iImage,int _iSkill,int _iLevel)
{
	SG_GUARDIAN_POST	packet;

	packet.base.set(sizeof(packet),dSG_GUARDIAN_POST);

	packet.bf12Caster	=	m_wSerialInField;
	packet.bf12Skill	=	_iSkill;
	packet.bf8Level		=	_iLevel;
	packet.wRegSerial	=	_iRegSerial;
	packet.wX			=	_iX;
	packet.wY			=	_iY;
	packet.wImage		=	_iImage;	//	이게 0xffff이면 포스트 사라지기
	packet.wTargetCount	=	0;	//	이게 0이면 걍 포스트 심기만 한다.

	packet.base.wSize	=	sizeof(SG_GUARDIAN_POST)-sizeof(packet.aData);

	m_lpField->addSendPacket(&packet,m_wSerialInField);
}

//	가디언 포스트 공격을 한다.
void
cACTOR::sendStrikeGuardianPostAttack(int _iRegSerial,int _iX,int _iY,int _iImage,CDamageActorInfoTrailer *_lpInfo,int _iSkill,int _iLevel)
{
	int					iIndex	=	0;
	SG_GUARDIAN_POST	packet;

	packet.base.set(sizeof(packet),dSG_GUARDIAN_POST);

	packet.bf12Caster	=	m_wSerialInField;
	packet.bf12Skill	=	_iSkill;
	packet.bf8Level		=	_iLevel;
	packet.wRegSerial	=	_iRegSerial;
	packet.wX			=	_iX;
	packet.wY			=	_iY;
	packet.wImage		=	_iImage;			//	이게 0xffff이면 포스트 사라지기
	packet.wTargetCount	=	0;	//	이게 0이면 걍 포스트 심기만 한다.
	packet.sCP			=	(short)(m_iCP/100);

	for (int i=0;i<_lpInfo->m_wCount;i++)
	{
		if (packet.wTargetCount	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK)
		{
			m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);

			packet.wTargetCount	=	0;
		}

		packet.aData[packet.wTargetCount].wActor			=	_lpInfo->m_aData[i].m_wActorSerial;
		packet.aData[packet.wTargetCount].wPhysicalDamage	=	(WORD)(_lpInfo->m_aData[i].m_hitInfo.m_dwPhysicalDamage/100);
		packet.aData[packet.wTargetCount].wMagicDamage		=	(WORD)(_lpInfo->m_aData[i].m_hitInfo.m_dwMagicDamage/100);
		packet.aData[packet.wTargetCount].dwResultField		=	_lpInfo->m_aData[i].m_hitInfo.m_dwResultField;

		packet.wTargetCount++;
	}

	if (packet.wTargetCount)
	{
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData)+sizeof(cWIDE_AREA_DAMAGE_E)*packet.wTargetCount;

		m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
	}
}

//	가디언 포스트를 뽑아내다.
void
cACTOR::sendPullUpGuardianPost(int _iRegSerial)
{
	SG_GUARDIAN_POST	packet;

	packet.base.set(sizeof(packet),dSG_GUARDIAN_POST);

	packet.bf12Caster	=	m_wSerialInField;
	packet.wImage		=	0xffff;
	packet.wRegSerial	=	_iRegSerial;
	packet.wTargetCount	=	0;
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData);

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}

//
//	죽음 메시지 전송
void
cACTOR::sendKillMessage(BOOL _bIsInstanceKill,BOOL _bIsReleaseSummon)
{
	SG_KILL			packet;

	packet.base.set(sizeof(SG_KILL),dSG_KILL);

	packet.wSerial					=	m_wSerialInField;
	packet.bf12DeathPeneltySecond	=	m_bf12DeathPeneltySecond;
	packet.bf1IsInstanceKill		=	_bIsInstanceKill;
	packet.bf1IsReleaseSummon		=	_bIsReleaseSummon;

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}


void
cACTOR::sendScreenEvent(int _iX , int _iY , int _iTime , int _iWaitTime)
{
	SG_SCREEN_EVENT		packet;	

	packet.base.set(sizeof(SG_SCREEN_EVENT),dSG_EVENT_SCREEN_MOVE);

	packet.waValue[0]				=		 _iX;
	packet.waValue[1]				=		 _iY;
	packet.waValue[2]				=		 _iTime;
	packet.waValue[3]				=		 _iWaitTime;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	멈췄다.
void
cACTOR::addPacketStop()
{
	SG_STOP		packet;

	packet.base.set(sizeof(SG_STOP),dSG_STOP);

	packet.bf12Serial	=	m_wSerialInField;
	packet.bf3Direct	=	m_wDirect;		//	이동 속도 보정
	packet.wPosX		=	m_pos.x;
	packet.wPosY		=	m_pos.y;	//	현재 위치
	packet.bf1IsChangeOwnerDirect	=	FALSE;

	if	(m_wSlaveActor != 0xffff)
	{
		packet.wPosX	=	m_posFeignDeath.x;
		packet.wPosY	=	m_posFeignDeath.y;
	}

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}

//	미스
void
cACTOR::addPacketMiss(int _iSkill)
{
	if (!m_lpField)
		return;

	if	(g_aSkill[_iSkill].m_bf1IsHideMissImage)
		return;

	SG_MISS		packet;

	packet.base.set(sizeof(SG_MISS),dSG_MISS);
	packet.wActor	=	m_wSerialInField;
	packet.wSkill	=	_iSkill;

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}

//
//	아이템 리로드 한다고 통보~~ -o-
void
cACTOR::sendReloadBeltItem()
{
	SG_RELOAD_BELT_ITEM	packet;

	packet.base.set(sizeof(SG_RELOAD_BELT_ITEM),dSG_RELOAD_BELT_ITEM);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	변신한다.. -o-
void
cACTOR::sendTransformation(int _iReason,BOOL _bIsWantOnlyAfterMotion)
{
	if(m_wRushTime)
		return;
	CG_TRANSFORMATION	packet;

	packet.base.set(sizeof(CG_TRANSFORMATION),dCG_TRANSFORMATION);
	packet.wPosX	=	m_pos.x;
	packet.wPosY	=	m_pos.y;
	
	m_lpField->transformation(&packet,m_wSerialInField,_iReason,_bIsWantOnlyAfterMotion);
}

void
cACTOR::addSendTalk(char *_lpstrTalk,int _iChatType, WORD _wSendTarget)
{
	SG_CHAT	sgChat;

	sgChat.bf11Serial	=	m_wSerialInField;
	sgChat.bf6ChatType	=	_iChatType;

	FillChattingBuffer(&sgChat,m_strName,_lpstrTalk);
			
	if(_wSendTarget == 0xffff)
	{
		m_lpField->addSendPacket((ALL_MSG*)&sgChat,m_wSerialInField);
	}
	else
	{
		int	iSize	=	((cMSG_BASE_TYPE *)&sgChat)->wSize;
		g_userPM.add(_wSendTarget,&sgChat,iSize);
	}
}
//
//	중독 공격 -o-
void
cACTOR::addSendSimpleHit(int _iPhysicalDamage,int _iMagicDamage,WORD _wAddHitType)
{
	SG_SIMPLE_HIT	packet;

	packet.base.set(sizeof(packet),dSG_SIMPLE_HIT);
	packet.wTarget			=	m_wSerialInField;
	packet.iPhysicalDamage			=	max(_iPhysicalDamage,0);
	packet.iMagicDamage				=	max(_iMagicDamage,0);
	packet.bf7AddHitType			=	_wAddHitType;
	
	LONGLONG	llHP		=	m_iHP;
	llHP					=	llHP*10000/getMaxHP();
	packet.bf15RemainHP		=	(WORD)llHP;

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}



void
cACTOR::addSendFirstAidHP()
{
	SG_FIRST_AID	packet;

	packet.base.set(sizeof(packet),dSG_FIRST_AID);
	packet.wSerial		=	m_wSerialInField;
	packet.iFirstAidHP	=	m_iFirstAidHP;

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}

//
//	대미지 입었다.
void
cACTOR::addSendHitDamage(cACTOR *_lpCaster,int _iPhysicalDamage,int _iMagicDamage,DWORD _dwResultField,int _iSkill)
{
	SG_HIT_DAMAGE	packet;

	packet.base.set(sizeof(packet),dSG_HIT_DAMAGE);

	if	(_lpCaster)
		packet.wCaster			=	_lpCaster->m_wSerialInField;

	packet.wTarget			=	m_wSerialInField;
	packet.iPhysicalDamage	=	_iPhysicalDamage;
	packet.iMagicDamage		=	_iMagicDamage;
	packet.bf11Skill		=	_iSkill;
	packet.dwResultField	=	_dwResultField;

	LONGLONG	llHP		=	m_iHP;
	llHP					=	llHP*10000/getMaxHP();
	packet.wRemainHP		=	(WORD)llHP;

	cFIELD	*lpField		=	m_lpField;

	if	(!lpField	&&	_lpCaster)
		lpField				=	_lpCaster->m_lpField;

	if	(lpField)
		lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}

void
cACTOR::addSendSkillEffect(cAbility *_lpAbility,int _iImageType)
{
	SG_SKILL_EFFECT	packet;

	packet.base.set(sizeof(packet),dSG_SKILL_EFFECT);
	packet.wTarget		=	m_wSerialInField;
	packet.wSkill		=	_lpAbility->m_wSkill;
	packet.bf8Level		=	_lpAbility->m_wLevel;
	packet.bf8ImageType	=	_iImageType;

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}

void
cACTOR::addSendCureByPotion(int _iBaseItem,int _iHealHP)
{
	SG_CURE_ACTOR_BY_POTION	packet;

	packet.base.set(sizeof(packet),dSG_CURE_ACTOR_BY_POTION);
	
	packet.wItem		=	_iBaseItem;
	packet.wActor		=	m_wSerialInField;
	packet.iHealPoint	=	_iHealHP;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	외관 장비 모양 정보 전송
void
cACTOR::sendAppearanceInfo()
{
	cItem	*lpWeapon	=	getWeapon();
	cItem	*lpShield	=	getShield();
	cItem	*lpArmor	=	getArmor();

	SG_CHANGE_APPEARANCE	packet;

	packet.base.set(sizeof(packet),dSG_CHANGE_APPEARANCE);

	packet.wSerial	=	m_wSerialInField;

	packet.bf4Weapon=	dVAILD_EQUIPMENT_VALUE_FOR_SAI;
	packet.bf4Shield=	dVAILD_EQUIPMENT_VALUE_FOR_SAI;
	packet.bf4Armor	=	dVAILD_EQUIPMENT_VALUE_FOR_SAI;

	if	(lpWeapon)
	{
		packet.bf4Weapon		=	lpWeapon->getBasicItem(TRUE)->m_wEquippedShape;
		packet.bf8WeaponEffect	=	getWeaponColorizeEffect();
	}
	if	(lpShield)
	{
		packet.bf4Shield		=	lpShield->getBasicItem()->m_wEquippedShape;
//		packet.bShieldEffect	=	getShieldColorizeEffect();
	}
	if	(lpArmor)
	{
		packet.bf4Armor			=	lpArmor->getBasicItem()->m_wEquippedShape;
		packet.bf3ArmorPalette	=	lpArmor->getBasicItem()->m_wPaletteIndex;
	}

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}

//
//	대화 전송
BOOL
cACTOR::sendSpeech(cACTOR *_lpAsker,CSpeech *_lpSpeech)
{
	SG_COMPLEX_SPEECH		packet;

	packet.wOwner					=	m_wSerialInField;//말한넘
	packet.wContentsCount			=	_lpSpeech->m_wContentsCount;//선택문 수
	packet.wSpeechIndex				=	_lpSpeech->m_wConversationIndex;//대화 인덱스
	packet.wDialogBoxSize			=	_lpSpeech->m_wDialogBoxSize;
	packet.wEventArea				=	_lpAsker->m_wEventArea;

	memcpy(packet.strText,_lpSpeech->m_strText,_lpSpeech->m_wLength);

	_lpAsker->m_wSelectConversation	=	_lpSpeech->m_wConversationIndex;
	_lpAsker->m_wTalkNpc			=	m_wSerialInField;

	if (_lpAsker->m_wEventArea		!=	0xffff)
		_lpAsker->m_wTalkNpc		=	_lpAsker->m_wEventArea;

	packet.base.set(sizeof(SG_COMPLEX_SPEECH)-dMAX_SPEECH_LENGTH+_lpSpeech->m_wLength,dSG_COMPLEX_SPEECH);

	g_userPM.add(_lpAsker->m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

//
//	다얄로그 종료 메시지 전송
void
cACTOR::sendEndDialg()
{
	SG_END_DIAOLG	packet;

	packet.base.set(sizeof(SG_END_DIAOLG),dSG_END_DIAOLG);

	packet.wOwner	=	m_wSerialInField;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	아이템의 유니크 시리얼 전송
void
cACTOR::sendItemUniqueSerial(int _iSlot,DWORD _dwSerial)
{
	SG_SET_ITEM_UNIQUE_SERIAL	packet;

	packet.base.set(sizeof(SG_SET_ITEM_UNIQUE_SERIAL),dSG_SET_ITEM_UNIQUE_SERIAL);

	packet.wSlot	=	_iSlot;
	packet.dwSerial	=	_dwSerial;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	특정 아이템 데이터 업데이트
void
cACTOR::sendUpdateItemData(int _iSlot,BOOL _bIsWantTwinkle)
{
	SG_UPDATE_ITEM_DATA	packet;
	cItem				*lpItem	=	NULL;

	packet.base.set(sizeof(SG_UPDATE_ITEM_DATA),dSG_UPDATE_ITEM_DATA);

	packet.wSlot=	_iSlot;
	packet.wIsTwinkle=	_bIsWantTwinkle;

	if	(_iSlot	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
		lpItem	=	&m_aEquip[_iSlot-dBORDER_OF_ITEM_AND_EQUIPMENT];
	else
	if	(_iSlot	>=	dOWN_ITEM_COUNT)
		lpItem	=	&m_aExtraInventory[_iSlot-dOWN_ITEM_COUNT];
	else
		lpItem	=	&m_aItems[_iSlot];

	if	(lpItem->m_wBaseItem==	0xffff)
		packet.wIsTwinkle	=	FALSE;

	memcpy(&packet.item,lpItem,sizeof(cItem));

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendUpdateItemDurability(int _iPart)
{
	SG_UPDATE_ITEM_DURABILITY	packet;

	packet.base.set(sizeof(packet),dSG_UPDATE_ITEM_DURABILITY);

	packet.bPart		=	(BYTE)_iPart;
	packet.bDurability	=	m_aEquip[_iPart].m_bDurability;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendUpdateWayPointInfo(int _iField,int _iWaypointSlot,int _iXPos,int _iYPos)
{
	SG_UPDATE_WAY_POINT_INFO	packet;

	packet.base.set(sizeof(packet),dSG_UPDATE_WAY_POINT_INFO);

	packet.wField			=	_iField;
	packet.wXPos			=	_iXPos;
	packet.wYPos			=	_iYPos;
	packet.wWayPointSlot	=	_iWaypointSlot;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendMoveState()
{
	SG_SET_MOVE_ABILITY	packet;

	packet.base.set(sizeof(packet),dSG_SET_MOVE_ABILITY);

	packet.wPPS				=	m_wPPS;
	packet.isRun			=	m_isRunning;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	
void
cACTOR::sendHitAction()
{
	SG_HIT_ACTION	packet;

	packet.base.set(sizeof(packet),dSG_HIT_ACTION);

	packet.wActor	=	m_wSerialInField;

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}

//
//	타이틀 습득
void
cACTOR::sendGetTitleMessage(int _iIndex,int _iTitle,int _iTitleLevel)
{
	SG_GET_TITLE	packet;

	packet.base.set(sizeof(packet),dSG_GET_TITLE);
	packet.wSlot	=	_iIndex;
	packet.bTitle	=	_iTitle;
	packet.bLevel	=	_iTitleLevel;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	타이틀 제거
void
cACTOR::sendRemoveTitleMessage(int _iIndex,int _iTitle,int _iTitleLevel)
{
	SG_REMOVE_TITLE	packet;

	packet.base.set(sizeof(packet),dSG_REMOVE_TITLE);
	packet.wSlot	=	_iIndex;
	packet.bTitle	=	_iTitle;
	packet.bLevel	=	_iTitleLevel;
	
	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	길드 생성 준비
BOOL
cACTOR::sendReadyToMakeGuild()
{
	SG_READY_TO_MAKE_GUILD	packet;

	memset(packet.strMemberList,0,sizeof(packet.strMemberList));

	cParty	party;

	packet.base.set(sizeof(packet),dSG_READY_TO_MAKE_GUILD);

	if (g_pPartyManager->getParty(&party,getPartySerial()))
	{
		for (int i=0;i<(int)party.getMemberCount();i++)
			strcpy(packet.strMemberList[i],party.m_aMember[i].m_strName);
	}
	else
		strcpy(packet.strMemberList[0],m_strName);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

//
//	길드전 스케쥴러 열기
BOOL
cACTOR::sendOpenGuildBattleScheduler()
{
	SG_OPEN_GUILD_BATTLE_SCHEDULER	packet;

	packet.base.set(sizeof(packet),dSG_OPEN_GUILD_BATTLE_SCHEDULER);

	packet.wBeginHour	=	g_iGuildBattleBeginTime/60;
	packet.wBeginMinute	=	g_iGuildBattleBeginTime%60;
	packet.wBattlePeriod=	g_iGuildBattlePeriod;
	packet.wBattleCount	=	dGUILD_BATTLE_COUNT;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

//
//	길드 마크 조합 인터페이스 오픈
BOOL
cACTOR::sendOpenGuildMarkComposer()
{
	SG_OPEN_GUILD_MARK_COMPOSER	packet;

	packet.base.set(sizeof(packet),dSG_OPEN_GUILD_MARK_COMPOSER);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

//
//	길드전 스케쥴 변경
void
cACTOR::sendChangeGuildBattleSchedule(int _iTimeOrder,int _iFieldOrder,CGuildBattleInfo *_lpInfo)
{
	SG_CHANGE_GUILD_BATTLE_SCHEDULE	packet;
	CTimeInfo	beginTime;

	packet.base.set(sizeof(packet),dSG_CHANGE_GUILD_BATTLE_SCHEDULE);

	g_gbsToday.getBeginTime(_iTimeOrder,&beginTime);

	packet.wTimeOrder				=	_iTimeOrder;
	packet.wFieldOrder				=	_iFieldOrder;

	packet.info.m_astrGuild[0][0]	=	NULL;
	packet.info.m_astrGuild[1][0]	=	NULL;
	packet.info.m_awBattleGuild[0]	=	_lpInfo->m_awBattleGuild[0];
	packet.info.m_awBattleGuild[1]	=	_lpInfo->m_awBattleGuild[1];
	packet.info.m_wFieldSerial		=	_lpInfo->m_wFieldSerial;

	CGuild	*lpGuild1	=	g_guildManager.getBySerial(_lpInfo->m_awBattleGuild[0]);
	CGuild	*lpGuild2	=	g_guildManager.getBySerial(_lpInfo->m_awBattleGuild[1]);

	if	(lpGuild1)
		strcpy(packet.info.m_astrGuild[0],lpGuild1->m_strName);
	if	(lpGuild2)
		strcpy(packet.info.m_astrGuild[1],lpGuild2->m_strName);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	길드 참가 결과 전송
void
cACTOR::sendJoinGuildResult(int _iResult)
{
	SG_ASK_JOIN_GUILD_RESULT	packet;

	packet.base.set(sizeof(packet),dSG_ASK_JOIN_GUILD_RESULT);
	packet.wResult		=	_iResult;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendGuildWorkMessage(int _iMessage,int _iValue1,int _iValue2,char *_lpstrName)
{
	SG_GUILD_WORK_MESSAGE	packet;

	packet.base.set(sizeof(packet),dSG_GUILD_WORK_MESSAGE);
	packet.wMessage	=	_iMessage;

	packet.wValue1	=	_iValue1;
	packet.wValue2	=	_iValue2;
	
	if	(_lpstrName)
		strcpy(packet.strName,_lpstrName);
	else
		packet.base.wSize	-=	sizeof(packet.strName);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendChangeGuildTaxRate(int _iTax)
{
	SG_CHANGE_GUILD_TAX_RATE	packet;	

	packet.base.set(sizeof(packet),dSG_CHANGE_GUILD_TAX_RATE);
	packet.wTaxRate	=	_iTax;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendChangeGuildNotice(char *_lpstrNotice)
{
	SG_CHANGE_GUILD_NOTICE		packet;	

	packet.base.set(sizeof(packet),dSG_CHANGE_GUILD_NOTICE);
	strcpy(packet.strGuildNotice,_lpstrNotice);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendChangeRankInGuild(int _iRank)
{
	SG_CHANGE_RANK_IN_GUILD		packet;	

	packet.base.set(sizeof(packet),dSG_CHANGE_RANK_IN_GUILD);
	packet.wRank	=	_iRank;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	스킬 레벨 업데이트 메시지 전송
void
cACTOR::sendUpdateGuildSkillLevel(int _iSkill,int _iLevel,int _iRemainSkillPoint)
{
	SG_UPDATE_GUILD_SKILL_LEVEL	packet;

	packet.base.set(sizeof(packet),dSG_UPDATE_GUILD_SKILL_LEVEL);

	packet.wGuildSerial		=	m_wGuildSerial;
	packet.wSkill			=	_iSkill;
	packet.wLevel			=	_iLevel;
	packet.wRemainSkillPoint=	_iRemainSkillPoint;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	이번주에 길드전 횟수 업데이트
void
cACTOR::sendUpdateGuildBattleCountAtThisWeek(int _iCount)
{
}

//
//	경험치 얻었다.
void
cACTOR::sendUpdateExp(BOOL _bIsRestore)
{
	SG_UPDATE_EXP	sgUpdateExp;

	sgUpdateExp.base.set(sizeof(SG_UPDATE_EXP),dSG_UPDATE_EXP);
	sgUpdateExp.wSkillExp	=	(WORD)m_dwSkillExperience;
	sgUpdateExp.iExp		=	m_iExperience;
	sgUpdateExp.bf10Level	=	m_iLevel;
	sgUpdateExp.bf1IsRestore=	_bIsRestore;

	g_userPM.add(m_iClientSerial,&sgUpdateExp,sgUpdateExp.base.wSize);
}

//
//	경험치 얻었다.
void
cACTOR::sendUpdatePetExp(int _iPetIndex)
{
	SG_UPDATE_PET_EXP	sgUpdateExp;

	cACTOR	*lpPet			=	getPetActor(_iPetIndex);

	sgUpdateExp.base.set(sizeof(SG_UPDATE_PET_EXP),dSG_UPDATE_PET_EXP);
	sgUpdateExp.iExp		=	(int)m_aPet[_iPetIndex].m_dwExperience;
	sgUpdateExp.bf4PetIndex	=	_iPetIndex;
	sgUpdateExp.bf10Level	=	m_aPet[_iPetIndex].m_bf10Level;

	g_userPM.add(m_iClientSerial,&sgUpdateExp,sgUpdateExp.base.wSize);
}

void
cACTOR::sendRemovePet(int _iPetIndex,BOOL _bIsMiniPet,BOOL _bIsRequireMessage)
{
	SG_PARTING_WITH_PET	packet;

	packet.base.set(sizeof(SG_PARTING_WITH_PET),dSG_PARTING_WITH_PET);
	packet.wPetIndex	=	_iPetIndex;
	packet.wIsMiniPet	=	_bIsMiniPet;
	packet.bf1IsRequireMessage	=	_bIsRequireMessage;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	CP 충전 메시지
void
cACTOR::sendChargeCP(int _iSkill)
{
	m_lpField->sendChargeCP(this,m_iCP,_iSkill);

	addPacketStop();
}

//
//	퀘스트 정보 전송
void
cACTOR::sendQuestInfo()
{
	SG_QUEST_INFO	packet;

	packet.base.set(sizeof(packet),dSG_QUEST_INFO);

	memcpy(packet.m_aProcessQuest,m_aProcessQuest,sizeof(m_aProcessQuest));

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	퀘스트 정보 전송
void
cACTOR::sendQuestChangeInfo(int _iProcessIndex,BOOL _bIsForQuestComplete)
{
	SG_CHANGE_QUEST_INFO	packet;
	packet.base.set(sizeof(packet),dSG_CHANGE_QUEST_INFO);

	packet.bf4ProcessSlot		=	_iProcessIndex;
	packet.bf1IsForQuestComplete=	_bIsForQuestComplete;
	memcpy(&packet.processQuest,&m_aProcessQuest[_iProcessIndex],sizeof(tsProcessQuestField));
	packet.wCountDownTime		=	m_awQuestCountDownTimer[_iProcessIndex];

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	아이템 수 감소
void
cACTOR::sendDecreaseItemCount(int _iSlot,int _iCount)
{
	SG_DECREASE_ITEM_COUNT	packet;

	packet.base.set(sizeof(packet),dSG_DECREASE_ITEM_COUNT);
	packet.wItemSlot	=	_iSlot;
	packet.wCount		=	_iCount;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	아이템 수 감소
void
cACTOR::sendAddItem(cItem *_lpItem,int _iReason)
{
	SG_ADD_ITEM	packet;

	packet.base.set(sizeof(packet),dSG_ADD_ITEM);
	memcpy(&packet.item,_lpItem,sizeof(cItem));
	packet.wReason	=	_iReason;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	아이템 사라짐
void
cACTOR::sendRemoveItem(int _iItem,int _iCount,BOOL _bIsSlot,int _iReason,BOOL _bIsIncludeEquipment)
{
	SG_REMOVE_ITEM	packet;

	packet.base.set(sizeof(packet),dSG_REMOVE_ITEM);
	packet.wItem	=	_iItem;
	packet.bf14Count=	_iCount;
	packet.bf1IsSlot=	_bIsSlot;
	packet.bf1IsIncludeEquipment	=	_bIsIncludeEquipment;
	packet.wReason	=	_iReason;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	돈 더해주기
void
cACTOR::sendAddGold(int _iGold,int _iReason)
{
	SG_ADD_GOLD	packet;

	packet.base.set(sizeof(packet),dSG_ADD_GOLD);
	packet.iAddGold	=	_iGold;
	packet.wReason	=	_iReason;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	클라이언트에게 메시지 전송
void
cACTOR::sendServerMessage(char *_lpstrMessage,int _iType)
{
	SG_MESSAGE	packet;

	packet.base.set(sizeof(packet),dSG_MESSAGE);
	strcpy(packet.strMessage,_lpstrMessage);
	packet.base.wSize	=	sizeof(packet) - dSERVER_MESSAGE_SIZE + strlen(_lpstrMessage)+1;
	packet.wType		=	_iType;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	
void
cACTOR::sendRegistServerMessage(int _iMessage,int _iType)
{
	SG_REGIST_MESSAGE	packet;

	packet.base.set(sizeof(packet),dSG_REGIST_MESSAGE);
	packet.wMessage	=	_iMessage;
	packet.wType	=	_iType;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}


void
cACTOR::sendRemainHP(cACTOR *_lpReceiver)
{
	SG_REMAIN_HP	packet;

	packet.base.set(sizeof(packet),dSG_REMAIN_HP);

	LONGLONG	llMaxHP		=	getMaxHP();
	LONGLONG	llHP		=	m_iHP;

	llHP					=	llHP*60000/llMaxHP;
	packet.iMaxHP			=	getMaxHP();
	packet.wRemainHPRate	=	(WORD)llHP;
	packet.wActor			=	m_wSerialInField;

	packet.bf1IsSyncUnionParty=	isUnionPartyMember();		// 수정
	packet.bf1IsGuildObject		=	m_bIsGuildObject;		// 길드오브젝트.	

	if	(m_iHP > 0)
		packet.wRemainHPRate	=	max(packet.wRemainHPRate,1);

	g_userPM.add(_lpReceiver->m_iClientSerial,&packet,packet.base.wSize);
}


//
//	뭔가 하다가 부딪혀서 막혔다.
void
cACTOR::sendBlockedMessage()
{
	SG_STOP	packet;

	packet.base.set(sizeof(SG_STOP),dSG_STOP);

	packet.bf12Serial	=	m_wSerialInField;
	packet.bf3Direct	=	m_wDirect;	//	방향
	packet.bf1IsChangeOwnerDirect	=	TRUE;
	packet.wPosX		=	m_pos.x;
	packet.wPosY		=	m_pos.y;			//	현재 위치

	if (m_wSlaveActor != 0xffff)
	{
		packet.wPosX=	m_posFeignDeath.x;
		packet.wPosY=	m_posFeignDeath.y;
	}

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//
void
cACTOR::addSendActorMagicEffectStatus(BOOL _isByGGG , BOOL _isEnd)
{
	SG_ACTOR_ENCHANTED_STATUS	packet;

	packet.base.set(sizeof(SG_ACTOR_ENCHANTED_STATUS),dSG_ACTOR_ENCHANTED_STATUS);

	packet.dwMask				=	m_dwEnchantedMask;
	packet.wItemEffect			=	m_wItemEffect;
	packet.wActor				=	m_wSerialInField;
	packet.bf7MagicCarpetShape	=	m_bf5MagicCarpetShape;
	packet.bf3CurrentDuelBuff	=	m_wDuelBuffType;
	if(m_GGG.m_wGGGTimeForSkillUp && m_bf3GreateGodGrace)
		packet.bf3CurrentGGGBuff	=	m_bf3GreateGodGrace;
	else
		packet.bf3CurrentGGGBuff	=	0;

	if(_isByGGG && !_isEnd)
	{
		packet.bf3CurrentDuelBuff	=	m_bf3GreateGodGrace;
	}

	packet.enchantedImage.copy(&m_enchantedImage);

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}

//
//	텔레포트 패킷 전송
void
cACTOR::sendTeleport(int _iSkill,int _iMethod)
{
	SG_TELEPORT	packet;

	packet.base.set(sizeof(SG_TELEPORT),dSG_TELEPORT);

	packet.wActor	=	m_wSerialInField;
	packet.wSkill	=	_iSkill;
	packet.wX		=	m_pos.x;
	packet.wY		=	m_pos.y;
	packet.wMethod	=	_iMethod;

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}

//
//	스킬 부가효과 적용 하라고 전송
void
cACTOR::sendApplySkillExtraEffect(int _iCaster,cAbility *_lpAbility, DWORD _dwKeepTime)
{ // 마지막 수정일 : 09.10.14
	SG_APPLY_SKILL_EXTRA_EFFECT	packet;

	packet.base.set(sizeof(SG_APPLY_SKILL_EXTRA_EFFECT),dSG_APPLY_SKILL_EXTRA_EFFECT);

	packet.wActor		=	_iCaster;
	packet.wSkill		=	_lpAbility->m_wSkill;
	packet.wLevel		=	_lpAbility->m_wLevel;
	packet.dwKeepTime	=	_dwKeepTime;			// 강제로 원하는 버프 지속시간을 적용시킨다.
	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}
//
//	유저 찾기 결과
void
cACTOR::sendFindUserResult(char *_lpstrTargetName,int _iResult,int _iFieldSerial,int _iJob)
{
	SG_FIND_USER_RESULT	packet;

	packet.base.set(sizeof(SG_FIND_USER_RESULT),dSG_FIND_USER_RESULT);

	packet.wResult	=	_iResult;
	packet.wField	=	_iFieldSerial;
	packet.wJob		=	_iJob;

	strcpy(packet.strTargetName,_lpstrTargetName);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	이벤트 메시지 전송
void
cACTOR::sendEventMessage(char *_lpstrText,BOOL _bIsIncludePartyMember,int _iChatType, int _iTime , BOOL _bEffectflickeringly)
{
	SG_EVENT_NOTICE		packet;

	packet.base.set(sizeof(SG_EVENT_NOTICE),dSG_EVENT_NOTICE);
	packet.wNoticeType	=	_iChatType;
	packet.wTime		=	_iTime;
	packet.wEffectflickeringly	=	_bEffectflickeringly;

	int	iTextLimit		=	sizeof(packet.strMessage)-2;

	strncpy(packet.strMessage,_lpstrText,iTextLimit);
	
	packet.strMessage[iTextLimit]	=	NULL;

	if	(_bIsIncludePartyMember && getPartySerial() != 0xffff)
	{
		cParty	party;

		if	(g_pPartyManager->getParty(&party,getPartySerial()))
		{
			for (int i=0;i<(int)party.getMemberCount();i++)
			{
				cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

				if	(!lpMember)
					continue;

				g_userPM.add(lpMember->m_iClientSerial,&packet,packet.base.wSize);
			}
		}

		return;
	}

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	길드가 해체 되었어요.
void
cACTOR::sendDissolutionGuildMessage()
{
	SG_DISSOLUTION_GUILD	packet;

	packet.base.set(sizeof(packet),dSG_DISSOLUTION_GUILD);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	길드 레벨 업데이트(뤠벨업!!)
void
cACTOR::sendUpdateGuildLevel(int _iLevel,DWORD _dwExp,int _iGuildPoint,int _iExpTaxRate)
{
	SG_UPDATE_GUILD_LEVEL	packet;

	packet.base.set(sizeof(packet),dSG_UPDATE_GUILD_LEVEL);
	packet.wGuildLevel	=	_iLevel;
	packet.dwGuildExp	=	_dwExp;
	packet.wGuildPoint	=	_iGuildPoint;
	packet.wExpTaxRate	=	_iExpTaxRate;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendSimpleGuildInfo()
{
	CGuild	*lpGuild	=	g_guildManager.getBySerial(m_wGuildSerial);

	SG_SIMPLE_GUILD_INFO	packet;

	packet.base.set(sizeof(packet),dSG_SIMPLE_GUILD_INFO);

	if	(lpGuild && lpGuild->getMember(m_strName))
	{
		memcpy(&packet.guildMarkInfo,&lpGuild->m_guildMarkInfo,sizeof(CGuildMarkInfo));
		packet.bf3HallLevel	=	(BYTE)lpGuild->m_wHallLevel;
		packet.bf1IsTwinkle	=	(BYTE)lpGuild->m_bf1IsTwinkle;
		packet.bf8Level		=	(BYTE)lpGuild->m_wLevel;
		strcpy(packet.strGuildName,lpGuild->m_strName);

		int	iCount	=	0,i;

		for (i=dMAX_GUILD_SKILL_COUNT-1;i>=0;i--)
		{
			if	(lpGuild->m_abGuildSkill[i])
			{
				iCount	=	i+1;
				break;
			}
		}

		for (i=0;i<iCount;i++)
			packet.abGuildSkill[i]	=	(BYTE)lpGuild->m_abGuildSkill[i];

		packet.wGuildSkillCount		=	iCount;
		packet.wGuildSerial			=	lpGuild->m_wSerial;
		packet.base.wSize			=	sizeof(packet)-sizeof(packet.abGuildSkill)+iCount;
	}
	else
	{
		lpGuild						=	NULL;
		m_wGuildSerial				=	0xffff;
		m_wGuildRank				=	0;
		packet.wGuildSerial			=	0xffff;
		packet.base.wSize			=	sizeof(cMSG_BASE_TYPE)+sizeof(WORD)+sizeof(packet.abGuildStatueStatus);
	}

	memset(packet.abGuildStatueStatus,0,sizeof(packet.abGuildStatueStatus));

	if	(m_lpField->m_wIsGuildHall)
	{
		CGuild	*lpOwnGuild	=	lpGuild;

		if	(m_lpField->m_bIs1LevelGuildHall	==	FALSE)
			lpOwnGuild	=	m_lpField->getHallOwnGuild();

		if	(lpOwnGuild)
		{
			int	iSkill	=	0;

			for (int i=0;i<dMAX_GUILD_SKILL_COUNT;i++)
			{
				if	(g_gsManager.m_aSkills[i].m_wSerial	==	0xffff)
					continue;

				if	(g_gsManager.m_aSkills[i].m_bf1IsStatueSkill)
				{
					int	iSlot	=	iSkill/5;
					int	iMask	=	(1<<(iSkill%5));

					if	(lpOwnGuild->m_abGuildSkill[i])
						packet.abGuildStatueStatus[iSlot]	+=	iMask;

					iSkill++;
				}
			}
		}
	}

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendGuildInfo(BOOL _bIsIgnoreSkill)
{
	SG_BASE_GUILD_INFO	packet;

	packet.base.set(sizeof(packet),dSG_BASE_GUILD_INFO);
	packet.wGuildSerial		=	m_wGuildSerial;

	if	(m_wGuildSerial	==	0xffff)
		packet.base.wSize	=	sizeof(packet)-sizeof(CGuildBaseInfo);
	else
	{
		CGuild	*lpGuildInfo	=	g_guildManager.getBySerial(m_wGuildSerial);

		if	(lpGuildInfo)
		{
			memcpy(&packet.guildInfo,lpGuildInfo,sizeof(CGuildBaseInfo));

			if (_bIsIgnoreSkill)
				packet.base.wSize	-=	sizeof(lpGuildInfo->m_abGuildSkill);
		}
		else
		{
			packet.wGuildSerial	=	0xffff;
			packet.base.wSize	=	sizeof(packet)-sizeof(CGuildBaseInfo);
		}
	}

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	멤버를 위한 길드전 정보 전송
void
cACTOR::sendGuildBattleInfoForMember(BOOL _bIsByClientRequest)
{
	SG_UPDATE_GUILD_BATTLE_INFO_FOR_MEMBER	packet;

	packet.base.set(sizeof(packet),dSG_UPDATE_GUILD_BATTLE_INFO_FOR_MEMBER);

	CGuild	*lpGuild	=	getGuild();

	if	(!lpGuild)	
		return;

	lpGuild->getGuildBattleInfoForMember(&packet.data);

	if	(_bIsByClientRequest	&&	packet.data.m_bIGB_Hour	==	0xff	&&	packet.data.m_bBSW_Hour	==	0xff)
		return;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendDeathPeneltySecond(BOOL _bIsDeathCurrentTime,BOOL _bIsRecoverDeathPeleltyTime)
{
	SG_DEATH_PENELTY_SECOND	packet;

	packet.base.set(sizeof(packet),dSG_DEATH_PENELTY_SECOND);

	packet.bf14PeneltySecond	=	m_bf12DeathPeneltySecond;
	packet.bf1IsDeath			=	_bIsDeathCurrentTime;
	packet.bf1IsRecover			=	_bIsRecoverDeathPeleltyTime;
	packet.wDeathPenelty		=	m_wCurrentLevelDeathPenelty;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendUseOrbResult(int _iResult,int _iSlot)
{
	SG_USE_ORB_RESULT	packet;

	packet.base.set(sizeof(packet),dSG_USE_ORB_RESULT);
	packet.wResult			=	_iResult;

	if (_iSlot	>=	0)
	{
		packet.wSlot			=	_iSlot;
		packet.wRememberField	=	m_aWayPoint[_iSlot].m_bf12Field;
		packet.wRememberXPos	=	m_aWayPoint[_iSlot].m_bf10XPos;
		packet.wRememberYPos	=	m_aWayPoint[_iSlot].m_bf10YPos;
	}

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendMoveFieldResult(int _iResult, char *_lpstrAddress,char *_lpstrMapname,int _iInstanceFieldFloor,int _iFieldSerial,int _iGuild)
{	// 마지막 수정일 : 09.10.07
	SG_MOVE_FIELD_RESULT	packet;

	packet.base.set(sizeof(SG_MOVE_FIELD_RESULT),dSG_MOVE_FIELD_RESULT);
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.strGuildName);

	int	iGuildHallLevel	=	0;
	int	iGuildHallShape	=	0;

	if	(_lpstrAddress)
		strcpy(packet.strIP,_lpstrAddress);
	if	(_lpstrMapname)
		strcpy(packet.strMapName,_lpstrMapname);

	if	(_iGuild	!=	0xffff)
	{
		CGuild	*lpGuild	=	g_guildManager.getBySerial(_iGuild);

		if	(lpGuild)
		{
			iGuildHallLevel		=	lpGuild->m_wHallLevel;
			iGuildHallShape		=	lpGuild->m_wGuildHallShape;
			
			if	(lpGuild->m_wHallLevel	==	1	||	lpGuild->m_wGuildHallShape	==	0xffff)
				iGuildHallShape		=	0;

			strcpy(packet.strGuildName,lpGuild->m_strName);
			packet.base.wSize	=	sizeof(packet);

			sprintf(packet.strMapName,"GH%d_%.2d.rmd",iGuildHallLevel,iGuildHallShape);
		}
	}
	
	packet.wResult			=	_iResult;
	packet.wFloor			=	_iInstanceFieldFloor;
	packet.wFieldSerial		=	_iFieldSerial;
	packet.bf4GuildHallLevel	=	iGuildHallLevel;

	packet.bGuildHallShape	=	iGuildHallShape;
	cFIELD* lpField = g_game.getFieldByUniqueSerial(_iFieldSerial);
	WORD	wWeather = eWEATHER_FINE;
	if(lpField)
	{
		wWeather = lpField->m_wWeather;
	}
	packet.bf4Weather		= wWeather;
	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}
void
cACTOR::sendMoveFieldNow()
{
	SG_MOVE_FIELD_NOW	packet;

	packet.base.set(sizeof(SG_MOVE_FIELD_NOW),dSG_MOVE_FIELD_NOW);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendCartMessage(int _iMessage)
{
	SG_CART_MESSAGE		packet;

	packet.base.set(sizeof(packet),dSG_CART_MESSAGE);
	packet.wCartMessage	=	_iMessage;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

int
cACTOR::sendPitchmanShopMessage(int _iMessage)
{
	SG_PITCHMAN_SHOP_MESSAGE		packet;

	packet.base.set(sizeof(packet),dSG_PITCHMAN_SHOP_MESSAGE);
	packet.wMessage	=	_iMessage;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	_iMessage;
}

void
cACTOR::sendWithdrawItemFromCart(int _iInventorySlot,int _iCartSlot)
{
	SG_WITHDRAW_ITEM_FROM_THE_CART	packet;

	packet.base.set(sizeof(packet),dSG_WITHDRAW_ITEM_FROM_THE_CART);

	cItem	*lpItem			=	getInventoryItem(_iInventorySlot);
	
	if	(!lpItem)
		return;

	packet.wCartSlot		=	_iCartSlot;
	packet.wInventorySlot	=	_iInventorySlot;

	memcpy(&packet.item,lpItem,sizeof(cItem));

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	카트 아이템 제거
void
cACTOR::sendDestroyCartItemResult(int _iResult,int _iCartSlot)
{
	SG_DESTROY_CART_ITEM_RESULT	packet;

	packet.base.set(sizeof(packet),dSG_DESTROY_CART_ITEM_RESULT);

	packet.wResult			=	_iResult;
	packet.wCartSlot		=	_iCartSlot;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	
void
cACTOR::sendLoadFailedCartPremiumItem(cItem *_lpItem,int _iInventorySlot)
{
	SG_LOAD_FAILED_CART_PREMIUM_ITEM	packet;

	packet.wInventorySlot	=	_iInventorySlot;
	memcpy(&packet.item,_lpItem,sizeof(cItem));

	packet.base.set(sizeof(packet),dSG_LOAD_FAILED_CART_PREMIUM_ITEM);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendGameClose(char *_lpstrCause)
{
	SG_OUT	packet;

	packet.base.set(sizeof(packet),dSG_OUT);

	strcpy(packet.strCause,_lpstrCause);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendUpdateBoostCount()
{
	SG_UPDATE_BOOST_COUNT	packet;

	packet.base.set(sizeof(packet),dSG_UPDATE_BOOST_COUNT);

	packet.bf3UseBoostCount		=	m_bf3UseBoostCount;
	packet.bf5RemainPet1BoostCount	=	m_aPet[0].m_bf5BoostCount;
	packet.bf5RemainPet2BoostCount	=	m_aPet[1].m_bf5BoostCount;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//	아이템 사기 실패 메시지를 플레이어에게 전송
void
cACTOR::sendFailedBuyItem(int _iReason)
{
	SG_FAILED_BUY_ITEM	packetFailed;

	packetFailed.base.set(sizeof(SG_FAILED_BUY_ITEM),dSG_FAILED_BUY_ITEM);
	packetFailed.wReason	=	_iReason;
	packetFailed.iGold		=	m_iGold;

	g_userPM.add(m_iClientSerial,&packetFailed,packetFailed.base.wSize);
}

//
//	
void
cACTOR::sendExpiredItem(int _iSlot,int _iItem)
{
	SG_EXPIRED_ITEM	packet;

	packet.base.set(sizeof(SG_EXPIRED_ITEM),dSG_EXPIRED_ITEM);
	packet.wSlot	=	_iSlot;
	packet.wItem	=	_iItem;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//
//!	아이템 데이터 전송
void
cACTOR::sendItemData()
{
	if	(isTooFastSendBigSizeData())
		return;

	setLastTimeSendBigSizeData(g_dwCurrentTime);

	SG_ITEM_DATA	packet;

	packet.base.set(sizeof(SG_ITEM_DATA),dSG_ITEM_DATA);
	packet.iGold				=	m_iGold;
	packet.wCount				=	0;
	packet.wExtraInventorySize	=	m_bf6ExtraInventorySize;
	
	m_bf6ExtraInventorySize		=	min(m_bf6ExtraInventorySize,dEXTRA_INVENTORY_SIZE);

	int		i;

	for (i=0;i<dEQUIPMENT_PART_COUNT+dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE;i++)
		packet.m_aItems[i].reset();

	for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
	{
		if (m_aEquip[i].m_wBaseItem	==	0xffff)
			continue;
			
		memcpy(&packet.m_aItems[packet.wCount],&m_aEquip[i],sizeof(CItemDefine));
		packet.m_aItems[packet.wCount].m_wSlot	=	i+dBORDER_OF_ITEM_AND_EQUIPMENT;
		packet.wCount++;
	}

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		if (m_aItems[i].m_wBaseItem	==	0xffff)
			continue;

		memcpy(&packet.m_aItems[packet.wCount],&m_aItems[i],sizeof(CItemDefine));
		packet.m_aItems[packet.wCount].m_wSlot	=	i;
		packet.wCount++;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		if (m_aExtraInventory[i].m_wBaseItem	==	0xffff)
			continue;

		memcpy(&packet.m_aItems[packet.wCount],&m_aExtraInventory[i],sizeof(CItemDefine));
		packet.m_aItems[packet.wCount].m_wSlot	=	i+dOWN_ITEM_COUNT;
		packet.wCount++;
	}

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.m_aItems)+packet.wCount*sizeof(cItemDataForRevise);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendUnionPartyDissolution(int _iCause)
{
	SG_UNION_PARTY_WORK			packet;
	ZeroMemory(&packet , sizeof(packet));
	
	packet.base.set(sizeof(packet),dSG_DISSOLUTION_UNION_PARTY);

	//0 번은 이유		모르면 걍 깨진거지.
	packet.wValue[0] = _iCause;
	
	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendDissolutionPartyMessage(int _iCause)
{
	SG_DISSOLUTION_PARTY	packet;

	packet.base.set(sizeof(packet),dSG_DISSOLUTION_PARTY);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendDisplacement(cACTOR *_lpTarget,BOOL _bIsDisplacement)
{
	SG_DISPLACEMENT		packet;

	packet.wIsDisplacement	=	_bIsDisplacement;
	
	if (_lpTarget)
		packet.wTarget		=	_lpTarget->m_wSerialInField;
	else
		packet.wTarget		=	0xffff;

	packet.base.set(sizeof(packet),dSG_DISPLACEMENT);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::addSendVerySimpleInfo(cACTOR *_lpReceiver)
{
	SG_VERY_SIMPLE_ACTOR_INFO	packet;
	int	iBufferSize		=	0;

	if (m_wActorKind	==	eAK_PLAYER)
	{
		CVerySimplePlayerInfo	*lpSimpleData	=	(CVerySimplePlayerInfo *)packet.aBuffer;

		getVerySimplePlayerInfo(lpSimpleData);

		iBufferSize	=	sizeof(CVerySimplePlayerInfo);
	}
	else
	if (m_wActorKind	==	eAK_MONSTER)
	{
		CVerySimpleMonsterInfo	*lpSimpleData	=	(CVerySimpleMonsterInfo	*)packet.aBuffer;

		getVerySimpleMonsterInfo(lpSimpleData);

		iBufferSize	=	sizeof(CVerySimpleMonsterInfo);
	}
	else
	{
		CVerySimpleNpcInfo	*lpSimpleData	=	(CVerySimpleNpcInfo*)packet.aBuffer;

		getVerySimpleNpcInfo(lpSimpleData);

		iBufferSize	=	sizeof(CVerySimpleNpcInfo);
	}

	packet.base.set(sizeof(packet)-sizeof(packet.aBuffer)+iBufferSize,dSG_VERY_SIMPLE_ACTOR_INFO);

	if	(_lpReceiver)
		g_userPM.add(_lpReceiver->m_iClientSerial,&packet,packet.base.wSize);
	else
		m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}

void
cACTOR::addSendSimpleInfo(cACTOR *_lpReceiver,BOOL _bIsSummonBeastPowerUp,BOOL _bIsSyncHeroInfo)
{
	SG_SIMPLE_ACTOR_INFO	packet;

	int		iBufferPoint=	0;

	if (m_wActorKind	==	eAK_PLAYER)
	{
		CSimplePlayerInfo	*lpSimpleData	=	(CSimplePlayerInfo *)&packet.aBuffer[iBufferPoint];

		getSimplePlayerInfo(lpSimpleData);
		
		lpSimpleData->bf1IsSyncHeroInfo	=	_bIsSyncHeroInfo;

		iBufferPoint	=	iBufferPoint+sizeof(CSimplePlayerInfo) - sizeof(lpSimpleData->strName) + strlen(lpSimpleData->strName)+1;
	}
	else
	if (m_wActorKind	==	eAK_MONSTER)
	{
		CSimpleMonsterInfo	*lpSimpleData	=	(CSimpleMonsterInfo	*)&packet.aBuffer[iBufferPoint];

		getSimpleMonsterInfo(lpSimpleData);

		if (isSummonBeast())
			lpSimpleData->bf1IsRegen	=	_bIsSummonBeastPowerUp;

		iBufferPoint	=	iBufferPoint+sizeof(CSimpleMonsterInfo) - sizeof(lpSimpleData->strName) + strlen(lpSimpleData->strName)+1;
	}
	else
	{
		CSimpleNpcInfo	*lpSimpleData	=	(CSimpleNpcInfo	*)&packet.aBuffer[iBufferPoint];

		getSimpleNpcInfo(lpSimpleData);

		iBufferPoint	=	iBufferPoint+sizeof(CSimpleNpcInfo) - sizeof(lpSimpleData->strName) + strlen(lpSimpleData->strName)+1;
	}
	
	packet.base.set(sizeof(packet)-sizeof(packet.aBuffer)+iBufferPoint,dSG_SIMPLE_ACTOR_INFO);

	if	(_lpReceiver)
		g_userPM.add(_lpReceiver->m_iClientSerial,&packet,packet.base.wSize);
	else
		m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}

void
cACTOR::addSendRevive()
{
	SG_REVIVE	packet;

	packet.base.set(sizeof(packet),dSG_REVIVE);
	packet.wSerial	=	m_wSerialInField;

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}

void
cACTOR::sendPetInfo(int _iPetIndex,int _iPetFieldSerial,BOOL _bIsTame)
{
	SG_PET_INFO	packet;

	packet.base.set(sizeof(SG_PET_INFO),dSG_PET_INFO);

	packet.bf4PetIndex	=	_iPetIndex;
	packet.bf1IsTame	=	_bIsTame;
	packet.wPetSerial	=	_iPetFieldSerial;

	cACTOR	*lpPet		=	getPetActor(_iPetIndex);

	if (lpPet)
	{
		packet.wMaxAttackPower		=	lpPet->getPhysicalAttackPower(TRUE)/100;
		packet.wMinAttackPower		=	lpPet->getPhysicalAttackPower(FALSE)/100;
		packet.iDefensePower		=	lpPet->getDefensivePower()/100;

		lpPet->m_wLastMaxAttackPower=	packet.wMaxAttackPower;
		lpPet->m_wLastMinAttackPower=	packet.wMinAttackPower;
		lpPet->m_iDefensePower		=	packet.iDefensePower;
	}

	memcpy(&packet.petInfo,&m_aPet[_iPetIndex],sizeof(cPetDefine));

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendSummonBeastInfo(int _iIndex,BOOL _bIsUpgrade)
{
	cACTOR	*lpSummonBeast	=	getSummonBeastActor(_iIndex);

	SG_SUMMON_BEAST_INFO	packet;

	packet.base.set(sizeof(SG_SUMMON_BEAST_INFO),dSG_SUMMON_BEAST_INFO);
	packet.bf4Index		=	_iIndex;

	if	(!lpSummonBeast)
	{
		packet.bf10Serial	=	0xffff;
		packet.bf10Level	=	0;
	}
	else
	{
		packet.bf10Serial	=	lpSummonBeast->m_wSerialInField;
		packet.bf10Level	=	lpSummonBeast->getLevel();
		packet.bf2Grade		=	lpSummonBeast->m_wSummonBeastGrade;
		packet.bf2Type		=	lpSummonBeast->m_wSummonBeastType;
		packet.bf1IsUpgrade	=	_bIsUpgrade;
	}

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendChangePetName(int _iPetIndex,char *_lpstrName,BOOL _bIsMiniPet,int _iResetItemIndex)
{
	SG_CHANGE_PET_NAME	packet;

	packet.base.set(sizeof(SG_CHANGE_PET_NAME),dSG_CHANGE_PET_NAME);
	strcpy(packet.strPetName,_lpstrName);
	packet.wPetIndex	=	_iPetIndex;
	packet.wIsMiniPet	=	_bIsMiniPet;
	packet.iResetItemIndex=	_iResetItemIndex;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendMakeBreedingRecordBook(int _iPetIndex)
{
	SG_MAKE_BREEDING_RECORD_BOOK	packet;

	packet.base.set(sizeof(packet),dSG_MAKE_BREEDING_RECORD_BOOK);
	packet.wPetIndex	=	_iPetIndex;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendSyncPetInfo(int _iPetIndex,int _iHP,int _iMaxHP,int _iMaxAttackPower,int _iMinAttackPower,int _iDefensePower,int _iStrength,int _iIntelligence,BOOL _bIsSummonBeast)
{
	SG_SYNC_PET_INFO	packet;

	packet.base.set(sizeof(packet),dSG_SYNC_PET_INFO);
	packet.bf29MaxHP		=	_iMaxHP;
	packet.bf2PetIndex		=	_iPetIndex;
	packet.bf1IsSummonBeast	=	_bIsSummonBeast;
	packet.wStrength		=	_iStrength;
	packet.wIntelligence	=	_iIntelligence;

	LONGLONG	llHP		=	_iHP;
	llHP					=	llHP*10000/_iMaxHP;
	packet.wRemainHP		=	(WORD)llHP;
	packet.wMinAttackPower	=	_iMinAttackPower;
	packet.wMaxAttackPower	=	_iMaxAttackPower;
	packet.iDefensePower	=	_iDefensePower;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendChangeTamerCommand(int _iPetIndex,int _iCommand,cACTOR *_lpTarget,int _iX,int _iY)
{
	SG_CHANGE_PET_COMMAND	packet;

	packet.base.set(sizeof(packet),dSG_CHANGE_PET_COMMAND);
	packet.bf2PetIndex		=	_iPetIndex;
	packet.bf4Command		=	_iCommand;
	packet.wX				=	_iX;
	packet.wY				=	_iY;

	if (_lpTarget)
		packet.bf10ProtectActor	=	_lpTarget->m_wSerialInField;
	else
		packet.bf10ProtectActor	=	1023;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendPetWaitCommand(int _iPetIndex)
{
	SG_PET_WAIT_COMMAND		packet;

	packet.base.set(sizeof(packet),dSG_PET_WAIT_COMMAND);
	packet.wPetIndex		=	_iPetIndex;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}


//
//	플레이어의 중요 정보를 업데이트 한다.
void
cACTOR::addSendPlayerBasicInformation()
{
	SG_UPDATE_BASIC_INFORMATION		packet;

	packet.base.set(sizeof(SG_UPDATE_BASIC_INFORMATION),dSG_UPDATE_BASIC_INFORMATION);

	LONGLONG	llRestHP	= m_iHP;
	llRestHP	*=	60000;
	llRestHP	/=	getMaxHP();

	packet.wHP		=	(WORD)llRestHP;
	packet.sCP		=	m_iCP/100;
	packet.wSerial	=	m_wSerialInField;

	m_iLastSyncHP	=	m_iHP;

	if	(m_iHP)
		packet.wHP	=	max(packet.wHP,1);

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}

//
//	플레이어의 중요 정보를 업데이트 한다.
void
cACTOR::sendPlayerBasicInformation()
{
	if	(!isPlayer())
		return;

	SG_UPDATE_BASIC_INFORMATION		packet;

	packet.base.set(sizeof(SG_UPDATE_BASIC_INFORMATION),dSG_UPDATE_BASIC_INFORMATION);

	LONGLONG	llRestHP	= m_iHP;

	llRestHP		*=	60000;
	llRestHP		/=	getMaxHP();

	packet.wHP		=	(WORD)llRestHP;
	packet.sCP		=	m_iCP/100;
	packet.wSerial	=	m_wSerialInField;

	if	(m_iHP)
		packet.wHP=	max(packet.wHP,1);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

BOOL
cACTOR::isTooFastSendBigSizeData()
{
	if (m_dwLastSendBigSizeDataTime+2000	>	g_dwCurrentTime)
		return	TRUE;

	return	FALSE;
}

void
cACTOR::setLastTimeSendBigSizeData(DWORD _dwTime)
{
	m_dwLastSendBigSizeDataTime	=	_dwTime;
}

void
cACTOR::sendPickPartyItemMessage(char *_lpstrName,cItem *_lpItem,BOOL _bIsSharingGold)
{
	SG_PICK_PARTY_ITEM	packet;

	packet.base.set(sizeof(packet),dSG_PICK_PARTY_ITEM);

	strcpy(packet.strPickerName,_lpstrName);
	packet.bf1IsSharingGold	=	_bIsSharingGold;

	memcpy(&packet.item,_lpItem,sizeof(cItem));

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendAccountInfo()
{
	SG_ACCOUNT_INFO	packet;

	packet.base.set(sizeof(packet),dSG_ACCOUNT_INFO);

	memcpy(packet.aPlayerCountPerServer,g_game.m_aiPlayerCountInfoInWorld,sizeof(packet.aPlayerCountPerServer));

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendMoveFailed(int _iReason)
{
	SG_MOVE_FAILED	packet;

	packet.base.set(sizeof(packet),dSG_MOVE_FAILED);

	packet.bf5Result=	_iReason;
	packet.bf1IsRun	=	m_isRunning;
	packet.bf10PPS	=	m_wPPS;
	packet.wX		=	m_pos.x;
	packet.wY		=	m_pos.y;	//	위치

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendItemPackName(int _iPackIndex)
{
	if (_iPackIndex	>=	dMAX_ITEM_PACK_COUNT)
		return;

	SG_ITEM_PACK_NAME	packet;

	packet.base.set(sizeof(packet),dSG_ITEM_PACK_NAME);
	packet.wPackIndex	=	_iPackIndex;
	strcpy(packet.strPackName,g_itemPackList.m_aPacks[_iPackIndex].m_strName);

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.strPackName)+strlen(packet.strPackName)+1;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendAskRemoveFriend(char *_lpstrClient)
{
	SG_ASK_REMOVE_FRIEND	packet;

	packet.base.set(sizeof(packet),dSG_ASK_REMOVE_FRIEND);

	strncpy(packet.strName,_lpstrClient,dNAME_LENGTH);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendAskRemoveFriendResult(char *_lpstrTarget,int _iResult)
{
	SG_ASK_REMOVE_FRIEND_RESULT	packet;

	packet.base.set(sizeof(packet),dSG_ASK_REMOVE_FRIEND_RESULT);

	strncpy(packet.strName,_lpstrTarget,dNAME_LENGTH);

	switch(_iResult)
	{
		case	eRFITLR_SUCCESS	:
			packet.wResult	=	eRFR_SUCCESS;
			break;
		case	eRFITLR_NOT_EXIST_NAME_IN_TARGET_FRIEND_LIST:
			packet.wResult	=	eRFR_NOT_EXIST_NAME_IN_TARGET_FRIEND_LIST;
			break;
		case	eRFITLR_NOT_CONNECTED	:
			packet.wResult	=	eRFR_TARGET_NOT_CONNECTED;
			break;
	}

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//	문 관련 메시지
void
cACTOR::sendDoorMessage(int _iArea,int _iKey,int _iSkill,int _iMessage,char *_lpstrCaster)
{
	SG_DOOR_MESSAGE	packet;

	packet.base.set(sizeof(packet),dSG_DOOR_MESSAGE);

	packet.wArea	=	_iArea;
	packet.wKey		=	_iKey;
	packet.wSkill	=	_iSkill;
	packet.wMessage	=	_iMessage;

	if (_lpstrCaster)
		strcpy(packet.strCasterName,_lpstrCaster);
	else
		packet.strCasterName[0]	=	NULL;

	packet.base.wSize=	sizeof(packet)-sizeof(packet.strCasterName)+strlen(packet.strCasterName)+1;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendGGAuthCode(void *_lpData)
{
#ifdef	_USE_NPGL
	if (g_bIsUseNPGL	==	FALSE)
		return;

	m_iLastSendGGAuthCodeTime	=	g_iCounterBySecond;
	
	SG_GG_AUTH_DATA	packet;

	packet.base.set(sizeof(packet),dSG_GG_AUTH_DATA);

	memcpy(packet.aBuffer,_lpData,sizeof(GG_AUTH_DATA));

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
#endif
}

void
cACTOR::sendArcaWork(int _iArcaIndex,int _iResult,int _iSpareValue,BOOL _bIsSendField)
{
	SG_ARCA_WORK	packet;

	packet.base.set(sizeof(packet),dSG_ARCA_WORK);
	packet.wArcaIndex	=	_iArcaIndex;
	packet.wWork		=	_iResult;
	packet.wValue1		=	_iSpareValue;

	if	(_bIsSendField)
		m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
	else
		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}


void
cACTOR::addSendCloseRangeArcaWork(int _iArcaIndex,int _iResult,int _iSpareValue,char *_lpstrName)
{
	SG_CLOSE_RANGE_ARCA_WORK	packet;

	packet.base.set(sizeof(packet),dSG_CLOSE_RANGE_ARCA_WORK);
	packet.wArcaIndex	=	_iArcaIndex;
	packet.wWork		=	_iResult;
	packet.wValue1		=	_iSpareValue;

	if (_lpstrName)
		strcpy(packet.strName,_lpstrName);
	else
		packet.base.wSize-=	dNAME_LENGTH;

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}

void
cACTOR::sendFieldTrapWork(int _iTrapIndex,int _iWork,int _iSpareValue,BOOL _bIsSendField)
{
	SG_FIELD_TRAP_WORK	packet;

	packet.base.set(sizeof(packet),dSG_FIELD_TRAP_WORK);
	packet.wTrapIndex	=	_iTrapIndex;
	packet.wWork		=	_iWork;
	packet.wValue1		=	_iSpareValue;

	if (_bIsSendField)
		m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
	else
		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::addSendCloseRangeFieldTrapWork(int _iTrapIndex,int _iWork,int _iSpareValue,char *_lpstrName)
{
	SG_CLOSE_RANGE_FIELD_TRAP_WORK	packet;

	packet.base.set(sizeof(packet),dSG_CLOSE_RANGE_FIELD_TRAP_WORK);
	packet.wTrapIndex	=	_iTrapIndex;
	packet.wWork		=	_iWork;
	packet.wValue1		=	_iSpareValue;

	if (_lpstrName)
		strcpy(packet.strName,_lpstrName);
	else
		packet.base.wSize-=	dNAME_LENGTH;

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}

void
cACTOR::addSendHitChainedDamage(cACTOR *_lpAttacker,cAbility *_lpAbility,int _iShotCount,int _iMissCount,int _iGap,
								int _iPhysicalDamage,int _iMagicDamage,int _iMinPhysicalDamage,int _iMinMagicDamage,
								int _iCriticalCount,int _iDoubleCriticalCount,int _iInstanceKillCount,
								int _iBlockingCount,int _iNoActionBlockCount,int _iDancingBlockCount, int _iHardBlowCount)
{
	SG_HIT_CHANED_DAMAGE	packet;

	packet.base.set(sizeof(packet),dSG_HIT_CHANED_DAMAGE);

	packet.bf5AttackCount			=	_iShotCount;	//	공격 횟수
	packet.bf5MissCount				=	_iMissCount;	//	미스 횟수
	packet.bf5CriticalCount			=	_iCriticalCount;	//	크리티컬 수
	packet.bf5DoubleCriticalCount	=	_iDoubleCriticalCount;	//	더블 크리티컬 수
	packet.bf5DancingBlockCount		=	_iDancingBlockCount;
	packet.bf4NoActionBlockingCount	=	_iNoActionBlockCount;

	packet.bf1IsCrushBlow			=	FALSE;	//	크러쉬 블로우 터졌다.
	packet.bf1IsAttackToShakle		=	_lpAttacker->isAttackToShakleActor(this);
//-------------------------------------------------------- 32bit
	packet.bf11Skill				=	_lpAbility->m_wSkill;
	packet.bf5BlockingCount			=	_iBlockingCount;
	packet.bf5ShotGap				=	_iGap;
	packet.bf11TargetSerial			=	m_wSerialInField;	//	누구를-_-?
	packet.bf1IsInstanceKill		=	min(_iInstanceKillCount,1);
//-------------------------------------------------------- 32bit

	packet.iPhysicalDamage			=	_iPhysicalDamage/100;
	packet.iMagicDamage				=	_iMagicDamage/100;	// 대미지
	packet.wMinPhysicalDamage		=	_iMinPhysicalDamage/100;
	packet.wMinMagicDamage			=	_iMinMagicDamage/100;
	
	packet.wCaster					=	_lpAttacker->m_wSerialInField;

	packet.bf5HardBlowCount				=	_iHardBlowCount;
	if	(_iPhysicalDamage)
		packet.iPhysicalDamage		=	max(packet.iPhysicalDamage,1);

	if	(_iMagicDamage)
		packet.iMagicDamage			=	max(packet.iMagicDamage,1);	// 대미지

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}
void
cACTOR::sendSecretDungeonMessage(int _iMessage,int _iValue1,int _iValue2)
{
	SG_SECRET_DUNGEON_MESSAGE	packet;

	packet.base.set(sizeof(packet),dSG_SECRET_DUNGEON_MESSAGE);

	packet.wMessage		=	_iMessage;
	packet.iValue1		=	_iValue1;
	packet.iValue2		=	_iValue2;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendBuyCarrotShopItemResult(int _iResult,int _iRemainCarrot,cItem *_lpItem)
{
	SG_BUY_CARROT_SHOP_ITEM_RESULT	packet;

	packet.base.set(sizeof(packet),dSG_BUY_CARROT_SHOP_ITEM_RESULT);

	packet.wResult				=	_iResult;
	packet.iRemainCarrotCount	=	_iRemainCarrot;

	if	(_lpItem)
		memcpy(&packet.item,_lpItem,sizeof(cItem));
	else
		packet.base.wSize	-=	sizeof(cItem);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendBannerText(int _iSerial,char *_lpstrText)
{
	SG_BANNER_TEXT	packet;

	packet.base.set(sizeof(packet),dSG_BANNER_TEXT);

	packet.wBannerSerial	=	_iSerial;
	strcpy(packet.strText,_lpstrText);
	packet.base.wSize		=	sizeof(packet)-sizeof(packet.strText)+strlen(packet.strText)+1;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendDuelRecord()
{
	SG_DUEL_RECORD	packet;

	packet.base.set(sizeof(packet),dSG_DUEL_RECORD);
	memcpy(&packet.record,&m_duelRecord,sizeof(cDuelRecord));

	packet.wWeeklyRank	=	m_wWeeklyDuelRank;
	packet.wDailyRank	=	m_wDailyDuelRank;
	packet.wTrialGameRank=	m_wTrialGameRank;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendDuelRanking(int _iType)
{
	if	(_iType	<	0	||	_iType	>	eTrialGameRanking)
		return;

	g_debugSign.m_iAuraSkillStep	=	_iType;

	SG_DUEL_RANKING	packet;

	packet.base.set(sizeof(packet),dSG_DUEL_RANKING);
	packet.bf2RankType	=	_iType;
	packet.bf10Count	=	min(g_duelRankerManager.getRankerCount(_iType),100);
	strcpy(packet.strTrialGameName,g_duelTrialGameManager.m_strTrialGameName);

	for (int i=0;i<packet.bf10Count;i++)
	{
		cDuelRankingInfo			*lpInfo		=	g_duelRankerManager.getRankInfo(_iType,i);
		cDuelRankingInfoForClient	*lpPacketData=	&packet.aList[i];

		strcpy(lpPacketData->m_strName,lpInfo->m_strName);
		lpPacketData->m_wJob	=	lpInfo->m_wJob;

		lpPacketData->m_iWinPoint	=	lpInfo->m_iWinPoint;	//	승점
		lpPacketData->m_wBattleCount=	lpInfo->m_iBattleCount;	//	전투 횟수
		lpPacketData->m_wWinCount	=	lpInfo->m_iWinCount;	//	승리 횟수
		lpPacketData->m_wDefeatCount=	lpInfo->m_iDefeatCount;	//	패배 횟수
		lpPacketData->m_wDrawCount	=	lpInfo->m_iDrawCount;	//	무승부 횟수
	}

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aList)+sizeof(cDuelRankingInfoForClient)*packet.bf10Count;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendDuelMsg(int _iMsg,int _iOutputType,char *_lpstrText)
{
	SG_DUEL_MSG	packet;

	packet.base.set(sizeof(packet),dSG_DUEL_MSG);
	packet.wMessage		=	_iMsg;
	packet.wOutputType	=	_iOutputType;

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.strText);

	if	(_lpstrText)
	{
		strcpy(packet.strText,_lpstrText);

		packet.base.wSize	+=	strlen(packet.strText)+1;
	}

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendOpenBadgeOfTraderShop()
{

	if	(m_lpField->m_bIsGuildPointBattleField	||	m_lpField->m_bIsSiegeWarfareField || m_lpField->m_bIsGuildBattleField)	//	길드전이나..포인트전.. 공성전때는 사용할수없다.
	{
		SG_USE_ITEM_RESULT	sgPacket;
		sgPacket.base.set(sizeof(sgPacket),dSG_USE_ITEM_RESULT);
		sgPacket.wTarget	=	m_wSerialInField;
		sgPacket.wResult	=	eUIR_CAN_NOT_USE_THIS_FIELD;
		g_userPM.add(m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

	SG_OPEN_SHOP	packet;
 
	packet.bf1IsDuelPointShop	=	FALSE;
	packet.bf15SellItemCount	=	6;
	packet.bf1IsLevelPerGoldShop=	FALSE;
	packet.bf15LevelPerGoldValue=	0;
	packet.wOwner				=	m_wSerialInField;
	packet.wShopSerial			=	0xfff0; // 임의로 설정...
	packet.wPriceFactor			=	25;
	packet.dwLimitPrice			=	10000;
	packet.wTokenItem			=	0xffff;
	packet.bf1IsNotBuyItem		=	FALSE;		// 상점 구입 불가.
	packet.bf1IsBadgeOfTraderShop	= TRUE;
	strcpy(packet.strName,"");
	
	for (int i=0;i<dBADGE_OF_TRADER_ITEM_LIST_COUNT;i++)
	{
		cItem	cBadgeOfTraderItem;
		
		if(m_lpField->m_shop.getIsBadgeOfTraderItemOption(i))
		{
			int iOption[4];
			iOption[0] = m_lpField->m_shop.getBadgeOfTraderItemOption1Min(i);
			iOption[1] = m_lpField->m_shop.getBadgeOfTraderItemOption1Max(i);
			iOption[2] = m_lpField->m_shop.getBadgeOfTraderItemOption2Min(i);
			iOption[3] = m_lpField->m_shop.getBadgeOfTraderItemOption2Max(i);

			g_im.generateItemByBaseItem(&cBadgeOfTraderItem, m_lpField->m_shop.getBadgeOfTraderItemSerial(i) ,m_lpField->m_shop.getBadgeOfTraderItemCount(i), -1, iOption);
		}
		else
		{
			g_im.generateItemByBaseItem(&cBadgeOfTraderItem, m_lpField->m_shop.getBadgeOfTraderItemSerial(i) ,m_lpField->m_shop.getBadgeOfTraderItemCount(i));
		}
		cBasicItem* lpBasic = cBadgeOfTraderItem.getBasicItem();
		memcpy(&packet.aItem[i],&cBadgeOfTraderItem,sizeof(CItemDefine));
		packet.aItem[i].m_dwSerial = m_lpField->m_shop.getBadgeOfTraderItemPrice(i);
		LONGLONG	llPrice	=	packet.aItem[i].m_dwSerial;
		llPrice				-=	llPrice*m_wCourtesyTitleValue/10000;

		if	(m_lpField->m_wIsGuildHall	==	FALSE)
			llPrice			+=	llPrice*getTradeBonusByA()/100;

		packet.aItem[i].m_dwSerial	=	(DWORD)llPrice;
		packet.aItem[i].m_dwSerial	=	max(packet.aItem[i].m_dwSerial,1);
	}

	int	iPacketSize	=	sizeof(SG_OPEN_SHOP) - sizeof(packet.aItem) + sizeof(CItemDefine)*packet.bf15SellItemCount;

	packet.base.set(iPacketSize,dSG_OPEN_SHOP);

	g_userPM.add(this->m_iClientSerial,&packet,packet.base.wSize);
	
	m_wShopSerial			=	packet.wShopSerial;

	
}
