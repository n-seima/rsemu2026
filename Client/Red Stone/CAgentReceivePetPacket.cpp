#include "CAgent.H"
#include "CActor.H"
#include "CGamePlay.H"
#include "CSkill.H"
#include "cMESSAGE.H"
#include "CSound.H"
#include "CEvent.H"
#include "CGuardianPost.H"
#include "CPortal.H"


//
//	Æ ¤º¸
void
cPACKET_HANDLER::ReceivePetInfo(SERVER_PACKETS *_lpPacket)
{
	SG_PET_INFO	*lpPacket	=	(SG_PET_INFO	*)_lpPacket;

	g_hero.setPetInfo(lpPacket->bf4PetIndex,&lpPacket->petInfo,lpPacket->wPetSerial,lpPacket->wMinAttackPower,lpPacket->wMaxAttackPower,lpPacket->iDefensePower);

	if (lpPacket->bf1IsTame)
		CGamePlay::AddSystemMessage(LTGREEN,dMSG_SUCCESS_TO_TAME_MONSTER);
}

//
//	Æ° ¾
void
cPACKET_HANDLER::ReceivePartingWithPet(SERVER_PACKETS *_lpPacket)
{
	SG_PARTING_WITH_PET	*lpPacket	=	(SG_PARTING_WITH_PET	*)_lpPacket;

	g_hero.partingWithPet(lpPacket->wPetIndex,lpPacket->wIsMiniPet,lpPacket->bf1IsRequireMessage);
}

//Æ ¸· º°æ
void
cPACKET_HANDLER::ReceiveChangePetCommand(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_PET_COMMAND	*lpPacket	=	(SG_CHANGE_PET_COMMAND	*)_lpPacket;

	g_hero.changeCommand(lpPacket->bf2PetIndex,lpPacket->bf4Command,lpPacket->wX,lpPacket->wY,lpPacket->bf10ProtectActor);
}

//Æ 
void
cPACKET_HANDLER::ReceiveSyncPetInfo(SERVER_PACKETS *_lpPacket)
{
	SG_SYNC_PET_INFO	*lpPacket	=	(SG_SYNC_PET_INFO	*)_lpPacket;

	g_hero.syncPetInfo(	lpPacket->bf2PetIndex,lpPacket->wRemainHP,lpPacket->bf29MaxHP,lpPacket->wMaxAttackPower,lpPacket->wMinAttackPower,lpPacket->iDefensePower,
						lpPacket->wStrength,lpPacket->wIntelligence,
						lpPacket->bf1IsSummonBeast);
}

//
void
cPACKET_HANDLER::ReceivePetWaitCommand(SERVER_PACKETS *_lpPacket)
{
	SG_PET_WAIT_COMMAND	*lpPacket	=	(SG_PET_WAIT_COMMAND	*)_lpPacket;

	g_hero.petWaitCommand(lpPacket->wPetIndex);
}

//
void
cPACKET_HANDLER::ReceiveMakeBreedingRecordBook(SERVER_PACKETS *_lpPacket)
{
	SG_MAKE_BREEDING_RECORD_BOOK	*lpPacket	=	(SG_MAKE_BREEDING_RECORD_BOOK	*)_lpPacket;

	g_hero.makeBreedingRecordBook(lpPacket->wPetIndex);
}

void
cPACKET_HANDLER::ReceiveChangePetName(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_PET_NAME	*lpPacket	=	(SG_CHANGE_PET_NAME	*)_lpPacket;

	g_hero.changePetName(lpPacket->wPetIndex,lpPacket->strPetName,lpPacket->wIsMiniPet,lpPacket->iResetItemIndex);
}


void
cPACKET_HANDLER::ReceiveSummonBeastInfo(SERVER_PACKETS *_lpPacket)
{
	SG_SUMMON_BEAST_INFO	*lpPacket	=	(SG_SUMMON_BEAST_INFO	*)_lpPacket;

	g_hero.setSummonBeastInfo(lpPacket->bf4Index,lpPacket->bf10Serial,lpPacket->bf10Level,lpPacket->bf2Grade,lpPacket->bf2Type);
}


void
cPACKET_HANDLER::ReceiveRidingDog(SERVER_PACKETS *_lpPacket)
{
	SG_RIDING_DOG			*lpPacket	=	(SG_RIDING_DOG	*)_lpPacket;

	g_hero.ridingDog(lpPacket->bf1IsRiding,lpPacket->bf2DogGrade,lpPacket->wPosX,lpPacket->wPosY);
}



