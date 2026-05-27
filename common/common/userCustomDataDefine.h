#ifndef	_userCustomDataDefine_h
#define	_userCustomDataDefine_h

#include "SFC.H"

#define	dNAME_LENGTH				18
#define	dFRIEND_GROUP_COUNT			10
#define	dFRIEND_GROUP_NAME_LENGTH	18
#define	dACTIVE_REACTION_COUNT		4

#define	dINCREASE_FRIEND_COUNT		// 친구목록 증가할 경우 주석을 푼다.

#ifdef	dINCREASE_FRIEND_COUNT

#define	dMAX_FRIEND_COUNT			150
#define	dMAX_IS_ONLINE_FRIEND		24

#else

#define	dMAX_FRIEND_COUNT			50
#define	dMAX_IS_ONLINE_FRIEND		8

#endif

#pragma pack(2)

//
//	Bottom Slot Type
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
	WORD	m_bf1IsOnline		:	1;	//	온라인이냐?
	WORD	m_bf4Group			:	4;	//	친구 그룹
	WORD	m_bf8Job			:	8;	//	직업

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
	BYTE	m_abBuffer[dMAX_IS_ONLINE_FRIEND];	//	dMAX_FRIEND_COUNT+1 (4바이트 정렬을 위해 1바이트 추가)

public:
					cFriendListOnlineInfoBase()
					{
						reset();
					}

	inline	void	reset()	//	초기화
	{
		memset(this,0,sizeof(cFriendListOnlineInfoBase));
	}

	inline	bool	isOnlineFriend(int _iIndex)	//	_iIndex번째 친구녀석은 현재 온라인이냐?
	{
		if	(m_abBuffer[_iIndex/8]&(1<<(_iIndex%8)))
			return	true;

		return	false;
	}

	inline	void	setOnlineStatus(int _iIndex)	//	온라인 상태로 설정
	{
		int	iCheckValue	=	(1<<(_iIndex%8));

		m_abBuffer[_iIndex/8] |= iCheckValue;
	}
};


class	cUserCustomDataDefine
{
public:
	//
	//	
	enum	eUSER_SKILL_SET
	{
		eUSS_LEFT_ABILITY,
		eUSS_SUB_ABILITY,
		eUSS_RIGHT_ABILITY,
		eUSS_SUPPORT_ABILITY,
		eUSS_QUICK_SKILL,
	};

	WORD				m_aLeftAbility[2],m_aSubAbility[2],m_aRightAbility[2],m_aSupportAbility[2];//선택된 어빌러티
	WORD				m_aQuickSkill[10][10][2];
	WORD				m_aActivePassiveSkill[2][dACTIVE_REACTION_COUNT];

	DWORD				m_bf1IsSaveAllDataByClient			:	1;	//	클라이언트에 의해 모든 정보가 한번 업데이트 됐다.
	DWORD				m_bf1IsChangeFriendListOnlineStatus	:	1;	//	친구 리스트의 온라인 상태가 갱신됐다.(변동이 있었다.)

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

	void			resetCurrentFriendListOnlineStatus();	//	현재 친구들의 온라인 상태 전부 오프라인으로 초기화
	void			updateFriendOnlineStatus(int _iIndex,bool _bIsOnline);	//	현재 친구들의 온라인 상태 변경(이전과 변동이 있다면 유저한테 전송한다.)
	bool			isChangedFriendListOnlineStatus();	//	친구들 온라인 상태에 변화가 있냐?
	void			getFriendListOnlineStatus(cFriendListOnlineInfoBase *_lpInfo);
};

#pragma pack()

#endif