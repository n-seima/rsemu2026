// EventManager.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "EventManager.h"
#include "stdafx.h"
#include "AdminTool.h"
#include "CharacterInfo.h"
#include "CScript.h"
#include "cADO_ADMIN.h"
#include "afxdlgs.h"
#include "cDECODER.h"
#include "atlbase.h"
#include "CSkill.h"
#include "cITEM_PREFIX.h"
#include "cPACKET_DBSERVER.h"
#include "cPACKET_USERDBSERVER.h"
#include "cPACKET_LOGSERVER.h"
#include "cPACKET_WORLDSERVER.h"
#include "SFC.h"
#include "ExpCalc.h"
#include "io.h"
#include "CMessage.h"
#include "cPACKET_TOOLLOGSERVER.h"
#include "AdminLogCode.h"
#include "cInventory.h"
#include "LottoEvent.h"
#include "EventTimeManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEventManager dialog

cADO_ADMIN		l_DBTemp;
CAvatarManager	l_amTemp;


CEventManager::CEventManager()
	: CPropertyPage(CEventManager::IDD)
{
	m_iSelectOXQuizSolution	=	0;
	//{{AFX_DATA_INIT(CEventManager)
	//}}AFX_DATA_INIT
}


void CEventManager::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventManager)
	DDX_Control(pDX, IDC_GR_END_COMMENT, m_ebGREndComment);
	DDX_Control(pDX, IDC_GR_BEGIN_COMMENT, m_ebGRBeginComment);
	DDX_Control(pDX, IDC_GR_ITEM_DROP_CHANCE, m_ebDropChance);
	DDX_Control(pDX, IDC_GR_EXP, m_ebExp);
	DDX_Control(pDX, IDC_GR_MINUTE2, m_ebMinute2);
	DDX_Control(pDX, IDC_GR_MINUTE1, m_ebMinute1);
	DDX_Control(pDX, IDC_GR_HOUR2, m_ebHour2);
	DDX_Control(pDX, IDC_GR_HOUR1, m_ebHour1);
	DDX_Control(pDX, IDC_GR_DAY2, m_ebDay2);
	DDX_Control(pDX, IDC_GR_DAY1, m_ebDay1);
	DDX_Control(pDX, IDC_GR_MONTH2, m_ebMonth2);
	DDX_Control(pDX, IDC_GR_MONTH1, m_ebMonth1);
	DDX_Control(pDX, IDC_GR_YEAR2, m_ebYear2);
	DDX_Control(pDX, IDC_GR_YEAR1, m_ebYear1);
	DDX_Control(pDX, IDC_QUESTION, m_editOxQuizQuestion);
	DDX_Control(pDX, IDC_YEAR, m_editYear);
	DDX_Control(pDX, IDC_MONTH, m_editMonth);
	DDX_Control(pDX, IDC_HOUR, m_editHour);
	DDX_Control(pDX, IDC_DAY, m_editDay);
	DDX_Control(pDX, IDC_OX_QUIZ_SOLUTION, m_cbOX);
	DDX_Control(pDX, IDC_EVENT_KIND,	m_cbEventKind);			// 이벤트 종류  09.10.09
	DDX_Control(pDX, IDC_EVENT_BUFF,	m_cbEventBuff);			// 이벤트 버프 종류  09.10.09
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEventManager, CDialog)
	//{{AFX_MSG_MAP(CEventManager)
	ON_BN_CLICKED(IDC_BOOKING_OX_QUIZ_EVENT, OnBookingOxQuizEvent)
	ON_BN_CLICKED(IDC_OX_QUIZ, OnOxQuiz)
	ON_BN_CLICKED(IDC_COMPLETE_OX_QUIZ, OnCompleteOxQuiz)
	ON_BN_CLICKED(IDC_CANCEL_OX_QUIZ_EVENT, OnCancelOxQuizEvent)
	ON_BN_CLICKED(IDC_BOOKING_OX_QUIZ_EVENT_FOR_TEST, OnBookingOxQuizEventForTest)
	ON_BN_CLICKED(IDC_REVIVE_OX_QUIZ_LOSER_BATTLE, OnReviveOxQuizLoserBattle)
	ON_BN_CLICKED(IDC_GR_APPLY, OnGrApply)
	ON_BN_CLICKED(IDC_GR_CANCEL, OnGrCancel)
	ON_BN_CLICKED(IDC_GET_SCHEDULE, OnGetSchedule)
	ON_BN_CLICKED(IDC_FILL_TIME, OnFillTime)
	ON_BN_CLICKED(IDC_APPOINTMENT_WORD_QUIZ, OnAppointmentWordQuiz)
	ON_BN_CLICKED(IDC_CANCEL_WORD_QUIZ, OnCancelWordQuiz)
	ON_BN_CLICKED(IDC_SETTING_WQ_QUESTION, OnSettingWqQuestion)
	ON_BN_CLICKED(IDC_CHANGE_WORD_QUIZ_ROUND, OnChangeWordQuizRound)
	ON_BN_CLICKED(IDC_END_WORD_QUIZ, OnEndWordQuiz)
	ON_BN_CLICKED(IDC_TEST_WORD_QUIZ, OnTestWordQuiz)
	ON_BN_CLICKED(IDC_EVENT_LIST_BUTTON_SERCH, OnEventListButtonSerch)
	ON_BN_CLICKED(IDC_EVENT_LIST_BUTTON_OK, OnEventListButtonOk)
	ON_BN_CLICKED(IDC_CHATTING_EVENT_BUTTON_SET, OnChattingEventButtonSet)
	ON_BN_CLICKED(IDC_CHATTING_EVENT_BUTTON_RELEASE, OnChattingEventButtonRelease)
	ON_BN_CLICKED(IDC_CHANGE_MESSAGE_OF_SOUND_OF_LEDERS_BELL, OnChangeMessageOfSoundOfLedersBell)
	ON_BN_CLICKED(IDC_RESET_SOUND_OF_LEDERS_BELL, OnResetSoundOfLedersBell)
	ON_BN_CLICKED(IDC_CANCEL_SOUND_OF_LEDERS_BELL, OnCancelSoundOfLedersBell)
	ON_BN_CLICKED(IDC_CHECK_EVENT_INFO, OnCheckEventInfo)
	ON_BN_CLICKED(IDC_LOTTO_EVENT, OnLottoEvent)
	ON_BN_CLICKED(IDC_EVENT_TIME, OnEventTime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEventManager message handlers

BOOL CEventManager::OnInitDialog() 
{
#ifndef _FOR_KOREA
	SetDlgItemText(IDC_STATIC1,g_strDlgText[60]);
	SetDlgItemText(IDC_STATIC2,g_strDlgText[61]);
//	SetDlgItemText(IDC_STATIC3,g_strDlgText[62]);
//	SetDlgItemText(IDC_STATIC4,g_strDlgText[63]);
//	SetDlgItemText(IDC_STATIC5,g_strDlgText[64]);
//	SetDlgItemText(IDC_STATIC6,g_strDlgText[65]);
//	SetDlgItemText(IDC_STATIC7,g_strDlgText[66]);
	SetDlgItemText(IDC_STATIC8,g_strDlgText[164]);
//	SetDlgItemText(IDC_BOOKING_OX_QUIZ_EVENT_FOR_TEST,g_strDlgText[67]);
//	SetDlgItemText(IDC_BOOKING_OX_QUIZ_EVENT,g_strDlgText[68]);
//	SetDlgItemText(IDC_CANCEL_OX_QUIZ_EVENT,g_strDlgText[69]);
	SetDlgItemText(IDC_STATIC9,g_strDlgText[71]);
	SetDlgItemText(IDC_STATIC10,g_strDlgText[72]);
	SetDlgItemText(IDC_OX_QUIZ,g_strDlgText[73]);
	SetDlgItemText(IDC_REVIVE_OX_QUIZ_LOSER_BATTLE,g_strDlgText[74]);
	SetDlgItemText(IDC_COMPLETE_OX_QUIZ,g_strDlgText[75]);
#endif

	CDialog::OnInitDialog();
	SetLogFolder();

	CTimeInfo	currentTime;

	currentTime.update();
	currentTime.increaseHour(1);

	SetDlgItemInt(IDC_YEAR	,currentTime.m_wYear		,FALSE);
	SetDlgItemInt(IDC_MONTH	,currentTime.m_wMonth		,FALSE);
	SetDlgItemInt(IDC_DAY	,currentTime.m_wDay			,FALSE);
	SetDlgItemInt(IDC_HOUR	,currentTime.m_wHour		,FALSE);
	SetDlgItemInt(IDC_MINUTE,0							,FALSE);
	SetDlgItemInt(IDC_OX_QUIZ_COUNT_DOWN,30				,FALSE);
	SetDlgItemInt(IDC_LIMIT_OX_QUIZ_PLAYER_COUNT,500	,FALSE);


	SetDlgItemInt(IDC_WQ_YEAR	,currentTime.m_wYear	,FALSE);
	SetDlgItemInt(IDC_WQ_MONTH	,currentTime.m_wMonth	,FALSE);
	SetDlgItemInt(IDC_WQ_DAY	,currentTime.m_wDay		,FALSE);
	SetDlgItemInt(IDC_WQ_HOUR	,currentTime.m_wHour	,FALSE);
	SetDlgItemInt(IDC_WQ_MINUTE,0						,FALSE);
	SetDlgItemInt(IDC_WORD_QUIZ_COUNT_DOWN,30			,FALSE);
	SetDlgItemInt(IDC_LIMIT_WORD_QUIZ_PLAYER_COUNT,500	,FALSE);
	
	SetDlgItemInt(IDC_WORD_EVENT_HOUR ,  0 , FALSE);
	SetDlgItemInt(IDC_WORD_EVENT_MINITE ,  0 , FALSE);

	SetDlgItemInt(IDC_WORD_EVENT_BEGIN_YEAR		,currentTime.m_wYear 	,FALSE);
	SetDlgItemInt(IDC_WORD_EVENT_BEGIN_MONTH	,currentTime.m_wMonth	,FALSE);
	SetDlgItemInt(IDC_WORD_EVENT_BEGIN_DAY		,currentTime.m_wDay		,FALSE);
	SetDlgItemInt(IDC_WORD_EVENT_BEGIN_HOUR		,currentTime.m_wHour	,FALSE);
	SetDlgItemInt(IDC_WORD_EVENT_BEGIN_MINITE	,0						,FALSE);

	SetDlgItemInt(IDC_WORD_EVENT_END_YEAR		,currentTime.m_wYear  	,FALSE);
	SetDlgItemInt(IDC_WORD_EVENT_END_MONTH		,currentTime.m_wMonth	,FALSE);
	SetDlgItemInt(IDC_WORD_EVENT_END_DAY		,currentTime.m_wDay		,FALSE);
	SetDlgItemInt(IDC_WORD_EVENT_END_HOUR		,currentTime.m_wHour	,FALSE);
	SetDlgItemInt(IDC_WORD_EVENT_END_MINITE		,0						,FALSE);
	
	m_cbOX.AddString("  O  ");
	m_cbOX.AddString("  X  ");
	m_cbOX.SetCurSel(m_iSelectOXQuizSolution);

	for(int i=0;i<dBASIC_ITEM_COUNT;++i)
	{
		if(g_aBasicItem[i].m_wKind == eIK_SPECIAL)
			m_cbEventBuff.AddString(g_aBasicItem[i].m_strName);
	}
	
	for(int j =0;j<dEVENT_NAME_COUNT;++j)
	{
		m_cbEventKind.AddString(g_strEventName[j]);
	}
	

	{
		m_bIsAppointmentGoldRushEvent	=	FALSE;
		m_bIsCancelGoldRushEvent		=	FALSE;

		m_ebYear1.SetWindowText("0");
		m_ebYear2.SetWindowText("0");
		m_ebMonth1.SetWindowText("0");
		m_ebMonth2.SetWindowText("0");
		m_ebDay1.SetWindowText("0");
		m_ebDay2.SetWindowText("0");
		m_ebHour1.SetWindowText("0");
		m_ebHour2.SetWindowText("0");
		m_ebMinute1.SetWindowText("0");
		m_ebMinute2.SetWindowText("0");
		m_ebExp.SetWindowText("0");
		m_ebDropChance.SetWindowText("0");

		m_ebGRBeginComment.SetWindowText("");
		m_ebGREndComment.SetWindowText("");
	}

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEventManager::OnBookingOxQuizEvent() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	if (g_iSelectedServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	int		iYear,iMonth,iDay,iHour,iMinute,iLimitPlayerCount;
	BOOL	bTemp;

	iYear	=	GetDlgItemInt(IDC_YEAR	,&bTemp,FALSE);
	iMonth	=	GetDlgItemInt(IDC_MONTH	,&bTemp,FALSE);
	iDay	=	GetDlgItemInt(IDC_DAY	,&bTemp,FALSE);
	iHour	=	GetDlgItemInt(IDC_HOUR	,&bTemp,FALSE);
	iMinute	=	GetDlgItemInt(IDC_MINUTE,&bTemp,FALSE);
	iLimitPlayerCount	=	GetDlgItemInt(IDC_LIMIT_OX_QUIZ_PLAYER_COUNT,&bTemp,FALSE);

	char	*lpstrSelectWorldName	=	sList[g_iSelectedServer].strName;

	if	(cMSG::YESNO(dMSG_OX_QUIZ,dMSG_CHECK_SELECT_WORLD_FORM,lpstrSelectWorldName)==IDNO)
		return;

	if (cMSG::YESNO(dMSG_OX_QUIZ,dMSG_BOOKING_OX_QUIZ_FORM,lpstrSelectWorldName,iYear,iMonth,iDay,iHour,iMinute) == IDNO)
		return;

	SERVERStoWORLD_APPOINTMENT_QUIZ_EVENT	packet;
	packet.base.set(sizeof(SERVERStoWORLD_APPOINTMENT_QUIZ_EVENT),dSERVERStoWORLD_APPOINTMENT_QUIZ_EVENT);

	packet.wBeginYear	=	iYear;
	packet.wBeginMonth	=	iMonth;
	packet.wBeginDay	=	iDay;
	packet.wBeginHour	=	iHour;
	packet.wBeginMinute	=	iMinute;
	packet.wLimitPlayerCount=	iLimitPlayerCount;
	packet.wIsTest		=	FALSE;
	packet.wType		=	eET_OX_QUIZ;


	SendPacketToWorld(g_iSelectedServer,(char *)&packet);
}

void CEventManager::OnOxQuiz() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	if (g_iSelectedServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	char	strQuestion[256];
	int		iSolution,iCount;
	BOOL	bTemp;
	
	GetDlgItemText(IDC_QUESTION,strQuestion,512);

	iSolution	=	m_cbOX.GetCurSel();
	iCount		=	GetDlgItemInt(IDC_OX_QUIZ_COUNT_DOWN	,&bTemp,FALSE);
	m_editOxQuizQuestion.GetWindowText(strQuestion,255);

	if (strlen(strQuestion) <= 2)
		return;

	char	*astrOxQuestionSolution[]	=	{"O","X"};
	char	*lpstrSelectWorldName	=	sList[g_iSelectedServer].strName;

	if	(cMSG::YESNO(dMSG_OX_QUIZ,dMSG_CHECK_SELECT_WORLD_FORM,lpstrSelectWorldName)==IDNO)
		return;

	if	(cMSG::YESNO(dMSG_OX_QUIZ,dMSG_OX_QUIZ_FORM,lpstrSelectWorldName,strQuestion,astrOxQuestionSolution[iSolution])==IDNO)
		return;

	SERVERStoWORLD_PRODUCE_OX_QUIZ	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_PRODUCE_OX_QUIZ);

	strcpy(packet.strQuestion,strQuestion);
	packet.wSolution	=	iSolution;
	packet.wCount		=	iCount;

	SendPacketToWorld(g_iSelectedServer,(char *)&packet);
}

void CEventManager::OnCompleteOxQuiz() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	if (g_iSelectedServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	char	*lpstrSelectWorldName	=	sList[g_iSelectedServer].strName;

	if	(cMSG::YESNO(dMSG_OX_QUIZ,dMSG_CHECK_SELECT_WORLD_FORM,lpstrSelectWorldName)==IDNO)
		return;

	if	(cMSG::YESNO(dMSG_OX_QUIZ,dMSG_ARE_YOU_END_OX_QUIZ_FORM,lpstrSelectWorldName)==IDNO)
		return;

	SERVERStoWORLD_END_QUIZ_EVENT	packet;
	
	packet.base.set(sizeof(packet),dSERVERStoWORLD_END_QUIZ_EVENT);
	packet.wType	=	eET_OX_QUIZ;

	SendPacketToWorld(g_iSelectedServer,(char *)&packet);
}

void CEventManager::OnCancelOxQuizEvent() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	if (g_iSelectedServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	char	*lpstrSelectWorldName	=	sList[g_iSelectedServer].strName;

	if	(cMSG::YESNO(dMSG_OX_QUIZ,dMSG_CHECK_SELECT_WORLD_FORM,lpstrSelectWorldName)==IDNO)
		return;

	if	(cMSG::YESNO(dMSG_OX_QUIZ,dMSG_ARE_YOU_CANCEL_OX_QUIZ_FORM,lpstrSelectWorldName)==IDNO)
		return;

	SERVERStoWORLD_CANCEL_QUIZ_EVENT	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_CANCEL_QUIZ_EVENT);

	packet.wType	=	eET_OX_QUIZ;

	SendPacketToWorld(g_iSelectedServer,(char *)&packet);
}

void CEventManager::OnBookingOxQuizEventForTest() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	if (g_iSelectedServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	int		iYear,iMonth,iDay,iHour,iMinute,iLimitPlayerCount;
	BOOL	bTemp;

	iYear	=	GetDlgItemInt(IDC_YEAR	,&bTemp,FALSE);
	iMonth	=	GetDlgItemInt(IDC_MONTH	,&bTemp,FALSE);
	iDay	=	GetDlgItemInt(IDC_DAY	,&bTemp,FALSE);
	iHour	=	GetDlgItemInt(IDC_HOUR	,&bTemp,FALSE);
	iMinute	=	GetDlgItemInt(IDC_MINUTE,&bTemp,FALSE);
	iLimitPlayerCount	=	GetDlgItemInt(IDC_LIMIT_OX_QUIZ_PLAYER_COUNT,&bTemp,FALSE);

	char	*lpstrSelectWorldName	=	sList[g_iSelectedServer].strName;

	if	(cMSG::YESNO(dMSG_OX_QUIZ_TEST,dMSG_CHECK_SELECT_WORLD_FORM,lpstrSelectWorldName)==IDNO)
		return;

	if (cMSG::YESNO(dMSG_OX_QUIZ_TEST,dMSG_BOOKING_OX_QUIZ_FORM,lpstrSelectWorldName,iYear,iMonth,iDay,iHour,iMinute) == IDNO)
		return;

	SERVERStoWORLD_APPOINTMENT_QUIZ_EVENT	packet;
	packet.base.set(sizeof(SERVERStoWORLD_APPOINTMENT_QUIZ_EVENT),dSERVERStoWORLD_APPOINTMENT_QUIZ_EVENT);

	packet.wBeginYear	=	iYear;
	packet.wBeginMonth	=	iMonth;
	packet.wBeginDay	=	iDay;
	packet.wBeginHour	=	iHour;
	packet.wBeginMinute	=	iMinute;
	packet.wLimitPlayerCount=	iLimitPlayerCount;
	packet.wIsTest		=	TRUE;
	packet.wType		=	eET_OX_QUIZ;

	SendPacketToWorld(g_iSelectedServer,(char *)&packet);
}

void CEventManager::OnReviveOxQuizLoserBattle() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	if	(g_iSelectedServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	char	*lpstrSelectWorldName	=	sList[g_iSelectedServer].strName;

	if	(cMSG::YESNO(dMSG_OX_QUIZ,dMSG_CHECK_SELECT_WORLD_FORM,lpstrSelectWorldName)==IDNO)
		return;

	if	(cMSG::YESNO(dMSG_OX_QUIZ,dMSG_REVIVE_OX_QUIZ_LOSER_BATTLE,lpstrSelectWorldName) == IDNO)
		return;

	SERVERStoWORLD_OX_QUIZ_LOSER_REVIVE_BATTLE	packet;
	packet.base.set(sizeof(SERVERStoWORLD_OX_QUIZ_LOSER_REVIVE_BATTLE),dSERVERStoWORLD_OX_QUIZ_LOSER_REVIVE_BATTLE);

	SendPacketToWorld(g_iSelectedServer,(char *)&packet);
}

void CEventManager::OnGrApply() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#else
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))

		return;
#endif

	if	(g_iSelectedServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	// TODO: Add your control notification handler code here
	cGoldRushDefine	goldRush;

	goldRush.m_begin.year				=	GetDlgItemInt(IDC_GR_YEAR1)%2000;
	goldRush.m_end.year					=	GetDlgItemInt(IDC_GR_YEAR2)%2000;
	goldRush.m_begin.month				=	GetDlgItemInt(IDC_GR_MONTH1);
	goldRush.m_end.month				=	GetDlgItemInt(IDC_GR_MONTH2);
	goldRush.m_begin.day				=	GetDlgItemInt(IDC_GR_DAY1);
	goldRush.m_end.day					=	GetDlgItemInt(IDC_GR_DAY2);
	goldRush.m_begin.hour				=	GetDlgItemInt(IDC_GR_HOUR1);
	goldRush.m_end.hour					=	GetDlgItemInt(IDC_GR_HOUR2);
	goldRush.m_begin.minute				=	GetDlgItemInt(IDC_GR_MINUTE1);
	goldRush.m_end.minute				=	GetDlgItemInt(IDC_GR_MINUTE2);
	goldRush.m_begin.second				=	0;
	goldRush.m_end.second				=	0;

	goldRush.m_wCorrectItemDropChance	=	GetDlgItemInt(IDC_GR_ITEM_DROP_CHANCE);
	goldRush.m_wBoostExperience			=	GetDlgItemInt(IDC_GR_EXP);

	m_ebGRBeginComment.GetWindowText(goldRush.m_strBeginComment,sizeof(goldRush.m_strBeginComment)-1);
	m_ebGREndComment.GetWindowText(goldRush.m_strEndComment,sizeof(goldRush.m_strEndComment)-1);

	if (strlen(goldRush.m_strBeginComment)<= 0)
	{
		MessageBox("please input begin comment"," ");
		return;
	}
	if (strlen(goldRush.m_strEndComment)<= 0)
	{
		MessageBox("please input end comment"," ");
		return;
	}

	CTimeInfo	currentTime;

	currentTime.update();

	currentTime.m_wYear		-=	2000;

	DWORD	dwCurrentTime	=	currentTime.m_wYear*60*24*31*12+currentTime.m_wMonth*60*24*31+currentTime.m_wDay*60*24+currentTime.m_wHour*60+currentTime.m_wMinute;
	DWORD	dwBeginTime		=	goldRush.m_begin.year*60*24*31*12+goldRush.m_begin.month*60*24*31+goldRush.m_begin.day*60*24+goldRush.m_begin.hour*60+goldRush.m_begin.minute;
	DWORD	dwEndTime		=	goldRush.m_end.year*60*24*31*12+goldRush.m_end.month*60*24*31+goldRush.m_end.day*60*24+goldRush.m_end.hour*60+goldRush.m_end.minute;

	if	(dwEndTime	<	dwBeginTime)
	{
		MessageBox("incorrect term"," ");
		return;
	}

	if	(goldRush.m_wBoostExperience	<	100	||	goldRush.m_wCorrectItemDropChance	<	100)
	{
		MessageBox("incorrect gold rush Data"," ");

		return;
	}
	
	if	(cMSG::YESNO("Gold Rush!!","Term %d/%d/%d %d:%d ~ %d/%d/%d %d:%d\n\nExperience Boost %d %%\n\nCorrect Item Drop Chance %d%%\n\nbegin comment : %s\n\nend comment : %s\n\n\n\ndo you want apply this info? in server [%s]",
		goldRush.m_begin.year+2000,goldRush.m_begin.month,goldRush.m_begin.day,goldRush.m_begin.hour,goldRush.m_begin.minute,
		goldRush.m_end.year+2000,goldRush.m_end.month,goldRush.m_end.day,goldRush.m_end.hour,goldRush.m_end.minute,
		goldRush.m_wBoostExperience,goldRush.m_wCorrectItemDropChance,
		goldRush.m_strBeginComment,goldRush.m_strEndComment,
		sList[g_iSelectedServer].strName)	==	IDNO)
		return;

	{
		if	(!g_db.Connect(	sList[g_iSelectedServer].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))
		{
			MessageBox(g_strToolMessage[38]);
			return;
		}

		g_db.spEvent_GoldRush_Create(&goldRush,strUserName);
		g_db.Close();
	}
	
	m_bIsAppointmentGoldRushEvent	=	TRUE;

	OnGetSchedule();	//	다시 정보를 요청해 실제로 저장된 데이터를 확인한다.
}

void CEventManager::OnGrCancel() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#else
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#endif

	if (g_iSelectedServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	// TODO: Add your control notification handler code here
	cGoldRushDefine	goldRush;

	goldRush.reset();
	
	goldRush.m_begin.year	=	1;
	goldRush.m_begin.month	=	1;
	goldRush.m_begin.day	=	1;
	goldRush.m_begin.hour	=	1;
	goldRush.m_begin.minute	=	0;

	goldRush.m_end.year		=	1;
	goldRush.m_end.month	=	1;
	goldRush.m_end.day		=	1;
	goldRush.m_end.hour		=	1;
	goldRush.m_end.minute	=	0;

	if	(cMSG::YESNO("Gold Rush!!","Cancel Gold Rush Event in Server [%s]\n\nReally?",sList[g_iSelectedServer].strName)	==	IDNO)
		return;

	{
		if	(!g_db.Connect(	sList[g_iSelectedServer].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))
		{
			MessageBox(g_strToolMessage[38]);
			return;
		}

		g_db.spEvent_GoldRush_Create(&goldRush,strUserName);
		g_db.Close();
	}

	m_bIsCancelGoldRushEvent	=	TRUE;

	OnGetSchedule();	//	다시 정보를 요청해 실제로 저장된 데이터를 확인한다.
}

void CEventManager::OnGetSchedule() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	if	(g_iSelectedServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	cGoldRushDefine	goldRush;

	if	(!g_db.Connect(	sList[g_iSelectedServer].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))
	{
		MessageBox(g_strToolMessage[38]);
		return;
	}

	g_db.spEvent_GoldRush_Get(&goldRush);
	g_db.Close();

	m_ebYear1.SetWindowText(_ms("%d",goldRush.m_begin.year+2000));
	m_ebYear2.SetWindowText(_ms("%d",goldRush.m_end.year+2000));
	m_ebMonth1.SetWindowText(_ms("%d",goldRush.m_begin.month));
	m_ebMonth2.SetWindowText(_ms("%d",goldRush.m_end.month));
	m_ebDay1.SetWindowText(_ms("%d",goldRush.m_begin.day));
	m_ebDay2.SetWindowText(_ms("%d",goldRush.m_end.day));
	m_ebHour1.SetWindowText(_ms("%d",goldRush.m_begin.hour));
	m_ebHour2.SetWindowText(_ms("%d",goldRush.m_end.hour));
	m_ebMinute1.SetWindowText(_ms("%d",goldRush.m_begin.minute));
	m_ebMinute2.SetWindowText(_ms("%d",goldRush.m_end.minute));
	m_ebExp.SetWindowText(_ms("%d",goldRush.m_wBoostExperience));
	m_ebDropChance.SetWindowText(_ms("%d",goldRush.m_wCorrectItemDropChance));

	m_ebGRBeginComment.SetWindowText(goldRush.m_strBeginComment);
	m_ebGREndComment.SetWindowText(goldRush.m_strEndComment);

	if	(m_bIsAppointmentGoldRushEvent)
		MessageBox("please check event info","Appointment Gold Rush Event");
	else
	if	(m_bIsCancelGoldRushEvent)
		MessageBox("please check event info","Cancel Gold Rush Event");
	else
		MessageBox("receive Gold Rush Event info"," ");

	m_bIsAppointmentGoldRushEvent	=	FALSE;
	m_bIsCancelGoldRushEvent		=	FALSE;
}

void CEventManager::OnFillTime() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	CTimeInfo	currentTime;

	currentTime.update();

	m_ebYear1.SetWindowText(_ms("%d",currentTime.m_wYear));
	m_ebYear2.SetWindowText(_ms("%d",currentTime.m_wYear));
	m_ebMonth1.SetWindowText(_ms("%d",currentTime.m_wMonth));
	m_ebMonth2.SetWindowText(_ms("%d",currentTime.m_wMonth));
	m_ebDay1.SetWindowText(_ms("%d",currentTime.m_wDay));
	m_ebDay2.SetWindowText(_ms("%d",currentTime.m_wDay));
	m_ebHour1.SetWindowText(_ms("%d",currentTime.m_wHour));
	m_ebHour2.SetWindowText(_ms("%d",currentTime.m_wHour));
	m_ebMinute1.SetWindowText(_ms("%d",0));
	m_ebMinute2.SetWindowText(_ms("%d",0));
	m_ebExp.SetWindowText(_ms("%d",200));
	m_ebDropChance.SetWindowText(_ms("%d",200));

	// TODO: Add your control notification handler code here
	
}

void CEventManager::OnAppointmentWordQuiz() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	if	(g_iSelectedServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	int		iYear,iMonth,iDay,iHour,iMinute,iLimitPlayerCount;
	BOOL	bTemp;

	iYear				=	GetDlgItemInt(IDC_WQ_YEAR	,&bTemp,FALSE);
	iMonth				=	GetDlgItemInt(IDC_WQ_MONTH	,&bTemp,FALSE);
	iDay				=	GetDlgItemInt(IDC_WQ_DAY	,&bTemp,FALSE);
	iHour				=	GetDlgItemInt(IDC_WQ_HOUR	,&bTemp,FALSE);
	iMinute				=	GetDlgItemInt(IDC_WQ_MINUTE,&bTemp,FALSE);
	iLimitPlayerCount	=	GetDlgItemInt(IDC_LIMIT_WORD_QUIZ_PLAYER_COUNT,&bTemp,FALSE);

	char	*lpstrSelectWorldName	=	sList[g_iSelectedServer].strName;

	if	(cMSG::YESNO("Word Quiz",dMSG_CHECK_SELECT_WORLD_FORM,lpstrSelectWorldName)==IDNO)
		return;

	if	(cMSG::YESNO("Word Quiz",dMSG_APPOINTMENT_WORD_OX_QUIZ_FORM,lpstrSelectWorldName,iYear,iMonth,iDay,iHour,iMinute) == IDNO)
		return;

	SERVERStoWORLD_APPOINTMENT_QUIZ_EVENT	packet;
	packet.base.set(sizeof(SERVERStoWORLD_APPOINTMENT_QUIZ_EVENT),dSERVERStoWORLD_APPOINTMENT_QUIZ_EVENT);
	ZeroMemory(packet.strEventName , 128);

	packet.wBeginYear		=	iYear;
	packet.wBeginMonth		=	iMonth;
	packet.wBeginDay		=	iDay;
	packet.wBeginHour		=	iHour;
	packet.wBeginMinute		=	iMinute;
	packet.wIsTest			=	FALSE;
	packet.wLimitPlayerCount=	iLimitPlayerCount;
	packet.wType			=	eET_WORD_QUIZ;
	GetDlgItemText(IDC_WORD_QUIZ_NAME , packet.strEventName , 128);

	if(STRICMP(packet.strEventName ,"" ) == 0)
	{
		ERRMSG("Check Event Name!!");
		return;
	}

	SendPacketToWorld(g_iSelectedServer,(char *)&packet);
}

void CEventManager::OnCancelWordQuiz() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	if	(g_iSelectedServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	char	*lpstrSelectWorldName	=	sList[g_iSelectedServer].strName;

	if	(cMSG::YESNO("Word Quiz",dMSG_CHECK_SELECT_WORLD_FORM,lpstrSelectWorldName)==IDNO)
		return;

	if	(cMSG::YESNO("Word Quiz",dMSG_ARE_YOU_CANCEL_WORD_QUIZ_FORM,lpstrSelectWorldName)==IDNO)
		return;

	SERVERStoWORLD_CANCEL_QUIZ_EVENT	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_CANCEL_QUIZ_EVENT);

	packet.wType	=	eET_WORD_QUIZ;

	SendPacketToWorld(g_iSelectedServer,(char *)&packet);
}

void CEventManager::OnSettingWqQuestion() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	if	(g_iSelectedServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	char	strQuestion[256];
	char	strSolution[32];
	int		iCount;
	BOOL	bTemp;

	GetDlgItemText(IDC_WQ_QUESTION,strQuestion,256);
	GetDlgItemText(IDC_WQ_SOLUTION,strSolution,32);

	iCount		=	GetDlgItemInt(IDC_WORD_QUIZ_COUNT_DOWN	,&bTemp,FALSE);

	if	(strlen(strQuestion) <= 2)
		return;

	char	*lpstrSelectWorldName	=	sList[g_iSelectedServer].strName;

	if	(cMSG::YESNO(dMSG_OX_QUIZ,dMSG_CHECK_SELECT_WORLD_FORM,lpstrSelectWorldName)==IDNO)
		return;

	if	(cMSG::YESNO(dMSG_OX_QUIZ,dMSG_WORD_QUIZ_FORM,lpstrSelectWorldName,strQuestion,strSolution)==IDNO)
		return;

	SERVERStoWORLD_PRODUCE_WORD_QUIZ	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_PRODUCE_WORD_QUIZ);

	strcpy(packet.strQuestion,strQuestion);
	strcpy(packet.strSolution,strSolution);
	packet.wCount		=	iCount;

	SendPacketToWorld(g_iSelectedServer,(char *)&packet);
}

void CEventManager::OnChangeWordQuizRound() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	if	(cMSG::YESNO("Change Word Quiz Round","Really?")==IDNO)
		return;

	SERVERStoWORLD_CHANGE_WORD_QUIZ_ROUND	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_CHANGE_WORD_QUIZ_ROUND);
	
	SendPacketToWorld(g_iSelectedServer,(char *)&packet);
}

void CEventManager::OnEndWordQuiz() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	if	(cMSG::YESNO("End Word Quiz","Really?")==IDNO)
		return;
	
	SERVERStoWORLD_END_QUIZ_EVENT	packet;
	
	packet.base.set(sizeof(packet),dSERVERStoWORLD_END_QUIZ_EVENT);
	packet.wType	=	eET_WORD_QUIZ;

	SendPacketToWorld(g_iSelectedServer,(char *)&packet);
}

void CEventManager::OnTestWordQuiz() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	if	(g_iSelectedServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	int		iYear,iMonth,iDay,iHour,iMinute,iLimitPlayerCount;
	BOOL	bTemp;

	iYear	=	GetDlgItemInt(IDC_WQ_YEAR	,&bTemp,FALSE);
	iMonth	=	GetDlgItemInt(IDC_WQ_MONTH	,&bTemp,FALSE);
	iDay	=	GetDlgItemInt(IDC_WQ_DAY	,&bTemp,FALSE);
	iHour	=	GetDlgItemInt(IDC_WQ_HOUR	,&bTemp,FALSE);
	iMinute	=	GetDlgItemInt(IDC_WQ_MINUTE,&bTemp,FALSE);
	iLimitPlayerCount	=	GetDlgItemInt(IDC_LIMIT_WORD_QUIZ_PLAYER_COUNT,&bTemp,FALSE);

	char	*lpstrSelectWorldName	=	sList[g_iSelectedServer].strName;

	if	(cMSG::YESNO("Test Word Quiz",dMSG_CHECK_SELECT_WORLD_FORM,lpstrSelectWorldName)==IDNO)
		return;

	if	(cMSG::YESNO("Test Word Quiz",dMSG_APPOINTMENT_WORD_OX_QUIZ_FORM,lpstrSelectWorldName,iYear,iMonth,iDay,iHour,iMinute) == IDNO)
		return;

	SERVERStoWORLD_APPOINTMENT_QUIZ_EVENT	packet;
	packet.base.set(sizeof(SERVERStoWORLD_APPOINTMENT_QUIZ_EVENT),dSERVERStoWORLD_APPOINTMENT_QUIZ_EVENT);
	ZeroMemory(packet.strEventName , 128);

	packet.wBeginYear	=	iYear;
	packet.wBeginMonth	=	iMonth;
	packet.wBeginDay	=	iDay;
	packet.wBeginHour	=	iHour;
	packet.wBeginMinute	=	iMinute;
	packet.wIsTest		=	TRUE;
	packet.wLimitPlayerCount=	iLimitPlayerCount;
	packet.wType		=	eET_WORD_QUIZ;

	GetDlgItemText(IDC_WORD_QUIZ_NAME , packet.strEventName , 128);

	if(STRICMP(packet.strEventName ,"" ) == 0)
	{
		ERRMSG("Check Event Name!!");
		return;
	}

	SendPacketToWorld(g_iSelectedServer,(char *)&packet);
}

void CEventManager::OnEventListButtonSerch() 
{
	if(g_iSelectedServer == -1)
	{
		ERRMSG("Select Server");
		return;
	}

	OPENFILENAME	OFN;
	char			szBuf[256];

	memset(szBuf, 0, 256);
	memset(&OFN, 0, sizeof(OPENFILENAME));

	OFN.lStructSize = sizeof(OPENFILENAME);			//사이즈 구조체
	OFN.hwndOwner	= NULL;						//부모윈도우
	OFN.lpstrFilter	= "EventList(txt)\0*.txt\0";			//파일 형식
	OFN.lpstrFile	= szBuf;						//파일이름이 담길곳
	OFN.Flags		= OFN_EXPLORER;	// | OFN_ALLOWMULTISELECT;	
	OFN.nMaxFile	= 255;			

	//만약 파일을 선택 하고 확인을 눌렀다면
	if(GetOpenFileName(&OFN) == false)	return;		

	m_EventList.load(sList[g_iSelectedServer].strName , OFN.lpstrFile);
}

void
cCheckEventList::load(char* _wServerName , char* _strFileName)
{
	SetLogFolder();
	
	cDECODER		decoder;
	
	if(!decoder.Upload(_strFileName , NULL))
		ERRMSG("Can`t Read EventCharacterList.");

	for (int i = 0 ; i < 20 ; i++)
	{
		ZeroMemory(m_ServerName[i] , dMAX_LIST_COUNT);
		ZeroMemory(m_strName[i] , dMAX_LIST_COUNT);
	}
	
	int iCount = 0 ;
	while (1)
	{
		char* strWord	= decoder.GetWordVerBC();
		
		if(!STRICMP("END" , strWord))
			break;
		else
		{
			if (STRICMP(_wServerName,strWord) == 0)		//서버 인덱스가 일치하는 플레이어만 잃는다.
			{			
				strcpy(&m_ServerName[iCount][0] , strWord);
				strcpy(&m_strName[iCount][0] , decoder.GetWord());
				iCount++;
			}
			else
			{
				decoder.GetWord();			//그냥 버퍼를 잃는다.
			}
		}
	}

	m_iListCount = iCount;

	MessageBox(NULL , _ms("%d User Loading Complite!!" , iCount) , "ERROR" , MB_OK);
}

void CEventManager::OnEventListButtonOk() 
{
	// TODO: Add your control notification handler code here

	if(g_iSelectedServer == -1)
	{
		ERRMSG("Select Server");
		return;
	}

	l_DBTemp.m_bIsNormalDB	=	TRUE;
	
	if	(!l_DBTemp.Connect(	sList[g_iSelectedServer].strDBIP	,strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))
	{
		MessageBox("DB Connet Error");
		return;
	}

	HANDLE	hFile;
	DWORD	dwRead;

	hFile = CreateFile("FailedList.txt", GENERIC_WRITE, 0, NULL,
				   CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


	for (int i = 0 ; i < m_EventList.m_iListCount ; i++)
	{
		BOOL isFailed = FALSE;

		//디비에서 케릭 정보를 로딩하자
		if	(!l_DBTemp.loadData(NULL,m_EventList.m_strName[i],&l_amTemp))
		{
			isFailed = TRUE;
		}
		
		if	(l_amTemp.m_iLoadDataCount	<=	0)
		{
			MessageBox(_ms("NAME:%s Can`t Load Data" , m_EventList.m_strName[i]));
			isFailed = TRUE;
		}
		//데이터 로드 완료.

		//윈도우 Edit 창의 텍스트 로딩.
		char strBuffer[256];
		ZeroMemory(strBuffer , 256);

		if(GetDlgItemText(IDC_EVENT_LIST_EDIT , strBuffer , 256) == NULL)
		{
			isFailed = TRUE;
		}

		//
		cInventory Invetory;
		
		Invetory.init(&l_amTemp , 0 , g_iSelectedServer);

		BOOL isInsertBank = IsDlgButtonChecked(IDC_WORD_EVENT_LIST_CHECK);

		if(!Invetory.OnAddByEventList(strBuffer , isInsertBank))
			isFailed = TRUE;

		CAvatarData		*lpPlayer	=	&l_amTemp.m_aData[0];

		if	(!g_toolLog.savePlayer(lpPlayer->m_strId,lpPlayer->m_strName,sList[g_iSelectedServer].strName))
		{
			isFailed = TRUE;
		}

		if	(!l_DBTemp.saveData((cP_PLAYER_SAVE_DATA *)lpPlayer,FALSE ,TRUE))
		{
			isFailed = TRUE;
		}

		if	(!g_toolLog.saveBankData(lpPlayer->m_strId,sList[g_iSelectedServer].strName))
		{
			isFailed = TRUE;
		}

		if (!l_DBTemp.spSaveBank(&l_amTemp.m_bank))
		{
			isFailed = TRUE;
		}
	
		if (isFailed)
		{
			char charTep = 9;
			char charEnter1 = 0x0D;
			char charEnter2 = 0x0A;

			WriteFile(hFile, m_EventList.m_ServerName[i], strlen(m_EventList.m_ServerName[i]), &dwRead, NULL);
			WriteFile(hFile, &charTep , 1 , &dwRead , NULL);
			WriteFile(hFile, m_EventList.m_strName[i], strlen(m_EventList.m_strName[i]), &dwRead, NULL);
			WriteFile(hFile, &charEnter1 , 1 , &dwRead , NULL);		//두문자를 쓰기위해...-_-;
			WriteFile(hFile, &charEnter2 , 1 , &dwRead , NULL);
		}
	}
	l_DBTemp.Close();
	CloseHandle(hFile);

	cMSG::Put("ITEM ADDITION","Complete!!");
}


void CEventManager::OnChattingEventButtonSet() 
{
	SendWorldToWordEvent(FALSE);
}


void CEventManager::OnChattingEventButtonRelease() 
{
	if(g_iSelectedServer == -1)
	{
		ERRMSG("Select Server");
		return;
	}

	// TODO: Add your control notification handler code here
	if	(cMSG::YESNO("End Event","Really?")==IDNO)
		return;
	
	SERVERStoWORLD_SET_EVENT_WORD	packet;
	ZeroMemory(&packet , sizeof(SERVERStoWORLD_SET_EVENT_WORD));

	packet.base.set(sizeof(packet),dSERVERStoWORLD_WORD_EVENT_END);
	
	if(!SendPacketToWorld(g_iSelectedServer,(char *)&packet))
	{
		ERRMSG("Sending Packet Error Retry!!");
		return;
	}
}


void CEventManager::SendWorldToWordEvent(BOOL isStartNow)
{		// 마지막 수정일 : 09.10.09
	if	(g_iSelectedServer == -1)
	{
		ERRMSG("Select Server");
		return;
	}
	
	// TODO: Add your control notification handler code here
	if	(cMSG::YESNO("Change Event Word ","Really?")==IDNO)
		return;
	
	SERVERStoWORLD_SET_EVENT_WORD	packet;
	ZeroMemory(&packet , sizeof(SERVERStoWORLD_SET_EVENT_WORD));
	
	if(isStartNow)
		packet.base.set(sizeof(packet),dSERVERStoWORLD_WORD_EVENT_START);
	else
		packet.base.set(sizeof(packet),dSERVERStoWORLD_CHANGE_EVENT_WORD);

	char TempBuf[256];
	ZeroMemory(TempBuf , 256);
	
	if(GetDlgItemText(IDC_WORD_EVENT_MINITE , TempBuf , 256) == 0)
	{
		ERRMSG("Edit Window Error!!");
		return;
	}
	
	//분을 대입
	packet.dwTimeInfo = atoi(TempBuf);
	
	if(GetDlgItemText(IDC_WORD_EVENT_HOUR , TempBuf , 256) == 0)
	{
		ERRMSG("Edit Window Error!!");
		return;
	}
	
	//시간을 추가.
	packet.dwTimeInfo += atoi(TempBuf) * 60;
	
	if(packet.dwTimeInfo == 0)
		ERRMSG("Set Time!!");
	

	packet.wBeginTimeYear		= GetDlgItemInt(IDC_WORD_EVENT_BEGIN_YEAR) - 2000;
	packet.wBeginTimeMonth		= GetDlgItemInt(IDC_WORD_EVENT_BEGIN_MONTH);
	packet.wBeginTimeDay		= GetDlgItemInt(IDC_WORD_EVENT_BEGIN_DAY);
	packet.wBeginTimeHour		= GetDlgItemInt(IDC_WORD_EVENT_BEGIN_HOUR);
	packet.wBeginTimeMinite		= GetDlgItemInt(IDC_WORD_EVENT_BEGIN_MINITE);
	packet.wEndTimeYear			= GetDlgItemInt(IDC_WORD_EVENT_END_YEAR) - 2000;
	packet.wEndTimeMonth		= GetDlgItemInt(IDC_WORD_EVENT_END_MONTH);
	packet.wEndTimeDay			= GetDlgItemInt(IDC_WORD_EVENT_END_DAY);
	packet.wEndTimeHour			= GetDlgItemInt(IDC_WORD_EVENT_END_HOUR);
	packet.wEndTimeMinite		= GetDlgItemInt(IDC_WORD_EVENT_END_MINITE);	

	packet.bf16EventKind		= m_cbEventKind.GetCurSel();		// 이벤트 ,,, 종류...

	if(packet.bf16EventKind == 0xffff)
	{// 이벤트 선택을 안했으면 에러메세지!
		ERRMSG("Event Empty!!");
		return;
	}

	packet.bf16BuffSerial		= 0xffff;
	
	if(packet.bf16EventKind == eEVENT_CHRISTMAS)		
	{
		char strItemName[256] = "";
		
		GetDlgItemText(IDC_EVENT_BUFF, strItemName,256);
		for(int i=0;i<dBASIC_ITEM_COUNT;++i)
		{
			if(g_aBasicItem[i].m_wKind != eIK_SPECIAL)
				continue;
			
			if(strcmp(g_aBasicItem[i].m_strName , strItemName) == 0)
			{
				packet.bf16BuffSerial = i;
				break;
			}
			
		}

		if(packet.bf16BuffSerial == 0xffff)
		{// 크리스마스 이벤트인데 버프선택안했으면 에러 메세지!
			ERRMSG("Buff Empty!!");
			return;
		}
	}
	if(GetDlgItemText(IDC_CHATTING_EVENT_EDIT , packet.strWordOrString , 256) == 0)
	{
		ERRMSG("Edit Window Error!!");
		return;
	}

	if(GetDlgItemText(IDC_CHATTING_EVENT_STARTMENT_EDIT , packet.strBeginMent , 256) == 0)
	{
		ERRMSG("Start Ment Edit Empty!!");
	}

	if(GetDlgItemText(IDC_CHATTING_EVENT_ENDMENT_EDIT , packet.strEndMent , 256) == 0)
	{
		ERRMSG("End Ment Edit Empty!!");
	}

	if(GetDlgItemText(IDC_CHATTING_EVENT_ACTIVEMENT_EDIT , packet.strActiveMent , 256) == 0)
	{
		ERRMSG("Active Ment Edit Empty!!");
	}

	if	(!SendPacketToWorld(g_iSelectedServer,(char *)&packet))
	{
		ERRMSG("Sending Packet Error Retry!!");
		return;
	}

	MessageBox("Setting Complate!!");
}

void CEventManager::OnChangeMessageOfSoundOfLedersBell() 
{
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;

	if	(g_iSelectedServer == -1)
	{
		ERRMSG("Select Server");
		return;
	}
	
	char	strText[128];

	GetDlgItemText(IDC_TEXT_SOUND_OF_LEDERS_BELL,strText,100);

	if	(cMSG::YESNO("change message","%s\n\nreally?",strText)	==	IDNO)
		return;

	SERVERStoWORLD_CHANGE_MSG_OF_LEDERS_BELL	packet;
	ZeroMemory(&packet , sizeof(SERVERStoWORLD_CHANGE_MSG_OF_LEDERS_BELL));

	packet.base.set(sizeof(packet),dSERVERStoWORLD_CHANGE_MSG_OF_LEDERS_BELL);
	strcpy(packet.strMsg,strText);
	
	if	(!SendPacketToWorld(g_iSelectedServer,(char *)&packet))
	{
		ERRMSG("Sending Packet Error Retry!!");
		return;
	}
}

void CEventManager::OnResetSoundOfLedersBell() 
{
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;

	if	(g_iSelectedServer == -1)
	{
		ERRMSG("Select Server");
		return;
	}
	
	if	(cMSG::YESNO("Reset Event","Really?")==IDNO)
		return;

	if	(!SendWP_SimpleWork(g_iSelectedServer,eSWAW_RESET_SOUND_OF_LEDERS_BELL_COOLTIME,0))
	{
		ERRMSG("Sending Packet Error Retry!!");
		return;
	}
}

void CEventManager::OnCancelSoundOfLedersBell() 
{
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;

	if	(g_iSelectedServer == -1)
	{
		ERRMSG("Select Server");
		return;
	}
	
	if	(cMSG::YESNO("Cancel Event","Really?")==IDNO)
		return;

	if	(!SendWP_SimpleWork(g_iSelectedServer,eSWAW_RESET_SOUND_OF_LEDERS_BELL_COOLTIME,1))
	{
		ERRMSG("Sending Packet Error Retry!!");
		return;
	}
}

void CEventManager::OnCheckEventInfo() 
{
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;

	if	(g_iSelectedServer == -1)
	{
		ERRMSG("Select Server");
		return;
	}
	
	if	(!SendWP_SimpleWork(g_iSelectedServer,eSWAW_ASK_SOUND_OF_LEDERS_BELL_EVENT_INFO,1))
	{
		ERRMSG("Sending Packet Error Retry!!");
		return;
	}
}

void CEventManager::OnLottoEvent() 
{
	CLottoEvent	dlg;

	dlg.DoModal();
}

void CEventManager::OnEventTime() 
{
	CEventTimeManager	dlg;
	
	dlg.DoModal();
}
