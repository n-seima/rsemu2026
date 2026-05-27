#ifndef _classPACKET_DBSERVER_H
#define _classPACKET_DBSERVER_H

//	#pragma once : 무조건 한번만 링크 ... 

#include "cPACKET_BASE.h"
#include "definePacketData.h"
#include "cGuildDefine.h"
#include "userCustomDataDefine.h"

#pragma pack(1)

class	cMSG_BASE_TYPE_FORDB
{
public:
	WORD	wSize,				//	패킷 사이즈
			wType;				//	패킷 타입

	inline	void	set(WORD size,WORD type)	{wSize	=	size,wType	=	type;}
};

/////////////////////////////////////////////////////////
//	PACKET TYPE
/////////////////////////////////////////////////////////
#define dDDMSG_IAM				0x4101		//	각 서버가 자신의 정보를 DB에게 알려준다. 
											//	가능하면 WORLD 서버만 연결해라
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Login <-> World <-> Avatar DB Server 
//┃
#define dLDMSG_GETAVATARS		0x4310		//	아마타 정보를 주라.
#define dLDMSG_CREATEAVATAR		0x4311		//	새로운 아바타 만들기 
#define dLDMSG_SELECTAVATAR		0x4312		//	선택한 아바타 
#define dLDMSG_DELETEAVATAR		0x4313		//	아바타 지우기 
#define dLDMSG_DELETELIST		0x4314		//	아바타 리스트 지우기 
#define dLDMSG_COPY_DATA		0x4315		//	다른 서버의 아바타 데이타 복사해두기
#define dLDMSG_LOGIN			0x4316		//	로그인 한 걸 아바타에 기록한다.
#define dLDMSG_LOGOUT			0x4317		//	로그아웃 한 걸 아바타에 기록한다.
#define dLDMSG_LOGINRESET		0x4318		//	로그아웃 한 걸 아바타에 기록한다.


#define dDLMSG_GETAVATARS		0x4710		//	아마타들 정보를 보내준다.
#define dDLMSG_CREATEAVATAR		0x4711		//	새로운 아바타 만들기 결과
#define dDLMSG_SELECTAVATAR		0x4712		//	선택한 아바타 결과
#define dDLMSG_DELETEAVATAR		0x4713		//	아바타 지우기 결과
#define dDLMSG_COPY_DATA_RESULT	0x4714		//	아바타 데이터 복사 결과

//	Admin 툴과 관련된 것들..
#define dLDMSG_DELETE_LIST		0x4321		//	특정 유저의 리스트 파일 지우기
#define dLDMSG_UPDATESAVE_USER	0x4322		//	특정 유저의 세이브 파일 업데이트 하기
#define dLDMSG_UPDATESAVE		0x4323		//	남아 있는 전 캐릭터의 세이브 파일 업데이트 하기
#define dLDMSG_GET_DATA			0x4324		//	어드민툴에서 데이타 요청
#define dLDMSG_SAVE_DATA		0x4325		//	데이타 세이브 요청 
#define dLDMSG_SET_ADMIN		0x4326		//	관리자로 설정하기

#define dDLMSG_DELETE_LIST		0x4721
#define dDLMSG_UPDATESAVE_USER	0x4722
#define dDLMSG_UPDATESAVE		0x4723
#define dDLMSG_GET_DATA			0x4724		//	어드민툴에서 데이타 요청
//┃
//┃Login <-> World <-> Avatar DB Server 
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Game <-> World <-> Avatar DB Server 
//┃
#define	dGDMSG_LOGINAVATAR					0x4401		//	아바타가 게임서버로 들어왔으니 가지고 있는 데이타 주라. 없음 말구.. -_-;
#define dGDMSG_LOGOUTAVATAR					0x4402		//	아바타 나간다. -0-
#define dGDMSG_KILLAVATAR					0x4403		//	이놈을 죽여라.

#define dDGMSG_LOGINAVATAR					0x4801		//	가져가라 --;
#define dDGMSG_LOGOUTAVATAR					0x4802		//	저장 결과를 돌려준다.


#define dGDMSG_BUYLIST						0x4411
#define dGDMSG_USEBUY						0x4412
#define dGDMSG_GET_CARROT_COUNT				0x4413		//	당근 개수 얻어오기
#define dGDMSG_USE_CARROT_LOG				0x4415		//	당근 사용에 대한 로그

#define dDGMSG_BUYLIST						0x4811
#define dDGMSG_USEBUY						0x4812
#define dDGMSG_PREMIUM_ITEM_PRICE_INFO		0x4813		//	프리미엄 아이템 가격 정보
#define dDGMSG_GET_CARROT_COUNT_RESULT		0x4814		//	당근 개수 얻어 오기 결과
#define dDGMSG_USE_CARROT_RESULT			0x4815		//	당근 사용 결과


#define dGDMSG_PROVISION_RANK_REQ			0x4421		//	보물상자 단서 랭킹 요청

#define dDGMSG_PROVISION_RANK_RET			0x4821		//	보물상자 단서 랭킹 결과


#define dGDMSG_PERSONALDATA					0x4431		//	친구리스트, 스킬 슬롯 데이터
#define dGDMSG_SAVE_PERSONALDATA			0x4432		//	저장하기

#define dDGMSG_PERSONALDATA					0x4831		//	친구리스트, 스킬 슬롯 데이터

#define dGDMSG_STAT_GOLD					0x4441		//	통계 - GOLD

#define dG2D_BUY_PREMIUM_ITEM_TO_WEB_DB		0x4442		//	프리미엄 아이템 구매한 걸 웹 DB로 전송한다.


//┃
//┃Game <-> World <-> Avatar DB Server 
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃World <-> Avatar DB Server 
//┃
#define	dWDMSG_SAVEAVA_TO_FILE	0x4501		//	아바타 데이타를 저장한다.
#define	dWDMSG_BANKOPEN			0x4502		//	은행을 열었다. 
#define	dWDMSG_SAVEBANK			0x4503		//	은행 정보 저장 - 아이템 하나씩..
#define dWDMSG_UPDATESAVE		0x4505		//	은행 데이타를 업데이트 한다.
#define dWDMSG_KILL_AVATAR		0x4506		//	강제 종료 시킬 유저

#define dWDMSG_GETGUILD			0x4511		//	길드 데이타를 요청한다.
#define dWDMSG_CREATEGUILD		0x4521		//	길드 생성
#define dWDMSG_DELETEGUILD		0x4522		//	길드 삭제
#define dWDMSG_JOINGUILD		0x4523		//	길드 참가
#define dWDMSG_EXITGUILD		0x4524		//	길드 탈퇴
#define dWDMSG_SAVEGUILDBASE	0x4525		//	길드 기본정보 저장하기
#define dWDMSG_CHANGERANK		0x4526
#define dWDMSG_DELETEGUILDER	0x4527		//	길드 리스트에서만 캐릭터 삭제

#define dWDMSG_SAVEGUILD_GUARDIAN		0x4528
#define dWDMSG_SAVEGUILD_INVENTORY		0x4529
#define dWDMSG_SAVEGUILD_MARK			0x4530
#define dWDMSG_SAVEGUILD_STRUCTURE		0x4531
//#define dWDMSG_SAVEGUILD_OBJECT		0x4531
#define dWDMSG_SAVEGUILD_PET			0x4532
//#define dWDMSG_SAVEGUILD_POLL			0x4533
//#define dWDMSG_SAVEGUILD_SELLINGITEM	0x4534
#define dWDMSG_SAVEGUILD_SKILL			0x4535
#define dWDMSG_MT_TIME_START			0x4536	//	정기점검 시간 시작.
#define dWDMSG_MT_TIME_END				0x4537	//	정기점검 시간 종료.
#define dWDMSG_ASK_COMPUTE_DUEL_RANKING	0x4538
#define dWDMSG_ASK_DUEL_RANKING_INFO	0x4539
#define dWDMSG_ASK_DUEL_RECORD			0x453a		//	결투 전적 요청
#define	dWDMSG_TRANS_AVATAR_TO_GVG		0x453b		//	아바타 정보 GVG 서버로 전공


#define dDWMSG_SAVEAVA_TO_FILE	0x4901
#define dDWMSG_BANKOPEN			0x4902		//	전체 데이타를 한번 보내준다.
#define	dDWMSG_SAVEBANK			0x4903		//	빼가는걸까 넣는걸까..

#define dDWMSG_GETGUILD			0x4911		//	길드 갯수와 정보를 보내준다.
#define dDWMSG_GUILD_MEMBER		0x4912		//	길드원의 데이타를 보내준다.
#define dDWMSG_GUILD_DATA		0x4913		//	길드데이타를 보내준다.
#define dDWMSG_CREATEGUILD		0x4921		//	길드 생성 결과 
#define dDWMSG_DELETEGUILD		0x4922		//	길드 삭제 결과
#define dDWMSG_JOINGUILD		0x4923		//	길드 참가
#define dDWMSG_EXITGUILD		0x4924		//	길드 참가
#define dDWMSG_CHANGERANK		0x4926		

#define dDWMSG_SAVEGUILD_GUARDIAN		0x4928
#define dDWMSG_SAVEGUILD_INVENTORY		0x4929
#define dDWMSG_SAVEGUILD_MARK			0x4930
#define dDWMSG_SAVEGUILD_OBJECT			0x4931
#define dDWMSG_SAVEGUILD_PET			0x4932
#define dDWMSG_SAVEGUILD_POLL			0x4933
#define dDWMSG_SAVEGUILD_SELLINGITEM	0x4934
#define dDWMSG_SAVEGUILD_SKILL			0x4935

#define dDWMSG_DUEL_RECORD				0x4936		//	결투 전적
#define	dDWMSG_TRANS_AVATAR_TO_GVG_RESULT	0x4937		//	아바타 정보 GVG 서버로 전송 결과

/////////////////////////////////////////////////////
//	EVENT - Gold Rush,ItemPack
#define dWDMSG_ASK_EVENTS_INFO			0x4551

#define dDWMSG_GR_INFO_RESULT			0x4951
#define dDWMSG_ITEMPACK_RESULT			0x4952
#define dDWMSG_DUEL_RANKING_INFO		0x4953

//┃
//┃World <-> Avatar DB Server 
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


/////////////////////////////////////////////////////////
//	PACKET s
/////////////////////////////////////////////////////////
struct	DDMSG_IAM
{
	cMSG_BASE_TYPE_FORDB		base;
	WORD						iIam;//0게임/1월드/2BC
};

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Login <-> World <-> Avatar DB Server 
//┃

struct CMMSG_DISCONNECT
{
	cMSG_BASE_TYPE_FORDB	base;
};

////////////////////////////////////////////////////////////////////////
//	login 서버에서  DB서버로 
//	0x4101 ~ 
////////////////////////////////////////////////////////////////////////

struct	LDMSG_GETAVATARS{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD						u_serial;
	DWORD						worldSerial;			//	world 서버의 유저 시리얼
	char						strID[dID_LENGTH];
};

struct LDMSG_CREATEAVATAR{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD						u_serial;
	DWORD						worldSerial;			//	world 서버의 유저 시리얼
	char						strID[dID_LENGTH];
	char						strName[dNAME_LENGTH];	//	이름
	char						strIP[dIP_SIZE];
	WORD						wJob;					//	직업
	WORD						wServerType;	//	서버 타입
};

struct	LDMSG_SELECTAVATAR{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD						loginSerial;
	DWORD						worldSerial;
	char						strID[dID_LENGTH];
	char						strNAME[dNAME_LENGTH];	//	선택한 캐릭터의 이름
};

struct	LDMSG_DELETEAVATAR{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD						u_serial;
	DWORD						worldSerial;			//	world 서버의 유저 시리얼
	char						strID[dID_LENGTH];
	char						strNAME[dNAME_LENGTH];	//	선택한 캐릭터의 이름
	char						strIP[dIP_SIZE];
};

struct	LDMSG_DELETELIST{
	cMSG_BASE_TYPE_FORDB		base;

	char						strId[dID_LENGTH];
};

struct	LDMSG_COPY_DATA
{
	cMSG_BASE_TYPE_FORDB		base;

	char			strId[dID_LENGTH];
	char			strOrgName[dNAME_LENGTH];
	WORD			wNameStoneIndex;
	WORD			wType;
	WORD			wWorld;
	cAVATAR_INFO	info;
};

struct LDMSG_LOGIN
{
	cMSG_BASE_TYPE_FORDB		base;

	char			strId[dID_LENGTH];
	char			strName[dNAME_LENGTH];
	int				externalType;
};

struct LDMSG_LOGOUT
{
	cMSG_BASE_TYPE_FORDB		base;
	
	char			strId[dID_LENGTH];
};

struct LDMSG_LOGINRESET
{
	cMSG_BASE_TYPE_FORDB		base;
	
};


struct	DLMSG_COPY_DATA_RESULT
{
	cMSG_BASE_TYPE_FORDB		base;

	char			strId[dID_LENGTH];

	WORD			wServerType;	//	서버 타입
	WORD			wResult;	//	0 실패,1 성공
};


////////////////////////////////////////////////////////////////////////
//	DB서버에서  채널 서버로 
//	0x4701 ~ 채널 서버가 링크 서버에게 
////////////////////////////////////////////////////////////////////////

struct DLMSG_GETAVATARS{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD						u_serial;
	DWORD						worldSerial;			//	world 서버의 유저 시리얼
	char						strID[dID_LENGTH];
	cAVATAR_INFO				aCharacter[dMAX_AVATAR_COUNT];
};

#define	dCREATE_AVATAR_RESULT_OK								0	//	성공
#define	dCREATE_AVATAR_RESULT_ALREADY_EXIST_NAME				1	//	이미 존재하는 이름이다.
#define	dCREATE_AVATAR_RESULT_COUNT_FULLED						2	//	아바타 수가 넘친다.
#define	dCREATE_AVATAR_RESULT_FAILED							3	//	알수 없는 이유로 실패
struct	DLMSG_CREATEAVATAR{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD						u_serial;
	DWORD						worldSerial;			//	world 서버의 유저 시리얼
	char						strID[dID_LENGTH];
	char						strName[dNAME_LENGTH];
	short						wResult;		//	결과
	WORD						wIndex;			//	몇번째 아바타에 추가되었는지...
	cAVATAR_INFO				aCharacter;		//	데이타도 보내준다. 
};


enum
{
	eDLMSG_SELECTAVATAR_SUCCESS,
	eDLMSG_SELECTAVATAR_FAIL,
	eDLMSG_EXIST_CONNECT_USER,
	eDLMSG_EXIST_SAVE_FAILED_DATA,
};

struct DLMSG_SELECTAVATAR{
	cMSG_BASE_TYPE_FORDB		base;

	WORD						wResult;
	char						strID[dID_LENGTH];
	char						strNAME[dNAME_LENGTH];
	DWORD						loginSerial;
	DWORD						worldSerial;
};

#define	dREMOVE_AVATAR_RESULT_FAILED							-1	//	실패
#define	dREMOVE_AVATAR_RESULT_AVATAR_NOT_EXIST					-2	//	존재하지 않는 아바타
struct	DLMSG_DELETEAVATAR{
	cMSG_BASE_TYPE_FORDB		base;

	char						strID[dID_LENGTH];
	char						strName[dNAME_LENGTH];
	DWORD						u_serial;
	DWORD						worldSerial;			//	world 서버의 유저 시리얼
	WORD						guildSerial;
	short						wResult;		//	결과
};
//┃
//┃Login <-> World <-> Avatar DB Server 
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Game <-> Avatar DB Server 
//┃
struct GDMSG_LOGINAVATAR		//	아바타가 게임서버로 들어왔으니 가지고 있는 데이타 주라. 없음 말구.. -_-;
{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD						u_serial;
	DWORD						worldSerial;
	WORD						badtime;
	WORD						isOper;
	WORD						isNetCafeUser;
	char						strId[dID_LENGTH];
	char						strName[dNAME_LENGTH];
	char						strIP[dIP_SIZE];
};

struct GDMSG_LOGOUTAVATAR
{
	cMSG_BASE_TYPE_FORDB			base;

	int								iActorSerialInServer;
	CPlayerSaveDataForServerDefine	pData;
};

struct GDMSG_KILLAVATAR
{
	cMSG_BASE_TYPE_FORDB			base;

	char							strId[dID_LENGTH];
};

struct DGMSG_LOGINAVATAR		//	가져가라 --;
{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD						u_serial;
	DWORD						worldSerial;
	WORD						badtime;
	WORD						Oper;
	WORD						wResult;
	WORD						isOper;
	WORD						isNetCafeUser;

	CPlayerSaveDataForServerDefine	pData;
};

enum
{
//★★★ DB저장 관련
//	DB에서 리턴됨
	eSAVE_SUCCESS_DB			=	1	,	//	성공
	eSAVE_CAN_NOT_FIND_DATA				,	//	찾을 수 업음
	eSAVE_FAIL_LITTLECOUNT				,	//	레벨이나 카운트가 잘못됨
	eSAVE_DB_FAILED						,	//	저장 실패
//	저장과정에서 리턴됨
	eSAVE_DB_UNKNOWN_ERROR				,	//	걍 실패 -o- 이유를 알수 없다.
//	중간에 데이트 체크하면서 걸림
	eSAVE_DB_INCORRECT_DATA				,
	eSAVE_DB_DATA_NULL					,

//★★★ Memory 저장 관련
	eSAVE_SUCCESS_SAVEFILE				,
	eSAVE_FAIL_SAVEFILE					,
	eSAVE_CAN_NOT_FIND_LOGIN_USER		,
	eSAVE_MEMORY_FAIL_LITTLECOUNT		,
	eSAVE_MEMORY_FAIL_LITTLECOUNT2		,	//	10 이상의 카운터 차이가 나올 때 
};

struct DGMSG_LOGOUTAVATAR
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD						wResult;
	DWORD						dwSaveCount;
	int							iActorSerialInServer;
	char						strName[dNAME_LENGTH];
};

#define dMAX_BUYLIST		48
typedef struct	
{
	DWORD	dwItemNo;
	DWORD	dwSqn;
	UTime	timeEndDate;
}tsBuyLst;

struct GDMSG_BUYLIST
{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD		dwSerialInServer;
	char		strId[dID_LENGTH];
}; 

//
//	Premium Item Use Type
enum
{
	ePIUT_WITHDRAW,		//	꺼냈다.
	ePIUT_REMOVE,		//	제거한다.
};

struct GDMSG_USEBUY
{
	cMSG_BASE_TYPE_FORDB		base;

	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
	char	strServerName[dWORLD_NAME_LENGTH];
	int		iLev;
	WORD	wJob;
	DWORD	dwUseSqnList;
	UTime	timeEndDate;

	DWORD	dwSerialInServer;
	WORD	wInventorySlot;
	WORD	wCartSlot;
	WORD	wType;
};

struct GDMSG_GET_CARROT_COUNT
{
	cMSG_BASE_TYPE_FORDB		base;

	char	strId[dID_LENGTH];
	DWORD	dwSerialInServer;

	UTime	time;
	char	strIP[dIP_SIZE];

#ifdef	_OGP_SERVICE
	cOGP_ServiceKey	ogpServiceKey;
#endif
};

struct GDMSG_USE_CARROT_LOG
{
	cMSG_BASE_TYPE_FORDB	base;

	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
	char	strServerName[dWORLD_NAME_LENGTH];
	DWORD	dwSerialInServer;
	WORD	lev;
	WORD	job;
	int		itemno;	//	구매한 아이템 번호
	WORD	itcnt;	//	구매한 아이템 갯수
	short	sPrice;	//	구매 가격
	
	WORD	wIsPublicItem;
	char	strIP[dIP_SIZE];
	char	strReceiverId[dID_LENGTH];

#ifdef	_OGP_SERVICE
	cOGP_ServiceKey	ogpServiceKey;
#endif
};


struct DGMSG_BUYLIST
{
	cMSG_BASE_TYPE_FORDB		base;

	char		strId[dID_LENGTH];

	DWORD		dwSerialInServer;
	WORD		wCount;
	tsBuyLst	List[dMAX_BUYLIST];
}; 

#define dRESULT_USEBUYITEM_SUCCESS			1
#define dRESULT_USEBUYITEM_USED				2	//	이미 사용한건데 -_-;
#define dRESULT_USEBUYITEM_NOTFIND			3	//	그런 아이템이 없다.
#define dRESULT_USEBUYITEM_FAIL				4	//	뭘까?
#define dRESULT_USEBUYITEM_DESTROY_SUCCESS	5	//	아이템 뽀개기 성공
#define dRESULT_USEBUYITEM_DESTROY_FAILED	6	//	아이템 뽀개기 실패

struct DGMSG_USEBUY
{
	cMSG_BASE_TYPE_FORDB		base;

	char		strId[dID_LENGTH];
	char		strName[dNAME_LENGTH];
	WORD		wResult;
	DWORD		dwAssentSqnList;
	UTime		timeEndDate;

	DWORD		dwSerialInServer;
	WORD		wInventorySlot;
	WORD		wCartSlot;

	WORD		wType;
};

#define	dMAX_PREMIUM_ITEM_COUNT	2048

struct DGMSG_PREMIUM_ITEM_PRICE_INFO
{
	cMSG_BASE_TYPE_FORDB	base;
	WORD					wCount;
	WORD					awPriceList[dMAX_PREMIUM_ITEM_COUNT];
};

enum
{
	eGCR_SUCCESS,		//	성공
	eGCR_NOTFOUNDUSER,	//	해당 유저를 찾을수 없다.
	eGCR_NOTAGREE,		//	동의하지 않은 유저
	eGCR_FAILED,		//	여튼 실패	
};

struct DGMSG_GET_CARROT_COUNT_RESULT
{
	cMSG_BASE_TYPE_FORDB	base;

	char	strId[dID_LENGTH];
	DWORD	dwSerialInServer;

	WORD	wResult;
	int		iRemainCarrotCount;
};

enum
{
	eUCR_SUCCESS			,	//	성공
	eUCR_NOTFOUNDUSER		,	//	해당 유저를 찾을수 없다.
	eUCR_LOW_COUNT			,	//	당근 갯수가 모잘라
	eUCR_AGREEMENT_FAILED	,	//	웹 에서 약관 승인 안함
	eUCR_FAILED				,	//	여튼 실패	
};

struct DGMSG_USE_CARROT_RESULT
{
	cMSG_BASE_TYPE_FORDB	base;

	char	strId[dID_LENGTH];
	char	strName[dID_LENGTH];
	DWORD	dwSerialInServer;

	WORD	wResult;

	int		itemno;	//	구매한 아이템 번호
	WORD	itcnt;	//	구매한 아이템 갯수
	int		crcnt;	//	남은 당근수
	WORD	wPrice;	//	가격
};


struct	GDMSG_PROVISION_RANK_REQ
{
	cMSG_BASE_TYPE_FORDB	base;
};

#define dMAX_PROVRANK_COUNT	200
typedef struct
{
	WORD		wRank;
	char		strName[dNAME_LENGTH];
}stProvRankField;

struct	DGMSG_PROVISION_RANK_RET
{
	cMSG_BASE_TYPE_FORDB	base;

	WORD		wCount;
	
	stProvRankField	list[dMAX_PROVRANK_COUNT];	//	wCount에 따라 사이즈를 조절해서 보낸다.
};

struct	GDMSG_PERSONALDATA
{
	cMSG_BASE_TYPE_FORDB	base;

	WORD	wSerial;				//	BC서버에 등록된 게임서버의 시리얼
	WORD	wIndex;					//	BC서버의 고유 인덱스
	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
};

struct GDMSG_SAVE_PERSONALDATA
{
	cMSG_BASE_TYPE_FORDB	base;

	WORD	wIndex;					//	BC서버의 고유 인덱스
	int		iDBIndex;				//	Avatar 의 DB 고유 인덱스
	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
	cUserCustomDataDefine	data;
};


struct	DGMSG_PERSONALDATA
{
	cMSG_BASE_TYPE_FORDB	base;

	WORD	wSerial;				//	BC서버에 등록된 게임서버의 시리얼
	WORD	wIndex;					//	BC서버의 고유 인덱스
	int		iDBIndex;				//	Avatar 의 DB 고유 인덱스
	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
	cUserCustomDataDefine	data;
};


struct GDMSG_STAT_GOLD
{
	cMSG_BASE_TYPE_FORDB	base;

	char	strServer[dWORLD_NAME_LENGTH];
	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
	LONGLONG	llGoldAll;
	LONGLONG	llGettingGold;
};

struct G2D_BUY_PREMIUM_ITEM_TO_WEB_DB
{
	cMSG_BASE_TYPE_FORDB	base;

	char	strServer[dWORLD_NAME_LENGTH];
	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];

	int		iItemIndex;
	int		iPrice;
	int		iItemCount;

	WORD	wPayMethod;	//	당근 상점용이니 100으로 고정한다.. 100이 뭐지? ;;

	char	strReceiverName[dNAME_LENGTH];

	int		iLev;
	WORD	wJob;

	DWORD	dwSerialInServer;
};

//┃
//┃Game <-> Avatar DB Server 
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃World <-> Avatar DB Server 
//┃
struct WDMSG_SAVEAVA_TO_FILE
{
	cMSG_BASE_TYPE_FORDB			base;

	int								iSerialInServer;
	WORD							wIsWantResult;	//	0 이면 답멜-_-이 필요 없는거고 1이면 필요한거..
	CPlayerSaveDataForServerDefine	pData;
};

struct WDMSG_BANKOPEN
{
	cMSG_BASE_TYPE_FORDB		base;

	int							iActorSerial;
	char						strID[dID_LENGTH];
};

struct WDMSG_SAVEBANK
{
	cMSG_BASE_TYPE_FORDB		base;

	int							iActorSerial;
	char						strID[dID_LENGTH];

	CBankInfo					cBank;
};

struct WDMSG_UPDATESAVE
{
	cMSG_BASE_TYPE_FORDB		base;

	char						strId[dID_LENGTH];
};

struct WDMSG_KILL_AVATAR
{
	cMSG_BASE_TYPE_FORDB		base;

	char						strId[dID_LENGTH];
};


#define	dSAVE_SUCCESS_DB			1
#define	dSAVE_SUCCESS_SAVEFILE		2
#define	dSAVE_FAIL_SAVEFILE			3
#define	dSAVE_FAIL_LITTLECOUNT		4

struct DWMSG_SAVEAVA_TO_FILE
{
	cMSG_BASE_TYPE_FORDB		base;

	int							iSerialInServer;
	char						strName[dNAME_LENGTH];
	WORD						wResult;
};


struct DWMSG_BANKOPEN
{
	cMSG_BASE_TYPE_FORDB		base;

	int							iActorSerial;
	CBankInfo					cBank;
};

//
//	Result of  Bank Data Save
enum
{
	eRBDS_SUCCESS,
	eRBDS_INCORRECT_COUNTER,
	eRBDS_FAILED,
	eRBDS_ID_NOT_MISMATCH,
};

struct DWMSG_SAVEBANK
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD						wResult;
	int							iActorSerial;
	char						strID[dID_LENGTH];
};

struct DWMSG_DUEL_RECORD
{
	cMSG_BASE_TYPE_FORDB		base;

	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
	cDuelRecord	record;
};

enum
{
	eTRAN_AVATAR_TO_GVG_RESULT_SUCCESS,
	eTRAN_AVATAR_TO_GVG_RESULT_FAILED,
	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_LOGINSERVER_NOT_ACTIVE,
	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_DBCSERVER_NOT_ACTIVE,
	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_CANNOT_FIND_GVG_SERVER,
	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_CREATE_AVATAR,
	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_SAVE_AVATAR,
	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_PERIOD,
	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_WAIT_FOR_RESULT,
	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_NOT_OWNED_GUILD,
	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_ALREADY_EXIST_DIFF_AVATAR,
};

enum
{
	eTRAN_GUILD_TO_GVG_RESULT_SUCCESS,
	eTRAN_GUILD_TO_GVG_RESULT_FAILED,
	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_LOGINSERVER_NOT_ACTIVE,
	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_DBCSERVER_NOT_ACTIVE,
	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_CANNOT_FIND_GVG_SERVER,
	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_CREATE_GUILD,
	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_SAVE_GUILD,
	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_PERIOD,
	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_WAIT_FOR_RESULT,
	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_MISMATCH_LEVEL,
	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_WAIT_ANOTHER_WORK,
	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_CAN_NOT_FIND_MASTER_NAME,
	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_GUILD_NOT_EXIST,
};

enum
{
	eGET_GVG_AVATAR_ORG_DATA_RESULT_SUCCESS,
	eGET_GVG_AVATAR_ORG_DATA_RESULT_SUCCESS_BY_IS_MOVEFIELD,
	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED,
	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_ORG_AVATAR,
	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_AVATAR,
	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_LOGINSERVER,
	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_WORLDSERVER,
	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_DBCSERVER,
};


struct DWMSG_TRANS_AVATAR_TO_GVG_RESULT
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD	wResult;
	WORD	wWorldServerIndex;
	char	strName[dNAME_LENGTH];
	char	strNewName[dNAME_LENGTH];
};

//	은행
//////////////////////////////////////////////////

//┃
//┃World <-> Avatar DB Server 
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃about GUILD
struct WDMSG_GETGUILD
{
	cMSG_BASE_TYPE_FORDB		base;
};

struct DWMSG_GETGUILD
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD						wCount;
	WORD						wAllCount;
	GuildListDB					List[dGUILD_COUNT_FORPACKET];	//	길드 인덱스 순서대로 보내준다.
};

struct GuildMemberDB
{
	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
	WORD	wRank;
	WORD	wJob;
	WORD	wLevel;
	DWORD	dwBestowValue;
};

struct DWMSG_GUILD_MEMBER
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD						wIsEnd;
	WORD						wCount;
	WORD						wSerial;
	GuildMemberDB				members[dGUILD_MAX_PLAYER];
};

struct WDMSG_CREATEGUILD
{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD		dwGameServerSerial;
	WORD		wIndex;
	char		strName[dGUILD_NAME_LENGTH];
	char		strMasterId[dID_LENGTH];
	char		strMasterName[dNAME_LENGTH];
	char		strSubMasterName[dNAME_LENGTH];
	WORD		wMasterJob;
	WORD		wMasterLevel;
};

struct WDMSG_DELETEGUILD
{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD		dwGameServerSerial;
	WORD		wIndex;
	char		strName[dGUILD_NAME_LENGTH];
	char		strRequestMan[dNAME_LENGTH];
	WORD		wReason;
};

struct WDMSG_JOINGUILD
{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD		dwGameServerSerial;
	WORD		wIndex;
	char		strName[dGUILD_NAME_LENGTH];
	char		strMaster[dNAME_LENGTH];
	char		strTargetId[dID_LENGTH];
	char		strTargetName[dNAME_LENGTH];
	WORD		wRank;
	WORD		wJob;
	WORD		wLevel;
};

struct WDMSG_EXITGUILD
{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD		dwGameServerSerial;
	WORD		wIndex;
	char		strName[dGUILD_NAME_LENGTH];
	char		strMaster[dNAME_LENGTH];
	char		strTargetName[dNAME_LENGTH];
};

struct WDMSG_SAVEGUILDBASE
{
	cMSG_BASE_TYPE_FORDB		base;

	CGuildSaveInfoForColumn		dataColumn;
	CGuildSaveInfoForBinary		dataBinary;
	CGuildSaveInfoMark			dataMark;
	CGuildSaveInfoBank			dataBank;
	CGuildSaveInfoRelShip		dataRelShip;
};

struct	WDMSG_SAVEGUILD_GUARDIAN
{
	cMSG_BASE_TYPE_FORDB		base;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
	char		strName[dGUILD_NAME_LENGTH];
	
	CGuildPetInfo	m_aGuildGuardian[dGUILD_GUARDIAN_TYPE_COUNT];
};

struct	WDMSG_SAVEGUILD_INVENTORY
{
	cMSG_BASE_TYPE_FORDB		base;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
	char		strName[dGUILD_NAME_LENGTH];
	CItemDefine		m_aInventory[c_iGuildInventorySize];
};

struct	WDMSG_SAVEGUILD_MARK
{
	cMSG_BASE_TYPE_FORDB		base;
	DWORD			dwGameServerSerial;
	WORD			wIndex;
	char			strName[dGUILD_NAME_LENGTH];
	CGuildMarkInfo	m_guildMarkInfo;
};

struct	WDMSG_SAVEGUILD_STRUCTURE
{
	cMSG_BASE_TYPE_FORDB		base;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
	char		strName[dGUILD_NAME_LENGTH];
	cGuildStructureInfo	m_aGuildStructure[dMAX_GUILD_STRUCTURE_COUNT];
};

struct	WDMSG_SAVEGUILD_PET
{
	cMSG_BASE_TYPE_FORDB		base;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
	char		strName[dGUILD_NAME_LENGTH];
	CGuildPetInfo	m_aGuildPet[dGUILD_PET_TYPE_COUNT];
};

/*
struct	WDMSG_SAVEGUILD_POLL
{
	cMSG_BASE_TYPE_FORDB		base;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
	char		strName[dGUILD_NAME_LENGTH];
	CGuildPollInfo	m_guildPoll;
};

struct	WDMSG_SAVEGUILD_SELLINGITEM
{
	cMSG_BASE_TYPE_FORDB		base;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
	char		strName[dGUILD_NAME_LENGTH];
	CSellingAgentItem	m_aSellingAgentItem[dMAX_GUILD_SELLING_AGENT_ITEM_COUNT];	//	최대 판매대행 아이템 수
};
*/
struct	WDMSG_SAVEGUILD_SKILL
{
	cMSG_BASE_TYPE_FORDB		base;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
	WORD		wGuildPoint;
	char		strName[dGUILD_NAME_LENGTH];
	WORD		m_aGuildSkill[dMAX_GUILD_SKILL_COUNT];	//	길드 스킬
};

class	WDMSG_MT_TIME_START
{
public:
	cMSG_BASE_TYPE_FORDB		base;

	WDMSG_MT_TIME_START()
	{
		base.wSize = sizeof(this);
		base.wType = dWDMSG_MT_TIME_END;
	}
};

class	WDMSG_MT_TIME_END
{
public:
	cMSG_BASE_TYPE_FORDB		base;

	WDMSG_MT_TIME_END()
	{
		base.wSize = sizeof(this);
		base.wType = dWDMSG_MT_TIME_END;
	}
};

//	결투 랭킹 계산 요청
struct	WDMSG_ASK_COMPUTE_DUEL_RANKING
{
	cMSG_BASE_TYPE_FORDB		base;
};

//	결투 랭킹 정보 요청
struct	WDMSG_ASK_DUEL_RANKING_INFO
{
	cMSG_BASE_TYPE_FORDB		base;
};

//	결투 전적 요청
struct WDMSG_ASK_DUEL_RECORD
{
	cMSG_BASE_TYPE_FORDB		base;

	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
};

struct WDMSG_TRANS_AVATAR_TO_GVG
{
	cMSG_BASE_TYPE_FORDB		base;

	CPlayerSaveDataForServerDefine	avatarData;
	int								iWorldIndex;
};

struct	WDMSG_TRANS_GUILD_TO_GVG
{
	cMSG_BASE_TYPE_FORDB		base;

	CGuildBaseInfo					guildData;
	char							strMasterName[dNAME_LENGTH];	//	길드 마스터 이름
	char							strMasterId[dID_LENGTH];
	WORD							wMasterLevel;
	WORD							wMasterJob;
	int								iWorldIndex;
	int								iEmptyGuildIndex;
};

#define	dGUILD_APPONT							1
#define	dGUILD_DISMISSAL						2
#define dGUILD_CHANGERANK_MASTER_APPONT_SPECIAL	3	//	마스터가 캐릭터를 지울때만 허용된다.

struct	WDMSG_CHANGERANK
{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD		dwGameServerSerial;		//	유저의 시리얼
	WORD		wIndex;					//	길드의 시리얼
	char		strName[dGUILD_NAME_LENGTH];//	길드이름
	char		strMaster[dNAME_LENGTH];//	게임 서버에서 보내준것. 그래도 돌려보낼 것.
	char		strTarget[dNAME_LENGTH];//	해당 유저이름
	WORD		wRank;					//	어떤 rank로 수정할 것인가
	WORD		wType;
	WORD		wRank2;
	WORD		wReason;
//	WORD		wRank3;
};

struct	WDMSG_DELETEGUILDER
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD		wIndex;
	char		strName[dGUILD_NAME_LENGTH];
	char		strTarget[dNAME_LENGTH];
};

#define dCREATEGUILD_RESULT_SUCCESS		0x01
#define dCREATEGUILD_RESULT_FULL		0x02		//	더이상 만들수 없습니다.
#define dCREATEGUILD_RESULT_SAMENAME	0x03		//	같은 이름의 길드가 존재한다.
#define dCREATEGUILD_RESULT_HAVEGUILD	0x04		//	마스터가 다른 길드가 있잖아 -_-a
#define	dCREATEGUILD_RESULT_FAIL		0x05		//	다른 이유로 실패

struct DWMSG_CREATEGUILD
{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD	dwGameServerSerial;
	WORD	wResult;
	WORD	wGuildSerial;
	char	strMasterId[dID_LENGTH];
	char	strMasterName[dNAME_LENGTH];
	char	strSubMasterName[dNAME_LENGTH];
	char	strGuildName[dGUILD_NAME_LENGTH];
	WORD	wMasterJob;
	WORD	wMasterLevel;
};

#define dDELETEGUILD_RESULT_SUCCESS		0x01
#define dDELETEGUILD_RESULT_NOTFIND		0x03		//	그런 길드는 없는데 .. ;
#define	dDELETEGUILD_RESULT_FAIL		0x02		//	다른 이유로 실패

struct DWMSG_DELETEGUILD
{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD		dwGameServerSerial;
	WORD		wResult;
	WORD		wGuildSerial;
	char		strName[dGUILD_NAME_LENGTH];
	char		strRequestMan[dNAME_LENGTH];
};

#define dJOINGUILD_RESULT_SUCCESS		0x01
#define dJOINGUILD_RESULT_NOTFIND		0x02	//	그런 길드는 없는데 .. ;
#define	dJOINGUILD_RESULT_FULL			0x03	//	푸 ~~ 울
#define	dJOINGUILD_RESULT_JOINED		0x04	//	이미 다른 길드에 조인되어 있다.
#define dJOINGUILD_RESULT_NEEDRANK		0x05
#define dJOINGUILD_RESULT_FAIL			0x06	//	뭘까~~~

struct DWMSG_JOINGUILD
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD		wResult;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
	WORD		wRank;
	char		strName[dGUILD_NAME_LENGTH];
	char		strMaster[dNAME_LENGTH];
	char		strTargetName[dNAME_LENGTH];
};

#define dEXITGUILD_RESULT_SUCCESS		0x01
#define dEXITGUILD_RESULT_NOTFIND		0x02	//	그런 길드는 없는데 .. ;
#define dEXITGUILD_RESULT_NOTJOIN		0x03	//	넌 길드 없스 ~~
#define dEXITGUILD_RESULT_NOTACOUNT		0x04	//	정보가 일치하지 않습니다.
#define dEXITGUILD_RESULT_FAIL			0x05	//	뭘까~~~
#define dEXITGUILD_RESULT_NEEDRANK		0x06	//	뭘까~~~
#define dEXITGUILD_RESULT_FAILED_BY_TARGET_IS_GUILD_MASTER	0x07	//	뭘까~~~
#define dEXITGUILD_RESULT_FAILED_BY_TARGET_IS_ELDER			0x08
#define dEXITGUILD_RESULT_FAILED_BY_IT_IS_ABLE_ONLY_MASTER	0x09

struct DWMSG_EXITGUILD
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD		wResult;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
	char		strName[dGUILD_NAME_LENGTH];
	char		strMaster[dNAME_LENGTH];
	char		strTargetName[dNAME_LENGTH];
};

#define dCHANGERANK_RESULT_SUCCESS		0x01
#define dCHANGERANK_RESULT_NOTFIND		0x02	//	그런 길드는 없는데 .. ;
#define dCHANGERANK_RESULT_NOTACOUNT	0x03	//	정보가 일치하지 않습니다.
#define dCHANGERANK_RESULT_FAIL			0x04	//	뭘까~~~

struct DWMSG_CHANGERANK
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD		wResult;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
	WORD		wType;
	char		strMaster[dNAME_LENGTH];
	char		strTarget[dNAME_LENGTH];
	WORD		wRank;
	WORD		wRank2;
	WORD		wRank3;
};

struct DWMSG_SAVEGUILD_GUARDIAN
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD		wResult;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
};

struct DWMSG_SAVEGUILD_INVENTORY
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD		wResult;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
};

struct DWMSG_SAVEGUILD_MARK
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD		wResult;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
};

struct DWMSG_SAVEGUILD_OBJECT
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD		wResult;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
};

struct DWMSG_SAVEGUILD_PET
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD		wResult;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
};

struct DWMSG_SAVEGUILD_POLL
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD		wResult;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
};

struct DWMSG_SAVEGUILD_SELLINGITEM
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD		wResult;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
};

struct DWMSG_SAVEGUILD_SKILL
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD		wResult;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
};

//┃about GUILD
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//	Admin 툴과 관련된 것들..
struct LDMSG_DELETE_LIST
{
	cMSG_BASE_TYPE_FORDB		base;

	char		strId[dID_LENGTH];
};

struct LDMSG_UPDATESAVE_USER
{
	cMSG_BASE_TYPE_FORDB		base;

	char		strId[dID_LENGTH];
};

struct LDMSG_UPDATESAVE
{
	cMSG_BASE_TYPE_FORDB		base;
};

struct LDMSG_GET_DATA
{
	cMSG_BASE_TYPE_FORDB		base;

	char	strId[dID_LENGTH];
};

struct LDMSG_SAVE_DATA
{
	cMSG_BASE_TYPE_FORDB		base;

	CPlayerSaveDataForServerDefine				pData;
};

struct LDMSG_SET_ADMIN
{
	cMSG_BASE_TYPE_FORDB		base;

	char	strId[dID_LENGTH];
	WORD	wLevel;
};

struct DLMSG_DELETE_LIST
{
	cMSG_BASE_TYPE_FORDB		base;

	char		strId[dID_LENGTH];	//	누구 리스트인지만 알려주면 되지뭐 ;;
};

struct DLMSG_UPDATESAVE_USER
{
	cMSG_BASE_TYPE_FORDB		base;

	char		strId[dID_LENGTH];
	WORD		wResult;
};

struct DLMSG_UPDATESAVE
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD		wSuccCount;
	WORD		wFailCount;
};

struct DLMSG_GET_DATA
{
	cMSG_BASE_TYPE_FORDB		base;

	CPlayerSaveDataForServerDefine				pData;
};

struct WDMSG_ASK_EVENTS_INFO
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD						isReally;
};

/////////////////////////////////////////////////////
//	EVENT - Gold Rush
struct DWMSG_GR_INFO_RESULT
{
	cMSG_BASE_TYPE_FORDB		base;

	UTime	begin,end;
	WORD	wBoostExp,wDropChance;

	char	strBeginComment[256];
	char	strEndComment[256];
};

/////////////////////////////////////////////////////
//	EVENT - ITEMPACK 
struct DWMSG_ITEMPACK_RESULT
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD			wSerial;
	cItemPackInfo	info;
};

struct DWMSG_DUEL_RANKING_INFO
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD	wCount;	//	몇개냐?

	char	astrName[100][dNAME_LENGTH];
	int		aiPoint[100];
};

//	PayLetter와의 작업 내용들.
#ifdef _FOR_THAI


#define dREQ_GETCASHPOINT		10	//	잔여 캐쉬 포인트 얻어오기
#define dREQ_BUYITEM			20	//	구매하기
//	그외 선물하기와 취소가 있지만 ... 사용 안한다.

typedef struct _BILL_PACK
{
	cMSG_BASE_TYPE_FORDB	base;
	DWORD	dwReqKey;
	char	szUserID[50+1];
	WORD	wPresentFlag;
	char    szActorName[50+1];
	DWORD	dwClientIP;
	WORD	wClientPort;
	char    szServerName[50+1]; 
    DWORD   dwTotalCash;
	DWORD	dwReqChargeCashAmt;
	DWORD	dwChargedCashAmt;
	__int64	I64ChargeNo;
    WORD    wItemCategory;
	DWORD	dwItemID;
	WORD	wItemCnt;
	DWORD	dwItemUnitPrice;
	WORD	wItemType;
	char	szItemName[50+1];
	char	szRetCode[4+1];
}BILL_PACK, *PBILL_PACK;

#endif

#pragma pack()

#endif	//	_classPACKET_DBSERVER_H
