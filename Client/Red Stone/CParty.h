#ifndef _classPARTY_H
#define _classPARTY_H

#include "cNEMO.H"
#include "definepacketdata.h"
#include "partyDefine.h"
#include "cPACKET_GAMESERVER.h"
//#include "cACTOR.h"

#define	dMINIMUM_PARTY_NAME_LENGTH	4

//
//	파티 멤버 정보
class	CPartyMemberInfo
{
public:
	char						m_strName[dNAME_LENGTH];//	이름
	WORD						m_wJob;					//	직업
	WORD						m_wLevel;				//	레벨
	WORD						m_wSerial;				//	필드에서의 시리얼(같은 필드가 아닐때는 0xffff)
	WORD						m_wNameBarSizeAtField;
	WORD						m_wHP;					//	남은 HP
	WORD						m_wStatusField;			//	상태 필드
	CPos						m_pos;					//	위치
	WORD						m_wFieldSerial;			//	현재 필드 시리얼
	WORD						m_wPlaySecretDungeonCount;

	WORD						m_wInstanceField,m_wInstanceFieldFloor;
	WORD						m_wIsDisconnected;
	int							m_iDuelVP;
};

//JBC		파티 연합 관련 09-02-02
class	CUnionPartyInfo
{
public:
	BOOL							m_bIsActiveUnionParty;
	WORD							m_wUnionPartyCount;
	WORD							m_wFocusParty;
	WORD							m_wFocusPartyMember;
	//UnionPartyManager와 유사한 역활을 한다.
	CUnionPartyMemberInfo			m_wUnionPartyInfo[dUNION_PARTY_COUNT][dPARTY_MAXPLAYER];

	WORD							m_waWantupdateActor[256];	//업데이트 해야하는 유저의 리스트.
	
	inline CUnionPartyMemberInfo*	getUnionPartyInfo(int _iPartyIndex , int _iPlayerIndex)
	{
		return	&m_wUnionPartyInfo[_iPartyIndex][_iPlayerIndex];
	}

	int							isUnionPartyMember(WORD wIndex);

	inline void					ResetUnionParty()
	{
		for (int i =0 ; i < dUNION_PARTY_COUNT ;i++)
		{
			for (int j = 0 ; j < dPARTY_MAXPLAYER ; j++)
			{
				m_wUnionPartyInfo[i][j].m_wUnionPartySerial = 0xffff;
				m_wUnionPartyInfo[i][j].m_wHP	= 0;
				m_wUnionPartyInfo[i][j].m_dwState	= 0;
				ZeroMemory(m_wUnionPartyInfo[i][j].m_strName , dNAME_LENGTH * sizeof(char));
			}
		}
		
	}

	inline	void	reset()
	{
		m_bIsActiveUnionParty	=	FALSE;
		m_wUnionPartyCount		=	0;
		m_wFocusParty			=	0xffff;
		m_wFocusPartyMember		=	0xffff;
//		m_wPointWantupdateActor =	0;

		for(int i =0  ; i < 256 ; i++)
			m_waWantupdateActor[i] = 0xffff;
		
		ResetUnionParty();
	}
	
	CUnionPartyInfo::CUnionPartyInfo()
	{
		reset();
	}

	void				getByName(char* _strName , WORD* _wpPartyindex , WORD* _wpPlayerindex);
	void				updateActorList();
	inline	int			getFreeActorListIndex()
	{
		for (int i = 0 ; i < 256 ; i++)	
		{
			if(m_waWantupdateActor[i] == 0xffff)
				return i;
		}
		
		return 0xffffffff;		//return FALSE;
	}

	inline	int			getupdateActorListIndexWantupdate()
	{
		for (int i = 0 ; i < 256 ; i++)	
		{
			if(m_waWantupdateActor[i] != 0xffff)
				return i;

			if(m_waWantupdateActor[i] == 0xffff)
				return 0xffffffff;
		}

		return 0xffffffff;		//return FALSE;
	}

	inline int			getPlaceInupdateActorList(WORD _wIndex)
	{
		for (int i = 0 ; i < 256 ; i++)	
		{
			if(m_waWantupdateActor[i] == _wIndex)
				return i;

			if(m_waWantupdateActor[i] == 0xffff)
				return 0xffffffff;
		}

		return 0xffffffff;		//return FALSE;
	}

};

extern CUnionPartyInfo		s_unionPartyInfo;

//
//	파티 정보
class	CPatyInfo
{
public:
	char						m_strName[dPARTYNAME_LENGTH];	//	파티 이름
	WORD						m_wSerial;
	WORD						m_wFocusMember,m_wFocusPet,m_wFocusSummonBeast;
	WORD						m_wMemberCount;					//	멤버 수
	WORD						m_wSelectMember;				//	선택한 넘
	CPartyMemberInfo			m_aMemberList[dPARTY_MAXPLAYER];//	파티원들의 기본 정보 - 첫번째는 무조건 리더
	BOOL						m_bIsExistPreviousField;
	WORD						m_wIsWaitPartyInfo;
	WORD						m_wGateField;

	WORD						m_wRecruitMemberRange;
	DWORD						m_dwPreferenceJobMask;		// 수정..

	char						m_strPartyProspectus[c_iPartyProspectusLength];

	DWORD						m_bf10GateArea			:	10;
	DWORD						m_bf3GoldShareMethod	:	3;
	DWORD						m_bf3ItemShareMethod	:	3;
	DWORD						m_bf3PartyObject		:	3;	//파티 목적
	DWORD						m_bf1IsOpenParty		:	1;	//멤버를 계속 모집한다.
	DWORD						m_bf1IsUnionParty		:	1;	//파티 연합 중이냐?

	
	void						updatePartyProspectus(int iRange,DWORD _dwMask);		// 수정.
	void						changePartyProspectusText(char *_lpstrText);

	int							getAverageLevel();
	void						addMember(int _iIndex,char *_lpstrName,int _iJob,int _iLevel,int _iSerial,
											int _iFieldSerial,int _iIF,int _iIFFloor,int _iSDPC,int _iDuelVP);
	void						removeMember(int _iIndex);
	void						changeLeader(int _iIndex);
	void						changeConnectStatus(int _iIndex,BOOL _bIsDisconnect);
	int							getFocusMember();	//	포커스가 간 멤버 찾아서 리턴(필드 안에 시리얼)
	int							getMemberIndex(char *_lpstrName);	//	_lpstrName이름을 갖는 멤버 인덱스 구하기
	void						setMemberInfo(int _iIndex,char *_lpstrName,int _iJob,int _iLevel,int _iSerial,int _iFieldSerial,BOOL _bIsFirst,int _iIF,int _iIFFloor,BOOL _bIsDisconnected,int _iSDPC,int _iDuelVP);	//	멤버 정보 설정
	BOOL						checkDisconnectLeader();

	inline	BOOL				isFullParty()	{if (m_wMemberCount	==	dPARTY_MAXPLAYER) return TRUE;return FALSE;}	//	풀파티냐?
	inline	CPartyMemberInfo*	getMember(int _iIndex){return	&m_aMemberList[_iIndex];}
	inline	int					getMemberCount()	{return	m_wMemberCount;}
	CPartyMemberInfo*			getMemberByActorSerial(int _iSerial);	//	액터 시리얼로 멤버를 구한다.
	CPartyMemberInfo*			getMemberByName(char *_lpstrName);	//	이름으로  멤버를 구한다.
	BOOL						isIncorrectData();	//	파티 정보에 문제가 있다.

	void						sendChangPreferenceJobMask(DWORD _dwPreferenceJobMask);		// 수정
	void						sendChangRangeOfRecruitPartyMemberLevel(int _iLevelRange);
	void						sendChangPartyProspectusText(char *_lpText);
	void						sendAskPartyProspectus();


	void						reset();

	inline	char*				getSelectMemberName()
	{
		if (m_wSelectMember	==	0xffff)
			return	NULL;

		return	m_aMemberList[m_wSelectMember].m_strName;
	}

	inline	BOOL				isPartyMember(int _iSerial)
	{
		for (int i=0;i<m_wMemberCount;i++)
			if (m_aMemberList[i].m_wSerial	==	_iSerial)	
				return	TRUE;

		return	FALSE;
	}

	inline	BOOL				isPartyLeader(int _iSerial)
	{
		if (m_wMemberCount	==	0)
			return	FALSE;

		if (m_aMemberList[0].m_wSerial	==	_iSerial)
			return	TRUE;

		return	FALSE;
	}


	inline	BOOL				isUnionPartyLeader(int _iSerial)
	{
		if (m_wMemberCount	==	0)
			return	FALSE;
		
		if (m_aMemberList[0].m_wSerial	==	_iSerial && m_bf1IsUnionParty)
			return	TRUE;
		
		return	FALSE;
	}

	char*						getPartyStatusString(int _iMethod=-1);
	char*						getItemSharingMethodString(int _iMethod=-1);
	char*						getGoldSharingMethodString(int _iMethod=-1);
	char*						getPartyObjectString(int _iMethod=-1);

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃partyList.cpp
	BOOL						m_bIsReceiveNewJoinPartyRequest;
	
	enum
	{
		eRequestJoinPartyUserCount	=	20,
	};

	class	cRequestJoinPartyUserInfo
	{
	public:
		char	m_strName[dNAME_LENGTH];
		WORD	m_wJob;
		WORD	m_wLevel;
		DWORD	m_dwAskTime;

		void	reset()
		{
			m_wLevel	=	0;
		}
	};

	class	cRequestJoinParty
	{
	public:
		int							m_iUserCount;
		int							m_iFocusUser;
		char						m_strWorkUser[dNAME_LENGTH];
		cRequestJoinPartyUserInfo	m_aList[eRequestJoinPartyUserCount];

		void						reset();
		void						update();
		BOOL						add(char *_lpstrName,int _iJob,int _iLevel);
		BOOL						remove(int _iIndex);
		BOOL						remove(char *_lpstrName);
		BOOL						removeSelectUser();
		cRequestJoinPartyUserInfo*	getByName(char *_lpstrName);
		int							getIndexByName(char *_lpstrName);
	};

	int							m_iPartyCount,m_iHeadPartySerial,m_iLastPartySerial;
	int							m_iSelectParty,m_iFocusParty;
	cRequestJoinParty			m_requestJoinPartyUserList;
	cPartyInfoForPL				m_aPartyList[c_iPartyBufferSizeForPartyList];

	void						receivePartyList(int _iPartyCount,cPartyInfoForPL *_lpList);
	void						resetPartyList();
	void						addRequestJoinPartyUser(char *_lpstrName,int _iJob,int _iLevel)
	{
		m_bIsReceiveNewJoinPartyRequest	=	TRUE;
		m_requestJoinPartyUserList.add(_lpstrName,_iJob,_iLevel);
	}

	void						sendAcceptJoinPartyRequest();
	BOOL						isExistNewRequestJoinPartyUser();
	void						resetNewRequestJoinPartyUserFlag();
//┃partyList.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	
};

#endif