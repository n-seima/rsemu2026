#pragma	once

#include	"SFC.h"

const	int		c_iMaxMainQuestValueCount	=	100;
const	int		c_iMaxMainQuestSwitchCount	=	100;
const	int		c_iUseMainQuestSwitchCount	=	96;

const	int		c_iMainQuestValueNameLength	=	32;
const	int		c_iMainQuestSwitchNameLength=	32;

const	int		c_iMaxChapterCount			=	32;
const	int		c_iMaxHighQuestCount		=	32;
const	int		c_iChapterNameLength		=	64;
const	int		c_iHighQuestNameLength		=	64;
const	int		c_iChapterCommentLength		=	1024;
const	int		c_iChapterEpilogCommentLength=	1024*3;


#define	dQUEST_COUNT					1024

#define	dQUEST_NAME_SIZE				64
#define	dQUEST_EXPLAIN_SIZE				2048
#define	dQUEST_STATUS_COUNT				6
#define	dMAX_PROCESS_QUEST_COUNT		6

#define	dEVENT_QUEST_COUNT				3
#define	dQUEST_CLIENT_INFO_SIZE			(256-48)
#define	dQUEST_VALUE_COMMENT_SIZE		128

#define	dNAME_LENGTH					18

#define	dQUEST_AND_ARBEIT_BORDER_VALUE	10000

enum
{
	eQK_NORMAL,
	eQK_SKILL_MASTER,
	eQK_TITLE,
	eQK_MAIN_STORY,
	eQK_GUILD,
};

//
//	Quest Type
enum
{
	eQT_LIVE,
	eQT_DEATH,
	eQT_EVENT,
};


//
//	Quest Type
enum
{
	eQT_NORMAL,
	eQT_ARBEIT,
};

#pragma	pack(2)

class	CQuestItem_common
{
public:
	WORD	m_wSerial;
	char	m_strName[dQUEST_NAME_SIZE];
	char	m_astrExplain[dQUEST_STATUS_COUNT][dQUEST_EXPLAIN_SIZE];

	WORD	m_wQuestType;	//	이벤트 퀘스트여?
	WORD	m_wBeginYear,m_wBeginMonth,m_wBeginDay;
	WORD	m_wExpireYear,m_wExpireMonth,m_wExpireDay;

	char	m_strQuestClientInfo[dQUEST_CLIENT_INFO_SIZE];
	WORD	m_wQuestKind;
	char	m_strBeginNPCName[dNAME_LENGTH];
	WORD	m_wBeginNPC_Field;

	WORD	m_wMinProprietyLevel,m_wMaxProprietyLevel;
	WORD	m_wMinLimitLevel,m_wMaxLimitLevel;
	char	m_strQuestValueComment[dQUEST_VALUE_COMMENT_SIZE];

	UINT	m_uiEnableJobMask;

	WORD	m_bf1IsMaleJobEquipAble				:	1;	//	남성 직업 가능
	WORD	m_bf1IsFemaleJobEquipAble			:	1;	//	여성 직업 가능
	WORD	m_isEnableCancel					:	1;
	WORD	m_isRemoveQuestItemByCancelQuest	:	1;
	WORD	m_isTestQuestInTestServer			:	1;
	WORD	m_bf1IsDisableQuest					:	1;	//6 전체목록에서 표시안함
	WORD	m_bf6ChapterNumber					:   6;	//12  챕터번호 // c_iMaxChapterCount
	WORD	: 0;
	WORD	m_bf6PartNumber						:   6; //6  파트번호 // c_iMaxHighQuestCount
	WORD	m_bf3Quarter						:	3; //9 분기퀘스트
	WORD	: 0;
	char	m_aSpareBuffer[30];
};

#pragma	pack()