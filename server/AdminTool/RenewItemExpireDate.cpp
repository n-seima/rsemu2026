// RenewItemExpireDate.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "RenewItemExpireDate.h"
#include "SFC.h"
#include "cMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RenewItemExpireDate dialog


RenewItemExpireDate::RenewItemExpireDate(CWnd* pParent /*=NULL*/)
	: CDialog(RenewItemExpireDate::IDD, pParent)
{
	//{{AFX_DATA_INIT(RenewItemExpireDate)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void RenewItemExpireDate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RenewItemExpireDate)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RenewItemExpireDate, CDialog)
	//{{AFX_MSG_MAP(RenewItemExpireDate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int
RenewItemExpireDate::open(char *_lpstrServerName)
{
	strcpy(m_strServerName,_lpstrServerName);

	return	DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// RenewItemExpireDate message handlers

void RenewItemExpireDate::OnOK() 
{
	int		iDays		=	GetDlgItemInt(IDC_RENEW_DAYS,0);
	int		iHours		=	GetDlgItemInt(IDC_RENEW_HOURS,0);
	int		iMinutes	=	GetDlgItemInt(IDC_RENEW_MINUTES,0);
	char	strText[128];

#ifdef	_FOR_KOREA
	if (iDays)
	{
		if (iHours && iMinutes)
			sprintf(strText,"%d 일 %d 시간 %d 분",iDays,iHours,iMinutes);
		else
		if (iHours)
			sprintf(strText,"%d 일 %d 시간",iDays,iHours);
		else
		if (iMinutes)
			sprintf(strText,"%d 일 %d 분",iDays,iMinutes);
		else
			sprintf(strText,"%d 일",iDays);
	}
	else
	if (iHours)
	{
		if (iMinutes)
			sprintf(strText,"%d 시간 %d 분",iHours,iMinutes);
		else
			sprintf(strText,"%d 시간",iHours);
	}
	else
		sprintf(strText,"%d 분",iMinutes);
#else
	if (iDays)
	{
		if (iHours && iMinutes)
			sprintf(strText,"%d Days %d Hours %d  Minutes",iDays,iHours,iMinutes);
		else
		if (iHours)
			sprintf(strText,"%d Days %d Hours",iDays,iHours);
		else
		if (iMinutes)
			sprintf(strText,"%d Days %d Minutes",iDays,iMinutes);
		else
			sprintf(strText,"%d Days",iDays);
	}
	else
	if (iHours)
	{
		if (iMinutes)
			sprintf(strText,"%d Hours %d Minutes",iHours,iMinutes);
		else
			sprintf(strText,"%d Hours",iHours);
	}
	else
		sprintf(strText,"%d Minutes",iMinutes);
#endif
	if	(cMSG::YESNO(g_strToolMessage[151],g_strToolMessage[152],m_strServerName,strText)	==	IDNO)
	{
		CDialog::OnCancel();
		return;
	}

	// TODO: Add extra validation here
	m_time.year		=	GetDlgItemInt(IDC_YEAR)%100;
	m_time.month	=	GetDlgItemInt(IDC_MONTH);
	m_time.day		=	GetDlgItemInt(IDC_DAY);
	m_time.hour		=	GetDlgItemInt(IDC_HOUR);
	m_time.minute	=	GetDlgItemInt(IDC_MINUTE);

	m_dwRenewMunute	=	iMinutes+iHours*60+iDays*60*24;

	m_wIsForOnlyYahoo	=	FALSE;
	m_wIsForOnlyRedgem	=	FALSE;

#ifdef	_FOR_KOREA
	m_wIsForOnlyYahoo	=	IsDlgButtonChecked(IDC_IS_FOR_ONLY_YAHOO);
	m_wIsForOnlyRedgem	=	IsDlgButtonChecked(IDC_IS_FOR_ONLY_REDGEM);
#endif

	CDialog::OnOK();
}

void RenewItemExpireDate::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL RenewItemExpireDate::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CTimeInfo	time;

	time.update();

	SetDlgItemInt(IDC_YEAR,time.m_wYear);
	SetDlgItemInt(IDC_MONTH,time.m_wMonth);
	SetDlgItemInt(IDC_DAY,time.m_wDay);
	SetDlgItemInt(IDC_HOUR,time.m_wHour);
	SetDlgItemInt(IDC_MINUTE,time.m_wMinute);

	SetDlgItemInt(IDC_RENEW_DAYS,0);
	SetDlgItemInt(IDC_RENEW_HOURS,0);
	SetDlgItemInt(IDC_RENEW_MINUTES,0);
	
#ifndef	_FOR_KOREA
	::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_IS_FOR_ONLY_YAHOO),SW_HIDE);
	::ShowWindow(::GetDlgItem(this->m_hWnd,IDC_IS_FOR_ONLY_REDGEM),SW_HIDE);
#endif
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
