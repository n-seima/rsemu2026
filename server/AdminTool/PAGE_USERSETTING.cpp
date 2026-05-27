// PAGE_USERSETTING.cpp : implementation file
//

#include "stdafx.h"
#include "AdminTool.h"
#include "PAGE_USERSETTING.h"
#include "cSRVUTIL.h"
#include "MSGDLG.h"
#include "NEWNAME.h"
#include "cPACKET_WORLDSERVER.h"
#include "cPACKET_DBSERVER.h"
#include "cPACKET_TOOLLOGSERVER.h"
#include "AdminLogCode.h"
#include "CMessage.h"
#include "AdminCommand.h"
#include "ThreadFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	dMAX_LINE_LENGTH	22	//	한줄에서 읽을 최대 글자수


BOOL	readLine(FILE *f,char *line)
{
	int		tmp;
	int		count=0;

	memset(line,0,dMAX_LINE_LENGTH);

	tmp = fgetc(f);
	while(tmp!=EOF && tmp!=10)
	{
		line[count++] = (char)tmp;
		tmp = fgetc(f);
	}
	line[count]=0;
	if(tmp==EOF)	return FALSE;
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPAGE_USERSETTING property page

IMPLEMENT_DYNCREATE(CPAGE_USERSETTING, CPropertyPage)

CPAGE_USERSETTING::CPAGE_USERSETTING() : CPropertyPage(CPAGE_USERSETTING::IDD)
{
	//{{AFX_DATA_INIT(CPAGE_USERSETTING)
	m_strWhy = _T("");
	m_strBlockDateLengthCheck = _T("");
	//}}AFX_DATA_INIT
}

CPAGE_USERSETTING::~CPAGE_USERSETTING()
{
}

void CPAGE_USERSETTING::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPAGE_USERSETTING)
	DDX_Control(pDX, IDC_CHKACCOUNT, m_ctlIsCheckUsingAccount);
	DDX_Control(pDX, IDC_SET_TESTER, m_ctlSetTester);
	DDX_Control(pDX, IDC_SEARCH_ID_INPUT, m_ctlSearchId);
	DDX_Control(pDX, IDC_AVARECOVER_NEWNAME, m_ctlAvaRecNewName);
	DDX_Control(pDX, IDC_AVARECOVER_NAME, m_ctlAvaRecName);
	DDX_Control(pDX, IDC_AVARECOVER_ID, m_ctlAvaRecId);
	DDX_Control(pDX, IDC_CNAME_NAME2, m_ctlChgNewName);
	DDX_Control(pDX, IDC_CNAME_NAME, m_ctlChgOldName);
	DDX_Control(pDX, IDC_CNAME_ID, m_ctlChgNameID);
	DDX_Control(pDX, IDC_BAD_CHARNAME, m_ctlBadCharName);
	DDX_Control(pDX, IDC_BADID, m_ctlBadID);
	DDX_Control(pDX, IDC_RECOVER_ID2, m_ctlRecIDNew);
	DDX_Control(pDX, IDC_BADWHY, m_ctlBadWhy);
	DDX_Control(pDX, IDC_OPER_LEVEL, m_ctlOperLevel);
	DDX_Control(pDX, IDC_RECOVER_ID, m_ctlRecID);
	DDX_Control(pDX, IDC_BADDATE, m_ctlBadDate);
	DDX_Control(pDX, IDC_BADNAME, m_ctlBadName);
	DDX_Control(pDX, IDC_ADMIN_NAME, m_ctlAdminName);
	DDX_Text(pDX, IDC_BADWHY, m_strWhy);
	DDV_MaxChars(pDX, m_strWhy, 60);
	DDX_Text(pDX, IDC_BADDATE, m_strBlockDateLengthCheck);
	DDV_MaxChars(pDX, m_strBlockDateLengthCheck, 10);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPAGE_USERSETTING, CPropertyPage)
	//{{AFX_MSG_MAP(CPAGE_USERSETTING)
	ON_BN_CLICKED(IDC_ADMIN, OnSetAdmin)
	ON_BN_CLICKED(IDC_ADMIN_COUNT, OnAdminCount)
	ON_BN_CLICKED(IDC_BADUSER, OnBaduser)
	ON_BN_CLICKED(IDC_RECOVER, OnRecover)
	ON_BN_CLICKED(IDC_DEL_BADID, OnDelBadid)
	ON_BN_CLICKED(IDC_DEL_BADNAME, OnDelBadname)
	ON_BN_CLICKED(IDC_CNAME, OnChangeName)
	ON_BN_CLICKED(IDC_CNAME2, OnChangenameSyc)
	ON_BN_CLICKED(IDC_RECOVER2, OnAvaRecover)
	ON_BN_CLICKED(IDC_SEARCH_ID, OnSearchId)
	ON_BN_CLICKED(IDC_LIST_DELETE, OnListDelete)
	ON_BN_CLICKED(IDC_SAVE_UPDATE, OnSaveUpdate)
	ON_BN_CLICKED(IDC_SAVE_UPDATE_ALL, OnSaveUpdateAll)
	ON_BN_CLICKED(IDC_CHECK_BADUSER, OnCheckBaduser)
	ON_BN_CLICKED(IDC_GUILD_CHECK, OnGuildCheck)
	ON_EN_CHANGE(IDC_AVARECOVER_NAME, OnChangeAvarecoverName)
	ON_EN_CHANGE(IDC_CNAME_NAME, OnChangeCnameName)
	ON_BN_CLICKED(IDC_UPDATELIST, OnUpdatelist)
	ON_BN_CLICKED(IDC_ID_CHECK, OnIdCheck)
	ON_BN_CLICKED(IDC_EMER, OnEmer)
	ON_BN_CLICKED(IDC_CHECK_OPERLEVEL, OnCheckOperlevel)
	ON_BN_CLICKED(IDC_SET_TESTER, OnSetTester)
	ON_BN_CLICKED(IDC_RESET_OPER, OnResetOper)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPAGE_USERSETTING message handlers

void CPAGE_USERSETTING::OnSetAdmin() 
{
	// TODO: Add your control notification handler code here
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))	return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	static int reHour = cSRVUTIL::GetHour()-1;	if(reHour<0)	reHour = 0;

	if(!m_ctlIsCheckUsingAccount.GetCheck() && (g_iSelectedServer==-1 || strcmp(sList[g_iSelectedServer].strName,"")==0)){	MessageBox(g_strToolMessage[37]);	return;	}

	char	name[dNAME_LENGTH];
	m_ctlAdminName.GetWindowText(name,dNAME_LENGTH);
	if(strcmp(name,"")==0)	{	MessageBox(g_strToolMessage[28]);	return;	}
	char strLevel[10];
	m_ctlOperLevel.GetWindowText(strLevel,10);

//	if(strstr(sList[g_iSelectedServer].strName,"소룬드")==NULL)
	{
		if(m_ctlIsCheckUsingAccount.GetCheck())	//	Account DB로 연결해서 계정의 정보를 얻어와라.
		{
			int iOper = atoi(strLevel);
			if(iOper>1)	iOper = 1;
			int ret = g_DBAccount.spSetOperatorInAccount(name,iOper);
			if(ret)		MessageBox(g_strToolMessage[159]);
			else		MessageBox(g_strToolMessage[160]);
		}else
		{
			if(!DB.Connect(	sList[g_iSelectedServer].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))	{	MessageBox(g_strToolMessage[38]);	return;	}

			PACKET_SETOPERATER	sPacket;
			sPacket.base.set(sizeof(PACKET_SETOPERATER),ePACKET_SETOPERATER);
			strcpy(sPacket.strId,strAdminId);
			strcpy(sPacket.strServerName,sList[g_iSelectedServer].strName);
			strcpy(sPacket.strUserName,name);
			sPacket.wOper = atoi(strLevel);
			_log("Change Operator Level : admin id[%s], server [%s], avatar name [%s], level[%d]",sPacket.strId,sPacket.strServerName,sPacket.strUserName,sPacket.wOper);
			if(!logSock.SendPacket((char *)&sPacket,sPacket.base.wSize))
			{	DB.Close();
				AfxMessageBox("Could not Send to Log Server!!");
				return;
			}

			DB.spSetOper(name,atoi(strLevel));
			DB.Close();
		}
	}
/*	WORD result = DB.spIsOper(name);
	switch(result)
	{
	case 0:
		DB.spSetOper(name,atoi(strLevel));
		MessageBox("운영자로 설정이 되었습니다.");
		break;
	case 99:
		MessageBox("잘못된 아이디입니다.");
		break;
	case 0xffff:
		MessageBox("현재 적용 불가능합니다. 지현이를 갈궈보세요 -_-;");
		break;
	}
*/
}

void CPAGE_USERSETTING::OnAdminCount()
{
	// TODO: Add your control notification handler code here
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))	return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	static int reHour = cSRVUTIL::GetHour()-1;	if(reHour<0)	reHour = 0;
	if(!m_ctlIsCheckUsingAccount.GetCheck() && (g_iSelectedServer==-1 || strcmp(sList[g_iSelectedServer].strName,"")==0)){	MessageBox(g_strToolMessage[37]);	return;	}

	if(m_ctlIsCheckUsingAccount.GetCheck())	//	Account DB로 연결해서 계정의 정보를 얻어와라.
	{
		WORD wCount = g_DBAccount.spGetOperatorCount();
		char tmp[256];
		sprintf(tmp,g_strToolMessage[161],wCount);
		MessageBox(tmp);
	}else
	{
		if(!DB.Connect(	sList[g_iSelectedServer].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))	{	MessageBox(g_strToolMessage[38]);	return;	}
		char txt[100];
		sprintf(txt,g_strToolMessage[29],DB.spGetOperCount());
		MessageBox(txt);
		DB.Close();
	}
}

void CPAGE_USERSETTING::OnBaduser() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	// TODO: Add your control notification handler code here
	char	name[dID_LENGTH],date[32],why[64];
	strcpy(why,"");
	m_ctlBadName.GetWindowText(name,dID_LENGTH);
	if(strcmp(name,"")==0)	{	MessageBox(g_strToolMessage[30]);	return;	}
	m_ctlBadDate.GetWindowText(date,32);
	unsigned int	dd = atoi(date);
	if(dd>0)
	{
		m_ctlBadWhy.GetWindowText(why,64);
		if(strcmp(why,"")==0)
		{
			MessageBox(g_strToolMessage[31]);
			return;
		}
		if(strlen(why)>60)
		{
			MessageBox(g_strToolMessage[32]);
			return;
		}
	}
	if(dd!=0 && dd< (cSRVUTIL::GetTime()*100)){	MessageBox(g_strToolMessage[33]);	return;	}

	char txt[100];
	if(dd==0)
		sprintf(txt,g_strToolMessage[34],g_DBAccount.spSetBadUser(name , dd , why));
	else
	{
		sprintf(txt,g_strToolMessage[35],g_DBAccount.spSetBadUser(name , dd , why));
		_log(g_strToolMessage[36],strUserName,name,date,why);
	}

	PACKET_SETBADUSER	sPacket;
	sPacket.base.set(sizeof(PACKET_SETBADUSER),ePACKET_SETBADUSER);
	strcpy(sPacket.strId,strAdminId);
	strcpy(sPacket.strUserId,name);
	sPacket.dwBlockDate = dd;
	strcpy(sPacket.strWhy,why);
	logSock.SendPacket((char *)&sPacket,sPacket.base.wSize);

	MessageBox(txt);
}

void CPAGE_USERSETTING::OnRecover() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif
	// TODO: Add your control notification handler code here
	char	id[dID_LENGTH],newid[dID_LENGTH];
	m_ctlRecID.GetWindowText(id,dID_LENGTH);
	m_ctlRecIDNew.GetWindowText(newid,dID_LENGTH);
	if(strcmp(id,"")==0)	 {	MessageBox(g_strToolMessage[35]);	return;	}
//	if(strcmp(newid,"")==0){	MessageBox("대체할 아이디를 입력하세요.");	return;	}
	PACKET_RECOVERID	sPacket;
	sPacket.base.set(sizeof(PACKET_RECOVERID),ePACKET_RECOVERID);
	strcpy(sPacket.strId,strAdminId);
	strcpy(sPacket.strOldUserId,id);
	strcpy(sPacket.strOldUserId,newid);
	if(!logSock.SendPacket((char *)&sPacket,sPacket.base.wSize))
	{
		AfxMessageBox("Could not Send to Log Server!!");
		return;
	}

	int result = g_DBAccount.spRecoverID(id,newid);
	switch(result)
	{
	case 0:	
		_log("%s[%s]",g_strToolMessage[39],strUserName);
		_log(g_strToolMessage[40],id);
		MessageBox(g_strToolMessage[39]);	
		break;
	case 1:	
		_log("%s[%s]",g_strToolMessage[39],strUserName);
		_log(g_strToolMessage[41],id,newid);
		MessageBox(g_strToolMessage[39]);	
		break;
	case 2:	MessageBox(g_strToolMessage[42]);			break;
	case 3:	MessageBox(g_strToolMessage[43]);	break;
	}
/*f(result>1)	return;

	result = 0;
	//	각 서버를 거치면서 삭제된 아바타를 복구한다.
	CMSGDLG		msgDLG;
	char msg[1024];
	char newname[dNAME_LENGTH];
	int iMaxProgress = iServerCount*5;	//	마지막 100은 DB에서 얻어오는 각종 통계자료 
	int iCurProgress = 0;
	msgDLG.Create(IDD_MSG_DIALOG);	
	msgDLG.m_ctlProgress.SetRange(0,iMaxProgress);
	msgDLG.ShowWindow(SW_SHOWNORMAL);
	CNEWNAME	nameInput;
	for(int i=0;i<iServerCount;i++)
	{
		//	테스트 서버의 경우는 건너뛴다.
		if(strcmp(sList[i].strDBIP,"211.189.122.58")==0)	continue;	
		if(DB.Connect(	sList[i].strDBIP , strAvatarDBCId,strAvatarDBCPw,"REDSTONE_AVATAR_SOURCE",1))
		{	
			for(int j=0;j<4;j++)
			{
				sprintf(msg,"[%s]의 [%s]서버 %d번째 캐릭터 복구 작업을 시작합니다.",id,sList[i].strName,j);
				msgDLG.m_ctlMsg.SetWindowText(msg);	msgDLG.m_ctlProgress.SetPos(iCurProgress+=1);	Sleep(500);

				memset(newname,0,dNAME_LENGTH);
				int ava_result = 0;
RE_RECOVER:
				ava_result = DB.spAvaRecover(id,newid,j,newname);
				switch(ava_result)
				{
				case -1:	MessageBox("DB에 관련된 에러입니다.");			break;
				case 0:		MessageBox("해당 인덱스의 캐릭터가 없습니다.");	break;
				case 1:
					{
						sprintf(msg,"[%s]의 [%s]서버 [%s]캐릭터가 원본 그대로 복구되었습니다.",id,sList[i].strName,newname);
						msgDLG.m_ctlMsg.SetWindowText(msg);Sleep(500);
						break;
					}
				case 2:
					{
						sprintf(msg,"[%s]의 [%s]서버 [%s]캐릭터가 이름 교체후 복구되었습니다.",id,sList[i].strName,newname);
						msgDLG.m_ctlMsg.SetWindowText(msg);Sleep(500);
						break;
					}
				case 3:
					{
						sprintf(msg,"[%s]의 [%s]서버 [%s]캐릭터의 이름이 사용중입니다. 새로운 캐릭터 이름을 입력해주세요.",id,sList[i].strName,newname);
						msgDLG.m_ctlMsg.SetWindowText(msg);Sleep(500);
						if(nameInput.DoModal()==IDOK)
						{
							strcpy(newname,nameInput.m_strNewName);
							goto RE_RECOVER;
						}else{
							//	이 캐릭터를 삭제해 버린다.
							DB.spAvaRecoverDel(id,j,newname);
						}
						break;
					}
				}
			}
			DB.Close();
		}else{
			sprintf(msg,"");
			MessageBox(msg,"아바타 DB 접속 실패");	continue;	
		}
		sprintf(msg,"[%s]의 [%s]서버 캐릭터 복구 작업이 완료되었습니다.",id,sList[i].strName);
		msgDLG.m_ctlMsg.SetWindowText(msg);Sleep(500);
		iCurProgress = (i+1)*5;	msgDLG.m_ctlProgress.SetPos(iCurProgress);
	}
	msgDLG.CloseWindow();
*/
}


BOOL CPAGE_USERSETTING::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

#ifndef _FOR_KOREA
	SetDlgItemText(IDC_STATIC1,g_strDlgText[123]);
	SetDlgItemText(IDC_STATIC2,g_strDlgText[124]);
	SetDlgItemText(IDC_CHECK_BADUSER,g_strDlgText[125]);
	SetDlgItemText(IDC_BADUSER,g_strDlgText[126]);
	SetDlgItemText(IDC_STATIC3,g_strDlgText[127]);
	SetDlgItemText(IDC_DEL_BADID,g_strDlgText[128]);
	SetDlgItemText(IDC_DEL_BADNAME,g_strDlgText[129]);
	SetDlgItemText(IDC_STATIC4,g_strDlgText[130]);
	SetDlgItemText(IDC_RECOVER,g_strDlgText[131]);
	SetDlgItemText(IDC_ID_CHECK,g_strDlgText[132]);
	SetDlgItemText(IDC_STATIC5,g_strDlgText[133]);
	SetDlgItemText(IDC_RECOVER2,g_strDlgText[134]);
	SetDlgItemText(IDC_UPDATELIST,g_strDlgText[135]);
	SetDlgItemText(IDC_STATIC6,g_strDlgText[136]);
	SetDlgItemText(IDC_CNAME2,g_strDlgText[137]);
	SetDlgItemText(IDC_CNAME,g_strDlgText[138]);
	SetDlgItemText(IDC_STATIC7,g_strDlgText[139]);
	SetDlgItemText(IDC_ADMIN,g_strDlgText[140]);
	SetDlgItemText(IDC_ADMIN_COUNT,g_strDlgText[141]);
	SetDlgItemText(IDC_STATIC8,g_strDlgText[142]);
	SetDlgItemText(IDC_SEARCH_ID,g_strDlgText[143]);
	SetDlgItemText(IDC_CHKACCOUNT,g_strDlgText[188]);
	SetDlgItemText(IDC_RESET_OPER,"Reset Seleted server's Operator list");
	SetDlgItemText(IDC_CHECK_OPERLEVEL,"Chk OP LV");

//	::ShowWindow( ::GetDlgItem(this->m_hWnd,IDC_STATIC6),SW_HIDE);
//	::ShowWindow( ::GetDlgItem(this->m_hWnd,IDC_STATIC_ID),SW_HIDE);
//	::ShowWindow( ::GetDlgItem(this->m_hWnd,IDC_CNAME_ID),SW_HIDE);
//	::ShowWindow( ::GetDlgItem(this->m_hWnd,IDC_STATIC_OLDNAME),SW_HIDE);
//	::ShowWindow( ::GetDlgItem(this->m_hWnd,IDC_STATIC_NEW_NAME),SW_HIDE);
//	::ShowWindow( ::GetDlgItem(this->m_hWnd,IDC_CNAME_NAME),SW_HIDE);
//	::ShowWindow( ::GetDlgItem(this->m_hWnd,IDC_CNAME_NAME2),SW_HIDE);
//	::ShowWindow( ::GetDlgItem(this->m_hWnd,IDC_CNAME2),SW_HIDE);
//	::ShowWindow( ::GetDlgItem(this->m_hWnd,IDC_CNAME),SW_HIDE);
	::ShowWindow( ::GetDlgItem(this->m_hWnd,IDC_STATIC_LINE1),SW_HIDE);
	::ShowWindow( ::GetDlgItem(this->m_hWnd,IDC_SET_TESTER),SW_HIDE);

#endif
	// TODO: Add extra initialization here
	m_ctlOperLevel.AddString("6");
	m_ctlOperLevel.AddString("5");
	m_ctlOperLevel.AddString("4");
	m_ctlOperLevel.AddString("3");
	m_ctlOperLevel.AddString("2");
	m_ctlOperLevel.AddString("1");
	m_ctlOperLevel.AddString("0");
	m_ctlOperLevel.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPAGE_USERSETTING::OnDelBadid() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
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
	char	strId[dID_LENGTH];
	m_ctlBadID.GetWindowText(strId,dID_LENGTH);
	if(strcmp(strId,"")==0)	{	MessageBox(g_strToolMessage[30]);	return;	}

	PACKET_DELID	sPacket;
	sPacket.base.set(sizeof(PACKET_DELID),ePACKET_DELID);
	strcpy(sPacket.strId,strAdminId);
	strcpy(sPacket.strUserId,strId);
	if(!logSock.SendPacket((char *)&sPacket,sPacket.base.wSize))
	{
		AfxMessageBox("Could not Send to Log Server!!");
		return;
	}

	int i = g_DBAccount.spDeleteID(strId);
	if(i)
	{
		MessageBox(g_strToolMessage[44]);
		_log("[%s] %s : [%s]",strUserName,g_strToolMessage[45],strId);
	}
	else
	{
		char result[100];
		sprintf(result,"ID delete error... %d",i);
		MessageBox(result);
	}

}

void CPAGE_USERSETTING::OnDelBadname() 
{
	// TODO: Add your control notification handler code here
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	char	strId[dID_LENGTH];
	m_ctlBadID.GetWindowText(strId,dID_LENGTH);
	if(strcmp(strId,"")==0)	{	MessageBox(g_strToolMessage[30]);	return;	}

	char	strName[22];
	this->m_ctlBadCharName.GetWindowText(strName,dNAME_LENGTH);
	if(strcmp(strName,"")==0){	MessageBox(g_strToolMessage[28]);	return;	}
	if(g_iSelectedServer==-1 || strcmp(sList[g_iSelectedServer].strName,"")==0){	MessageBox(g_strToolMessage[37]);	return;	}

	char txt[1024];
	sprintf(txt,g_strToolMessage[46],sList[g_iSelectedServer].strName,sList[g_iSelectedServer].strDBIP,strId,strName);
	if(MessageBox(txt,"WARNING !!",MB_YESNO)==IDNO)	return;

	if(!DB.Connect(	sList[g_iSelectedServer].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))	{	MessageBox(g_strToolMessage[38]);	return;	}
	
	PACKET_DELAVA	sPacket;
	sPacket.base.set(sizeof(PACKET_DELAVA),ePACKET_DELAVA);
	strcpy(sPacket.strId,strAdminId);
	strcpy(sPacket.strUserId,strId);
	strcpy(sPacket.strUserName,strName);
	if(!logSock.SendPacket((char *)&sPacket,sPacket.base.wSize))
	{	DB.Close();
		AfxMessageBox("Could not Send to Log Server!!");
		return;
	}

	int result = DB.spDeleteAvatar(strId,strName);
	DB.Close();
	if(result!=-1)
	{
		MessageBox("FAIL !!");
	}else{
		sprintf(txt,g_strToolMessage[47],sList[g_iSelectedServer].strName,sList[g_iSelectedServer].strDBIP,strId,result,strName);
		_log(txt);
		MessageBox(txt);
	}
}

void CPAGE_USERSETTING::OnChangeName() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))	return;
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
	char strId[dID_LENGTH];		memset(strId,0,dID_LENGTH);
	char oldName[dNAME_LENGTH];	memset(oldName,0,dNAME_LENGTH);
	char newName[dNAME_LENGTH];	memset(newName,0,dNAME_LENGTH);

	m_ctlChgNameID.GetWindowText(strId,dID_LENGTH);
	if(strcmp(strId,"")==0 || strlen(strId)<2){	MessageBox(g_strToolMessage[30]);	return;	}
	m_ctlChgOldName.GetWindowText(oldName,dNAME_LENGTH);
	if(strcmp(oldName,"")==0 || strlen(oldName)<2){	MessageBox(g_strToolMessage[28]);	return;	}
	m_ctlChgNewName.GetWindowText(newName,dNAME_LENGTH);
	if(strcmp(newName,"")==0 || strlen(newName)<2){	MessageBox(g_strToolMessage[28]);	return;	}

	char txt[1024];
	sprintf(txt,g_strToolMessage[48],sList[g_iSelectedServer].strName,sList[g_iSelectedServer].strDBIP,strId,oldName,newName);
	if(MessageBox(txt,g_strToolMessage[49],MB_YESNO)==IDNO)	return;

	if(!DB.Connect(	sList[g_iSelectedServer].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))	{	MessageBox(g_strToolMessage[38]);	return;	}

	PACKET_CHANGENAME	sPacket;
	sPacket.base.set(sizeof(PACKET_CHANGENAME),ePACKET_CHANGENAME);
	strcpy(sPacket.strId,strAdminId);
	strcpy(sPacket.strServerName,sList[g_iSelectedServer].strName);
	strcpy(sPacket.strUserId,strId);
	strcpy(sPacket.strOldUserName,oldName);
	strcpy(sPacket.strNewUserName,newName);

	if(!logSock.SendPacket((char *)&sPacket,sPacket.base.wSize))
	{	DB.Close();
		AfxMessageBox("Could not Send to Log Server!!");
		return;
	}

	int result = DB.spAvaRename(strId,oldName,newName);
	switch(result)
	{
	case -1:
		MessageBox(g_strToolMessage[50]);
		break;
	case 0:
		MessageBox(g_strToolMessage[51]);
		break;
	case 1:
		if(strcmp(oldName,newName)!=0)
		{
			if(DB.spAvaRenameSyc(strId,newName))
			{
				_log(g_strToolMessage[52],strUserName,sList[g_iSelectedServer].strName,strId,oldName,newName);
				MessageBox(g_strToolMessage[53]);
			}else{
				_log(g_strToolMessage[54],strUserName,strId,oldName,newName);
				MessageBox(g_strToolMessage[53]);
			}
		}
		break;
	case 2:
		MessageBox(g_strToolMessage[56]);
		break;
	}
	DB.Close();

	//	USER DBC에 등록해서 실행중인 서버에 변경을 요청한다.
	if(result==0 || result==1)
	{
		if(!g_adCommand.connectCommandDB())
		{
			MessageBox(" Cannot Connect to Command DB Server!!\nContact your Operating Team","Error!!");
			return ;
		}
		UTime	regTime,finTime;

		cSRVUTIL::GetUTime((cpUTime *)&regTime,60);
		cSRVUTIL::GetUTime((cpUTime *)&finTime,60);

		g_adCommand.ReserveCommand(regTime,finTime,eCOMMONTYPE_RENAME_AVATAR,
			g_iSelectedServer+1,0,0,oldName,newName,strUserName);
	}
}

void CPAGE_USERSETTING::OnChangenameSyc() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif
	// TODO: Add your control notification handler code here
	char strId[dID_LENGTH];		memset(strId,0,dID_LENGTH);
	char oldName[dNAME_LENGTH];	memset(oldName,0,dNAME_LENGTH);

	m_ctlChgNameID.GetWindowText(strId,dID_LENGTH);
	if(strcmp(strId,"")==0 || strlen(strId)<4){	MessageBox(g_strToolMessage[30]);	return;	}
	m_ctlChgOldName.GetWindowText(oldName,dNAME_LENGTH);
	if(strcmp(oldName,"")==0 || strlen(oldName)<4){	MessageBox(g_strToolMessage[28]);	return;	}

	OnChangenameSyc( g_iSelectedServer, strId, oldName);
}

void CPAGE_USERSETTING::OnChangenameSyc(int serverindex,char *strId, char *newname)
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif
	if(strcmp(strId,"")==0 || strlen(strId)<4){	MessageBox(g_strToolMessage[30]);	return;	}
	if(strcmp(newname,"")==0 || strlen(newname)<4){	MessageBox(g_strToolMessage[28]);	return;	}

	if(!DB.Connect(	sList[serverindex].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))	{	MessageBox(g_strToolMessage[38]);	return;	}
	
	char oldName[dNAME_LENGTH];
	int guildIdx = DB.spAvaRenameSyc(strId,newname,oldName);
	if( guildIdx >=0 )
	{
//		_log(g_strToolMessage[52],strUserName,sList[g_iSelectedServer].strName,strId,oldName,newName);
		MessageBox(g_strToolMessage[53]);
	}else{
//		_log(g_strToolMessage[54],strUserName,strId,oldName,newName);
		MessageBox(g_strToolMessage[55]);
	}
	DB.Close();

	//	WORLD 서버에 접속해서 변경된 정보의 적용을 요청한다.
	if(guildIdx != 0xffff)
	{
		BOOL	isSingleuseConnection = FALSE;
		if	(IsConnectedServer(serverindex)	==	FALSE)
		{	//	강제로 접속을 시도한다.
			MessageBox("해당 월드 서버와 접속이 끊어져 있어서 연결을 자동으로 설정합니다.",sList[serverindex].strIP,MB_OK);
			if(!sList[serverindex].worldsock.init(dWORLDSERVER_PORT,sList[serverindex].strIP,sList[serverindex].strIP,sizeof(cMSG_BASE_TYPE_FORWORLD)))
			{
				_log("ERROR -> connect to world server : %d , %s",serverindex,sList[serverindex].strIP);
				MessageBox("Failed to connect the world server. You have to send the DB Administrator this message.","ERROR!!",MB_OK);
				return ; 	
			}
			sList[serverindex].isConnect = TRUE;
			sList[serverindex].hThread = (HANDLE)_beginthread(cThreadFunc::WorldCtlThread, 0 ,(void*)serverindex);
			isSingleuseConnection = TRUE;
//			m_ctlServerTree.SetItemImage( sList[serverindex].hTreeItem, 1, 1 );			
		}
		SERVERStoWORLD_SYNC_CHANGEDNAME		sPacket;
		sPacket.base.set(sizeof(SERVERStoWORLD_SYNC_CHANGEDNAME),dSERVERStoWORLD_SYNC_CHANGEDNAME);
		sPacket.guildSerial = (WORD)guildIdx;
		strcpy(sPacket.strID,strId);
		strcpy(sPacket.oldName,oldName);
		strcpy(sPacket.newName,newname);

		sList[serverindex].worldsock.SendPacket((char *)&sPacket,sPacket.base.wSize);
		sList[serverindex].worldsock.ReallySend();
		if(isSingleuseConnection)
		{
			MessageBox("패킷을 종료합니다.",sList[serverindex].strIP,MB_OK);
			Sleep(1000);
			sList[serverindex].worldsock.kill();
			sList[serverindex].isConnect = FALSE;
		}
	}else
	{
		MessageBox("해당 캐릭터는 길드 정보를 포함하고 있지 않습니다.","");
	}
}

void CPAGE_USERSETTING::OnAvaRecover() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))	return;
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
	char strId[dID_LENGTH];		memset(strId,0,dID_LENGTH);
	char oldName[dNAME_LENGTH];	memset(oldName,0,dNAME_LENGTH);
	char newName[dNAME_LENGTH];	memset(newName,0,dNAME_LENGTH);

	int  iSelectedServerCur = g_iSelectedServer;
	m_ctlAvaRecId.GetWindowText(strId,dID_LENGTH);
	if(strcmp(strId,"")==0 || strlen(strId)<4){	MessageBox(g_strToolMessage[30]);	return;	}
	m_ctlAvaRecName.GetWindowText(oldName,dNAME_LENGTH);
	if(strcmp(oldName,"")==0 || strlen(oldName)<4){	MessageBox(g_strToolMessage[28]);	return;	}
	m_ctlAvaRecNewName.GetWindowText(newName,dNAME_LENGTH);
	if(strcmp(newName,"")==0 || strlen(newName)<4){	MessageBox(g_strToolMessage[28]);	return;	}

#ifdef _FOR_KOREA
	if(strstr(sList[iSelectedServerCur].strName,"소룬드"))
	{
		MessageBox("소룬드 서버의 인덱스 수정은 잠시 막아두었습니다.");
		return;
	}
#endif

	if(!DB.Connect(	sList[iSelectedServerCur].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))	{	MessageBox(g_strToolMessage[38]);	return;	}
	PACKET_RECOVERAVA	sPacket;
	sPacket.base.set(sizeof(PACKET_RECOVERAVA),ePACKET_RECOVERAVA);
	strcpy(sPacket.strId,strAdminId);
	strcpy(sPacket.strServerName,sList[iSelectedServerCur].strName);
	strcpy(sPacket.strUserId,strId);
	strcpy(sPacket.strOldUserName,oldName);
	strcpy(sPacket.strNewUserName,newName);
	if(!logSock.SendPacket((char *)&sPacket,sPacket.base.wSize))
	{	DB.Close();
		AfxMessageBox("Could not Send to Log Server!!");
		return;
	}

	char strTxt[1024];
	int ret = DB.spAvaRecover(strId,oldName,newName);
	switch(ret)
	{
	case -1:
		sprintf(strTxt,g_strToolMessage[57]);
		break;
	case 0:
		{	_log(g_strToolMessage[58],strUserName,sList[iSelectedServerCur].strName,strId,oldName);
			sprintf(strTxt,g_strToolMessage[59]);
			DeleteList(iSelectedServerCur, strId);
			break;
		}
	case 1:
		{	_log(g_strToolMessage[60],strUserName,sList[iSelectedServerCur].strName,strId,oldName,newName);
			sprintf(strTxt,g_strToolMessage[61]);
			OnChangenameSyc(iSelectedServerCur,strId,newName);
			DeleteList(iSelectedServerCur, strId);
			break;
		}
	case 17:
		sprintf(strTxt,g_strToolMessage[62]);
		break;
	case 18:
		sprintf(strTxt,g_strToolMessage[63]);
		break;
	case 4:
		sprintf(strTxt,g_strToolMessage[64]);
		break;
	case 5:
		_log(g_strToolMessage[65],strUserName,sList[iSelectedServerCur].strName,strId,oldName,newName);
		sprintf(strTxt,g_strToolMessage[66]);
		MessageBox(strTxt,"ERROR!!");
		break;
	case 6:
		MessageBox("This data is too old. It can't recover!!", "ERROR!!");
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		if(DB.spChangeIndex(strId,newName,ret%10))
		{
			sprintf(strTxt,g_strToolMessage[68]);
			_log(" CHECK :: ");
			_log(g_strToolMessage[69],strUserName,sList[iSelectedServerCur].strName,strId,oldName,newName,ret%10);
		}else
		{
			sprintf(strTxt,g_strToolMessage[70]);
			_log(" CHECK :: ");
			_log(g_strToolMessage[71],strUserName,sList[iSelectedServerCur].strName,strId,oldName,newName,ret%10);
		}
		//	index가 변경된 경우다. 
		DeleteList(iSelectedServerCur, strId);
		break;
	}
	MessageBox(strTxt);
	DB.Close();
}

void CPAGE_USERSETTING::OnSearchId() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif
	// TODO: Add your control notification handler code here
	if	(g_iSelectedServer==-1)
	{	
		MessageBox(g_strToolMessage[37]);
		return;
	}

	SERVERStoWORLD_ISCONNECT_USER	isCon;
	isCon.base.set(sizeof(SERVERStoWORLD_ISCONNECT_USER),dSERVERStoWORLD_ISCONNECT_USER);
	m_ctlSearchId.GetWindowText(isCon.strId,dID_LENGTH);

	if	(strlen(isCon.strId)	<	4)
	{	
		MessageBox(g_strToolMessage[30]);
		return;
	}

	int	iss = g_iSelectedServer;

	if	(!sList[iss].isConnect)
	{	
		MessageBox(g_strToolMessage[38]);
		return;
	}

	sList[iss].worldsock.SendPacket((char *)&isCon,isCon.base.wSize);
}

void CPAGE_USERSETTING::OnListDelete()
{
	// TODO: Add your control notification handler code here
	if(g_iSelectedServer==-1)
	{	MessageBox(g_strToolMessage[37]);
		return;
	}
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	char	strId[dID_LENGTH];
	m_ctlSearchId.GetWindowText(strId,dID_LENGTH);
	if(strcmp(strId,"")==0 || strlen(strId)<4)
	{	MessageBox(g_strToolMessage[30]);
		return;
	}
	DeleteList(g_iSelectedServer,strId);
}

void CPAGE_USERSETTING::OnSaveUpdate() 
{
	// TODO: Add your control notification handler code here
	if(g_iSelectedServer==-1)
	{	MessageBox(g_strToolMessage[37]);
		return;
	}
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if	(CheckOperatorLevel(eAL_OPERATOR)	==	FALSE)
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if	(CheckOperatorLevel(eAL_OPERATOR)	==	FALSE)
		return;
#endif

/*	SERVERStoWORLD_ISCONNECT_USER	isCon;
	isCon.base.set(sizeof(SERVERStoWORLD_ISCONNECT_USER),dSERVERStoWORLD_ISCONNECT_USER);
	m_ctlSearchId.GetWindowText(isCon.strId,dID_LENGTH);
	if(strcmp(isCon.strId,"")==0 || strlen(isCon.strId)<4)
	{	MessageBox(g_strToolMessage[30]);
		return;
	}
*/
	int	iss = g_iSelectedServer;
	if( !sList[iss].dbcsock.init(dDBCACHE_PORT,sList[iss].strDBCIP,sList[iss].strDBCIP,sizeof(cMSG_BASE_TYPE_FORDB)) )
	{
		MessageBox(g_strToolMessage[38]);
		return;
	}
	//	DBC에 연결되었으면 할일을 한다.
/*	LDMSG_UPDATESAVE_USER		updateSaveUser;
	updateSaveUser.base.set(sizeof(LDMSG_UPDATESAVE_USER),dLDMSG_UPDATESAVE_USER);
	m_ctlSearchId.GetWindowText(updateSaveUser.strId,dID_LENGTH);

	sList[iss].dbcsock.SendPacket((char *)&updateSaveUser,updateSaveUser.base.wSize);
	//	메세지가 돌아오기를 기다린다.
	int	sleepcount = 0;
	DLMSG_UPDATESAVE_USER	sUpdateSave;
	memset(&sUpdateSave,0,sizeof(DLMSG_UPDATESAVE_USER));

	char recvpacket[dMAX_PACKET_LENGTH];
	while(sleepcount<5000)
	{
		++sleepcount;
		//	특정 패킷을 기다리는걸 하나 만들어둔다.
		if(sList[iss].dbcsock.GetRecvPacket(recvpacket,dDLMSG_UPDATESAVE_USER))
			break;
		Sleep(1);
	}
	sList[iss].dbcsock.kill();

	if(sleepcount<5000)
	{
		//	해당작업을 하고 
		memcpy(&sUpdateSave,recvpacket,sizeof(DLMSG_UPDATESAVE_USER));
		char tmp[512];
		sprintf(tmp,"[%s]의 세이브 파일이 정상적으로 업데이트되었습니다.",sUpdateSave.strId);
		MessageBox(tmp,"세이브 파일 업데이트");
	}else{
		MessageBox("해당 서버에서 응답이 없습니다.","세이브 파일 업데이트");
	}

	//	종료한다.
	sList[iss].dbcsock.kill();	
*/
}

void CPAGE_USERSETTING::OnSaveUpdateAll() 
{
	// TODO: Add your control notification handler code here
/*	char	txt[1024];
	sprintf(txt,"[%s] 서버가 종료되어 있어야 하며, DB 및 save파일을 백업후 사용해 주세요. 진행하시겠습니까?",sList[g_iSelectedServer].strName);
	if(MessageBox(txt,"WARNING!!",MB_YESNO)==IDNO)	return;

	int	iss = g_iSelectedServer;
	if( !sList[iss].dbcsock.init(dDBCACHE_PORT,sList[iss].strDBCIP,sizeof(cMSG_BASE_TYPE_FORDB)) )
	{
		MessageBox(g_strToolMessage[38]);
		return;
	}
	//	DBC에 연결되었으면 할일을 한다.
	LDMSG_UPDATESAVE		updateSave;
	updateSave.base.set(sizeof(LDMSG_UPDATESAVE),dLDMSG_UPDATESAVE);
	sList[iss].dbcsock.SendPacket((char *)&updateSave,updateSave.base.wSize);
	//	메세지가 돌아오기를 기다린다.
	int	sleepcount = 0;
	DLMSG_UPDATESAVE	sUpdateSave;
	memset(&sUpdateSave,0,sizeof(DLMSG_UPDATESAVE));

	char recvpacket[dMAX_PACKET_LENGTH];
	while(sleepcount<60000)
	{
		++sleepcount;
		//	특정 패킷을 기다리는걸 하나 만들어둔다.
		if(sList[iss].dbcsock.GetRecvPacket(recvpacket,dDLMSG_UPDATESAVE))
			break;
		Sleep(1);
	}
	sList[iss].dbcsock.kill();

	if(sleepcount<60000)
	{
		//	해당작업을 하고 
		memcpy(&sUpdateSave,recvpacket,sizeof(DLMSG_UPDATESAVE));
		char tmp[512];
		sprintf(tmp,"세이브 파일 정리 \n Success [%d], Fail[%d]",sUpdateSave.wSuccCount,sUpdateSave.wFailCount);
		MessageBox(tmp,"세이브 파일 업데이트");
	}else{
		MessageBox("작업이 너무 늦어지거나 해당 서버에서 응답이 없습니다.","세이브 파일 업데이트");
	}

	//	종료한다.
	sList[iss].dbcsock.kill();	
*/
}

void CPAGE_USERSETTING::OnCheckBaduser() 
{
	// TODO: Add your control notification handler code here
	char	name[dID_LENGTH],why[128],txt[256];
	strcpy(why,"");
	m_ctlBadName.GetWindowText(name,dID_LENGTH);
	if(strcmp(name,"")==0)	{	MessageBox(g_strToolMessage[30]);	return;	}
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	g_DBAccount.spGetBadInfo(name,why);
	if(stricmp(why,"CANLOGIN")==0)
	{
		sprintf(txt,g_strToolMessage[72],name);
	}
	else if(stricmp(why,"NOTFOUND")==0)
	{
		sprintf(txt,g_strToolMessage[75],name);
	}
	else
	{
		sprintf(txt,g_strToolMessage[73],name,why);
	}
	MessageBox(txt);
}

void	CPAGE_USERSETTING::DeleteList(int iSelectedServerCur, char * strId)
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	if(strcmp(strId,"")==0)
	{
		MessageBox (" ERROR : ID is empty");
		return ;
	}
	LDMSG_DELETE_LIST	delList;
	delList.base.set(sizeof(LDMSG_DELETE_LIST),dLDMSG_DELETE_LIST);
	strcpy(delList.strId,strId);

	if( !sList[iSelectedServerCur].dbcsock.init(dDBCACHE_PORT,sList[iSelectedServerCur].strDBCIP,"Ava DBC",sizeof(cMSG_BASE_TYPE_FORDB)) )
	{
		MessageBox(g_strToolMessage[38]);
		return;
	}
	//	DBC에 연결되었으면 할일을 한다.
	sList[iSelectedServerCur].dbcsock.SendPacket((char *)&delList,delList.base.wSize);
	//	메세지가 돌아오기를 기다린다.
	int	sleepcount = 0;
	DLMSG_DELETE_LIST	sDelList;
	memset(&sDelList,0,sizeof(DLMSG_DELETE_LIST));

	char recvpacket[dMAX_PACKET_LENGTH];
	while(sleepcount<10000)
	{
		++sleepcount;
		if(sList[iSelectedServerCur].dbcsock.GetRecvPacket(recvpacket))
			break;
		Sleep(1);
	}
	sList[iSelectedServerCur].dbcsock.kill();

	MessageBox(g_strToolMessage[74],"Update Index");
}

void CPAGE_USERSETTING::OnGuildCheck() 
{
	// TODO: Add your control notification handler code here
/*	if(!DB.Connect(	"211.189.122.91" , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))
	{	
		MessageBox("DB서버에 연결 실패했습니다.");	return;	
	}
	typedef struct 
	{
		DWORD		idx;
		char		master[18];
		char		name[22];
		WORD		guild;
		WORD		guildRank;
		BOOL		isRight;
	}guildMembers;

	guildMembers		gms[4000];
	int					gmsCount = 0;
	int i;
	memset(gms,0xff,sizeof(guildMembers)*4000);
	//	정보를 다 읽어온다.
	{
		_RecordsetPtr  pRs = NULL;
		_variant_t     v_bufferdata;
		char	       str_Query[1024];
		int i_RecCount;
		char *pBuf = NULL;
		HRESULT hr = CoInitialize(0);

		sprintf(str_Query,"SELECT idx,master,name,guild,guildRank FROM Avatar_Current WHERE guild <> 65535");
		printf(str_Query);
		pRs.CreateInstance(__uuidof(Recordset)); 

		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)DB.GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		i_RecCount = pRs->RecordCount;

		if(!FAILED(hr))
		{ 
			for(i = 0 ;i < i_RecCount ;i++)
			{
				v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
				gms[i].idx = v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
				strcpy(gms[i].master,(_bstr_t)v_bufferdata);
				v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
				strcpy(gms[i].name,(_bstr_t)v_bufferdata);
				v_bufferdata = pRs->Fields->GetItem((LONG)3)->Value;
				gms[i].guild = v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)4)->Value;
				gms[i].guildRank = v_bufferdata.intVal;
				gmsCount++;
				pRs->MoveNext();
			}
		}
		pRs->Close();
	}
	//	검사를 다 해보고... 
	int incorrectCount = 0;
	{
		_RecordsetPtr  pRs = NULL;
		_variant_t     v_bufferdata;
		char	       str_Query[1024];
		int i_RecCount;
		char *pBuf = NULL;
		HRESULT hr = CoInitialize(0);
		for(i = 0 ;i < gmsCount ;i++)
		{
			sprintf(str_Query,"SELECT wGuildSerial FROM GuildMemberList WHERE strName='%s'",gms[i].name);
			printf(str_Query);
			pRs.CreateInstance(__uuidof(Recordset)); 

			pRs->CursorLocation  = adUseClient;	
			hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)DB.GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
			i_RecCount = pRs->RecordCount;
			if(i_RecCount>0)	gms[i].isRight = 1;
			else{
				gms[i].isRight = 0;
				incorrectCount++;
			}
		}
		pRs->Close();
	}

	//	틀린 녀석들은 초기화 하고...
	{
		int		disCount = 0;
		for(i = 0 ;i < gmsCount ;i++)
		{
			if(gms[i].isRight == 0)
			{
				try{
					_CommandPtr		pCmd = NULL;
					HRESULT			hr = CoInitialize(0);

					//Command 객체 이용
					pCmd.CreateInstance(__uuidof(Command));
					pCmd->ActiveConnection = DB.GetConnection();
					pCmd->CommandText = L"SPSETGUILD";
					pCmd->CommandType  = adCmdStoredProc;

					DEF_PARAM(pCmd,"@GUILD") = _variant_t((long)65535);
					DEF_PARAM(pCmd,"@GUILDRANK") = _variant_t((long)65535);
					DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)gms[i].idx);

					hr = pCmd->Execute(NULL,NULL,NULL);
					if(FAILED(hr)){		return ;	}
					VARIANT regIdx;
					regIdx = pCmd->GetParameters()->GetItem("@RESULT")->GetValue();

					if(regIdx.intVal==0)
					{
						disCount++;
					}
				}catch(...)
				{	
				}
			}
		}
	}
	DB.Close();
*/
}

void CPAGE_USERSETTING::OnChangeAvarecoverName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))	return;
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
	char oldName[dNAME_LENGTH];	memset(oldName,0,dNAME_LENGTH);
	char newName[dNAME_LENGTH];	memset(newName,0,dNAME_LENGTH);

	m_ctlAvaRecName.GetWindowText(oldName,dNAME_LENGTH);
	if(strcmp(oldName,"")==0)	return;

	m_ctlAvaRecNewName.SetWindowText(oldName);
}

void CPAGE_USERSETTING::OnChangeCnameName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))	return;
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
	char oldName[dNAME_LENGTH];	memset(oldName,0,dNAME_LENGTH);
	char newName[dNAME_LENGTH];	memset(newName,0,dNAME_LENGTH);

	m_ctlChgOldName.GetWindowText(oldName,dNAME_LENGTH);
	if(strcmp(oldName,"")==0)	return;

	m_ctlChgNewName.SetWindowText(oldName);	
}

void CPAGE_USERSETTING::OnUpdatelist() 
{
	// TODO: Add your control notification handler code here
	char strId[dID_LENGTH];		memset(strId,0,dID_LENGTH);
	m_ctlAvaRecId.GetWindowText(strId,dID_LENGTH);
	if(strcmp(strId,"")==0 || strlen(strId)<4){	MessageBox(g_strToolMessage[30]);	return;	}
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	int  iSelectedServerCur = g_iSelectedServer;
	DeleteList( iSelectedServerCur , strId);
}

void CPAGE_USERSETTING::OnIdCheck() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR | eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	//	ID에 대한 현재 정보를 확인한다. 
	stIDInfo	idInfo;
	char	id[dID_LENGTH];

	m_ctlRecID.GetWindowText(id,dID_LENGTH);
	if(strcmp(id,"")==0)	 {	MessageBox(g_strToolMessage[30]);	return;	}
	char	txt[512];
	WORD wResult = g_DBAccount.spGetIDInfo(id,&idInfo);
	switch(wResult)
	{
	case 0:
		{
			sprintf(txt,g_strToolMessage[50]);
			break;
		}
	case 1:
		{
			sprintf(txt,g_strToolMessage[75],id);
			break;
		}
	case 2:
		{
			sprintf(txt,g_strToolMessage[76],idInfo.dwLastConnect);
			break;
		}
	case 3:
		{
			sprintf(txt,g_strToolMessage[77],idInfo.dwStartConnect,idInfo.dwLastConnect,idInfo.strConnectedServerName,idInfo.dwBlock,idInfo.strWhyBlock );
			break;
		}
	}
	MessageBox(txt,"Get ID Info",MB_OK);
}

void CPAGE_USERSETTING::OnEmer() 
{
	return;
	// TODO: Add your control notification handler code here
#ifdef _FOR_JAPAN
	cDynamicScript		emerScp;
	char				emerName[1024];
	int					succCount = 0,failCount = 0;
	FILE *				sfp;
	FILE *				ffp;
	int  iSelectedServerCur = g_iSelectedServer;

	char txt[1024];
	sprintf(txt,"Server [%s]. is it right??",sList[iSelectedServerCur].strName);
	if(MessageBox(txt,"Warning!! Changing Avatar Name!!",MB_YESNO)==IDNO)	return;

	sprintf(emerName,"%s/chgNameList.txt",strCurPath);
	emerScp.Read(emerName,2000);

	sprintf(txt,"Server [%s]. Avatar Count [%d]. is it right??",sList[iSelectedServerCur].strName,emerScp.scpcount-1);
	if(MessageBox(txt,"Warning!! Changing Avatar Name!!",MB_YESNO)==IDNO)	return;

	sprintf(emerName,"%s/emerSuccessList.txt",strCurPath);
	DeleteFile(emerName);
	sfp = fopen(emerName,"w+");
	if(sfp==NULL){	MessageBox("FILE CREATE ERROR 1");	return;	}

	sprintf(emerName,"%s/emerFailList.txt",strCurPath);
	DeleteFile(emerName);
	ffp = fopen(emerName,"w+");
	if(ffp==NULL){	fclose(sfp);	MessageBox("FILE CREATE ERROR 2");	return;	}

	if(!DB.Connect(	sList[g_iSelectedServer].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))	{	MessageBox(g_strToolMessage[38]);	return;	}
	for(int i = 0;i<emerScp.scpcount-1;i++)
	{
		Sleep(1);
		char strId[dID_LENGTH];		memset(strId,0,dID_LENGTH);
		char oldName[dNAME_LENGTH];	memset(oldName,0,dNAME_LENGTH);
		char newName[dNAME_LENGTH];	memset(newName,0,dNAME_LENGTH);
		char n[10];

		sprintf(n,"%d",i+1);
		strcpy(strId,emerScp.getData(n,1));
		strcpy(oldName,emerScp.getData(n,2));
		strcpy(newName,emerScp.getData(n,3));

		if(strcmp(strId,"")==0 || strlen(strId)<4)
		{	
			fprintf(ffp,"%d = FAIL INFO : *Id[%s] , oldName[%s] , newName[%s]\n",failCount,strId,oldName,newName);failCount++;
			continue;
		}
		if(strcmp(oldName,"")==0 || strlen(oldName)<4)
		{	
			fprintf(ffp,"%d = FAIL INFO : Id[%s] , *oldName[%s] , newName[%s]\n",failCount,strId,oldName,newName);failCount++;
			continue;
		}
		if(strcmp(newName,"")==0 || strlen(newName)<4)
		{	
			fprintf(ffp,"%d = FAIL INFO : Id[%s] , oldName[%s] , *newName[%s]\n",failCount,strId,oldName,newName);failCount++;
			continue;
		}

		int result = DB.spAvaRename(strId,oldName,newName);
		switch(result)
		{
		case -1:
			fprintf(ffp,"%d = FAIL DB : Id[%s] , oldName[%s] , newName[%s]\n",failCount,strId,oldName,newName);failCount++;
			break;
		case 0:
			fprintf(ffp,"%d = oldName and newName are in use!! : Id[%s] , oldName[%s] , newName[%s]\n",failCount,strId,oldName,newName);failCount++;
			break;
		case 1:
			if(strcmp(oldName,newName)!=0)
			{
				if(DB.spAvaRenameSyc(strId,newName))
				{
					fprintf(sfp,"%d = [%s] | [%s] -> NEW NAME [%s]\n",succCount,strId,oldName,newName);
					succCount++;
				}else{
					fprintf(ffp,"%d = name sync fail : Id[%s] , oldName[%s] , newName[%s]\n",failCount,strId,oldName,newName);failCount++;
				}
			}else{
				fprintf(ffp,"%d = oldname is newname -_-; : Id[%s] , oldName[%s] , newName[%s]\n",failCount,strId,oldName,newName);failCount++;
			}
			break;
		case 2:
			fprintf(ffp,"%d = oldname is incorrect : Id[%s] , oldName[%s] , newName[%s]\n",failCount,strId,oldName,newName);failCount++;
			break;
		}
	}
	DB.Close();

	char tmp[1024];
	sprintf(tmp," Success [%d], Fail [%d]",succCount,failCount);
	MessageBox(tmp,"The END !!!");
	fclose(ffp);
	fclose(sfp);
#endif
}

void CPAGE_USERSETTING::OnCheckOperlevel() 
{
	if(m_ctlIsCheckUsingAccount.GetCheck())
	{
		MessageBox("Incorrect Command");
		return;
	}
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	char name[dNAME_LENGTH];	memset(name,0,dNAME_LENGTH);

	m_ctlAdminName.GetWindowText(name,dNAME_LENGTH);
	if(strcmp(name,"")==0 || strlen(name)<4){	MessageBox(g_strToolMessage[28]);	return;	}
	if(g_iSelectedServer==-1 || strcmp(sList[g_iSelectedServer].strName,"")==0){	MessageBox(g_strToolMessage[37]);	return;	}

	if(m_ctlIsCheckUsingAccount.GetCheck())	//	Account DB로 연결해서 계정의 정보를 얻어와라.
	{
	}else
	{
		if(!DB.Connect(	sList[g_iSelectedServer].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))	{	MessageBox(g_strToolMessage[38]);	return;	}
		int opLevel = DB.spGetOperLevel(name);
#ifdef _FOR_KOREA
		if(opLevel>6)
			MessageBox("호혹.. !! \n 문제가 발생하였습니다. 잠시 작업을 멈추시고, 개발팀의 귀염둥이 지현이를 불러주세요.!!");
#endif
		char tmp[10];
		sprintf(tmp,"%d",opLevel);
		m_ctlOperLevel.SetWindowText(tmp);
		DB.Close();
	}
}

void CPAGE_USERSETTING::OnSetTester() 
{
	if(m_ctlIsCheckUsingAccount.GetCheck())
	{
		MessageBox("Incorrect Command");
		return;
	}

	// TODO: Add your control notification handler code here
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	if(m_ctlIsCheckUsingAccount.GetCheck())	return;

	OPENFILENAME	ofn;

	char	strName[512]	=	"";

	SetCurrentDirectory(g_strLogFolder);

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow()->m_hWnd;
	ofn.lpstrFilter		=	"*.txt";
	ofn.lpstrTitle		=	g_strToolMessage[153];
	ofn.lpstrFile		=	strName;
	ofn.nMaxFile		=	512;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING;
	ofn.lpstrDefExt		=	"txt";
	ofn.lpstrInitialDir	=	g_strLogFolder;

	if (!GetOpenFileName(&ofn))	return;

	FILE	*fp;
	fp	=	fopen(strName,"r");
	if(!fp)	{	MessageBox("Not found file!!",strName);	return;	}
//	if(!DB.Connect(	"211.189.40.164" , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))	{	MessageBox(g_strToolMessage[38]);	return;	}
	//	리스트 읽어서 테섭의 SetOper로 넘긴다.
	char	_readline[32];
	int	listcount=0;
	while(1)
	{
		int		tmp=0;
		int		count=0;
		memset(_readline,0,32);

		tmp = fgetc(fp);
		while(tmp!=EOF && tmp!=10)
		{
			_readline[count++] = (char)tmp;
			tmp = fgetc(fp);
		}
		_readline[count]=0;
		if(tmp==EOF)	break;
		if(count==0)	break;
		if(stricmp(_readline,"END")==0)	break;
		//	Set Oper
		DB.spSetOper(_readline,1);
		listcount++;
	}
	char	tmpStr[128];
	sprintf(tmpStr,"총 %d명의 유저를 테스터로 지정하였습니다",listcount);
	DB.Close();
	fclose(fp);

	MessageBox(tmpStr,"Set Tester !!");
}

void CPAGE_USERSETTING::OnResetOper() 
{
	// TODO: Add your control notification handler code here
	if(!DB.Connect(	sList[g_iSelectedServer].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))	{	MessageBox(g_strToolMessage[38]);	return;	}

	DB.spResetOperator();

	DB.Close();
}
