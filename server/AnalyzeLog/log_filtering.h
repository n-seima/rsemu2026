#pragma	once

#include	"cITEM.h"
#include	"cPACKET_LOGSERVER.h"

union	ALL_LOGMSG;

extern	BOOL			g_bIsOnlySetItem,g_bIsOnlyUnique,g_bIsOnlyDX,g_bIsOnlyEternal,g_bIsCheckIncorrectItem;	//	아이템 종류 필터
extern	int				g_iEternalGrade;
extern	int				g_iJobFilter;

extern	cITEM			g_aItemListForLog[512];

extern	char			g_astrIDForLog[512][dID_LENGTH];
extern	char			g_astrNameForLog[512][dNAME_LENGTH];
extern	char			g_astrGuildNameLog[512][dNAME_LENGTH];
extern	char			g_astrIPForLog[512][dIP_LENGTH];

extern	int				ExportIDWithLog(ALL_LOGMSG *_lpAllLog);
extern	int				ExportNameWithLog(ALL_LOGMSG *_lpAllLog);
extern	int				ExportIPWithLog(ALL_LOGMSG *_lpAllLog);

extern	BOOL			GetSaveLogIDAndName(char *_lpstrIDAnmName,char *_lpstrID,char *_lpstrName);
extern	BOOL			GetSaveLogIDAndName2(char *_lpstrIDAnmName,char *_lpstrID,char *_lpstrName);

extern	BOOL			CheckIsSerchData(int _iFilterIndex,char *_lpstrValue,BOOL _bIsCheckAndCondition,char *_lpstrFilterId,char *_lpstrFilterName,DWORD _dwFilterItem,DWORD _dwFilterItemSerial,char* _lpstrFilterIp,ALL_LOGMSG *_lpLog);
extern	BOOL			IsExistIncorrectPrefixItemLog(ALL_LOGMSG *_lpLog);	//	잘못된 접두사 아이템을 가진 로그냐?
extern	int				GetItemInLog(ALL_LOGMSG *_lpLog,cITEM *_lpItemBuffer=NULL);	//	이 로그에서 아이템 추출

extern	BOOL			IsSkipDataByCheckTimeAndSerchType(BOOL _bIsIgnoreSaveLog,BOOL _bIsAddSaveLog,ALL_LOGMSG *_lpLog,tsLogTime *_lpTime,int yy1,int yy2,int mm1,int mm2,int dd1,int dd2,int hour1,int hour2,int min1,int min2,int _iType,int _iCompareIndex);

//	필터
enum
{
	eALL,
	eITEM_ALL,
	eABOUT_GUILD_WORK,

	eITEM_DROP,	//	아이템 드랍
	eITEM_PICK,	//	아이템 줏은거
	eITEM_BUY_SELL,	//	아이템 판매한거
	eITEM_DEL,	//	아이템 부순거
	eITEM_TRADE,
	eITEM_BANK,

	eITEM_PREMIUM_ITEM,
	eITEM_CREATE_ITEM,
	eITEM_USE_ITEM,

	eITEM_EXPI,
	eITEM_COPY,
	eITEM_ENCH,
	eITEM_PITCHMAN_SHOP,
	eITEM_FIND_INCORECT_ITEM,
	eITEM_RENEW_TERM,

	eAVA_ALL,
	eAVA_CREATE,
	eAVA_DELETE,
	eAVA_LOGIN,
	eAVA_LOGOUT,
	eAVA_SAVE,

	eABOUT_EVENT,
	eABOUT_QUEST,
	eABOUT_BACK_SERVER,
	eABOUT_PET,
	eABOUT_MINIPET,
	eABOUT_BOTTOMLESS_BOX,
	eABOUT_HACKING	,
	eABOUT_CARROT_SHOP,
	eABOUT_DUEL,
	eABOUT_REBIRTH,
	eABOUT_ERROR,

	eABOUT_OPERATOR,

};

extern	BOOL	g_abIsAboutItemLog[256];
