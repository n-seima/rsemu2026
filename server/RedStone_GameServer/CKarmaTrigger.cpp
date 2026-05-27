#include "CKarma.h"
#include "cACTOR.H"
#include "CQuest.H"
#include "CGame.H"
#include "secretDungeon.H"
#include "CEvent.H"
#include "CGuild.H"

//
//	레벨 체크
BOOL
CKarmaItem::checkLevel(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
	if (_lpAsker->m_iLevel >= m_aValue[0] && _lpAsker->m_iLevel <= m_aValue[1])	return	TRUE;

	return	FALSE;
}

//
//	어딘가의 길드에 속해있다.
BOOL
CKarmaItem::checkIsAssignedAnyGuild(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 길드에 [guild_member]0"																	,
//	{"가입해 있다.","가입해 있지 않다.","end"},							//	eKARMA_KW_GUILD_MEMBER
	BOOL	bCheckStatus	=	m_aValue[0];

	if (bCheckStatus == 0)
	{
		if (_lpAsker->m_wGuildSerial == 0xffff)	return	FALSE;

		return	TRUE;
	}

	if (_lpAsker->m_wGuildSerial	==	0xffff)	return	TRUE;

	return	FALSE;
}

//	길드 안에 직위 체크
BOOL
CKarmaItem::checkRankInGuild(cACTOR *_lpAsker)
{
	if	(_lpAsker	==	NULL)
		return	FALSE;
//	" 길드에 [guild_member]0"																	,
//	{"가입해 있다.","가입해 있지 않다.","end"},							//	eKARMA_KW_GUILD_MEMBER
	int		iRank		=	m_aValue[0];
	int		iIdaAnida	=	m_aValue[1];
	BOOL	bIsOK		=	FALSE;

	if	(!_lpAsker->getGuild())
	{
		if	(iIdaAnida	==	0)
			return	FALSE;

		return	TRUE;
	}

	switch(iRank)
	{
		case	0	:
			if	(_lpAsker->m_wGuildRank	==	dGUILD_CLASS_NOT)
				bIsOK		=	TRUE;
			break;
		case	1	:
			if	(_lpAsker->m_wGuildRank	==	dGUILD_CLASS_ROOKIE)
				bIsOK		=	TRUE;
			break;
		case	2	:
			if	(_lpAsker->m_wGuildRank	==	dGUILD_CLASS_MEMBER)
				bIsOK		=	TRUE;
			break;
		case	3	:
			if	(_lpAsker->m_wGuildRank	==	dGUILD_CLASS_PARLIAMENT)
				bIsOK		=	TRUE;
			break;
		case	4	:
			if	(_lpAsker->m_wGuildRank	==	dGUILD_CLASS_SUBMASTER)
				bIsOK		=	TRUE;
			break;
		case	5	:
			if	(_lpAsker->m_wGuildRank	==	dGUILD_CLASS_ELDER)
				bIsOK		=	TRUE;
			break;
		case	6	:
			if	(_lpAsker->m_wGuildRank	==	dGUILD_CLASS_MASTER)
				bIsOK		=	TRUE;
			break;
	}

	if	(iIdaAnida	==	0)
		return	bIsOK;

	return	1-bIsOK;
}

//
//	아이템 체크
BOOL
CKarmaItem::checkItem(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	[item]0의 보유 갯수가 [spin]1[0,255]개 [equal]2 
	int	iItem		=	m_aValue[0];
	int	iCount		=	m_aValue[1];
	int	iMethod		=	m_aValue[2];
	int	iIncudeEquip=	m_aValue[3];
	int	iItemCount	=	0;

	iItemCount		=	_lpAsker->getItemCount(iItem,iIncudeEquip);

	if	(iMethod	==	0)	//	이다
		if	(iItemCount	==	iCount)
			return	TRUE;

	if	(iMethod	==	1)	//	이하다
		if	(iItemCount	<=	iCount)
			return	TRUE;

	if	(iMethod	==	2)	//	이상이다
		if	(iItemCount	>= iCount)
			return	TRUE;

	return	FALSE;
}

//
//	남아 있는 빈 아이템 슬롯의 수
BOOL
CKarmaItem::checkRemainItemSlotCount(cACTOR *_lpActor)
{
	if (_lpActor	==	NULL)
		return	FALSE;
//	남은 아이템 슬롯의 수가 [spin]0[0,255]개 [equal]1
//	{"이다","이하이다","이상이다","end"},								//	eKARMA_KW_EQUAL			,		//	이상이다. 이하이다. 이다.
	int	iSlotCount	=	m_aValue[0];
	int	iMethod		=	m_aValue[1];

	int	iRemainITemSlotCount	=	_lpActor->getRemainItemSlotCount();

	if	(iMethod	==	0)	//	이다
		if	(iRemainITemSlotCount==	iSlotCount)
			return	TRUE;

	if	(iMethod	==	1)	//	이하다
		if	(iRemainITemSlotCount<=	iSlotCount)
			return	TRUE;

	if	(iMethod	==	2)	//	이상이다
		if	(iRemainITemSlotCount>= iSlotCount)
			return	TRUE;

	return	FALSE;
}

//	마스터한 스킬의 갯수 체크
BOOL
CKarmaItem::checkMasterSkillCount(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
	int	iCheckCount	=	m_aValue[0];
	int	iMethod		=	m_aValue[1];

	int	iMasterSkillCount	=	_lpAsker->getMasterSkillCount();

	if (iMethod	==	0)	//	이다
		if (iMasterSkillCount==	iCheckCount)	return	TRUE;

	if (iMethod	==	1)	//	이하다
		if (iMasterSkillCount<=	iCheckCount)	return	TRUE;

	if (iMethod	==	2)	//	이상이다
		if (iMasterSkillCount>= iCheckCount)	return	TRUE;

	return	FALSE;
}

//
//	특정 아이템을 착용하고 있냐?
BOOL
CKarmaItem::checkEquippedItem(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" [item]0을 [item_equip]1"																	,
	int	iItem		=	m_aValue[0];
	int	iMethod		=	m_aValue[1];
//	{"장착하고 있다.","장착하고 있지 않다.","end"},						//	eKARMA_KW_ITEM_EQUIP
	BOOL	bIsExist = FALSE;
	for (int i=0;i<dEQUIPMENT_PART_COUNT;i++)
	{
		if	(_lpAsker->m_aEquip[i].m_wBaseItem	== 0xffff	)
			continue;
		if	(_lpAsker->m_aEquip[i].m_bCount		== 0		)
			continue;

		if	(_lpAsker->m_aEquip[i].m_wBaseItem	== iItem)
		{
			bIsExist	=	TRUE;
		}


	}

	if(iMethod == 1)
	{
		bIsExist = !bIsExist;	
	}

	return	bIsExist;
}


//
//	돈 체크
BOOL
CKarmaItem::checkGold(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 보유한 금액이 [number]0[0,2147483647]골드 [equal]1"										,
	int	iGold	=	m_aValue[0];
	int	iMethod	=	m_aValue[1];
	int	iByLevel=	m_aValue[2];

	if	(iByLevel)
		iGold	*=	_lpAsker->m_iLevel;

	if	(iMethod	==	0)	//	이다
		if	(_lpAsker->m_iGold	== iGold)
			return	TRUE;

	if	(iMethod	==	1)	//	이하다
		if	(_lpAsker->m_iGold	<= iGold)
			return	TRUE;

	if	(iMethod	==	2)	//	이상이다
		if	(_lpAsker->m_iGold	>= iGold)
			return	TRUE;

	return	FALSE;
}

//
//	스킬 레벨 체크
BOOL
CKarmaItem::checkSkill(cACTOR *_lpAsker)
{
	if	(_lpAsker	==	NULL)
		return	FALSE;
//	" 스킬 [skill]0의 레벨이 [spin]1[0,50] [equal]2"											,
//	{"이다","이하이다","이상이다","end"},								//	eKARMA_KW_EQUAL			,		//	이상이다. 이하이다. 이다.
	int	iSkill	=	m_aValue[0];
	int	iLevel	=	m_aValue[1];
	int	iMethod	=	m_aValue[2];

	for (int i=0;i<dABILITY_COUNT;i++)
	{
		cAbility	*lpAbility	=	&_lpAsker->m_aAbility[i];

		if	(lpAbility->m_wSkill!=	iSkill)
			continue;

		if	(iMethod == 0)
			if	(iLevel == lpAbility->m_wLevel)
				return	TRUE;

		if	(iMethod == 1)
			if	(iLevel >= lpAbility->m_wLevel)
				return	TRUE;

		if	(iMethod == 2)
			if	(iLevel <= lpAbility->m_wLevel)
				return	TRUE;
	}

	if	(iMethod == 0)
		if	(iLevel == 0)
			return	TRUE;

	if	(iMethod == 1)
		if	(iLevel >= 0)
			return	TRUE;

	if	(iMethod == 2)
		if	(iLevel <= 0)
			return	TRUE;

	return	FALSE;
}

//
//	보유 타이틀 확인
BOOL
CKarmaItem::checkTitle(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
// 칭호 [title]0의 레벨이 [spin]1[0,10] [equal]2 
	int	iTitle				=	m_aValue[0];
	int	iTitleLevel			=	m_aValue[1];
	int	iMethod				=	m_aValue[2];
	int	iCharacterTitleLevel=	0;
//	{"이다","이하이다","이상이다","end"},								//	eKARMA_KW_EQUAL			,		//	이상이다. 이하이다. 이다.

	CPlayerTitleInfo	*lpTitleInfo	=	_lpAsker->getTitle(iTitle);

	if	(lpTitleInfo)
		iCharacterTitleLevel=	lpTitleInfo->m_bLevel;

	if	(iMethod	==	0 && iCharacterTitleLevel == iTitleLevel)
		return	TRUE;
	if	(iMethod	==	1 && iCharacterTitleLevel <= iTitleLevel)
		return	TRUE;
	if	(iMethod	==	2 && iCharacterTitleLevel >= iTitleLevel)
		return	TRUE;

	return	FALSE;
}

//
//	직업 체크
BOOL
CKarmaItem::checkJob(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;

	int		iJob		=	m_aValue[0];
	int		iMethod		=	m_aValue[1];		// 0 이다, 1아니다. 
	int		iInclude	=	m_aValue[2];		// 0 이면 변신후 직업 포함.. 1이면 직업무시.
//	" 직업이 [player_job]0 [ida_anida]1. [include_transformation_job]2"							,
	
	BOOL	bIsSameJob	=	FALSE;
	
	if(iMethod)	// 아니다.
	{

		if(iJob != _lpAsker->m_wJob)
		{
			bIsSameJob = TRUE;
		}
		if(!iInclude)	// 변신후 직업포함..
		{
			if(iJob != _lpAsker->getAnotherJob() && bIsSameJob)
				bIsSameJob = TRUE;
		}
		
	}
	else
	{		// 이다..
		if(iJob == _lpAsker->m_wJob)
		{
			bIsSameJob = TRUE;
		}
		if(!iInclude)
		{
			if(iJob == _lpAsker->getAnotherJob())
				bIsSameJob = TRUE;
		}
	}

	return	bIsSameJob;
}

//
//	성향 체크
BOOL
CKarmaItem::checkAllignment(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 성향이 [allignment]0이다."																,
//	{"절대 악","악","중립","선","절대 선","end"},						//	eKARMA_KW_ALLIGNMENT	,
	int	iAllignmentCondition	=	m_aValue[0];
	int	iAllignment				=	_lpAsker->getAllignment();

	if (iAllignmentCondition+1	==	iAllignment)	return	TRUE;

	return	FALSE;
}

//
//	플래이 시간 체크
BOOL
CKarmaItem::checkPlayTime(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
	int	iHour		=	m_aValue[0];
	int	iMethod		=	m_aValue[1];
//	{"이다","이하이다","이상이다","end"},								//	eKARMA_KW_EQUAL			,		//	이상이다. 이하이다. 이다.
//	" 플레이 시간이 [number]0[0,2147483647]시간 [equal]1"										,

	int	iPlayTime	=	_lpAsker->m_dwPlayTime/60;

	if (iMethod	==	0)
	{
		if (iHour	==	iPlayTime)	return	TRUE;

		return	FALSE;
	}

	if (iMethod	==	1)
	{
		if (iHour	>=	iPlayTime)	return	TRUE;

		return	FALSE;
	}

	if (iMethod	==	2)
	{
		if (iHour	<=	iPlayTime)	return	TRUE;

		return	FALSE;
	}

	return	FALSE;
}

//
//	기본 능력치 체크
BOOL
CKarmaItem::checkBasicPower(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" [power]0가 [spin]1[1,10000] [equal]2"														,
//	{"이다","이하이다","이상이다","end"},								//	eKARMA_KW_EQUAL			,		//	이상이다. 이하이다. 이다.
	int	iPowerType	=	m_aValue[0];
	int	iValue		=	m_aValue[1];
	int	iMethod		=	m_aValue[2];
	int	iPowerValue;

	switch(iPowerType)
	{
		case	eABP_LEVEL				:
			iPowerValue	=	_lpAsker->m_iLevel;
			break;
		case	eABP_STRENGTH			:
			iPowerValue	=	_lpAsker->getStrength();
			break;
		case	eABP_DEXTERITY			:
			iPowerValue	=	_lpAsker->getAgility();
			break;
		case	eABP_CONSTITUTION		:
			iPowerValue	=	_lpAsker->getConstitution();
			break;
		case	eABP_WISDOM				:
			iPowerValue	=	_lpAsker->getWisdom();
			break;
		case	eABP_INTELLIGENCE		:
			iPowerValue	=	_lpAsker->getIntelligence();
			break;
		case	eABP_CHARISMA			:
			iPowerValue	=	_lpAsker->getCharisma();
			break;
		case	eABP_LUCK				:
			iPowerValue	=	_lpAsker->getLuck();
			break;
		case	eABP_ATTACK				:
			iPowerValue	=	_lpAsker->getPhysicalAttackPower(NULL,0,FALSE,FALSE,NULL,FALSE,NULL);
			break;
		case	eABP_DEFENSE			:
			iPowerValue	=	_lpAsker->getDefensivePower();
			break;
		case	eABP_FIRE_RESIST		:
			iPowerValue	=	_lpAsker->getFireResistance();
			break;
		case	eABP_WATER_RESIST		:
			iPowerValue	=	_lpAsker->getWaterResistance();
			break;
		case	eABP_WIND_RESIST		:
			iPowerValue	=	_lpAsker->getWindResistance();
			break;
		case	eABP_EARTH_RESIST		:
			iPowerValue	=	_lpAsker->getEarthResistance();
			break;
		case	eABP_LIGHT_RESIST		:
			iPowerValue	=	_lpAsker->getLightResistance();
			break;
		case	eABP_DARK_RESIST		:
			iPowerValue	=	_lpAsker->getDarkResistance();
			break;
		case	eABP_MAGIC_RESIST		:
			iPowerValue	=	_lpAsker->getAllMagicResistance();
			break;
	}

	if (iMethod	==	0	&&	iValue	==	iPowerValue)	return	TRUE;
	if (iMethod	==	1	&&	iValue	>=	iPowerValue)	return	TRUE;
	if (iMethod	==	2	&&	iValue	<=	iPowerValue)	return	TRUE;

	return	FALSE;
}

BOOL
CKarmaItem::checkBookCompletePercentage(cACTOR *_lpAsker)
{
	int		iBook			=	m_aValue[0];
	int		iCheckPercentage=	m_aValue[1];
	int		iCheckMethod	=	m_aValue[2];
//	" [book]0의 완성도가 [spin]1[0,10000]％ [equal]2"											,
//	{"이다","이하이다","이상이다","이 아니다","end"},								//	eKARMA_KW_EQUAL			,		//	이상이다. 이하이다. 이다.
	int		iPercentage		=	_lpAsker->getBookCompletePercentage(iBook);

	if	(iPercentage	==	-1)
		return	FALSE;

	if	(iCheckMethod	==	0)
	{
		if	(iPercentage	==	iCheckPercentage)
			return	TRUE;

		return	FALSE;
	}
	if	(iCheckMethod	==	1)
	{
		if	(iPercentage	>=	iCheckPercentage)
			return	TRUE;

		return	FALSE;
	}
	if	(iCheckMethod	==	2)
	{
		if	(iPercentage	<=	iCheckPercentage)
			return	TRUE;

		return	FALSE;
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkExistBook(cACTOR *_lpAsker)
{
	int		iBook			=	m_aValue[0];
	int		iCheckMethod	=	m_aValue[1];
//	" [book]0이 [exist_not_exist]1"																,
//	{"있다","없다","end"},
	int		iPercentage		=	_lpAsker->getBookCompletePercentage(iBook);

	if	(iCheckMethod	==	0)
	{
		if	(iPercentage	==	-1)
			return	FALSE;

		return	TRUE;
	}
	if	(iCheckMethod	==	1)
	{
		if	(iPercentage	!=	-1)
			return	FALSE;

		return	TRUE;
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkExistPage(cACTOR *_lpAsker)
{
//	" 책 [book]0에 카테고리 [spin]1[1,18]번에 페이지 [spin]2[1,8]이 [exist_not_exist]3."		,
	int		iBook		=	m_aValue[0];
	int		iCategory	=	m_aValue[1]-1;
	int		iPage		=	m_aValue[2]-1;
	int		iCheckMethod=	m_aValue[3];

	BOOL	bIsAvailPage=	_lpAsker->isAvailBookPage(iBook,iCategory,iPage);

	if	(iCheckMethod	==	0)
		return	bIsAvailPage;

	return	1-bIsAvailPage;
}

//	책에 페이지 추가하는 아이템 체크
BOOL
CKarmaItem::checkExistAddBookPageItem(cACTOR *_lpAsker)
{
	int		iBook		=	m_aValue[0];
	int		iCategory	=	m_aValue[1]-1;
	int		iPage		=	m_aValue[2]-1;
	int		iCheckMethod=	m_aValue[3];

	BOOL	bIsExistAddBookPageItem	=	FALSE;
	
	if	(_lpAsker->getAddBookPageItem(iBook,iCategory,iPage))
		bIsExistAddBookPageItem	=	TRUE;

	if	(iCheckMethod	==	0)
		return	bIsExistAddBookPageItem;

	return	1-bIsExistAddBookPageItem;
}

//	현재 상태 체크
BOOL
CKarmaItem::checkCurrentStatus(cACTOR *_lpAsker)
{
	int		iCheckStatus	=	m_aValue[0];
	int		iOnOff			=	m_aValue[1];

	switch(iCheckStatus)
	{
		case	0	:	//	양탄자 타고 있는지 체크
		{
			if	(iOnOff	==	0)
			{
				if	(_lpAsker->m_bf5MagicCarpetShape)
					return	TRUE;
			}
			else
			{
				if	(_lpAsker->m_bf5MagicCarpetShape)
					return	FALSE;
			}
			break;
		}
	}

	return	FALSE;
}

//
//	파티의 리더냐?
BOOL
CKarmaItem::checkIsPartyLeader(cACTOR *_lpAsker)
{
	if	(_lpAsker	==	NULL)
		return	FALSE;

	int		iMethod			=	m_aValue[0];
//	{"파티의 리더이다.","파티의 리더가 아니다.","end"},					//	eKARMA_KW_IS_PARTY_LEADER

	BOOL	isPartyLeader	=	_lpAsker->isPartyLeader();

	if	(iMethod	==	0)
		return	isPartyLeader;

	return	1-isPartyLeader;
}

//
//	기본 능력치 체크
BOOL
CKarmaItem::checkPartyMemberCount(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 파티원이 [spin]0[1,8]명 [equal]1"															,
//	{"이다","이하이다","이상이다","이 아니다","end"},								//	eKARMA_KW_EQUAL			,		//	이상이다. 이하이다. 이다.
	int	iMemberCount=	m_aValue[0];
	int	iMethod		=	m_aValue[1];

	if (_lpAsker->getPartySerial()	==	0xffff)
	{
		if (iMemberCount	==	0)
			return	TRUE;

		return	FALSE;
	}

	cParty	party;

	if	(!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))
		return	FALSE;

	int	iFieldMemberCount	=	0;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		if (party.m_aMember[i].m_iZoneSerial	==	0xffffffff)
			continue;

		iFieldMemberCount++;
	}

	if (iMethod	==	0 && iMemberCount == iFieldMemberCount)	return	TRUE;
	if (iMethod	==	1 && iMemberCount >= iFieldMemberCount)	return	TRUE;
	if (iMethod	==	2 && iMemberCount <= iFieldMemberCount)	return	TRUE;
	if (iMethod	==	3 && iMemberCount != iFieldMemberCount)	return	TRUE;

	return	FALSE;
}

BOOL
CKarmaItem::checkPartyMemberCountInCurrentField(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 현재 필드에 있는 파티원이 [spin]0[0,8]명 [equal]1"	,															,
//	{"이다","이하이다","이상이다","이 아니다","end"},								//	eKARMA_KW_EQUAL			,		//	이상이다. 이하이다. 이다.
	int	iMemberCount=	m_aValue[0];
	int	iMethod		=	m_aValue[1];

	if (_lpAsker->getPartySerial()	==	0xffff)
	{
		if (iMemberCount	==	0)
			return	TRUE;

		return	FALSE;
	}

	cParty	party;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))
		return	FALSE;

	int	iFieldMemberCount	=	0;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);
		if(lpMember && lpMember->isOnSameField(_lpAsker))
			iFieldMemberCount++;
	}

	if (iMethod	==	0 && iMemberCount == iFieldMemberCount)	return	TRUE;
	if (iMethod	==	1 && iMemberCount >= iFieldMemberCount)	return	TRUE;
	if (iMethod	==	2 && iMemberCount <= iFieldMemberCount)	return	TRUE;
	if (iMethod	==	3 && iMemberCount != iFieldMemberCount)	return	TRUE;

	return	FALSE;
}


//
//	파티 멤버 레벨
BOOL
CKarmaItem::checkPartyMemberLevel(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	파티원의 레벨이 [spin]0[1,1000]~[spin]1[1,1000]이다.
	int	iLevel1	=	m_aValue[0];
	int	iLevel2	=	m_aValue[1];

	if (_lpAsker->getPartySerial()	==	0xffff)
		return FALSE;

	cParty	party;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))
		return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		if (party.m_aMember[i].m_bf10Level	<	(DWORD)iLevel1	||	party.m_aMember[i].m_bf10Level	>	(DWORD)iLevel2	)
			return	FALSE;
	}

	return	TRUE;
}

//
//	파티 멤버들의 길드 가입 여부
BOOL
CKarmaItem::checkPartyMembersAssignedGuild(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 모든 파티원이 길드에 [guild_member]0"														,
//	{"가입해 있다.","가입해 있지 않다.","end"},							//	eKARMA_KW_GUILD_MEMBER
	int	iMethod	=	m_aValue[0];

	if (_lpAsker->getPartySerial()	==	0xffff)	return FALSE;

	cParty	party;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))
		return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember)
			return	FALSE;
		if (lpMember->isOnSameField(_lpAsker)==FALSE)
			return	FALSE;

		if (iMethod	==	0	&&	lpMember->m_wGuildSerial	==	0xffff) 
			return	FALSE;
		if (iMethod	==	1	&&	lpMember->m_wGuildSerial	!=	0xffff)
			return	FALSE;
	}

	return	TRUE;
}

//
//	길드에 가입해 있는 파티원
BOOL
CKarmaItem::checkAssignedGuildPartyMember(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 길드에 소속해 있는 파티원이 [party_guild_member_exist]0"									,
//	{"있다.","없다","end"},												//	eKARMA_KW_SPECIFIC_ITEM_OWN_MEMBER_EXIST
	int	iMethod	=	m_aValue[0];

	if (_lpAsker->getPartySerial()	==	0xffff)	return FALSE;

	cParty	party;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))	return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember													)
		{
			if (iMethod	==	1)	return	FALSE;
			continue;
		}
		if (lpMember->isOnSameField(_lpAsker)==FALSE)
		{
			if (iMethod	==	1)
				return	FALSE;
			continue;
		}

		if (iMethod	==	0	&&	lpMember->m_wGuildSerial	!=	0xffff) return	TRUE;
		if (iMethod	==	1	&&	lpMember->m_wGuildSerial	==	0xffff) return	TRUE;
	}

	return	FALSE;
}

//
//	파티원이 모두 같은 길드에 가입해 있냐?
BOOL
CKarmaItem::checkAllPartyMembersAssignedSameGuild(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 모든 파티원이 [all_party_member_in_same_guild]0"
//	{"같은 길드에 가입해 있다.","서로 다른 길드에 가입해 있다.","end"},	//	eKARMA_KW_ALL_PARTY_MEMBER_IN_SAME_GUILD
	int		iMethod	=	m_aValue[0];
	int		iGuild;
	cParty	party;

	if (_lpAsker->getPartySerial()	==	0xffff)	return FALSE;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))	return	FALSE;
	if	(_lpAsker->m_wGuildSerial	==	0xffff		)	return	FALSE;

	iGuild	=	_lpAsker->m_wGuildSerial;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember	)
			return	FALSE;
		if (!lpMember->isOnSameField(_lpAsker))
			return	FALSE;
		if (lpMember->m_wGuildSerial	==	0xffff	)
			return	FALSE;

		if (iMethod	==	0	&&	iGuild	!=	lpMember->m_wGuildSerial)	return	FALSE;
		if (iMethod	==	1	&&	iGuild	==	lpMember->m_wGuildSerial)	return	FALSE;
	}

	return	TRUE;
}

//
//	파티원이 소지 하고 있는 아이템 체크
BOOL
CKarmaItem::checkAllPartyMembersOwnItem(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 모든 파티원이 [item]0을 [spin]1[0,255]개 이상 보유하고 있다."								,
	int		iItem	=	m_aValue[0];
	int		iCount	=	m_aValue[1];

	cParty	party;

	if (_lpAsker->getPartySerial()	==	0xffff)	return FALSE;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))	return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember	)
			return	FALSE;
		if (!lpMember->isOnSameField(_lpAsker))
			return	FALSE;
		if (lpMember->getItemCount(iItem)	< iCount)
			return	FALSE;
	}

	return	TRUE;
}

//
//	파티원 모두가 특정 아이템을 소지하고 있지 않다.
BOOL
CKarmaItem::checkAllPartyMembersNotOwnItem(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 모든 파티원이 [item]0을 보유하고 있지 않다."												,
	int		iItem	=	m_aValue[0];

	cParty	party;

	if (_lpAsker->getPartySerial()	==	0xffff)	return FALSE;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))	return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember	)
			return	FALSE;
		if (!lpMember->isOnSameField(_lpAsker))
			return	FALSE;

		if (lpMember->getItemCount(iItem))
			return	FALSE;
	}

	return	TRUE;
}

//
//	파티원 모두가 특정 아이템을 소지하고 있지 않다.
BOOL
CKarmaItem::checkPartyOwnItem(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" [item]0을 [spin]1[1,255]개 이상 보유한 파티원이 [specific_item_own_member_exist]2"		,
//	{"있다.","없다","end"},
	int	iItem	=	m_aValue[0];
	int	iCount	=	m_aValue[1];
	int	iMethod	=	m_aValue[2];

	cParty	party;

	if (_lpAsker->getPartySerial()	==	0xffff)	return FALSE;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))	return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember													)
		{
			if (iMethod	==	1)	return	FALSE;
			continue;
		}
		if (!lpMember->isOnSameField(_lpAsker))
		{
			if (iMethod	==	1)	return	FALSE;
			continue;
		}

		if (iMethod	==	0	&&	lpMember->getItemCount(iItem) >= iCount	)	return	TRUE;
		if (iMethod	==	1	&&	lpMember->getItemCount(iItem) >= iCount	)	return	FALSE;
	}

	if (iMethod	==	1)	return	TRUE;

	return	FALSE;
}

//
//	파티원 모두의 골드를 체크한다.
BOOL
CKarmaItem::checkGoldSumOfPartyMembers(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 파티원의 소지 금액 합이 [number]0[1,2147483647]골드 [equal]1"								,
//	{"이다","이하이다","이상이다","end"},								//	eKARMA_KW_EQUAL			,		//	이상이다. 이하이다. 이다.

	DWORD	dwGold		=	(DWORD)m_aValue[0];
	int		iMethod		=	m_aValue[1];
	DWORD	dwGoldSum	=	0;

	cParty	party;

	if (_lpAsker->getPartySerial()	==	0xffff)	return FALSE;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))	return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember													)
			return	FALSE;
		if (!lpMember->isOnSameField(_lpAsker))
			return	FALSE;

		dwGoldSum	+=	lpMember->m_iGold;
	}

	if (iMethod	==	0	&&	dwGold	==	dwGoldSum)	return	TRUE;
	if (iMethod	==	1	&&	dwGold	>=	dwGoldSum)	return	TRUE;
	if (iMethod	==	2	&&	dwGold	<=	dwGoldSum)	return	TRUE;

	return	FALSE;
}

//
//	모든 파티원의 골드 소지량 검사
BOOL
CKarmaItem::checkGoldOfAllPartyMembers(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 모든 파티원이 [number]0[1,2147483647]골드 이상 소지하고 있다."							,
	int		dwGold	=	m_aValue[0];

	cParty	party;

	if (_lpAsker->getPartySerial()	==	0xffff)	return FALSE;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))	return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember)
			return	FALSE;
		if (!lpMember->isOnSameField(_lpAsker))
			return	FALSE;

		if (lpMember->m_iGold	<	dwGold)	return	FALSE;
	}

	return	TRUE;
}

//
//	파티원의 골드 소지량 검사
BOOL
CKarmaItem::checkOwnGoldOfPartyMember(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" [number]0[1,2147483647]골드 이상 소지하고 있는 파티원이 [exist]1"							,
//	{"있다.","없다","end"},
	int		iGold		=	m_aValue[0];
	int		iMethod		=	m_aValue[1];

	cParty	party;

	if (_lpAsker->getPartySerial()	==	0xffff)	return FALSE;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))	return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember													)
		{
			if (iMethod	==	1)	return	FALSE;
			continue;
		}
		if (!lpMember->isOnSameField(_lpAsker))
		{
			if (iMethod	==	1)
				return	FALSE;
			continue;
		}

		if (iMethod	==	0	&&	iGold	<=	lpMember->m_iGold		)	return	TRUE;
		if (iMethod	==	1	&&	iGold	<=	lpMember->m_iGold		)	return	FALSE;
	}

	if (iMethod	==	1	)	return	TRUE;

	return	FALSE;
}

//
//파티원들의 스킬 체크
BOOL
CKarmaItem::checkPartyMembersSkillLevel(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 레벨 [spin]0[0,50]이상의 [skill]1이 있는 파티원이 [exist]2"								,
//	{"있다.","없다","end"},
	int	iLevel		=	m_aValue[0];
	int	iSkill		=	m_aValue[1];
	int	iMethod		=	m_aValue[2];

	cParty	party;

	if (_lpAsker->getPartySerial()	==	0xffff)	return FALSE;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))	return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember													)
		{
			if (iMethod	==	1)	return	FALSE;
			continue;
		}
		if (!lpMember->isOnSameField(_lpAsker))
		{
			if (iMethod	==	1)
				return	FALSE;
			continue;
		}

		cAbility	*lpAbility	=	lpMember->getAbilityBySkill(iSkill);

		if (!lpAbility)	continue;

		if (iMethod	==	0	&&	iLevel	<=	lpAbility->m_wLevel		)	return	TRUE;
		if (iMethod	==	1	&&	iLevel	<=	lpAbility->m_wLevel		)	return	FALSE;
	}

	if (iMethod	==	1	)	return	TRUE;

	return	FALSE;
}

//
//모든 파티멤버의 스킬 체크
BOOL
CKarmaItem::checkAllPartyMembersSkill(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 모든 파티원이 레벨 [spin]0[0,50]이상의 [skill]1을 익히고 있다."							,
	int	iLevel		=	m_aValue[0];
	int	iSkill		=	m_aValue[1];

	cParty	party;

	if (_lpAsker->getPartySerial()	==	0xffff)
		return FALSE;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))
		return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember	)
			return	FALSE;
		if (!lpMember->isOnSameField(_lpAsker))
			return	FALSE;

		cAbility	*lpAbility	=	lpMember->getAbilityBySkill(iSkill);

		if (!lpAbility)	return	FALSE;

		if (iLevel	>	lpAbility->m_wLevel		)	return	FALSE;
	}

	return	TRUE;
}

//
//	파티안에 타이틀 체크
BOOL
CKarmaItem::checkTitleInPartyMember(cACTOR *_lpAsker)
{
	if	(_lpAsker	==	NULL)
		return	FALSE;

	int	iTitleLevel	=	m_aValue[0];
	int	iTitle		=	m_aValue[1];
	int	iMethod		=	m_aValue[2];
//	" [spin]0[1,10]레벨 이상의 [title]1 칭호를 가진 파티원이 [exist]2"							,
//	{"있다.","없다","end"},
	cParty	party;

	if	(_lpAsker->getPartySerial()	==	0xffff)
		return FALSE;

	if	(!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))
		return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if	(!lpMember	)
		{
			if	(iMethod	==	1)
				return	FALSE;

			continue;
		}

		if	(!lpMember->isOnSameField(_lpAsker))
		{
			if	(iMethod	==	1)
				return	FALSE;

			continue;
		}

		int		iChacterTitleLevel	=	0;

		CPlayerTitleInfo*	lpTitle	=	lpMember->getTitle(iTitle);

		if	(lpTitle)
			iChacterTitleLevel		=	lpTitle->m_bLevel;

		if	(iMethod	==	0	&&	iChacterTitleLevel	>=	iTitleLevel)
			return	TRUE;
	}

	if	(iMethod	==	1	)
		return	TRUE;

	return	FALSE;
}

//
//	모든 파티원의 칭호 체크
BOOL
CKarmaItem::checkAllPartyMembersTitle(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
	int	iTitleLevel	=	m_aValue[0];
	int	iTitle		=	m_aValue[1];
	int	iOwn		=	m_aValue[2];
//	" 모든 파티원이 [spin]0[1,10]레벨 이상의 [title]1 칭호를 [own]2"
//	{"보유하고 있다.","보유하고 있지 않다.","end"},						//	eKARMA_KW_OWN

	cParty	party;

	if (_lpAsker->getPartySerial()	==	0xffff)
		return FALSE;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))
		return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember		)
			return	FALSE;
		if (!lpMember->isOnSameField(_lpAsker))
			return	FALSE;

		CPlayerTitleInfo*	lpTitle	=	lpMember->getTitle(iTitle);

		if	(lpTitle	==	NULL)
		{
			if (iOwn	==	0)
				return	FALSE;
			continue;
		}

		if	(lpTitle->m_bLevel	<	iTitleLevel)
		{
			if (iOwn	==	0)
				return	FALSE;
		}
		else
		{
			if (iOwn	==	1)
				return	FALSE;
		}
	}

	return	TRUE;	//	모두 칭호를 가지고 있거나 모두 가지고 있지 않다.
}

//
//	파티 멤버의 직업 체크
BOOL
CKarmaItem::checkPartyMemberJob(cACTOR *_lpAsker)
{
	if	(_lpAsker	==	NULL)
		return	FALSE;

	int	iJob		=	m_aValue[0];
	int	iExist		=	m_aValue[1];
	int	iInclude	=	m_aValue[2];
	int	iCheckJob	=	iJob;

	if	(iCheckJob	>=	18)	//	직업이 16개 이후로 추가 안될거라고 단정하고 만들어서 이상하게 되어 있음 - -
		iCheckJob	-=	2;	//	자세한 이유를 보려면 망치 코드까지 뒤져 볼것

//	" 직업이 [player_job]0인 파티원이 [exist]1. [include_transformation_job]2"
//	{"있다.","없다","end"},												//	eKARMA_KW_EXIST
	cParty	party;

	if	(_lpAsker->getPartySerial()	==	0xffff)
		return FALSE;

	if	(!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))
		return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);
		
		if	(!lpMember	)
		{
			if (iExist	==	1)	return	FALSE;
			continue;
		}

		if	(!lpMember->isOnSameField(_lpAsker))
		{
			if (iExist	==	1)
				return	FALSE;
			continue;
		}

		BOOL	bIsSameJob	=	FALSE;

		if	(iJob	==	16)
		{
			if	(c_aIsMale[_lpAsker->m_wJob])
				bIsSameJob	=	TRUE;
		}
		else
		if	(iJob	==	17)
		{
			if	(c_aIsMale[_lpAsker->m_wJob]	==	FALSE)
				bIsSameJob	=	TRUE;
		}
		else
		{
			if	(iInclude	==	0)
			{
				if	(lpMember->m_wJob/2	==	iCheckJob/2)
					bIsSameJob	=	TRUE;
			}
			else
			{
				if	(lpMember->m_wJob	==	iCheckJob)
					bIsSameJob	=	TRUE;
			}
		}

		if	(bIsSameJob)
		{
			if	(iExist	==	0)
				return	TRUE;

			if	(iExist	==	1)
				return	FALSE;
		}
	}

	if	(iExist	==	0)
		return	FALSE;

	return	TRUE;
}

BOOL
CKarmaItem::checkDeahtAllPartyMember(cFIELD *_lpField)
{
	if	(_lpField	==	NULL)
		return	TRUE;

	cInstanceField	*lpIF	=	_lpField->getMasterInstanceField();

	if	(!lpIF)
		return	TRUE;

	if	(lpIF->m_iAlivePlayerCount	<=	0)
		return	TRUE;

	return	FALSE;
}

BOOL
CKarmaItem::checkAllPartyMembersJob(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 모든 파티원의 직업이 [player_job]0이다. [include_transformation_job]1"						,
	int	iJob		=	m_aValue[0];
	int	iInclude	=	m_aValue[1];
	int	iCheckJob	=	iJob;

	if	(iCheckJob	>=	18)	//	직업이 16개 이후로 추가 안될거라고 단정하고 만들어서 이상하게 되어 있음 - -
		iCheckJob	-=	2;	//	자세한 이유를 보려면 망치 코드까지 뒤져 볼것

	cParty	party;

	if	(_lpAsker->getPartySerial()	==	0xffff)
		return FALSE;

	if	(!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))
		return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if	(!lpMember			)
			return	FALSE;

		if	(!lpMember->isOnSameField(_lpAsker))
			return	FALSE;

		BOOL	bIsSameJob	=	FALSE;

		if	(iJob	==	16)
		{
			if	(c_aIsMale[_lpAsker->m_wJob])
				bIsSameJob	=	TRUE;
		}
		else
		if	(iJob	==	17)
		{
			if	(c_aIsMale[_lpAsker->m_wJob]	==	FALSE)
				bIsSameJob	=	TRUE;
		}
		else
		{
			if	(iInclude	==	0)
			{
				if	(lpMember->m_wJob/2	==	iCheckJob/2)
					bIsSameJob	=	TRUE;
			}
			else
			{
				if	(lpMember->m_wJob	==	iCheckJob)
					bIsSameJob	=	TRUE;
			}
		}

		if	(!bIsSameJob)
			return	FALSE;
	}

	return	TRUE;
}

//
//파티멤버의 성향 체크
BOOL
CKarmaItem::checkPartyMembersAllignment(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 성향이 [allignment]0인 파티원이 [exist]1"													,
//	{"절대 악","악","중립","선","절대 선","end"},						//	eKARMA_KW_ALLIGNMENT	,
//	{"있다.","없다","end"},
	int	iCheckAllignment=	m_aValue[0];
	int	iMethod			=	m_aValue[1];

	cParty	party;

	if (_lpAsker->getPartySerial()	==	0xffff)	return FALSE;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))	return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember													)
		{
			if (iMethod	==	1)	return	FALSE;
			continue;
		}
		if (!lpMember->isOnSameField(_lpAsker))
		{
			if (iMethod	==	1)
				return	FALSE;
			continue;
		}

		int	iAllignment	=	lpMember->getAllignment();

		if(iMethod	==	0	&&	iCheckAllignment+1	==	iAllignment)	return	TRUE;
		if(iMethod	==	1	&&	iCheckAllignment+1	==	iAllignment)	return	FALSE;
	}

	if (iMethod	==	1	)	return	TRUE;

	return	FALSE;
}

//
//모든 파티멤버의 성향 체크
BOOL
CKarmaItem::checkAllPartyMembersAllignment(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 모든 파티원의 성향이 [allignment]0이다."													,
	int	iCheckAllignment=	m_aValue[0];

	cParty	party;

	if (_lpAsker->getPartySerial()	==	0xffff)	return FALSE;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))	return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember													)	return	FALSE;
		if (!lpMember->isOnSameField(_lpAsker))
			return	FALSE;

		int	iAllignment	=	lpMember->getAllignment();

		if	(iCheckAllignment+1	!=	iAllignment)	return	FALSE;
	}

	return	TRUE;
}

//
//파티멤버의 기본 능력치 체크
BOOL
CKarmaItem::checkPartyMembersBasicPower(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" [power]0가 [spin]1[1,10000]이상인 파티원이 [exist]2"										,
//	{"있다.","없다","end"},

	int	iPowerType	=	m_aValue[0];
	int	iValue		=	m_aValue[1];
	int	iMethod		=	m_aValue[2];
	int	iPowerValue;

	cParty	party;

	if (_lpAsker->getPartySerial()	==	0xffff)	return FALSE;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))	return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember													)
		{
			if (iMethod	==	1)	return	FALSE;
			continue;
		}
		if (!lpMember->isOnSameField(_lpAsker))
		{
			if (iMethod	==	1)
				return	FALSE;
			continue;
		}

		iPowerValue	=	lpMember->getBasicPowerValue(iPowerType);

		if (iMethod	==	0	&&	iValue	<=	iPowerValue	)	return	TRUE;
		if (iMethod	==	1	&&	iValue	<=	iPowerValue	)	return	FALSE;
	}

	if (iMethod	==	1)	return	TRUE;

	return	FALSE;
}

//
//모든 파티멤버의 기본 능력치 체크
BOOL
CKarmaItem::checkAllPartyMembersBasicPower(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 모든 파티원의 [power]0가 [spin]1[1,10000] [equal]2"										,
//	{"이다","이하이다","이상이다","end"},								//	eKARMA_KW_EQUAL			,		//	이상이다. 이하이다. 이다.

	int	iPowerType	=	m_aValue[0];
	int	iValue		=	m_aValue[1];
	int	iMethod		=	m_aValue[2];
	int	iPowerValue;

	cParty	party;

	if (_lpAsker->getPartySerial()	==	0xffff)	return FALSE;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))	return	FALSE;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember													)	return	FALSE;
		if (!lpMember->isOnSameField(_lpAsker))
			return	FALSE;

		iPowerValue	=	lpMember->getBasicPowerValue(iPowerType);

		if (iMethod	==	0	&&	iValue	!=	iPowerValue	)	return	FALSE;
		if (iMethod	==	1	&&	iValue	<	iPowerValue	)	return	FALSE;
		if (iMethod	==	2	&&	iValue	>	iPowerValue	)	return	FALSE;
	}

	return	TRUE;
}

//
//	모든 파티 멤버의 위치 체크
BOOL
CKarmaItem::checkPartyMembersLocation(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 모든 파티원이 [party_member_location]0"													,
//	{"모여있다.","같은 필드 안에 있다.","end"},							//	eKARMA_KW_PARTY_MEMBER_LOCATION
	int		iMethod		=	m_aValue[0],i;

	cParty	party;

	if (_lpAsker->getPartySerial()	==	0xffff)
		return FALSE;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))
		return	FALSE;

	for (i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember													)
			return	FALSE;

		if (!lpMember->isOnSameField(_lpAsker))
			return	FALSE;
	}

	if	(iMethod	==	1)
		return	TRUE;	//	그냥 같은 필드 안에 있음 된다.

	cACTOR	*lpLeader		=	party.getActorByMemberIndex(0);

	for (i=1;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpLeader->isTargetInAttackRange(lpMember,dINRANGE_PARTY_MEMBER_DISTANCE_FOR_TRIGGER))
			return	FALSE;
	}

	return	TRUE;
}

//	스킬을 마스터한 수가 [spin]0[0,60]개 이상인 파티 멤버의 수가 [spin]1[0,8]명 [equal]2
BOOL
CKarmaItem::checkHaveMasterSkillPartyMemberCount(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
	int		iCheckSkillCount	=	m_aValue[0];
	int		iCheckMemberCount	=	m_aValue[1];
	int		iMethod				=	m_aValue[2],i;
	int		iMemberCount		=	0;

	cParty	party;

	if (_lpAsker->getPartySerial()	==	0xffff)
		return FALSE;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))
		return	FALSE;

	for (i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember	)
			continue;

		if (lpMember->getMasterSkillCount() >= iCheckSkillCount)
			iMemberCount++;
	}

	if (iMethod	==	0)	//	이다
		if (iMemberCount==	iCheckMemberCount)	return	TRUE;

	if (iMethod	==	1)	//	이하다
		if (iMemberCount<=	iCheckMemberCount)	return	TRUE;

	if (iMethod	==	2)	//	이상이다
		if (iMemberCount>=	iCheckMemberCount)	return	TRUE;

	return	FALSE;
}

//	모든 파티원이 [spin]0[0,60]개 [only_equal]1의 스킬을 마스터 했다.
BOOL
CKarmaItem::checkAllPartyMembersMasterSkillCount(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
	int		iCheckSkillCount	=	m_aValue[0];
	int		iMethod				=	m_aValue[1],i;

	cParty	party;

	if (_lpAsker->getPartySerial()	==	0xffff)	return FALSE;

	if (!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))	return	FALSE;

	for (i=0;i<party.m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember	)
			return	FALSE;

		if (iMethod	==	0)	//	이상
		{
			if (lpMember->getMasterSkillCount() < iCheckSkillCount)	//	적다
				return	FALSE;
		}
		else
		if (iMethod	==	1)	//	이하
		{
			if (lpMember->getMasterSkillCount() > iCheckSkillCount)	//	많다
				return	FALSE;
		}
	}

	return	TRUE;
}
BOOL
CKarmaItem::checkQuestCountdownTimer(cACTOR *_lpAsker,int _iQuestType)
{
	if	(_lpAsker	==	NULL)
		return	FALSE;
//	" 퀘스트 [quest]0을 [체커가 체크되어 있다.]1"
//	"보유하고 있다.","보유하고 있지 않다."
	int		iQuestIndex			=	m_aValue[0];
	int		iCheckStatus		=	m_aValue[1];
	int		iTarget				=	m_aValue[2];
	BOOL	bIsCheckedChecker	=	FALSE;
	int		iCountdownStatus;
//	{"완료되었다.","아직 완료되지 않았다.","취소됐다.","end",},			//	eKARMA_KW_QUEST_COUNTDOWN_CHECK

//	{"타겟","모든 파티 멤버","파티 리더","파티원중 아무나 하나","end"},	//	eKARMA_KW_QUEST_TARGET
//	{"완료 체커가 체크되어 있다.","완료 체커가 체크되어 있지 않다.","end"},	//	eKARMA_KW_QUEST_COMPLETE_CHECKER

	switch(iTarget)
	{
		case	0	:
		{
			iCountdownStatus	=	_lpAsker->getQuestCountdownTimerStatus(_iQuestType,iQuestIndex);

			if	(iCheckStatus	==	iCountdownStatus)
				return	TRUE;

			return	FALSE;
		}

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if	(!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					return	FALSE;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				iCountdownStatus	=	lpMember->getQuestCountdownTimerStatus(_iQuestType,iQuestIndex);

				if (iCheckStatus	!=	iCountdownStatus)
					return	FALSE;
			}

			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if (!lpLeader)
				return	FALSE;

			if (!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			iCountdownStatus	=	lpLeader->getQuestCountdownTimerStatus(_iQuestType,iQuestIndex);

			if (iCheckStatus	==	iCountdownStatus)
				return	TRUE;

			return	FALSE;
		}

		case	3	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					continue;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				iCountdownStatus	=	lpMember->getQuestCountdownTimerStatus(_iQuestType,iQuestIndex);

				if	(iCheckStatus	==	iCountdownStatus)
					return	TRUE;
			}

			return	FALSE;
		}
	}

	return	FALSE;

}

BOOL
CKarmaItem::isCheckedQuestCompleteChecker(cACTOR *_lpAsker,int _iQuestType)
{
	if	(_lpAsker	==	NULL)
		return	FALSE;
//	" 퀘스트 [quest]0을 [체커가 체크되어 있다.]1"
//	"보유하고 있다.","보유하고 있지 않다."
	int		iQuestIndex			=	m_aValue[0];
	int		iMethod				=	m_aValue[1];
	int		iTarget				=	m_aValue[2];
	BOOL	bIsCheckedChecker	=	FALSE;
//	{"타겟","모든 파티 멤버","파티 리더","파티원중 아무나 하나","end"},	//	eKARMA_KW_QUEST_TARGET
//	{"완료 체커가 체크되어 있다.","완료 체커가 체크되어 있지 않다.","end"},	//	eKARMA_KW_QUEST_COMPLETE_CHECKER

	switch(iTarget)
	{
		case	0	:
		{
			bIsCheckedChecker		=	_lpAsker->isCheckedQuestCompleteChecker(_iQuestType,iQuestIndex);

			if (iMethod	==	0)
				return	bIsCheckedChecker;

			if (iMethod	==	1)
				if (bIsCheckedChecker)	
					return	FALSE;

			return	TRUE;
		}

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if	(!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					return	FALSE;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				BOOL	bIsCheckedChecker		=	lpMember->isCheckedQuestCompleteChecker(_iQuestType,iQuestIndex);

				if	(iMethod	==	0)
				{
					if	(bIsCheckedChecker	==	FALSE)
						return	FALSE;
				}
				if (iMethod	==	1)
				{
					if	(bIsCheckedChecker)
						return	FALSE;
				}
			}

			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if (!lpLeader)
				return	FALSE;

			if (!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			BOOL	bIsCheckedChecker		=	lpLeader->isCheckedQuestCompleteChecker(_iQuestType,iQuestIndex);

			if (iMethod	==	0)
				return	bIsCheckedChecker;
			if (iMethod	==	1)
				if (bIsCheckedChecker)	
					return	FALSE;

			return	TRUE;
		}

		case	3	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					continue;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				BOOL	bIsCheckedChecker		=	lpMember->isCheckedQuestCompleteChecker(_iQuestType,iQuestIndex);

				if	(iMethod	==	0)
				{
					if (bIsCheckedChecker	==	TRUE)
						return	TRUE;
				}
				if	(iMethod	==	1)
				{
					if	(bIsCheckedChecker)
						return	TRUE;
				}
			}

			return	FALSE;
		}
	}

	return	FALSE;

}

BOOL
CKarmaItem::checkProcessAbleQuest(cACTOR *_lpAsker,int _iQuestType)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
	int		iTarget		=	m_aValue[0];
	int		iQuestIndex	=	m_aValue[1];
	int		iMethod		=	m_aValue[2];

//	" [target_player]0가 [quest]1을 [quest_able]2"			
//	{"타겟","타겟 파티원","end"},										//	eKARMA_KW_TARGET_PLAYER
//	{"할 수 있다.","할 수 없다.","end"},								//	eKARMA_KW_QUEST_ABLE				,

	switch(iTarget)
	{
		case	0	:
		{
			BOOL	bIsComplete	=	_lpAsker->isProcessAbleQuest(_iQuestType,iQuestIndex);

			if (iMethod	==	0)	return	bIsComplete;
			if (iMethod	==	1)	return	1-bIsComplete;

			return	TRUE;
		}

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					return	FALSE;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				BOOL	bIsAble	=	lpMember->isProcessAbleQuest(_iQuestType,iQuestIndex);

				if	(iMethod	==	0)
				{
					if	(bIsAble	==	FALSE)
						return	FALSE;
				}
				else
				if	(iMethod	==	1)
				{
					if	(bIsAble)
						return	FALSE;
				}
			}
			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if	(!lpLeader)
				return	FALSE;

			if	(!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			BOOL	bIsAble	=	lpLeader->isProcessAbleQuest(_iQuestType,iQuestIndex);

			if	(iMethod	==	0)
			{
				if	(bIsAble==	TRUE)
					return	TRUE;

				return	FALSE;
			}
			else
			if	(iMethod	==	1)
			{
				if	(bIsAble==	FALSE)
					return	TRUE;

				return	FALSE;
			}

			return	FALSE;
		}

		case	3	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					continue;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				BOOL	bIsAble	=	lpMember->isProcessAbleQuest(_iQuestType,iQuestIndex);

				if	(iMethod	==	0)
				{
					if	(bIsAble==	TRUE)
						return	TRUE;
				}
				else
				if	(iMethod	==	1)
				{
					if	(bIsAble==	FALSE)
						return	TRUE;
				}
			}

			return	FALSE;
		}
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkCheckGuildPlaceOfRelated(cACTOR *_lpAsker)
{
	if	(_lpAsker	==	NULL)
		return	FALSE;

	int		iField		=	m_aValue[0];
	int		iIdaAnida	=	m_aValue[1];

	CGuild	*lpGuild	=	_lpAsker->getGuild();

	if	(!lpGuild)
	{
		if	(iIdaAnida	==	1)
			return	TRUE;

		return	FALSE;
	}

	BOOL	bIsRelatedField	=	lpGuild->isRelatedPlace(iField);

	if	(iIdaAnida	==	0)
		return	bIsRelatedField;

	return	1-bIsRelatedField;
//	" 필드 [related_place_of_guild]0가 연고지 [ida_anida]1",
}

//	길드홀 레벨 체크
BOOL
CKarmaItem::checkGuildHallLevel(cACTOR *_lpAsker)
{
	if	(_lpAsker	==	NULL)
		return	FALSE;

	int		iLevel			=	m_aValue[0];
	int		iDa_Iha_Isang	=	m_aValue[1];
	int		iHallLevel		=	0;

	CGuild	*lpGuild		=	_lpAsker->getGuild();

	if	(lpGuild)
		iHallLevel	=	lpGuild->m_wHallLevel;

	if	(iDa_Iha_Isang	==	0)
	{
		if	(iHallLevel	==	iLevel)
			return	TRUE;

		return	FALSE;
	}

	if	(iDa_Iha_Isang	==	1)
	{
		if	(iHallLevel	<=	iLevel)
			return	TRUE;

		return	FALSE;
	}

	if	(iDa_Iha_Isang	==	2)
	{
		if	(iHallLevel	>=	iLevel)
			return	TRUE;

		return	FALSE;
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkRebirthCount(cACTOR *_lpAsker)//	전생 횟수 체크.
{
	if	(_lpAsker	==	NULL)
		return	FALSE;
	
//	" 전생 횟수가 [spin]0[0,30]회 [equal]1"							,
	int		iCheckCount		=	m_aValue[0];
	int		iDa_Iha_Isang	=	m_aValue[1];
	int		iRebirthCount	=	_lpAsker->m_bf4RebirthCount;
	
	if	(iDa_Iha_Isang	==	0)
	{
		if	(iRebirthCount	==	iCheckCount)
			return	TRUE;
		
		return	FALSE;
	}
	
	if	(iDa_Iha_Isang	==	1)
	{
		if	(iRebirthCount	<=	iCheckCount)
			return	TRUE;
		
		return	FALSE;
	}
	
	if	(iDa_Iha_Isang	==	2)
	{
		if	(iRebirthCount	>=	iCheckCount)
			return	TRUE;
		
		return	FALSE;
	}
	
	return	FALSE;
}

BOOL
CKarmaItem::checkRoenGoodWill(cACTOR *_lpAsker)
{
	if	(_lpAsker	==	NULL)
		return	FALSE;
	
	int		iValue			=	m_aValue[0];
	int		iDa_Iha_Isang	=	m_aValue[1];
	int		iGoodWill		=	0;
	
	CGuild	*lpGuild		=	_lpAsker->getGuild();
	
	if	(!lpGuild)
		return FALSE;

	iGoodWill	=	lpGuild->m_bf3GuildGoodwillForRoen;
	
	if	(iDa_Iha_Isang	==	0)
	{
		if	(iGoodWill	==	iValue)
			return	TRUE;
		
		return	FALSE;
	}
	
	if	(iDa_Iha_Isang	==	1)
	{
		if	(iGoodWill	<=	iValue)
			return	TRUE;
		
		return	FALSE;
	}
	
	if	(iDa_Iha_Isang	==	2)
	{
		if	(iGoodWill	>=	iValue)
			return	TRUE;
		
		return	FALSE;
	}
	
	return	FALSE;
}

BOOL
CKarmaItem::checkIsOwnGuildHall(cACTOR *_lpAsker)
{
	if	(_lpAsker	==	NULL)
		return	FALSE;

	int		iIdaAnida	=	m_aValue[0];

	CGuild	*lpGuild	=	_lpAsker->getGuild();

	if	(!lpGuild)
	{
		if	(iIdaAnida	==	1)
			return	TRUE;

		return	FALSE;
	}

	if	(_lpAsker->m_lpField->m_bIs1LevelGuildHall	||	_lpAsker->m_lpField->m_wHallOwnGuild	==	_lpAsker->m_wGuildSerial)
		return	1-iIdaAnida;

	return	iIdaAnida;
//	"  현재 필드가 소속 길드의 길드홀 [ida_anida]0",
}

BOOL
CKarmaItem::checkGuildStatueSkillLevel(cACTOR *_lpAsker)
{
//	{"이다","이하이다","이상이다","end"},								//	eKARMA_KW_EQUAL			,		//	이상이다. 이하이다. 이다.
	if	(_lpAsker	==	NULL)
		return	FALSE;

	int		iSkillIndex	=	m_aValue[0];
	int		iValue		=	m_aValue[1];
	int		iMethod		=	m_aValue[2];

	CGuild	*lpGuild	=	_lpAsker->getGuild();

	if	(!lpGuild)
		return	FALSE;

	int		iSkillLevel	=	lpGuild->m_abGuildSkill[iSkillIndex];

	if	(iMethod	==	0)	//	이다
	{
		if	(iSkillLevel	==	iValue)
			return	TRUE;

		return	FALSE;
	}
	if	(iMethod	==	1)	//	이하이다
	{
		if	(iSkillLevel	<=	iValue)
			return	TRUE;

		return	FALSE;
	}
	if	(iMethod	==	2)	//	이상이다
	{
		if	(iSkillLevel	>=	iValue)
			return	TRUE;

		return	FALSE;
	}

	return	FALSE;
}

//
//	완료한 퀘스트 체크
BOOL
CKarmaItem::checkCompleteQuest(cACTOR *_lpAsker,int _iQuestType)
{
	if	(_lpAsker	==	NULL)
		return	FALSE;
//	" 퀘스트 [quest]0을 [quest_complete]1"
//	"완료했다.","완료하지 않았다."
	int		iQuestIndex	=	m_aValue[0];
	int		iMethod		=	m_aValue[1];
	int		iTarget		=	m_aValue[2];
//	{"타겟","모든 파티 멤버","파티 리더","파티원중 아무나 하나","end"},	//	eKARMA_KW_QUEST_TARGET

	switch(iTarget)
	{
		case	0	:
		{
			BOOL	bIsComplete	=	_lpAsker->isCompleteQuest(_iQuestType,iQuestIndex);

			if	(iMethod	==	0)
				return	bIsComplete;
			if	(iMethod	==	1)
				return	1-bIsComplete;

			return	TRUE;
		}

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					return	FALSE;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				if	(lpMember->isCompleteQuest(_iQuestType,iQuestIndex) == iMethod)
					return	FALSE;
			}

			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if (!lpLeader)
				return	FALSE;

			if (!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			if (lpLeader->isCompleteQuest(_iQuestType,iQuestIndex) == iMethod)
				return	FALSE;

			return	TRUE;
		}

		case	3	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if (!lpMember)
					continue;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				if	(lpMember->isCompleteQuest(_iQuestType,iQuestIndex) != iMethod)
					return	TRUE;
			}

			return	FALSE;
		}
	}

	return	FALSE;
}

//
//	소유하고 있는 퀘스트 체크
BOOL
CKarmaItem::checkProcessQuest(cACTOR *_lpAsker,int _iQuestType)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 퀘스트 [quest]0을 [quest_own]1"
//	"보유하고 있다.","보유하고 있지 않다."
	int		iQuestIndex	=	m_aValue[0];
	int		iMethod		=	m_aValue[1];
	int		iTarget		=	m_aValue[2];
//	{"타겟","모든 파티 멤버","파티 리더","파티원중 아무나 하나","end"},	//	eKARMA_KW_QUEST_TARGET

	switch(iTarget)
	{
		case	0	:
		{
			BOOL	bIsProcessQuest	=	_lpAsker->isOwnQuest(_iQuestType,iQuestIndex);
			BOOL	bIsComplete		=	FALSE;

			bIsComplete		=	_lpAsker->isCompleteQuest(_iQuestType,iQuestIndex);

			if (iMethod	==	0)
				return	bIsProcessQuest;
			if (iMethod	==	1)
			{
				if (bIsComplete || bIsProcessQuest)	
					return	FALSE;
			}

			return	TRUE;
		}

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					return	FALSE;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				BOOL	bIsProcessQuest	=	lpMember->isOwnQuest(_iQuestType,iQuestIndex);
				BOOL	bIsComplete		=	FALSE;

				bIsComplete		=	lpMember->isCompleteQuest(_iQuestType,iQuestIndex);

				if (iMethod	==	0)
				{
					if	(bIsProcessQuest	==	FALSE)
						return	FALSE;
				}
				if (iMethod	==	1)
				{
					if	(bIsProcessQuest || bIsComplete)
						return	FALSE;
				}
			}

			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if (!lpLeader)
				return	FALSE;

			if (!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			BOOL	bIsProcessQuest	=	lpLeader->isOwnQuest(_iQuestType,iQuestIndex);
			BOOL	bIsComplete		=	FALSE;

			bIsComplete		=	lpLeader->isCompleteQuest(_iQuestType,iQuestIndex);

			if (iMethod	==	0)
				return	bIsProcessQuest;
			if (iMethod	==	1)
			{
				if (bIsComplete || bIsProcessQuest)	
					return	FALSE;
			}

			return	TRUE;
		}

		case	3	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if (!lpMember)
					continue;

				if (!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				BOOL	bIsProcessQuest	=	lpMember->isOwnQuest(_iQuestType,iQuestIndex);
				BOOL	bIsComplete		=	FALSE;

				bIsComplete		=	lpMember->isCompleteQuest(_iQuestType,iQuestIndex);

				if	(iMethod	==	0)
				{
					if	(bIsProcessQuest	==	TRUE)
						return	TRUE;
				}
				if	(iMethod	==	1)
				{
					if	(bIsProcessQuest == FALSE && bIsComplete == FALSE)
						return	TRUE;
				}
			}

			return	FALSE;
		}
	}

	return	FALSE;
}

//
//	진행중인 퀘스트의 상태BOOL
BOOL
CKarmaItem::checkQuestProcessLevel(cACTOR *_lpAsker,int _iQuestType)
{
	if (_lpAsker	==	NULL)
		return	FALSE;

	g_debugSign.m_iTrigerErrorDetail = 1;

//	" 퀘스트 [quest]0의 진행 상태가 [quest_process_level]1 이다."								,
	int	iQuest				=	m_aValue[0];
	int	iCheckProcessLevel	=	m_aValue[1];
	int	iTarget				=	m_aValue[2];
//	{"타겟","모든 파티 멤버","파티 리더","파티원중 아무나 하나","end"},	//	eKARMA_KW_QUEST_TARGET

	switch(iTarget)
	{
		case	0	:
		{
			g_debugSign.m_iTrigerErrorDetailValue = 1;

			int	iProcessLevel		=	_lpAsker->getQuestProcessLevel(_iQuestType,iQuest);

			if	(iProcessLevel		==	0xffff)
				return	FALSE;
			if	(iProcessLevel		==	iCheckProcessLevel)
				return	TRUE;

			return	FALSE;
		}

		case	1	:
		{
			g_debugSign.m_iTrigerErrorDetailValue = 2;

			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					return	FALSE;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				if	(lpMember->getQuestProcessLevel(_iQuestType,iQuest)	!=	iCheckProcessLevel)
					return	FALSE;
			}

			return	TRUE;
		}

		case	2	:
		{
			g_debugSign.m_iTrigerErrorDetailValue = 3;

			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if (!lpLeader)
				return	FALSE;

			if	(!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			int	iProcessLevel		=	lpLeader->getQuestProcessLevel(_iQuestType,iQuest);

			if	(iProcessLevel		==	0xffff)
				return	FALSE;
			if	(iProcessLevel		==	iCheckProcessLevel)
				return	TRUE;

			return	FALSE;
		}

		case	3	:
		{
			g_debugSign.m_iTrigerErrorDetailValue = 4;

			cParty	*lpParty	=	_lpAsker->getParty();

			if	(!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					continue;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				if	(lpMember->getQuestProcessLevel(_iQuestType,iQuest)	==	iCheckProcessLevel)
					return	TRUE;
			}

			return	FALSE;
		}
	}

	g_debugSign.m_iTrigerErrorDetail = -1;

	return	FALSE;
}

//
//	진행중인 퀘스트의 수치
BOOL
CKarmaItem::checkQuestQuestValue(cACTOR *_lpAsker,int _iQuestType)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 퀘스트 [quest]0의 수치가 [spin]1[0,20000] [equal]2"										,
//	{"이다","이하이다","이상이다","end"},								//	eKARMA_KW_EQUAL			,		//	이상이다. 이하이다. 이다.
	int	iQuest			=	m_aValue[0];
	int	iCheckValue		=	m_aValue[1];
	int	iMethod			=	m_aValue[2];
	int	iTarget			=	m_aValue[3];
//	{"타겟","모든 파티 멤버","파티 리더","파티원중 아무나 하나","end"},	//	eKARMA_KW_QUEST_TARGET

	switch(iTarget)
	{
		case	0	:
		{
			int	iQuestValue		=	_lpAsker->getQuestValue(_iQuestType,iQuest);
			
			if	(iQuestValue	==	0xffff)
				return	FALSE;

			if	(iMethod	==	0)
				if	(iCheckValue	==	iQuestValue)
					return	TRUE;
			if	(iMethod	==	1)
				if	(iCheckValue	>=	iQuestValue)
					return	TRUE;
			if	(iMethod	==	2)
				if	(iCheckValue	<=	iQuestValue)
					return	TRUE;

			return	FALSE;
		}

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					return	FALSE;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				int	iQuestValue		=	lpMember->getQuestValue(_iQuestType,iQuest);

				if	(iQuestValue	==	0xffff)
					return	FALSE;

				if	(iMethod	==	0)
					if	(iQuestValue	!=	iCheckValue)
						return	FALSE;

				if	(iMethod	==	1)
					if	(iQuestValue	>	iCheckValue)
						return	FALSE;

				if	(iMethod	==	2)	
					if	(iQuestValue	<	iCheckValue)
						return	FALSE;
			}

			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if (!lpLeader)
				return	FALSE;

			if (!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			int	iQuestValue		=	lpLeader->getQuestValue(_iQuestType,iQuest);

			if	(iQuestValue	==	0xffff)
				return	FALSE;

			if (iMethod	==	0)	if (iCheckValue	==	iQuestValue)	return	TRUE;
			if (iMethod	==	1)	if (iCheckValue	>=	iQuestValue)	return	TRUE;
			if (iMethod	==	2)	if (iCheckValue	<=	iQuestValue)	return	TRUE;

			return	FALSE;
		}

		case	3	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if	(!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					return	FALSE;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				int	iQuestValue		=	lpMember->getQuestValue(_iQuestType,iQuest);

				if	(iQuestValue	==	0xffff)
					return	FALSE;

				if	(iMethod	==	0)
					if	(iQuestValue	==	iCheckValue)
						return	TRUE;

				if	(iMethod	==	1)
					if	(iQuestValue	<=	iCheckValue)
						return	TRUE;

				if	(iMethod	==	2)
					if	(iQuestValue	>=	iCheckValue)
						return	TRUE;
			}

			return	TRUE;
		}
	}

	return	FALSE;
}

//
//	소유하고 있는 퀘스트 체크
BOOL
CKarmaItem::checkProcessQuestCount(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" 진행중인 퀘스트의 수가 [spin]0[1,10000] [equal]1"
//	"이다","이하이다","이상이다"
	int	iCount		=	m_aValue[0];
	int	iMethod		=	m_aValue[1];
	int	iTarget		=	m_aValue[2];
	int	iKind		=	m_aValue[3];

//	{"타겟","모든 파티 멤버","파티 리더","파티원중 아무나 하나","end"},	//	eKARMA_KW_QUEST_TARGET

	switch(iTarget)
	{
		case	0	:
		{
			int		iProcessQuestCount	=	_lpAsker->getProcessQuestCount(iKind);

			if	(iMethod	==	0)	
				if	(iProcessQuestCount	==	iCount)
					return	TRUE;

			if	(iMethod	==	1)
				if	(iProcessQuestCount	<=	iCount)
					return	TRUE;

			if	(iMethod	==	2)	
				if	(iProcessQuestCount	>=	iCount)
					return	TRUE;

			return	FALSE;
		}

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					return	FALSE;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				int		iProcessQuestCount	=	lpMember->getProcessQuestCount(iKind);

				if	(iMethod	==	0)	
					if	(iProcessQuestCount	!=	iCount)	
						return	FALSE;
				if	(iMethod	==	1)	
					if	(iProcessQuestCount	>	iCount)	
						return	FALSE;
				if	(iMethod	==	2)	
					if	(iProcessQuestCount	<	iCount)	
						return	FALSE;
			}

			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if	(!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if	(!lpLeader)
				return	FALSE;

			if	(!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			int		iProcessQuestCount	=	lpLeader->getProcessQuestCount(iKind);

			if	(iMethod	==	0)	
				if	(iProcessQuestCount	==	iCount)
					return	TRUE;

			if	(iMethod	==	1)
				if	(iProcessQuestCount	<=	iCount)
					return	TRUE;

			if	(iMethod	==	2)	
				if	(iProcessQuestCount	>=	iCount)
					return	TRUE;

			return	FALSE;
		}

		case	3	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					return	FALSE;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				int		iProcessQuestCount	=	lpMember->getProcessQuestCount(iKind);

				if	(iMethod	==	0)	
					if	(iProcessQuestCount	==	iCount)
						return	TRUE;

				if	(iMethod	==	1)
					if	(iProcessQuestCount	<=	iCount)
						return	TRUE;

				if	(iMethod	==	2)	
					if	(iProcessQuestCount	>=	iCount)
						return	TRUE;
			}

			return	TRUE;
		}
	}

	return	FALSE;
}

//
//	진행중인 퀘스트 빈슬롯?
BOOL
CKarmaItem::checkRemainProcessQuestSlot(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
	int	iMethod		=	m_aValue[0];
//	{"있다.","없다","end"},												//	eKARMA_KW_SPECIFIC_ITEM_OWN_MEMBER_EXIST
	int	iTarget		=	m_aValue[3];
//	{"타겟","모든 파티 멤버","파티 리더","파티원중 아무나 하나","end"},	//	eKARMA_KW_QUEST_TARGET

	switch(iTarget)
	{
		case	0	:
		{
			int		iProcessQuestCount	=	_lpAsker->getProcessQuestCount();

			if	(iMethod	==	0)
				if	(iProcessQuestCount	<	dMAX_PROCESS_QUEST_COUNT)
					return	TRUE;
			if	(iMethod	==	1)
				if	(iProcessQuestCount	>=	dMAX_PROCESS_QUEST_COUNT)
					return	TRUE;

			return	FALSE;
		}

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					return	FALSE;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				int		iProcessQuestCount	=	lpMember->getProcessQuestCount();

				if	(iMethod	==	0)
					if	(iProcessQuestCount	>=	dMAX_PROCESS_QUEST_COUNT)
						return	FALSE;
				if	(iMethod	==	1)	
					if (iProcessQuestCount	<	dMAX_PROCESS_QUEST_COUNT)
						return	FALSE;
			}

			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if (!lpLeader)
				return	FALSE;

			if (!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			int		iProcessQuestCount	=	lpLeader->getProcessQuestCount();

			if	(iMethod	==	0)
				if (iProcessQuestCount	<	dMAX_PROCESS_QUEST_COUNT)
					return	TRUE;
			if	(iMethod	==	1)
				if (iProcessQuestCount	>=	dMAX_PROCESS_QUEST_COUNT)
					return	TRUE;

			return	FALSE;
		}

		case	3	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					return	FALSE;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				int		iProcessQuestCount	=	lpMember->getProcessQuestCount();

				if	(iMethod	==	0)
					if	(iProcessQuestCount	<	dMAX_PROCESS_QUEST_COUNT)
						return	TRUE;
				if	(iMethod	==	1)
					if	(iProcessQuestCount	>=	dMAX_PROCESS_QUEST_COUNT)
						return	TRUE;
			}

			return	TRUE;
		}
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkWordQuizStartTime()
{
	if	(g_eventManager.getWordQuiz()->m_wStatus	==	eWQPS_READY)
		return	TRUE;

	return	FALSE;
}

//
//	OX퀴즈 시작 시간 체크
BOOL
CKarmaItem::checkOXQuizStartTime()
{
	if	(g_eventManager.getOXQuizStatus()	==	eOIPS_READY)
		return	TRUE;

	return	FALSE;
}

//	현재 시간 체크 
BOOL
CKarmaItem::checkCurrentTime(BOOL _isType3)
{
	CTimeInfo	checkBeginTime,checkEndTime;

	memset(&checkBeginTime,0,sizeof(CTimeInfo));

	checkBeginTime.m_wYear	=	m_aValue[0];
	checkBeginTime.m_wMonth	=	m_aValue[1];
	checkBeginTime.m_wDay	=	m_aValue[2];
	checkBeginTime.m_wHour	=	m_aValue[3];

	memcpy(&checkEndTime,&checkBeginTime,sizeof(CTimeInfo));

	checkEndTime.increaseHour(m_aValue[4]);

	DWORD	dwCurrentTimeValue	= 0;
	DWORD	dwBeginTimeValue	= 0;
	DWORD	dwEndTimeValue		= 0;
	if(_isType3 == FALSE)
		dwCurrentTimeValue	=	(g_currentTime.m_wYear-2000)*24*31*12+ g_currentTime.m_wMonth*24*31+ g_currentTime.m_wDay*24+g_currentTime.m_wHour;
	else
		dwCurrentTimeValue	=	g_currentTime.m_wMonth*24*31+ g_currentTime.m_wDay*24+g_currentTime.m_wHour;

	if(_isType3 == FALSE)
		dwBeginTimeValue	=	(checkBeginTime.m_wYear-2000)*24*31*12+ checkBeginTime.m_wMonth*24*31+ checkBeginTime.m_wDay*24+checkBeginTime.m_wHour;
	else
		dwBeginTimeValue	=	checkBeginTime.m_wMonth*24*31+ checkBeginTime.m_wDay*24+checkBeginTime.m_wHour;

	if(_isType3 == FALSE)
		dwEndTimeValue		=	(checkEndTime.m_wYear-2000)*24*31*12+ checkEndTime.m_wMonth*24*31+ checkEndTime.m_wDay*24+checkEndTime.m_wHour;
	else
		dwEndTimeValue		=	checkEndTime.m_wMonth*24*31+ checkEndTime.m_wDay*24+checkEndTime.m_wHour;

	if (dwCurrentTimeValue	>=	dwBeginTimeValue && dwCurrentTimeValue	<=	dwEndTimeValue)
		return	TRUE;

	return	FALSE;
}

BOOL
CKarmaItem::checkCurrentMinute()
{

	int iStartMinute	= m_aValue[0];
	int iEndMinute		= m_aValue[1];

	if(iStartMinute > iEndMinute)
		return FALSE;

	if(iStartMinute <= g_currentTime.m_wMinute && g_currentTime.m_wMinute <= iEndMinute)
		return TRUE;

	return FALSE;
}

BOOL
CKarmaItem::checkPrizePlayer(cACTOR* _lpActor)
{
	//g_eventManager.m_EventList.isPrizePlayer(_lpActor->m_strName , _lpActor->m_strId);
	return TRUE;
}


BOOL
CKarmaItem::checkCurrentWeekDay()
{
	int		iWeekDay	= m_aValue[0];			//	요일	
	int		iIdaAnida	= m_aValue[1];			//	이다 아니다.

	if(iIdaAnida == 0)		// 이다
		if(g_currentTime.m_wWeekDay == iWeekDay)
			return TRUE;

	if(iIdaAnida == 1)		// 아니다
		if(g_currentTime.m_wWeekDay != iWeekDay)		//아니다.
			return TRUE;
	
	return	FALSE;
}


//	현재 시간 체크 
BOOL
CKarmaItem::checkCurrentTime2(BOOL _isType4)
{
	CTimeInfo	checkTime;

	memset(&checkTime,0,sizeof(CTimeInfo));

	checkTime.m_wYear	=	m_aValue[0];
	checkTime.m_wMonth	=	m_aValue[1];
	checkTime.m_wDay	=	m_aValue[2];
	checkTime.m_wHour	=	m_aValue[3];

	DWORD	dwCurrentTimeValue	=	0 ;
	DWORD	dwCheckTimeValue	=	0 ;

	if(_isType4 == FALSE)
		dwCurrentTimeValue = (g_currentTime.m_wYear-2000)*24*31*12+ g_currentTime.m_wMonth*24*31+ g_currentTime.m_wDay*24+g_currentTime.m_wHour;
	else
		dwCurrentTimeValue = g_currentTime.m_wMonth*24*31+ g_currentTime.m_wDay*24+g_currentTime.m_wHour;

	if(_isType4 == FALSE)
		dwCheckTimeValue	=	(checkTime.m_wYear-2000)*24*31*12 + checkTime.m_wMonth*24*31+ checkTime.m_wDay*24+checkTime.m_wHour;
	else
		dwCheckTimeValue	=	checkTime.m_wMonth*24*31+ checkTime.m_wDay*24+checkTime.m_wHour;

	if (m_aValue[4]	==	0)
	{
		if (dwCurrentTimeValue	>=	dwCheckTimeValue)
			return	TRUE;

		return	FALSE;
	}

	if (dwCurrentTimeValue	<	dwCheckTimeValue)
		return	TRUE;

	return	FALSE;
}

BOOL
CKarmaItem::checkIsFullGuild()
{
	int	iAbleDisable	=	m_aValue[0];

	if	(iAbleDisable	==	0)
	{
		if	(g_guildManager.m_iGuildCount	>=	dGUILD_MAX_COUNT)
			return	FALSE;
		
		return	TRUE;
	}

	if	(iAbleDisable	==	1)
	{
		if	(g_guildManager.m_iGuildCount	>=	dGUILD_MAX_COUNT)
			return	TRUE;
		
		return	FALSE;
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkEventTime()
{
	int		iEvent	=	m_aValue[0];
	BOOL	bIsNot	=	m_aValue[1];

	return	bIsNot ^ g_eventTimeManager.isEventTime(iEvent);
}

BOOL
CKarmaItem::checkNotObjectMacro(cACTOR *_lpAsker)
{
	if	(_lpAsker->m_wCurrentClickObject	==	_lpAsker->m_wLastClickObject)
		return	FALSE;

	return	TRUE;
}

BOOL
CKarmaItem::checkIsTestServer()
{
	int	iTestNormal	=	m_aValue[0];

	if	(iTestNormal	==	0)
	{
		if	(g_bIsTestServer)
			return	TRUE;

		return	FALSE;
	}

	if	(iTestNormal	==	1)
	{
		if	(g_bIsTestServer)
			return	FALSE;

		return	TRUE;
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkTodayTime()
{
	int	iBeginHour		=	m_aValue[0];
	int	iBeginMinute	=	m_aValue[1];
	int	iEndHour		=	m_aValue[2];
	int	iEndMinute		=	m_aValue[3];
	int iBeginData		=	iBeginHour*100 + iBeginMinute;
	int iEndData		=	iEndHour*100	+iEndMinute;
	int iCurrentData	=	g_currentTime.m_wHour * 100 + g_currentTime.m_wMinute ;
	DWORD dwBeginTime	=	g_currentTime.m_wMonth * 1000000 + g_currentTime.m_wDay * 10000 + iBeginData;

	DWORD dwEndTime	=	g_currentTime.m_wMonth * 1000000 + g_currentTime.m_wDay * 10000 + iEndData;
	DWORD dwCurrentTime = g_currentTime.m_wMonth * 1000000 + g_currentTime.m_wDay * 10000 + g_currentTime.m_wHour * 100 + g_currentTime.m_wMinute ;

	if(iBeginData > iEndData)		// 시작시간이 종료시간보다 클경우. 종료 날짜를 하루 증가 시킨당. 
	{
		if(iBeginData <  iCurrentData)
			dwEndTime	+=	10000;
		if(iEndData >  iCurrentData)
			dwBeginTime	-=	10000;
	}

	if (dwBeginTime < dwCurrentTime && dwCurrentTime < dwEndTime)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL
CKarmaItem::checkEnemyCountInRange(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;

	int	iRange		=	m_aValue[0];
	int	iCheckCount	=	m_aValue[1];
	int	iCheckMethod=	m_aValue[2];

//	{"이다","이하이다","이상이다","end"},								//	eKARMA_KW_EQUAL			,		//	이상이다. 이하이다. 이다.
//	" 중심에서 [spin]0[1,20000] 픽셸안에 적의 수가 [spin]1[0,1000]명 [equal]2",
	int	iEnemyCount	=	_lpAsker->getInRangeEnemyCount(iRange);

	switch(iCheckMethod)
	{
		case	-1	:
			return	FALSE;

		case	0	:
			if (iEnemyCount	==	iCheckCount)
				return	TRUE;
			return	FALSE;

		case	1	:
			if (iEnemyCount	<=	iCheckCount)
				return	TRUE;
			return	FALSE;

		case	2	:
			if (iEnemyCount	>=	iCheckCount)
				return	TRUE;
			return	FALSE;
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkRemainHP(cACTOR *_lpAsker,cACTOR *_lpOwner)
{
	if	(_lpAsker	==	NULL)
		return	FALSE;

	int	iCheckHP	=	m_aValue[0];
	int	iCheckMethod=	m_aValue[1];
	int	iTarget		=	m_aValue[2];

	cACTOR	*lpTarget;

	if	(iTarget	==	0)
		lpTarget	=	_lpAsker;
	else
		lpTarget	=	_lpOwner;

	int	iRemainHP	=	lpTarget->getRemainHPRate();

//	" ![N]체력 체크 "						,
//	" 체력이 [spin]0[0,100]％ [equal]1",

	switch(iCheckMethod)
	{
		case	0	:
			if (iRemainHP	==	iCheckHP)
				return	TRUE;
			return	FALSE;

		case	1	:
			if (iRemainHP	<=	iCheckHP)
				return	TRUE;
			return	FALSE;

		case	2	:
			if (iRemainHP	>=	iCheckHP)
				return	TRUE;
			return	FALSE;
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkArriveArea(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL || _lpAsker->m_lpField == NULL)
		return FALSE;
//	" [target_player]0이 [area]1 [area_in_out]2",
	int	iTarget		=	m_aValue[0];
	int	iArea		=	m_aValue[1];
	int	iCheckMethod=	m_aValue[2];

	switch(iTarget)
	{
		case	0	:	//	플레이어
		{
			BOOL	bIsComplete	=	_lpAsker->isInArea(iArea);

			if	(iCheckMethod	==	0)
				return	bIsComplete;
			if	(iCheckMethod	==	1)
				return	1-bIsComplete;

			return	TRUE;
		}

		case	1	:	//	파티원 모두
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if	(!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					return	FALSE;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				if	(lpMember->isInArea(iArea) == iCheckMethod)//	만족하지 않는 경우
					return	FALSE;
			}

			return	TRUE;
		}

		case	2	:	//	파티 리더
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if	(!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if	(!lpLeader)
				return	FALSE;

			if	(!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			if	(lpLeader->isInArea(iArea) == iCheckMethod)	//	만족하지 않는 경우
				return	FALSE;

			return	TRUE;
		}

		case	3	:	//	파티원중 하나
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if	(!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					continue;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				if	(lpMember->isInArea(iArea) != iCheckMethod)	//	만족하는 경우
					return	TRUE;
			}

			return	FALSE;
		}
		
		case	4	:	//	모든 몬스터
		{
			cFIELD	*lpField	=	_lpAsker->m_lpField;

			int	iTeam	=	1;

			for (int i=0;i<lpField->m_aiActorCount[1];i++)
			{
				int		iActor		=	lpField->m_awActorList[1][i];
				cACTOR	*lpActor	=	lpField->getActor(iActor);

				if	(!lpActor)
					continue;

				if	(lpActor->isInArea(iArea) == iCheckMethod)	//	만족하는 경우
					return	FALSE;
			}

			return	TRUE;
		}
	}

	if	(iTarget	>=	10000)
	{
		iTarget	-=	10000;

		cFIELD	*lpField	=	_lpAsker->m_lpField;

		int		iCount		=	0;

		for (int i=0;i<lpField->m_aiActorCount[1];i++)
		{
			int		iActor		=	lpField->m_awActorList[1][i];
			cACTOR	*lpActor	=	lpField->getActor(iActor);

			if	(!lpActor)
				continue;

			if	(lpActor->m_wCharacterSerial	!=	iTarget)
				continue;

			if	(lpActor->isInArea(iArea) == iCheckMethod)	//	만족하는 경우
				return	FALSE;

			iCount++;
		}

		if	(iCount)
			return	TRUE;

		return	FALSE;
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkWorldValue()
{
	int	iValueIndex		=	m_aValue[0];
	int	iCheckValue		=	m_aValue[1];
	int	iCheckMethod	=	m_aValue[2];
	
	int	iValue			=	g_aiWorldValue[iValueIndex];
	
	switch(iCheckMethod)
	{
	case	0	:
		if	(iValue	==	iCheckValue)
			return	TRUE;
		return	FALSE;
		
	case	1	:
		if	(iValue	<=	iCheckValue)
			return	TRUE;
		return	FALSE;
		
	case	2	:
		if	(iValue	>=	iCheckValue)
			return	TRUE;
		return	FALSE;
	}
	
	return	FALSE;

}

BOOL
CKarmaItem::checkFieldValue(cFIELD *_lpField)
{
	int	iValueIndex		=	m_aValue[0];
	int	iCheckValue		=	m_aValue[1];
	int	iCheckMethod	=	m_aValue[2];

	int	iValue			=	_lpField->m_aiFieldValue[iValueIndex];

	switch(iCheckMethod)
	{
		case	0	:
			if	(iValue	==	iCheckValue)
				return	TRUE;
			return	FALSE;

		case	1	:
			if	(iValue	<=	iCheckValue)
				return	TRUE;
			return	FALSE;

		case	2	:
			if	(iValue	>=	iCheckValue)
				return	TRUE;
			return	FALSE;
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkDungeonValue(cFIELD *_lpField)
{
	int	iDungeonIndex	=	m_aValue[0];
	int	iDungeonValue	=	m_aValue[1];
	int	iCheckMethod	=	m_aValue[2];

	cInstanceField	*lpIF	=	_lpField->getMasterInstanceField();

	if (!lpIF)
	{
		if ((g_bIsWantPrintTriggerCondition == 1))						
			printf("checkDungeonValue -여긴 비밀 던전이 아니잖아!!\n");

		return	FALSE;
	}

	int	iValue			=	lpIF->getDungeonValue(iDungeonIndex);

//	" [dungeon_value]0이 [spin]1[0,20000] [equal]2",

	if	(iValue	==	-1)
		return	FALSE;

	switch(iCheckMethod)
	{
		case	0	:
			if	(iValue	==	iDungeonValue)
				return	TRUE;
			return	FALSE;

		case	1	:
			if	(iValue	<=	iDungeonValue)
				return	TRUE;
			return	FALSE;

		case	2	:
			if	(iValue	>=	iDungeonValue)
				return	TRUE;
			return	FALSE;
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkTargetFlee(cACTOR *_lpOwner)
{
	if (_lpOwner	==	NULL)
		return	FALSE;
//	" 타겟이 [spin]0[0,20000]초 이상 공격 받지 않고 있다.",
	int	iCheckFleeTime	=	m_aValue[0];

	cACTOR	*lpTarget	=	_lpOwner->m_lpTarget;

	if (!lpTarget)
		return	FALSE;

	if (lpTarget->m_wPeaceTime	>=	iCheckFleeTime)
		return	TRUE;

	return	FALSE;
}

BOOL
CKarmaItem::checkUsedSkill(cACTOR *_lpAsker)
{
	if (_lpAsker	==	NULL)
		return	FALSE;
//	" [quest_target]0가 [area]1 안에서 [skill]2를 사용해다.",
	int	iTarget		=	m_aValue[0];
	int	iArea		=	m_aValue[1];
	int	iSkill		=	m_aValue[2];
	int	iCheckMethod=	0;

	switch(iTarget)
	{
		case	0	:
		{
			BOOL	bIsComplete	=	_lpAsker->isUsedSkill(iArea,iSkill);

			if (iCheckMethod	==	0)	return	bIsComplete;
			if (iCheckMethod	==	1)	return	1-bIsComplete;

			return	TRUE;
		}

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if (!lpMember)
					return	FALSE;

				if (!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				if (lpMember->isUsedSkill(iArea,iSkill) == iCheckMethod)
					return	FALSE;
			}

			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if (!lpLeader)
				return	FALSE;

			if (!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			if (lpLeader->isUsedSkill(iArea,iSkill) == iCheckMethod)
				return	FALSE;

			return	TRUE;
		}

		case	3	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if (!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if (!lpMember)
					continue;

				if (!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				if (lpMember->isUsedSkill(iArea,iSkill) != iCheckMethod)
					return	TRUE;
			}

			return	FALSE;
		}
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkUsedItem(cACTOR *_lpAsker)
{
	if	(_lpAsker	==	NULL)
		return	FALSE;

//	" [quest_target]0이 아이템을 사용했다.",
	int	iTarget		=	m_aValue[0];
	int	iCheckMethod=	0;

	switch(iTarget)
	{
		case	0	:
		{
			BOOL	bIsComplete	=	_lpAsker->isUsedItem();

			if	(iCheckMethod	==	0)
				return	bIsComplete;
			if	(iCheckMethod	==	1)
				return	1-bIsComplete;

			return	TRUE;
		}

		case	1	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if	(!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					return	FALSE;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				if	(lpMember->isUsedItem() == iCheckMethod)
					return	FALSE;
			}

			return	TRUE;
		}

		case	2	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if	(!lpParty)
				return	FALSE;

			cACTOR	*lpLeader	=	lpParty->getActorByMemberIndex(0);

			if	(!lpLeader)
				return	FALSE;

			if	(!lpLeader->isOnSameField(_lpAsker))
				return	FALSE;

			if	(lpLeader->isUsedItem() == iCheckMethod)
				return	FALSE;

			return	TRUE;
		}

		case	3	:
		{
			cParty	*lpParty	=	_lpAsker->getParty();

			if	(!lpParty)
				return	FALSE;

			for (int i=0;i<lpParty->m_wMemberCount;i++)
			{
				cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

				if	(!lpMember)
					continue;

				if	(!lpMember->isOnSameField(_lpAsker))
					return	FALSE;

				if	(lpMember->isUsedItem() != iCheckMethod)
					return	TRUE;
			}

			return	FALSE;
		}
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkPlayerAction(cACTOR *_lpAsker)
{
	if( !_lpAsker )
		return FALSE;

	int		iAction		=	m_aValue[0];
	BOOL	bIsNot		=	m_aValue[1];

	if(_lpAsker->m_iAnm + 1 == iAction)
		return 1 - bIsNot;

	return bIsNot;
}


BOOL
CKarmaItem::checkGuide(cACTOR *_lpOwner)
{
	if (_lpOwner	==	NULL)
		return	FALSE;

	int		iCheck			=	m_aValue[0];
	cACTOR	*lpGuideActor	=	_lpOwner->getGuide();

	if (iCheck	==	0 && lpGuideActor)
		return	TRUE;
	if (iCheck	==	1 && lpGuideActor	==	NULL)
		return	TRUE;

	return	FALSE;
}

BOOL
CKarmaItem::checkSecretDungeonPlayTime(cFIELD *_lpField)
{
//	" 인스턴스 던전 생성된 시간이 [spin]0[0,20000]분 [equal]1",
//	{"이다","이하이다","이상이다","end"},								//	eKARMA_KW_EQUAL			,		//	이상이다. 이하이다. 이다.

	int				iCheckTime	=	m_aValue[0];
	int				iMethod		=	m_aValue[1];
	cInstanceField	*lpIF		=	_lpField->getMasterInstanceField();

	if	(!lpIF)
		return	FALSE;

	if (iMethod	==	0)
		if (lpIF->m_wGenerateMinute		==	iCheckTime)
			return	TRUE;

	if (iMethod	==	1)
		if (lpIF->m_wGenerateMinute		<=	iCheckTime)
			return	TRUE;

	if (iMethod	==	2)
		if (lpIF->m_wGenerateMinute		>=	iCheckTime)
			return	TRUE;

	return	FALSE;
}


BOOL
CKarmaItem::checkSecretDungeonCountdownTimer(cFIELD *_lpField)
{
//	" 카운트 다운 타이머의 남은 시간이 [spin]0[0,60]시간 [spin]1[0,60]분 [spin]2[0,60]초 [countdown_timer_status]3", 
//	{"이다","가 아직 안됐다.","를 지났다.","end"},						//	eKARMA_KW_COUNTDOWN_STATUS				,

	int				iTime		=	m_aValue[0]*60*60+m_aValue[1]*60+m_aValue[2];
	int				iMethod		=	m_aValue[3];
	cInstanceField	*lpIF		=	_lpField->getMasterInstanceField();

	if	(!lpIF)
		return	FALSE;

	if	(iMethod	==	0)
		if	(lpIF->m_wCountdownTimer		==	iTime)
			return	TRUE;

	if	(iMethod	==	1)
		if	(lpIF->m_wCountdownTimer		>=	iTime)
			return	TRUE;

	if	(iMethod	==	2)
		if	(lpIF->m_wCountdownTimer		<=	iTime)
			return	TRUE;

	return	FALSE;
}

BOOL
CKarmaItem::checkMainQuestValue(cACTOR *_lpAsker)
{
//	" 메인 퀘스트 수치 [main_quest_value]0의 값이 [spin]1[0,20000] [equal]2"		,
	int	iValueIndex	=	m_aValue[0];
	int	iValue		=	m_aValue[1];
	int	iEqualLowHigh=	m_aValue[2];

	int	iAskerValue	=	_lpAsker->getMainQuestValue(iValueIndex);

	if	(iEqualLowHigh	==	0)
	{
		if	(iAskerValue	==	iValue)
			return	TRUE;
	}
	else
	if	(iEqualLowHigh	==	1)
	{
		if	(iAskerValue	<=	iValue)
			return	TRUE;
	}
	else
	if	(iEqualLowHigh	==	2)
	{
		if	(iAskerValue	>=	iValue)
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkMainQuestSwitch(cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

//	" 메인 퀘스트 스위치 [main_quest_switch]0가 [check_on_off]1"		,
	int		iSwitchIndex		=	m_aValue[0];
	int		iOnOff				=	m_aValue[1];

	BOOL	bIsAskerSwitchStatus=	_lpAsker->getSwitchStatus(iSwitchIndex);

	if	(iOnOff	!=	bIsAskerSwitchStatus)
		return	TRUE;

	return	FALSE;
}

BOOL
CKarmaItem::checkSex(cACTOR *_lpAsker)
{	
	if	(!_lpAsker)
		return	FALSE;

	if	(_lpAsker->m_wJob	>=	dPLAYER_JOB_COUNT)
		return	FALSE;
	
	int	iManWomen			=	m_aValue[0];

	if	(iManWomen	==	0)
		return	c_aIsMale[_lpAsker->m_wJob];

	return	1-c_aIsMale[_lpAsker->m_wJob];
}

BOOL
CKarmaItem::checkCH5HeavenBonusCount(cACTOR *_lpAsker)
{
	int	iBonusType		=	m_aValue[0];
	int	iCheckCount		=	m_aValue[1];
	int	iEqualLowHigh	=	m_aValue[2];
	int	iBonusPenelty	=	m_aValue[3];

	int	iReceiveCount	=	0;

	switch(	iBonusType)
	{
		case	0	:
			if	(iBonusPenelty	==	1)
			{
				ERRMSG("크아악!!","누가 천상계 스탯 페널티 체크하래!!!");
				return	FALSE;
			}

			iReceiveCount	=	_lpAsker->m_bf4CH5StateBonusCount;
			break;
		case	1	:
			if	(iBonusPenelty	==	1)
			{
				ERRMSG("크아악!!","누가 천상계 스킬 페널티 체크하래!!!");
				return	FALSE;
			}
			iReceiveCount	=	_lpAsker->m_bf4CH5SkillBonusCount;
			break;
		case	2	:
			if	(iBonusPenelty	==	0)
				iReceiveCount	=	_lpAsker->m_bf4CH5HPBonusCount;
			else
				iReceiveCount	=	_lpAsker->m_bf4CH5PeneltyHPBonusCount;
			break;
		case	3	:
			if	(iBonusPenelty	==	0)
				iReceiveCount	=	_lpAsker->m_bf4CH5CPBonusCount;
			else
				iReceiveCount	=	_lpAsker->m_bf4CH5PeneltyCPBonusCount;
			break;
	}

	if	(iEqualLowHigh	==	0)
	{
		if	(iReceiveCount	==	iCheckCount)
			return	TRUE;

		return	FALSE;
	}
	if	(iEqualLowHigh	==	1)
	{
		if	(iReceiveCount	<=	iCheckCount)
			return	TRUE;

		return	FALSE;
	}
	if	(iEqualLowHigh	==	2)
	{
		if	(iReceiveCount	>=	iCheckCount)
			return	TRUE;

		return	FALSE;
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkGuildBonusSkillPointPenelty(cACTOR *_lpAsker)
{
	int		iExistNotExist	=	m_aValue[0];
	CGuild	*lpGuild		=	_lpAsker->getGuild();

	if	(!lpGuild)
		return	FALSE;

	if	(iExistNotExist	==	0)
	{
		if	(lpGuild->m_bf6PeneltyBonusGuildPoint)
			return	TRUE;
		return	FALSE;
	}

	if	(lpGuild->m_bf6PeneltyBonusGuildPoint	==	0)
		return	TRUE;

	return	FALSE;
}

BOOL
CKarmaItem::checkTimeStamp(cACTOR *_lpAsker)
{
	int	iItem		=	m_aValue[0];
	int	iDay		=	m_aValue[1];
	int	iHour		=	m_aValue[2];
	int	iJinaAnjina	=	m_aValue[3];

	cItem	*lpItem	=	_lpAsker->getItemByBasicItem(iItem);

	if	(!lpItem)
		return	FALSE;

	CTimeInfo	itemTime;

	itemTime.m_wYear	=	(short)(lpItem->m_year+2000);
	itemTime.m_wMonth	=	lpItem->m_month;
	itemTime.m_wDay		=	lpItem->m_day;
	itemTime.m_wHour	=	lpItem->m_hour;
	itemTime.m_wMinute	=	lpItem->m_minute;

	int	iCheckMinute		=	iDay*60*24+iHour*60;
	int	iItemMinuteValue	=	itemTime.getMinuteValue(2000);
	int	iCurrentMinuteValue	=	g_currentTime.getMinuteValue(2000);

	if	(iCheckMinute	==	0)
	{
		if	(iItemMinuteValue	==	0)
			return	TRUE;

		return	FALSE;
	}

	if	(iJinaAnjina	==	0)
	{
		if	(iCurrentMinuteValue	>	iItemMinuteValue+iCheckMinute)
			return	TRUE;

		return	FALSE;
	}

	if	(iCurrentMinuteValue	<=	iItemMinuteValue+iCheckMinute)
		return	TRUE;

	return	FALSE;	
}

BOOL
CKarmaItem::checkSameLevelIncreaeHPAmulet(cACTOR *_lpAsker)
{
	int		iOrder	=	1;
	BOOL	bIsExist=	FALSE;
	int		iExistNotExist	=	m_aValue[0];

	int					iTitleLevel	=	0;
	CPlayerTitleInfo	*lpTitle	=	_lpAsker->getTitle(eTN_RED_STONE_HUNTER);

	if	(lpTitle)
		iTitleLevel	=	lpTitle->m_bLevel;

	while(1)
	{
		cItem	*lpItem	=	_lpAsker->getItemByBasicItem(g_im.m_wCH5BonusHPAmulet,iOrder);

		iOrder++;

		if	(!lpItem)
			break;

		if	(lpItem->m_aOption[0]	==	iTitleLevel*5)
		{
			bIsExist	=	TRUE;
			break;
		}
	} 

	if	(iExistNotExist	==	0)
		return	bIsExist;

	return	1-bIsExist;
}

BOOL
CKarmaItem::checkSameLevelIncreaeCPAmulet(cACTOR *_lpAsker)
{
	int		iOrder	=	1;
	BOOL	bIsExist=	FALSE;
	int		iExistNotExist	=	m_aValue[0];

	int					iTitleLevel	=	0;
	CPlayerTitleInfo	*lpTitle	=	_lpAsker->getTitle(eTN_RED_STONE_HUNTER);

	if	(lpTitle)
		iTitleLevel	=	lpTitle->m_bLevel;

	while(1)
	{
		cItem	*lpItem	=	_lpAsker->getItemByBasicItem(g_im.m_wCH5BonusCPAmulet,iOrder);

		iOrder++;

		if	(!lpItem)
			break;

		if	(lpItem->m_aOption[0]	==	iTitleLevel*5)
		{
			bIsExist	=	TRUE;
			break;
		}
	} 

	if	(iExistNotExist	==	0)
		return	bIsExist;

	return	1-bIsExist;
}

//	아이템이 아니라 마지막으로 받은 카운트를 체크한다.
BOOL
CKarmaItem::checkLastLevelWhatReceivedCH5EquipLevelBonus(cACTOR *_lpAsker)
{
	BOOL	bIsExist		=	FALSE;
	int		iExistNotExist	=	m_aValue[0];

	int		iTitleLevel		=	0;
	CPlayerTitleInfo	*lpTitle	=	_lpAsker->getTitle(eTN_RED_STONE_HUNTER);

	if	(lpTitle)
		iTitleLevel	=	lpTitle->m_bLevel;

	if	((int)_lpAsker->m_bf4LevelOfLatestReceivedCH5EquipLevelBonus	>	iTitleLevel)
		_lpAsker->m_bf4LevelOfLatestReceivedCH5EquipLevelBonus	=	0;

	if	(iTitleLevel	<=	(int)_lpAsker->m_bf4LevelOfLatestReceivedCH5EquipLevelBonus)
		bIsExist	=	TRUE;

	if	(iExistNotExist	==	0)
		return	bIsExist;

	return	1-bIsExist;
}

BOOL
CKarmaItem::checkEventTimeBeforeAfter()
{
	int	iEventIndex	=	m_aValue[0];

	if	(iEventIndex	<0	||	iEventIndex	>=	g_eventTimeManager.m_iCount)
		return	FALSE;

	BOOL bIsBefore		=	m_aValue[1];	

	if(bIsBefore)
	{
		if(g_eventTimeManager.m_aEventTimeList[iEventIndex].m_iBeginTimeMinuteValue > g_iCurrentTimeMinuteValue)
			return TRUE;
	}
	else
	{
		if(g_eventTimeManager.m_aEventTimeList[iEventIndex].m_iEndTimeMinuteValue < g_iCurrentTimeMinuteValue)
			return TRUE;
	}

	return FALSE;
}


BOOL
CKarmaItem::checkEventCountTheHours()
{
	int	iEventIndex	=	m_aValue[0];

	if	(iEventIndex	<0	||	iEventIndex	>=	g_eventTimeManager.m_iCount)
		return	FALSE;

	int iTime		=	m_aValue[1] * 60;	
	int	iEqual		=	m_aValue[2];
	int iMinute	=		g_eventTimeManager.m_aEventTimeList[iEventIndex].m_iEndTimeMinuteValue - g_iCurrentTimeMinuteValue;

	if(iMinute< 0)
		return FALSE;

	switch(iEqual)
	{
	case 0:
		{
			if(iTime	==	iMinute)
				return TRUE;
			break;
		}
	case 1:
		{
			if(iTime >= iMinute)
				return TRUE;
			break;
		}
	case 2:
		{
			if(iTime <= iMinute)
				return TRUE;
			break;
		}
	}
	return FALSE;
}

BOOL
CKarmaItem::checkCheckEnableIncreaseGuildStatueLevel(cACTOR *_lpAsker)
{
	int		iExistNotExist	=	m_aValue[0];
	CGuild	*lpGuild		=	_lpAsker->getGuild();

	if	(!lpGuild)
		return	FALSE;

	int					iTitleLevel	=	0;
	CPlayerTitleInfo	*lpTitle	=	_lpAsker->getTitle(eTN_RED_STONE_HUNTER);

	if	(lpTitle)
		iTitleLevel	=	lpTitle->m_bLevel;

	int	iLimitLevel	=	iTitleLevel/2;

	if	(iExistNotExist	==	0)
	{
		if	(lpGuild->checkLevelUpAbleGuildStatue(iLimitLevel))
			return	TRUE;
		return	FALSE;
	}

	if	(lpGuild->checkLevelUpAbleGuildStatue(iLimitLevel)	==	FALSE)
		return	TRUE;

	return	FALSE;
}

BOOL
CKarmaItem::checkEnableIncreaseBonusGuildPoint(cACTOR *_lpAsker)
{
	int		iExistNotExist	=	m_aValue[0];
	CGuild	*lpGuild		=	_lpAsker->getGuild();

	if	(!lpGuild)
		return	FALSE;

	if	(iExistNotExist	==	0)
	{
		if	(lpGuild->m_bf6BonusGuildPoint	<	c_iMaxBonusGuildPoint)
			return	TRUE;

		return	FALSE;
	}

	if	(lpGuild->m_bf6BonusGuildPoint	>=	c_iMaxBonusGuildPoint)
		return	TRUE;

	return	FALSE;
}

BOOL
CKarmaItem::checkCanReceiveCH5RewardTitleBonus(cACTOR *_lpAsker)
{
	int					iTitleLevel	=	0;
	CPlayerTitleInfo	*lpTitle	=	_lpAsker->getTitle(eTN_RED_STONE_HUNTER);

	int	iExistNotExist			=	m_aValue[0];

	if	(lpTitle)
		iTitleLevel	=	lpTitle->m_bLevel;

	int	iCount		=	iTitleLevel*100/3;

	if	(iCount%100)
		iCount		+=	100;

	iCount			/=	100;

	int	iAvailCount	=	0;

	for (int i=0;i<dMAX_PLAYER_TITLE_COUNT;i++)
	{
		CPlayerTitleInfo	*lpTitleInfo	=	&_lpAsker->m_aTitle[i];

		if	(lpTitleInfo->m_bTitle	==	0xff)
			continue;

		if	(lpTitleInfo->m_bTitle	==	eTN_FRONTIER	||	lpTitleInfo->m_bTitle	==	eTN_RED_STONE_HUNTER)
			continue;

		iAvailCount	+=	10-lpTitleInfo->m_bLevel;
	}

	if	(iExistNotExist	==	0)
	{
		if	(iAvailCount	>=	iCount)
			return	TRUE;
		return	FALSE;
	}

	if	(iAvailCount	<	iCount)
		return	TRUE;

	return	FALSE;
}

BOOL
CKarmaItem::checkAttr(cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	int	iPlusMinus	=	m_aValue[0];
	int	iValue		=	m_aValue[1];
	int	iEqualLowHigh=	m_aValue[2];

	if	(iPlusMinus	==	1)
		iValue		*=	-1;

	if	(iEqualLowHigh	==	0)
	{
		if	(_lpAsker->m_sAllignment	==	iValue)
			return	TRUE;

		return	FALSE;
	}
	if	(iEqualLowHigh	==	1)
	{
		if	(_lpAsker->m_sAllignment	<=	iValue)
			return	TRUE;

		return	FALSE;
	}
	if	(iEqualLowHigh	==	2)
	{
		if	(_lpAsker->m_sAllignment	>=	iValue)
			return	TRUE;

		return	FALSE;
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkAttendance(cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	int	iItem		=	m_aValue[0];
	int	iCompleteNot=	m_aValue[1];

	cItem	*lpItem	=	_lpAsker->getItemByBasicItem(iItem);

	if	(!lpItem)
		return	FALSE;

	BOOL		bIsComplete	=	FALSE;
	CTimeInfo	tomorrow;

	memcpy(&tomorrow,&g_currentTime,sizeof(tomorrow));

	tomorrow.increaseDay(1);

	int	iTomorrowCheck	=	(tomorrow.m_wYear-2000)*31*12+tomorrow.m_wMonth*31+tomorrow.m_wDay;
	int	iItemCheck		=	lpItem->m_year*31*12+lpItem->m_month*31+lpItem->m_day;

	if	(iItemCheck	>=	iTomorrowCheck)
		bIsComplete		=	TRUE;

	if	(iCompleteNot	==	0)
		return	bIsComplete;

	return	1-bIsComplete;
}

BOOL
CKarmaItem::checkCheckOwnItemBooleanOption(cACTOR *_lpAsker,int _iCategory,int _iExist_NotExist)
{
	if	(!_lpAsker)
		return	FALSE;

	enum
	{
		eFREE_TOWN_TELEPORT	=	0,
		eCALL_MAGIC_CARPET,
		eCALL_ADD_GAIN_MYSTIC_STONE,
		eCALL_ADD_GAIN_TANTILIS_RELIC,
		eCALL_IGNORE_GUILD_HALL_LEVEL_FOR_ENTRY_TANTILIS_EXILE,
		eINCREASE_INFINITY_SERIOUS_UPGRADE_CHANGE,
		eFREE_PAST_PORTAL,
		eHALF_PAST_PORTAL,
	};

	BOOL	bIsExitItem		=	FALSE;

	switch(_iCategory)
	{
		case	eFREE_PAST_PORTAL	:
			bIsExitItem	= _lpAsker->checkFreePastPortalOption();
			break;
		case	eHALF_PAST_PORTAL	:
			bIsExitItem	= _lpAsker->checkHalfPastPortalOption();
			break;
		case	eFREE_TOWN_TELEPORT	:
			bIsExitItem	= _lpAsker->checkFreeTeleportOption();
			break;
		case	eCALL_MAGIC_CARPET	:
			bIsExitItem	= _lpAsker->checkRequestSummonCarpetOption();
			break;

		case	eCALL_ADD_GAIN_MYSTIC_STONE	:
			bIsExitItem	= _lpAsker->m_bf1IsAddGetMysticStone;
			break;

		case	eCALL_ADD_GAIN_TANTILIS_RELIC	:
			bIsExitItem	= _lpAsker->m_bf1IsAddGetTantalissRelic;	//	
			break;

		case	eCALL_IGNORE_GUILD_HALL_LEVEL_FOR_ENTRY_TANTILIS_EXILE	:
			bIsExitItem	= _lpAsker->m_bf1IsIgnoreGuildHallLevelForEntryTantalisExile;	//	
			break;

		case	eINCREASE_INFINITY_SERIOUS_UPGRADE_CHANGE	:
			bIsExitItem	= _lpAsker->m_bf1IsInfinitySeriousUpgradeChanceUp;	//	
			break;
	}

	if	(bIsExitItem)
		return	1-_iExist_NotExist;

	return	_iExist_NotExist;
}

BOOL
CKarmaItem::checkSecretDungeonPlayCount(cACTOR *_lpAsker)//	비밀던전 횟수 체크.
{
	if	(_lpAsker	==	NULL)
		return	FALSE;
	
//	" 비밀던전 횟수가 [spin]0[0,30]회 [equal]1"							,
	int		iCheckCount		=	m_aValue[0];
	int		iDa_Iha_Isang	=	m_aValue[1];
	int		iSecretDungeonPlayCount	=	_lpAsker->m_bf4SecretDungeonPlayCount;
	
	if	(iDa_Iha_Isang	==	0)
	{
		if	(iSecretDungeonPlayCount	==	iCheckCount)
			return	TRUE;
		
		return	FALSE;
	}
	
	if	(iDa_Iha_Isang	==	1)
	{
		if	(iSecretDungeonPlayCount	<=	iCheckCount)
			return	TRUE;
		
		return	FALSE;
	}
	
	if	(iDa_Iha_Isang	==	2)
	{
		if	(iSecretDungeonPlayCount	>=	iCheckCount)
			return	TRUE;
		
		return	FALSE;
	}
	
	return	FALSE;
}

BOOL
CKarmaItem::checkCheckOwnItemBooleanOption(cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;


	int	iCategory		=	m_aValue[0];			//타운 텔레포트 무료 옵션아이템인지. 양탄자 소환 의뢰 옵션인지.
	int iExist_NotExist	=	m_aValue[1];			//있다 없다.
	int iTarget			=	m_aValue[2];			//있다 없다.

//	{"타겟","모든 파티원","파티 리더","파티원중 아무나 한명","end"},	//	eKARMA_KW_TARGET_PLAYER

	if	(iTarget	==	0)
	{
		return	checkCheckOwnItemBooleanOption(_lpAsker,iCategory,iExist_NotExist);
	}
	else
	if	(_lpAsker->getPartySerial()	==	0xffff)
		return	FALSE;

	cParty	party;

	g_pPartyManager->getParty(&party,_lpAsker->getPartySerial());

	int	iCompleteCount	=	0;

	if	(iTarget	==	1)
	{
		for (int i=0;i<(int)party.getMemberCount();i++)
		{
			cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

			if	(!lpMember)
				continue;

			iCompleteCount	+=	checkCheckOwnItemBooleanOption(lpMember,iCategory,iExist_NotExist);
		}

		if	(iCompleteCount	==	party.getMemberCount())
			return	TRUE;

		return	FALSE;
	}
	else
	if	(iTarget	==	2)
	{
		cACTOR	*lpLeader	=	party.getActorByMemberIndex(0);

		return	checkCheckOwnItemBooleanOption(lpLeader,iCategory,iExist_NotExist);
	}
	else
	if	(iTarget	==	3)
	{
		for (int i=0;i<(int)party.getMemberCount();i++)
		{
			cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

			if	(!lpMember)
				continue;

			if	(checkCheckOwnItemBooleanOption(lpMember,iCategory,iExist_NotExist))
				return	TRUE;
		}

		return	FALSE;
	}

	return	FALSE;
}


BOOL
CKarmaItem::checkMemberSex(cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	cParty	party;

	if	(!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))
		return	FALSE;

	int	iManWomen	=	m_aValue[0];
	int	iCheckCount	=	m_aValue[1];
	int	iEqualLowHigh=	m_aValue[2];

	int	iManCount	=	0;
	int	iWomanCount	=	0;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		if	(party.m_aMember[i].m_wFieldSerial	!=	_lpAsker->m_wCurrentField)
			continue;
		if	(party.m_aMember[i].m_bf1IsDisconnected)
			continue;

		cACTOR	*lpActor	=	party.getActorByMemberIndex(i);

		if	(!lpActor	||	lpActor->isDeath()	||	lpActor->isIdlePlayer())
			continue;

		if	(c_aIsMale[party.m_aMember[i].m_bf8Job])
			iManCount++;
		else
			iWomanCount++;
	}

	if	(iWomanCount	>=	2)
		iWomanCount		=	1;
	if	(iManCount		>=	2)
		iManCount		=	1;

	if	(iEqualLowHigh	==	0)
	{
		if	(iManWomen	==	0)
		{
			if	(iManCount	==	iCheckCount)
				return	TRUE;
		}
		else
		{
			if	(iWomanCount	==	iCheckCount)
				return	TRUE;
		}
	}
	else
	if	(iEqualLowHigh	==	1)
	{
		if	(iManWomen	==	0)
		{
			if	(iManCount	<=	iCheckCount)
				return	TRUE;
		}
		else
		{
			if	(iWomanCount<=	iCheckCount)
				return	TRUE;
		}
	}
	else
	if	(iEqualLowHigh	==	2)
	{
		if	(iManWomen	==	0)
		{
			if	(iManCount	>=	iCheckCount)
				return	TRUE;
		}
		else
		{
			if	(iWomanCount>=	iCheckCount)
				return	TRUE;
		}
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkPartyLevelRange(cACTOR *_lpAsker)
{
	if	(!_lpAsker)
		return	FALSE;

	cParty	party;

	if	(!g_pPartyManager->getParty(&party,_lpAsker->getPartySerial()))
		return	FALSE;

	int	iValue		=	m_aValue[0];
	int	iEqualLowHigh=	m_aValue[1];

	int	iHighLevel	=	0;
	int	iLowLevel	=	1000;

	for (int i=0;i<party.m_wMemberCount;i++)
	{
		if	(party.m_aMember[i].m_bf10Level	>	(DWORD)iHighLevel)
			iHighLevel	=	party.m_aMember[i].m_bf10Level;
		if	(party.m_aMember[i].m_bf10Level	<	(DWORD)iLowLevel)
			iLowLevel	=	party.m_aMember[i].m_bf10Level;
	}

	int	iLevelRange		=	iHighLevel-iLowLevel;

	if	(iEqualLowHigh	==	0)
	{
		if	(iLevelRange	==	iValue)
			return	TRUE;
	}
	else
	if	(iEqualLowHigh	==	1)
	{
		if	(iLevelRange	<=	iValue)
			return	TRUE;
	}
	else
	if	(iEqualLowHigh	==	2)
	{
		if	(iLevelRange	>=	iValue)
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkCurrentPetCount(cACTOR *_lpAsker)	//	현재 펫의 수 체크
{
//		" 현재 소환된 펫의 수가 [spin]0[0,2] [equal]1"	,
	int	iCheckCount		=	m_aValue[0];
	int	iEqualLowHigh	=	m_aValue[1];
	int	iPetCount		=	_lpAsker->getPetCount();

	if	(iEqualLowHigh	==	0)
	{
		if	(iPetCount	==	iCheckCount)
			return	TRUE;
	}
	else
	if	(iEqualLowHigh	==	1)
	{
		if	(iPetCount	<=	iCheckCount)
			return	TRUE;
	}
	else
	if	(iEqualLowHigh	==	2)
	{
		if	(iPetCount	>=	iCheckCount)
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkAvailAddPet(cACTOR *_lpAsker)	//펫 추가 가능한지 체크
{
	int		iExistNotExist	=	m_aValue[0];
	BOOL	bIsFull			=	FALSE;
	
	if	(_lpAsker->getPetCount() >= dOWN_PET_COUNT)
		bIsFull	=	TRUE;

	if	(iExistNotExist	==	0)
		return	1-bIsFull;

	return	bIsFull;
}

BOOL
CKarmaItem::checkProbability()
{
	int	iChance	=	m_aValue[0];

	if	(random(100)	<	iChance)
		return	TRUE;

	return	FALSE;
}

BOOL
CKarmaItem::checkProbability2()
{
	int	iChance	=	m_aValue[0];

	if	(largeRandom(50000)	<	iChance)
		return	TRUE;

	return	FALSE;
}


BOOL
CKarmaItem::checkTargetMonsterLevel(cACTOR *_lpAsker,cACTOR *_lpOwner)
{
	if	(!_lpAsker	||	!_lpOwner)
		return	FALSE;

	int	iValue		=	_lpAsker->m_iLevel-m_aValue[0];
	int	iEqualLowHigh=	m_aValue[1];

	if	(iEqualLowHigh	==	0)
	{
		if	(_lpOwner->m_iLevel	==	iValue)
			return	TRUE;
	}
	else
	if	(iEqualLowHigh	==	1)
	{
		if	(_lpOwner->m_iLevel	<=	iValue)
			return	TRUE;
	}
	else
	if	(iEqualLowHigh	==	2)
	{
		if	(_lpOwner->m_iLevel	>=	iValue)
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CKarmaItem::checkEventValue()
{
	int	iEvent			=	m_aValue[0];
	int	iValue			=	m_aValue[1];
	int	iEqualLowHigh	=	m_aValue[2];

	if	(iEqualLowHigh	==	0)
	{
		if	(g_eventManager.getDBControlledEventValue(iEvent)	==	iValue)
			return	TRUE;

		return	FALSE;
	}

	if	(iEqualLowHigh	==	1)
	{
		if	(g_eventManager.getDBControlledEventValue(iEvent)	<=	iValue)
			return	TRUE;

		return	FALSE;
	}

	if	(iEqualLowHigh	==	2)
	{
		if	(g_eventManager.getDBControlledEventValue(iEvent)	>=	iValue)
			return	TRUE;

		return	FALSE;
	}

	return	FALSE;
}

//	지역 체크
BOOL
CKarmaItem::checkLocal()
{
	int	iLocal		=	m_aValue[0];
	int	iCurLocal	=	-1;
	int	iIdaAnida	=	m_aValue[1];

#ifdef	_FOR_KOREA
	iCurLocal	=	0;
#elif	_FOR_JAPAN
	iCurLocal	=	1;
#elif	_FOR_CHINA
	iCurLocal	=	2;
#elif	_FOR_INTER
	iCurLocal	=	3;
#elif	_FOR_USA
	iCurLocal	=	4;
#elif	_FOR_THAI
	iCurLocal	=	5;
#endif

	if	(iCurLocal	==	iLocal)
		return	1-iIdaAnida;

	return	iIdaAnida;
}

BOOL
CKarmaItem::checkIshaveBlindEffect(cACTOR *_lpAsker)
{
	int	iIsHaveBlindEffect		=	m_aValue[0];			//상태 이다 아니다.
	
	if(_lpAsker->m_wBlindByEvent &	dAMEM_NEW_BLIND_VERY_SMALL || _lpAsker->m_wBlindByEvent &	dAMEM_NEW_BLIND_SMALL || _lpAsker->m_wBlindByEvent & dAMEM_NEW_BLIND_VERY_SMALL|| _lpAsker->m_wBlindByEvent &	dAMEM_NEW_BLIND_MIDDLE
		|| _lpAsker->m_wBlindByEvent &	dAMEM_NEW_BLIND_LARGE || _lpAsker->m_wBlindByEvent & dAMEM_NEW_BLIND_VERY_LARGE)
		return iIsHaveBlindEffect;

	return FALSE;
}


//
BOOL
CKarmaItem::checkIshaveGrace(cACTOR *_lpAsker)
{
	int	iIsHaveGrace		=	m_aValue[0];			//보유하고 있다 아니다.

	if(iIsHaveGrace)			//보유하지 않았다.
	{
		if(_lpAsker->m_bf3GreateGodGrace == 0)
			return TRUE;
	}
	else
	{
		if(_lpAsker->m_bf3GreateGodGrace != 0)
			return TRUE;
	}

	return FALSE;
}

//	시즌 변수 체크
BOOL
CKarmaItem::checkSeasonVariable()
{
	int	iRank1		=	m_aValue[0];			//시즌 변수.
	int	iIdaAnida	=	m_aValue[1];			//이다 이상이다 이하이다.

	BOOL Result = FALSE;

	switch(m_aValue[1])
	{
		case 0:				//이다.
			if(m_aValue[0] == g_iSeasonVariable)
				Result = TRUE;
			break;
		case 1:				//이상이다.
			if(m_aValue[0] >= g_iSeasonVariable)
				Result = TRUE;
			break;
		case 2:				//이하이다.
			if(m_aValue[0] <= g_iSeasonVariable)
				Result = TRUE;
			break;
	}
	
	return	Result;
}

//	결투 랭킹 체크
BOOL
CKarmaItem::checkDuelRanking(cACTOR *_lpOwner)
{
	if	(!_lpOwner)
		return	FALSE;

	int	iRank1		=	m_aValue[0];
	int	iRank2		=	m_aValue[1];
	int	iIdaAnida	=	m_aValue[2];

	if	(_lpOwner->m_wWeeklyDuelRank	>=	iRank1	&&	_lpOwner->m_wWeeklyDuelRank	<=	iRank2	)
		return	1-iIdaAnida;

	return	iIdaAnida;
}

//	몬스터 이벤트 체크
BOOL
CKarmaItem::checkMonsterEvent(cACTOR *_lpOwner)
{
	if	(!_lpOwner)
		return	FALSE;

	int	iEvent		=	m_aValue[0];
	int	iIdaAnida	=	m_aValue[1];

	if	(_lpOwner->m_wEventValue	==	iEvent)
		return	1-iIdaAnida;

	return	iIdaAnida;
}

BOOL
CKarmaItem::checkTileSet(cFIELD *_lpField)
{
	if	(_lpField	==	NULL)
		return	FALSE;

	int		iTileSet		=	m_aValue[0];
	BOOL	bIsNot				=	m_aValue[1];

	BOOL	bIsOK		=	FALSE;
	
	if( _lpField->m_wTileSet == iTileSet)
		bIsOK = TRUE;
	
	if(bIsNot)
	{
		bIsOK = 1-bIsOK;
	}

	return	bIsOK;
}

BOOL
CKarmaItem::checkTransformationMonster(cACTOR *_lpAsker)
{
	if	(_lpAsker	==	NULL)
		return	FALSE;

	BOOL	bIsNot				=	m_aValue[0];

	BOOL	bIsOK		=	FALSE;
	
	if( _lpAsker->m_wTransformationTime)
		bIsOK = TRUE;
	
	if(bIsNot)
	{
		bIsOK = 1-bIsOK;
	}

	return	bIsOK;
}

//													
//	만족하냐?												
BOOL
CKarmaItem::isCompleteTrigger(cACTOR *_lpAsker,cACTOR *_lpOwner,cFIELD *_lpField)
{
	BOOL	bIsOk	=	FALSE;
	char	*strOk[]=	{"실패","성공"};

	g_debugSign.m_iTempStep	=	0x8000+m_wItem;
	g_debugSign.m_iTrigerStep	=	0;
	
	if	(_lpOwner)
		_lpOwner	=	_lpOwner;

	g_debugSign.m_iTrigerStep	=	1;
	g_debugSign.m_iTrigerStepError	= m_wItem;

	switch(m_wItem)												
	{												
//	플레이어 자신과 관련된 사항												
		case	eTRIGGER_LEVEL_CHECK									:	// 레벨이 [spin]0[1,1000]~[spin]1[1,1000]이다.
			bIsOk	=	checkLevel(_lpAsker);								
			if ((g_bIsWantPrintTriggerCondition == 1))										
				printf("          check레벨체크 - %s\n",strOk[bIsOk]);									
			return	bIsOk;									

		case	eTRIGGER_IS_GUILD_MEMBER								:	// 길드에 [guild_member]0 		
			bIsOk	=	checkIsAssignedAnyGuild(_lpAsker);									
			if ((g_bIsWantPrintTriggerCondition == 1))											
				printf("          check길드 맴버 체크 - %s\n",strOk[bIsOk]);										
			return	bIsOk;										

		case	eTRIGGER_IS_RANK_IN_GUILD								:	// 길드에 [guild_member]0 		
			bIsOk	=	checkRankInGuild(_lpAsker);									
			if ((g_bIsWantPrintTriggerCondition == 1))											
				printf("          check길드내 직위 체크 - %s\n",strOk[bIsOk]);										
			return	bIsOk;										

		case	eTRIGGER_OWN_ITEM										:	// [item]0의 보유 갯수가 [spin]1[0,255]개 [equal]2 
			bIsOk	=	checkItem(_lpAsker);									
			if ((g_bIsWantPrintTriggerCondition == 1))											
				printf("          check일반아이템 소유 - %s\n",strOk[bIsOk]);										
			return	bIsOk;										

		case	eTRIGGER_OWN_SPECIAL_ITEM								:	// [special_item]0의 보유 갯수가 [spin]1[0,255]개 [equal]2		
			bIsOk	=	checkItem(_lpAsker);							
			if ((g_bIsWantPrintTriggerCondition == 1))									
				printf("          check특정 아이템 소유 - %s\n",strOk[bIsOk]);								
			return	bIsOk;								

		case	eTRIGGER_OWN_QUEST_ITEM									:
			bIsOk	=	checkItem(_lpAsker);							
			if ((g_bIsWantPrintTriggerCondition == 1))									
				printf("          check퀘스트 아이템 소유 - %s\n",strOk[bIsOk]);								
			return	bIsOk;								

		case	eTRIGGER_REMAIN_ITEM_SLOT_COUNT							:		
			bIsOk	=	checkRemainItemSlotCount(_lpAsker);							
			if ((g_bIsWantPrintTriggerCondition == 1))											
				printf("          check아이템 슬롯 체크 - %s\n",strOk[bIsOk]);										
			return	bIsOk;										

		case	eTRIGGER_MASTER_SKILL_COUNT								:			
			bIsOk	=	checkMasterSkillCount(_lpAsker);									
			if ((g_bIsWantPrintTriggerCondition == 1))											
				printf("          check마스터 스킬 체크 - %s\n",strOk[bIsOk]);										
			return	bIsOk;										

		case	eTRIGGER_EQUIP_ITEM										:	// [item]0을 [item_equip]1 
			bIsOk	=	checkEquippedItem(_lpAsker);									
			if ((g_bIsWantPrintTriggerCondition == 1))											
				printf("          check장비 아이템 체크 - %s\n",strOk[bIsOk]);										
			return	bIsOk;										

		case	eTRIGGER_OWN_GOLD										:	// 보유한 금액이 [number]0[0,2147483647]골드 [equal]1 
			bIsOk	=	checkGold(_lpAsker);									
			if ((g_bIsWantPrintTriggerCondition == 1))											
				printf("          check소유 골드 체크 - %s\n",strOk[bIsOk]);										
			return	bIsOk;										

		case	eTRIGGER_SKILL_LEVEL									:	// 스킬 [skill]0의 레벨이 [spin]1[0,50] [equal]2 	
			bIsOk	=	checkSkill(_lpAsker);									
			if	((g_bIsWantPrintTriggerCondition == 1))											
				printf("          check스킬 레벨 - %s\n",strOk[bIsOk]);										
			return	bIsOk;										

		case	eTRIGGER_TITLE_LEVEL									:	// 칭호 [title]0의 레벨이 [spin]1[0,10] [equal]2 	
			bIsOk	=	checkTitle(_lpAsker);									
			if ((g_bIsWantPrintTriggerCondition == 1))											
				printf("          check칭호 레벨 - %s\n",strOk[bIsOk]);										
			return	bIsOk;										

		case	eTRIGGER_CHECK_JOB										:	// 직업이 [player_job]0인 파티원이 [exist]1
		case	eTRIGGER_CHECK_PLAYER_JOB_BY_QUEST						:
		case	eTRIGGER_CHECK_PLAYER_JOB_BY_ARBEIT						:
			
			bIsOk	=	checkJob(_lpAsker);									
			if ((g_bIsWantPrintTriggerCondition == 1))											
				printf("          check직업 체크 - %s\n",strOk[bIsOk]);										
			return	bIsOk;										

		case	eTRIGGER_CHECK_JOB_LEVEL								:	// 직업레벨이 [spin]0[1,3] [equal]1 		
			if ((g_bIsWantPrintTriggerCondition == 1))									
				printf("          check직업 레벨 체크 - %s\n",strOk[bIsOk]);								
			return	bIsOk;								
			break;									

		case	eTRIGGER_CHECK_ALLIGNMENT								:	// 성향이 [allignment]0이다. 
			bIsOk	=	checkAllignment(_lpAsker);							
			if ((g_bIsWantPrintTriggerCondition == 1))									
				printf("          check성향 체크 - %s\n",strOk[bIsOk]);								
			return	bIsOk;								

		case	eTRIGGER_CHECK_PLAY_TIME								:	// 플레이 시간이 [number]0[0,2147483647]시간 [equal]1 
			bIsOk	=	checkPlayTime(_lpAsker);							
			if ((g_bIsWantPrintTriggerCondition == 1))									
				printf("          check플레이 타임 체크 - %s\n",strOk[bIsOk]);								
			return	bIsOk;								

		case	eTRIGGER_CHECK_BASIC_POWER								:	// [power]0가 [spin]1[1,10000] [equal]2 
			bIsOk	=	checkBasicPower(_lpAsker);							
			if ((g_bIsWantPrintTriggerCondition == 1))									
				printf("          check능력치 체크 - %s\n",strOk[bIsOk]);								
			return	bIsOk;								

		case	eTRIGGER_CHECK_BOOK_COMPLETE_PERCENTAGE					:	// [power]0가 [spin]1[1,10000] [equal]2 
			bIsOk	=	checkBookCompletePercentage(_lpAsker);							
			if	((g_bIsWantPrintTriggerCondition == 1))									
				printf("          check책 완성도 체크 - %s\n",strOk[bIsOk]);								
			return	bIsOk;								

		case	eTRIGGER_CHECK_EXIST_BOOK								:	// [power]0가 [spin]1[1,10000] [equal]2 
			bIsOk	=	checkExistBook(_lpAsker);							
			if	((g_bIsWantPrintTriggerCondition == 1))									
				printf("          check책 보유 여부 체크 - %s\n",strOk[bIsOk]);								
			return	bIsOk;

		case	eTRIGGER_CHECK_BOOK_PAGE								:	// [power]0가 [spin]1[1,10000] [equal]2 
			bIsOk	=	checkExistPage(_lpAsker);							
			if	((g_bIsWantPrintTriggerCondition == 1))									
				printf("          check페이지 체크 - %s\n",strOk[bIsOk]);								
			return	bIsOk;								
		case	eTRIGGER_CHECK_OWN_ADD_BOOK_PAGE_ITEM					:	// [power]0가 [spin]1[1,10000] [equal]2 
			bIsOk	=	checkExistAddBookPageItem(_lpAsker);							
			if	((g_bIsWantPrintTriggerCondition == 1))									
				printf("          check페이지 체크 - %s\n",strOk[bIsOk]);								
			return	bIsOk;								
		case	eTRIGGER_CHECK_CURRENT_STATUS							:	// [power]0가 [spin]1[1,10000] [equal]2 
			bIsOk	=	checkCurrentStatus(_lpAsker);							
			if	((g_bIsWantPrintTriggerCondition == 1))									
				printf("          check현재 상태 체크 - %s\n",strOk[bIsOk]);								
			return	bIsOk;								
		case	eTRIGGER_CHECK_GUILD_TITLE								:	// 길드안에서의 직위가 [guild_title]0 이다. 
			if	((g_bIsWantPrintTriggerCondition == 1))									
				printf("          check길드 랭킹 체크 - %s\n",strOk[bIsOk]);								
			return	bIsOk;													
			break;														

		case	eTRIGGER_CHECK_OPERATOR									:	
		{
			if (_lpAsker && _lpAsker->m_wOperatorLevel	>=	2)												
				bIsOk	=	1-m_aValue[0];
			else														
				bIsOk	=	m_aValue[0];

			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check운영자 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
		}

//	소속된 파티와 관련된 사항																

		case	eTRIGGER_IS_PARTY_LEADER								:						
			bIsOk	=	checkIsPartyLeader(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check파티 리더 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_PARTY_MEMBER_COUNT								:	// 파티원이 [spin]0[1,8]명 [equal]1 					
			bIsOk	=	checkPartyMemberCount(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check파티원 수 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
		case	eTRIGGER_PARTY_MEMBER_COUNT_IN_CURRENT_FIELD								:	// " 현재 필드에 있는 파티원이 [spin]0[0,8]명 [equal]1"	,				
			bIsOk	=	checkPartyMemberCountInCurrentField(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check현재 필드에 있는 파티원 수 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_CHECK_PARTY_MEMBER_LEVEL						:	// 파티원의 레벨이 [spin]0[1,1000]~[spin]1[1,1000]이다. 							
			bIsOk	=	checkPartyMemberLevel(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check파티원 레벨 범위 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_ALL_PARTY_MEMBER_IN_GUILD						:	// 모든 파티원이 길드에 [guild_member]0 							
			bIsOk	=	checkPartyMembersAssignedGuild(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check파티 모두 길드원 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_IN_GUILD_MEMBER_EXIST_IN_PARTY					:	// 길드에 소속해 있는 파티원이 [party_guild_member_exist]0 								
			bIsOk	=	checkAssignedGuildPartyMember(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check파티 중 길드원 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_ALL_PARTY_MEMBER_IN_SAME_GUILD					:	// 모든 파티원이 [all_party_member_in_same_guild]0 								
			bIsOk	=	checkAllPartyMembersAssignedSameGuild(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check모두 같은 길드원 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_ALL_PARTY_MEMBER_OWN_SPECIFIC_ITEM				:	// 모든 파티원이 [item]0을 [spin]1[0,255]개 이상 보유하고 있다. 									
			bIsOk	=	checkAllPartyMembersOwnItem(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check모든 파티원의 일반 아이템 보유 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_ALL_PARTY_MEMBER_NOT_OWN_SPECIFIC_ITEM			:	// 모든 파티원이 [item]0을 보유하고 있지 않다. 										
			bIsOk	=	checkAllPartyMembersNotOwnItem(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check모든 파티원이 아이템을 가지고 있지 않음 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_OWN_SPECIFIC_ITEM_MEMBER_EXIST_IN_PARTY		:	// [item]0을 [spin]1[1,255]개 이상 보유한 파티원이 [specific_item_own_member_exist]2 											
			bIsOk	=	checkPartyOwnItem(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check파티원 중 누군가가 보유한 일반 아이템 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_GOLD_SUM_OF_PARTY								:	// 파티원의 소지 금액 합이 [number]0[1,2147483647]골드 [equal]1 					
			bIsOk	=	checkGoldSumOfPartyMembers(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check파티원의 소지 금액의 합 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_CHECK_GOLD_IN_ALL_PARTY_MEMBER					:									
			bIsOk	=	checkGoldOfAllPartyMembers(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check파티원의 소지금액 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_CHECK_GOLD_IN_PARTY_MEMBER						:	// [number]0[1,2147483647]골드 이상 소지하고 있는 파티원이 [exist]1 							
			bIsOk	=	checkOwnGoldOfPartyMember(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check모든 파티원의 소지금액 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_CHECK_SKILL_IN_PARTY_MEMBER					:	// 레벨 [spin]0[0,50]이상의 [skill]1이 있는 파티원이 [exist]2 								
			bIsOk	=	checkPartyMembersSkillLevel(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check파티원 중 아무나의 스킬 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_ALL_PARTY_MEMBER_LEARNED_SSPECIFIC_SKILL		:	// 모든 파티원이 레벨 [spin]0[0,50]이상의 [skill]1을 익히고 있다. 											
			bIsOk	=	checkAllPartyMembersSkill(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check모든 파티원의 스킬 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
																	
		case	eTRIGGER_CHECK_TITLE_IN_PARTY_MEMBER					:	// 칭호 [title]0을 보유한 파티원이 [exist]1 								
			bIsOk	=	checkTitleInPartyMember(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check칭호를 가진 파티원의 유무 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_OWNED_TITLE_BY_ALL_PATY_MEMBER					:	// 모든 파티원이 칭호 [title]0을 [own]1 								
			bIsOk	=	checkAllPartyMembersTitle(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check칭호를 모두 가지고 있는지의 유무 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_CHECK_JOB_IN_PARTY								:	// 직업이 [hero_job]0인 파티원이 [exist]1 					
			bIsOk	=	checkPartyMemberJob(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check파티원 중 하나의 직업 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_CHECK_ALL_PARTY_MEMBERS_LIFE					:
			bIsOk	=	checkDeahtAllPartyMember(_lpField);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check모든 파티원이 살아 있는지 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;

		case	eTRIGGER_CHECK_ALL_PARTY_MEMBER_JOB						:	//	모든 파티원의 직업이 [player_job]이다.						
			bIsOk	=	checkAllPartyMembersJob(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check모두 특정 직업인지 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_CHECK_JOB_LEVEL_IN_ALL_PARTY_MEMBER			:	// 모든 파티원의 직업레벨이 [spin]0[1,3]이다. 										
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check모든 파티원의 특정 레벨 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
			break;														
																	
		case	eTRIGGER_CHECK_ALLIGNMENT_IN_PARTY_MEMBER				:	// 성향이 [allignment]0인 파티원이 [exist]1 									
			bIsOk	=	checkPartyMembersAllignment(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check파티원 내 성향 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_CHECK_ALLIGNMENT_IN_ALL_PARTY_MEMBER			:	// 모든 파티원의 성향이 [allignment]0이다. 										
			bIsOk	=	checkAllPartyMembersAllignment(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check모두 같은 성향인지 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_CHECK_BASKC_POWER_IN_PARTY_MEMBER				:	// [power]0가 [spin]1[1,10000]이상인 파티원이 [exist]2 									
			bIsOk	=	checkPartyMembersBasicPower(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check파티원 중 하나의 능력치 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_CHECK_BASKC_POWER_IN_ALL_PARTY_MEMBER			:	// 모든 파티원의 [power]0가 [spin]1[1,10000] [equal]2 										
			bIsOk	=	checkAllPartyMembersBasicPower(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check모든 파티원의 능력치 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
																	
		case	eTRIGGER_CHECK_PARTY_MEMBER_LOCATION					:	//	모든 파티원이 [guild_member_location]							
			bIsOk	=	checkPartyMembersLocation(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check파티원의 모임 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_HAVE_MASTER_SKILL_PARTY_MEMBER					:	//	스킬을 마스터한 수가 [spin]0[0,60]개 이상인 파티 멤버의 수가 [spin]1[0,8]명 [equal]2							
			bIsOk	=	checkHaveMasterSkillPartyMemberCount(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check스킬을 마스터한 파티원 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGERCHECK_ALL_PARTY_MEMBERS_MASTER_SKILL_COUNT		:	//	모든 파티원이 [spin]0[0,60]개 [only_equal]1의 스킬을 마스터 했다.										
			bIsOk	=	checkAllPartyMembersMasterSkillCount(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check일부 맴버의 마스터 스킬 수와 그 맴버 수 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

//	소속 길드와 관련된 사항																

		case	eTRIGGER_CHECK_ATTACED_GUILD_LEVEL						:	// 소속 길드의 레벨이 [spin]0[1,10000] [equal]1 							
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check소속 길드 레벨 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
			break;														

		case	eTRIGGER_CHECK_MEMBER_COUNT_IN_ATTACED_GUILD			:	// 소속 길드의 길드원 수가 [spin]0[1,10000] [equal]1 										
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check소속 길드원의 수 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
			break;														

		case	eTRIGGER_CHECK_SUB_GUILD_MASTER_COUNT_IN_ATTACED_GUILD	:	// 소속 길드의 서브 마스터의 수가 [spin]0[1,10000] [equal]1 												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check소속 서브 마스터의 수 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
			break;														

		case	eTRIGGER_CHECK_ELDER_MEMEBER_COUNT_IN_ATTACED_GUILD		:	// 소속 길드의 원로 길드원의 수가 [spin]0[1,10000] [equal]1 											
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check소속 원로 길드원의 수 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
			break;														

		case	eTRIGGER_CHECK_OWN_GUILD_ITEM							:	// 소속 길드에서 [guild_item]0을 [item_own]1 						
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check길드 아이템 보유 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
			break;														

		case	eTRIGGER_CHECK_GOLD_OF_ATTACED_GUILD					:	// 소속 길드에 보유 금액이 [number]0[1,2147483647]골드 [equal]1 								
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check길드 보유 금액 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
			break;														
																	
		case	eTRIGGER_CHECK_GENERATE_TIME_OF_ATTACED_GUILD			:	// 소속 길드가 만들어진후 경과한 시간이 [number]0[1,2147483647]일 [equal]1 										
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check길드 생성 시간 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
			break;														

		case	eTRIGGER_CHECK_RANKING_OF_ATTACED_GUILD					:	// 소속 길드의 랭킹이 [spin]0[1,10000] [equal]1 								
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check길드 랭킹 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
			break;														

		case	eTRIGGER_CHECK_GUILD_MASTER_LEVEL						:	// 길드 마스터의 레벨이 [spin]0[1,10000] [equal]1 							
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check길마 레벨 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
			break;														

		case	eTRIGGER_CHECK_GUILD_MASTER_JOB							:	// 길드 마스터의 직업이 [hero_job]0이다. 						
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check길마 직업 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;
			break;

		case	eTRIGGER_CHECK_GUILD_MASTER_JOB_LEVEL					:	// 길드 마스터의 직업레벨이 [spin]0[1,3] [equal]1 								
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check길마 직업레벨 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;
			break;

		case	eTRIGGER_CHECK_GUILD_MASTER_ALLIGNMENT					:	// 길드 마스터의 성향이 [allignment]0 이다. 								
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check길드 마스터의 성향 - %s\n",strOk[bIsOk]);													
			return	bIsOk;
			break;

		case	eTRIGGER_CHECK_GUILD_MASTER_BASIC_POWER					:	// 길드 마스터의 [power]0이 [spin]1[1,10000] [equal]2 								
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check길드 마스터의 능력 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;
			break;					

		case	eTRIGGER_CHECK_GUILD_STATUE_SKILL_LEVEL					:
			bIsOk	=	checkGuildStatueSkillLevel(_lpAsker);												
			if	((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check길드 스킬 레벨 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;
			break;

		case	eTRIGGER_CHECK_GUILD_PLACE_OF_RELATED	:
			bIsOk	=	checkCheckGuildPlaceOfRelated(_lpAsker);												
			if	((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check길드 연고지 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;
			break;

		case	eTRIGGER_CHECK_GUILD_HALL_LEVEL			:
			bIsOk	=	checkGuildHallLevel(_lpAsker);												
			if	((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check길드홀 레벨 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;
			break;

		case	eTRIGGER_CHECK_IS_OWN_GUILD_HALL		:
			bIsOk	=	checkIsOwnGuildHall(_lpAsker);												
			if	((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check소유 길드홀 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;
			break;

		case	eTRIGGER_CHECK_GUILD_ROEN_GOOD_WILL_CHECK		:
			bIsOk	=	checkRoenGoodWill(_lpAsker);												
			if	((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check로엔의 호감도 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;
			break;

		case	eTRIGGER_CHECK_REBIRTH_COUNT					:	//	전생 횟수 체크
			bIsOk	=	checkRebirthCount(_lpAsker);												
			if	((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check전생 횟수 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;

//	퀘스트와 관련된 사항																

		case	eTRIGGER_CHECK_COMPLETE_QUEST							:	// 퀘스트 [string]0을 [quest_complete]0 						
			bIsOk	=	checkCompleteQuest(_lpAsker,eQT_NORMAL);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check퀘스트 완료 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
																	
		case	eTRIGGER_CHECK_OWN_QUEST								:	// 퀘스트 [string]0을 [quest_own]0 					
			bIsOk	=	checkProcessQuest(_lpAsker,eQT_NORMAL);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check퀘스트 진행 중 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
																	
		case	eTRIGGER_CHECK_QUEST_PROCESS_LEVEL						:// 퀘스트 진행 상태"					:			
			bIsOk	=	checkQuestProcessLevel(_lpAsker,eQT_NORMAL);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check퀘스트 단계 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
																	
		case	eTRIGGER_CHECK_QUEST_VALUE								:// 퀘스트 수치"						:
			bIsOk	=	checkQuestQuestValue(_lpAsker,eQT_NORMAL);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check퀘스트 수치 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
																	
		case	eTRIGGER_CHECK_OWN_ARBEIT								:// 아르바이트 진행 여부"				:		
			bIsOk	=	checkProcessQuest(_lpAsker,eQT_ARBEIT);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check아르바이트 진행 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
																	
		case	eTRIGGER_CHECK_ARBEIT_PROCESS_LEVEL						:// 아르바이트 진행 상태"				:				
			bIsOk	=	checkQuestProcessLevel(_lpAsker,eQT_ARBEIT);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check아르바이트 단계 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
																	
		case	eTRIGGER_CHECK_ARBEIT_VALUE								:// 아르바이트 수치"					:	
			bIsOk	=	checkQuestQuestValue(_lpAsker,eQT_ARBEIT);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check아르바이트 수치 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
																	
		case	eTRIGGER_CHECK_PROCESS_QUEST_COUNT						:	// 	진행중인 퀘스트의 수가 [spin]0[1,10000] [equal]1						
			bIsOk	=	checkProcessQuestCount(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check진행중인 퀘스트 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
																	
		case	eTRIGGER_REMAIN_PROCESS_QUEST_SLOT						://	새로운 퀘스트를 추가 시킬 수 [exist]0							
			bIsOk	=	checkRemainProcessQuestSlot(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check새로운 퀘스트 추가 가능체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
									
		case	eTRIGGER_CHECK_COMPLETE_ARBEIT						://	퀘스트 [string]을 [quest_complete]
			bIsOk	=	checkCompleteQuest(_lpAsker,eQT_ARBEIT);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check아르바이트 완료 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_CHECK_QUEST_COMPLETE_CHECKER				://	퀘스트를 완료 상태로 체크한다.
			bIsOk	=	isCheckedQuestCompleteChecker(_lpAsker,eQT_NORMAL);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check퀘스트 완료 체커 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
		case	eTRIGGER_CHECK_ARBEIT_COMPLETE_CHECKER				://	퀘스트를 완료 상태로 체크한다.
			bIsOk	=	isCheckedQuestCompleteChecker(_lpAsker,eQT_ARBEIT);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check아르바이트 완료 체커 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_CHECK_QUEST_COUNTDOWN_CHECKER				://	퀘스트를 완료 상태로 체크한다.
			bIsOk	=	checkQuestCountdownTimer(_lpAsker,eQT_NORMAL);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check퀘스트 카운트다운 타이머 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_CHECK_ARBEIT_COUNTDOWN_CHECKER				://	퀘스트를 완료 상태로 체크한다.
			bIsOk	=	checkQuestCountdownTimer(_lpAsker,eQT_ARBEIT);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check아르바이트 카운트다운 타이머 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
			
		case	eTRIGGER_CHECK_ENABLE_QUEST							:	//	퀘스트를 시작 할 수 있는 상태다.
			bIsOk	=	checkProcessAbleQuest(_lpAsker,eQT_NORMAL);
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check퀘스트 수행 가능상태 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
			
		case	eTRIGGER_CHECK_ENABLE_ARBEIT						:	//	수행 가능한 아르바이트다.
			bIsOk	=	checkProcessAbleQuest(_lpAsker,eQT_ARBEIT);
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check아르바이트 수행 가능상태 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
			
		case	eTRIGGER_CHECK_OX_QUIZ_START_TIME						://	OX 퀴즈 시작 시간 체크							
			bIsOk	=	checkOXQuizStartTime();												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          checkOX 퀴즈 시작 시간 - %s\n",strOk[bIsOk]);													
			return	bIsOk;											
			
		case	eTRIGGER_CHECK_WORD_QUIZ_START_TIME						://	OX 퀴즈 시작 시간 체크							
			bIsOk	=	checkWordQuizStartTime();												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check낱말 퀴즈 시작 시간 - %s\n",strOk[bIsOk]);													
			return	bIsOk;

		case	eTRIGGER_CHECK_IS_FULL_GUILD						:	//	더 이상 길드 만들 수 없다.
			bIsOk	=	checkIsFullGuild();												
			if	((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check길드 생성 가/부 - %s\n",strOk[bIsOk]);													
			return	bIsOk;

		case	eTRIGGER_CHECK_IS_TEST_SERVER						:	//	테섭이다.
			bIsOk	=	checkIsTestServer();												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check테스트 서버 - %s\n",strOk[bIsOk]);													
			return	bIsOk;

		case	eTRIGGER_CHECK_BLOCK_TO_OBJECT_MACRO			:	//	테섭이다.
			bIsOk	=	checkNotObjectMacro(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check 오브젝트 매크로 방지 - %s\n",strOk[bIsOk]);													
			return	bIsOk;

		case	eTRIGGER_CHECK_EVENT_TIME			:
			bIsOk	=	checkEventTime();
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check이벤트 타임 1 - %s\n",strOk[bIsOk]);													
			return	bIsOk;
		case	eTRIGGER_CHECK_TIME										:				
			bIsOk	=	checkCurrentTime();												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check시간 체크 1 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_CHECK_TIME2									:					
			bIsOk	=	checkCurrentTime2();												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check시간체크 2 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_CHECK_TIME3										:				
			bIsOk	=	checkCurrentTime(TRUE);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check시간 체크 3 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
			
		case	eTRIGGER_CHECK_TIME4									:					
			bIsOk	=	checkCurrentTime2(TRUE);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check시간체크 4 - %s\n",strOk[bIsOk]);													
			return	bIsOk;		

		case	eTRIGGER_CHECK_WEEKDAY									:
			bIsOk	=	checkCurrentWeekDay();												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check요일체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;		

		case	eTRIGGER_CHECK_MINUTE										:				
			bIsOk	=	checkCurrentMinute();												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check분 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;

		case	eTRIGGER_CHECK_PRIZE_PLAYER								:
			bIsOk	=	checkPrizePlayer(_lpAsker);

			return	bIsOk;
		case	eTRIGGER_CHECK_TODAY_TIME								:						
			bIsOk	=	checkTodayTime();												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check오늘의 시간 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_CHECK_ENEMY_COUNT_IN_RANGE						:						
			bIsOk	=	checkEnemyCountInRange(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check주변에 적의 수 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													
		case	eTRIGGER_CHECK_HP_PERCENTAGE							:	//	자신의 hp가 x% 이상/이하이다.
			bIsOk	=	checkRemainHP(_lpAsker,_lpOwner);												
			if ((g_bIsWantPrintTriggerCondition == 1))														
				printf("          check체력 체크 - %s\n",strOk[bIsOk]);													
			return	bIsOk;													

		case	eTRIGGER_CHECK_ARRIVE_AREA								:	//	영역 x에 도착 했다.
			bIsOk	=	checkArriveArea(_lpAsker);												
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check영역 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;													

		case	eTRIGGER_CHECK_DUNGEON_FLAG								:
			bIsOk	=	checkDungeonValue(_lpField);												
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check던전 수치 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;				
			
		case	eTRIGGER_CHECK_FIELD_VALUE		:
			bIsOk	=	checkFieldValue(_lpField);												
			if	((g_bIsWantPrintTriggerCondition == 1))
				printf("          check필드 수치 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;				

		case	eTRIGGER_CHECK_WORLD_VALUE		:
			bIsOk	=	checkWorldValue();												
			if	((g_bIsWantPrintTriggerCondition == 1))
				printf("          check필드 수치 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;				

		case	eTRIGGER_CHECK_TARGET_FLEE								:
			bIsOk	=	checkTargetFlee(_lpOwner);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check타겟 도망 중 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_USE_SKILL								:
			bIsOk	=	checkUsedSkill(_lpAsker);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check스킬 사용 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_TARGET_USE_BELT_ITEM						:
			bIsOk	=	checkUsedItem(_lpAsker);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check아이템 사용 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_GUIDE									:
			bIsOk	=	checkGuide(_lpOwner);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check길잡이 유/무 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_PLAY_TIME_IN_THIS_DUNGEON				:
			bIsOk	=	checkSecretDungeonPlayTime(_lpField);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check비밀던전 플레이 타임 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_COUNTDOWN_TIMER							:
			bIsOk	=	checkSecretDungeonCountdownTimer(_lpField);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check비밀던전 플레이 타임 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_MAIN_QUEST_VALUE						:	//	메인 퀘스트 수치 x의 값이 y [이다/이상이다/이하이다]
			bIsOk	=	checkMainQuestValue(_lpAsker);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check메인 퀘스트 수치 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_MAIN_QUEST_SWITCH					:	//	 메인 퀘스트 스위치 x 번이 켜져 [있다/꺼져 있다.]
			bIsOk	=	checkMainQuestSwitch(_lpAsker);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check메인 퀘스트 스위치 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_SEX									://	성별 체크
			bIsOk	=	checkSex(_lpAsker);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check플레이어 직업 성별 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;
		case	eTRIGGER_CHECK_ATTENDANCE							:
			bIsOk	=	checkAttendance(_lpAsker);
			if	((g_bIsWantPrintTriggerCondition == 1))
				printf("          check출석 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_ITEM_BOOLEAN_OPTION	:
			bIsOk	=	checkCheckOwnItemBooleanOption(_lpAsker);
			if	((g_bIsWantPrintTriggerCondition == 1))
				printf("          check아이템 옵션 리스트 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;
			
		case	eTRIGGER_CHECK_ATTR				:
			bIsOk	=	checkAttr(_lpAsker);
			if	((g_bIsWantPrintTriggerCondition == 1))
				printf("          check성향 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_CAN_RECEIVE_CH5REWARD_TITLE_BONUS	:
			bIsOk	=	checkCanReceiveCH5RewardTitleBonus(_lpAsker);
			if	((g_bIsWantPrintTriggerCondition == 1))
				printf("          check챕터 5 칭호 보너스 받을 수 있는지 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_ENABLE_INCREASE_GUILD_STATUE_LEVEL	:
			bIsOk	=	checkCheckEnableIncreaseGuildStatueLevel(_lpAsker);
			if	((g_bIsWantPrintTriggerCondition == 1))
				printf("          check챕터 5 길드 석상 레벨 증가 가능 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_SAME_LEVEL_INCREASE_CP_AMULET	:
			bIsOk	=	checkSameLevelIncreaeCPAmulet(_lpAsker);
			if	((g_bIsWantPrintTriggerCondition == 1))
				printf("          check같은 레벨의 CP 증가 부적 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_ACTIVE_PET_COUNT	:
			bIsOk	=	checkCurrentPetCount(_lpAsker);
			if	((g_bIsWantPrintTriggerCondition == 1))
				printf("          check현재 소환된 펫의 수 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_ADD_ABLE_PET				:
			bIsOk	=	checkAvailAddPet(_lpAsker);
			if	((g_bIsWantPrintTriggerCondition == 1))
				printf("          check펫 추가 가능 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_SAME_LEVEL_INCREASE_HP_AMULET	:
			bIsOk	=	checkSameLevelIncreaeHPAmulet(_lpAsker);
			if	((g_bIsWantPrintTriggerCondition == 1))
				printf("          check같은 레벨의 체력 증가 부적 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_SAME_LEVEL_OVER_LEVEL_EQUIP_AMULET	:
			bIsOk	=	checkLastLevelWhatReceivedCH5EquipLevelBonus(_lpAsker);
			if	((g_bIsWantPrintTriggerCondition == 1))
				printf("          check같은 레벨의 레벨 초과 착용 가능 부적 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_ITEM_TIME_STAMP	:
			bIsOk	=	checkTimeStamp(_lpAsker);
			if	((g_bIsWantPrintTriggerCondition == 1))
				printf("          check타임 스탬프 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_GUILD_SKILL_BONUS_PENELTY	:
			bIsOk	=	checkGuildBonusSkillPointPenelty(_lpAsker);
			if	((g_bIsWantPrintTriggerCondition == 1))
				printf("          check챕터 5 길드 스킬 보너스 페널티 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_ENABLE_INCREASE_BONUS_GUILD_POINT	:
			bIsOk	=	checkEnableIncreaseBonusGuildPoint(_lpAsker);
			if	((g_bIsWantPrintTriggerCondition == 1))
				printf("          check길드 보너스 포인트 증가 가능 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_CH5_HEAVEN_BONUS_COUNT	:
			bIsOk	=	checkCH5HeavenBonusCount(_lpAsker);
			if	((g_bIsWantPrintTriggerCondition == 1))
				printf("          check챕터 5 천상계 보너스 받은 회수 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_MEMBER_SEX							://	멤버의 성별 체크
			bIsOk	=	checkMemberSex(_lpAsker);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check파티 멤버 성별 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;
		case	eTRIGGER_CHECK_LEVEL_RANGE							://	최고 레벨과 최저 레벨의 차이
			bIsOk	=	checkPartyLevelRange(_lpAsker);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check파티 레벨 범위체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;
		case	eTRIGGER_CHECK_MONSTER_LEVEL						://	몬스터 레벨 체크
			bIsOk	=	checkTargetMonsterLevel(_lpAsker,_lpOwner);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check몬스터 레벨 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_CHANCE								://	확률 체크
			bIsOk	=	checkProbability();
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check몬스터 레벨 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;
		case	eTRIGGER_CHECK_CHANCE2								://	확률 체크
			bIsOk	=	checkProbability2();
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check확률 체크2 - %s\n",strOk[bIsOk]);
			return	bIsOk;
		case	eTRIGGER_CHECK_LOCAL								:
			bIsOk	=	checkLocal();
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check국가 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;
			
		case	eTRIGGER_CHECK_EVENT_VALUE							:
			bIsOk	=	checkEventValue();
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check이벤트 수치 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_MONSTER_EVENT_VALUE					:
			bIsOk	=	checkMonsterEvent(_lpOwner);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check몬스터 이벤트 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;

		case	eTRIGGER_CHECK_DUEL_RANKING			:
			bIsOk	=	checkDuelRanking(_lpOwner);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check결투 랭킹 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;
		case	eTRIGGER_SEASON_VALIABLE			:
			bIsOk	=	checkSeasonVariable();
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check시즌 변수 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;
		case	eTRIGGER_CHECK_HAVE_GRACE			:
			bIsOk	=	checkIshaveGrace(_lpAsker);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check은총 보유 여부 - %s\n",strOk[bIsOk]);
			return	bIsOk;
		case	eTRIGGER_CHECK_IS_BLIND_NEW			:
			bIsOk	=	checkIshaveBlindEffect(_lpAsker);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check블라인드(영구) 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;
		case	eTRIGGER_CHECK_TILESET			:
			bIsOk	=	checkTileSet(_lpField);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check타일셋 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;
			
		case	eTRIGGER_CHECK_TRANSFORMATION_MONSTER			:
			bIsOk	=	checkTransformationMonster(_lpAsker);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check변신중 - %s\n",strOk[bIsOk]);
			return	bIsOk;
		case	eTRIGGER_CHECK_EVENT_COUNT_THE_HOURS			:
			bIsOk	=	checkEventCountTheHours();
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check이벤트 남은 시간 - %s\n",strOk[bIsOk]);
			return	bIsOk;
		case	eTRIGGER_CHECK_EVENT_TIME_BEFORE_AFTER			:
			bIsOk	=	checkEventTimeBeforeAfter();
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check이벤트 시간 전후 - %s\n",strOk[bIsOk]);
			return	bIsOk;
		case	eTRIGGER_CHECK_PLAYER_ACTION			:
			bIsOk	=	checkPlayerAction(_lpAsker);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check플레이어 액션 - %s\n",strOk[bIsOk]);
			return	bIsOk;
		case	eTRIGGER_CHECK_SECRET_DUNGEON_PLAY_COUNT			:
			bIsOk	=	checkSecretDungeonPlayCount(_lpAsker);
			if ((g_bIsWantPrintTriggerCondition == 1))
				printf("          check비밀던전 횟수 체크 - %s\n",strOk[bIsOk]);
			return	bIsOk;
	}	
	g_debugSign.m_iTrigerStep	=	3;

	g_debugSign.m_iTempStep	=	16;
																	
	return	FALSE;															
}