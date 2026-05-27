#ifndef _install_message_define_h
#define _install_message_define_h

extern	char	*g_strMessage[];

#define	dMSG_GAME_NAME											g_strMessage[0]
#define	dMSG_SELECT_INSTALL_FOLDER								g_strMessage[1]
#define	dMSG_PERMIT_LICENCE										g_strMessage[2]
#define	dMSG_PLEASE_READ_LINCENCE								g_strMessage[3]
#define	dMSG_SETUP_GAME											g_strMessage[4]
#define	dMSG_QUEST_INSTALL_FOLLOW_FOLDER						g_strMessage[5]

#define	dMSG_CANCELED_PROCESS									g_strMessage[7]
#define	dMSG_END_SETUP											g_strMessage[8]
#define	dMSG_REMOVE_INSTALLED_FILES								g_strMessage[9]
#define	dMSG_CALC_REMAIN_TIME									g_strMessage[10]




#define	dMSG_INSTALL											g_strMessage[14]
#define	dMSG_INSTALL_GAME										g_strMessage[15]

#define	dMSG_ERROR_INSTALL										g_strMessage[16]
#define	dMSG_CAN_NOT_CREATE_FOLDER								g_strMessage[17]
#define	dMSG_EXTRACT_FOLDER										g_strMessage[18]
#define	dMSG_REMAIN_TIME_FORM									g_strMessage[19]
#define	dMSG_COPY_SPEED											g_strMessage[20]

#define	dMSG_COMPLETE											g_strMessage[21]
#define	dMSG_COMPLETE_ALL										g_strMessage[22]
#define	dMSG_MANGCHI											g_strMessage[24]
#define	dMSG_REMOVE_GAME										g_strMessage[25]

#define	dMSG_CURRENT_FILE										g_strMessage[26]
#define	dMSG_ERROR												g_strMessage[27]
#define	dMSG_ERRORED_EXIT										g_strMessage[28]
#define	dMSG_TITLE												g_strMessage[29]
#define	dMSG_ERROR_FIND_EXIST_REDSTONE							g_strMessage[30]

#define	dMSG_REMOVE_PRE_RED_STONE								g_strMessage[31]
#define	dMSG_BUT_DAMAGED_REGISTRY_ARE_YOU_CONTINUE				g_strMessage[32]
#define	dMSG_SETTING											g_strMessage[33]
#define	dMSG_READ_LICENSE										g_strMessage[34]
#define	dMSG_LICENSE_QUESTION									g_strMessage[35]

#define	dMSG_INSTALL_COMPLETE									g_strMessage[36]
#define	dMSG_PRESSS_COMPLETE_BUTTON								g_strMessage[37]
#define	dMSG_INSTALL_FOLDER_FORM								g_strMessage[38]


extern	char	g_strLicence[2048*20];
extern	char	g_strLicence2[20][2048];

#ifdef	_FOR_THAI
extern	char	*g_strWelcome[];
#else
extern	char	g_strWelcome[];
#endif

extern	char	g_strGameName[128];

#endif