/********************************************************************
	created:	2002/04/20
	created:	20:4:2002   9:52
	filename: 	C:\Work\Project Red Stone\Red Stone\cPACKET.h
	file path:	C:\Work\Project Red Stone\Red Stone
	file base:	cPACKET
	file ext:	h
	author:		이영찬
	
	purpose:	패킷 정의.. 후.. 후후후...
*********************************************************************/

//	패킷은 워드형이다 그러니까... 0xffff가 넘는 수치로 정의 할 수 없다.
//	const WORD 로 선언했다가 바꿨는데... 그 이유는...
//	visual assist에서 #define 으로 선언된 것은 매크로로 인색해서 색칠-_-을 해주는데..
//	const 로 선언된건 색칠을 안해준다. 그래서... 그랬다. 머... 그랬다고... 그랬다고... 그랬는데... 그랬어... 랬어... 어... ;;;

#ifndef _classPACKET_H
#define _classPACKET_H

#include "SFC.H"
#include "SFC.H"

#pragma pack(1)

class	cMSG_BASE_TYPE
{
public:
	WORD	wSize,				//	패킷 사이즈
			wType,				//	패킷 타입
			wProtectType;		//	보안 타입
	inline	void	set(WORD size,WORD type,WORD protectType=0)	{ wSize = size,wType = type,wProtectType=protectType;}
};

#define dCL_CLIENT_TYPE					0x1007	//	클라이언트 타입을 전송해 준다.

//
//	Client Type
enum
{
	eCT_LAUNCHER	,	//	런쳐
	eCT_GAME		,	//	게임
	eCT_DUMMY_GAME	,	//	더미 게임
};

//
//	클라이언트 타입을 전송해 준다.
struct	CL_CLIENT_TYPE
{
	cMSG_BASE_TYPE	base;

	WORD			wType;
};

#define dSL_NOTICE_FOR_LAUNCHER			0x1108	//	런쳐한테 보내는 공지

#define	dNOTICE_FOR_LAUNCHER_SIZE					2048
#define	dNOTICE_FOR_LAUNCHER_DOWNLOAD_ADDRESS_SIZE	256

//
//	Server Status
enum
{
	eSS_ON,					//	켜져 있음
	eSS_OFF,				//	꺼져 있음
	eSS_READY_TO_PATCH,		//	패치 준비중
	eSS_INSPECTION,			//	점검중
};


struct SL_NOTICE_FOR_LAUNCHER
{
	cMSG_BASE_TYPE	base;

	WORD			wLastestVersion;	//	버젼
	WORD			wServerStatus;		//	서버 상태
	char			strDownloadAddress[dNOTICE_FOR_LAUNCHER_DOWNLOAD_ADDRESS_SIZE];	//	다운로드 주소
	char			strMessage[dNOTICE_FOR_LAUNCHER_SIZE];	//	공지 메시지
};


union	SERVER_PACKETS
{
	cMSG_BASE_TYPE				base;	//	기본 패킷

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃로그인 서버
	SL_NOTICE_FOR_LAUNCHER		mSL_NOTICE_FOR_LAUNCHER;
//┃로그인 서버
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
};	//	union	SERVER_PACKETS

#pragma pack()

#endif