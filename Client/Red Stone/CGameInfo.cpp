#include "CGame.H"

char					CGameInfo::s_strLastSelectServerName[dNAME_LENGTH];
cSERVER_INFO			CGameInfo::s_aServerList[dMAX_SERVER_COUNT];
int						CGameInfo::s_iWorldServerCount			=	0;
int						CGameInfo::s_iServerGameVersion;					//	 서버의 게임 버젼

cAVATAR_INFO			CGameInfo::s_aAvatarList[dMAX_AVATAR_COUNT];		//	자신의 아바타들의 정보
char					CGameInfo::s_strLastID[dID_LENGTH]			=	"";	//	로그인 한 아이뒤
char					CGameInfo::s_strLastPassword[64]	=	"";	//	로그인 한 아이뒤

char					CGameInfo::s_strLastSelectAvatarName[dNAME_LENGTH]	=	"";
char					CGameInfo::s_strMessage[256]				=	"";

char					CGameInfo::s_strBookingToMoveFieldFileName[64];					//	맵 이름
char					CGameInfo::s_strBookingToMoveFieldServeAddress[20];
int						CGameInfo::s_iMySerial						=	0xffff;	//	나 자신의 시리얼

int						CGameInfo::s_aNetworkEvent[10];							//	갖가지 이벤트를 처리하기 위한 10개
int						CGameInfo::s_iSelectAvatar					=	0;
char					CGameInfo::s_strSelectAvatarName[dNAME_LENGTH];	//	선택한 아바타 이름

char					CGameInfo::s_strLoginServerAddress[64]		=	{0,};
char					CGameInfo::s_strGameServerAddress[64]		=	{0,};
int						CGameInfo::s_iLoginServerPort;
int						CGameInfo::s_iFrameCounter					=	0;						//	프레임 카운트
BOOL					CGameInfo::s_isUpdateTimingByUpdatePeriod	=	FALSE;			//	업데이트 주기에 의한 업데이트 타이밍이다.
BOOL					CGameInfo::s_bIs1Second						=	FALSE;
int						CGameInfo::s_iFrameCounterFor1SecondWork	=	0;

int						CGameInfo::s_iConnectType					=	eCONNECT_TYPE_FIRST;							//	필드 이동 중이다.
int						CGameInfo::s_isEnterenceGame				=	FALSE;
char					CGameInfo::s_strSelectServerName[dNAME_LENGTH];
char					CGameInfo::s_strGuildHallOwnerGuildName[dGUILD_NAME_LENGTH];
BOOL					CGameInfo::s_bIsGuildBattleField			=	FALSE;
BOOL					CGameInfo::s_bIsSiegeWarfareField			=	FALSE;
BOOL					CGameInfo::s_bIsVillage						=	FALSE;
BOOL					CGameInfo::s_bIsObserverMode				=	FALSE;
BOOL					CGameInfo::s_bIsGuildPointBattleField		=	FALSE;
BOOL					CGameInfo::s_bIsOXQuizField					=	FALSE;
BOOL					CGameInfo::s_bIsWordQuizField				=	FALSE;
BOOL					CGameInfo::s_bIsEventField					=	FALSE;
BOOL					CGameInfo::s_bIsHideInterface				=	FALSE;
BOOL					CGameInfo::s_bIsBlockUseSkill				=	FALSE;
BOOL					CGameInfo::s_bIsGuildHall					=	FALSE;
//BOOL					CGameInfo::s_bIsBossZone					=	FALSE;
BOOL					CGameInfo::s_iMinimapDisplayLevel			=	0;
BOOL					CGameInfo::s_iFieldSerial					=	0xffff;
BOOL					CGameInfo::s_iRemainGuildStategyPoint		=	0;

int						CGameInfo::s_iLoginServerLocalCount			=	0;
cLoginServerIPInfo		CGameInfo::s_aLoginServerIPList[20];
