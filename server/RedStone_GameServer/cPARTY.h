#ifndef _classPARTY_H
#define	_classPARTY_H

#include "SFC.H"
#include "definePacketData.h"
#include "cPACKET_WORLDSERVER.h"
#include "cItem.H"
#include "partyDefine.H"

#define	dMINIMUM_AVAIL_PARTY_RANGE				1600*1600
#define	dPARTY_MEMBER_MOVE_FIELD_STATUS_TIME	(1000*60)

class	cACTOR;

class	cPARTY_MEMBER
{
public:
	char		m_strName[dNAME_LENGTH];
	int			m_iZoneSerial;
	WORD		m_wFieldSerial;	//	필드 번호
	DWORD		m_dwNameHashCode;
	int			m_iDuelVP;

	DWORD		m_bf10InstanceField			:	10;
	DWORD		m_bf4InstanceFieldFloor		:	4;
	DWORD		m_bf3PlayGuildDungeonCount	:	3;

	DWORD		m_bf8RestHP					:	8;		//	남은 체력 100분률
	DWORD		m_bf1IsMoveField			:	1;
	DWORD		m_bf1IsDisconnected			:	1;
	DWORD		m_bf8Job					:	8;
	DWORD		m_bf10Level					:	10;
	DWORD		m_bf4PlaySecretDungeonCount	:	4;

				cPARTY_MEMBER()
				{
					reset();
				}

	void		reset()
	{
		m_wFieldSerial		=	0xffff;
		m_bf8RestHP			=	100;
		m_strName[0]		=	NULL;
		m_bf1IsMoveField	=	FALSE;
		m_bf1IsDisconnected	=	FALSE;
	}

	void		copy(CSimplePartyMemberInfo *_lpMember)
	{
		m_dwNameHashCode			=	GetHashCode(_lpMember->m_strName);
		strcpy(m_strName,_lpMember->m_strName);
		m_wFieldSerial				=	_lpMember->m_wCurrentField;
		m_bf8Job					=	_lpMember->m_bf8Job;
		m_bf10Level					=	_lpMember->m_bf10Level;
		m_bf8RestHP					=	_lpMember->m_bf8RemainHP;
		m_bf1IsMoveField			=	_lpMember->m_bf1IsMoveField;
		m_iZoneSerial				=	_lpMember->m_iZoneSerial;
		m_bf1IsDisconnected			=	_lpMember->m_bf1IsDisconnected;
		m_bf10InstanceField			=	_lpMember->m_bf10InstanceField;
		m_bf4InstanceFieldFloor		=	_lpMember->m_bf4InstanceFieldFloor;
		m_bf4PlaySecretDungeonCount	=	_lpMember->m_bf4SecretDungeonPlayCount;
		m_iDuelVP					=	_lpMember->m_iDuelVP;
		m_bf3PlayGuildDungeonCount	=	_lpMember->m_bf3GuildDungeonPlayTime;
	}

	BOOL		isInSameField(int _iField,int _iIF,int _iIFFloor);
	DWORD		getInfoForUnionPartyState();
};

class					cUnionParty;

class cParty
{
public:

	DWORD				m_dwUniqueSerial;
	WORD				m_wSerial;//파티 시리얼
	DWORD				m_dwNameHashCode,m_dwCheckSum;
	char				m_strPartyName[dPARTYNAME_LENGTH];//파티 이름
	cPARTY_MEMBER		m_aMember[dPARTY_MAXPLAYER];//멤버들
	WORD				m_wMemberCount;
	WORD				m_wGateField;
	WORD				m_wLeaderPlayMinuteOnThisFieldTime;

	DWORD				m_bf10GateArea				:	10;
	DWORD				m_bf3GoldShareMethod		:	3;
	DWORD				m_bf3ItemShareMethod		:	3;
	DWORD				m_bf3PartyObject			:	3;	//파티 목적
	DWORD				m_bf1IsOpenParty			:	1;	//멤버를 계속 모집한다.

	DWORD				m_bf3LastReceiveGoldMember	:	3;
	DWORD				m_bf3LastReceiveItemMember	:	3;		//26

	DWORD				m_bf1IsUnionParty			:	1;	//연합 파티 이냐?

	WORD				m_wUnionPartyserial;			//연합 파티 시리얼			//ex>몇번째 연합 파티냐를 표현.
	WORD				m_wUnionPartyIndex;				//소속된 연합 파티 인덱스	//ex>연합 파티의 몇번째 파티냐를 표현한다.

	DWORD				m_dwLastTimeToDropItem;		

						cParty()
						{
							m_bf1IsUnionParty = FALSE;
							reset();
						}

	inline	void			setUnionMember()	{m_bf1IsUnionParty = TRUE;}
	inline	void			releaseUnionParty()	{m_bf1IsUnionParty = FALSE;}
	//inline	cUnionParty*	getUnionParty(WORD _wSerial)	{return g_UnionpartyManager.m_den.notCheckedget(_wSerial);}

	inline	int			getMemberCount()
	{
		return	m_wMemberCount;
	}
	inline	BOOL		isFullParty()
	{
		if (m_wMemberCount	>=	dPARTY_MAXPLAYER)
			return	TRUE;		

		return	FALSE;
	}	//	풀파티-_-?

	inline	BOOL		isLeader(char *_lpstrName)	
	{
		if	(STRICMP(m_aMember[0].m_strName,_lpstrName)==0) 
			return	TRUE;
		return FALSE;
	}

	inline	void		reset()	//	초기화
	{
		m_wSerial							=	0xffff;
		m_wMemberCount						=	0;
		m_dwLastTimeToDropItem				=	0;

		m_bf3PartyObject					=	ePO_HUNT;
		m_bf3GoldShareMethod				=	ePGSM_FREE;
		m_bf3ItemShareMethod				=	ePISM_FREE;
		m_bf1IsOpenParty					=	TRUE;//멤버를 계속 모집한다.
		m_bf3PartyObject					=	ePO_HUNT;
		m_wLeaderPlayMinuteOnThisFieldTime	=	0;
		m_wUnionPartyserial					=	0xffff;
		m_wUnionPartyIndex					=	0xffff;
	}

	inline	int			getTopLevel()
	{
		int	iTopLevel	=	0;

		for (int i=0;i<(int)m_wMemberCount;i++)
			if	(m_aMember[i].m_bf10Level > (DWORD)iTopLevel)
				iTopLevel	=	m_aMember[i].m_bf10Level;

		return	iTopLevel;
	}

	BOOL				checkMemberLevel(int _iMinLevel,int _iMaxLevel);
	BOOL				checkSecretDungeonLimitLevel(int _iLevel);
	BOOL				isOnField(int _iMember,int _iFieldSerial,int _iGateField=0xffff);
	DWORD				getCheckSum();
	int					getAbsoluteGoodMemberCount(int _iFieldSerial=0xffff,cPOINT *_lpPos=NULL);	//	절대선 성향의 멤버들 수 구함
	int					getGoodMemberCount(int _iFieldSerial=0xffff,BOOL _isIncludeAbsoluteGood=FALSE,cPOINT *_lpPos=NULL);	//	선 성향의 멤버들 수 구함
	int					getNeutralMemberCount(int _iFieldSerial=0xffff,cPOINT *_lpPos=NULL);	//	중립 성향의 멤버들 수 구함
	int					getEvilMemberCount(int _iFieldSerial=0xffff,BOOL _isIncludeAbsoluteEvil=FALSE,cPOINT *_lpPos=NULL);	//	악 성향의 멤버들 수 구함
	int					getAbsoluteEvilMemberCount(int _iFieldSerial=0xffff,cPOINT *_lpPos=NULL);	//	절대악 성향의 멤버들 수 구함
	cACTOR *			getActorByMemberIndex(int _iMember);//	_iMember 리턴~
	cACTOR *			getActorByMemberNameHashCode(DWORD _dwNameHashCode);//	NameHashCode로 맴버리턴~
	cACTOR *			getThisTimePicker(int _iField,cPOINT *_lpPos,cACTOR *_lpPicker,cItem	*_lpPickItem);
	void				changeLastItemPickMember(char *_lpstrName);
	void				sharingGold(int _iField,cPOINT *_lpPos,cACTOR *_lpPicker,cItem	*_lpPickItem);
	cPARTY_MEMBER*		getMemberInfoByName(char *_lpstrName);//	멤버 정보 리턴~
	cPARTY_MEMBER*		getMemberInfoByName(char *_lpstrName,DWORD _dwNameHashcode);//	멤버 정보 리턴~
	cPARTY_MEMBER*		getMemberInfoByIndex(int _iMember);//	멤버 정보 리턴~
	int					getInrangeMember(int _iField,cPOINT *_lpPos,WORD *_lpwMemberList,cItem *_lpCheckJobItem=NULL);
	cACTOR *			getRandomInrangeMember(cACTOR *_lpPicker);
	cItem*				getItemByBasicItem(int _iBaseItem);

	BOOL				isEnableToTryDuelSatus();	//	결투 신청이 가능한 상태냐?

	int					getMemberCountForGiveExp(int _iField,cPOINT *_lpPos=NULL,int *_lpiBonusExp=NULL);//특정 필드 안에 멤버의 수
	int					getMemberCountForMovePacket(int _iField);//특정 필드 안에 멤버의 수
	int					getMaxBonusExp();
	int					getInrangeMemberLevelSum(int _iField,cPOINT *_lpPos=NULL);//특정 필드 안에 멤버의 수
	BOOL				checkMember();
	void				sendPickItemMessage(cACTOR *_lpPicker,cItem *_lpItem,BOOL _bIsSharingGold=FALSE);
	void				sendEnterToTherSecretDungeonMessage(int _iField,int _iArea);
	int					getSecretDungeonScore();
	void				sendPacketToAllMember(void *_lpPacket,int _iSize);
	void				sendPacketToSameFieldMember(cACTOR *_lpSender,void *_lpPacket,int _iSize,int _iRange=-1);

	inline	void		copy(cParty *_lpPartyInfo){memcpy(this,_lpPartyInfo,sizeof(cParty));}//복 사 ㅡㅁㅡ
	BOOL				addMember(cPARTY_MEMBER *_lpNewMember);//멤버추가
	BOOL				joinPartyMember(CSimplePartyMemberInfo *_lpMember);//멤버추가
	BOOL				checkData();	//	데이터 체크

	inline	void		init(int _iSerial,DWORD _dwUniqueSerial,char *_lpstrPartyName)//파티 초기화
	{
		reset();

		m_wSerial		=	_iSerial;
		m_dwUniqueSerial=	_dwUniqueSerial;
		strcpy(m_strPartyName,_lpstrPartyName);

		m_dwNameHashCode=	GetHashCode((BYTE *)_lpstrPartyName);
	}

	inline	int			getMemberIndex(char *_strMemberName)//멤버 인덱스 얻어오기
	{
		for (int i=0;i<(int)m_wMemberCount;i++)
			if (STRCMP(_strMemberName,m_aMember[i].m_strName)==0) 
				return i;

		return	0xffff;
	}

	BOOL				isNotJJUL(cACTOR *_lpChecker);
	BOOL				isGoodParty(int _iLevelRange,cACTOR *_lpTarget,cACTOR *_lpChecker);

	int					getAverageLevel(int _iField=0xffff,cPOINT	*_lpPos=NULL);
	BOOL				changeLeader(char *_strMemberName);
	BOOL				changeMemberConnectStatus(char *_strMemberName,BOOL _bIsDisconnect);
	BOOL				removeMember(char *_strMemberName);	//멤버 제거
	BOOL				disconnectMember(char *_strMemberName);	//멤버 제거
	void				disconnectField(int *_lpiFieldList,int _iFieldCount);
	inline	BOOL		isOpenParty()	//	멤버를 계속 모집하고 있는 파티냐?
	{
		if (m_wMemberCount	>=	dPARTY_MAXPLAYER	)
			return	FALSE;

		return	m_bf1IsOpenParty;
	}

	void				syncMemberStatus(cACTOR *_lpActor);		//	멤버들의 상태 싱크 시킴
	void				syncMemberInfo(char *_lpstrName);
	void				syncMemberHP(cACTOR *_lpActor);	
	void				operateOncePerSecond();
	int					getCountOfMemberOwnedItem(WORD _wSerial, BOOL _bIsIncludEquippedItem);
	int					getItemCountInParty(WORD _wSerial, BOOL _bIsIncludEquippedItem);
};

class cPartyManager
{
	BOOL				makeParty(int _iPartyIndex,DWORD _dwUniqueSerial,char *_lpstrPartyName);//파티 만들기

public:
	CRITICAL_SECTION	m_csPartyWork;

	cDen<cParty>		m_den;

						cPartyManager();
						~cPartyManager();

	void				init(int _iCount);
	void				close();

	int					checkDisconnectPlayer(cACTOR *_lpActor,BOOL _bIsWantPartySerial);
	BOOL				pickPartyItem(int _iPartyIndex,int _iPickMember,int _iFieldSerial,int _iX,int _iY,cItem *_lpItem);	//	아이템을 줏었다.

	//	아래 두 함수는 반드시 블럭킹이 된 상태에서만 사용한다.
	inline	cParty*		get(int _iPartyIndex) {return m_den.get(_iPartyIndex);}	//	블럭킹이 되어 있다는 전제하에서 사용
	cParty*				get(int _iPartyIndex,char *_lpstrName);
	cParty*				get(char *_lpstrName);
	cParty*				getByUniqueSerial(int _iPartyIndex,DWORD _dwUniqueSerial);
	
	void				setMemberZoneSerial(int _iParty,char *_lpstrName,int _iZoneSerial);
	int					getCount();	//총파티의 수
	BOOL				getParty(cParty *_lpParty,int _iPartyIndex);	//	_lpParty에 담아준다.
	void				updatePartyInfo(WORLDtoSERVERS_UPDATE_PARTYINFO* _lpPacket);
	void				dissolutionParty(int _iPartySerial);	//	파티 해체
	void				dissolutionParty(char *_lpstrPartyName);	//	파티 해체
	BOOL				joinPartyMember(int _iPartyIndex,char *_lpstrPartyName,CSimplePartyMemberInfo *_lpMember);//멤버 추가
	BOOL				operatePartyWork(int _iPartyIndex,DWORD _dwUniqueSerial,int _iWork,char *_strName);//멤버 제거
	BOOL				disconnectMember(int _iPartyIndex,char *_lpstrPartyName,char *_strName);//멤버 제거
	void				changePartyName(int _iPartyIndex,char *_lpstrPartyName,char *_strChangeName);
	void				changePartyStatus(int _iPartyIndex,DWORD _dwUniqueSerial,int _iGoldShareMethod,int _iItemShareMethod,int _iPartyObject,BOOL _bIsOpenParty,
		int _iLastReceiveGoldMember,int _iLastReceiveItemMember,BOOL _bIsChangeSetting);
	BOOL				sendWPChangeMemberInfo(int _iPartyIndex,char *_lpstrName,int _iLevel,int _iPSC,int _iJob,int _iGDPC = 0);	//	멤버 레벨 업
	void				changeMemberInfo(int _iPartyIndex,DWORD _dwUniqueSerial,char *_lpstrMemberName,int _iLevel,int _iPSC,int _iJob, int _iGDPC = 0);	//	멤버 레벨 업데이트
	void				moveFieldMember(int _iSerial,char *_lpstrPartyName,char *_lpstrName,int _iDestField,BOOL _bIsLeave,int _iZoneSerial,int _iIFSerial,int _iIFFloor);
	void				disconnectField(int *_lpiFieldList,int _iFieldCount);
	void				operateOncePerSecond();
	
	BOOL				isNotJJUL(cACTOR *_lpChecker);
	BOOL				isGoodParty(cACTOR *_lpTarget,cACTOR *_lpChecker);


};

extern	cPartyManager*			g_pPartyManager;


class cUnionParty
{
public:
	DWORD				m_dwUniqueSerial;
	WORD				m_wSerial;
	DWORD				m_dwNameHashCode,m_dwCheckSum;
	char				m_strUnionPartyName[dPARTYNAME_LENGTH];//파티 이름
	cParty*				m_apMemberParty[dUNION_PARTY_COUNT];		//연합 파티를 구성하는 파티들의 포인터.
	WORD				m_wMemberPartyCount;						//연합 파티의 수.

	WORD				m_bf3UnionPartyObject		:	3		;		//파티 목적
	cUnionParty()
	{
		reset();
	}

	void				syncUnionPartyMemberHP(cACTOR* _lpActor);
	void				dissolutionUnionParty(WORD _wBecause);
	BOOL				CreateUnionParty(cParty* _AskerParty , cParty* _ReplayerParty);
	
	inline	int			getMemberPartyCount()
	{
		return	m_wMemberPartyCount;
	}
	inline	BOOL		isFullUnionParty()
	{
		if (m_wMemberPartyCount	>=	dUNION_PARTY_COUNT)
			return	TRUE;		
		
		return	FALSE;
	}	//	풀연합파티-_-?
	
	inline	BOOL		isUnionLeader(char *_lpstrName)	
	{
		if	(STRICMP(m_apMemberParty[0]->m_aMember[0].m_strName,_lpstrName)==0) 
			return	TRUE;
		return FALSE;
	}
	
	inline	void		reset()	//	초기화
	{
		m_wSerial							=	0xffff;
		m_wMemberPartyCount					=	0;
		
		m_bf3UnionPartyObject				=	ePO_HUNT;

		for (int i = 0 ; i < dUNION_PARTY_COUNT ; i++)
			m_apMemberParty[i] = NULL;
	}

	inline	void		init(int _iSerial,DWORD _dwUniqueSerial,char *_lpstrPartyName)//파티 초기화
	{
		reset();
		
		m_wSerial		=	_iSerial;
		m_dwUniqueSerial=	_dwUniqueSerial;
		strcpy(m_strUnionPartyName,_lpstrPartyName);
		
		m_dwNameHashCode=	GetHashCode((BYTE *)_lpstrPartyName);
	}

	inline	void		copy(cUnionParty *_lpUnionPartyInfo){memcpy(this,_lpUnionPartyInfo,sizeof(cUnionParty));}//복 사 ㅡㅁㅡ
	inline	int			getFreeIndex()
	{
		for (int i = 0 ; i < dUNION_PARTY_COUNT ; i++)
		{
			if (m_apMemberParty[i] == NULL)
				return i;
		}
		return NULL;
	}
	inline	int			insertParty(cParty*	lpParty)
	{
		int iFreeIndex = getFreeIndex();

		m_apMemberParty[iFreeIndex] = lpParty;

		return iFreeIndex;
	}
	inline	int			getPartyCount()
	{
		for (int i = 0 ; i < dUNION_PARTY_COUNT; i++)
		{
			if(m_apMemberParty[i] == NULL)
				return i;
		}
		return 0;
	}
	inline	cParty*		getPartyByIndex(int _iInsert)
	{
		return m_apMemberParty[_iInsert];
	}
	BOOL				isCorrectUnionParty();
};


class cUnionPartyManager
{
	BOOL				makeUnionParty(DWORD _dwUniqueSerial , char* _strName ,WORD _wIndex, WORD* _pwPartySerialList = NULL);// 유니온 파티 구성.
	
public:
	DWORD				dwUniqueUnionSerialCount;
	CRITICAL_SECTION	m_csUnionPartyWork;
	
	cDen<cUnionParty>	m_den;
	
	cUnionPartyManager();
	~cUnionPartyManager();

	inline	void				init(int _iCount)
	{
		m_den.init(_iCount);
	}
	void				close();

	//	아래 두 함수는 반드시 블럭킹이 된 상태에서만 사용한다.
	inline cUnionParty*	get(int _iUnionPartyIndex) {return m_den.get(_iUnionPartyIndex);}	//	블럭킹이 되어 있다는 전제하에서 사용
	cUnionParty*		get(int _iUnionPartyIndex,char *_lpstrName);
	cUnionParty*		get(char *_lpstrName);
	cUnionParty*		getFreeDenIndex(WORD* wIndex);
	cUnionParty*		getDenIndex(WORD _wIndex);
	cUnionParty*		getByUniqueSerial(int _iUnionPartyIndex,DWORD _dwUniqueSerial);
	cUnionParty*		getByName(char *_lpstrName);
	
	int					getCount();	//총파티의 수
	void				updateUnionPartyInfo(WORLDtoSERVERS_UPDATE_UNIONPARTYINFO* _lpPacket);
	void				dissolutionUnionParty(int _iPartySerial , WORD _wBecause);	//	파티 해체
	BOOL				joinUnionPartyMember(int _iUnionPartyIndex,cParty *_lpMember);
	BOOL				CreateUnionPartyWork(cACTOR* _lpReplayer , cACTOR* _lpAsker);
	BOOL				sendUnionPartyEtcInfo(WORD _wWork ,cParty* _lpParty , WORD _Value0 = 0, WORD _Value1 = 0, WORD _Value2 = 0, WORD _Value3 = 0, 
									   WORD _Value4 = 0, WORD _Value5 = 0, BOOL _sendAllMember = TRUE);
	BOOL				sendUnionPartyMemberInfoToAllUnionParty(int _iUnionPartyIndex , int _iUnionPartyMemberIndex , int _iSendPartyMemberIndex);
	BOOL				sendUnionPartyMemberInfo(int _iUnionPartyIndex , int _iUnionPartyMemberIndex , int _iSendPartyMemberIndex , int _iClientSerial);
	BOOL				sendUnionPartyInfo(int _iUnionPartyIndex , int _iUnionPartyMemberIndex ,int _iEffectivenessCount, int _iTargetClientSerial);
	void				CreateUnionPartyResultWork(WORD _wUnionPartySerial , cParty* _lpAskerParty , cParty* _lpReplayerParty);
};

extern	cUnionPartyManager*		g_pUnionpartyManager;

#endif
