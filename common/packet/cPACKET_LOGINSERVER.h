#ifndef _classPACKET_LOGINSERVER_H
#define _classPACKET_LOGINSERVER_H

#include "cPACKET_BASE.h"
#include "definePacketData.h"

#pragma pack(1)





#define	dCL_LOGIN						0x1001
#define dCL_CREATE_AVATAR				0x1004
#define dCL_REMOVE_AVATAR				0x1005
#define dCL_SELECT_AVATAR				0x1006
#define dCL_CLIENT_TYPE					0x1007
#define dCL_LOGIN_OTP					0x1012

#define dCL_COPY_DATA					0x1013








#define dSL_RESULT_LOGIN				0x1101
#define dSL_SERVER_LIST					0x1102
#define dSL_AVATAR_LIST					0x1103
#define dSL_RESULT_CREATE_AVATAR		0x1104
#define dSL_RESULT_REMOVE_AVATAR		0x1105
#define dSL_OUT							0x1107
#define dSL_NOTICE_FOR_LAUNCHER			0x1108
#define dSL_RESULT_SELECT_AVATAR		0x1109

#define dSL_PATCH						0x1110
#define dSL_RESULT_OF_COPY_AVATAR		0x1111
#define dSL_LOGIN_RESULT_OTP			0x1112








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
};

struct	CL_LOGIN_OTP
{
	cMSG_BASE_TYPE	base;

	char			strOTPCode[12];
};


struct	CL_REMOVE_AVATAR
{
	cMSG_BASE_TYPE	base;
	WORD			wSecurityCode;
	UINT			uiSecurityCodeRet;
	char			strName[dNAME_LENGTH];
};struct	CL_CREATE_AVATAR
{
	cMSG_BASE_TYPE	base;
	WORD			wJob;
	WORD			wSecurityCode;
	UINT			uiSecurityCodeRet;
	char			strName[dNAME_LENGTH];
};


struct CL_SELECT_AVATAR
{
	cMSG_BASE_TYPE	base;
	WORD			wSecurityCode;
	UINT			uiSecurityCodeRet;
	char			strName[dNAME_LENGTH];
};



enum
{
	eCT_LAUNCHER	,
	eCT_GAME		,
	eCT_DUMMY_GAME	,
};



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
	WORD			wWorld;
	WORD			wType;
};









#define dRESULT_LOGIN_SUCCESS			0x000
#define dRESULT_LOGIN_FAILED			0x001
#define dRESULT_LOGIN_USEDID			0x002
#define dRESULT_LOGIN_RETRY				0x003
#define dRESULT_LOGIN_NOT_SAME_VERSION	0x004
#define dRESULT_LOGIN_OTP				0x005
#define dRESULT_LOGIN_SUCCESS_BUT_NOT_USE_OTP	0x006
#define dRESULT_LOGIN_SUCCESS_PERSONALCOM 0x007
#define dRESULT_LOGIN_FAIL_PERSONALCOM	0x008

struct	SL_RESULT_LOGIN
{
	cMSG_BASE_TYPE	base;
	DWORD			dwResult;
	DWORD			dwCode;
	WORD			wSecurityCode;

	char			strWhy[64];
};






class	cSERVER_INFO
{
public:
	char	m_strName[dWORLD_NAME_LENGTH];
	WORD	m_wType			:	10;
	WORD	m_bf1IsInnerOpen:	1;
	WORD	m_wServerPlaceInList;
	WORD	m_wMessage;
};

struct	SL_SERVER_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;
	WORD			wSecurityCode;
	cSERVER_INFO	aServerList[dMAX_SERVER_COUNT];
};




struct	SL_AVATAR_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			wSecurityCode;
	cAVATAR_INFO	aAvatarList[dMAX_AVATAR_COUNT];
};




struct	SL_RESULT_SELECT_AVATAR
{
	cMSG_BASE_TYPE	base;

	WORD			wServerType;
	WORD			wSecurityCode;
	WORD			wResult;
};



#define	dRESULT_CREATE_AVATAR_SUCESS			0
#define	dRESULT_CREATE_AVATAR_FAILED			1
#define	dRESULT_CREATE_AVATAR_EXIST_SAME_NAME	2
#define	dRESULT_CREATE_AVATAR_AVATAR_FULL		3
#define	dRESULT_CREATE_FAILED_BY_INCORRECT_CHAR	4

struct	SL_RESULT_CREATE_AVATAR
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wIndex;
	WORD			wSecurityCode;

	cAVATAR_INFO	avatarInfo;
};




#define	dRESULT_REMOVE_AVATAR_SUCESS	0
#define	dRESULT_REMOVE_AVATAR_FAILED	1

struct	SL_RESULT_REMOVE_AVATAR
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD	wSecurityCode;
};


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


#define dOTP_RESULT_INCORRECT	0x01
#define dOTP_RESULT_EXPIRE		0x02
#define dOTP_RESULT_FAIL		0x03

struct SL_LOGIN_RESULT_OTP
{
	cMSG_BASE_TYPE	base;

	int				iResult;
};

#define	dNOTICE_FOR_LAUNCHER_SIZE					2048
#define	dNOTICE_FOR_LAUNCHER_DOWNLOAD_ADDRESS_SIZE	256



enum
{
	eSS_ON,
	eSS_OFF,
	eSS_READY_TO_PATCH,
	eSS_INSPECTION,
};

struct SL_NOTICE_FOR_LAUNCHER
{
	cMSG_BASE_TYPE	base;

	WORD			wLastestVersion;
	WORD			wServerStatus;
	char			strDownloadAddress[dNOTICE_FOR_LAUNCHER_DOWNLOAD_ADDRESS_SIZE];
	char			strMessage[dNOTICE_FOR_LAUNCHER_SIZE];
};











struct	SL_OUT
{
	cMSG_BASE_TYPE	base;

	char			strCause[128];
};




#define	dLOGINSERVER_SECURITY_RET	0xa2f4c2d2

#pragma pack()


#endif