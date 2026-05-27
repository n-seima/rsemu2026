
// RSS_Agent.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.


// CRSS_AgentApp:
// 이 클래스의 구현에 대해서는 RSS_Agent.cpp을 참조하십시오.
//

class CRSS_AgentApp : public CWinAppEx
{
public:
	CRSS_AgentApp();

// 재정의입니다.
	public:
	virtual BOOL InitInstance();

// 구현입니다.

	DECLARE_MESSAGE_MAP()
};

extern CRSS_AgentApp theApp;