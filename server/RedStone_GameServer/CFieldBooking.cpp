#include "cFIELD.H"
#include "CGame.H"
#include "Client.H"
#include "debugCode.H"

//
//	액터 죽이기 예약
void
cFIELD::bookingDeathActor(int _iKillerSerial,int _iSerial,int _iZoneSerial,BOOL _bIsRelease,BOOL _bIsByKarma)
{
	BW_DEATH_ACTOR	packetDeathActor;

	packetDeathActor.base.set(sizeof(BW_DEATH_ACTOR),dBW_DEATH_ACTOR);

	packetDeathActor.wSerial			=	_iSerial;
	packetDeathActor.iZoneSerial		=	_iZoneSerial;
	packetDeathActor.wKiller			=	_iKillerSerial;
	packetDeathActor.bf1IsReleaseSummon	=	_bIsRelease;
	packetDeathActor.bf1IsByKarma		=	_bIsByKarma;

	addReceivePacket((ALL_MSG*)&packetDeathActor,_iSerial,TRUE);
}

void
cFIELD::bookingMonsterTame(char *_lpstrCasterName,int _iTamedLevel,int _iTamedLoyalty,int _iTargetFieldSerial,int _iTargetZoneSerial)
{
	BW_MONSTER_TAME	packet;

	packet.base.set(sizeof(BW_MONSTER_TAME),dBW_MONSTER_TAME);

	packet.wTamedLevel			=	_iTamedLevel;
	packet.wTamedLoyalty		=	_iTamedLoyalty;
	packet.wTargetFieldSerial	=	_iTargetFieldSerial;
	packet.wTargetZoneSerial	=	_iTargetZoneSerial;

	strcpy(packet.strCaster,_lpstrCasterName);

	addReceivePacket((ALL_MSG*)&packet,0,TRUE);
}

BOOL
cFIELD::bookingMakeSkillZone(cACTOR *_lpCaster,cAbility *_lpAbility)
{
	CSkill	*lpSkill			=	_lpAbility->getSkill();

	if	(!lpSkill)
		return	FALSE;

	int		iSkillLimitCount	=	lpSkill->m_wLimitActiveCount;

	if	(iSkillLimitCount	!=	0xffff)
	{
		int	iCount	=	m_skillManager.getActiveSkillCount(lpSkill->m_wSerial,_lpCaster->m_iZoneSerial);

		if	(iCount	>=	iSkillLimitCount)
		{
//			_lpCaster->sendEtcWork(eEW_IS_LIMIT_ACTIVE_COUNT_SKILL,lpSkill->m_wSerial,lpSkill->m_wLimitActiveCount);
			return	FALSE;
		}
	}

	m_skillManager.getActiveSkillCount(_lpCaster->m_dwNameHashCode,_lpAbility->m_wSkill);

	BW_MAKE_SKILL_ZONE	packet;

	packet.base.set(sizeof(BW_MAKE_SKILL_ZONE),dBW_MAKE_SKILL_ZONE);

	packet.wCaster				=	_lpCaster->m_wSerialInField;
	packet.dwCasterNameHashCode	=	_lpCaster->m_dwNameHashCode;
	packet.wSkill				=	_lpAbility->m_wSkill;
	packet.wSkillLevel			=	_lpAbility->m_wLevel;

	addReceivePacket((ALL_MSG*)&packet,0,TRUE);

	return	TRUE;
}

void
cFIELD::bookingGeneratePet(char *_lpstrTamerName,int _iPetIndex,BOOL _bIsUnseal)
{
	BW_GENERATE_PET	packet;

	packet.base.set(sizeof(BW_GENERATE_PET),dBW_GENERATE_PET);

	strcpy(packet.strTamer,_lpstrTamerName);
	packet.bf4PetIndex		=	_iPetIndex;
	packet.bf1IsUnseal		=	_bIsUnseal;

	addReceivePacket((ALL_MSG*)&packet,0,TRUE);
}

void
cFIELD::bookingGenerateSummonBeast(int _iX,int _iY,char *_lpstrTamerName,int _iType,int _iGrade,int _iLevel,int _iRemainHP,BOOL _bIsSummon)
{
	if	(_iType		>=	dSUMMON_BEAST_TYPE_COUNT)
	{
		CLOG("_bookingGenerateSummonBeast","%s %d dSUMMON_BEAST_TYPE_COUNT",_lpstrTamerName,_iType);
		return;
	}
	if	(_iGrade	>=	dSUMMON_BEAST_GRADE_COUNT)
	{
		CLOG("_bookingGenerateSummonBeast","%s %d dSUMMON_BEAST_GRADE_COUNT",_lpstrTamerName,_iGrade);
		return;
	}

	BW_GENERATE_SUMMON_BEAST	packet;

	packet.base.set(sizeof(BW_GENERATE_SUMMON_BEAST),dBW_GENERATE_SUMMON_BEAST);

	strcpy(packet.strTamer,_lpstrTamerName);

	packet.bf10Level	=	_iLevel;
	packet.bf2Type		=	_iType;
	packet.bf2Grade		=	_iGrade;
	packet.bf1IsSummon	=	_bIsSummon;
	packet.iRemainHP	=	_iRemainHP;

	packet.wX			=	_iX;
	packet.wY			=	_iY;

	addReceivePacket((ALL_MSG*)&packet,0,TRUE);
}

//
//	플레이어 제거(예약을 한다.)
BOOL
cFIELD::bookingReadyToExit(int _iSerial,char *_lpstrName)
{
	cACTOR	*lpActor	=	getActor(_iSerial);

	if (!lpActor || lpActor->m_wSerialInField	==	0xffff)
	{
		CLOG("bookingExit","cFIELD::bookingReadyToExit - lpActor->m_wSerialInField == 0xffff");

		return	FALSE;
	}

	BW_READY_TO_EXIT	packetExitPlayer;

	packetExitPlayer.base.set(sizeof(packetExitPlayer),dBW_READY_TO_EXIT);
	packetExitPlayer.wSerial	=	_iSerial;
	strcpy(packetExitPlayer.strName,_lpstrName);

	addReceivePacket((ALL_MSG *)&packetExitPlayer,lpActor->m_wSerialInField,TRUE);

	return	TRUE;
}

//
//	플레이어 제거(예약을 한다.)
BOOL
cFIELD::bookingExitPlayer(int _iSerial,char *_lpstrName)
{
	cACTOR	*lpActor	=	getActor(_iSerial);

	if (!lpActor || lpActor->m_wSerialInField	==	0xffff)
	{
		CLOG("bookingExit","cFIELD::bookingExitPlayer - lpActor->m_wSerialInField == 0xffff");

		return	FALSE;
	}

	if	(lpActor->m_wWaitBuyCarrotShopItemResultTime)
		return	FALSE;

	BW_EXIT_GAME	packetExitPlayer;

	packetExitPlayer.base.set(sizeof(packetExitPlayer),dBW_EXIT_GAME);
	packetExitPlayer.wSerial	=	lpActor->m_wSerialInField;
	strcpy(packetExitPlayer.strName,_lpstrName);

	addReceivePacket((ALL_MSG *)&packetExitPlayer,lpActor->m_wSerialInField,TRUE);
	
	lpActor->m_GGG.ResetGGG();

	return	TRUE;
}

//
// 필드 이동 예약
BOOL
cFIELD::bookingToExitGameForMoveField(int _iSerial,char *_lpstrName)
{
	cACTOR	*lpActor	=	getActor(_iSerial);

	if (!lpActor || lpActor->m_wSerialInField	==	0xffff)
	{
		CLOG("bookingMoveField","cFIELD::bookingExitPlayer - lpActor->m_wSerialInField == 0xffff");

		return	FALSE;
	}

	BW_EXIT_GAME_FOR_MOVE_FIELD	packetExitPlayer;

	packetExitPlayer.base.set(sizeof(BW_EXIT_GAME_FOR_MOVE_FIELD),dBW_EXIT_GAME_FOR_MOVE_FIELD);
	packetExitPlayer.wSerial	=	_iSerial;
	strcpy(packetExitPlayer.strName,_lpstrName);

	addReceivePacket((ALL_MSG *)&packetExitPlayer,lpActor->m_wSerialInField,TRUE);

	return	TRUE;
}

//
//	더미 만들기 예약
void
cFIELD::bookingMakeDummy(int _iX,int _iY,int _iSerial,int _iUpkeepTime,int _iAddHPPercentage)
{
	BW_MAKE_DUMMY	bw;

	bw.base.set(sizeof(BW_MAKE_DUMMY),dBW_MAKE_DUMMY);

	bw.wLord			=	_iSerial;
	bw.wUpkeepTime		=	_iUpkeepTime;
	bw.iX				=	_iX;
	bw.iY				=	_iY;
	bw.wAddHPPercentage	=	_iAddHPPercentage;

	addReceivePacket((ALL_MSG *)&bw,_iSerial,TRUE);
}

//
//	더미 만들기 예약
void
cFIELD::appointmentMakeIllusion(int _iSkill,int _iX,int _iY,int _iLordSerial,DWORD _dwLordNameHashCode,int _iUpkeepTime,int _iHP,BOOL _bIsFirstTargetIllusion,int _iIllusionJob)
{
	BW_MAKE_ILLUSION	bw;

	bw.base.set(sizeof(BW_MAKE_ILLUSION),dBW_MAKE_ILLUSION);

	bw.wLord			=	_iLordSerial;
	bw.dwLordHashCode	=	_dwLordNameHashCode;
	bw.wUpkeepTime		=	_iUpkeepTime;
	bw.iX				=	_iX;
	bw.iY				=	_iY;
	bw.iHP				=	_iHP;
	bw.wJob				=	_iIllusionJob;
	bw.wIsFirstTarget	=	_bIsFirstTargetIllusion;
	bw.wSkill			=	_iSkill;

	addReceivePacket((ALL_MSG *)&bw,_iLordSerial,TRUE);
}


//
//	캐릭터 복사 예약
void
cFIELD::bookingCopyActor(int _iX,int _iY,int _iLord,int _iTarget,int _iUpkeepTime,int _iHP)
{
	BW_COPY_ACTOR	bw;

	bw.base.set(sizeof(BW_COPY_ACTOR),dBW_COPY_ACTOR);

	bw.wLord			=	_iLord;
	bw.wTarget			=	_iTarget;
	bw.wUpkeepTime		=	_iUpkeepTime;
	bw.iX				=	_iX;
	bw.iY				=	_iY;
	bw.iHP				=	_iHP;

	addReceivePacket((ALL_MSG *)&bw,_iTarget,TRUE);
}

//
//	빙의 예약
void
cFIELD::bookingDisplacement(cACTOR *_lpCaster,cACTOR *_lpTarget,BOOL _bIsRelease)
{
	if (_bIsRelease	==	FALSE)
		if (_lpCaster==NULL || _lpTarget== NULL)
			return;
	
	BW_DISPLACEMENT	bw;

	bw.base.set(sizeof(BW_DISPLACEMENT),dBW_DISPLACEMENT);

	bw.wCaster		=	0xffff;
	bw.wTarget		=	0xffff;

	if(_lpCaster)
		bw.wCaster	=	_lpCaster->m_wSerialInField;
	if(_lpTarget)
		bw.wTarget	=	_lpTarget->m_wSerialInField;

	bw.wIsRelease	=	_bIsRelease;

	addReceivePacket((ALL_MSG *)&bw,bw.wCaster,TRUE);
}

//
//	챠밍 예약
void
cFIELD::bookingCharming(cACTOR *_lpCaster,cACTOR *_lpTarget,BOOL _bIsRelease,int _iTime)
{
	if (_lpTarget	==	NULL)
		return;
	
	if (_lpTarget->isPlayer())
		return;

	BW_CHARMING	bw;

	bw.base.set(sizeof(BW_CHARMING),dBW_CHARMING);

	bw.wIsRelease			=	_bIsRelease;
	bw.wTime				=	_iTime;

	bw.wTarget				=	_lpTarget->m_wSerialInField;
	strcpy(bw.strTargetName,_lpTarget->m_strName);

	if (_lpCaster)
	{
		bw.wCaster				=	_lpCaster->m_wSerialInField;
		strcpy(bw.strCasterName,_lpCaster->m_strName);
	}

	addReceivePacket((ALL_MSG *)&bw,bw.wTarget,TRUE);
}

void
cFIELD::bookingIntervalShooter(int _iSkillIndex,cACTOR *_lpCaster,cACTOR *_lpTarget)
{
	BW_INTERVAL_SHOOTER	packet;

	packet.base.set(sizeof(packet),dBW_INTERVAL_SHOOTER);

	packet.wSkillIndex			=	_iSkillIndex;
	packet.wCaster				=	_lpCaster->m_wSerialInField;
	packet.wTarget				=	_lpTarget->m_wSerialInField;
	packet.dwCasterNameHashCode	=	_lpCaster->m_dwNameHashCode;
	packet.dwTargetNameHashCode	=	_lpTarget->m_dwNameHashCode;

	addReceivePacket((ALL_MSG *)&packet,packet.wTarget,TRUE);
}