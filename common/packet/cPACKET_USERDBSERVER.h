#ifndef _classPACKET_USER_DBSERVER_H
#define _classPACKET_USER_DBSERVER_H

//	#pragma once : 무조건 한번만 링크 ... 
#include "cPACKET_BASE.h"
#include "definePacketData.h"
#include "adCommandDefine.h"
#include "carrotshopdefine.h"

#pragma pack(1)


class	cMSG_BASE_TYPE_FORUSERDB
{
public:
	WORD	wSize				//	패킷 사이즈
,			wType				//	패킷 타입
,			wSerial;
	inline	void	set(WORD size,WORD type,int serial=-1)	{wSize	=	size,wType	=	type;wSerial = serial;}
};
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Login->User DB
//┃패킷 인덱스 정의
//	━━필드 진입 전					0x1401 ~ 0x1500 
#define	dLU_LOGIN						0x1401	//	아바타 정보까지 보내며, 바로 게임으로 들어간다.
#define	dLU_WLOGIN						0x1402	//	월드 서버에서 유저 로그인을 확인해 준다.
#define dLU_LOGOUT						0x1403	//	서버에서 로그아웃 사실을 알려준다.
#define dLU_LOGIN_OTP					0x1404	//	입력된 OTP를 전달한다.

#define dSU_LOGIN						0x1410	//	각 서버들이 유저 Db에 로그인을...
#define dSU_INFO						0x1411	//	변경된 서버 정보를 보내준다.
#define dSU_NOT_DIS						0x1412	//	dis할 유저를 찾지 못했다.
//┃Login->User DB
//┃패킷 인덱스 정의
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃User DB->Login
//┃패킷 인덱스 정의
//	━━필드 진입 전					0x1501 ~ 0x1600
#define dUL_RESULT_LOGIN				0x1501	//	로그인 결과
#define dUL_RESULT_LOGIN_OTP			0x1507	//	OTP입력 결과
#define dUL_RESULT_WLOGIN				0x1502	//	로그인 결과
#define dUL_SERVER_LIST_ALL				0x1503	//	서버리스트
#define dUL_SERVER_ADD					0x1504	//	서버 추가
#define dUL_SERVER_DELETE				0x1505	//	서버 삭제

#define dUS_DIS_USER					0x1506	//	해당 유저를 죽여라... to world
//┃User DB->Login
//┃패킷 인덱스 정의
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Command
#define dUL_ONOFF						0x1451	//	로그인 서버 온/오프
#define dUL_CHANGEVER					0x1452	//	로그인 서버의 버젼을 수정한다.
#define dUL_CHANGEMAXUSER				0x1453	//	로그인 서버의 최대 수용인원을 수정한다.

#define dUW_NOTICE						0x1461	//	공지 or 유료 유저 메세지 전송 
#define dUW_BANISH						0x1462	//	특정 유저 추방하기

#define dUW_WORLD_CLOSE					0x1471	//	월드를 닫는다.
#define dUW_RENAME_AVATAR				0x1472	//	아바타 이름 변경
//┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Server Info
#define dWU_WORLDINFO					0x14A1	//	월드 서버가 보낸자료. 월드의 정보를 DB에 갱신한다.

#define dWU_ASK_CARROTSHOPINFO			0x14B1	//	게임내 당근 상점의 정보를 받아온다.
#define dUW_ASK_CARROTSHOPINFO_RESULT	0x14F1

#define dWU_ASK_INCREASE_DB_EVENT_VALUE			0x14B2	//	이벤트 수치 증가 요청
#define dUW_RESULT_OF_INCREASE_DB_EVENT_VALUE	0x14F2	//	이벤트 수치 증가 요청 결과

#define dWU_ASK_DB_CONTROLLED_EVENT_INFO		0x14B3
#define dUW_DB_CONTROLLED_EVENT_INFO			0x14F3

#define dWU_ASK_DUEL_RECORD						0x14B4
#define dUW_DUEL_RECORD							0x14F4

#define dWU_UPDATE_DUEL_RECORD					0x14B5	//	결투 전적 업데이트

#define dWU_ASK_DUEL_RANKING_INFO				0x14B6	//	결투 랭킹 요청
#define dUW_DUEL_RANKING_INFO					0x14F5	//	결투 랭킹

#define dWU_ASK_DUEL_TEAM_INFO					0x14B7	//	결투 팀 정보 요청
#define dUW_DUEL_TEAM_INFO						0x14F6	//	결투 팀 정보 전송

#define dWU_UPDATE_DUEL_TEAM_RECORD				0x14B8	//	결투 전적 업데이트

#define dWU_RESET_DUEL_TEAM_INFO				0x14B9	//	결투 전적 초기화

#define dUW_RESET_WEEKLY_DUEL_INFO				0x14F7	//	주간 결투 전적 초기화 요청


//┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//////////////////////////////////////////////////////////////////////////
//	로그 정보 처리
	enum
	{
		dWU_LOG_INOUT		=	0x1510,
		dWU_LOG_QUEST,
	};
//	로그 정보 처리
//////////////////////////////////////////////////////////////////////////


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Login->User DB
//┃로그인/로그아웃
struct	LU_LOGIN
{
	cMSG_BASE_TYPE_FORUSERDB	base;
	char			strId[dID_LENGTH];
	char			strPassword[dPASS_LENGTH];
	char			strSelectServer[dNAME_LENGTH];
	char			strLoginIp[dIP_SIZE];
	char			strMacAddress[64];
	char			strClientCode[12];	//	클라이언트 고유 코드
	WORD			disconnectExistID;	//	접속해 있는 아이디의 접속을 끊고 접속.
};	//	struct	CL_LOGIN

struct  LU_LOGIN_OTP
{
	cMSG_BASE_TYPE_FORUSERDB	base;
	char			strId[dID_LENGTH];
	char			strOTP[12];	//	클라이언트 고유 코드
	char			strIp[16];
};

struct  LU_WLOGIN
{
	cMSG_BASE_TYPE_FORUSERDB	base;
	DWORD			serial;
	char			strId[dID_LENGTH];
	DWORD			dwCode;
	char			strName[dNAME_LENGTH];
	char			strIP[dIP_SIZE];
	char			strMacAddress[64];
	char			strClientCode[12];	//	클라이언트 고유 코드
};

struct LU_LOGOUT
{
	cMSG_BASE_TYPE_FORUSERDB	base;
	char			strId[dID_LENGTH];
	char			strIp[dIP_SIZE];
	//	기타 갱신될 내용들
};

struct SU_LOGIN
{
	cMSG_BASE_TYPE_FORUSERDB	base;
	char				id[dWORLD_NAME_LENGTH];	//	서버의 이름		- login서버일 경우 이름만 받아둔다.
	char				ip[dIP_SIZE];	//	서버의 IP
	WORD				wServerIndex;
	WORD				wServerListIndex;
	WORD				wUserCount;		//	서버에 접속한 총 인원
	WORD				wType : 10;			//	서버의 타입 0 - NORMAL , 1 - TEST
	WORD				m_bf1IsInnerOpen :	1;
	WORD				wMessage;			//	서버의 타입 0 - NORMAL , 1 - TEST
};

struct	UL_RESULT_WLOGIN
{
	cMSG_BASE_TYPE_FORUSERDB	base;
	DWORD				dwResult;	// 0 - 누군가 로그인 되어 있거나.. 없는 아이디이다.
	DWORD				serial;
	WORD				isOper;
	WORD				isNetCafeUser;
	char				strId[dID_LENGTH];
	char				strName[dNAME_LENGTH];
	char				strIP[dIP_SIZE];
};

struct SU_INFO
{
	cMSG_BASE_TYPE_FORUSERDB	base;
	WORD				wUserCount;		//	서버에 접속한 총 인원
	WORD				wType : 10;			//	서버의 타입 0 - NORMAL , 1 - TEST
	WORD				m_bf1IsInnerOpen : 1;
	WORD				wMessage;		//	서버의 타입 0 - NORMAL , 1 - TEST
};

struct	SU_NOT_DIS
{
	cMSG_BASE_TYPE_FORUSERDB	base;
	char			strId[dID_LENGTH];
	char			strIp[dIP_SIZE];
};
//┃로그인/로그아웃
//┃Login->User DB
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃User DB->Login
//┃로그인/로그아웃
#define dRESULT_LOGIN_SUCCESS			0x000	//	성공
#define dRESULT_LOGIN_FAILED			0x001	//	실패
#define dRESULT_LOGIN_USEDID			0x002	//	이미 같은 아이디가 로그인 함
#define dRESULT_LOGIN_RETRY				0x003	//	상냥하게 '다시 해부려라 +_+' 
#define dRESULT_LOGIN_NOT_SAME_VERSION	0x004	//	버젼이 틀리네... 
#define dRESULT_LOGIN_OTP				0x005	//	1차 성공. OTP 입력 바람.
#define dRESULT_LOGIN_SUCCESS_BUT_NOT_USE_OTP	0x006	//	성공. OTP는 사용하지 않음.
#define dRESULT_LOGIN_SUCCESS_PERSONALCOM	0x007	//	PERSONAL COMM 성공. 
#define dRESULT_LOGIN_FAIL_PERSONALCOM		0x008	//	PERSONAL COMM 실패. 


struct UL_RESULT_LOGIN
{
	cMSG_BASE_TYPE_FORUSERDB	base;
	char			strID[dID_LENGTH];
	DWORD			dwResult;
	DWORD			dwCode;
	BOOL			isOTPUser;
	char			strWhy[64];
};

#define dRESULT_LOGIN_OTP_SUCCESS		0x00		//	성공
#define dRESULT_LOGIN_OTP_INCORRECT		0x01		//	잘못된 OTP코드
#define dRESULT_LOGIN_OTP_EXPIRE		0x02		//	OTP파기됨. 다시 요청할 것!
#define dRESULT_LOGIN_OTP_ERROR			0x03		//	기타 에러. -0-??

struct UL_RESULT_LOGIN_OTP
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	char			strID[dID_LENGTH];
	DWORD			dwResult;
};
//
//━>서버리스트

#define	dSERVER_TYPE_NORMAL		0	//	노말 서버
#define	dSERVER_TYPE_TEST		1	//	테스트 서버

class	cSERVER_SINFO	//	서버정보
{
public:
	char	m_strName[dWORLD_NAME_LENGTH];	//	서버 이름
	char	m_strIP[dIP_SIZE];			//	서버 IP

	WORD	m_wType					:	10;		//	서버 타입
	WORD	m_bf1IsInnerOpen		:	1;		//	내부 오픈

	WORD	m_wCount;					//	접속 인원
	WORD	m_wMessage;					//	특별한 메세지
	WORD	m_bf8ServerPlaceInList	:	8;		//	서버 인덱스(보이는거
	WORD	m_bf8SettingServerIndex	:	8;		//	서버 인덱스(config-world.txt 파일에 SERVERINDEX 의 인덱스... 불변 인덱스다)

};	//	class	cSERVER_SINFO

struct	UL_SERVER_LIST_ALL
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	WORD			wCount;				//	서버군 수
	cSERVER_SINFO	aServerList[dMAX_SERVER_COUNT];
};

struct	UL_SERVER_ADD
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	cSERVER_SINFO		ssInfo;
};

struct	UL_SERVER_DELETE
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	int				idx;
};

//<━서버리스트

struct  US_DIS_USER
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	char			strId[dID_LENGTH];
	WORD			wWhy;				//	0 - 시스템의 문제로 종료하겠습니다.
										//	1 - 누군가 접속해서 종료합니다.
};

//┃로그인/로그아웃
//┃User DB->Login
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Command

/////////////////////////////////////////////	to LOGIN
struct	UL_ONOFF	//	로그인 서버 온/오프
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	DWORD	onoff;		//	끌까 말까 ~
	char	msg[4000];	//	공지사항 올리기 ~
};

struct	UL_CHANGEVER	//	로그인 서버의 버젼을 수정한다.
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	int		ver;
};

struct	UL_CHANGEMAXUSER//	로그인 서버의 최대 수용인원을 수정한다.
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	int		maxuser;
};

/////////////////////////////////////////////	to GAME
struct	UW_NOTICE	//	공지 or 유료 유저 메세지 전송 
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	char	msg[dCHAT_LENGTH];
	WORD	isUser;	//	유료 유저 메세지일 경우
	char	who[dNAME_LENGTH];
};

struct	UW_BANISH	//	특정 유저 추방하기
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	char	who[dNAME_LENGTH];	//	누구에게
	char	msg[dCHAT_LENGTH];			//	추방전에 보내줄 메세지
};

/////////////////////////////////////////////	to WORLD
struct	UW_WORLD_CLOSE	//	월드를 닫는다.
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	UTime	relTime,finTime;
	char	msg[dCHAT_LENGTH];
};

struct UW_RENAME_AVATAR
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	char	strOldName[dNAME_LENGTH];
	char	strNewName[dNAME_LENGTH];
};


//┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Server Info

#define dSERVERINFO_COUNT	10
#define dMAXSERVICE_COMPANY_COUNT	5

struct SERVERINFO
{
	WORD	idx;		//	worldidx + oneself
	WORD	worldidx;	//	worldidx
	WORD	servertype;	//	0-world, 1-game
	char	ip[16];		//	ip address
	WORD	curusercount;
	WORD	maxusercounttoday;
};

struct WU_WORLDINFO//	월드 서버가 보낸자료. 월드의 정보를 DB에 갱신한다.
{
	enum
	{
		eSERVICE_YAHOO,
		eSERVICE_PARAN,
		eSERVICE_RESERVE2,
		eSERVICE_RESERVE3,
		eSERVICE_RESERVE4,
	};
	cMSG_BASE_TYPE_FORUSERDB	base;

	SERVERINFO		info[dSERVERINFO_COUNT];	//	 0 은 월드 자체.. , 1부터 각 게임서버
#if defined(_FOR_KOREA) || defined(_FOR_THAI)
	DWORD	curusercountReserve[dMAXSERVICE_COMPANY_COUNT];
#endif
};

struct WU_ASK_INCREASE_DB_EVENT_VALUE
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	char	strUserID[dNAME_LENGTH];	//	문제 없을 경우는 이벤트 응모자로 기록
	char	strUserName[dNAME_LENGTH];	//	얘는 필요 없음. 그냥 리턴할때 담아 주면 됨
	WORD	wEventIndex;				//	이벤트 인덱스
	int		iMaxCount;					//	이벤트 최대 응모 가능 회수
	BOOL	bReset;						//	이벤트 수치 및 응모 유저 ID 초기화
	BOOL	bIsAllowMulipleID;			//	같은 ID의 유저가 중복 신청 가능한지 체크
};

enum
{
	//	result of increase db event value
	eROIDEV_SUCCESS,
	eROIDEV_EVENT_COUNT_FULL,	//	이벤트가 풀이다.
	eROIDEV_ALREADY_EXIST_USER,	//	이미 존재하는 계정이다.
	eROIDEV_RESET,	//	리셋 했다.
};

struct UW_RESULT_OF_INCREASE_DB_EVENT_VALUE
{
	cMSG_BASE_TYPE_FORUSERDB	base;
//	결과
	WORD	wResult;	

//	아래 정보는 WU_ASK_INCREASE_DB_EVENT_VALUE 패킷에서 받은 정보 그대로 복사
	char	strUserID[dNAME_LENGTH];
	char	strUserName[dNAME_LENGTH];	//	WU_ASK_INCREASE_DB_EVENT_VALUE 에 값을 그대로 돌려줌
	WORD	wEventIndex;				//	이벤트 인덱스 WU_ASK_INCREASE_DB_EVENT_VALUE 에 값을 그대로 돌려줌

//	증가한 이벤트 수치
	int		iValue;	//	증가한 수치
};

struct WU_ASK_CARROTSHOPINFO
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	WORD	wIsUpdateNow;
};

struct UW_ASK_CARROTSHOPINFO_RESULT
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	cCarrotShopDefine			cShopInfo;
	WORD	wIsUpdateNow;
};

struct WU_ASK_DB_CONTROLLED_EVENT_INFO
{
	cMSG_BASE_TYPE_FORUSERDB	base;
};

struct UW_DB_CONTROLLED_EVENT_INFO
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	int		aiValues[c_iDBControlledEvnetCount];
};

struct WU_ASK_DUEL_RECORD
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	char		strId[dID_LENGTH];
	int			iGSSerialInWorld;	//	이 정보를 요청한 게임서버의 월드상에서의 시리얼
};

struct	UW_DUEL_RECORD
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	char		strId[dID_LENGTH];
	cDuelRecord	record;

	int			iGSSerialInWorld;//	이 정보를 요청한 게임서버의 월드상에서의 시리얼
};

struct WU_UPDATE_DUEL_RECORD
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	char		strId[dID_LENGTH];
	int			iGSSerialInWorld;	//	이 정보를 요청한 게임서버의 월드상에서의 시리얼

	cDuelRecord	record;
};

struct WU_UPDATE_DUEL_TEAM_RECORD
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	char			strTeamName[c_iDuelTeamNameLength];
	int				iGSSerialInWorld;	//	이 정보를 요청한 게임서버의 월드상에서의 시리얼

	cDuelTeamRecord	record;
};

struct WU_RESET_DUEL_TEAM_INFO
{
	cMSG_BASE_TYPE_FORUSERDB	base;
};

struct WU_ASK_DUEL_RANKING_INFO
{
	cMSG_BASE_TYPE_FORUSERDB	base;
};

struct UW_DUEL_RANKING_INFO
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	WORD				wIsDailyRanking;	//	0 : 주간 랭킹,1 : 일간 랭킹
	WORD				wWeeklyRankerCount,wTrialGameRankerCount;	//	주간 랭커 수,TrialGame 랭커 수
	cDuelRankingInfo	aList[200];	//	주간 랭킹 + TrialGame랭킹
};

struct WU_ASK_DUEL_TEAM_INFO
{
	cMSG_BASE_TYPE_FORUSERDB	base;
};

struct UW_DUEL_TEAM_INFO
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	WORD			wTotalTeamCount;
	WORD			wTeamCount;
	cDuelTeamInfo	aTeams[100];
};

struct UW_RESET_WEEKLY_DUEL_INFO
{
	cMSG_BASE_TYPE_FORUSERDB	base;
};


//┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//////////////////////////////////////////////////////////////////////////
//	로그 정보 처리
struct WU_LOG_INOUT
{	
	cMSG_BASE_TYPE_FORUSERDB	base;

	BOOL	isIn;	//	0 로그아웃 1 로그인
	short	lev;
	short	job;
	char	strID[20];
	char	strName[20];
	char	worldName[20];

};

struct WU_LOG_QUEST
{
	cMSG_BASE_TYPE_FORUSERDB	base;

	BOOL	isStartQuest;	//	0 Complete 1 Get 2 Cancel
	short	questIdx;
	short	lev;
	short	job;
	char	strID[20];
	char	strName[20];
	char	worldName[20];
};
//	로그 정보 처리
//////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif	//	_classPACKET_DBSERVER_H
