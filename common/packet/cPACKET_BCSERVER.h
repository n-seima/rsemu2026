#pragma once

#include "cPACKET_BASE.h"
#include "cPACKET_GAMESERVER.h"
#include "userCustomDataDefine.h"
#include "definePacketData.h"
#include "partyDefine.h"

#pragma pack(1)

const	int	c_iBCS_TransPacketBufferSize	=	dMAX_PACKET_LENGTH-20;

class CChattingLog
{
public:
	enum
	{
		eCT_TALK=0	,//	일반 말하기
		eCT_SAY		,//	귓말하기
		eCT_PARTY	,//	파티원 대화
		eCT_GUILD	,//	길드원 대화
		eCT_SHOUT	,//	외치기
		eCT_ADMIN	,//	관리자 전달 메세지
	};

	struct stWho
	{
		char	m_strId[dID_LENGTH];
		char	m_strName[dNAME_LENGTH];
	};

	UTime	m_stTime;
	WORD	m_wFieldIndex,m_wChattingType;
	stWho	m_stSender,m_stReceiver;
	char	m_strMessage[dCHAT_LENGTH];

	CChattingLog()
	{
		memset(this,0,sizeof(CChattingLog));
	}
};

typedef enum
{	//	기존의 패킷 타입과 모양새를 갖추기 위해 그냥 d로 시작

	//////////////////////////////////////////////////////////
	//	Client <---> BroadCast Server
	dCB_LOGIN = 0x7001,//client가 broadcast에게 로그인정보
	dCB_UPDATE_USER_ALLINFO	,	//	유저가 예전에 가지고 있던 전체 유저 정보를 업데이트 한다.
								//	m_bf1IsSaveAllDataByClient 플래그 세팅할것
								//	m_bf1IsSaveAllDataByClient 이미 이 플래그가 세팅된 유저는 무시할것
	dCB_CHANGE_USER_SKILL_SETTING,
	dCB_CHANGE_FRIEND_GROUP_NAME,
	dCB_ADD_FRIEND,
	dCB_REMOVE_FRIEND,
	dCB_CHANGE_FRIEND_GROUP,	//	특정 친구의 그룹을 변경

	dBC_LOGIN_RESULT,
	dBC_UPDATE_FRIEND_ONLINE_STATUS	,	//	친구 목록의 온라인 상태 갱신

	dCB_CHANGE_PARTY_PROSPECTUS			,	//	파티 모집요강 변경
	dCB_ASK_PARTY_PROSPECTUS			,	//	파티 모집요강 요청
	dCB_ASK_OPEN_PARTY_LIST				,	//	파티 리스트 요청

	dBC_SERVER_MESSAGE					,	//	서버 메시지
	dBC_OPEN_PARTY_LIST					,	//	오픈된 파티 리스트
	dBC_UPDATE_PARTY_PROSPECTUS			,	//	파티 모집요강

	dBC_INFO_GUILD_MEMBER_CONNECTION_LIST,		// 길드멤버 접속 리스트 ..
	//	Client <---> BroadCast Server
	//////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////
	//	Game Server <---> BroadCast Server
	dGB_LOGIN = 0x7011,//게임서버가 broadcast에게 로그인 정보
	dGB_REG_CLIENT,//새롭게 참가한 클라이언트의 정보
	dGB_REG_OLDCLIENT,//플레이 중인데도 아직 BC에 등록되지 않은 유저에 대한 등록을 요청
	dGB_ADD_FRIEND,
	dGB_FORCED_REMOVE_FRIEND,	//	친구의 요청으로 친구 목록에서 친구의 이름을 뺀다...?
	dGB_GUILD_CHAT,
	dGB_PARTY_CHAT,	//	파티 채팅
	dGB_GUILD_WORK_BUS,	//	길드원 전체에게 보내는 내용 처리
	dGB_PARTY_PACKET_DELIVER,
	dGB_PACKET_DELIVER,		//	패킷 배달
	dGB_UPDATE_INFO			,	//	클라이언트의 정보가 업데이트 되었다.
	dGB_ASK_ITEM_INFO		,	//	운영자가 특정 캐릭터의 아이템 정보를 요청함

	dBG_REG_CLIENT_RESULT = 0x7112,	//	클라이언트 등록 결과
	dBG_REG_OLDCLIENT_RESULT,		//	클라이언트 등록 결과 옛날거?
	dBG_REG_CLIENT_SERIAL	,		//	등록 클라이언트 시리얼
	dBG_ASK_CLIENT_INFO		,		//	게임 서버에서 커뮤니티 서버에 특정 시리얼에 관한 작업을 요청했는데, 실제로 커뮤니티 서버에는 그게 없다.
	dBG_ASK_ITEM_INFO		,

	//	Game Server <---> BroadCast Server
	//////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////
	//	World Server <---> BroadCast Server
	dWB_REMOVE_USERINFO	= 0x7051,	//	유저 정보 제거
	dWB_PARTY_LIST,
	dWB_UPDATE_PARTY_INFO,
	dWB_GUILD_WORK_BUS,


	dWB_WORLDNAME,

	dBW_ASK_PARTY_LIST,
	dBW_ASK_PARTY_INFO,

	//	World Server <---> BroadCast Server
	//////////////////////////////////////////////////////////

}eBCPACKET;


/////////////////////////////////////////////////////////////////
//	
struct CB_LOGIN
{
	cMSG_BASE_TYPE	base;

	WORD		wIndex;		//	허가받은 인덱스
	DWORD		dwCRC32;
	WORD		wGuildIndex;
	char		strId[dID_LENGTH];		//	이하는 정상유저인지 확인을 위해
	char		strName[dNAME_LENGTH];
	WORD		wGuildDisplayIndex;		//	길드창에 디폴드로 보이는 순서. 음...그러니까. 게임 서버 상의 인덱스라고 하는게 맞을려나;
};

struct	CB_UPDATE_USER_ALLINFO
{
	cMSG_BASE_TYPE	base;

	char	strId[dID_LENGTH];		//	처음 등록
	char	strName[dNAME_LENGTH];

	WORD	wFriendCount;

	cUserCustomDataDefine	data;
};

struct	CB_CHANGE_USER_SKILL_SETTING
{
	cMSG_BASE_TYPE	base;

	WORD	wWhat;
	WORD	wValue1,wValue2,wValue3,wValue4;
};

struct	CB_CHANGE_FRIEND_GROUP_NAME
{
	cMSG_BASE_TYPE	base;

	WORD			wGroup;
	char			strName[dFRIEND_GROUP_NAME_LENGTH];
};

struct	CB_CHANGE_PARTY_PROSPECTUS	//	파티 모집요강 변경
{
	cMSG_BASE_TYPE	base;

	WORD			wPartySerial;
	DWORD			dwPreferenceJobMask;		// 직업이 추가되어 WORD -> DWORD 형으로 전환.
	WORD			bf1IsChangeText				:	1;
	WORD			bf10RecruitMemberLevelRange	:	10;

	char			strPartyProspectus[c_iPartyProspectusLength];
};

struct	CB_ASK_PARTY_PROSPECTUS
{
	cMSG_BASE_TYPE	base;

	WORD			wPartySerial;
};

struct	BC_UPDATE_PARTY_PROSPECTUS
{
	cMSG_BASE_TYPE	base;

	WORD			wPartySerial;
	DWORD			dwPreferenceJobMask;			// 직업이 추가되어 WORD -> DWORD 형으로 전환.
	WORD			bf1IsIncludeText			:	1;	//	텍스트도 변경되냐?
	WORD			bf10RecruitMemberLevelRange	:	10;

	char			strPartyProspectus[c_iPartyProspectusLength];
};

struct	CB_ASK_OPEN_PARTY_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			bf10LastReceiveParty	:	10;
	WORD			bf1IsWantNext			:	1;
};

struct	CB_REMOVE_FRIEND
{
	cMSG_BASE_TYPE	base;

	char			strName[dNAME_LENGTH];
};

struct	CB_CHANGE_FRIEND_GROUP
{
	cMSG_BASE_TYPE	base;

	WORD			wChangeGroup;
	char			strName[dNAME_LENGTH];
};

struct BC_LOGIN_RESULT
{
	enum
	{
		eFAILED,					//	비정상적이다 다시 접속 시도 해주세요
		eREQUIRE_UPDATE_DATA,		//	정상적으로 접속했다. 데이터 전송함
		eDO_NOT_REQUIRE_UPDATE_DATA,//	정상적으로 접속했다. 데이터 전송 필요 없다.
	};

	cMSG_BASE_TYPE			base;

	WORD					wResult;		//	1 - 정상, 0 - 이상유저,다시 접속을 시도해주세요.
	WORD					wFriendCount;
	WORD					wBCSerial;

	cUserCustomDataDefine	data;
};

struct BC_SERVER_MESSAGE
{
	cMSG_BASE_TYPE				base;

	WORD						wMessage;

	enum
	{
		eINCORRECT_PARTY_INFO_PLEASE_RETRY_WORK,
		eCOMPLETE_ASK_WORK,
	};
};

struct BC_OPEN_PARTY_LIST
{
	cMSG_BASE_TYPE				base;

	WORD			wCount;
	cPartyInfoForPL	aList[c_iPartyBufferSizeForPartyList];
};

struct BC_UPDATE_FRIEND_ONLINE_STATUS	//	친구 목록의 온라인 상태 갱신
{
	cMSG_BASE_TYPE				base;
	
	cFriendListOnlineInfoBase	onlineStatus;
};
struct BC_INFO_GUILD_MEMBER_CONNECTION_LIST	//	길드 목록의 온라인 상태 갱신
{
	cMSG_BASE_TYPE				base;
	WORD	wCount;			// 접속한 길드원 수 
	DWORD	adwHashNameCode[c_iMaxGuildMemberCount];		//	접속한 길드원 네임..
};
/////////////////////////////////////////////////////////////////
//	
struct GB_LOGIN
{
	cMSG_BASE_TYPE	base;

	char		strName[dNAME_LENGTH];	//	게임서버의 이름, 중복되면 안된다.
};

struct GB_REG_CLIENT
{
	cMSG_BASE_TYPE	base;

	char		strId[dID_LENGTH];		//	이하는 정상유저인지 확인을 위해
	char		strName[dNAME_LENGTH];

	WORD		wLevel;
	WORD		wGuildSerial;
	WORD		wGuildRank;
	WORD		wAvatarIndex;
	WORD		wOper;					//	운영자 레벨
	WORD		wSerial;				//	돌려받을때 사용할 게임서버내의 client serial
	WORD		wCurrentField;
	WORD		wPartySerial;
};

struct	GB_ADD_FRIEND
{
	cMSG_BASE_TYPE		base;

	cFriendInfoDefine	aInfo[2];
	WORD				wIsRecoverList;
};

struct	GB_FORCED_REMOVE_FRIEND
{
	cMSG_BASE_TYPE		base;

	char				strOwnerName[dNAME_LENGTH],strFriendName[dNAME_LENGTH];
};

struct	GB_GUILD_CHAT
{
	cMSG_BASE_TYPE		base;

	WORD				wBCSerial;
	char				strName[dNAME_LENGTH];
	char				strMessage[dCHAT_LENGTH];	//	메시지
};

struct	GB_GUILD_WORK_BUS
{
	cMSG_BASE_TYPE		base;

	WORD				wGuildSerial;
	BYTE				aBuffer[c_iBCS_TransPacketBufferSize];
};

struct	GB_PARTY_PACKET_DELIVER
{
	cMSG_BASE_TYPE		base;

	WORD				wPartySerial;
	WORD				wFieldSerial;
	BYTE				aBuffer[c_iBCS_TransPacketBufferSize];
};

struct	GB_PACKET_DELIVER
{
	cMSG_BASE_TYPE		base;

	WORD				wDeliverTargetCount;
	WORD				wPacketOwner;
	WORD				wTargetField;
	BYTE				aBuffer[c_iBCS_TransPacketBufferSize];
};

struct	GB_PARTY_CHAT
{
	cMSG_BASE_TYPE		base;

	WORD				wBCSerial;
	char				strName[dNAME_LENGTH];
	char				strMessage[dCHAT_LENGTH];	//	메시지
};

struct GB_UPDATE_INFO
{
	cMSG_BASE_TYPE		base;

	char		strName[dNAME_LENGTH];
	WORD		wBCSerial;

	WORD		wLevel;
	WORD		wGuildSerial;
	WORD		wGuildRank;
	WORD		wOper;		//	운영자 레벨
	WORD		wCurrentField;
	WORD		wPartySerial;
	WORD		wAvatarIndex;	//	아바타 인덱스 
};

struct GB_ASK_ITEM_INFO
{
	cMSG_BASE_TYPE		base;

	char		strAskerName[dNAME_LENGTH];
	char		strTargetName[dNAME_LENGTH];
};

struct BG_ASK_ITEM_INFO
{
	cMSG_BASE_TYPE		base;

	WORD		wAskerBCSerial;
	char		strTargetName[dNAME_LENGTH];
};

struct BG_REG_CLIENT_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD		wIndex;					//	BC의 임시테이블에 둥록된 인덱스
										//	0xffff - 에러. 정상적으로 작업이 불가능합니다. (인원수 초과 혹은 기타 문제)
										//	0xfffe - 이미 등록된 유저입니다.
	WORD		wSerial;				//	게임서버내의 client serial
};

struct BG_REG_CLIENT_SERIAL
{
	cMSG_BASE_TYPE	base;

	WORD			wRealIndex;				//	BC에 정상적으로 등록된 인덱스 
	WORD			wSerial;
};

struct BG_ASK_CLIENT_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;				//	BC에 정상적으로 등록된 인덱스 
};

/////////////////////////////////////////////////////////////////
//	

struct WB_WORLDNAME
{
	cMSG_BASE_TYPE	base;

	char	strWorldName[dWORLD_NAME_LENGTH];
};

struct WB_REMOVE_USERINFO
{
	cMSG_BASE_TYPE	base;

	char	strId[dID_LENGTH];
};

const	int	c_iPartyCountForWBPartyListPacket	=	50;

struct	WB_PARTY_LIST
{
	cMSG_BASE_TYPE	base;

	DWORD			bf8CurrentCount	:	8;
	DWORD			bf1IsFirst		:	1;
	DWORD			bf1IsLast		:	1;

	cPartyInfoForBCS	aList[c_iPartyCountForWBPartyListPacket];
};

struct	WB_UPDATE_PARTY_INFO
{
	cMSG_BASE_TYPE		base;

	cPartyInfoForBCS	partyInfo;
};

struct	WB_GUILD_WORK_BUS
{
	cMSG_BASE_TYPE		base;
	WORD				wGuildSerial;
	char				aBuffer[c_iBCS_TransPacketBufferSize];
};

struct	BW_ASK_PARTY_LIST
{
	cMSG_BASE_TYPE	base;
};

struct	BW_ASK_PARTY_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wPartySerial;
};

#pragma pack()