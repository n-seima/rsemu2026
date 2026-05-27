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

#include "cPACKET_LOGINSERVER.h"

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