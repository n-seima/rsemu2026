#ifndef	_classGLOBAL_MESSAGE_H
#define	_classGLOBAL_MESSAGE_H

#include "sfc.h"
#include "cItem_Prefix.h"

#define	dMSG_BROKEN											g_strMessage[0]
#define	dMSG_CAN_NOT_IMPROVE								g_strMessage[1]
#define	dMSG_CURSED											g_strMessage[2]
#define	dMSG_TOO_ADD_MANY_GOLD_FOR_PLAYER					g_strMessage[3]
#define	dMSG_TOO_ADD_MANY_GOLD_FOR_BANK						g_strMessage[4]
#define	dMSG_TOO_LOW_GOLD_IN_PLAYER							g_strMessage[5]

#define	dMSG_TOO_LOW_GOLD_IN_BANK							g_strMessage[6]
#define	dMSG_SAVE_GOLD_FORM									g_strMessage[7]
#define	dMSG_ADD_GOLD_FORM									g_strMessage[8]
#define	dMSG_DRAW_GOLD_FROM_PLAYER_FORM						g_strMessage[9]
#define	dMSG_DRAW_GOLD_FROM_BANK_FORM						g_strMessage[10]

#define	dMSG_BOOKING_OX_QUIZ_FORM							g_strMessage[11]
#define	dMSG_IS_NOT_CONNECTED_WORLD_SERVER					g_strMessage[12]
#define	dMSG_KRR											g_strMessage[13]
#define	dMSG_OX_QUIZ_FORM									g_strMessage[14]
#define	dMSG_OX_QUIZ										g_strMessage[15]

#define	dMSG_ARE_YOU_END_OX_QUIZ_FORM						g_strMessage[16]
#define	dMSG_ARE_YOU_CANCEL_OX_QUIZ_FORM					g_strMessage[17]
#define	dMSG_CHECK_SELECT_WORLD_FORM						g_strMessage[18]
#define	dMSG_OX_QUIZ_TEST									g_strMessage[19]
#define	dMSG_REVIVE_OX_QUIZ_LOSER_BATTLE					g_strMessage[20]

#define	dMSG_APPOINTMENT_WORD_OX_QUIZ_FORM					g_strMessage[21]
#define	dMSG_ARE_YOU_CANCEL_WORD_QUIZ_FORM					g_strMessage[22]
#define	dMSG_WORD_QUIZ_FORM									g_strMessage[23]

#define dSUNDAY												g_strWeekName[0]
#define dMONDAY												g_strWeekName[1]
#define dTUESDAY											g_strWeekName[2]
#define dWEDNESDAY											g_strWeekName[3]
#define dTHURSDAY											g_strWeekName[4]
#define dFRIDAY												g_strWeekName[5]
#define dSATURDAY											g_strWeekName[6]
#define dEVENT_NAME_COUNT	2	// 이벤트 개수....g_strEventName과 같아야한다+_+;

extern	char	*g_strMessage[];
extern	char	*g_strMiniPetName[];
extern	char	g_strToolMessage[200][256];
extern	char	g_strDlgText[200][64];
extern	char	*g_strJobName[];
extern	char	*g_strWeekName[];
extern	char	g_strItemCategory[100][32];
extern	cITEM_PREFIX::cTYPE		g_aItemPrefixType[dMAX_ITEM_PREFIX_TYPE_COUNT];

extern	int		initMessage();
extern	char	g_strGameFolder[512];
extern	char	*g_strEventName[];		// 이벤트 이름.  09.10.09

#endif
