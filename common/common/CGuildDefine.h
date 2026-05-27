//
// CGuildDefine.h: �流E�⺻����
// : ��������, ��ų����, ������ �翁E ����E ƁE ��
//////////////////////////////////////////////////////////////////////

#ifndef _classGuildDefine_h
#define _classGuildDefine_h

#include <winsock2.h>
#ifndef _WINDOWS_
#include <windows.h>
#endif
#include <mmsystem.h>
#include "STACK.H"
#include "RSComm.h"
#include "definepacketdata.h"

#define	dGUILD_BATTLE_COUNT_AT_DAY				1	//	�Ϸ翡 �Ϲ� �����E��ՁE������?
#define	dGUILD_BATTLE_COUNT_AT_WEEK				3	//	�����Ͽ� �Ϲ� �����E��ՁE������?
#define	dTIME_TO_REMOVE_GUILD_BY_NOT_GVG_BATTLE	30

extern	int	g_iGuildBattleCountAtDay;
extern	int	g_iGuildBattleCountAtWeek;

//	0 �Ͽ���
#define	dRESET_WEEKDAY_TO_RESET_DAY_BASED_GUILD_INFO	3	//	������ ���� ������

const	int		c_iSiegeWarfareWeekDay			=	6;		//	�����
const	int		c_iResetSiegeWarfareWeekDay	=	0;
#ifdef	_FOR_JAPAN
const	int		c_iSiegeWarfareBeginHour		=	19;
#else
const	int		c_iSiegeWarfareBeginHour		=	17;
#endif
const	int		c_iSiegeWarfareBeginMinute		=	0;
const	int		c_iSiegeWarfarePeriodMinute	=	60;
const	int		c_iSiegeWarfareUpkeepMinute	=	40;
const	int		c_iSiegeWarfareBeginNoticeTime	=	10;
const	int		c_iSiegeWarfareEndNoticeTime	=	5;

const	int		c_iUpdateGuildWorkHour			=	6;
const	int		c_iMinCountForRelatedPlaceOfGuiild	=	1;

const	int		c_iGuildStategyPointCount		=	3;
const	int		c_iMinLevelForCheckUpkeepGuildPetAndGuardianLevel	=	10;

const	int		c_iDownGuildPetGuardianLevelFactor	=	10;	//	�����

const	int		c_iWarningForDownGuildPetGuardianLevelWeekDay1	=	6;	//	�����
const	int		c_iWarningForDownGuildPetGuardianLevelWeekDay2	=	0;	//	�Ͽ���
const	int		c_iDownGuildPetGuardianLevelWeekDay				=	1;	//	������

const	int		c_iGuildCrestHP					=	200000;
const	int		c_iGuildObjectHP				=	50000;

const	int		c_iGuildBattleBeginTime			=	10*60;

const	int		c_iMaxBonusGuildPoint			=	40;

#define dMAX_FIGHT_ATWEEK						4
#define dMAX_GUILD_VILLAGE_COUNT				7
#define dGUILD_NOTICE_LENGTH					256
#define	dGUILD_POLL_COMMENT_LENGTH				256
#define	dMAX_ALLIENCE_GUILD_COUNT				5
#define	dMAX_ENEMY_GUILD_COUNT					5

#define	dMAX_GUILD_SKILL_COUNT					150	//	�ִ�E�流E��ų ��E
#define	dGUILD_INVENTORY_SIZE					100	//	(OLD)�流E�κ��丮 ������E�Ϲ� �������� ����EѴ�.)
#define	dDEFAULT_GUILD_INVENTORY_SIZE			0	//	����Ʈ �流E�κ��丮 ������E
#define	dDEFAULT_GUILD_VICTORY_POINT			10

#define	dGUILD_PET_TYPE_COUNT					4	//	�流EƁE����E��E
#define	dGUILD_GUARDIAN_TYPE_COUNT				4	//	�流E����E����E��E
#define	dMAX_GUILD_PET_COUNT					50	//	�ִ�E�流EƁE��E
#define	dMAX_GUILD_GUARDIAN_COUNT				10	//	�ִ�E�流E����E��E
#define	dMAX_GUILD_OBJECT_COUNT					100	//	�ִ�E�流E����E�Ʈ ��E
#define	dMAX_GUILD_STRUCTURE_COUNT				200	//	�ִ�E�����E��Ʈ���� ��E
#define dMAX_GUILD_INVENTORY_COUNT				200	//	�ִ�E�流E�κ��丮 ��E
#define	dMAX_GUILD_SELLING_AGENT_ITEM_COUNT		50
#define	dMAX_GUILD_LEVEL						200
#define	dMAX_GUILD_SKILL_LEVEL					100
#define	dGUILD_MASTER_MINIMUM_LEVEL				50
#define	dGUILD_SUB_MASTER_MINIMUM_LEVEL			30
#define	dGUILD_MEMBER_MINIMUM_LEVEL				10


const	int	c_iGuildInventorySize		=	200;
const	int	c_iMaxGuildPetExp			=	25000000;

#ifdef	_FOR_JAPAN
#define	dMAX_GUILD_BATTLE_COUNT_AT_WEEK			5
#else
#define	dMAX_GUILD_BATTLE_COUNT_AT_WEEK			3
#endif

const	int	c_iNormalGuildSkillCount			=	100;
const	int	c_iGuildHallSellEnchantItemCount	=	32;

const	int	c_iGuildBattlePeroidByFieldOrder	=	5;
const	int	c_iSameTimeFiledCountInGuildBattle	=	2;
const	int	c_iGuildBankCount					=	10;


const	int	c_iGuildStatueSkillBeginIndex		=	100;
const	int	c_iGuildStatueCount					=	6;
const	int	c_iGuildStatuePartsCount			=	5;
const	int	c_iGuildStatueSetEffectRequireCount	=	3;

const	int	c_iMaxSubGuildMasterCount			=	10;
const	int	c_iMaxSubGuildCongressCount			=	30;
const	int	c_iMaxGuildMemberCount				=	200;

#define	dCANCEL_GUILD_BATTLE_PENELTY_COUNT					5
#define	dMINIMUM_DIFFERENT_VICTORY_POINT_FOR_GUILD_BATTLE	20
#define	dGUILD_BATTLE_CANCEL_ABLE_TIME						60
#define	dGUILD_BATTLE_REQUEST_PRICE							10000

const	int	c_iLimitOfTryGuildPointBattlePerWeek	=	2;
const	int	c_iLimitOfTryGuildDungenPerDay			=	2;



#define	dGUILD_BATTLE_LOBBY_WARP_RANGE			300
#define	dMINIMUM_DAY_FOR_GUILD_BATTLE			7
#define	dMINIMUM_LEVEL_FOR_GUILD_BATTLE			5
#define	dMINUMUM_GUILD_BATTLE_VICTORY_POINT		1
#define	dMAXIMUM_GUILD_BATTLE_VICTORY_POINT		5


enum
{
	eGUILDNOTICETYPE_NORMAL,	//	�Ϲ� ����E	
	eGUILDNOTICETYPE_RUSH,		//	����
};

enum
{
	eGUILD_PET_OBITIAN	=	1,
	eGUILD_PET_DASH_BLADE	,
	eGUILD_PET_ASTRO_BOW	,
	eGUILD_PET_CRISTAL_WATER,
};

enum
{ // �流E���� ���� 
		eDGR_NORMAL,		// ������ ��忁EŻŁE		
		eDGR_CREATE_FAIL,	//	���� ����
		eDGR_TIME_OVER,		//	��忁E������ ����ح
		eDGR_NOT_GVG_BATTLE,	//	GVG �����ε� �����E� ���ؼ� ��ü��
};

enum
{ // �流E��ũ ����E 
		eCGR_APPOINT	=	1,		// 	�Ӹ�E		
		eCGR_DISMISSAL,		//	����
		eCGR_CHANGERANK_MASTER_APPOINT_SPECIAL,	//	�����Ͱ� �ɸ��� ����E		
		eCGR_TIME_OVER,		//	�����Ⱓ ���Ӿ���
};

enum
{
	eGUILD_GUARDIAN_SHIELD_CRAFT	=	1,
	eGUILD_GUARDIAN_ELEMENTAL_POST	,
	eGUILD_GUARDIAN_ETHER_SHELL		,
	eGUILD_GUARDIAN_HOLLOW_KNIGHT	,
};

const	int	c_iGuildHallCount				=	40;
const	int	c_iMaxGuildHallLevel			=	5;

class	cGuildHallInfo
{
public:
	WORD	m_wOwnGuild;
	DWORD	m_dwHashCode;
	WORD	m_wHallSerial;

	DWORD	m_bf3HallLevel	:	3;	//	Ȧ ����
	DWORD	m_bf1IsOpen		:	1;	//	���� �ֳ�?
	DWORD	m_bf3HallShape	:	3;	//	Ȧ ��E	
	DWORD	m_bf5Slot		:	5;	//	Ȧ �ε���
	DWORD	m_bf12BattleField:	12;

	WORD	m_wInvadeGuild;			//	���� �流E
	void	reset()
	{
		m_wOwnGuild		=	0xffff;
		m_dwHashCode	=	0xffffffff;
	}
};

class	cSiegeWarfareInfoOfGuild
{
public:
	WORD	m_wInvadeGuild,m_wDefenseGuild;
	WORD	m_wBattleField;
	BYTE	m_bHour,m_bMinute;
};


class	cGuildInventoryItemForPacket	:	public CItemDefine
{
public:
	WORD	m_wSlot;
};

//
//	��忁E����
class CGuildMan
{
public:
	WORD		wSerial;
	char		m_strId[dID_LENGTH];
	char		m_strName[dNAME_LENGTH];
	WORD		m_wRank;					//	�流E�ȿ��� �ڽ��� ��ġ
	WORD		m_wJob;						//	����E	
	WORD		m_wLevel;					//	ĳ���� ����
	WORD		m_bf1IsConnected	:	1;
	WORD		m_bf1IsPermitToTryGuildPointBattle	:	1;	//	�流E����Ʈ��EE��� ����� �⹁E	
	WORD		m_bf3ReservePet		:	3;	//	�����E� ���� ������� ����� ƁE
	DWORD		m_dwBestowValue;			//	���̳� �����ۿ� ��E� ��μ�ġ, ������ �ִ�. (�������̳� ���� �ٽ� ������ ��E�ִ�.)

				CGuildMan()
				{
					close();
				}

	void	copy(CGuildMan * _p)
	{	
		memcpy(this,_p,sizeof(CGuildMan));	
	}

	void	close()
	{
		memset(this,0,sizeof(CGuildMan));
		wSerial				=	0xffff;
	}
};

//
//	Guild Poll Type
enum
{
	eGPT_NORMAL_EVENT,
	eGPT_CHANGE_MASTER,
};

#define	dGUILD_POLL_CONTENTS_COUNT		10
#define	dGUILD_POLL_CONTENTS_LENGTH		64

//
//
class	CGuildPollInfo
{
public:
	WORD		m_wType,wSerial;	//	Ÿ�԰�E��ǥ �ø���E	
	char		m_strComment[dGUILD_POLL_COMMENT_LENGTH];	//	��ǥ �Ȱ�?
	WORD		m_awResult[dGUILD_POLL_CONTENTS_COUNT];	//	��ǥ �ᰁE	
	char		m_strContents[dGUILD_POLL_CONTENTS_COUNT][dGUILD_POLL_CONTENTS_LENGTH];	//	���� ����
	UTime		m_timeBegin,m_timeEnd;	//	��ǥ ���� �� �ð�

	void		reset()
	{
		m_wType		=	0xffff;
		wSerial		=	0;
		memset(m_awResult,0,sizeof(m_awResult));
	}
};

//
//	�流EƁE����
class	CGuildPetInfoOld
{
public:
	char		m_strName[dNAME_LENGTH];
	WORD		m_wSerial;
	WORD		m_wLevel;

	DWORD		m_dwStrengthExp;
	DWORD		m_dwConstitutionExp;
	DWORD		m_dwAgilityExp;
	DWORD		m_dwIntelligenceExp;
	DWORD		m_dwWisdomExp;
	DWORD		m_dwCharismaExp;
	DWORD		m_dwLuckExp;

	void		reset()
	{
		memset(this,0,sizeof(CGuildPetInfoOld));
		m_wSerial	=	0xffff;
	}
};

class	CGuildPetInfo
{
public:
	WORD		m_wExpDuringWeek;
	DWORD		m_dwExperience;

	void		reset()
	{
		m_dwExperience	=	0;
		m_wExpDuringWeek=	0;
	}
};


class	CGuildSkillInfo
{
public:
	WORD		m_wSkill;
	WORD		m_wLevel;
		
	void		reset()
	{
		m_wSkill	=	0xffff;
		m_wLevel	=	0xffff;
	}
};

class	CGuildObjectInfoOld
{
public:
	WORD		m_wObject;
	WORD		m_wXPos,m_wYPos;
	WORD		m_wValue;		//	��Ÿ ����?

	void		reset()
	{
		memset(this,0,sizeof(CGuildObjectInfoOld));
		m_wObject	=	0xffff;
	}
};

#pragma pack(2)
class	cGuildStructureInfo
{
public:
	WORD		m_wObject;
	WORD		m_wXPos,m_wYPos;

	void		reset()
	{
		memset(this,0,sizeof(cGuildStructureInfo));
		m_wObject	=	0xffff;
	}
};

#pragma pack()

typedef union _union_owned_guild_mark_item
{
    struct 
	{
		DWORD		m_bf1IsOwnedExpandPack	: 1;	//	1
		DWORD		m_bf1IsOwnedSpecialPack	: 1;	//	2
		DWORD		m_bf1IsOwnedLuxuryPack	: 1;	//	3

		DWORD		m_bf1IsOwnedHangulPack	: 1;	//	4
		DWORD		m_bf1IsOwnedEnglishPack	: 1;	//	5
		DWORD		m_bf1IsOwnedHanmunPack	: 1;	//	6
		DWORD		m_bf1IsOwnedItemPack	: 1;	//	7
		DWORD		m_bf1IsOwnedAnimalPack	: 1;	//	8
		
		DWORD		m_bf1IsOwnedSignalPack	: 1;	//	9
		DWORD		m_bf1IsOwnedUnique1Pack	: 1;	//	10
		DWORD		m_bf1IsOwnedUnique2Pack	: 1;	//	11
    };

    DWORD	m_dwValue;

}	UOwnedGuildMarkItemInfo;

class	CGuildMarkInfo
{
public:
	WORD		m_wIcon;
	BYTE		m_bIconColor;
	BYTE		m_bBaseShape;
	BYTE		m_bBaseColorPartition;
	BYTE		m_bBaseColor1,m_bBaseColor2;
	BYTE		m_bBaseLineColor;

	UOwnedGuildMarkItemInfo	m_ownedMarkItem;
	DWORD		m_dwValue2;

	void		reset()
	{
		memset(this,0xff,sizeof(CGuildMarkInfo));

		m_ownedMarkItem.m_dwValue	=	0;
	}
};
//
enum
{
	eGMIP_DEFAULT,
	eGMIP_EXPAND,
	eGMIP_SPECIAL,
	eGMIP_LUXURY,
	eGMIP_ETC,
};

class	CGuildMarkPackInfo
{
public:
	WORD	m_wBasePackBegin,m_wBasePackEnd;
	WORD	m_wExpandPackBegin,m_wExpandPackEnd;
	WORD	m_wSpecialPackBegin,m_wSpecialPackEnd;
	WORD	m_wLuxuryPackBegin,m_wLuxuryPackEnd;

	int		getPart(int _iIndex)
	{
		if (_iIndex >= m_wBasePackBegin && _iIndex <= m_wBasePackEnd)
			return	eGMIP_DEFAULT;
		if (_iIndex >= m_wExpandPackBegin && _iIndex <= m_wExpandPackEnd)
			return	eGMIP_EXPAND;
		if (_iIndex >= m_wSpecialPackBegin && _iIndex <= m_wSpecialPackEnd)
			return	eGMIP_SPECIAL;
		if (_iIndex >= m_wLuxuryPackBegin && _iIndex <= m_wLuxuryPackEnd)
			return	eGMIP_LUXURY;

		return	eGMIP_ETC;
	}
};

class	CGuildMarkItemPackInfo
{
public:
	CGuildMarkPackInfo	m_base,m_division;
	CGuildMarkPackInfo	m_symbolHangul,m_symbolEnglish,m_symbolHanmun,m_symbolItem,m_symbolAnimal ;

	inline	BOOL		isOwnedFittingPack(int _iPart,UOwnedGuildMarkItemInfo *_lpInfo)
	{
		switch(_iPart)
		{
			case	eGMIP_DEFAULT	:
				return	TRUE;
			case	eGMIP_EXPAND	:
				if (_lpInfo->m_bf1IsOwnedExpandPack)
					return	TRUE;
				break;
			case	eGMIP_SPECIAL	:
				if (_lpInfo->m_bf1IsOwnedSpecialPack)
					return	TRUE;
				break;
			case	eGMIP_LUXURY	:
				if (_lpInfo->m_bf1IsOwnedLuxuryPack)
					return	TRUE;
				break;
		}

		return	FALSE;
	}
	///

	BOOL				isEnableSignalSymbol(int _iIndex,UOwnedGuildMarkItemInfo *_lpInfo)
	{
		if (_lpInfo->m_bf1IsOwnedSignalPack)
			return	TRUE;
		
		return	FALSE;
	}

	BOOL				isEnableUnique1Symbol(int _iIndex,UOwnedGuildMarkItemInfo *_lpInfo)
	{
		if (_lpInfo->m_bf1IsOwnedUnique1Pack)
			return	TRUE;
		
		return	FALSE;
	}

	BOOL				isEnableUnique2Symbol(int _iIndex,UOwnedGuildMarkItemInfo *_lpInfo)
	{
		if (_lpInfo->m_bf1IsOwnedUnique2Pack)
			return	TRUE;
		
		return	FALSE;
	}

	////
	BOOL				isEnableDivision(int _iIndex,UOwnedGuildMarkItemInfo *_lpInfo)
	{
		return	isOwnedFittingPack(m_division.getPart(_iIndex),_lpInfo);
	}

	BOOL				isEnableBase(int _iIndex,UOwnedGuildMarkItemInfo *_lpInfo)
	{
		return	isOwnedFittingPack(m_base.getPart(_iIndex),_lpInfo);
	}

	BOOL				isEnableHangulSymbol(int _iIndex,UOwnedGuildMarkItemInfo *_lpInfo)
	{
		if (_lpInfo->m_bf1IsOwnedHangulPack)
			return	TRUE;

		return	isOwnedFittingPack(m_symbolHangul.getPart(_iIndex),_lpInfo);
	}

	BOOL				isEnableEnglishSymbol(int _iIndex,UOwnedGuildMarkItemInfo *_lpInfo)
	{
		if (_lpInfo->m_bf1IsOwnedEnglishPack)
			return	TRUE;

		return	isOwnedFittingPack(m_symbolEnglish.getPart(_iIndex),_lpInfo);
	}

	BOOL				isEnableHanmunSymbol(int _iIndex,UOwnedGuildMarkItemInfo *_lpInfo)
	{
		if (_lpInfo->m_bf1IsOwnedHanmunPack)
			return	TRUE;

		return	isOwnedFittingPack(m_symbolHanmun.getPart(_iIndex),_lpInfo);
	}
	BOOL				isEnableItemSymbol(int _iIndex,UOwnedGuildMarkItemInfo *_lpInfo)
	{
		if (_lpInfo->m_bf1IsOwnedItemPack)
			return	TRUE;

		return	isOwnedFittingPack(m_symbolItem.getPart(_iIndex),_lpInfo);
	}
	BOOL				isEnableAnimalSymbol(int _iIndex,UOwnedGuildMarkItemInfo *_lpInfo)
	{
		if (_lpInfo->m_bf1IsOwnedAnimalPack)
			return	TRUE;

		return	isOwnedFittingPack(m_symbolAnimal.getPart(_iIndex),_lpInfo);
	}

	BOOL				isEnableSymbol(int _iIndex,UOwnedGuildMarkItemInfo *_lpInfo)
	{
		int	iKind	=	_iIndex	/	1000;

		_iIndex		=	_iIndex	-	iKind*1000;

		switch(iKind)
		{
			case	0	:	//	�ѱ�
				return	isEnableHangulSymbol(_iIndex,_lpInfo);

			case	1	:	//	����E				
				return	isEnableEnglishSymbol(_iIndex,_lpInfo);

			case	2	:	//	�ѹ�
				return	isEnableHanmunSymbol(_iIndex,_lpInfo);

			case	3	:	//	������
				return	isEnableItemSymbol(_iIndex,_lpInfo);

			case	4	:	//	����
				return	isEnableAnimalSymbol(_iIndex,_lpInfo);
			case	5	:
				return	isEnableSignalSymbol(_iIndex,_lpInfo);;
			case	6	:
				return	isEnableUnique1Symbol(_iIndex,_lpInfo);;
			case	7	:
				return	isEnableUnique2Symbol(_iIndex,_lpInfo);;
		}

		return	FALSE;
	}

	int					getSymbolPart(int _iIndex)
	{
		int	iKind	=	_iIndex	/	1000;

		_iIndex		=	_iIndex	-	iKind*1000;

		switch(iKind)
		{
			case	0	:	//	�ѱ�
				return	m_symbolHangul.getPart(_iIndex);

			case	1	:	//	����E				
				return	m_symbolEnglish.getPart(_iIndex);

			case	2	:	//	�ѹ�
				return	m_symbolHanmun.getPart(_iIndex);

			case	3	:	//	������
				return	m_symbolItem.getPart(_iIndex);

			case	4	:	//	����
				return	m_symbolAnimal.getPart(_iIndex);

			case	5	:
				return	0xffff;

			case	6	:
				return	0xffff;

			case	7	:
				return	0xffff;
		}

		return	0xffff;
	}
};
// 
// //
// //	�ʿ信 ����E��E����ְų� ��E��� �ʿ䰡 ���� ����
// class	CGuildAdvanceInfoOld
// {
// public:
// //	�流E�⺻ ����
// 	WORD				m_wSerial;								//	�流E��ܯ �ø���E
// 	char				m_strName[dGUILD_NAME_LENGTH];			//	�流E�̸�
// 	WORD				m_wLevel;								//	�流E����
// 	DWORD				m_dwExp;								//	�流E����ġ
// 	int					m_iVictoryPoint;						//	�流E����
// 	WORD				m_wVictoryContinue,m_wVictoryCount,m_wDefeatCount,m_wDrawCount,m_wCancelCount;
// 
// 	DWORD				m_dwGold;								//	�流E���� �ݾ�
// 	WORD				m_wHallLevel;							//	���Ȧ ����
// 	WORD				m_wMaxMember,m_wMemberCount;			//	�ִ�E��忁E��E����E��忁E��E
// 	WORD				m_wMaxSubMaster,m_wSubMasterCount;		//	�ִ�E����E�渶 ��E����E����E�渶 ��E
// 	WORD				m_wMaxCongressMan,m_wCongressManCount;	//	�ִ�E�流E�ǿ�E��E����E���流E�ǿ�E��E
// 	WORD				m_wIsGetGuildExp,m_wExpTaxRate;			//	�流E����ġ�� �������ΰ�? �流E����ġ ����-_-
// 	WORD				m_awGuildVillage[dMAX_GUILD_VILLAGE_COUNT];
// 
// 	CGuildMarkInfo		m_guildMarkInfo;						//	�流E��ũ
// 	WORD				m_wGuildPoint;
// 	UTime				m_timeOrganizeDate;						//	�流E���� �ð�.
// 
// 	WORD				m_wBestowItemMinimumLevel,m_wSellingAgentItemMinimumLevel;	//	��� �ϴ� �����۰�E�ǸŴ�E�E������ �ּ� ����
// 	WORD				m_wRelatedPlaceCount,m_wFightGuild[dMAX_FIGHT_ATWEEK];//	���ֿ� �ο�EȽ��E �ֱ� �ο�E��嵁E.
// //	����E
// 	char	m_strNotice[dGUILD_NOTICE_LENGTH];		//	�流E����E
//  WORD				m_wNoticeType;							//	�流E����EŸ��
// 
// //	�流E������ ����
// 	char				m_strMasterName[dNAME_LENGTH];			//	�流E������ �̸�
// 	WORD				m_wElderCount;			//	�流E��μ�E// 	
//	WORD				m_wGuildMasterLevel;	//	�流E�������� ������E����
// 
// //	�����E����
// 	WORD				m_wTargetGuild,m_wInvadeGuild;			//	���� ��ǥ �流E ħ��-_-�流E �����E������Ʈ�� ��ǥ��尡 �̹� ������E���ִ� ���Ը �Ѵ�.
// 	UTime				m_timeLastConnectTime;					//	������ �������� ���������� ������ �ð�
// //	�流E��ų ����
// 	WORD				m_aGuildSkill[dMAX_GUILD_SKILL_COUNT];	//	�流E��ų
// 
// 	UTime				m_timeLastConnectedMasterTime;			//	�流E�����Ͱ� ���������� ������ �ð�
// 
// 	WORD				m_wSetGuildTaxExpSettingDay;								//	���������� �流E����ġ�� ��E� ������ �ٲ۳�(������ ��E���� �����Ǿ�E�ִٸ�E����E�Ұ���.)
// 	WORD				m_wGuildInventorySize;										//	�流E�κ��丮 ������E// 
// 	WORD				m_aAllienceGuild[dMAX_ALLIENCE_GUILD_COUNT],m_aEnemyGuild[dMAX_ENEMY_GUILD_COUNT];
// 	CGuildPetInfoOld	m_aGuildPet[dMAX_GUILD_PET_COUNT];
// 	CGuildPetInfoOld	m_aGuildGuardian[dMAX_GUILD_GUARDIAN_COUNT];
// 	CGuildObjectInfoOld	m_aGuildObject[dMAX_GUILD_OBJECT_COUNT];
// 	UTime				m_timeBeginGuildMaster;										//	�流E������ ÁE� �ð�
// 
// 	CItemDefine			m_aInventory[dGUILD_INVENTORY_SIZE];						//	�流E����E������
// 	CSellingAgentItem	m_aSellingAgentItem[dMAX_GUILD_SELLING_AGENT_ITEM_COUNT];	//	�ִ�E�ǸŴ�E�E������ ��E
// 	CGuildPollInfo		m_guildPoll;												//	�流EƁE};

//
//	DB�� �÷����� ������ ������
class	CGuildSaveInfoForColumn
{
public:
	WORD	m_wSerial;	//���ϵ� �ø���E	
	char	m_strName[dGUILD_NAME_LENGTH];//����̸�
	WORD	m_wLevel;//��巹��
	int		m_iVictoryPoint;//������ �����E�¸� ����Ʈ
	int		m_iHonorPoint;//���Ȧ ��E���E�¸� ����Ʈ
	WORD	m_wGuildPoint;
	int		m_wHallLevel;//���Ȧ ����
	UTime	m_timeOrganizeDate;// �������ð� - ������ �ʿ䰡 ����.
	char	m_strNotice[dGUILD_NOTICE_LENGTH];//����E	
	WORD	m_wNoticeType;//����Ÿ��
	char 	m_strMasterName[dNAME_LENGTH];//�流E������ ��E	
	UTime	m_timeLastConnectTime;//��忁E�� ������E���������� �����ߴٸ�E.
	WORD	m_wGuildHallShape;//�流E�ʵ�E��E (�� �������� ������ �����Ѵ�.)
	LONGLONG	m_llGuildGold;// ����� ��E
};

//
//	DB�� ���̳ʸ��� ����� ������
class	CGuildSaveInfoForBinary
{
public:
	DWORD	m_dwExp;
	WORD	m_wVictoryContinue,m_wVictoryCount,m_wDefeatCount,m_wDrawCount,m_wCancelCount;
	WORD	m_wBattle_VCC,m_wBattle_VC,m_wBattle_DC,m_wBattle_CC;
	WORD 	m_wMaxMember;
	WORD	m_wMaxSubMaster;
	WORD 	m_wMaxCongressMan;
	WORD	m_wIsGetGuildExp,m_wExpTaxRate;
	WORD	m_awGuildVillage[dMAX_GUILD_VILLAGE_COUNT];
	DWORD	m_bf1SubMasterEnableBanish				:	1;
	DWORD	m_bf1IsPermitVisitor					:	1;	//	���Ȧ�� ������°� �㿁E�.
	DWORD	m_bf2TryGuildPointBattleCountPerWeek	:	2;	//	�̹�E�ֿ� �流E����Ʈ��E� ����E� Ƚ��E	
	DWORD	m_bf1IsTestSiegeWarfareGuild			:	1;	//	�׽�Ʈ �流E	
	DWORD	m_bf1IsTwinkle							:	1;	//	�流E��ũ�� ��¦ �Ÿ���.
	DWORD	m_bf6BonusGuildPoint					:	6;
	DWORD	m_bf6PeneltyBonusGuildPoint				:	6;  //	22

	DWORD	m_bf3GuildGoodwillForRoen				:	3;	//	�ο��� ȣ����. 25
	DWORD	m_bf3GuildDungeonPlayTime				:	3;	//	�流E���� �÷��� Ƚ��E	
	DWORD	m_bf1IsUseGuildMasterGrace				:	1;	//	�流E������ ���� �ߵ� �޳�? 29

	WORD	m_wRelatedPlaceCount,m_wFightGuild[dMAX_FIGHT_ATWEEK];
	WORD	m_wElderCount;
	WORD	m_wTargetGuild,m_wInvadeGuild;
	WORD	m_wSetGuildTaxExpSettingDay;
	WORD	m_wGuildInventorySize;
	UTime	m_timeBeginGuildMaster;
	UTime	m_timeLastConnectedMasterTime;//�流E�����Ͱ� ���������� ������ �ð�

//	�߰��� ����E	
	int		m_aiWithdrawGoldLimitPerDay[5];			//	������ �Ϸ翡 ���� ��E�ִ� ��E����
	BYTE	m_aWithdrawItemCountPerDay[4][5];			//	������ �Ϸ翡 ���� ��E�ִ� ������ ��E����
	BYTE	m_bChangeInventorySettingAdmin;			//	�κ��丮 ���� ����E������ ����
	BYTE	m_abInventorySize[4];					//	�κ��丮 ������E	
	WORD	m_awGuildInventoryManagerVillage[8];	//	�流Eâ��E������ �ִ� ����E
	UINT	m_bf6LastGVGBattleYear	:	6;	//	���������� GVG �������� ��E��� �ð�
	UINT	m_bf4LastGVGBattleMonth:	4;
	UINT	m_bf5LastGVGBattleDay	:	5;
	UINT	m_bf5ThisWeekGuildBattleCount:	5;	//	�ִ�E�����EȽ��E����
	UINT	m_bf3ThisDayGuildBattleCount:	3;	//	�ϴ�E�����EȽ��E����

};

class	CGuildSaveInfoSkill
{
public:
	WORD	m_aGuildSkill[dMAX_GUILD_SKILL_COUNT];
};

class	CGuildSaveInfoMark
{
public:
	CGuildMarkInfo	m_guildMarkInfo;
};

class	CGuildSaveInfoBank
{
public:
	WORD	m_awGuildBank[c_iGuildBankCount];
};

class	CGuildSaveInfoRelShip
{
public:
	WORD	m_aAllienceGuild[dMAX_ALLIENCE_GUILD_COUNT],m_aEnemyGuild[dMAX_ENEMY_GUILD_COUNT];		//	���� �流E��E�E���[����E�E����]
};

class	CGuildSaveInfoPet
{
public:
	CGuildPetInfo	m_aGuildPet[dGUILD_PET_TYPE_COUNT];
};

class	CGuildSaveInfoGuardian
{
public:
	CGuildPetInfo	m_aGuildGuardian[dGUILD_GUARDIAN_TYPE_COUNT];
};

class	CGuildSaveInfoInventory
{
public:
	CItemDefine			m_aInventory[c_iGuildInventorySize];
};

class	CGuildSaveInfoStructure
{
public:
	cGuildStructureInfo	m_aGuildStructure[dMAX_GUILD_STRUCTURE_COUNT];
};

class	cGuildInfoForTransToGVG
{
public:
	char				m_strName[dGUILD_NAME_LENGTH];			//	�流E�̸�
	WORD				m_wLevel;								//	�流E����
	CGuildMarkInfo		m_guildMarkInfo;						//	�流E��ũ
	WORD				m_wGuildPoint;							//	��ų ����Ʈ

	DWORD				m_bf1IsTwinkle							:	1;	//	�流E��ũ�� ��¦ �Ÿ���.
	DWORD				m_bf6BonusGuildPoint					:	6;
	DWORD				m_bf6PeneltyBonusGuildPoint				:	6;

	BYTE				m_abGuildSkill[dMAX_GUILD_SKILL_COUNT];			//	�流E��ų
};

//
//	�流E�⺻���� �翁Eڰ� �流E������ ��û�Ҷ� ������ �ּ����� ����
class	CGuildBaseInfo
{
public:
//	�流E�⺻ ����
	WORD				m_wSerial;								//	�流E��ܯ �ø���E	
	char				m_strName[dGUILD_NAME_LENGTH];			//	�流E�̸�
	WORD				m_wLevel;								//	�流E����
	DWORD				m_dwExp;								//	�流E����ġ
	int					m_iVictoryPoint;						//	�流E����
	WORD				m_wVictoryContinue,m_wVictoryCount,m_wDefeatCount,m_wDrawCount,m_wCancelCount;

	//	���Ȧ������ ���ϴµ� ��E�. 15�Ͽ� �ѹ��� �����ȴ�.
	int					m_iHonorPoint;							//	��۹ ����Ʈ[�̸��� �ٲ�]
	WORD				m_wHallLevel;							//	���Ȧ ����
	WORD				m_wMaxMember,m_wMemberCount;			//	�ִ�E��忁E��E����E��忁E��E	
	WORD				m_wMaxSubMaster,m_wSubMasterCount;		//	�ִ�E����E�渶 ��E����E����E�渶 ��E
	WORD				m_wMaxCongressMan,m_wCongressManCount;	//	�ִ�E�流E�ǿ�E��E����E���流E�ǿ�E��E	
	WORD				m_wIsGetGuildExp,m_wExpTaxRate;			//	�流E����ġ�� �������ΰ�? �流E����ġ ����-_-
	WORD				m_awGuildVillage[dMAX_GUILD_VILLAGE_COUNT];

	CGuildMarkInfo		m_guildMarkInfo;						//	�流E��ũ
	WORD				m_wGuildPoint;							//	��ų ����Ʈ
	UTime				m_timeOrganizeDate;						//	�流E���� �ð�.

	DWORD				m_bf1SubMasterEnableBanish				:	1;	//	m_wBestowItemMinimumLevel,m_wSellingAgentItemMinimumLevel ����
	DWORD				m_bf1IsPermitVisitor					:	1;	//	���Ȧ�� ������°� �㿁E�.
	DWORD				m_bf2TryGuildPointBattleCountPerWeek	:	2;	//	�̹�E�ֿ� �流E����Ʈ��E� ����E� Ƚ��E	
	DWORD				m_bf1IsTestSiegeWarfareGuild			:	1;	//	�̹�E�ֿ� �流E����Ʈ��E� ����E� Ƚ��E	
	DWORD				m_bf1IsTwinkle							:	1;	//	�流E��ũ�� ��¦ �Ÿ���.
	DWORD				m_bf6BonusGuildPoint					:	6;
	DWORD				m_bf6PeneltyBonusGuildPoint				:	6;
	DWORD				m_bf3GuildGoodwillForRoen				:	3;	//	�ο��� ȣ����.
	//DWORD				m_bf3GuildDungeonPlayTime				:	3;	//	�流E���� ī���
	DWORD				m_bf1IsUseGuildMasterGrace				:	1;	//	�̹��ֿ� �流E������ ���� �ߵ��޳�?
																//	11

//	DWORD				m_bf31SpareBitField						:	31;	//	m_wBestowItemMinimumLevel,m_wSellingAgentItemMinimumLevel ����
//	2*5
	WORD				m_wRelatedPlaceCount,m_wFightGuild[dMAX_FIGHT_ATWEEK];//	���ֿ� �ο�EȽ��E �ֱ� �ο�E��嵁E.
//	����E	
	char				m_strNotice[dGUILD_NOTICE_LENGTH];		//	�流E����E	
	WORD				m_wNoticeType;							//	�流E����EŸ��

//	�流E������ ����
	char				m_strMasterName[dNAME_LENGTH];			//	�流E������ �̸�
	WORD				m_wElderCount;			//	�流E��μ�E	
	WORD				m_wGuildMasterLevel;	//	�流E�������� ������E����

//	�����E����
	WORD				m_wTargetGuild,m_wInvadeGuild;			//	���� ��ǥ �流E ħ��-_-�流E �����E������Ʈ�� ��ǥ��尡 �̹� ������E���ִ� ���Ը �Ѵ�.
	UTime				m_timeLastConnectTime;					//	������ �������� ���������� ������ �ð�

	UTime				m_timeLastConnectedMasterTime;			//	�流E�����Ͱ� ���������� ������ �ð�

	WORD				m_wSetGuildTaxExpSettingDay;	//	���������� �流E����ġ�� ��E� ������ �ٲ۳�(������ ��E���� �����Ǿ�E�ִٸ�E����E�Ұ���.)[����E�E����]
	WORD				m_wGuildInventorySize;			//	�流E�κ��丮 ������w����E�E����]
	UTime				m_timeBeginGuildMaster;			//	�流E������ ÁE� �ð�[����E�E����]
	LONGLONG			m_llGuildGold;					//	�流E���� �ݾ�[�߰���]

	WORD				m_wGuildHallShape;				//	���Ȧ ���[�߰���]
	WORD				m_wBattle_VCC,m_wBattle_VC,m_wBattle_DC,m_wBattle_CC;	//	����,��,��,ĵ��[�߰���]
//	-------------------------------- �߰���
	int					m_aiWithdrawGoldLimitPerDay[5];			//	������ �Ϸ翡 ���� ��E�ִ� ��E����
	BYTE				m_aWithdrawItemCountPerDay[4][5];			//	������ �Ϸ翡 ���� ��E�ִ� ������ ��E����
	BYTE				m_bChangeInventorySettingAdmin;			//	�κ��丮 ���� ����E������ ����
	BYTE				m_abInventorySize[4];					//	�κ��丮 ������E	
	WORD				m_awGuildInventoryManagerVillage[8];	//	�流Eâ��E������ �ִ� ����E
	UINT				m_bf6LastGVGBattleYear		:	6;	//	���������� GVG �������� ��E��� �ð�
	UINT				m_bf4LastGVGBattleMonth		:	4;
	UINT				m_bf5LastGVGBattleDay		:	5;

	UINT				m_bf5ThisWeekGuildBattleCount:	5;	//	�ִ�E�����EȽ��E����
	UINT				m_bf3ThisDayGuildBattleCount:	3;	//	�ϴ�E�����EȽ��E����

//	�流E��ų ����
	BYTE				m_abGuildSkill[dMAX_GUILD_SKILL_COUNT];	//	�流E��ų

//	�ִ�E����
	int					getMaximumExpTax()
	{
	//	�流E����ġ ����E��ġ�� �÷��̾�E����ġ�� 0.05%~(10/��巹��)%�� (����E�� ���� ó��)
		return	10*100/m_wLevel;
	}

//	�ּ� ����
	int					getMinimumExpTax()
	{
		return	5;
	}
	int					getFullSkillPoint();
	void				reset();
};

const	int	c_i2LevelGuildHallFieldSerial	=	943;
const	int	c_i5LevelGuildHallFieldSerial	=	982;

//
//	�ʿ信 ����E��E����ְų� ��E��� �ʿ䰡 ���� ����
class	CGuildAdvanceInfo : public CGuildBaseInfo
{
public:
	WORD				m_awGuildBank[c_iGuildBankCount];	//	�流E������ �ʵ�E�ø���[�߰���]
	WORD				m_aAllienceGuild[dMAX_ALLIENCE_GUILD_COUNT],m_aEnemyGuild[dMAX_ENEMY_GUILD_COUNT];		//	���� �流E��E�E���[����E�E����]
	CGuildPetInfo		m_aGuildPet[dGUILD_PET_TYPE_COUNT],m_aGuildGuardian[dGUILD_GUARDIAN_TYPE_COUNT];	//	�流EƁE����E[�����]
	CItemDefine			m_aInventory[c_iGuildInventorySize];						//	�流E����E������[������EĿ��E
	cGuildStructureInfo	m_aGuildStructure[dMAX_GUILD_STRUCTURE_COUNT];				//	�流E��Ʈ����[�߰���]

//	CGuildObjectInfoOld	m_aGuildObject[dMAX_GUILD_OBJECT_COUNT];	//	[����݁E
//	CSellingAgentItem	m_aSellingAgentItem[dMAX_GUILD_SELLING_AGENT_ITEM_COUNT];	//	�ִ�E�ǸŴ�E�E������ ��E[����݁E
//	CGuildPollInfo		m_guildPoll;												//	�流EƁE[����݁E


// 	void				copyData(CGuildAdvanceInfoOld *_lpOldData);

	void				getSaveData_Column(CGuildSaveInfoForColumn * _lpData);
	void				getSaveData_Binary(CGuildSaveInfoForBinary * _lpData);
	void				getSaveData_Bank(CGuildSaveInfoBank * _lpData);
	void				getSaveData_Mark(CGuildSaveInfoMark * _lpData);
	void				getSaveData_RelShip(CGuildSaveInfoRelShip * _lpData);
	void				getSaveData_Skill(CGuildSaveInfoSkill * _lpData);
	void				getSaveData_Pet(CGuildSaveInfoPet * _lpData);
	void				getSaveData_Guardian(CGuildSaveInfoGuardian * _lpData);
	void				getSaveData_Inventory(CGuildSaveInfoInventory * _lpData);
	void				getSaveData_Structure(CGuildSaveInfoStructure * _lpData);

	void				setSaveData_Column(CGuildSaveInfoForColumn * _lpData);
	void				setSaveData_Binary(CGuildSaveInfoForBinary * _lpData);
	void				setSaveData_Bank(CGuildSaveInfoBank * _lpData);
	void				setSaveData_Mark(CGuildSaveInfoMark * _lpData);
	void				setSaveData_RelShip(CGuildSaveInfoRelShip * _lpData);
	void				setSaveData_Skill(CGuildSaveInfoSkill * _lpData);
	void				setSaveData_Pet(CGuildSaveInfoPet * _lpData);
	void				setSaveData_Guardian(CGuildSaveInfoGuardian * _lpData);
	void				setSaveData_Inventory(CGuildSaveInfoInventory * _lpData);
	void				setSaveData_Structure(CGuildSaveInfoStructure * _lpData);


	void				reset();

	int					getCheckSum();
	int					getInventoryItemCount();
	void				resetInventory();

	int					GetItemPlace(WORD _wBaseItem,DWORD _dwSerial);	//	������ ��ġ ����E
	BOOL				storeItem(int _iSlot,CItemDefine *_lpItem);	//	������ ����E	
	BOOL				removeItem(int _iSlot,CItemDefine *_lpItem);	//	������ ����
	BOOL				storeGold(int _iGold);	//	��E����E	
	BOOL				removeGold(int _iGold);	//	��E����
	BOOL				changeItemPlace(int _iSlot1,int _iSlot2);	//	������ ��ġ ����E
	BOOL				addRelatedPlace(int _iField,int _iPrice);	//	����݁E�߰�
	BOOL				removeRelatedPlace(int _iField);			//	����݁E����

	BOOL				isRelatedPlace(int _iField);	//	����݁E�ʵ��?
	int					getRelatedPlaceCount();			//	����݁E����E	
	int					getMaxRelatedPlaceCount();		//	����E��E�ִ� ����݁E����E

};

#define	dBATTLE_GUILD_COUNT_IN_FIELD		2
#define	dGUILD_BATTLE_COUNT					23
#define	dGUILD_BATTLE_COUNT_IN_SAME_TIME	10
#define	dGUILD_BATTLE_LAST_SERIAL_FIELD		999

const	int	c_iFirstFieldSerialForGuildBattle	=	966;
const	int	c_iLastFieldSerialForGuildBattle	=	985;


class	CGuildBattleInfoForClient
{
public:
	WORD	m_awBattleGuild[2];
	WORD	m_wFieldSerial;
	char	m_astrGuild[2][dGUILD_NAME_LENGTH];
};

class	CGuildBattleInfoForMember
{
public:
	BYTE	m_bIGB_Hour,m_bIGB_Minute;
	WORD	m_wIGB_FieldSerial;
	char	m_strIGB_TargetGuildName[dGUILD_NAME_LENGTH];

	BYTE	m_bBSW_Hour,m_bBSW_Minute;	//	������E�ð�
	WORD	m_bf1IsDefense	:	1;	//	���܁E̴�.
	char	m_strSW_TargetGuildName[dGUILD_NAME_LENGTH];	//	������EŸ�� �流E
	CGuildBattleInfoForMember();

	void	reset();
};

class	CGuildBattleInFieldInfoForClient
{
public:
	CGuildBattleInfoForClient	m_aBattleGuildInfo[dGUILD_BATTLE_COUNT_IN_SAME_TIME];
};

class	CGuildBattleInfo
{
public:
	WORD				m_awBattleGuild[dBATTLE_GUILD_COUNT_IN_FIELD];
	WORD				m_wFieldSerial;

	BOOL				isBattleGuild(int _iGuildSerial)
	{
		if	(getTeam(_iGuildSerial) ==	0xffff)
			return	FALSE;

		return	TRUE;
	}

	int					getTeam(int _iGuildSerial)
	{
		if	(_iGuildSerial	==	0xffff)
			return	0xffff;

		for (int i=0;i<dBATTLE_GUILD_COUNT_IN_FIELD;i++)
			if	(m_awBattleGuild[i]	==	_iGuildSerial)
				return	i;

		return	0xffff;
	}
};

class	CGuildBattleInField
{
public:
	CGuildBattleInfo			m_aBattleGuildInfo[dGUILD_BATTLE_COUNT_IN_SAME_TIME];

	void						reset()
	{
		memset(m_aBattleGuildInfo,0xff,sizeof(m_aBattleGuildInfo));
	}
	void						cancel()
	{
		for (int i=0;i<dGUILD_BATTLE_COUNT_IN_SAME_TIME;i++)
		{
			m_aBattleGuildInfo[i].m_awBattleGuild[0]	=	0xffff;
			m_aBattleGuildInfo[i].m_awBattleGuild[1]	=	0xffff;
		}
	}
};

class	CGuildBattleSchedule	//	�����E���ѷα� ����
{
public:
	WORD						m_wYear,m_wMonth,m_wDay;

	CGuildBattleInField			m_aBattleGuildSchedule[dGUILD_BATTLE_COUNT];

	void						setDate(int _iYear,int _iMonth,int _iDay)	//	��E��E��
	{
		m_wYear	=	_iYear;
		m_wMonth=	_iMonth;
		m_wDay	=	_iDay;
	}
	
	DWORD						getCheckSum()
	{
		DWORD	dwCheckSum	=	0;

		dwCheckSum	+=	m_wYear;
		dwCheckSum	+=	m_wMonth;
		dwCheckSum	+=	m_wDay;

		for (int i=0;i<dGUILD_BATTLE_COUNT;i++)
		{
			for (int j=0;j<dGUILD_BATTLE_COUNT_IN_SAME_TIME;j++)
			{
				dwCheckSum	+=	m_aBattleGuildSchedule[i].m_aBattleGuildInfo[j].m_awBattleGuild[0];
				dwCheckSum	+=	m_aBattleGuildSchedule[i].m_aBattleGuildInfo[j].m_awBattleGuild[1];
				dwCheckSum	+=	m_aBattleGuildSchedule[i].m_aBattleGuildInfo[j].m_wFieldSerial;
			}
		}

		return	dwCheckSum;
	}
};

//
//	Guild Battle Result
enum
{
	eGBR_WIN,
	eGBR_LOSE,
	eGBR_DRAW,
};

class	CGuildBattleResultInfo
{
public:
	WORD						m_wGuildSerial;
	int							m_iVictoryPoint;	//	�流E����
	WORD						m_wVictoryContinue,m_wVictoryCount,m_wDefeatCount,m_wDrawCount,m_wCancelCount;
	WORD						m_wResult;
	WORD						m_wIncreaseVictoryPoint;
	WORD						m_wFightAtWeek;
};

typedef struct
{
	WORD	wIndex;
	char	strGuildName[dGUILD_NAME_LENGTH];
	WORD	wLevel;
	WORD	wHallLevel;
	DWORD	dwExp;
	DWORD	dwGold;
	char	strNotice[dGUILD_NOTICE_LENGTH];
	WORD	wNoticeType;
	char	strMaster[dNAME_LENGTH];
	WORD	wElderCount;
	WORD	wMasterLevel;
	UTime	dwStartMasterDate;
	WORD	wIsGetExp;
	WORD	wRateGetExp;
	WORD	wDateChangeExp;
	WORD	wMaxMember;
	WORD	wMaxSubMaster;
	WORD	wMaxCongress;
	WORD	wVillage[dMAX_GUILD_VILLAGE_COUNT];
	DWORD	dwLastConnect;
	WORD	wBestowItemLevel;
	WORD	wSellingAgentItemLevel;
	WORD	wTargetGuild;
	WORD	wInvadeGuild;
	UTime	dwLastConnectTime;
	WORD	GuildPoint;
	DWORD	dwVicPoint;
	WORD	wVicContinue;
	WORD	wFightAtWeek;
	WORD	wFight[dMAX_FIGHT_ATWEEK];
	DWORD	dwWin;
	DWORD	dwLose;
	DWORD	dwDraw;
	DWORD	dwCancel;
	UTime	dwLastConnectedMasterTime;

	CGuildMarkInfo		m_guildMarkInfo;	//	�流E��ũ
	WORD				m_aGuildSkill[dMAX_GUILD_SKILL_COUNT];	//	�流E��ų
	CGuildPetInfo		m_aGuildPet[dMAX_GUILD_PET_COUNT];
	CGuildPetInfo		m_aGuildGuardian[dMAX_GUILD_GUARDIAN_COUNT];
	CGuildObjectInfoOld	m_aGuildObject[dMAX_GUILD_OBJECT_COUNT];
	CItemDefine			m_aInventory[c_iGuildInventorySize];						//	�流E����E������
	CSellingAgentItem	m_aSellingAgentItem[dMAX_GUILD_SELLING_AGENT_ITEM_COUNT];	//	�ִ�E�ǸŴ�E�E������ ��E	
	CGuildPollInfo		m_guildPoll;												//	�流EƁE
}
	GuildListDBOld;


	typedef struct
	{
		CGuildSaveInfoForColumn		dataColumn;
		CGuildSaveInfoForBinary		dataBinary;
		CGuildSaveInfoMark			dataMark;
		CGuildSaveInfoBank			dataBank;
		CGuildSaveInfoRelShip		dataRelShip;
		CGuildSaveInfoSkill			dataSkill;
		CGuildSaveInfoPet			dataPet;
		CGuildSaveInfoGuardian		dataGuardian;
		CGuildSaveInfoInventory		dataInventory;
		CGuildSaveInfoStructure		dataStructure;
	}
	GuildListDB;

#endif
