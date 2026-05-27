#include "CHero.H"
#include "CActor.H"
#include "CMessage.h"
#include "rebirth_define.h"
#include "cSound.h"

CHero		g_hero;
CHero		g_controlMonster;

/*
class	cOldSaveClientData_front
{
public:
	WORD				m_aLeftAbility[2],m_aSubAbility[2],m_aRightAbility[2],m_aSupportAbility[2];//선택된 어빌러티
	WORD				m_aQuickSkill[10][10][2];
	int					m_wCurrentQuickSkillSlot;
};

class	cOldSaveClientData_body
{
public:
	WORD				m_aActivePassiveSkill[2][2];
};

class	cOldSaveClientData_rear
{
public:
	BOOL				m_isRunning;					//	뛰는중
	WORD				m_wIsOnSubAbility;

	CFriendInfo			m_aFriendList[dOLD_MAX_FRIEND_COUNT];	//친구
	char				m_strFriendGroup[dMAX_FRIEND_GROUP_COUNT][dNAME_LENGTH];
};

class	cOldSaveClientData	:	public cOldSaveClientData_front,public cOldSaveClientData_body,public cOldSaveClientData_rear
{
public:
};
*/
//
//	생성자
CHero::CHero()
{
	m_dwLastShoutTime		=	0;
	m_wPrincessWeaponOwner	=	0xffff;
	m_wShoutCount			=	0;
	m_bf1IsNetCafeUser		=	FALSE;
	// 10.01.14
	m_bUseSkillSlotItem		=	FALSE;
}

//
//	데이터 리셋
void
CHero::reset(BOOL _bIsFirst)
{
	m_wWaitMoveAreaResultTime	=	0;
	m_dwLastReceiveCommandTime	=	timeGetTime();

	m_wBardTitleValue			=	0;
	m_wBardTitleValueTime		=	0;

	m_bonusItemDropChance.resetAidData();
	m_bonusExp.resetAidData();

	m_iBookedDroppingItemSlot	=	0xffff;
	m_iBookedPickItem			=	0xffff;
	m_wIsReadyToFireChargedMiniPetSkill	=	FALSE;

	m_wPitchmanShopSerial		=	0xffff;
	m_wPitchmanShopSize			=	dPITCHMAN_SHOP_MINIMUM_SIZE;
	m_wRestraintTimeByAttackAction	=	0;
	//m_wRestraintTimeByCarrotItem	=	0;
	m_iLastActionTime			=	0;
	m_wEngageTarget				=	0xffff;
	m_iWaitMoveResultTime		=	0;
	m_iLastMovedTime			=	0;
	m_dwLastReportTime			=	0;
	m_wSpecialSkillField		=	0;
	m_iLastTransformationTime	=	0;
	m_wCaughtFieldTrap			=	0xffff;
	m_wWorkFieldTrap			=	0xffff;
	m_wPrincessWeaponOwner		=	0xffff;

	m_wDruggistTitleFactor		=	100;
	m_wCampingMasterTitleFactor	=	100;
	m_wBoostPower				=	100;

	{
		for (int i=0;i<dTWINKLE_ITEM_COUNT;i++)
			m_aiTwinkleItem[i]	=	-1;
	}

	m_bIsUseTempBufferForCheckUseableItem	=	FALSE;

	m_bf1IsNotFocusStatus		=	FALSE;
//	m_isRunning					=	FALSE;				//	뛰는중
	m_isWalking					=	FALSE;				//	걷는중
	m_isTransAble				=	FALSE;
	m_bf1IsReloadingBeltItem	=	FALSE;
	m_bf1IsHideWeapon1			=	FALSE;
	m_bf1IsHideWeapon2			=	FALSE;
	m_bf1IsWaitGuildInfo		=	TRUE;	//	길드 정보 기다리는중

	m_bf1IsNotFocusStatus		=	FALSE;;	//	포커스를 받지 않는 상태
	m_bf1IsWaitShoppingCommandResult	=	FALSE;
	m_bf1IsWaitTeleportResult			=	FALSE;	//	오브 사용 결과 기다리는중
	m_bf1IsWaitGuildCommandResult		=	FALSE;	//	길드 커맨드 결과 기다리는중

	m_bf1IsOwnedPitchmanShopSignBoard1	=	FALSE;
	m_bf1IsOwnedPitchmanShopSignBoard2	=	FALSE;
	m_bf1IsOwnedPitchmanShopSignBoard3	=	FALSE;
	m_bf1IsOwnedPitchmanShopSignBoard4	=	FALSE;
	m_bf1IsOwnedPitchmanShopSignBoard5	=	FALSE;
	m_bf1IsOwnedPitchmanShopLoudSpeaker	=	FALSE;
	m_bf1IsOwnedPitchmanColorPen		=	FALSE;
	m_bf1IsOwnedPitchmanShopBoldPen		=	FALSE;
	m_bf1IsOwnedPitchmanShopLongPen		=	FALSE;
	m_bf1IsOwnedPitchmanShopTwinkle		=	FALSE;
	m_bf1IsWaitArcaWorkResult			=	FALSE;
	m_bf1IsWaitTrapWorkResult			=	FALSE;
	m_bf1IsWaitDoorWorkResult			=	FALSE;
	m_bf1IsReadyToEventAreaWork			=	TRUE;
	m_bf1IsUseAbleSoundOfLeadersBellStatus	=	FALSE;
	

	m_wWantSummonedBeastIndex			=	0xffff;
	m_wNonBattleTime					=	0;

	m_wLeftAbility						=	0xffff;
	m_wSubAbility						=	0xffff;
	m_wRightAbility						=	0xffff;
	m_wSupportAbility					=	0xffff;
	m_wSlaveMonster						=	0xffff;
	m_wRestraintTimeByLogout			=	0xffff;

	m_iStripIndex	 					=	0xffff;		//	장비슬롯 초기화
	m_iEquipItemSlot					=	0xffff;		//	장비슬롯 초기화
	m_iStripItemSlot					=	0xffff;		//	장비슬롯 초기화
	m_dwQuickSlotDelayTime				=	timeGetTime();	
	int	i;

	for (i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
		m_aSummonBeast[i].reset();

	if	(_bIsFirst)
	{
		cPLAYER_STATUS::reset();

		for (i=0;i<dOWN_PET_COUNT;i++)
			m_aPet[i].reset();

		cUserCustomDataDefine::reset();

		m_wDeathPenelty				=	0;
		m_bf1IsWaitSimpleGuildInfo	=	TRUE;	//	길드 정보 기다리는중
		m_bf1IsWaitToReceiveDuelRecord=	FALSE;	//	결투 전적 정보 기다리는중
		m_bf1IsReceivedDuelRecord	=	FALSE;	//	결투 전적 정보 받았다.
	}
	
	m_dwGeneatedToolTipSerial			=	0xffffffff;	
	m_dwGeneatedToolTipHeroCheckSum		=	0xffffffff;	
	m_iGeneatedToolTipItemCheckSum		=	0xffffffff;	
	m_iGeneatedToolTipBaseSerial		=	0xffff;

	lockOff();
}

//
//	초기화
void
CHero::init()
{
	if	(isRequireFullInfo())
	{
		s_agent.sendAskPlayerInfo("");
		return;
	}

	m_posClick.Set(0 , 0);			//JBC 이동중 도착점 X표시 08-06-30

	s_agent.sendCheckHeroInfo(getCheckSum());

	reset(FALSE);
	readUserData();	//	유저 데이터 리드..-_-a

	if	(g_lpHero)
		sendTurnOnPassiveSkill();	//	패시브 스킬 정보 전송

	if	(g_lpHero	&&	m_bf1IsRunning)
		s_agent.sendSetMoveAbility();

	m_wIsProcessInitData	=	TRUE;
	correctCustomData();
	m_wIsProcessInitData	=	FALSE;

	m_level.m_sValue	=	m_iLevel;

	getRangeOfDefaultAttack();
	buildPower();

	if(g_map.m_bf1IsNotUseSkillBeforeTrans)
	{
		if(!g_hero.m_wTransformationTime )
			g_hero.setReactionAllOff();
	}
}

//
//	변신
void
CHero::transformation()
{
	m_iLastTransformationTime=	s_iFrameCounter;

	m_wJob					=	m_wTransformationJob;

	releaseEngagedTarget();
	reduceCP(m_wSpentCPForTransformation);
	setLeftSkill();

	m_wLeftAbility		=	m_aLeftAbility[(m_wJob)%2];
	m_wSubAbility		=	m_aSubAbility[(m_wJob)%2];
	m_wRightAbility		=	m_aRightAbility[(m_wJob)%2];
	m_wSupportAbility	=	m_aSupportAbility[(m_wJob)%2];

	g_lpHero->m_iHP	=	min(g_lpHero->m_iHP,getMaxHP());

	correctQuickSlotData();
	getRangeOfDefaultAttack();
	sendTurnOnPassiveSkill();

	buildPower();
}

//
//	변신할껴!!
void
CHero::sendTransformation()
{
	if(m_wRushTime)
		return;
	if	(g_lpControlActor						)
		return;
	if	(g_lpHero->m_isExclusiveAction			)
		return;
	if	(isAbleToTransformation()	!=	eCTR_OK	)
		return;

	m_iLastActionTime	=	s_iFrameCounter;

	g_lpHero->stop();

	m_isWalking		=	FALSE;
	m_wEngageTarget	=	0xffff;

	s_agent.sendTransformation();
}

//
//	앉거나 일어서거나
void
CHero::sendSitdown()
{
	// insu add
	if(s_iPopupInterface == ePIW_CARROT_SHOP)
		return;
	// insu add end
	if(m_wRushTime)
		return;
	if	(g_lpControlActor				)
		return;
	if	(g_lpHero->m_isExclusiveAction	)
		return;
	if	(!isAbleToSitdown()				)
		return;

	m_iLastActionTime	=	s_iFrameCounter;

	if	(m_isWalking)
		g_lpHero->stop();

	m_isWalking		=	FALSE;
	m_wEngageTarget	=	0xffff;

	s_agent.sendSitdown();
}
//
//	락온 한다.
void
CHero::lockOn(int _iTarget,int _iAbility,BOOL _bIsLockedByLeft)
{
	if	(m_wEngageTarget!=	_iTarget)
		stopBattle();	//	락온 하려는 타겟이 교전중이 녀석이 아니면 교전 취소

	m_wLockedTarget		=	_iTarget;
	m_wLockedAbility	=	_iAbility;
	m_bIsLockedByLeft	=	_bIsLockedByLeft;
}

void
CHero::resetAllForDuel()
{
	resetStatePoint();
	resetSkillPoint();

	int	i;

	for (i=0;i<dOWN_ITEM_COUNT+m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	getInventoryItem(i);

		if	(!lpItem)
			continue;

		lpItem->reset();
	}

	for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
		m_aEquip[i].reset();

	syncHeroApprearance();

	m_iGold	=	c_iDefaultGoldForDuelPlayer;

	AddSystemMessage(LTYELLOW,dMSG_RESET_PLAYER_INFO_FOR_DUEL_SERVER);
}

//
//	스탯 재분배
void
CHero::resetOneStatePoint(int _iState)
{
	CPlayerSaveData	player;

	if	(m_wJob == dJOB_WEREWOLF	||	m_wJob == dJOB_FALLEN_ANGEL	||	m_wJob == dJOB_DEVIL	||	m_wJob == dJOB_MAGICAL_GIRL)
		m_wJob--;

	cFILE	file;

	file.Open("data/defaultJob.dat","rb");
	file.Seek(m_wJob*sizeof(CPlayerSaveDataForServerDefine),SEEK_SET);
	file.Read(&player,sizeof(player));

	file.Close();

	CPlayerSaveData		*lpJob	=	&player;
	int					iJob		=	m_wJob/2,i;
	short*				lpsState	=	NULL;
	int					iJobBasicState=	0;

	switch(_iState)
	{
		case	eSTATE_STRENGTH		:
			lpsState	=	&m_sStrength;
			iJobBasicState=	lpJob->m_sStrength;
			break;
		case	eSTATE_AGILITY		:
			lpsState	=	&m_sAgility;
			iJobBasicState=	lpJob->m_sAgility;
			break;
		case	eSTATE_CONSTITUTION	:
			lpsState	=	&m_sConstitution;
			iJobBasicState=	lpJob->m_sConstitution;
			break;
		case	eSTATE_INTELLIGENCE	:
			lpsState	=	&m_sIntelligence;
			iJobBasicState=	lpJob->m_sIntelligence;
			break;
		case	eSTATE_WISDOM	:
			lpsState	=	&m_sWisdom;
			iJobBasicState=	lpJob->m_sWisdom;
			break;
		case	eSTATE_CHARISMA	:
			lpsState	=	&m_sCharisma;
			iJobBasicState=	lpJob->m_sCharisma;
			break;
		case	eSTATE_LUCK		:
			lpsState	=	&m_sLuck;
			iJobBasicState=	lpJob->m_sLuck;
			break;
	}

	int		iOrgState	=	(*lpsState);
	(*lpsState)			=	iJobBasicState;

	for (i=2;i<=m_iLevel;i++)
	{
		//		힘	민첩성	건강	지식	지혜	카리스마	운	사용자
		(*lpsState)		+=	g_aiLevelUpValue[iJob][_iState];
	}

	int	iResetPoint		=	iOrgState-(*lpsState);
	
	m_wLevelPoint		+=	iResetPoint;
	
	buildPower();

	CGamePlay::AddSystemMessage(CTBLUE,dMSG_RESET_STATE_POINT_COMMENT);
}

BOOL
CHero::checkResetAbleState(int _iState)
{
	CPlayerSaveData	player;

	if	(m_wJob == dJOB_WEREWOLF	||	m_wJob == dJOB_FALLEN_ANGEL	||	m_wJob == dJOB_DEVIL	||	m_wJob == dJOB_MAGICAL_GIRL)
		m_wJob--;

	cFILE	file;

	file.Open("data/defaultJob.dat","rb");
	file.Seek(m_wJob*sizeof(CPlayerSaveDataForServerDefine),SEEK_SET);
	file.Read(&player,sizeof(player));

	file.Close();

	CPlayerSaveData		*lpJob	=	&player;
	int					iJob		=	m_wJob/2,i;
	int					iJobBasicState=	0;
	int					iTargetStatePoint	=	0;

	switch(_iState)
	{
		case	eSTATE_STRENGTH		:
			iTargetStatePoint	=	m_sStrength;
			iJobBasicState=	lpJob->m_sStrength;
			break;
		case	eSTATE_AGILITY		:
			iTargetStatePoint	=	m_sAgility;
			iJobBasicState=	lpJob->m_sAgility;
			break;
		case	eSTATE_CONSTITUTION	:
			iTargetStatePoint	=	m_sConstitution;
			iJobBasicState=	lpJob->m_sConstitution;
			break;
		case	eSTATE_INTELLIGENCE	:
			iTargetStatePoint	=	m_sIntelligence;
			iJobBasicState		=	lpJob->m_sIntelligence;
			break;
		case	eSTATE_WISDOM	:
			iTargetStatePoint	=	m_sWisdom;
			iJobBasicState		=	lpJob->m_sWisdom;
			break;
		case	eSTATE_CHARISMA	:
			iTargetStatePoint	=	m_sCharisma;
			iJobBasicState		=	lpJob->m_sCharisma;
			break;
		case	eSTATE_LUCK		:
			iTargetStatePoint	=	m_sLuck;
			iJobBasicState		=	lpJob->m_sLuck;
			break;
	}

	int		iOrgState	=	iTargetStatePoint;
	iTargetStatePoint	=	iJobBasicState;

	for (i=2;i<=m_iLevel;i++)
	{
		//		힘	민첩성	건강	지식	지혜	카리스마	운	사용자
		iTargetStatePoint	+=	g_aiLevelUpValue[iJob][_iState];
	}

	if	(iTargetStatePoint	==	iOrgState)
		return	FALSE;
	
	return	TRUE;
}


void
CHero::rebirth(BOOL _bIsNotResetLevel)
{
	m_bf4RebirthCount++;
	
	if(!_bIsNotResetLevel)
	{
		m_iLevel		=	1;
		m_iExperience	=	0;
		
		resetSkillPoint();
		resetStatePoint();
		
		int	i;
		
		for (i=0;i<dOWN_ITEM_COUNT;i++)
		{
			cItem	*lpItem	=	(cItem *)&m_aItems[i];
			
			if	(lpItem->m_wBaseItem == 0xffff || !lpItem->isBreedingRecordBook())
				continue;
			
			cPetDefine	*lpPetInfo	=	(cPetDefine	*)&lpItem->m_bCount;
			
			lpPetInfo->m_bf10Level		=	1;
			lpItem->m_dwSerial			=	0;
		}
		
		for (i=0;i<m_bf6ExtraInventorySize;i++)
		{
			cItem	*lpItem	=	(cItem *)&m_aExtraInventory[i];
			
			if	(lpItem->m_wBaseItem == 0xffff || !lpItem->isBreedingRecordBook())
				continue;
			
			cPetDefine	*lpPetInfo	=	(cPetDefine	*)&lpItem->m_bCount;
			
			lpPetInfo->m_bf10Level		=	1;
			lpItem->m_dwSerial			=	0;
		}
	}
	g_esm.play("rebirth.wav");
	
	g_lpHero->addHitEffect(0,0,g_im.m_wRebirthImage);
}
//
//	스탯 재분배
void
CHero::resetStatePoint()
{
	CPlayerSaveData	player;

	if	(m_wJob == dJOB_WEREWOLF	||	m_wJob == dJOB_FALLEN_ANGEL	||	m_wJob == dJOB_DEVIL	||	m_wJob == dJOB_MAGICAL_GIRL)
		m_wJob--;

	cFILE	file;

	file.Open("data/defaultJob.dat","rb");
	file.Seek(m_wJob*sizeof(CPlayerSaveDataForServerDefine),SEEK_SET);
	file.Read(&player,sizeof(player));

	file.Close();

	CPlayerSaveData		*lpJob	=	&player;
	int					iJob	=	m_wJob/2,i;

	m_wLevelPoint		=	m_bf4CH5StateBonusCount*5;
	m_wLevelPoint		+=	c_aBonusStatePointForRebirth[m_bf4RebirthCount];

	m_sStrength			=	lpJob->m_sStrength;		//	힘			5 1,000
	m_sAgility			=	lpJob->m_sAgility;		//	민첩성		5 1,000
	m_sConstitution		=	lpJob->m_sConstitution;	//	건강		5 1,000
	m_sWisdom			=	lpJob->m_sWisdom;		//	지혜		5 1,000
	m_sIntelligence		=	lpJob->m_sIntelligence;	//	지식		5 1,000
	m_sCharisma			=	lpJob->m_sCharisma;		//	카리스마	5 1,000
	m_sLuck				=	lpJob->m_sLuck;			//	운			5 1,000
	m_iCP				=	lpJob->m_iCP;

	for (i=2;i<=m_iLevel;i++)
	{
//		힘	민첩성	건강	지식	지혜	카리스마	운	사용자
		m_sStrength			+=	g_aiLevelUpValue[iJob][0];
		m_sAgility			+=	g_aiLevelUpValue[iJob][1];
		m_sConstitution		+=	g_aiLevelUpValue[iJob][2];
		m_sIntelligence		+=	g_aiLevelUpValue[iJob][3];
		m_sWisdom			+=	g_aiLevelUpValue[iJob][4];
		m_sCharisma			+=	g_aiLevelUpValue[iJob][5];
		m_sLuck				+=	g_aiLevelUpValue[iJob][6];
		m_wLevelPoint		+=	g_aiLevelUpValue[iJob][7];
	}

	buildPower();

	CGamePlay::AddSystemMessage(CTBLUE,dMSG_RESET_STATE_POINT_COMMENT);
}

//
//	스탯 재분배
void
CHero::resetSkillPoint()
{
	CPlayerSaveData	player;

	if	(m_wJob == dJOB_WEREWOLF	||	m_wJob == dJOB_FALLEN_ANGEL	||	m_wJob == dJOB_DEVIL	||	m_wJob == dJOB_MAGICAL_GIRL)
		m_wJob--;

	cFILE	file;

	if	(!file.Open("data/defaultJob.dat","rb"))
		return;

	file.Seek(m_wJob*sizeof(CPlayerSaveDataForServerDefine),SEEK_SET);
	file.Read(&player,sizeof(player));

#ifdef	_DEBUG
	if	(player.m_wJob	!=	m_wJob)
		cMSG::Error("스탯 재분 에러!!","defaultJob.dat 파일이 이상하던지 버그가 있습니다!");
#endif

	file.Close();

	m_dwSkillExperience	=	getSkillPointSumByLevel();

	memcpy(m_aAbility,player.m_aAbility,sizeof(m_aAbility));

	memset(m_aRightAbility,0xff,sizeof(m_aRightAbility));
	memset(m_aSupportAbility,0xff,sizeof(m_aSupportAbility));
	memset(m_aSubAbility,0xff,sizeof(m_aSubAbility));
	memset(m_aQuickSkill,0xff,sizeof(m_aQuickSkill));
	m_wSubAbility		=	0xffff;
	m_wRightAbility		=	0xffff;
	m_wSupportAbility	=	0xffff;

	int	iSlot	=	m_wJob%2;

	//JBC	스킬 재분배시 리엑션 스킬 초기화.	08-09-30
	for (int i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		m_aActivePassiveSkill[iSlot][i]	=	0xffff;	
	}
	
	CGamePlay::AddSystemMessage(CTBLUE,dMSG_RESET_SKILL_POINT_COMMENT);

	buildPower();
}

void
CHero::resetOneState(int _iItemSlot,int _iPower,int _iValue,int _iLevelPoint)
{
	cItem	*lpItem	=	getInventoryItem(_iItemSlot);

	if	(lpItem)
		lpItem->decreaseCount(1);

	switch(_iPower)
	{
		case	0	:	//"힘",
			m_sStrength	=	_iValue;
			break;
		case	1	:	//"민첩성",
			m_sAgility	=	_iValue;
			break;
		case	2	:	//"건강",
			m_sConstitution	=	_iValue;
			break;
		case	3	:	//"카리스마",
			m_sCharisma	=	_iValue;
			break;
		case	4	:	//"지식",
			m_sIntelligence	=	_iValue;
			break;
		case	5	:	//"지혜",
			m_sWisdom	=	_iValue;
			break;
		case	6	:	//"행운",
			m_sLuck	=	_iValue;
			break;
	}

	m_wLevelPoint	=	_iLevelPoint;

	CGamePlay::AddSystemMessage(WHITE,dMSG_RESET_STATE_FORM,g_pstrEquipUseLimitContents[_iPower+1]);

	buildPower();
}

void
CHero::resetOneSkill(int _iItemSlot,int _iAbility,int _iLevel,int _iSkillExperience)
{
	cItem	*lpItem	=	getInventoryItem(_iItemSlot);
	CSkill	*lpSkill=	m_aAbility[_iAbility].getSkill();

	if	(lpItem)
		lpItem->decreaseCount(1);

	int	iDeltaPoint	=	_iSkillExperience-m_dwSkillExperience;

	AddSystemMessage(WHITE,dMSG_CHANGE_SKILL_LEVEL_FORM,lpSkill->m_strName,_iLevel);
	AddSystemMessage(WHITE,dMSG_GET_SKILL_EXP_FORM,iDeltaPoint);

	m_aAbility[_iAbility].m_wLevel	=	_iLevel;
	m_dwSkillExperience				=	_iSkillExperience;
}

//	직업 이름 리턴
char*
CHero::getJobName()
{
	return	g_pstrHeroJobName[m_wJob];
}

BOOL
CHero::checkLastActionTime(int _iCheckTime)
{
	if (m_iLastActionTime+_iCheckTime	>	s_iFrameCounter)
		return	FALSE;

	m_iLastActionTime	=	s_iFrameCounter;

	return	TRUE;
}
