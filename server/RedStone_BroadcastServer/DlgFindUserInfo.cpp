// DlgFindUserInfo.cpp : implementation file
//

#include "stdafx.h"
#include "RedStone_BroadCastServer.h"
#include "DlgFindUserInfo.h"
#include <mmsystem.h>

#include "TemporaryListForEntry.h"
#include "Client.h"
#include "cSRVUTIL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFindUserInfo property page

IMPLEMENT_DYNCREATE(CDlgFindUserInfo, CPropertyPage)

CDlgFindUserInfo::CDlgFindUserInfo() : CPropertyPage(CDlgFindUserInfo::IDD)
{
	//{{AFX_DATA_INIT(CDlgFindUserInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgFindUserInfo::~CDlgFindUserInfo()
{
}

void CDlgFindUserInfo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFindUserInfo)
	DDX_Control(pDX, IDC_EDIT2, m_ctlSendEdit);
	DDX_Control(pDX, IDC_EDIT1, m_ctlRecvEdit);
	DDX_Control(pDX, IDC_COMBO1, m_ctlServerList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFindUserInfo, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgFindUserInfo)
	ON_BN_CLICKED(IDC_OUTPUT_ALL_INFO, OnOutputAllInfo)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnKillThisServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFindUserInfo message handlers

void CDlgFindUserInfo::OnOutputAllInfo() 
{
	// TODO: Add your control notification handler code here

	char strFn[128];

	sprintf(strFn,"ALLINFO_%d_%d.txt",cSRVUTIL::GetTime(),cSRVUTIL::GetTimes());

	FILE * fp = fopen(strFn,"a+");
	
	if(!fp)
	{
		MessageBox("Can't open the file.");
		return;
	}

	fseek(fp,0,SEEK_SET);

	fprintf(fp,"==================================\n");
	fprintf(fp," 유저 정보 및 각종 데이터 모음... \n");
	fprintf(fp,"==================================\n");

	fprintf(fp,"START Temporary List==============\n");
	for(int i = dBC_MAX_USER_COUNT;i;)
	{	--i;
		stCLIENTDATA * _pData = cTempList.GetDataPnt(i);
		if(!_pData || _pData->wSerial==0xffff)	continue;

		fprintf(fp, " id[%s] name[%s], field[%d], delay time [%d(sec) %d/%d/%d %d:%d:%d]\n",_pData->strId,_pData->strName,_pData->wCurrentField,((timeGetTime()-_pData->dwRegistTime)/1000),_pData->timeRegistedTime.year, _pData->timeRegistedTime.month, _pData->timeRegistedTime.day, _pData->timeRegistedTime.hour,_pData->timeRegistedTime.minute, _pData->timeRegistedTime.second);
	}
	fprintf(fp,"End Temporary List================\n\n\n");

	fprintf(fp,"START Regiested Client List=======\n");
	for(i = dBC_MAX_USER_COUNT;i;)
	{	--i;
		CClient * _pClient = UserManager.GetClientPnt(i);
		if(!_pClient || _pClient->m_wSerial==0xffff || _pClient->m_isGameServer)	continue;

		fprintf(fp, " id[%s] name[%s] ip[%s]\n",_pClient->m_strId,_pClient->m_strName,_pClient->m_strIp);
	}
	fprintf(fp,"END Regiested Client List=========\n\n\n");

	fprintf(fp,"START Game Server List============\n");
	for(i = dBC_MAX_USER_COUNT;i;)
	{	--i;
		CClient * _pClient = UserManager.GetClientPnt(i);
		if(!_pClient || _pClient->m_wSerial==0xffff || !_pClient->m_isGameServer)	continue;

		fprintf(fp, " id[%s] name[%s] ip[%s]\n",_pClient->m_strId,_pClient->m_strName,_pClient->m_strIp);
	}
	fprintf(fp,"END Game Server List==============\n\n\n");

	fclose(fp);
}

void CDlgFindUserInfo::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(!this->IsWindowVisible())
	{
		KillTimer(1);
	}else
	{
		if(m_dequeServerList.size())
		{
			cGSvrInfo info = m_dequeServerList.at(m_ctlServerList.GetCurSel());
			CClient * _pClient = UserManager.GetClientPnt(info.iSerial);
			if(!_pClient || _pClient->m_wSerial==0xffff || !_pClient->m_isGameServer)
			{
				m_ctlRecvEdit.SetWindowText(" Incorrect Game Server Infomation");
			}else
			{
				char tmp[128];
				sprintf(tmp,"%d",_pClient->m_dwRecvSizePerSec);
				m_ctlRecvEdit.SetWindowText( tmp );
				sprintf(tmp,"%d",_pClient->m_dwSendSizePerSec);
				m_ctlSendEdit.SetWindowText( tmp );
			}
		}
	}	
	CPropertyPage::OnTimer(nIDEvent);
}

BOOL CDlgFindUserInfo::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add your specialized creation code here
	for(int i = dBC_MAX_USER_COUNT;i;)
	{	--i;
		CClient * _pClient = UserManager.GetClientPnt(i);
		if(!_pClient || _pClient->m_wSerial==0xffff || !_pClient->m_isGameServer)	continue;

		m_ctlServerList.InsertString(0,_pClient->m_strIp);
		cGSvrInfo info(_pClient->m_wSerial,_pClient->m_strIp);
		m_dequeServerList.push_front(info);
	}
	if(m_ctlServerList.GetCount())
	{
		m_ctlServerList.SetCurSel(0);
	}

	SetTimer(1,1000,NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFindUserInfo::OnKillThisServer() 
{
	// TODO: Add your control notification handler code here
	if(m_dequeServerList.size())
	{
		cGSvrInfo info = m_dequeServerList.at(m_ctlServerList.GetCurSel());
		CClient * _pClient = UserManager.GetClientPnt(info.iSerial);
		if(!_pClient || _pClient->m_wSerial==0xffff || !_pClient->m_isGameServer)
		{
		}else
		{
			MessageBox("Disconnect this server!!",_pClient->m_strIp);
			DisUser(_pClient->m_wSerial,"Disconnected by operator");
		}
	}
}
