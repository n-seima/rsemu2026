#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"
#include "itemPackList.H"
#include	"actor_mini_pet.h"
//
//	_lpAsker가 이벤트*-_-* 하자고 요청했다.
BOOL
cACTOR::eventWith(cACTOR *_lpAsker)
{
	cNPC	*lpNpc	=	getNpc();

	if	(!lpNpc)
		return	FALSE;

	if	(!lpNpc->m_pSpeech)	
		return	FALSE;

	if (g_bIsWantPrintTriggerCondition)
		printf("\n◆ '%s'와의 이벤트 시작\n",m_strName);

	if (!lpNpc->m_pSpeech->operateFirstSpeech(_lpAsker,this))
		return FALSE;

	return	TRUE;
}

//
//	대사 선택
BOOL
cACTOR::selectSpeech(cACTOR *_lpAsker,int _iConversationIndex,int _iSelectSpeech,BOOL _bIsInnerSelect)
{
	g_debugSign.m_iTempStep	=	1;
	
	CConversationContainer	*lpCC		=	NULL;

	if (_lpAsker->m_wEventArea	!=	0xffff)
	{
		cAreaInfo	*lpEventArea=	_lpAsker->m_lpField->getArea(_lpAsker->m_wEventArea);

		if	(!lpEventArea)
			return	FALSE;

		lpCC	=	&lpEventArea->m_cc;
	}
	else
	{
		cNPC	*lpNpc		=	getNpc();

		if	(lpNpc)
			lpCC	=	lpNpc->m_pSpeech;
	}

	if	(!lpCC)
		return	FALSE;

	if	(_bIsInnerSelect	==	FALSE)
	{
		g_debugSign.m_iTempStep	=	2;

		if	(_lpAsker->m_wSelectConversation	!=	_iConversationIndex || _lpAsker->m_wTalkNpc	!=	m_wSerialInField)
		{
//			CLOG("__suspectedHacker","%s/%s %s[%d,%d] with %s",_lpAsker->m_strId,_lpAsker->m_strName,m_lpField->m_strFileName,_lpAsker->m_pos.mx,_lpAsker->m_pos.my,m_strName);
//			printf("suspectedHacker!! %s/%s %s[%d,%d] with %s\n",_lpAsker->m_strId,_lpAsker->m_strName,m_lpField->m_strFileName,_lpAsker->m_pos.mx,_lpAsker->m_pos.my,m_strName);

			g_debugSign.m_iTempStep	=	3;
			return	FALSE;
		}

		if (_lpAsker->m_wBlockSelectSpeechTime)
			return	FALSE;
	}

	_lpAsker->m_wBlockSelectSpeechTime	=	4;

	g_debugSign.m_iTempStep	=	4;

	return	lpCC->operateSelectSpeechReaction(_lpAsker,this,_iConversationIndex,_iSelectSpeech);
}

void
cACTOR::closeTalk()
{
	m_wTalkNpc				=	0xffff;
	m_wSelectConversation	=	0xffff;

	sendEndDialg();
}

void
cACTOR::transToEventMob(int _iEvent,int _iEventMob)
{
//	return;
	
	switch(_iEvent)
	{
		case	0	:
		{
			m_bf10Costume				=	_iEventMob;
			m_bf1IsEvilSpritOfChristmas	=	TRUE;
			addSendVerySimpleInfo();
			break;
		}

		case	1	:
			m_bf10Costume				=	_iEventMob;
			m_bf1IsEvilSpritOfChristmas	=	FALSE;
			addSendVerySimpleInfo();
			break;

		case	2	:
			m_bf10Costume				=	_iEventMob;
			m_bf1IsEvilSpritOfChristmas	=	FALSE;
			addSendVerySimpleInfo();
			break;
	}

	if	(m_aAbility[0].m_wSkill	!=	910)
	{
		for (int i=0;i<dABILITY_COUNT;i++)	
		{
			m_aAbility[i].m_wSkill	=	910+i;
			m_aAbility[i].m_wLevel	=	0;

			if	(g_aSkill[m_aAbility[i].m_wSkill].m_wSerial	==	0xffff)
				m_aAbility[i].m_wSkill	=	0xffff;
		}

		m_dwSkillExperience	=	getSkillPointSumByLevel()+1;

		sendPlayerInfo(this);
	}

	buildPower();
}

void
cACTOR::transToMonster(WORD	_wEffect, WORD _wTime,	WORD _wLevel)
{

	m_wTransformationTime	= _wTime;
	m_wTransformationLevel	= _wLevel;
	switch(_wEffect)
	{
	case eIE_TRANSFORMATION_HUNTER:
		m_wTransformationSkin = dJOB_MONSTER_DARK_ELF;
		m_wTransformationSkill	=	dSKILL_TRANSFORMATION_BOW_ATTACK;		// 변신 스킬
		break;
		
	case eIE_TRANSFORMATION_THIEF:
		m_wTransformationSkin = dJOB_MONSTER_ASSASSIN;
		m_wTransformationSkill	=	dSKILL_TRANSFORMATION_SWORD_ATTACK;		// 변신 스킬
		break;
	case eIE_TRANSFORMATION_MAGICIAN:
		m_wTransformationSkin = dJOB_MONSTER_FALLEN_WIZARD;
		m_wTransformationSkill	=	dSKILL_TRANSFORMATION_FIRE_BOLT;		// 변신 스킬
		break;

	case	eIE_ACL_TRANSFORMATION_REPTILE:

		if(m_wTransformationSkin == dJOB_MONSTER_REPTILE)
		{
			releaseTransformationMonster();
			buildPower();
			addSendSimpleInfo();
			return;
		}

		m_wTransformationSkin = dJOB_MONSTER_REPTILE;
		m_wTransformationSkill	=	dSKILL_TRANSFORMATION_SWORD_ATTACK;		// 변신 스킬
		break;
	}

	cActor_miniPet* lpMiniPet = this->iMiniPet();
	if(lpMiniPet)
	{
		lpMiniPet->activateMiniPet(lpMiniPet->m_bf5ActiveMiniPet2, FALSE);	 
		lpMiniPet->activateMiniPet(lpMiniPet->m_bf5ActiveMiniPet, FALSE);
	}
	
	int iBeastCount = getSummonBeastCount();
	
	for(int i=0;i<iBeastCount;++i)
	{
		partingWithPet(i,TRUE);
	}
	m_GGG.ResetGGG();

	m_bf5MagicCarpetShape	=	0;
	dispell();
	cureWrongStatus();
	reactionAllOff();
	addSendVerySimpleInfo();
	buildPower();
	m_iHP = getMaxHP();
}


void
cACTOR::sprinkleEventGift(int _iCount)
{
	int	iItem		=	0xffff;
	int	iPackSerial	=	0xffff;

	if (m_bf1IsEvilSpritOfChristmas)
		iPackSerial	=	0;

	if (iPackSerial	==	0xffff)
		return;

	iItem		=	g_itemPackList.getLinkItem(iPackSerial);

	if (iPackSerial	==	0xffff	||	iItem	==	0xffff)
		return;

	CDroppedItem	droppedItem;
	cFIELD			*lpField		=	m_lpField;

	g_im.generateItemByBaseItem(&droppedItem.m_item,iItem,1);

	droppedItem.m_item.m_aOption[0]	=	iPackSerial;
	droppedItem.m_wOwner			=	0xffff;
	droppedItem.m_wSealedTime		=	0;

	for (int i=0;i<_iCount;i++)
	{
		droppedItem.m_item.m_dwSerial	=	largeRandom(0x7ffffff0)+10;

		while(1)
		{
			droppedItem.m_pos.x				=	m_pos.x+random(200)-100;
			droppedItem.m_pos.y				=	m_pos.y+random(200)-100;

			if	(m_lpField->isMissileBlockedLine(m_pos.x,m_pos.y,droppedItem.m_pos.x,droppedItem.m_pos.y))
				continue;

			break;

		}
		lpField->dropItem(&droppedItem);
	}
}

void
cACTOR::dropItemForEventMob()
{
	if (m_bf1IsEvilSpritOfChristmas)
	{
		if (random(4)	!=	1)
			return;
	}
	else
		return;

	sprinkleEventGift(1);
}



int
cACTOR::summonMonster(int _iValue)
{

	if(m_lpField->m_wIsGuildHall || m_lpField->m_wIsVillage || m_lpField->isSecretDungeon() ||
		m_lpField->m_bIsGuildPointBattleField || m_lpField->m_bIsGuildBattleField || m_lpField-> m_bIsSiegeWarfareField)
	{
		return eUIR_CAN_NOT_USE_IN_FIELD;
	}

	std::vector<int> vecMobList;
	int iCount = 0;
	int iMonsterLevel = _iValue;
	for (int i=0;i<dMAX_NPC_COUNT && iCount<m_lpField->m_iSavedActorCount;++i)
	{
		if (m_lpField->m_apNpcGenerateData[i]	==	NULL)
			continue;		

		if (m_lpField->m_apNpcGenerateData[i]->m_wActorKind	!=	eAK_MONSTER)
			continue;
		
		if (m_lpField->m_apNpcGenerateData[i]->m_wActorSerial	!=	0xffff)
			continue;
		
		if	(m_lpField->m_apNpcGenerateData[i]->m_wIsBlockToAutoRegen)
			continue;
		
		int			iCharacterSerial	=	m_lpField->m_apNpcGenerateData[i]->m_wCharacter;
		cCHARACTER	*lpCharacter		=	&m_lpField->m_aCharacter[iCharacterSerial];
		cJOB* lpJob	=	lpCharacter->getJob();
		
		if(lpJob && lpJob->m_wMonsterLevel >= iMonsterLevel)
			continue;

		vecMobList.push_back(i);		
		++iCount;
	}
	
	int iMobCount = vecMobList.size();

	if(iMobCount == 0)
	{
		return eUIR_BE_MANY_CHARACTER_IN_FIELD;
	}

	int iRand = random(iMobCount);
	int iSerial = vecMobList[iRand];
	
	m_lpField->generateNpc(m_lpField->m_apNpcGenerateData[iSerial],FALSE, FALSE, m_pos.x,m_pos.y);	

	

	return	eUIR_OK;
}
