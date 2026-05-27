#ifndef	_userCustomDataDefine_h
#define	_userCustomDataDefine_h

#include "SFC.H"

#define	dNAME_LENGTH				18
#define	dFRIEND_GROUP_COUNT			10
#define	dFRIEND_GROUP_NAME_LENGTH	18
#define	dACTIVE_REACTION_COUNT		4

#define	dINCREASE_FRIEND_COUNT

#ifdef	dINCREASE_FRIEND_COUNT

#define	dMAX_FRIEND_COUNT			150
#define	dMAX_IS_ONLINE_FRIEND		24

#else

#define	dMAX_FRIEND_COUNT			50
#define	dMAX_IS_ONLINE_FRIEND		8

#endif

#pragma pack(2)



enum
{
	eBST_SKILL,
	eBST_ITEM,
	eBST_TITLE,
};

class	cQuickSlotSkillDefine
{
public:
	WORD	m_bf2Kind			:	2;
	WORD	m_bf14Skill			:	14;
};

class	cQuickSlotItemDefine
{
public:
	WORD	m_bf2Kind			:	2;
	WORD	m_bf7ItemSlot		:	7;
	WORD	m_bf10ItemCheckSum	:	8;
};

class	cQuickSlotTitleDefine
{
public:
	WORD	m_bf2Kind			:	2;
	WORD	m_bf14TitleIndex	:	14;
};

class	cFriendInfoDefine
{
public:
	WORD	m_bf1IsOnline		:	1;
	WORD	m_bf4Group			:	4;
	WORD	m_bf8Job			:	8;

	char	m_strName[dNAME_LENGTH];

	cFriendInfoDefine()
	{
		reset();
	}

	void	reset()
	{
		m_strName[0]	=	NULL;
	}
};


class	cFriendListOnlineInfoBase
{
	BYTE	m_abBuffer[dMAX_IS_ONLINE_FRIEND];

public:
					cFriendListOnlineInfoBase()
					{
						reset();
					}

	inline	void	reset()
	{
		memset(this,0,sizeof(cFriendListOnlineInfoBase));
	}

	inline	bool	isOnlineFriend(int _iIndex)
	{
		if	(m_abBuffer[_iIndex/8]&(1<<(_iIndex%8)))
			return	true;

		return	false;
	}

	inline	void	setOnlineStatus(int _iIndex)
	{
		int	iCheckValue	=	(1<<(_iIndex%8));

		m_abBuffer[_iIndex/8] |= iCheckValue;
	}
};


class	cUserCustomDataDefine
{
public:


	enum	eUSER_SKILL_SET
	{
		eUSS_LEFT_ABILITY,
		eUSS_SUB_ABILITY,
		eUSS_RIGHT_ABILITY,
		eUSS_SUPPORT_ABILITY,
		eUSS_QUICK_SKILL,
	};

	WORD				m_aLeftAbility[2],m_aSubAbility[2],m_aRightAbility[2],m_aSupportAbility[2];
	WORD				m_aQuickSkill[10][10][2];
	WORD				m_aActivePassiveSkill[2][dACTIVE_REACTION_COUNT];

	DWORD				m_bf1IsSaveAllDataByClient			:	1;
	DWORD				m_bf1IsChangeFriendListOnlineStatus	:	1;

	char				m_strGroupName[dFRIEND_GROUP_COUNT][dFRIEND_GROUP_NAME_LENGTH];
	cFriendInfoDefine	m_aFriendList[dMAX_FRIEND_COUNT];


	cUserCustomDataDefine()
	{
		reset();
	}

	void				copy(cUserCustomDataDefine *_lpData);
	BOOL				getUserCustomDataCRC32(DWORD &_dwResult);
	void				reset()
	{
		memset(this,0,sizeof(cUserCustomDataDefine));

		memset(m_aLeftAbility,0xff,sizeof(m_aLeftAbility));
		memset(m_aSubAbility,0xff,sizeof(m_aSubAbility));
		memset(m_aRightAbility,0xff,sizeof(m_aRightAbility));
		memset(m_aSupportAbility,0xff,sizeof(m_aSupportAbility));

		memset(m_aQuickSkill,0xff,sizeof(m_aQuickSkill));

		memset(m_aActivePassiveSkill,0xff,sizeof(m_aActivePassiveSkill));
	}

	void			changeSkillInfo(eUSER_SKILL_SET _info,int _iValue1,int _iValue2,int _iValue3=0,int _iValue4=0);

	void			addFriend(cFriendInfoDefine *_lpInfo);
	BOOL			removeFriend(char *_lpstrName);
	void			changeFriendGroupName(int _iGroup,char *_lpstrName);
	void			changeFriendGroup(int _iGroup,char *_lpstrName);
	int				getFriendCount();

	void			resetCurrentFriendListOnlineStatus();
	void			updateFriendOnlineStatus(int _iIndex,bool _bIsOnline);
	bool			isChangedFriendListOnlineStatus();
	void			getFriendListOnlineStatus(cFriendListOnlineInfoBase *_lpInfo);
};

#pragma pack()

#endif