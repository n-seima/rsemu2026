#ifndef	_redstoneClientCommonDefine_h
#define	_redstoneClientCommonDefine_h

#include "CTitleDefine.H"
#include "CItemEffectDefine.H"

class	CItemGenerateEffectText
{
public:
	int			m_iSerial;
	char		m_strReferenceText[256];
};

class	CPrefixEffectText
{
public:
	int			m_iSerial;
	char		m_strReferenceText[256];
};

class	CTitleEffectText
{
public:
	int			m_iSerial;
	char		m_strReferenceText[256];
};

class	cEventMobText
{
public:
	WORD		m_wBody;
	char		m_strMobName[64];
};

class	cHelpTextInfo
{
public:
	WORD		m_wTextOutputMethod;
	WORD		m_wTextShape;
	WORD		m_wX,m_wY;
	WORD		m_wAnm;
	char		m_strText[128];
};

enum
{
	eHTP_CENTER,
	eHTP_LEFT,
	eHTP_RIGHT,
	eHTS_BOX,
	eHTS_NORMAL
};

#define	dMAX_HELP_TEXT_COUNT	100
#define	dMAX_EVENT_MOB_COUNT	100

extern	cHelpTextInfo			g_aHelpTextInfo[dMAX_HELP_TEXT_COUNT];
extern	CPrefixEffectText		g_aPrefixHyperText[dMAX_ITEM_PREFIX_TYPE_COUNT];
extern	CTitleEffectText		g_aTitleHyperText[dMAX_TITLE_COUNT];
extern	CItemGenerateEffectText	g_aItemEffectHyperText[dMAX_ITEM_EFFECT_DATA];
extern	cEventMobText			g_aEventMobList[dMAX_EVENT_MOB_COUNT];

//	??????? ???? ??E?
enum
{
//	PVUI -> Project VERSION UPDATE INFO
	ePVUI_02_ADD_ARBEIT_QUEST		=	2	,
	ePVUI_03_ADD_QUEST_CLIENT_NAME	=	3	,
	ePVUI_04_ADD_QUEST_VALUE_COMMENT		,
	ePVUI_05_ADD_QUEST_CLIENT_INFO			,
	ePVUI_06_ADD_QUEST_ITEM_REMOVE_INFO		,
	ePVUI_07_REMOVE_QUEST_ITEM_REMOVE_INFO	,
	ePVUI_08_ADD_QUEST_PROPIETY_PLAYER_INFO	,
	ePVUI_09_ADD_PORJECT_KARMA				,
	ePVUI_10_ADD_PORJECT_KARMA_MONSTER_GEN	,
	ePVUI_11_RENEWAL_ENABLE_QUEST_JOB_INFO	,
	ePVUI_12_ENCRYPT						,
	ePVUI_13_ENCRYPT_MORE					,
	ePVUI_14_ENCRYPT2						,

	ePVUI_CURRENT_VERSION					,
};

#define	dCURRENT_PROJECT_VERSION	(ePVUI_CURRENT_VERSION-1)

extern	char	g_strProjectHeader[100][50];

#endif
