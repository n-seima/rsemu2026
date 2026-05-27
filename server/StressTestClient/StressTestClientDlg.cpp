// StressTestClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StressTestClient.h"
#include "StressTestClientDlg.h"
#include "Tracer.h"
#include "cSOCKET.h"
#include "mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int	l_iPort		=	50631;

class CClient	:	public cSOCKET
{
public:
	int		m_iSerial;
	DWORD	m_dwTickCounter;
	DWORD	m_dwSendByte,m_dwSendCount;
	BOOL	m_bIsSendComplete;

	void	reset()
	{
		m_iSerial			=	0xffffffff;
		m_dwTickCounter		=	0;
		m_dwSendByte		=	0;
		m_dwSendCount		=	0;
		m_bIsSendComplete	=	FALSE;
	}

	void	send(int _iSendSize)
	{
		CG_STRESSDATA	sPacket;

		sPacket.base.set(_iSendSize,dCG_STRESSDATA);

		SendPacket((char *)&sPacket,sPacket.base.wSize);
	}

	void	sendPacket(cMSG_BASE_TYPE *_lpPacket)
	{
		SendPacket((char *)_lpPacket,_lpPacket->wSize);
	}
};


DebugTracer * cLOG;
CClient		*m_pClient	=	NULL;

/////////////////////////////////////////////////////////////////////////////
// CStressTestClientDlg dialog

CStressTestClientDlg::CStressTestClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStressTestClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStressTestClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon				=	AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_iOldUserCount		=	0;
	m_iConnectUserCount	=	0;
	m_iTickCounter		=	0;
	m_bIsRegistClinet	=	FALSE;
}

void CStressTestClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStressTestClientDlg)
	DDX_Control(pDX, IDC_UPDATE_SPEED, m_staticUpdateSpeed);
	DDX_Control(pDX, IDC_CONNECTED_USER_COUNT, m_editConnectUserCount);
	DDX_Control(pDX, IDC_PORT, m_ctlPort);
	DDX_Control(pDX, IDC_IPADDR, m_ctlIP);
	DDX_Control(pDX, IDC_DATAAMOUNT, m_ctlDataAmount);
	DDX_Control(pDX, IDC_USERCOUNT, m_ctlUserCount);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStressTestClientDlg, CDialog)
	//{{AFX_MSG_MAP(CStressTestClientDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDSTART, OnStart)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStressTestClientDlg message handlers

BOOL CStressTestClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_isConnecting	=	FALSE;
	cLOG			=	new DebugTracer(DTType_File,"Stress Test Client");

	m_iAverageUpdateTime	=	0;
	m_iUpdateTimeCounter	=	0;
	m_iUpdateTimeSum		=	0;

	FILE	*fp;

	fp	=	fopen("config.dat","rb");

	if (fp)
	{
		char	strIp[20];
		int		iPort;

		fread(strIp,20,1,fp);
		fread(&iPort,4,1,fp);

		fclose(fp);

		SetDlgItemText(IDC_IPADDR,strIp);
		SetDlgItemInt(IDC_PORT,iPort);
	}

	SetTimer(1,100,NULL);
	SetTimer(2,100,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CStressTestClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CStressTestClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

#define dMAXDATALENGTH	102400
#define dMAXUSERCOUNT	5000

void CStressTestClientDlg::OnStart() 
{
	char	strDataLength[10];
	char	strUserCount[10];

	m_ctlDataAmount.GetWindowText(strDataLength,8);
	m_ctlUserCount.GetWindowText(strUserCount,8);

	m_iUserCount	=	atoi(strUserCount);	

	m_iUserCount	=	min(m_iUserCount,1000);
	m_iUserCount	=	max(m_iUserCount,1);

	// TODO: Add your control notification handler code here
	{
		if(MessageBox("Do you start Stress Test?","START!!",MB_YESNO)==IDNO)
			return;

		if(!ConnectServer())
		{	
			MessageBox("CONNECT FAIL !!");	
			return;	
		}
		m_isConnecting	=	TRUE;
	}

//	CG_STRESSCLIENT	sPacket;
//	sPacket.base.set(sizeof(sPacket),dCG_STRESSCLIENT);
//	sPacket.wSendSize = atoi(strDataLength);
//	sPacket.wUserCount = atoi(strUserCount);
//	m_socSock.SendPacket((char *)&sPacket,sPacket.base.wSize);
}

BOOL CStressTestClientDlg::ConnectServer()
{
	char	strIp[18];
	char	strPort[10];
	int		iPort;
	int		iUniqueSerial	=	timeGetTime();

	m_ctlIP.GetWindowText(strIp,16);
	m_ctlPort.GetWindowText(strPort,8);

	iPort	=	atoi(strPort);

	m_iAverageUpdateTime	=	0;
	m_iUpdateTimeCounter	=	0;
	m_iUpdateTimeSum		=	0;

	if	(strcmp(strIp,"")==0 || strlen(strIp)<=8)
	{
		MessageBox("Input IP Address... ");

		return FALSE;
	}

	if	(strcmp(strPort,"")==0 || iPort<=1000)
	{
		MessageBox("Input Port Number.. ");

		return FALSE;
	}

	if	(m_iOldUserCount	!=	m_iUserCount)
	{
		m_iOldUserCount		=	m_iUserCount;

		if	(m_iConnectUserCount	==	0)
		{
			m_iConnectUserCount		=	m_iUserCount;
			m_iSendCountPer10Second	=	max(m_iUserCount/100,1);
			m_iSendSizePer10Second	=	m_iUserCount/20+60;
			m_iSendSizePer10Second	=	max(m_iSendSizePer10Second,60);
		}

		if	(m_pClient)
			delete [] m_pClient;

		m_pClient		=	new CClient	[m_iUserCount];

		for (int i=0;i<m_iUserCount;i++)
		{
			if	(!m_pClient[i].init(iPort,strIp,sizeof(cMSG_BASE_TYPE)))
				return	FALSE;

			m_pClient[i].reset();
			
			CG_CONNECT_STRESS_CLIENT	packet;

			packet.base.set(sizeof(packet),dCG_CONNECT_STRESS_CLIENT);
			packet.iUniqueSerial			=	iUniqueSerial;

			m_pClient[i].sendPacket((cMSG_BASE_TYPE *)&packet);

			m_pClient[i].m_iSerial			=	i;
			m_pClient[i].m_bIsSendComplete	=	TRUE;
			m_pClient[i].m_dwTickCounter	=	i%10;

			if	(m_bIsRegistClinet	==	FALSE)
			{
				m_bIsRegistClinet	=	TRUE;

				CG_REGIST_STRESS_CLIENT	packetRegist;

				packetRegist.base.set(sizeof(packetRegist),dCG_REGIST_STRESS_CLIENT);
				packetRegist.iUniqueSerial	=	iUniqueSerial;

				m_pClient[0].sendPacket((cMSG_BASE_TYPE *)&packetRegist);
			}

			char	strText[50];

			sprintf(strText,"%d",i+1);

			m_editConnectUserCount.SetWindowText(strText);

			Sleep(1);
		}

		CG_MODIFY_STRESS_CLIENT	packetModify;

		packetModify.base.set(sizeof(packetModify),dCG_MODIFY_STRESS_CLIENT);

		m_pClient[m_iUserCount-1].sendPacket((cMSG_BASE_TYPE *)&packetModify);
	}

	FILE *fp	=	fopen("config.dat","wb");

	if (fp)
	{
		fwrite(strIp,20,1,fp);
		fwrite(&iPort,4,1,fp);

		fclose(fp);
	}

	return	TRUE;
}

void CStressTestClientDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	int	i;

	if(nIDEvent ==1 && m_isConnecting)
	{
		for (i=0;i<m_iUserCount;i++)
		{
			if (!m_pClient[i].isLoopRecv)	
				continue;

			WORD	recvsize=0,type=0;
			char	recvpacket[dMAX_PACKET_LENGTH];

			while(m_pClient[i].GetRecvPacket(recvpacket))
			{
				recvsize = *(WORD *)recvpacket;
				type = *(WORD *)(recvpacket+2);

				switch(type)
				{
					case dSG_STRESSCLIENT:
					{
						SG_STRESSCLIENT	* rPacket = (SG_STRESSCLIENT *)recvpacket;

						break;
					}

					case dSG_STRESS_SERVER_INFO	:
					{
						SG_STRESS_SERVER_INFO	*lpPacket	=	(SG_STRESS_SERVER_INFO *)recvpacket;

						m_iConnectUserCount		=	lpPacket->iConnectUserCount;
						m_iSendCountPer10Second	=	max(m_iConnectUserCount/100,1);
						m_iSendSizePer10Second	=	m_iConnectUserCount/15+60;
						m_iSendSizePer10Second	=	max(m_iSendSizePer10Second,60);

						m_iUpdateTimeSum		+=	lpPacket->iUpdateTime;
						m_iUpdateTimeCounter++;

						if	(m_iUpdateTimeCounter	>=	10)
						{
							m_iAverageUpdateTime=	m_iUpdateTimeSum/10;
							m_iUpdateTimeCounter=	0;
							m_iUpdateTimeSum	=	0;
						}

						char	strText[50];

						sprintf(strText,"%d",m_iConnectUserCount);

						m_editConnectUserCount.SetWindowText(strText);

						sprintf(strText,"%d:%d[every 10 second]",lpPacket->iUpdateTime,m_iAverageUpdateTime);

						m_staticUpdateSpeed.SetWindowText(strText);

						break;
					}
				}
			}
		}
	}

	if	(nIDEvent ==2 && m_isConnecting)
	{
		m_iTickCounter++;

		for (i=0;i<m_iUserCount;i++)
		{
			CClient	*lpClient		=	&m_pClient[i];

			if	(lpClient->m_bIsSendComplete == FALSE)
			{
				int	iSendCount				=	max(m_iSendCountPer10Second/10,1);
				int	iCurrentTimeSendSize	=	m_iSendSizePer10Second/10/m_iSendCountPer10Second;

				if	(lpClient->m_dwTickCounter	==	9)
					iCurrentTimeSendSize	=	m_iSendSizePer10Second-lpClient->m_dwSendByte;

				int	iSendSize				=	iCurrentTimeSendSize/iSendCount;
				int	iCurrentTimeSendSizeSum	=	0;

				for (int iCount=0;iCount<iSendCount;iCount++)
				{
					if	(iCount	==	iSendCount-1)
						iSendSize	=	max(iCurrentTimeSendSize-iCurrentTimeSendSizeSum,6);

					if	(int(iSendSize+lpClient->m_dwSendByte)	>	m_iSendSizePer10Second)
						iSendSize	=	max(m_iSendSizePer10Second-lpClient->m_dwSendByte,6);

					if	(iSendSize)
					{
						lpClient->m_dwSendByte	+=	iSendSize;
						lpClient->send(iSendSize);
					}

					if (int(lpClient->m_dwSendByte)	>=	m_iSendSizePer10Second)
					{
						lpClient->m_bIsSendComplete	=	TRUE;
						break;
					}
				}
			}

			lpClient->m_dwTickCounter++;

			if (lpClient->m_dwTickCounter	>=	10)
			{
				lpClient->m_dwTickCounter	=	0;
				lpClient->m_dwSendByte		=	0;
				lpClient->m_dwSendCount		=	0;
				lpClient->m_bIsSendComplete	=	FALSE;
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}