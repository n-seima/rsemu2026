#ifndef _classPACKET_LOGINSERVER_H
#define _classPACKET_LOGINSERVER_H

#include "cPACKET_BASE.h"
#include "definePacketData.h"

#pragma pack(1)

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早Client->Server
//早ぬ韃 檣策蝶 薑曖
//	收收в萄 霞殮 瞪					0x1001 ~ 0x1100 
#define	dCL_LOGIN						0x1001	//	嬴夥顫 薑爾梱雖 爾頂貊, 夥煎 啪歜戲煎 菟橫除棻.
#define dCL_CREATE_AVATAR				0x1004	//	嬴夥顫 儅撩
#define dCL_REMOVE_AVATAR				0x1005	//	嬴夥顫 薯剪
#define dCL_SELECT_AVATAR				0x1006	//	嬴夥顫 摹鷗
#define dCL_CLIENT_TYPE					0x1007	//	贗塭檜樹お 顫殮擊 瞪歎п 遽棻.
#define dCL_LOGIN_OTP					0x1012	//	OTP囀萄蒂 爾頂遽棻.

#define dCL_COPY_DATA					0x1013	//	晦襄 憮幗曖 議葛攪蒂 纔蝶お煎 犒餌и棻.
//早Client->Server
//早ぬ韃 檣策蝶 薑曖
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早Server->Client
//早ぬ韃 檣策蝶 薑曖
//	收收в萄 霞殮 瞪					0x1101 ~ 0x1200
#define dSL_RESULT_LOGIN				0x1101	//	煎斜檣 唸婁
#define dSL_SERVER_LIST					0x1102	//	憮幗葬蝶お
#define dSL_AVATAR_LIST					0x1103	//	嬴夥顫 葬蝶お
#define dSL_RESULT_CREATE_AVATAR		0x1104	//	嬴夥顫 儅撩
#define dSL_RESULT_REMOVE_AVATAR		0x1105	//	嬴夥顫 薯剪
#define dSL_OUT							0x1107	//	凱. 釭陛幗溥..
#define dSL_NOTICE_FOR_LAUNCHER			0x1108	//	楛藥и纔 爾頂朝 奢雖
#define dSL_RESULT_SELECT_AVATAR		0x1109	//	嬴夥顫 摹鷗 唸婁

#define dSL_PATCH						0x1110	//	ぬ纂 幗蟒擊 爾頂遽棻.
#define dSL_RESULT_OF_COPY_AVATAR		0x1111	//	ぬ纂 幗蟒擊 爾頂遽棻.
#define dSL_LOGIN_RESULT_OTP			0x1112	//	OTP殮溘縑 渠и 唸婁蒂 給溥遽棻.

//早Server->Client
//早ぬ韃 檣策蝶 薑曖
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早Client->Server
//早煎斜檣/煎斜嬴醒
struct	CL_LOGIN
{
	cMSG_BASE_TYPE	base;
	WORD			wVersion;
	char			strId[dID_LENGTH];
	char			strPassword[dPASS_LENGTH];
	char			strSelectServer[dNAME_LENGTH];
	char			strMacAddress[64];
	char			strClientCode[12];

	WORD			b1IsYahoo				:	1;
	WORD			b1IsDisconnectExistID	:	1;
	WORD			b1IsUseToken			:	1;

	WORD			wSecurityCode1,wSecurityCode2;

	char			strToken[64];
};	//	struct	CL_LOGIN

struct	CL_LOGIN_OTP
{
	cMSG_BASE_TYPE	base;

	char			strOTPCode[12];
};


struct	CL_REMOVE_AVATAR	//	嬴夥顫 薯剪
{
	cMSG_BASE_TYPE	base;
	WORD			wSecurityCode;
	UINT			uiSecurityCodeRet;
	char			strName[dNAME_LENGTH];	//	檜葷
};struct	CL_CREATE_AVATAR	//	嬴夥顫 儅撩
{
	cMSG_BASE_TYPE	base;
	WORD			wJob;	//	霜機
	WORD			wSecurityCode;
	UINT			uiSecurityCodeRet;
	char			strName[dNAME_LENGTH];	//	檜葷
};


struct CL_SELECT_AVATAR
{
	cMSG_BASE_TYPE	base;
	WORD			wSecurityCode;
	UINT			uiSecurityCodeRet;
	char			strName[dNAME_LENGTH];	//	檜葷
};

//
//	Client Type
enum
{
	eCT_LAUNCHER	,	//	楛藥
	eCT_GAME		,	//	啪歜
	eCT_DUMMY_GAME	,	//	渦嘐 啪歜
};

//
//	贗塭檜樹お 顫殮擊 瞪歎п 遽棻.
struct	CL_CLIENT_TYPE
{
	cMSG_BASE_TYPE	base;

	WORD			wType;
};

struct	CL_COPY_DATA
{
	cMSG_BASE_TYPE	base;

	cAVATAR_INFO	info;

	WORD			wSecurityCode;
	UINT			uiSecurityCodeRet;
	WORD			wWorld;	//	
	WORD			wType;
};
//早煎斜檣/煎斜嬴醒
//早Client->Server
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早Server->Client
//早煎斜檣 憮幗

//收>煎斜檣 唸婁
#define dRESULT_LOGIN_SUCCESS			0x000	//	撩奢
#define dRESULT_LOGIN_FAILED			0x001	//	褒ぬ
#define dRESULT_LOGIN_USEDID			0x002	//	檜嘐 偽擎 嬴檜蛤陛 煎斜檣 л
#define dRESULT_LOGIN_RETRY				0x003	//	鼻傖ж啪 '棻衛 п睡溥塭 +_+' 
#define dRESULT_LOGIN_NOT_SAME_VERSION	0x004	//	幗蟒檜 ぎ葬啻... 
#define dRESULT_LOGIN_OTP				0x005	//	1離 撩奢. OTP 殮溘 夥塋.
#define dRESULT_LOGIN_SUCCESS_BUT_NOT_USE_OTP	0x006	//	撩奢. OTP朝 餌辨ж雖 彊擠.
#define dRESULT_LOGIN_SUCCESS_PERSONALCOM 0x007	//	PERSONAL COMM 撩奢. 
#define dRESULT_LOGIN_FAIL_PERSONALCOM	0x008	//	PERSONAL COMM 褒ぬ. 

struct	SL_RESULT_LOGIN
{
	cMSG_BASE_TYPE	base;
	DWORD			dwResult;
	DWORD			dwCode;
	WORD			wSecurityCode;

	char			strWhy[64];
};
//<收煎斜檣 唸婁

//
//收>憮幗葬蝶お
//	憮幗 顫殮擎 rscoomm.h 縑 enServerType 霤褻

class	cSERVER_INFO	//	憮幗薑爾
{
public:
	char	m_strName[dWORLD_NAME_LENGTH];	//	憮幗 檜葷
	WORD	m_wType			:	10;	//	憮幗 顫殮
	WORD	m_bf1IsInnerOpen:	1;	//	頂睡 螃Ъ 憮幗
	WORD	m_wServerPlaceInList;	//	贗塭檜樹お 憮幗 葬蝶お縑憮 ル衛腎朝 牖憮
	WORD	m_wMessage;
};	//	class	cSERVER_INFO

struct	SL_SERVER_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;				//	憮幗捱 熱
	WORD			wSecurityCode;
	cSERVER_INFO	aServerList[dMAX_SERVER_COUNT];
};
//<收憮幗葬蝶お

//
//收>嬴夥顫 葬蝶お
struct	SL_AVATAR_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			wSecurityCode;
	cAVATAR_INFO	aAvatarList[dMAX_AVATAR_COUNT];
};
//<收嬴夥顫 葬蝶お

//
//	嬴夥顫 犒餌 唸婁
struct	SL_RESULT_SELECT_AVATAR
{
	cMSG_BASE_TYPE	base;

	WORD			wServerType;
	WORD			wSecurityCode;
	WORD			wResult;	//	0 褒ぬ,1 撩奢
};

//
//收>嬴夥顫 儅撩 唸婁
#define	dRESULT_CREATE_AVATAR_SUCESS			0	//	撩奢
#define	dRESULT_CREATE_AVATAR_FAILED			1	//	褒ぬ
#define	dRESULT_CREATE_AVATAR_EXIST_SAME_NAME	2	//	褒ぬ
#define	dRESULT_CREATE_AVATAR_AVATAR_FULL		3	//	渦 儅撩 й 熱 橈棻.
#define	dRESULT_CREATE_FAILED_BY_INCORRECT_CHAR	4	//	議葛攪 檜葷戲煎 餌辨 й 熱 橈朝 僥濠陛 んл腎橫 氈棻.

struct	SL_RESULT_CREATE_AVATAR
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;	//	唸婁
	WORD			wIndex;		//	賃廓簞疏?
	WORD			wSecurityCode;

	cAVATAR_INFO	avatarInfo;	//	嬴夥顫 薑爾
};
//<收嬴夥顫 儅撩 唸婁

//
//收>嬴夥顫 薯剪 唸婁
#define	dRESULT_REMOVE_AVATAR_SUCESS	0	//	撩奢
#define	dRESULT_REMOVE_AVATAR_FAILED	1	//	褒ぬ

struct	SL_RESULT_REMOVE_AVATAR
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;	//	唸婁
	WORD	wSecurityCode;
};
//<收嬴夥顫 薯剪

struct	SL_RESULT_OF_COPY_AVATAR
{
	cMSG_BASE_TYPE	base;

	WORD	wServerType;
	WORD	wResult;
	WORD	wSecurityCode;
};

struct SL_PATCH
{
	cMSG_BASE_TYPE	base;

	int				iVer;
};


#define dOTP_RESULT_INCORRECT	0x01	//	澀跤 殮溘脾.
#define dOTP_RESULT_EXPIRE		0x02	//	だ晦脾
#define dOTP_RESULT_FAIL		0x03	//	劃陛 檜鼻и 縑楝. 剪曖 debugging辨橾 閨!!

struct SL_LOGIN_RESULT_OTP
{
	cMSG_BASE_TYPE	base;

	int				iResult;
};

#define	dNOTICE_FOR_LAUNCHER_SIZE					2048
#define	dNOTICE_FOR_LAUNCHER_DOWNLOAD_ADDRESS_SIZE	256

//
//	Server Status
enum
{
	eSS_ON,					//	麵螳 氈擠
	eSS_OFF,				//	疏螳 氈擠
	eSS_READY_TO_PATCH,		//	ぬ纂 遽綠醞
	eSS_INSPECTION,			//	薄匐醞
};

struct SL_NOTICE_FOR_LAUNCHER
{
	cMSG_BASE_TYPE	base;

	WORD			wLastestVersion;	//	幗蟒
	WORD			wServerStatus;		//	憮幗 鼻鷓
	char			strDownloadAddress[dNOTICE_FOR_LAUNCHER_DOWNLOAD_ADDRESS_SIZE];	//	棻遴煎萄 輿模
	char			strMessage[dNOTICE_FOR_LAUNCHER_SIZE];	//	奢雖 詭衛雖
};

//早Server->Client
//早煎斜檣 憮幗
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早Server->Client
//早

//
//	DIS 詭衛雖
struct	SL_OUT
{
	cMSG_BASE_TYPE	base;

	char			strCause[128];		//	避檜朝 檜嶸 -_x
};	//	struct	SG_CHAT
//早Server->Client
//早
//曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

#define	dLOGINSERVER_SECURITY_RET	0xa2f4c2d2

#pragma pack()


#endif	//	_classPACKET_LOGINSERVER_H