#include "debugCode.h"

int		g_bIsInnerTestServer			=	0;
int		g_bIsTestServer					=	0;
int		g_iOutputStatus					=	0;
BOOL	g_bIsTestGuildBattle			=	FALSE;
BOOL	g_bIsBlockSetItem				=	TRUE;
BOOL	g_bIsChansTestServer			=	FALSE;
BOOL	g_bIsDuelTestPC					=	FALSE;
BOOL	g_bIsStressTestServer			=	FALSE;
BOOL	g_bIsTestBCS_Deliver			=	FALSE;
BOOL	g_bIsEnablePVPMagicDamageRule	=	FALSE;
BOOL	g_bIsTestSiegeWarfare			=	FALSE;
BOOL	g_bIsIgnoreSSInfo				=	FALSE;
BOOL	g_bIsStopInstanceGuildBattle	=	FALSE;
BOOL	g_bIsWantPrintTimeKarmaInfo		=	FALSE;	//	시간에 의해 주기적으로 발동하는 카르마 정보 출력
BOOL	g_bIsUseXTrap					=	TRUE;
BOOL	g_bIsOperatorServer				=	FALSE;	//	 개발자 서버다..
BOOL	g_bIsMainTestServer				=	FALSE;	//	본서버랑 설정이 똑같은 테섭. 결재나 몇가지 게임 외적인것만 테스트다.

#ifdef	_FOR_JAPAN
BOOL	g_bIsEnableGuildChat	=	TRUE;
#else
BOOL	g_bIsEnableGuildChat	=	TRUE;
#endif

cDebugValue	g_debugSign;

void
cDebugValue::reset()
{
	memset(this,0xffff,sizeof(cDebugValue));
}

void
cDebugValue::init()
{
}

void
cDebugValue::writeSign(char *_lpstrText)
{
}