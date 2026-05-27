#include "cMAIN.H"
#include "CGame.H"
#include "CAgent.H"
#include "CGamePlay.H"
#include "ngpBrown.h"
#include "CMessage.h"
#include "kls.H"
#include "CLoadTextCode.h"
#include "CMap.h"
#include "tdlanguage.h"
#include "zmouse.h"
#include "tool.h"
#include "CWindowInterface.h"
#include "CAgent.h"
#include "CHero.h"
#include "CDecoder.h"
#ifdef	_USE_HS
#include "HShield.h"
#endif

//
//	키로거 체크
BOOL
cMAIN::CheckKeyLogger()
{
	return	TRUE;
	int				ret;
	KeyLoggerList	*list;

	ngpBrown_Init(NULL);

	list	=	(KeyLoggerList*)malloc(sizeof(KeyLoggerList));
	ret		=	ngpBrown_CheckKeyLogger(list);

	if (ret	!=	0)
	{
		free(list);

		return	ERRMSG(dMSG_REDSTONE_EXCUTER_ERROR,dMSG_CAN_NOT_FIND_SOME_FILE);
	}
	else
	{
		if (list->Num)
		{
			for (int i=0;i<list->Num;i++)
				ERRMSG(dMSG_HACKING_PROGRAM_FOUND,"%s\n\n%s/%s",dMSG_CHECK_SYSTEM,list->keyloggers[i].Name,list->keyloggers[i].FileName);

			free(list);

			return	FALSE;
		}
	}

	free(list);

	return	TRUE;
}

#ifdef	_USE_NPGL
BOOL
cMAIN::InitGG()
{
#ifdef	_FOR_JAPAN
	g_npgl	=	new CNPGameLib("RedStoneJP");
#elif	_FOR_USA
	g_npgl	=	new CNPGameLib("RedStoneUS");
#endif

	DWORD dwResult = g_npgl->Init();

	if (dwResult != NPGAMEMON_SUCCESS)
	{
		TCHAR msg[256];
		LPCSTR lpszMsg;

		// '6. 주요에러코드'를 참조하여 상황에 맞는 메시지를 출력해줍니다.
		switch (dwResult)
		{
			case NPGAMEMON_ERROR_EXIST:
				lpszMsg = dMSG_GGM_ALREADY_ACTIVE_GG_PLEASE_RESTART_GAME;
				break;
			case NPGAMEMON_ERROR_GAME_EXIST:
				lpszMsg = dMSG_GGM_DOUBLE_START_GAME;
				break;
			case NPGAMEMON_ERROR_INIT:
				lpszMsg = dMSG_GGM_GG_INIT_ERROR;
				break;
			case NPGAMEMON_ERROR_AUTH_GAMEGUARD:
			case NPGAMEMON_ERROR_NFOUND_GG:
			case NPGAMEMON_ERROR_AUTH_INI:
			case NPGAMEMON_ERROR_NFOUND_INI:
				lpszMsg = dMSG_GGM_INCORRECT_GG_FILE;
				break;
			case NPGAMEMON_ERROR_CRYPTOAPI:
				lpszMsg = dMSG_GGM_INCORRECT_WINDOW_SYSTEM_FILE;
				break;
			case NPGAMEMON_ERROR_EXECUTE:
				lpszMsg = dMSG_GGM_FAILED_TO_BEGIN_GG;
				break;
			case NPGAMEMON_ERROR_ILLEGAL_PRG:
				lpszMsg = dMSG_GGM_FIND_HACK;
				break;
			case NPGMUP_ERROR_ABORT:
				lpszMsg = dMSG_GGM_CANCEL_GG_UPDATE;
				break;
			case NPGMUP_ERROR_CONNECT:
			case NPGMUP_ERROR_DOWNCFG:
				lpszMsg = dMSG_GGM_FAILED_TO_CONNECT_GG_UPDATE_SERVER;
				break;
			case NPGMUP_ERROR_AUTH:
				lpszMsg = dMSG_GGM_CAN_NOT_COMPLETE_TO_GG_UPDATE;
				break;
			case NPGAMEMON_ERROR_NPSCAN:
				lpszMsg = dMSG_GGM_FAILED_TO_LOADING_MODULE_FOR_DETECT_HACK;
				break;
			default:
				lpszMsg = dMSG_GGM_FAILED_TO_BEGIN_GG2;
				break;
		}

		wsprintf(msg,dMSG_GGM_GG_ERROR_FORM, dwResult);

		MessageBox(GetActiveWindow(), lpszMsg, msg, MB_ICONERROR|MB_TOPMOST);

		return	FALSE;
	}

	return TRUE;
}
#endif

#ifdef	_USE_HS

int __stdcall AhnHS_Callback(long lCode, long lParamSize, void* pParam);

BOOL
cMAIN::InitHS()
{
	TCHAR	szFullFileName[512];

	lstrcpy(szFullFileName,g_strLogFolder);
	lstrcat(szFullFileName, _T("\\HackShield\\EhSvc.dll"));

	g_dwMainThreadID	=	GetCurrentThreadId();

#ifdef	_IS_DEV_CLIENT
	char	*lpstrLicenseKey	=	"AF9ED1318FD53363";
	int		iGameCode			=	3903;
#elif	_IS_DEBUG
	char	*lpstrLicenseKey	=	"1D396C37A2567CF9";
	int		iGameCode			=	3902;
#else
	char	*lpstrLicenseKey	=	"6D5D8BF64C7A6141";
	int		iGameCode			=	3901;
#endif

	DWORD	dwOption			=	AHNHS_CHKOPT_ALL |	AHNHS_USE_LOG_FILE |	
									AHNHS_ALLOW_SVCHOST_OPENPROCESS |AHNHS_ALLOW_LSASS_OPENPROCESS |
									AHNHS_ALLOW_CSRSS_OPENPROCESS |	AHNHS_DONOT_TERMINATE_PROCESS;

	int		iResult	=	_AhnHS_Initialize(szFullFileName,AhnHS_Callback,iGameCode,lpstrLicenseKey,dwOption,AHNHS_SPEEDHACK_SENSING_RATIO_NORMAL);

	assert(iResult != HS_ERR_INVALID_PARAM);
	assert(iResult != HS_ERR_INVALID_LICENSE);
	assert(iResult != HS_ERR_ALREADY_INITIALIZED);

	if	(iResult != HS_ERR_OK) 
	{
		//Error 처리 
		switch(iResult)
		{
			case HS_ERR_ANOTHER_SERVICE_RUNNING:
			{
				MessageBox(GetActiveWindow(), _T(dMSG_ALREADY_RUNNING_REDSTONE),"Red Stone", MB_OK);
				break;
			}
			case HS_ERR_INVALID_FILES:
			{
				MessageBox(GetActiveWindow(), _T(dMSG_HS_INCORRECT_DATA_PLEASE_REINSTALL), "Red Stone", MB_OK);
				break;
			}
			case HS_ERR_DEBUGGER_DETECT:
			{
				MessageBox(GetActiveWindow(), _T(dMSG_HS_FOUND_DEBUGGER_STOP_DEBUGER_AND_RESTART), "Red Stone", MB_OK);
				break;
			}
			case HS_ERR_NEED_ADMIN_RIGHTS:
			{
				MessageBox(GetActiveWindow(), _T(dMSG_HS_ERR_NEED_ADMIN_RIGHTS), "Red Stone", MB_OK);
				break;
			}
			case HS_ERR_COMPATIBILITY_MODE_RUNNING:
			{
				MessageBox(GetActiveWindow(), _T(dMSG_HS_ERR_COMPATIBILITY_MODE_RUNNING), "Red Stone", MB_OK);
				break;				
			}
			default:
			{
				TCHAR szMsg[255];
				wsprintf(szMsg, _T(dMSG_HS_OCCUR_PROBLEM_IN_DEFEND_HACKING_FORM), iResult);
				MessageBox(GetActiveWindow(), szMsg, "Red Stone", MB_OK);
				break;
			}
		}

		return FALSE;
	}

	iResult = _AhnHS_StartService();
	assert(iResult != HS_ERR_NOT_INITIALIZED);
	assert(iResult != HS_ERR_ALREADY_SERVICE_RUNNING);

	if	(iResult != HS_ERR_OK)
	{
		TCHAR szMsg[255];
		wsprintf(szMsg, _T(dMSG_HS_OCCUR_PROBLEM_IN_DEFEND_HACKING_FORM), iResult);
		MessageBox(GetActiveWindow(), szMsg, "Red Stone", MB_OK);

		return FALSE;
	}

	return	TRUE;
}

int __stdcall 
AhnHS_Callback(long lCode, long lParamSize, void* pParam)
{
	HWND	hActiveHandle	=	cMAIN::s_hWnd;

	if	(hActiveHandle	==	NULL) 
		hActiveHandle	=	GetActiveWindow();

	switch(lCode)
	{
		//Engine Callback
		case AHNHS_ENGINE_DETECT_GAME_HACK:
		{
			TCHAR szMsg[255];
			
			wsprintf(szMsg, _T(dMSG_HS_END_CLIENT_BY_FOUND_HACKING_TOOL_FORM), (char*)pParam);
			MessageBox(hActiveHandle, szMsg, "Red Stone", MB_OK);
			cMAIN::Exit(TRUE);
			break;
		}

		//일부 API가 이미 후킹되어 있는 상태
		//그러나 실제로는 이를 차단하고 있기 때문에 다른 후킹시도 프로그램은 동작하지 않습니다.
		//이 Callback은 단지 경고 내지는 정보제공 차원에서 제공되므로 게임을 종료할 필요가 없습니다.
		case AHNHS_ACTAPC_DETECT_ALREADYHOOKED	:
		{
			PACTAPCPARAM_DETECT_HOOKFUNCTION pHookFunction = (PACTAPCPARAM_DETECT_HOOKFUNCTION)pParam;
			TCHAR szMsg[255];
			wsprintf(szMsg, _T("[HACKSHIELD] Already Hooked\n- szFunctionName : %s\n- szModuleName : %s\n"), 
				pHookFunction->szFunctionName, pHookFunction->szModuleName);
			OutputDebugString(szMsg);
			break;
		}

		case AHNHS_ACTAPC_DETECT_AUTOMOUSE		:
		{
			TCHAR szMsg[255];
			
			wsprintf(szMsg, _T(dMSG_HS_OCCUR_PROBLEM_IN_DEFEND_HACKING_FORM), lCode);
			OutputDebugString(szMsg);
			break;
		}

		//Speed 관련
		case AHNHS_ACTAPC_DETECT_SPEEDHACK:
		case AHNHS_ACTAPC_DETECT_SPEEDHACK_APP:
		{
			MessageBox(hActiveHandle, _T(dMSG_FOUND_SPEED_HACK), "Red Stone", MB_OK);
			cMAIN::Exit(TRUE);
			break;
		}

		//디버깅 방지 
		case AHNHS_ACTAPC_DETECT_KDTRACE:	
		case AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED:
		{
			TCHAR szMsg[255];
			wsprintf(szMsg, _T(dMSG_HS_FOUND_TRY_DEBUGGING_FORM), lCode);
			MessageBox(hActiveHandle, szMsg, "Red Stone", MB_OK);
			cMAIN::Exit(TRUE);
			break;
		}

		//그외 해킹 방지 기능 이상 
		case AHNHS_ACTAPC_DETECT_DRIVERFAILED:
		case AHNHS_ACTAPC_DETECT_HOOKFUNCTION:
		case AHNHS_ACTAPC_DETECT_MESSAGEHOOK:
		case AHNHS_ACTAPC_DETECT_MODULE_CHANGE:
		{
			TCHAR szMsg[255];
			
			wsprintf(szMsg, _T(dMSG_HS_OCCUR_PROBLEM_IN_DEFEND_HACKING_FORM), lCode);
			MessageBox(hActiveHandle, szMsg, "Red Stone", MB_OK);
			cMAIN::Exit(TRUE);
			break;
		}
	}
	return 1;
}

#endif

