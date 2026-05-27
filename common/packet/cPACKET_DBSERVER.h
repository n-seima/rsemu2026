#ifndef _classPACKET_DBSERVER_H
#define _classPACKET_DBSERVER_H



#include "cPACKET_BASE.h"
#include "definePacketData.h"
#include "cGuildDefine.h"
#include "userCustomDataDefine.h"

#pragma pack(1)

class	cMSG_BASE_TYPE_FORDB
{
public:
	WORD	wSize,
			wType;

	inline	void	set(WORD size,WORD type)	{wSize	=	size,wType	=	type;}
};




#define dDDMSG_IAM				0x4101




#define dLDMSG_GETAVATARS		0x4310
#define dLDMSG_CREATEAVATAR		0x4311
#define dLDMSG_SELECTAVATAR		0x4312
#define dLDMSG_DELETEAVATAR		0x4313
#define dLDMSG_DELETELIST		0x4314
#define dLDMSG_COPY_DATA		0x4315
#define dLDMSG_LOGIN			0x4316
#define dLDMSG_LOGOUT			0x4317
#define dLDMSG_LOGINRESET		0x4318


#define dDLMSG_GETAVATARS		0x4710
#define dDLMSG_CREATEAVATAR		0x4711
#define dDLMSG_SELECTAVATAR		0x4712
#define dDLMSG_DELETEAVATAR		0x4713
#define dDLMSG_COPY_DATA_RESULT	0x4714


#define dLDMSG_DELETE_LIST		0x4321
#define dLDMSG_UPDATESAVE_USER	0x4322
#define dLDMSG_UPDATESAVE		0x4323
#define dLDMSG_GET_DATA			0x4324
#define dLDMSG_SAVE_DATA		0x4325
#define dLDMSG_SET_ADMIN		0x4326

#define dDLMSG_DELETE_LIST		0x4721
#define dDLMSG_UPDATESAVE_USER	0x4722
#define dDLMSG_UPDATESAVE		0x4723
#define dDLMSG_GET_DATA			0x4724







#define	dGDMSG_LOGINAVATAR					0x4401
#define dGDMSG_LOGOUTAVATAR					0x4402
#define dGDMSG_KILLAVATAR					0x4403

#define dDGMSG_LOGINAVATAR					0x4801
#define dDGMSG_LOGOUTAVATAR					0x4802


#define dGDMSG_BUYLIST						0x4411
#define dGDMSG_USEBUY						0x4412
#define dGDMSG_GET_CARROT_COUNT				0x4413
#define dGDMSG_USE_CARROT_LOG				0x4415

#define dDGMSG_BUYLIST						0x4811
#define dDGMSG_USEBUY						0x4812
#define dDGMSG_PREMIUM_ITEM_PRICE_INFO		0x4813
#define dDGMSG_GET_CARROT_COUNT_RESULT		0x4814
#define dDGMSG_USE_CARROT_RESULT			0x4815


#define dGDMSG_PROVISION_RANK_REQ			0x4421

#define dDGMSG_PROVISION_RANK_RET			0x4821


#define dGDMSG_PERSONALDATA					0x4431
#define dGDMSG_SAVE_PERSONALDATA			0x4432

#define dDGMSG_PERSONALDATA					0x4831

#define dGDMSG_STAT_GOLD					0x4441

#define dG2D_BUY_PREMIUM_ITEM_TO_WEB_DB		0x4442









#define	dWDMSG_SAVEAVA_TO_FILE	0x4501
#define	dWDMSG_BANKOPEN			0x4502
#define	dWDMSG_SAVEBANK			0x4503
#define dWDMSG_UPDATESAVE		0x4505
#define dWDMSG_KILL_AVATAR		0x4506

#define dWDMSG_GETGUILD			0x4511
#define dWDMSG_CREATEGUILD		0x4521
#define dWDMSG_DELETEGUILD		0x4522
#define dWDMSG_JOINGUILD		0x4523
#define dWDMSG_EXITGUILD		0x4524
#define dWDMSG_SAVEGUILDBASE	0x4525
#define dWDMSG_CHANGERANK		0x4526
#define dWDMSG_DELETEGUILDER	0x4527

#define dWDMSG_SAVEGUILD_GUARDIAN		0x4528
#define dWDMSG_SAVEGUILD_INVENTORY		0x4529
#define dWDMSG_SAVEGUILD_MARK			0x4530
#define dWDMSG_SAVEGUILD_STRUCTURE		0x4531

#define dWDMSG_SAVEGUILD_PET			0x4532


#define dWDMSG_SAVEGUILD_SKILL			0x4535
#define dWDMSG_MT_TIME_START			0x4536
#define dWDMSG_MT_TIME_END				0x4537
#define dWDMSG_ASK_COMPUTE_DUEL_RANKING	0x4538
#define dWDMSG_ASK_DUEL_RANKING_INFO	0x4539
#define dWDMSG_ASK_DUEL_RECORD			0x453a
#define	dWDMSG_TRANS_AVATAR_TO_GVG		0x453b


#define dDWMSG_SAVEAVA_TO_FILE	0x4901
#define dDWMSG_BANKOPEN			0x4902
#define	dDWMSG_SAVEBANK			0x4903

#define dDWMSG_GETGUILD			0x4911
#define dDWMSG_GUILD_MEMBER		0x4912
#define dDWMSG_GUILD_DATA		0x4913
#define dDWMSG_CREATEGUILD		0x4921
#define dDWMSG_DELETEGUILD		0x4922
#define dDWMSG_JOINGUILD		0x4923
#define dDWMSG_EXITGUILD		0x4924
#define dDWMSG_CHANGERANK		0x4926		

#define dDWMSG_SAVEGUILD_GUARDIAN		0x4928
#define dDWMSG_SAVEGUILD_INVENTORY		0x4929
#define dDWMSG_SAVEGUILD_MARK			0x4930
#define dDWMSG_SAVEGUILD_OBJECT			0x4931
#define dDWMSG_SAVEGUILD_PET			0x4932
#define dDWMSG_SAVEGUILD_POLL			0x4933
#define dDWMSG_SAVEGUILD_SELLINGITEM	0x4934
#define dDWMSG_SAVEGUILD_SKILL			0x4935

#define dDWMSG_DUEL_RECORD				0x4936
#define	dDWMSG_TRANS_AVATAR_TO_GVG_RESULT	0x4937



#define dWDMSG_ASK_EVENTS_INFO			0x4551

#define dDWMSG_GR_INFO_RESULT			0x4951
#define dDWMSG_ITEMPACK_RESULT			0x4952
#define dDWMSG_DUEL_RANKING_INFO		0x4953









struct	DDMSG_IAM
{
	cMSG_BASE_TYPE_FORDB		base;
	WORD						iIam;
};





struct CMMSG_DISCONNECT
{
	cMSG_BASE_TYPE_FORDB	base;
};






struct	LDMSG_GETAVATARS{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD						u_serial;
	DWORD						worldSerial;
	char						strID[dID_LENGTH];
};

struct LDMSG_CREATEAVATAR{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD						u_serial;
	DWORD						worldSerial;
	char						strID[dID_LENGTH];
	char						strName[dNAME_LENGTH];
	char						strIP[dIP_SIZE];
	WORD						wJob;
	WORD						wServerType;
};

struct	LDMSG_SELECTAVATAR{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD						loginSerial;
	DWORD						worldSerial;
	char						strID[dID_LENGTH];
	char						strNAME[dNAME_LENGTH];
};

struct	LDMSG_DELETEAVATAR{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD						u_serial;
	DWORD						worldSerial;
	char						strID[dID_LENGTH];
	char						strNAME[dNAME_LENGTH];
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

	WORD			wServerType;
	WORD			wResult;
};







struct DLMSG_GETAVATARS{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD						u_serial;
	DWORD						worldSerial;
	char						strID[dID_LENGTH];
	cAVATAR_INFO				aCharacter[dMAX_AVATAR_COUNT];
};

#define	dCREATE_AVATAR_RESULT_OK								0
#define	dCREATE_AVATAR_RESULT_ALREADY_EXIST_NAME				1
#define	dCREATE_AVATAR_RESULT_COUNT_FULLED						2
#define	dCREATE_AVATAR_RESULT_FAILED							3
struct	DLMSG_CREATEAVATAR{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD						u_serial;
	DWORD						worldSerial;
	char						strID[dID_LENGTH];
	char						strName[dNAME_LENGTH];
	short						wResult;
	WORD						wIndex;
	cAVATAR_INFO				aCharacter;
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

#define	dREMOVE_AVATAR_RESULT_FAILED							-1
#define	dREMOVE_AVATAR_RESULT_AVATAR_NOT_EXIST					-2
struct	DLMSG_DELETEAVATAR{
	cMSG_BASE_TYPE_FORDB		base;

	char						strID[dID_LENGTH];
	char						strName[dNAME_LENGTH];
	DWORD						u_serial;
	DWORD						worldSerial;
	WORD						guildSerial;
	short						wResult;
};







struct GDMSG_LOGINAVATAR
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

struct DGMSG_LOGINAVATAR
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


	eSAVE_SUCCESS_DB			=	1	,
	eSAVE_CAN_NOT_FIND_DATA				,
	eSAVE_FAIL_LITTLECOUNT				,
	eSAVE_DB_FAILED						,

	eSAVE_DB_UNKNOWN_ERROR				,

	eSAVE_DB_INCORRECT_DATA				,
	eSAVE_DB_DATA_NULL					,


	eSAVE_SUCCESS_SAVEFILE				,
	eSAVE_FAIL_SAVEFILE					,
	eSAVE_CAN_NOT_FIND_LOGIN_USER		,
	eSAVE_MEMORY_FAIL_LITTLECOUNT		,
	eSAVE_MEMORY_FAIL_LITTLECOUNT2		,
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



enum
{
	ePIUT_WITHDRAW,
	ePIUT_REMOVE,
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
	int		itemno;
	WORD	itcnt;
	short	sPrice;
	
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
#define dRESULT_USEBUYITEM_USED				2
#define dRESULT_USEBUYITEM_NOTFIND			3
#define dRESULT_USEBUYITEM_FAIL				4
#define dRESULT_USEBUYITEM_DESTROY_SUCCESS	5
#define dRESULT_USEBUYITEM_DESTROY_FAILED	6

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
	eGCR_SUCCESS,
	eGCR_NOTFOUNDUSER,
	eGCR_NOTAGREE,
	eGCR_FAILED,
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
	eUCR_SUCCESS			,
	eUCR_NOTFOUNDUSER		,
	eUCR_LOW_COUNT			,
	eUCR_AGREEMENT_FAILED	,
	eUCR_FAILED				,
};

struct DGMSG_USE_CARROT_RESULT
{
	cMSG_BASE_TYPE_FORDB	base;

	char	strId[dID_LENGTH];
	char	strName[dID_LENGTH];
	DWORD	dwSerialInServer;

	WORD	wResult;

	int		itemno;
	WORD	itcnt;
	int		crcnt;
	WORD	wPrice;
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
	
	stProvRankField	list[dMAX_PROVRANK_COUNT];
};

struct	GDMSG_PERSONALDATA
{
	cMSG_BASE_TYPE_FORDB	base;

	WORD	wSerial;
	WORD	wIndex;
	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
};

struct GDMSG_SAVE_PERSONALDATA
{
	cMSG_BASE_TYPE_FORDB	base;

	WORD	wIndex;
	int		iDBIndex;
	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
	cUserCustomDataDefine	data;
};


struct	DGMSG_PERSONALDATA
{
	cMSG_BASE_TYPE_FORDB	base;

	WORD	wSerial;
	WORD	wIndex;
	int		iDBIndex;
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

	WORD	wPayMethod;

	char	strReceiverName[dNAME_LENGTH];

	int		iLev;
	WORD	wJob;

	DWORD	dwSerialInServer;
};









struct WDMSG_SAVEAVA_TO_FILE
{
	cMSG_BASE_TYPE_FORDB			base;

	int								iSerialInServer;
	WORD							wIsWantResult;
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










struct WDMSG_GETGUILD
{
	cMSG_BASE_TYPE_FORDB		base;
};

struct DWMSG_GETGUILD
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD						wCount;
	WORD						wAllCount;
	GuildListDB					List[dGUILD_COUNT_FORPACKET];
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
	CSellingAgentItem	m_aSellingAgentItem[dMAX_GUILD_SELLING_AGENT_ITEM_COUNT];
};
*/
struct	WDMSG_SAVEGUILD_SKILL
{
	cMSG_BASE_TYPE_FORDB		base;
	DWORD		dwGameServerSerial;
	WORD		wIndex;
	WORD		wGuildPoint;
	char		strName[dGUILD_NAME_LENGTH];
	WORD		m_aGuildSkill[dMAX_GUILD_SKILL_COUNT];
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


struct	WDMSG_ASK_COMPUTE_DUEL_RANKING
{
	cMSG_BASE_TYPE_FORDB		base;
};


struct	WDMSG_ASK_DUEL_RANKING_INFO
{
	cMSG_BASE_TYPE_FORDB		base;
};


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
	char							strMasterName[dNAME_LENGTH];
	char							strMasterId[dID_LENGTH];
	WORD							wMasterLevel;
	WORD							wMasterJob;
	int								iWorldIndex;
	int								iEmptyGuildIndex;
};

#define	dGUILD_APPONT							1
#define	dGUILD_DISMISSAL						2
#define dGUILD_CHANGERANK_MASTER_APPONT_SPECIAL	3

struct	WDMSG_CHANGERANK
{
	cMSG_BASE_TYPE_FORDB		base;

	DWORD		dwGameServerSerial;
	WORD		wIndex;
	char		strName[dGUILD_NAME_LENGTH];
	char		strMaster[dNAME_LENGTH];
	char		strTarget[dNAME_LENGTH];
	WORD		wRank;
	WORD		wType;
	WORD		wRank2;
	WORD		wReason;

};

struct	WDMSG_DELETEGUILDER
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD		wIndex;
	char		strName[dGUILD_NAME_LENGTH];
	char		strTarget[dNAME_LENGTH];
};

#define dCREATEGUILD_RESULT_SUCCESS		0x01
#define dCREATEGUILD_RESULT_FULL		0x02
#define dCREATEGUILD_RESULT_SAMENAME	0x03
#define dCREATEGUILD_RESULT_HAVEGUILD	0x04
#define	dCREATEGUILD_RESULT_FAIL		0x05

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
#define dDELETEGUILD_RESULT_NOTFIND		0x03
#define	dDELETEGUILD_RESULT_FAIL		0x02

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
#define dJOINGUILD_RESULT_NOTFIND		0x02
#define	dJOINGUILD_RESULT_FULL			0x03
#define	dJOINGUILD_RESULT_JOINED		0x04
#define dJOINGUILD_RESULT_NEEDRANK		0x05
#define dJOINGUILD_RESULT_FAIL			0x06

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
#define dEXITGUILD_RESULT_NOTFIND		0x02
#define dEXITGUILD_RESULT_NOTJOIN		0x03
#define dEXITGUILD_RESULT_NOTACOUNT		0x04
#define dEXITGUILD_RESULT_FAIL			0x05
#define dEXITGUILD_RESULT_NEEDRANK		0x06
#define dEXITGUILD_RESULT_FAILED_BY_TARGET_IS_GUILD_MASTER	0x07
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
#define dCHANGERANK_RESULT_NOTFIND		0x02
#define dCHANGERANK_RESULT_NOTACOUNT	0x03
#define dCHANGERANK_RESULT_FAIL			0x04

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

	char		strId[dID_LENGTH];
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



struct DWMSG_GR_INFO_RESULT
{
	cMSG_BASE_TYPE_FORDB		base;

	UTime	begin,end;
	WORD	wBoostExp,wDropChance;

	char	strBeginComment[256];
	char	strEndComment[256];
};



struct DWMSG_ITEMPACK_RESULT
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD			wSerial;
	cItemPackInfo	info;
};

struct DWMSG_DUEL_RANKING_INFO
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD	wCount;

	char	astrName[100][dNAME_LENGTH];
	int		aiPoint[100];
};


#ifdef _FOR_THAI


#define dREQ_GETCASHPOINT		10
#define dREQ_BUYITEM			20


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

#endif
