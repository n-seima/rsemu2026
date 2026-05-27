#ifndef _classPartyManager_H
#define	_classPartyManager_H

#include	"SFC.H"
#include	"definePacketData.H"
#include	"partyDefine.H"
#include	"cPACKET_WORLDSERVER.h"

class	CPartyMemberInfo
{
public:
	char	m_strName[dNAME_LENGTH];

	WORD	m_wCurrentField;						//	현재 소속 필드
	int		m_iZoneSerial;
	DWORD	m_dwNameHashCode,m_dwLeaveFieldTime;	//	필드 이동중이라고 해 놓고 2분간 필드 이동 완료 메시지 안오면 짤린것으로 간주한다.
	int		m_iDuelVP;

	DWORD	m_bf8Job				:	8;	//8	직업
	DWORD	m_bf10Level				:	10;	//18	레벨
	DWORD	m_bf8RemainHP			:	8;	//26	레벨
	DWORD	m_bf1IsDisconnected		:	1;	//27
	DWORD	:0;

	WORD	m_bf10InstanceField			:	10;		//10
	WORD	m_bf4InstanceFieldFloor		:	4;		//14
	WORD	:0;
	WORD	m_bf4SecretDungeonPlayCount	:	4;		//4
	WORD	m_bf3GuildDungeonPlayCount	:	3;		//7
	WORD	:0;

	void	copy(CPartyMemberInfo *_lpMember)
	{
		memcpy(this,_lpMember,sizeof(CPartyMemberInfo));
	}
	void	getSimpleInfo(CSimplePartyMemberInfo *_lpInfo)
	{
		_lpInfo->m_bf8RemainHP				=	m_bf8RemainHP;
		_lpInfo->m_bf10Level				=	m_bf10Level;
		_lpInfo->m_bf8Job					=	m_bf8Job;
		_lpInfo->m_wCurrentField			=	m_wCurrentField;
		_lpInfo->m_iZoneSerial				=	m_iZoneSerial;
		_lpInfo->m_bf10InstanceField		=	m_bf10InstanceField;
		_lpInfo->m_bf4InstanceFieldFloor	=	m_bf4InstanceFieldFloor;
		_lpInfo->m_bf1IsDisconnected		=	m_bf1IsDisconnected;
		_lpInfo->m_bf4SecretDungeonPlayCount=	m_bf4SecretDungeonPlayCount;
		_lpInfo->m_iDuelVP					=	m_iDuelVP;

		if	(m_dwLeaveFieldTime)	
			_lpInfo->m_bf1IsMoveField	=	TRUE;
		else
			_lpInfo->m_bf1IsMoveField	=	FALSE;

		strcpy(_lpInfo->m_strName,m_strName);
	}
};


class	CPartyInfo
{
public:
	WORD				m_wSerial;					//	파티의 고유 시리얼
	DWORD				m_dwUniqueSerial;
	DWORD				m_dwNameHashCode;
	WORD				m_wGateField;
	WORD				m_wMemberCount;
	char				m_strName[dPARTYNAME_LENGTH];

	DWORD				m_bf10GateArea			:	10;
	DWORD				m_bf3GoldShareMethod	:	3;
	DWORD				m_bf3ItemShareMethod	:	3;
	DWORD				m_bf3PartyObject		:	3;	//파티 목적
	DWORD				m_bf1IsOpenParty		:	1;	//멤버를 계속 모집한다.
	
	DWORD				m_bf3LastReceiveGoldMember	:	3;
	DWORD				m_bf3LastReceiveItemMember	:	3;

	DWORD				m_bf1IsUnionParty			:	1;

	WORD				m_wUnionPartyserial;
	

	CPartyMemberInfo	m_aMembers[dPARTY_MEMBER_COUNT];

						CPartyInfo()
						{
							reset();
						}

	inline void			setUnionParty()	{m_bf1IsUnionParty = TRUE;}
	inline void			releaseUnionParty()	{m_bf1IsUnionParty = FALSE;}

	void				reset();
	char*				getTooOldTimeMemberForWaitEnterField();
	DWORD				getCheckSum();
	void				create(int _iSerial,char *_lpstrPartyName,char *_lpstrLeaderName,int _iField,int _iLevel,int _iJob,int _iRemainHP,int _iMasterSPC,int _iZoneSerial,int _iInstanceField,int _iInstanceFieldFloor,int _iMasterDuelVP);
	int					joinMember(char *_lpstrName,int _iField,int _iLevel,int _iJob,int _iRemainHP,int _iSPC,int _iZoneSerial,int _iInstanceField,int _iInstanceFieldFloor,int _iMemberDuelVP);
	int					leaveMember(char *_lpstrName);
	int					changeLeader(char *_lpstrName);
	int					changeConnectStatus(char *_lpstrName);
	void				disconnectMember(char *_lpstrName);
	int					getConnectedMemberCount();
	int					leaveFieldMember(char *_lpstrName,int _iDestFieldSerial,int _iIFSerial,int _iIFFloor);
	int					enterFieldMember(char *_lpstrName,int _iDestFieldSerial,int _iZoneSerial,int _iIFSerial,int _iIFFloor);
	void				getSimpleInfo(CSimplePartyInfo *_lpInfo);
	void				getInfoForBCS(cPartyInfoForBCS *_lpInfo);
	void				changeName(char *_lpstrChangeName);
	int					updateMemberInfo(char *_lpstrName,int _iLevel,int _iPSC,int _iJob,int _iGDPC);
	void				disconnectField(int *_lpiFieldList,int _iFieldCount);
	CPartyMemberInfo*	getPartyMember(char *_lpstrName,DWORD _dwNameHashCode);
};

class	CPartyManager
{
public:
	static	BOOL		s_bIsExist;
	int					m_iCount,m_iRookie;
	DWORD				m_dwCreateSerial;
	DWORD				m_dwLastUpdateTime;
	CPartyInfo			m_aParty[dPARTY_COUNT];
	CRITICAL_SECTION	m_cs;

						CPartyManager();
						~CPartyManager();

	void				reset();
	void				update();
	void				updateAllPartyInfo(void *_lpServer);
	CPartyInfo*			getBySerial(int _iPartySerial);
	CPartyInfo*			getByName(char *_lpstrLeaderName);
	CPartyInfo*			getByUniqueSerial(DWORD _dwSerial);
	CPartyInfo*			getByPlayer(char *_lpstrName);
	int					checkDisconnectPlayer(char *_lpstrName);
	
	int					createParty(char *_lpstrPartyName,	char *_lpstrLeaderName,int _iField,int _iLevel,int _iJob,int _iRemainHP,int _iMasterSPC,int _iMasterZoneSerial,
															char *_lpstrMemberName,int _iMemberField,int _iMemberLevel,int _iMemberJob,int _iMemberRemainHP,int _iMemberSPC,int _iMemberZoneSerial,
															int _iMasterDuelVP,int _iTargetDuelVP);
	int					dissolutionParty(char *_lpstrPartyName);
	int					dissolutionParty(int _iPartyIndex);
	int					joinPartyMember(char *_lpstrPartyName,char *_lpstrName,int _iField,int _iLevel,int _iJob,int _iRemainHP,int _iSPC,int _iZoneSerial,int _iIFSerial,int _iIFFloor,int _iVP);
	int					operatePartyWork(int _iSerial,int _iWork,char *_lpstrName);
	int					changePartyName(char *_lpstrPartyName,char *_lpstrChangePartyName);
	int					updateMemberInfo(DWORD _dwSerial,char *_lpstrName,int _iLevel,int _iPSC,int _iJob,int _iGDPC);

	int					leaveField(char *_lpstrPartyName,char *_lpstrName,int _iDestFieldSerial,int _iIFSerial,int _iIFFloor);
	int					enterField(char *_lpstrPartyName,char *_lpstrName,int _iDestFieldSerial,int _iZoneSerial,int _iIFSerial,int _iIFFloor);
	int					getInfoForBCS(int _iSerial,cPartyInfoForBCS *_lpInfo);
	int					getSimpleInfoBySerial(int _iSerial,CSimplePartyInfo *_lpInfo);
	int					getSimpleInfoByName(char *_lpstrPartyName,CSimplePartyInfo *_lpInfo);
	int					getPartySerial(char *_lpstrPartyName);
	int					getPartySerial(DWORD _dwSerial);
	void				disconnectField(int *_lpiFieldList,int _iFieldCount);
	void				disconnectMember(char *_lpstrPartyName);
	void				removePartyMember(char *_lpstrPartyName);

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃partyPacket.cpp
	void				operateJoinPartyApplication(void *_lpClient,char *_lpPacketData);
	void				operateJoinPartyApplicationAnswer(void *_lpClient,char *_lpPacketData);

	void				operateMoveFieldPartyMemberPacket(void *_lpClient,char *_lpPacketData);
	void				operateCreatePartyPacket(void *_lpClient,char *_lpPacketData);
	void				operateJoinPartyPacket(void *_lpClient,char *_lpPacketData);
	void				operatePartyWorkPacket(void *_lpClient,char *_lpPacketData);
	void				operateRenamePartyPacket(void *_lpClient,char *_lpPacketData);
	void				operateChangePartyStatusPacket(void *_lpClient,char *_lpPacketData);
	void				operateAskPartyListPacket(void *_lpClient,char *_lpPacketData);
	void				operateAskPartyInfoPacket(void *_lpClient,char *_lpPacketData);
	void				operateClosePartyPacket(void *_lpClient,char *_lpPacketData);
	void				operateUpdatePartyMemberInfoPacket(void *_lpClient,char *_lpPacketData);
	void				operateCheckPartyInfoByActorPacket(void *_lpClient,char *_lpPacketData);
	void				operatePartyChatPacket(void *_lpClient,char *_lpPacketData);
	void				operateEnterIFPartyPacket(void *_lpClient,char *_lpPacketData);

	void				sendJoinPartyApplication(void *_lpClient,char *_lpstrAskerName,char *_lpstrLeaderName,int _iJob,int _iLevel);
	void				sendJoinPartyApplicationAnswer(void *_lpClient,char *_lpstrAskerName,char *_lpstrLeaderName,char *_lpstrPartyName,int	_iPartySerial,int _iAnswer);

	void				sendChatPacket(void *_lpClient,int _iPartySerial,char *_lpstrPartyName,char *_lpstrMessage,char *_lpstrMember);
	void				sendValidPartyPacket(void *_lpClient,char *_lpstrPartyName);
	void				sendValidPartyPacket(void *_lpClient,int _iPartySerial);
	void				sendUpdatePartyInfoPacket(void *_lpClient,char *_lpstrPartyName,BOOL _bIsCreate=FALSE);
	void				sendUpdatePartyInfoPacket(void *_lpClient,int _iIndex,BOOL _bIsCreate=FALSE);
	void				sendChangePartyStatusPacket(void *_lpClient,int _iIndex,BOOL _bIsChangeSetting);
	void				sendFailedToCreateParty(void *_lpClient,char *_lpstrAsker,int _iReason);
	void				sendJoinPartyResult(void *_lpClient,int _iResult,char *_lpstrPartyName,char *_lpstrActorName,int _iLevel,int _iJob,int _iRemainHP,int _iSPC,int _iFieldSerial,int _iZoneSerial,int _iIFSerial,int _iIFFloor,int _iDuelVP);
	void				sendPartyWorkPacket(void *_lpClient,DWORD _dwSerial,char *_lpstrMember,int _iWork);
	void				sendDisconnectPartyMemberPacket(void *_lpClient,char *_lpstrPartyName,int _iPartySerial,char *_lpstrMember);
	void				sendDissolutionPartyPacket(void *_lpClient,char *_lpstrPartyName);
	void				sendChangePartyNameResultPacket(void *_lpClient,int _iPartySerial,char *_lpstrPartyName,char *_lpstrChangePartyName,int _iResult);
	void				sendUpdateMemberInfoPacket(void *_lpClient,DWORD _dwSerial,char *_lpstrMember,int _iLevel,int _iPSC,int _iJob,int _iGPDC);

	void				sendUpdatePartyInfoToBCS(int _iIndex);
	void				sendAllPartyInfoToBCS();
	
	void				operateGuildMemberJoinParty(void *_lpClient,char *_lpPacketData);
	void				operateGuildMemberJoinPartyAnswer(void *_lpClient,char *_lpPacketData);
	void				sendGuildMemberJoinParty(void *_lpClient,char *_lpstrAskerName,char *_lpstrTargetName,int _iJob,int _iLevel);
	void				sendGuildMemberJoinPartyAnswer(void *_lpClient,char *_lpstrAskerName,char *_lpstrTargetName,int _iAnswer);
//┃partyPacket.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
};

extern	CPartyManager	g_partyManager;

//JBC	파티 연합 관련 사항 09-01-29
class cUnionParty
{
public:
	WORD				m_wSerial;
	DWORD				m_dwNameHashCode;
	char				m_strUnionPartyName[dPARTYNAME_LENGTH];//파티 이름
	CPartyInfo*			m_apMemberParty[dUNION_PARTY_COUNT];		//연합 파티를 구성하는 파티들의 포인터.
	WORD				m_wMemberPartyCount;						//연합 파티의 수.

	WORD				m_bf3UnionPartyObject		:	3		;		//파티 목적

	BOOL				CreateUnionParty(CPartyInfo* _AskerParty , CPartyInfo* _ReplayerParty);
	void				dissolutuionParty(int _iBecause);
	inline void			reset()
	{	
		m_bf3UnionPartyObject	=	ePO_HUNT;
		m_wSerial				=	0xffff;
		m_wMemberPartyCount		=	0;
		m_dwNameHashCode		=	0;

		ZeroMemory(&m_strUnionPartyName , sizeof(m_strUnionPartyName));

		for (int i =0  ; i < dUNION_PARTY_COUNT ; i++)
		{
			m_apMemberParty[i] = NULL;
		}
	}	

	inline	int			getMemberPartyCount()
	{
		for (int i =0  ; i < dUNION_PARTY_COUNT ; i++)
		{
			if(m_apMemberParty[i] == NULL)
				return i;
		}
		
		return NULL;
	}
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

	inline void			reset()
	{
		dwUniqueUnionSerialCount	=	0;
	}

	BOOL				operateCreateUnionPartyPacket(void *_lpClient,char *_lpPacketData);
	cUnionParty*		getFreeDenIndex(WORD* _wIndex);
	void				dissolutionUnionParty(WORD _wIndex , int _iBecause);
	BOOL				isCorrectUnionParty(int _iSerial);

};

extern	cUnionPartyManager		g_UnionpartyManager;

#endif	
