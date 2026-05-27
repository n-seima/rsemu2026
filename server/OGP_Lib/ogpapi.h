///////////////////////////////////////////////////////////////////////////////
// OGPAPI Library for C++ INCLUDE HEADER
///////////////////////////////////////////////////////////////////////////////

//Service ID
#define OGP_SI_LATALE		 5
#define OGP_SI_LOSTSAGA		 9
#define OGP_SI_LUNIA		10
#define OGP_SI_NEOONLINE	11
#define OGP_SI_HOCKEYDASH	12
#define OGP_SI_REDSTONE		13
#define OGP_SI_ZONE4		14
#define OGP_SI_SPACECRAFT	15


//Return Type
#define OGP_RT_XML			0
#define OGP_RT_JSON			1

//Launcher Message
#define OGP_LM_LOGIN_OK			10000
#define OGP_LM_LOGIN_FAIL		10001
#define OGP_LM_GAME_CLOSED		10010

//
#define OGP_RT_MAXSIZE			1024

///////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
typedef struct tagOGP_ITEMINFO
{
	char	ReceiverID[16];
	int		Astro;
	int		ItemID;
	char	ItemName[100];
	int		Quantity;
	int		Duration;
	int		UsedType;
	char	Note[50];
} OGP_ITEMINFO;
#pragma pack(pop)

#ifndef _OGPAPI

#pragma comment(lib, "ogpapi.lib")

extern "C" __declspec(dllimport) BOOL _InitOGPLibrary(int nServiceid, LPCTSTR pstrServicekey, LPCTSTR pstrServiceURL, int nReturnType=OGP_RT_XML, int nReturnMaxCount=OGP_RT_MAXSIZE);

extern "C" __declspec(dllimport) BOOL _FreeOGPLibrary();
extern "C" __declspec(dllimport) int _CheckValidToken(LPCTSTR pstrToken, LPCTSTR pstrUserid, LPCTSTR pstrUserip, int nRequesttype, LPTSTR lpRetString);
extern "C" __declspec(dllimport) int _RefreshToken(LPCTSTR pstrToken, LPCTSTR pstrUserid, LPCTSTR pstrUserip, LPTSTR lpRetString);
extern "C" __declspec(dllimport) int _GetUserInfo(LPCTSTR pstrToken, LPCTSTR pstrUserid, LPTSTR lpRetString);
extern "C" __declspec(dllimport) int _GetUserAstro(LPCTSTR pstrToken, LPCTSTR pstrUserid, LPCTSTR pstrUserip, LPTSTR lpRetString);
extern "C" __declspec(dllimport) int _BuyItem(LPCTSTR pstrToken, LPCTSTR pstrUserid, LPCTSTR pstrUserip, OGP_ITEMINFO * pItemInfo, LPTSTR lpRetString);
extern "C" __declspec(dllimport) int _CallbackBuyItem(LPCTSTR pstrToken, LPCTSTR pstrUserid, LPCTSTR pstrUserip, int nTransactionID, int nStatus, LPTSTR lpRetString);
extern "C" __declspec(dllimport) int _SendDirectHTTP(LPCTSTR pstrURL, LPTSTR lpRetString);
extern "C" __declspec(dllimport) BOOL _SendMessageToLauncher(WPARAM wParam, LPARAM lParam);

#endif