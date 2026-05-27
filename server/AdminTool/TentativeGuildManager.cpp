// TentativeGuildManager.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "TentativeGuildManager.h"
#include "CMessage.h"
#include "cSRVUTIL.h"
#include "cPACKET_WORLDSERVER.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTentativeGuildManager dialog

IMPLEMENT_DYNCREATE(CTentativeGuildManager, CPropertyPage)

CTentativeGuildManager::CTentativeGuildManager() : CPropertyPage(CTentativeGuildManager::IDD)
{
	//{{AFX_DATA_INIT(CPAGE_USERSETTING)
	//}}AFX_DATA_INIT
}

CTentativeGuildManager::~CTentativeGuildManager()
{
}

void CTentativeGuildManager::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTentativeGuildManager)
	DDX_Control(pDX, IDC_BATTLE_GUILD2, m_ebGuild2Name);
	DDX_Control(pDX, IDC_BATTLE_GUILD1, m_ebGuild1Name);
	DDX_Control(pDX, IDC_FIELD9, m_ctlField9);
	DDX_Control(pDX, IDC_FIELD8, m_ctlField8);
	DDX_Control(pDX, IDC_FIELD7, m_ctlField7);
	DDX_Control(pDX, IDC_FIELD6, m_ctlField6);
	DDX_Control(pDX, IDC_FIELD5, m_ctlField5);
	DDX_Control(pDX, IDC_FIELD4, m_ctlField4);
	DDX_Control(pDX, IDC_FIELD3, m_ctlField3);
	DDX_Control(pDX, IDC_FIELD2, m_ctlField2);
	DDX_Control(pDX, IDC_FIELD10, m_ctlField10);
	DDX_Control(pDX, IDC_FIELD1, m_ctlField1);
	DDX_Control(pDX, IDC_SGUILD9, m_ctlSGuild9);
	DDX_Control(pDX, IDC_SGUILD8, m_ctlSGuild8);
	DDX_Control(pDX, IDC_SGUILD7, m_ctlSGuild7);
	DDX_Control(pDX, IDC_SGUILD6, m_ctlSGuild6);
	DDX_Control(pDX, IDC_SGUILD5, m_ctlSGuild5);
	DDX_Control(pDX, IDC_SGUILD4, m_ctlSGuild4);
	DDX_Control(pDX, IDC_SGUILD3, m_ctlSGuild3);
	DDX_Control(pDX, IDC_SGUILD2, m_ctlSGuild2);
	DDX_Control(pDX, IDC_SGUILD10, m_ctlSGuild10);
	DDX_Control(pDX, IDC_SGUILD1, m_ctlSGuild1);
	DDX_Control(pDX, IDC_FGUILD9, m_ctlFGuild9);
	DDX_Control(pDX, IDC_FGUILD8, m_ctlFGuild8);
	DDX_Control(pDX, IDC_FGUILD7, m_ctlFGuild7);
	DDX_Control(pDX, IDC_FGUILD6, m_ctlFGuild6);
	DDX_Control(pDX, IDC_FGUILD5, m_ctlFGuild5);
	DDX_Control(pDX, IDC_FGUILD4, m_ctlFGuild4);
	DDX_Control(pDX, IDC_FGUILD3, m_ctlFGuild3);
	DDX_Control(pDX, IDC_FGUILD2, m_ctlFGuild2);
	DDX_Control(pDX, IDC_FGUILD10, m_ctlFGuild10);
	DDX_Control(pDX, IDC_FGUILD1, m_ctlFGuild1);
	DDX_Control(pDX, IDC_DATE_LIST, m_ctlDateList);
	DDX_Control(pDX, IDC_TIME_LIST, m_ctlTimeList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTentativeGuildManager, CDialog)
	//{{AFX_MSG_MAP(CTentativeGuildManager)
	ON_BN_CLICKED(IDC_GET_GUILDLIST, OnGetGuildlist)
	ON_CBN_SELCHANGE(IDC_DATE_LIST, OnSelchangeDateList)
	ON_CBN_SELCHANGE(IDC_TIME_LIST, OnSelchangeTimeList)
	ON_BN_CLICKED(IDC_MODIFY1, OnModify1)
	ON_BN_CLICKED(IDC_MODIFY2, OnModify2)
	ON_BN_CLICKED(IDC_MODIFY3, OnModify3)
	ON_BN_CLICKED(IDC_MODIFY4, OnModify4)
	ON_BN_CLICKED(IDC_MODIFY5, OnModify5)
	ON_BN_CLICKED(IDC_MODIFY6, OnModify6)
	ON_BN_CLICKED(IDC_MODIFY7, OnModify7)
	ON_BN_CLICKED(IDC_MODIFY8, OnModify8)
	ON_BN_CLICKED(IDC_MODIFY9, OnModify9)
	ON_BN_CLICKED(IDC_MODIFY10, OnModify10)
//	ON_BN_CLICKED(IDC_GETGUILDINFO, OnGetguildinfo)
	ON_BN_CLICKED(IDC_INITGUILD_1, OnInitguild1)
	ON_BN_CLICKED(IDC_BEGIN_TEST_BATTLE, OnBeginTestBattle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTentativeGuildManager message handlers

BOOL CTentativeGuildManager::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int		i = 0;

	m_ctlDateList.AddString("Today");
	m_ctlDateList.AddString("Tomorrow");
	m_ctlDateList.SetCurSel(0);

	m_ctlTimeList.AddString("09:00 ~ 09:30");
	m_ctlTimeList.AddString("09:40 ~ 10:10");
	m_ctlTimeList.AddString("10:20 ~ 10:50");
	m_ctlTimeList.AddString("11:00 ~ 11:30");
	m_ctlTimeList.AddString("11:40 ~ 12:10");
	m_ctlTimeList.AddString("12:20 ~ 12:50");
	m_ctlTimeList.AddString("13:00 ~ 13:30");
	m_ctlTimeList.AddString("13:40 ~ 14:10");
	m_ctlTimeList.AddString("14:20 ~ 14:50");
	m_ctlTimeList.AddString("15:00 ~ 15:30");
	m_ctlTimeList.AddString("15:40 ~ 16:10");
	m_ctlTimeList.AddString("16:20 ~ 16:50");
	m_ctlTimeList.AddString("17:00 ~ 17:30");
	m_ctlTimeList.AddString("17:40 ~ 18:10");
	m_ctlTimeList.AddString("18:20 ~ 18:50");
	m_ctlTimeList.AddString("19:00 ~ 19:30");
	m_ctlTimeList.AddString("19:40 ~ 20:10");
	m_ctlTimeList.AddString("20:20 ~ 20:50");
	m_ctlTimeList.AddString("21:00 ~ 21:30");
	m_ctlTimeList.AddString("21:40 ~ 22:10");
	m_ctlTimeList.AddString("22:20 ~ 22:50");
	m_ctlTimeList.AddString("23:00 ~ 23:30");
	m_ctlTimeList.AddString("23:40 ~ 00:10");
	m_ctlTimeList.SetCurSel(0);

	memset(&todaySchedule,0,sizeof(CGuildBattleSchedule));
	memset(&tomorrowSchedule,0,sizeof(CGuildBattleSchedule));

	SetDlgItemInt(IDC_BATTLE_FIELD,990);
	SetDlgItemInt(IDC_BATTLE_TIME,5);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTentativeGuildManager::recvGuildData(char	* data)
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

 	WORLDtoSERVERS_GUILD_BATTLE_SCHEDULE	* rPacket;
	rPacket = (WORLDtoSERVERS_GUILD_BATTLE_SCHEDULE	*)data;
	memcpy(&todaySchedule,&rPacket->todaySchedule,sizeof(CGuildBattleSchedule));

	//	첫번째 세팅 해준다.
	m_ctlDateList.SetCurSel(0);
	m_ctlTimeList.SetCurSel(0);

	SetGuildDataSetting(0);

	MessageBox("Receive guild battle schedule","Complete",MB_OK);	
}

void CTentativeGuildManager::SetGuildDataSetting(int _iCount)
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	if	(todaySchedule.m_wYear == 0)
	{
		MessageBox(g_strToolMessage[78],"ERROR",MB_OK);	
		return;
	}

	CGuildBattleSchedule	* setSchedule = NULL; 

	WORD	wDay = m_ctlDateList.GetCurSel();
	if(wDay)	setSchedule = &tomorrowSchedule;
	else		setSchedule = &todaySchedule;

	//	길드 목록 combo에 넣고...
	//	이름으로 넣는게 아니구.. 인덱스로 길드 지정해준다.
	m_ctlFGuild1.SetWindowText(  "" );	m_ctlFGuild2.SetWindowText(  "" );	m_ctlFGuild3.SetWindowText(  "" );
	m_ctlFGuild4.SetWindowText(  "" );	m_ctlFGuild5.SetWindowText(  "" );	m_ctlFGuild6.SetWindowText(  "" );
	m_ctlFGuild7.SetWindowText(  "" );	m_ctlFGuild8.SetWindowText(  "" );	m_ctlFGuild9.SetWindowText(  "" );
	m_ctlFGuild10.SetWindowText(  "" );
	m_ctlSGuild1.SetWindowText(  "" );	m_ctlSGuild2.SetWindowText(  "" );	m_ctlSGuild3.SetWindowText(  "" );
	m_ctlSGuild4.SetWindowText(  "" );	m_ctlSGuild5.SetWindowText(  "" );	m_ctlSGuild6.SetWindowText(  "" );
	m_ctlSGuild7.SetWindowText(  "" );	m_ctlSGuild8.SetWindowText(  "" );	m_ctlSGuild9.SetWindowText(  "" );
	m_ctlSGuild10.SetWindowText(  "" );

	for	(int i=0;i<10;i++)
	{
		int	iGuild1	=	setSchedule->m_aBattleGuildSchedule[_iCount].m_aBattleGuildInfo[i].m_awBattleGuild[0];
		int	iGuild2	=	setSchedule->m_aBattleGuildSchedule[_iCount].m_aBattleGuildInfo[i].m_awBattleGuild[1];

		switch(i)
		{
			case	0	:
				if	(iGuild1	!=	0xffff	&&	guildInfo[iGuild1].wIndex	!=	0xffff)
					m_ctlFGuild1.SelectString(0,guildInfo[ iGuild1].strName);
				if	(iGuild2	!=	0xffff	&&	guildInfo[iGuild2].wIndex	!=	0xffff)
					m_ctlSGuild1.SelectString(0,guildInfo[ iGuild2].strName);
				break;
			case	1	:
				if	(iGuild1	!=	0xffff	&&	guildInfo[iGuild1].wIndex	!=	0xffff)
					m_ctlFGuild2.SelectString(0,guildInfo[ iGuild1].strName);
				if	(iGuild2	!=	0xffff	&&	guildInfo[iGuild2].wIndex	!=	0xffff)
					m_ctlSGuild2.SelectString(0,guildInfo[ iGuild2].strName);
				break;
			case	2	:
				if	(iGuild1	!=	0xffff	&&	guildInfo[iGuild1].wIndex	!=	0xffff)
					m_ctlFGuild3.SelectString(0,guildInfo[ iGuild1].strName);
				if	(iGuild2	!=	0xffff	&&	guildInfo[iGuild2].wIndex	!=	0xffff)
					m_ctlSGuild3.SelectString(0,guildInfo[ iGuild2].strName);
				break;
			case	3	:
				if	(iGuild1	!=	0xffff	&&	guildInfo[iGuild1].wIndex	!=	0xffff)
					m_ctlFGuild4.SelectString(0,guildInfo[ iGuild1].strName);
				if	(iGuild2	!=	0xffff	&&	guildInfo[iGuild2].wIndex	!=	0xffff)
					m_ctlSGuild4.SelectString(0,guildInfo[ iGuild2].strName);
				break;
			case	4	:
				if	(iGuild1	!=	0xffff	&&	guildInfo[iGuild1].wIndex	!=	0xffff)
					m_ctlFGuild5.SelectString(0,guildInfo[ iGuild1].strName);
				if	(iGuild2	!=	0xffff	&&	guildInfo[iGuild2].wIndex	!=	0xffff)
					m_ctlSGuild5.SelectString(0,guildInfo[ iGuild2].strName);
				break;
			case	5	:
				if	(iGuild1	!=	0xffff	&&	guildInfo[iGuild1].wIndex	!=	0xffff)
					m_ctlFGuild6.SelectString(0,guildInfo[ iGuild1].strName);
				if	(iGuild2	!=	0xffff	&&	guildInfo[iGuild2].wIndex	!=	0xffff)
					m_ctlSGuild6.SelectString(0,guildInfo[ iGuild2].strName);
				break;
			case	6	:
				if	(iGuild1	!=	0xffff	&&	guildInfo[iGuild1].wIndex	!=	0xffff)
					m_ctlFGuild7.SelectString(0,guildInfo[ iGuild1].strName);
				if	(iGuild2	!=	0xffff	&&	guildInfo[iGuild2].wIndex	!=	0xffff)
					m_ctlSGuild7.SelectString(0,guildInfo[ iGuild2].strName);
				break;
			case	7	:
				if	(iGuild1	!=	0xffff	&&	guildInfo[iGuild1].wIndex	!=	0xffff)
					m_ctlFGuild8.SelectString(0,guildInfo[ iGuild1].strName);
				if	(iGuild2	!=	0xffff	&&	guildInfo[iGuild2].wIndex	!=	0xffff)
					m_ctlSGuild8.SelectString(0,guildInfo[ iGuild2].strName);
				break;
			case	8	:
				if	(iGuild1	!=	0xffff	&&	guildInfo[iGuild1].wIndex	!=	0xffff)
					m_ctlFGuild9.SelectString(0,guildInfo[ iGuild1].strName);
				if	(iGuild2	!=	0xffff	&&	guildInfo[iGuild2].wIndex	!=	0xffff)
					m_ctlSGuild9.SelectString(0,guildInfo[ iGuild2].strName);
				break;
			case	9	:
				if	(iGuild1	!=	0xffff	&&	guildInfo[iGuild1].wIndex	!=	0xffff)
					m_ctlFGuild10.SelectString(0,guildInfo[ iGuild1].strName);
				if	(iGuild2	!=	0xffff	&&	guildInfo[iGuild2].wIndex	!=	0xffff)
					m_ctlSGuild10.SelectString(0,guildInfo[ iGuild2].strName);
				break;
		}
	}

	char tmp[10];

	m_ctlField1.SetWindowText(  itoa(setSchedule->m_aBattleGuildSchedule[_iCount].m_aBattleGuildInfo[0].m_wFieldSerial,tmp,10) );
	m_ctlField2.SetWindowText(  itoa(setSchedule->m_aBattleGuildSchedule[_iCount].m_aBattleGuildInfo[1].m_wFieldSerial,tmp,10) );
	m_ctlField3.SetWindowText(  itoa(setSchedule->m_aBattleGuildSchedule[_iCount].m_aBattleGuildInfo[2].m_wFieldSerial,tmp,10) );
	m_ctlField4.SetWindowText(  itoa(setSchedule->m_aBattleGuildSchedule[_iCount].m_aBattleGuildInfo[3].m_wFieldSerial,tmp,10) );
	m_ctlField5.SetWindowText(  itoa(setSchedule->m_aBattleGuildSchedule[_iCount].m_aBattleGuildInfo[4].m_wFieldSerial,tmp,10) );
	m_ctlField6.SetWindowText(  itoa(setSchedule->m_aBattleGuildSchedule[_iCount].m_aBattleGuildInfo[5].m_wFieldSerial,tmp,10) );
	m_ctlField7.SetWindowText(  itoa(setSchedule->m_aBattleGuildSchedule[_iCount].m_aBattleGuildInfo[6].m_wFieldSerial,tmp,10) );
	m_ctlField8.SetWindowText(  itoa(setSchedule->m_aBattleGuildSchedule[_iCount].m_aBattleGuildInfo[7].m_wFieldSerial,tmp,10) );
	m_ctlField9.SetWindowText(  itoa(setSchedule->m_aBattleGuildSchedule[_iCount].m_aBattleGuildInfo[8].m_wFieldSerial,tmp,10) );
	m_ctlField10.SetWindowText( itoa(setSchedule->m_aBattleGuildSchedule[_iCount].m_aBattleGuildInfo[9].m_wFieldSerial,tmp,10) );
}

void CTentativeGuildManager::OnGetGuildlist() 
{
	// TODO: Add your control notification handler code here
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif


	if (g_iSelectedServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	if	(IsConnectedServer(g_iSelectedServer)	==	FALSE)
		return;

	SERVERStoWORLD_ASK_GUILD_BATTLE_SCHEDULE	sPacket;
	sPacket.base.set(sizeof(SERVERStoWORLD_ASK_GUILD_BATTLE_SCHEDULE),dSERVERStoWORLD_ASK_GUILD_BATTLE_SCHEDULE);
	
	char tmp[1024];

	sprintf(tmp,g_strToolMessage[79],sList[g_iSelectedServer].strName,sList[g_iSelectedServer].strIP);

	if	(MessageBox("Ask guild battle schedule",tmp,MB_YESNO)==IDNO)
		return;

	if(g_iSelectedServer>=0 && sList[g_iSelectedServer].isConnect)
	{
		//	DB에서 길드 데이타를 얻어온다.
		if(!DB.Connect(	sList[g_iSelectedServer].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))
		{	
			MessageBox("Fail Connect!!");	
			return;	
		}

		DB.spGetGuildBasicData(guildInfo);

		DB.Close();

		m_ctlFGuild1.ResetContent();		m_ctlSGuild1.ResetContent();		m_ctlFGuild2.ResetContent();
		m_ctlSGuild2.ResetContent();		m_ctlFGuild3.ResetContent();		m_ctlSGuild3.ResetContent();
		m_ctlFGuild4.ResetContent();		m_ctlSGuild4.ResetContent();		m_ctlFGuild5.ResetContent();
		m_ctlSGuild5.ResetContent();		m_ctlFGuild6.ResetContent();		m_ctlSGuild6.ResetContent();
		m_ctlFGuild7.ResetContent();		m_ctlSGuild7.ResetContent();		m_ctlFGuild8.ResetContent();
		m_ctlSGuild8.ResetContent();		m_ctlFGuild9.ResetContent();		m_ctlSGuild9.ResetContent();
		m_ctlFGuild10.ResetContent();		m_ctlSGuild10.ResetContent();

		m_ctlFGuild1.AddString(" ");			m_ctlSGuild1.AddString(" ");
		m_ctlFGuild2.AddString(" ");			m_ctlSGuild2.AddString(" ");
		m_ctlFGuild3.AddString(" ");			m_ctlSGuild3.AddString(" ");
		m_ctlFGuild4.AddString(" ");			m_ctlSGuild4.AddString(" ");
		m_ctlFGuild5.AddString(" ");			m_ctlSGuild5.AddString(" ");
		m_ctlFGuild6.AddString(" ");			m_ctlSGuild6.AddString(" ");
		m_ctlFGuild7.AddString(" ");			m_ctlSGuild7.AddString(" ");
		m_ctlFGuild8.AddString(" ");			m_ctlSGuild8.AddString(" ");
		m_ctlFGuild9.AddString(" ");			m_ctlSGuild9.AddString(" ");
		m_ctlFGuild10.AddString(" ");			m_ctlSGuild10.AddString(" ");

		for(int i = 0;i<dGUILD_MAX_COUNT;i++)
		{
			if	(guildInfo[i].wIndex == 0xffff)
				continue;
			m_ctlFGuild1.AddString(guildInfo[i].strName);			m_ctlSGuild1.AddString(guildInfo[i].strName);
			m_ctlFGuild2.AddString(guildInfo[i].strName);			m_ctlSGuild2.AddString(guildInfo[i].strName);
			m_ctlFGuild3.AddString(guildInfo[i].strName);			m_ctlSGuild3.AddString(guildInfo[i].strName);
			m_ctlFGuild4.AddString(guildInfo[i].strName);			m_ctlSGuild4.AddString(guildInfo[i].strName);
			m_ctlFGuild5.AddString(guildInfo[i].strName);			m_ctlSGuild5.AddString(guildInfo[i].strName);
			m_ctlFGuild6.AddString(guildInfo[i].strName);			m_ctlSGuild6.AddString(guildInfo[i].strName);
			m_ctlFGuild7.AddString(guildInfo[i].strName);			m_ctlSGuild7.AddString(guildInfo[i].strName);
			m_ctlFGuild8.AddString(guildInfo[i].strName);			m_ctlSGuild8.AddString(guildInfo[i].strName);
			m_ctlFGuild9.AddString(guildInfo[i].strName);			m_ctlSGuild9.AddString(guildInfo[i].strName);
			m_ctlFGuild10.AddString(guildInfo[i].strName);			m_ctlSGuild10.AddString(guildInfo[i].strName);
		}

		sList[g_iSelectedServer].worldsock.SendPacket((char *)&sPacket,sPacket.base.wSize);	
		memset(&todaySchedule,0,sizeof(CGuildBattleSchedule));
		memset(&tomorrowSchedule,0,sizeof(CGuildBattleSchedule));
	}else
		MessageBox("You have to connect any server!!");
}

void CTentativeGuildManager::OnSelchangeDateList() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif	// TODO: Add your control notification handler code here
	//	첫번째 세팅 해준다.
	m_ctlTimeList.SetCurSel(0);

	SetGuildDataSetting(0);
}

void CTentativeGuildManager::OnSelchangeTimeList() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	// TODO: Add your control notification handler code here
	SetGuildDataSetting(m_ctlTimeList.GetCurSel());
}

void CTentativeGuildManager::changeGuildBattle(char* _strGuild1, char* _strGuild2, WORD	_wField, int _iField)
{
	if(strcmp(_strGuild1,"")==0 || strcmp(_strGuild2,"")==0 || _iField<990 || _iField>999)
	{
		MessageBox("Incorrect data");
		return;
	}

#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	if	(IsConnectedServer(g_iSelectedServer)	==	FALSE)
		return;

	if	(g_iSelectedServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	char tmp[512];
	sprintf(tmp,"Do you wanna regist this guild battle info? / SERVER [%s]",sList[g_iSelectedServer].strName);
	if	(MessageBox(tmp,"warning",MB_YESNO)==IDNO)
		return;

	WORLDtoSERVERS_CHAGEGUILDBATTLE		sPacket;
	sPacket.base.set(sizeof(WORLDtoSERVERS_CHAGEGUILDBATTLE),dWORLDtoSERVERS_CHAGEGUILDBATTLE);

	sPacket.isTime = m_ctlTimeList.GetCurSel();
	strcpy(sPacket.strGuildName1,_strGuild1);
	strcpy(sPacket.strGuildName2,_strGuild2);
	sPacket.wField = _wField;

	sList[g_iSelectedServer].worldsock.SendPacket((char *)&sPacket,sPacket.base.wSize);
}

void CTentativeGuildManager::OnModify1() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	// TODO: Add your control notification handler code here
	char	strGuild1[dGUILD_NAME_LENGTH],strGuild2[dGUILD_NAME_LENGTH],strField[4];
	m_ctlFGuild1.GetWindowText(strGuild1,dGUILD_NAME_LENGTH);
	m_ctlSGuild1.GetWindowText(strGuild2,dGUILD_NAME_LENGTH);
	m_ctlField1.GetWindowText(strField,4);

	changeGuildBattle(strGuild1,strGuild2,0, atoi(strField));
}

void CTentativeGuildManager::OnModify2() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	// TODO: Add your control notification handler code here
	char	strGuild1[dGUILD_NAME_LENGTH],strGuild2[dGUILD_NAME_LENGTH],strField[4];
	m_ctlFGuild2.GetWindowText(strGuild1,dGUILD_NAME_LENGTH);
	m_ctlSGuild2.GetWindowText(strGuild2,dGUILD_NAME_LENGTH);
	m_ctlField2.GetWindowText(strField,4);
	changeGuildBattle(strGuild1,strGuild2,1, atoi(strField));
}

void CTentativeGuildManager::OnModify3() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	// TODO: Add your control notification handler code here
	char	strGuild1[dGUILD_NAME_LENGTH],strGuild2[dGUILD_NAME_LENGTH],strField[4];
	m_ctlFGuild3.GetWindowText(strGuild1,dGUILD_NAME_LENGTH);
	m_ctlSGuild3.GetWindowText(strGuild2,dGUILD_NAME_LENGTH);
	m_ctlField3.GetWindowText(strField,4);
	changeGuildBattle(strGuild1,strGuild2,2, atoi(strField));
}

void CTentativeGuildManager::OnModify4() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	// TODO: Add your control notification handler code here
	char	strGuild1[dGUILD_NAME_LENGTH],strGuild2[dGUILD_NAME_LENGTH],strField[4];
	m_ctlFGuild4.GetWindowText(strGuild1,dGUILD_NAME_LENGTH);
	m_ctlSGuild4.GetWindowText(strGuild2,dGUILD_NAME_LENGTH);
	m_ctlField4.GetWindowText(strField,4);
	changeGuildBattle(strGuild1,strGuild2,3, atoi(strField));
}

void CTentativeGuildManager::OnModify5() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	// TODO: Add your control notification handler code here
	char	strGuild1[dGUILD_NAME_LENGTH],strGuild2[dGUILD_NAME_LENGTH],strField[4];
	m_ctlFGuild5.GetWindowText(strGuild1,dGUILD_NAME_LENGTH);
	m_ctlSGuild5.GetWindowText(strGuild2,dGUILD_NAME_LENGTH);
	m_ctlField5.GetWindowText(strField,4);
	changeGuildBattle(strGuild1,strGuild2,4, atoi(strField));
}

void CTentativeGuildManager::OnModify6() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	// TODO: Add your control notification handler code here
	char	strGuild1[dGUILD_NAME_LENGTH],strGuild2[dGUILD_NAME_LENGTH],strField[4];
	m_ctlFGuild6.GetWindowText(strGuild1,dGUILD_NAME_LENGTH);
	m_ctlSGuild6.GetWindowText(strGuild2,dGUILD_NAME_LENGTH);
	m_ctlField6.GetWindowText(strField,4);
	changeGuildBattle(strGuild1,strGuild2,5, atoi(strField));
}

void CTentativeGuildManager::OnModify7() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	// TODO: Add your control notification handler code here
	char	strGuild1[dGUILD_NAME_LENGTH],strGuild2[dGUILD_NAME_LENGTH],strField[4];
	m_ctlFGuild7.GetWindowText(strGuild1,dGUILD_NAME_LENGTH);
	m_ctlSGuild7.GetWindowText(strGuild2,dGUILD_NAME_LENGTH);
	m_ctlField7.GetWindowText(strField,4);
	changeGuildBattle(strGuild1,strGuild2,6, atoi(strField));
}

void CTentativeGuildManager::OnModify8() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	// TODO: Add your control notification handler code here
	char	strGuild1[dGUILD_NAME_LENGTH],strGuild2[dGUILD_NAME_LENGTH],strField[4];
	m_ctlFGuild8.GetWindowText(strGuild1,dGUILD_NAME_LENGTH);
	m_ctlSGuild8.GetWindowText(strGuild2,dGUILD_NAME_LENGTH);
	m_ctlField8.GetWindowText(strField,4);
	changeGuildBattle(strGuild1,strGuild2,7, atoi(strField));
}

void CTentativeGuildManager::OnModify9() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	// TODO: Add your control notification handler code here
	char	strGuild1[dGUILD_NAME_LENGTH],strGuild2[dGUILD_NAME_LENGTH],strField[4];
	m_ctlFGuild9.GetWindowText(strGuild1,dGUILD_NAME_LENGTH);
	m_ctlSGuild9.GetWindowText(strGuild2,dGUILD_NAME_LENGTH);
	m_ctlField9.GetWindowText(strField,4);
	changeGuildBattle(strGuild1,strGuild2,8, atoi(strField));
}

void CTentativeGuildManager::OnModify10() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	// TODO: Add your control notification handler code here
	char	strGuild1[dGUILD_NAME_LENGTH],strGuild2[dGUILD_NAME_LENGTH],strField[4];
	m_ctlFGuild10.GetWindowText(strGuild1,dGUILD_NAME_LENGTH);
	m_ctlSGuild10.GetWindowText(strGuild2,dGUILD_NAME_LENGTH);
	m_ctlField10.GetWindowText(strField,4);
	changeGuildBattle(strGuild1,strGuild2,9, atoi(strField));
}
/*
void CTentativeGuildManager::OnGetguildinfo() 
{
	// TODO: Add your control notification handler code here
	if (!CheckOperatorLevel(eAL_TESTER))
		return;

	if (!IsConnectedAllServer(TRUE,TRUE))
		return;

	if (g_iSelectedServer	==	-1)
	{
		MessageBox("please select world","");
		return;
	}

	if(!DB.Connect(	sList[g_iSelectedServer].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))
	{	
		MessageBox("Fail Connect!!");	
		return;	
	}

	DB.spGetGuildBasicData(guildInfo);
	DB.Close();
}
*/
void CTentativeGuildManager::OnInitguild1() 
{
	// TODO: Add your control notification handler code here
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	if	(IsConnectedServer(g_iSelectedServer)	==	FALSE)
		return;

	if	(g_iSelectedServer	==	-1)
	{
		MessageBox("Select a world, please!!","");
		return;
	}

	if(!DB.Connect(	sList[g_iSelectedServer].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))
	{	
		MessageBox("Fail Connect!!");	
		return;	
	}

	//	DB백업하고 할것!!
	{
		_RecordsetPtr  pRs = NULL;
		char	       str_Query[1024];
		int i_RecCount;
		char *pBuf = NULL;
		HRESULT hr = CoInitialize(0);

		sprintf(str_Query,"SELECT data FROM Avatar_Current WHERE guildRank = 5");
		printf(str_Query);
		pRs.CreateInstance(__uuidof(Recordset)); 

		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)DB.GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		i_RecCount = pRs->RecordCount;

		char	* uncompressdata = new char[sizeof(cPLAYER_DATA)];
		cPLAYER_DATA		pData;

		if(!FAILED(hr))
		{ 
			for(int i = 0 ;i < i_RecCount ;i++)
			{
				_variant_t     v_bufferdata;
				VariantInit(&v_bufferdata);
				v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
				memset(uncompressdata,0,sizeof(cPLAYER_DATA));
				memset(&pData,0,sizeof(cPLAYER_DATA));

				int	unsize	=	DB.VariantToBlob(v_bufferdata,uncompressdata);

				if(unsize<sizeof(cPLAYER_DATA))
				{
					WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&pData,unsize,sizeof(cPLAYER_DATA));
					if(ww == 0)
					{
						if	(unsize >= 2048	&&	unsize	<sizeof(cPLAYER_DATA))
						{
							memset(&pData,0,sizeof(cPLAYER_DATA));
							memcpy(&pData,uncompressdata,unsize);
						}

						_log(" uncompress error 2");
					}
				}
				else
				{
					memcpy(&pData,uncompressdata,sizeof(cPLAYER_DATA));
				}

				pData.m_wGuildRank = 2;
				VariantClear(&v_bufferdata);
				DB.spSaveAva(&pData);
				pRs->MoveNext();
			}
		}

		delete [] uncompressdata;

		pRs->Close();
	}

	DB.Close();
}

void
CTentativeGuildManager::OnBeginTestBattle() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	if	(g_iSelectedServer	==	-1)
	{
		MessageBox("please select server"," ");
		return;
	}

	if	(sList[g_iSelectedServer].isTestServer	==	0)
	{
		MessageBox("It's not test server!!"," ");
		return;
	}
	if	(sList[g_iSelectedServer].isConnect		==	FALSE)
	{
		MessageBox("It's not connected server!!");	
		return;
	}


	SERVERStoWORLD_BEGIN_TEST_GUILD_BATTLE	packet;

	packet.base.set(sizeof(SERVERStoWORLD_BEGIN_TEST_GUILD_BATTLE),dSERVERStoWORLD_BEGIN_TEST_GUILD_BATTLE);

	m_ebGuild1Name.GetWindowText(packet.strGuild1,dGUILD_NAME_LENGTH-2);
	m_ebGuild2Name.GetWindowText(packet.strGuild2,dGUILD_NAME_LENGTH-2);
	
	if (strlen(packet.strGuild1) < 2 || strlen(packet.strGuild2) < 2 )
	{
		MessageBox("require guild Name","");
		return;
	}

	packet.wField	=	GetDlgItemInt(IDC_BATTLE_FIELD);
	packet.wTime	=	GetDlgItemInt(IDC_BATTLE_TIME);

	sList[g_iSelectedServer].worldsock.SendPacket((char *)&packet,packet.base.wSize);	
}
