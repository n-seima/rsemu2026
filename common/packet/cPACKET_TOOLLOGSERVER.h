#ifndef _classPACKET_TOOLLOGSERVER_H
#define _classPACKET_TOOLLOGSERVER_H

#include "RSComm.h"
#include "cItemDefine.h"

#pragma pack(1)

#define dIP_LENGTH				16
#define dPACKAGE_MAXSIZE		5120

class	cMSG_BASE_TYPE_FORTOOLLOG
{
public:
	WORD	wSize,				//	패킷 사이즈
			wType;				//	패킷 타입
	inline	void	set(WORD size,WORD type)	{wSize	=	size,wType	=	type;}
};

enum
{
	ePACKET_COMMON		=	0x1001	,	//	파일에 저장되는 내용이다.
	ePACKET_SEARCH					,	//	어드민툴에서 검색을 요청하다.

	ePACKET_LOGIN		=	0x2001	,	//	어드민툴 로그인 하기
	ePACKET_SENDNOTICE				,	//	공지 날리기 
	ePACKET_KILLWORLD				,	//	월드 죽이기
	ePACKET_LOGINSERVER				,	//	로그인 서버 죽이기/살리기

	ePACKET_SETBADUSER	=	0x3001	,	//	불량 유저 설정
	ePACKET_DELID					,	//	ID 삭제
	ePACKET_DELAVA					,	//	캐릭터 삭제
	ePACKET_RECOVERID				,	//	계정복구
	ePACKET_RECOVERAVA				,	//	캐릭터 복구 
	ePACKET_CHANGENAME				,	//	캐릭터 이름 변경 
	ePACKET_SETOPERATER				,	//	운영자 설정하기

	ePACKET_DATASAVE	=	0x4001	,	//	불량 유저 설정
	ePACKET_CHANGELEVEL				,	//	래벨 변경
	ePACKET_CHANGELASTVILLAGE		,	//	마지막 마을 변경 
	ePACKET_CHANGELASTFIELD			,	//	마지막 필드 변경 

	ePACKET_INSERTITEMTOBANK		,	//	은행에 아이템 삽입
	ePACKET_INSERTITEM				,	//	인벤토리에 아이템 저장
	ePACKET_REMOVEITEM				,	//	인벤토리에서 아이템 삭제
	ePACKET_ADDEXP					,	//	경험치 추가

	ePACKET_ADDGOLD					,	//	골드 추가
	ePACKET_ADDGOLDTOBANK			,	//	은행에 골드 추가
	ePACKET_REMOVEGOLDTOBANK		,	//	은행에서 골드 삭제
	ePACKET_REMOVEGOLD				,	//	캐릭터에서 골드 삭제
	ePACKET_REMOVEITEM_BUTTON		,	//	아이템 제거.. (버튼 사용)

	ePACKET_RECOVERITEM_BUTTON		,	//	아이템 복구 .. (버튼 사용)
	ePACKET_RESETSKILLPOINT			,	//	스킬 리셋
	ePACKET_RESETSTATPOINT			,	//	스텟 리셋
	ePACKET_MOVETO					,	//	마지막 마을 혹은 감옥 혹은 입구로 이동 
	ePACKET_DATACOPY				,	//	파일로 저장한걸 특정 캐릭터에 복사한다.

	ePACKET_REMOVEITEM_FROM_BANK	,	//	은행에서 아이템 삭제
	ePACKET_SAVE_BANK_DATA			,	//	은행 데이터 저장
	ePACKET_FAILED_WORK				,	//	로그를 보낸후에 무슨 작업을 했는데, 그 작업이 실패 했다.(ex 세이브 로그 보내고 세이브 했는데 무슨 이유로 세이브 실패!!)

	ePACKET_RENEW_PLAYER_ITEM		,	//	플레이어 아이템 기간 연장
	ePACKET_RENEW_BANK_ITEM			,	//	은행 아이템 기간 연장
	ePACKET_WORK_MESSAGE			,
	ePACKET_LOAD_PLAYER_DATA		,	//	계정 혹은 플레이어 데이터를 로딩 했다.

	ePACKET_CHANGE_AVATAR_INDEX		,	//	아바타 인덱스를 바꿈
	ePACKET_CHANGE_CURRENT_FIELD	,	//	현재 필드를 바꿈
	ePACKET_CHANGE_SECRET_DUNGEON_PLAY_COUNT,	//	비밀 던젼 플레이 수를 바꿈
	ePACKET_CHANGE_EXTRA_INVENTORY_SIZE,	//	인벤토리 사이즈를 바꿈
	ePACKET_CHANGE_PENELTY_TIME,		//	페널티 시간을 바꿈
	ePACKET_CHANGE_DEATH_PENELTY_TIME,	//	죽음 페널티 시간을 바꿈
	ePACKET_CHANGE_OPERATOR_SHAPE	,	//	죽음 페널티 시간을 바꿈
	
	ePACKET_CHANGE_GUILD_DUNGEON_PLAYTIME	,	//	죽음 페널티 시간을 바꿈
};

struct PACKET_COMMON
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;	//	size = 28 + msg size

	char		strId[dID_LENGTH];
	DWORD		dwtime;				//	마지막 패킷은 0xffffffff
	char		strMsg[512];
};

struct PACKET_SEARCH
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;

	WORD		_yy,_mm,_dd,_hh;
	WORD		yy,mm,dd,hh;
	char		strId[dID_LENGTH];
	char		strIp[dIP_LENGTH];
};

///////////////////////////////////////////////////
//	FOR LOGIN & START PAGE
struct PACKET_LOGIN
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;

	char		strId[dID_LENGTH];
	char		strIp[dIP_LENGTH];
};

struct PACKET_SENDNOTICE				
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;

	char		strId[dID_LENGTH];
	char		strMsg[dCHAT_LENGTH];
	char		strServerName[32];
};

struct PACKET_KILLWORLD				
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;

	char		strId[dID_LENGTH];
	char		strServerName[32];
};

struct PACKET_LOGINSERVER				
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;

	char		strId[dID_LENGTH];
	WORD		wKill;				//	 0 - Open , 1 - Kill
};
//	FOR LOGIN & START PAGE
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//	FOR SETTING USER & AVATAR 
struct PACKET_SETBADUSER	
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	DWORD		dwBlockDate;
	char		strWhy[dCHAT_LENGTH];
};

struct PACKET_DELID					
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
};

struct PACKET_DELAVA					
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
};

struct PACKET_RECOVERID				
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strOldUserId[dID_LENGTH];
	char		strNewUserId[dID_LENGTH];
};

struct PACKET_RECOVERAVA				
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strOldUserName[dNAME_LENGTH];
	char		strNewUserName[dNAME_LENGTH];
	char		strServerName[32];
};

struct PACKET_CHANGENAME				
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strOldUserName[dNAME_LENGTH];
	char		strNewUserName[dNAME_LENGTH];
	char		strServerName[32];
};

struct PACKET_SETOPERATER				
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];
	
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
	WORD		wOper;
};
//	FOR SETTING USER & AVATAR 
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//	FOR AVA DATA
struct PACKET_DATASAVE	
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
};

struct PACKET_WORK_MESSAGE
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
	char		strMessage[256];
};

struct PACKET_CHANGELEVEL
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];
	
	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
	WORD		wOldLev,wNewLev;
};

struct PACKET_CHANGELASTVILLAGE
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
	WORD		wOldVillage,wNewVillage;
};

struct PACKET_CHANGELASTFIELD
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
	WORD		wOldField,wNewField;
};

struct PACKET_INSERTITEMTOBANK
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
	CItemDefine	item;

	char		strMsg[256];
};

struct PACKET_INSERTITEM
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];
	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
	CItemDefine	item;
	
	char		strMsg[256];
};

struct PACKET_REMOVEITEM
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];
	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
	CItemDefine	item;
	char		strMsg[256];
};

struct PACKET_REMOVEITEM_FROM_BANK
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];
	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
	CItemDefine	item;
	char		strMsg[256];
};

struct PACKET_SAVE_BANK_DATA
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];
	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
};

struct PACKET_ADDEXP					
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];
	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
	int			iAddExp;
};


struct PACKET_ADDGOLD					
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
	DWORD		dwAddGold;
};

struct PACKET_ADDGOLDTOBANK			
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
	DWORD		dwAddGold;
};

struct PACKET_REMOVEGOLDTOBANK		
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];
	
	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
	DWORD		dwRemoveGold;
};

struct PACKET_REMOVEGOLD				
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
	DWORD		dwRemoveGold;
};

struct PACKET_REMOVEITEM_BUTTON		
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];
};

struct PACKET_RECOVERITEM_BUTTON		
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];
};

struct PACKET_RESETSKILLPOINT			
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
};

struct PACKET_RESETSTATPOINT			
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
};

struct PACKET_MOVETO					
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
	WORD		wPoint;	//	0 - 마을로, 1 - prison , 2 - 입구로...
};

struct PACKET_DATACOPY
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strSourceId[dID_LENGTH];
	char		strSourceName[dNAME_LENGTH];
	char		strTargetId[dID_LENGTH];
	char		strTargetName[dNAME_LENGTH];
	char		strServerName[32];
};

struct PACKET_CHANGE_AVATAR_INDEX
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];

	WORD		wOld,wNew;
};

struct PACKET_CHANGE_CURRENT_FIELD
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];

	WORD		wOld,wNew;
};

struct PACKET_CHANGE_SECRET_DUNGEON_PLAY_COUNT
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];

	WORD		wOld,wNew;
};
struct PACKET_CHANGE_EXTRA_INVENTORY_SIZE
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];

	WORD		wOld,wNew;
};
struct PACKET_CHANGE_PENELTY_TIME
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];

	WORD		wOld,wNew;
};
struct PACKET_CHANGE_DEATH_PENELTY_TIME
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];

	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];

	WORD		wOld,wNew;
};

struct PACKET_FAILED_WORK
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];
};

struct PACKET_RENEW_PLAYER_ITEM
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];
	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
	CItemDefine	item;
	WORD		wDay;
	WORD		wHour;
	WORD		wMinute;
};

struct PACKET_RENEW_BANK_ITEM
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];
	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
	CItemDefine	item;
	WORD		wDay;
	WORD		wHour;
	WORD		wMinute;
};

struct	PACKET_LOAD_PLAYER_DATA
{
	cMSG_BASE_TYPE_FORTOOLLOG	base;
	
	char		strId[dID_LENGTH];
	char		strUserId[dID_LENGTH];
	char		strUserName[dNAME_LENGTH];
	char		strServerName[32];
};

//	FOR AVA DATA
///////////////////////////////////////////////////



union	ALL_TOOLLOGMSG
{
	cMSG_BASE_TYPE_FORTOOLLOG		base;

	PACKET_COMMON					mPACKET_COMMON;	//	어드민툴에서 검색결과로 돌려준다.
	PACKET_SEARCH					mPACKET_SEARCH;//	어드민툴에서 검색을 요청한다.

	PACKET_LOGIN					mPACKET_LOGIN;	//	어드민툴 로그인 하기
	PACKET_SENDNOTICE				mPACKET_SENDNOTICE;	//	공지 날리기 
	PACKET_KILLWORLD				mPACKET_KILLWORLD;	//	월드 죽이기
	PACKET_LOGINSERVER				mPACKET_LOGINSERVER;	//	로그인 서버 죽이기/살리기

	PACKET_SETBADUSER				mPACKET_SETBADUSER;	//	불량 유저 설정
	PACKET_DELID					mPACKET_DELID;	//	ID 삭제
	PACKET_DELAVA					mPACKET_DELAVA;	//	캐릭터 삭제
	PACKET_RECOVERID				mPACKET_RECOVERID;	//	계정복구
	PACKET_RECOVERAVA				mPACKET_RECOVERAVA;	//	캐릭터 복구 
	PACKET_CHANGENAME				mPACKET_CHANGENAME;	//	캐릭터 이름 변경 
	PACKET_SETOPERATER				mPACKET_SETOPERATER;	//	운영자 설정하기

	PACKET_DATASAVE					mPACKET_DATASAVE;	//	불량 유저 설정
	PACKET_CHANGELEVEL				mPACKET_CHANGELEVEL;	//	래벨 변경
	PACKET_CHANGELASTVILLAGE		mPACKET_CHANGELASTVILLAGE;	//	마지막 마을 변경 
	PACKET_CHANGELASTFIELD			mPACKET_CHANGELASTFIELD;	//	마지막 필드 변경 

	PACKET_INSERTITEMTOBANK			mPACKET_INSERTITEMTOBANK;	//	은행에 아이템 삽입
	PACKET_INSERTITEM				mPACKET_INSERTITEM;	//	인벤토리에 아이템 저장
	PACKET_REMOVEITEM				mPACKET_REMOVEITEM;	//	인벤토리나 은행에서 아이템 삭제
	PACKET_ADDEXP					mPACKET_ADDEXP;	//	경험치 추가

	PACKET_ADDGOLD					mPACKET_ADDGOLD;	//	골드 추가
	PACKET_ADDGOLDTOBANK			mPACKET_ADDGOLDTOBANK;	//	은행에 골드 추가
	PACKET_REMOVEGOLDTOBANK			mPACKET_REMOVEGOLDTOBANK;	//	은행에서 골드 삭제
	PACKET_REMOVEGOLD				mPACKET_REMOVEGOLD;	//	캐릭터에서 골드 삭제
	PACKET_REMOVEITEM_BUTTON		mPACKET_REMOVEITEM_BUTTON;	//	아이템 제거.. (버튼 사용)

	PACKET_RECOVERITEM_BUTTON		mPACKET_RECOVERITEM_BUTTON;	//	아이템 복구 .. (버튼 사용)
	PACKET_RESETSKILLPOINT			mPACKET_RESETSKILLPOINT;	//	스킬 리셋
	PACKET_RESETSTATPOINT			mPACKET_RESETSTATPOINT;	//	스텟 리셋
	PACKET_MOVETO					mPACKET_MOVETO;	//	마지막 마을 혹은 감옥 혹은 입구로 이동 
};

#define	dMAX_MSG_COUNT		10000

class cTOOLLOG_QUE
{
	ALL_TOOLLOGMSG	m_aPacket[dMAX_MSG_COUNT];		//	패킷들..
	int			m_iFirstQue,m_iLastQue,m_iCurrentQue,m_iCount,m_iCounter;		//	첫번째,마지막 패킷, 쌓여 있는패킷 수
	CRITICAL_SECTION	cs;

public:
					cTOOLLOG_QUE()	{	InitializeCriticalSection(&cs);reset();	}
					virtual ~cTOOLLOG_QUE()	{DeleteCriticalSection(&cs);	}
	void			reset();									//	초기화
	inline	int		getCount(){return	m_iCount;}
	BOOL			addPacket(ALL_TOOLLOGMSG *_lpPacket);				//	패킷 추가
	BOOL			isReceivedPacket(int _iPacket);				//	받은 패킷이냐?
	ALL_TOOLLOGMSG	*	popPacket();								//	큐에서 패킷을 빼낸다.

	ALL_TOOLLOGMSG	*	getOne();									//	큐에서 패킷 하나를 얻어낸다.
	void			setFront()
	{	
		m_iCounter		=	m_iCount;
		m_iCurrentQue	=	m_iFirstQue;
	}

};	//	class cPACKET_QUE

extern cTOOLLOG_QUE		g_ToolLogQue;

#pragma pack()

#endif	//	_classPACKET_TOOLLOGSERVER_H

