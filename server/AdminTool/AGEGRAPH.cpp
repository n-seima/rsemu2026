// AGEGRAPH.cpp : implementation file
//

#include "stdafx.h"
#include "AdminTool.h"
#include "AGEGRAPH.h"
#include "CMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WHITE		RGB(255,255,255)
#define BLACK		RGB(0,0,0)

#define	LIGHTGRAY	RGB(192,192,192)
#define YELLOW		RGB(255,255,0)
#define LIGHTCYAN	RGB(0,255,255)

#define BLUE		RGB(0,0,255)
#define	RED			RGB(255,0,0)
#define BROWN		RGB(128,128,0)
#define GREEN		RGB(0,255,0)
#define MAGENTA		RGB(128,0,128)

#define dSTART_LEVEL	1
#define dMAX_LEVEL		1000
#define dMAX_PERSONAL	500


/////////////////////////////////////////////////////////////////////////////
// CAGEGRAPH dialog


CAGEGRAPH::CAGEGRAPH(CWnd* pParent /*=NULL*/)
	: CDialog(CAGEGRAPH::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAGEGRAPH)
	//}}AFX_DATA_INIT
	memset(iConnectGraph,0,sizeof(int)*8*24);
	memset(iPlayGraph,0,sizeof(int)*8*24);
	memset(iDL,0,sizeof(int)*dMAX_WORLD_SERVER_COUNT*200);
}

void CAGEGRAPH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAGEGRAPH)
	DDX_Control(pDX, IDC_SERVER_LIST, m_ctlServerList);
	DDX_Control(pDX, IDC_TYPE, m_ctlTypeList);
	DDX_Control(pDX, IDC_SELECT_LIST, m_ctlSelectList);
	DDX_Control(pDX, IDC_AGE_LIST, m_ctlAgeList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAGEGRAPH, CDialog)
	//{{AFX_MSG_MAP(CAGEGRAPH)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_AGE_LIST, OnSelchangeAgeList)
	ON_CBN_SELCHANGE(IDC_SELECT_LIST, OnSelchangeSelectList)
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	ON_CBN_SELCHANGE(IDC_SERVER_LIST, OnSelchangeServerList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAGEGRAPH message handlers

void	CAGEGRAPH::Set(int **c,int **p,int t,vector<cADO_ADMIN::stUserStatsPack> * _pVector)
{
	iServer = 0;	iSetType = 0;	iSelectGraph = 0;	iSetAge = -1;	time = t;
	memcpy(iConnectGraph,c,sizeof(int)*8*24);
	memcpy(iPlayGraph,p,sizeof(int)*8*24);
	m_pVector = _pVector;
	m_allMaxIndex = 0;
	int tmp = 0;;
	for(int j=0;j<1440;j++)
	{
		int tot = 0;
		for(int i = 0;i<g_iWorldCount;i++)
		{
			if((int)m_pVector[i].size() <= j )	continue;
			cADO_ADMIN::stUserStatsPack _pack = m_pVector[i].at(j);
			tot += _pack.iAllUserCount;
		}
		m_VectorAll.push_back( tot );
		if(tot>tmp)
		{
			m_allMaxIndex = j;
			tmp = tot;
		}
	}
}

BOOL CAGEGRAPH::OnInitDialog() 
{
#ifndef _FOR_KOREA
	SetWindowText(g_strDlgText[2]);
#endif
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctlSelectList.AddString("0. 모두 보기");
	m_ctlSelectList.AddString("1. 접속 횟수별");
	m_ctlSelectList.AddString("2. 플레이 시간별");
	m_ctlSelectList.SetCurSel(0);
	iSelectGraph = 0;

	m_ctlAgeList.AddString("0. 모두 보기");
	m_ctlAgeList.AddString("13세 이하");
	m_ctlAgeList.AddString("14세 ~ 16세");
	m_ctlAgeList.AddString("17세 ~ 19세");
	m_ctlAgeList.AddString("20세 ~ 22세");
	m_ctlAgeList.AddString("23세 ~ 25세");
	m_ctlAgeList.AddString("26세 ~ 29세");
	m_ctlAgeList.AddString("30세 ~ 35세");
	m_ctlAgeList.AddString("36세 이상");
	m_ctlAgeList.SetCurSel(0);

	m_ctlTypeList.AddString("0. 연령, 시간대별 자료");
	m_ctlTypeList.AddString("1. 레벨 분포도");
	m_ctlTypeList.AddString("2. 시간별 유저 접속 통계");
	m_ctlTypeList.SetCurSel(0);

	char s_name[34];
	m_ctlServerList.AddString("0. 모든 서버 보기");
	for(int i=0;i<g_iWorldCount;i++)
	{	sprintf(s_name,"%d. %s",i+1,sList[i].strName);
		m_ctlServerList.AddString(s_name);
	}
	m_ctlServerList.SetCurSel(0);

	//	파일을 열어서 데이타를 읽어온다.
	memset(iDL,0,sizeof(int)*dMAX_WORLD_SERVER_COUNT*200);
	char ScorePath[1024],fn[1080];
	sprintf(ScorePath,"%s\\Score",strCurPath);
	for(i=0;i<g_iWorldCount-1;i++)
	{
		if( strcmp(sList[i].strName,"")==0)	continue;
		sprintf(fn,"%s/%s/%08d_LevelScore.txt",ScorePath,sList[i].strName,time);
		char txt[2000];	memset(txt,0,2000);
		FILE *lScore = fopen(fn,"r");
		if(lScore)
		{
			fread(txt,2000,1,lScore);
			{
				int count = strlen(txt)-2;
				char tmp[10];

				for(int j=0;j<dMAX_LEVEL/5;j++)
				{
					memset(tmp,0,10);
					int tmpCount = 0;
					while(1)
					{
						if(txt[count]!='|')
						{
							if(txt[count]=='*')	txt[count] = 0;	//	너무 많을 경우 자리수가 모자란다.
							tmp[tmpCount]=txt[count];
							tmpCount++;
							--count;
							if(count<0)	break;
						}else{
							--count;
							break;
						}
					}
					if(tmp=="")
					{
						printf("-0- BREAK!!");
					}
					if(count<=0)	break;
					iDL[i][j] = atoi(tmp);
				}
			}
			fclose(lScore);
		}
	}
	m_ctlAgeList.EnableWindow();
	m_ctlSelectList.EnableWindow();
	m_ctlServerList.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAGEGRAPH::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	//	화면 세팅 
	CRect	m_DlgBox;
	GetClientRect(m_DlgBox);

	dc.SetMapMode(MM_ISOTROPIC);
	dc.SetWindowExt(5000,5000);
	dc.SetViewportExt(5000,-5000);
	dc.SetViewportOrg(m_DlgBox.left,m_DlgBox.bottom);

	CFont	Font;	CFont	* OldFont;
	Font.CreateFont(16,8,0,0,0,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"굴림체");
	OldFont = (CFont *)dc.SelectObject(&Font);

	CBrush	BrushBlack;		BrushBlack.CreateSolidBrush(BLACK);
	CBrush	BrushWhite;		BrushWhite.CreateSolidBrush(WHITE);
	CBrush	BrushRed;		BrushRed.CreateSolidBrush(RED);

	CPen	PenBlack;		PenBlack.CreatePen(PS_SOLID,1,BLACK);
	CPen	PenWhite;		PenWhite.CreatePen(PS_DOT,1,WHITE);
	CPen	PenGray;		PenGray.CreatePen(PS_DOT,1,LIGHTGRAY);
	CPen	Pen[8];	CPen	Pen2[8];
	Pen[0].CreatePen(PS_SOLID,1,LIGHTGRAY);
	Pen[1].CreatePen(PS_SOLID,1,YELLOW);
	Pen[2].CreatePen(PS_SOLID,1,BLUE);
	Pen[3].CreatePen(PS_SOLID,1,RED);
	Pen[4].CreatePen(PS_SOLID,1,BROWN);
	Pen[5].CreatePen(PS_SOLID,1,GREEN);
	Pen[6].CreatePen(PS_SOLID,1,MAGENTA);
	Pen[7].CreatePen(PS_SOLID,1,LIGHTCYAN);

	Pen2[0].CreatePen(PS_SOLID,2,LIGHTGRAY);
	Pen2[1].CreatePen(PS_SOLID,2,YELLOW);
	Pen2[2].CreatePen(PS_SOLID,2,BLUE);
	Pen2[3].CreatePen(PS_SOLID,2,RED);
	Pen2[4].CreatePen(PS_SOLID,2,BROWN);
	Pen2[5].CreatePen(PS_SOLID,2,GREEN);
	Pen2[6].CreatePen(PS_SOLID,2,MAGENTA);
	Pen2[7].CreatePen(PS_SOLID,2,LIGHTCYAN);

	//	그래프 틀을 만든다.
//	dc.Rectangle(60,110,685,385);
	dc.SelectObject(&PenWhite);
	int cnt = 0;
	char txt[100];

	switch(iSetType)
	{
	case 2:	//	동접자 통계 보기
		{
			CRect	rect2(40,110,904,390);
			dc.FillRect(&rect2,&BrushBlack);
			{	//	가로 그리기//	48 = 1440 / 30; 904/48 = 18
				for(int cnt = 1;cnt<48;cnt++)
				{
					dc.SetBkColor(BLACK);
					dc.MoveTo(40+18*cnt,110);
					dc.LineTo(40+18*cnt,390);
					if( !(cnt%2))
					{	dc.SetBkColor(RGB(215,215,215));
						sprintf(txt,"%02d",cnt/2);
						dc.TextOut(40+18*cnt-9,100,txt,strlen(txt));
					}
				}
				dc.SetBkColor(RGB(215,215,215));
				sprintf(txt,"[Hour(s)]");
				dc.TextOut(40+18*cnt-60,85,txt,strlen(txt));
			}
			{	//	세로 그리기//500명 단위, 8번 그리자. 280 / 8 = 35
				for(int cnt = 1;cnt<9;cnt++)
				{
					dc.SetBkColor(BLACK);
					dc.MoveTo(40,110+35*cnt);
					dc.LineTo(904,110+35*cnt);
					dc.SetBkColor(RGB(215,215,215));
					sprintf(txt,"%03d",cnt*500);
					dc.TextOut(10,110+35*cnt-5,txt,strlen(txt));
				}
				dc.SetBkColor(RGB(215,215,215));
				sprintf(txt,"[Person(s)]");
				dc.TextOut(10,110+35*cnt,txt,strlen(txt));
			}
			{	//	유저접속 통계를 그린다. 5분단위로.. 864픽셀 / ( 288 = 1440/5) = 3pixel
				for(int cnt=0;cnt<g_iWorldCount;cnt++)
				{
					cADO_ADMIN::stUserStatsPack _firstpack = m_pVector[cnt].at(0);
					if(iServer==0 || iServer==cnt+1)
					{
						dc.SelectObject(&Pen[cnt+2]);
						//	start 지점은 무조건..
						if(m_pVector[cnt].size()==0)	continue;
						int y = ( _firstpack.iAllUserCount/14 ); // 35pixel = 500명
						y = y + (y/250)*5 +110;
						if(y>390) y = 390;
						dc.MoveTo(40,y);
						/* 이부분 문제네. 기존은 int라 상관없었는데..*/
//						vector<int>::iterator pmaxvalue = max_element(m_pVector[cnt].begin(),m_pVector[cnt].end());
//						int maxcount = *pmaxvalue;
						int maxcount = 0;
						for(int c = 1;c<1440;c++)
						{
							cADO_ADMIN::stUserStatsPack _pack = m_pVector[cnt].at(c);
							if(maxcount == _pack.iAllUserCount)
								dc.FillRect( CRect( 40+(c/5)*3-1,y -6 ,40+(c/5)*3+1,y+6),&BrushRed);
							if(iServer==0 && m_allMaxIndex == c)
							{	//최고 점을 기록한다.
								dc.FillRect( CRect( 40+(c/5)*3-1,110,40+(c/5)*3+1,390),&BrushRed);
							}
							if(c%5)
								continue;
							int check = _pack.iAllUserCount;
							if(check <10)
							{
								cADO_ADMIN::stUserStatsPack _nextpack = m_pVector[cnt].at(c+1);
								cADO_ADMIN::stUserStatsPack _prevpack = m_pVector[cnt].at(c-1);
								check = _nextpack.iAllUserCount;
								if(check <10)
									check = _prevpack.iAllUserCount;
							}
							y = 110 + (check /14 );
							if(y>390) y = 390;
							dc.LineTo(40+(c/5)*3,y);
/*							if(c%5==0)
							{
								dc.SetBkColor(RGB(215,215,215));
								sprintf(txt,"%d",iDL[cnt][c-1]);
								dc.TextOut(60+c*10,y+23,txt,strlen(txt));
							}
*/
						}
					}
				}
			}//	유저접속 통계를 그린다.
		}
		break;
	case 0:
		{
			CRect	rect(60,110,685,385);
			dc.FillRect(&rect,&BrushBlack);

			for(cnt = 1;cnt<25;cnt++)
			{
				dc.SetBkColor(BLACK);
				dc.MoveTo(60+25*cnt,110);
				dc.LineTo(60+25*cnt,385);
				dc.SetBkColor(RGB(215,215,215));
				sprintf(txt,"%02d",cnt-1);
				dc.TextOut(60+25*cnt-9,100,txt,strlen(txt));
			}
			sprintf(txt,"(시)");
			dc.TextOut(60+25*25-9,100,txt,strlen(txt));

			for(cnt = 1;cnt<11;cnt++)
			{
				dc.SetBkColor(BLACK);
				dc.MoveTo(60,110+cnt*25);
				dc.LineTo(685,110+cnt*25);
				dc.SetBkColor(RGB(215,215,215));
				sprintf(txt,"%02d",1000*cnt);
				dc.TextOut(10,110+cnt*25+13,txt,strlen(txt));
			}
			sprintf(txt,"(번)");
			dc.TextOut(10,110+11*25+8,txt,strlen(txt));
			sprintf(txt,"(×160분)");
			dc.TextOut(10,110+12*25,txt,strlen(txt));

			for(cnt = 8;cnt>0;cnt--)
			{
				dc.SelectObject(&Pen[8-cnt]);
				dc.MoveTo(690,350-30*cnt);
				dc.LineTo(710,350-30*cnt);
			}

			sprintf(txt,"13세 이하");	dc.TextOut(715,350-30*8+10,txt,strlen(txt));
			sprintf(txt,"14 ~ 16세");	dc.TextOut(715,350-30*7+10,txt,strlen(txt));
			sprintf(txt,"17 ~ 19세");	dc.TextOut(715,350-30*6+10,txt,strlen(txt));
			sprintf(txt,"20 ~ 22세");	dc.TextOut(715,350-30*5+10,txt,strlen(txt));
			sprintf(txt,"23 ~ 25세");	dc.TextOut(715,350-30*4+10,txt,strlen(txt));
			sprintf(txt,"26 ~ 29세");	dc.TextOut(715,350-30*3+10,txt,strlen(txt));
			sprintf(txt,"30 ~ 35세");	dc.TextOut(715,350-30*2+10,txt,strlen(txt));
			sprintf(txt,"36세 이상");	dc.TextOut(715,350-30*1+10,txt,strlen(txt));

			dc.SelectObject(&Pen[3]);	dc.MoveTo(690,370);	dc.LineTo(710,370);
			sprintf(txt,"접속 횟수별");	dc.TextOut(715,380,txt,strlen(txt));

			dc.SelectObject(&Pen2[3]);		dc.MoveTo(690,400);	dc.LineTo(710,400);
			sprintf(txt,"플레이 시간별");	dc.TextOut(715,410,txt,strlen(txt));
	
			//	그래프를 그리자.. 
			for(cnt=0;cnt<8;cnt++)
			{
				if((iSelectGraph==0 || iSelectGraph==1) && (iSetAge == -1 || cnt == iSetAge))
				{
					dc.SelectObject(&Pen[cnt]);
					dc.MoveTo(60+25,110+(iConnectGraph[cnt][0]/40));
					for(int c = 2;c<24;c++)	dc.LineTo(60+25*c,110+(iConnectGraph[cnt][c]/40));
				}
				if((iSelectGraph==0 || iSelectGraph==2) && (iSetAge == -1 || cnt == iSetAge))
				{
					dc.SelectObject(&Pen2[cnt]);
					int value = iPlayGraph[cnt][0];
					if(value>40000)	value = 43600;
					dc.MoveTo(60+25,110+value/160);
					for(int c = 2;c<24;c++){
						int value = iPlayGraph[cnt][c];
						if(value>40000)	value = 43600;
						dc.LineTo(60+25*c,110+value/160);
					}
				}
			}
			break;
		}
	case 1:
		{
			CRect	rect(60,110,685,385);
			dc.FillRect(&rect,&BrushBlack);

//#define dSTART_LEVEL	1
//#define dMAX_LEVEL		1000
//#define dMAX_PERSONAL	500
			// 가로.. 레벨표기
			int iTextTerm = 2;	//	레벨 텍스트를 표기하는 간격.
			int iVertLineTerm = 625  / (dMAX_LEVEL/25);//세로줄을 그리는 간격.
			for(cnt = 1;cnt<(dMAX_LEVEL/25);cnt++)
			{
				dc.SetBkColor(BLACK);
				dc.MoveTo(60+iVertLineTerm*cnt,110);
				if(cnt%iTextTerm)
					dc.SelectObject(&PenWhite);
				else
					dc.SelectObject(&PenGray);
				dc.LineTo(60+iVertLineTerm*cnt,385);
				dc.SetBkColor(RGB(215,215,215));
				
				if(cnt%iTextTerm)
				{	sprintf(txt,"%02d",cnt*25);
					dc.TextOut(60+iVertLineTerm*cnt-9,100,txt,strlen(txt));
				}
			}
			sprintf(txt,"(Lev)");
			dc.TextOut(685,100,txt,strlen(txt));

			// 세로.. 인원표기
			int iHoriLineTerm = (int)(275/((float)dMAX_PERSONAL/(float)50));//세로줄을 그리는 간격.iHoriLineTerm = dMAX_PERSONAL/50
			for(cnt = 1;cnt<(dMAX_PERSONAL/50);cnt++)
			{
				dc.SetBkColor(BLACK);
				dc.MoveTo(60,110+cnt*iHoriLineTerm);
				if(cnt%iTextTerm)
					dc.SelectObject(&PenWhite);
				else
					dc.SelectObject(&PenGray);
				dc.LineTo(685,110+cnt*iHoriLineTerm);
				dc.SetBkColor(RGB(215,215,215));
				
//				if(cnt%iTextTerm)
				{	sprintf(txt,"%02d",50*cnt);
					dc.TextOut(10,110+cnt*iHoriLineTerm+13,txt,strlen(txt));
				}
			}
			sprintf(txt,"(개)");
			dc.TextOut(10,110+11*iHoriLineTerm+13,txt,strlen(txt));

			for(cnt = g_iWorldCount;cnt>=0;cnt--)
			{
				dc.SelectObject(&Pen[cnt+2]);
				dc.MoveTo(690,300-30*cnt);
				dc.LineTo(710,300-30*cnt);
				sprintf(txt,sList[cnt].strName);	dc.TextOut(715,300-30*cnt+10,txt,strlen(txt));
			}

			//	얻어온 데이타를 그린다.
			for(cnt=g_iWorldCount;cnt>=0;cnt--)
			{
				char txt[10];
				if(iServer==0 || iServer==cnt+1)
				{
					dc.SelectObject(&Pen[cnt+2]);
					//	625,275(w,h)
					//	60,110(start position)
					//	iHoriLineTerm 는  dMAX_PERSONAL/50를 의미

					int y = iDL[cnt][0];
					if(y>dMAX_PERSONAL)	y = dMAX_PERSONAL;
					y  = (int)(110+( (((float)y/(float)50)) * iHoriLineTerm ));
					if(y>400)	y = 400;
					dc.MoveTo(70+10,y);
					dc.SetBkColor(RGB(215,215,215));
					sprintf(txt,"%d",iDL[cnt][0]);	dc.TextOut(50,y+23,txt,strlen(txt));

					int iGCount = 0;
					int iDrawTextPos = 0;
					for(int c = 1;c<dMAX_LEVEL/5;c++)
					{
						++iGCount;
						y = iDL[cnt][(c-1)];
						if(y>dMAX_PERSONAL)	y = dMAX_PERSONAL;	
						y  = (int)(110+( (((float)y/(float)50)) * iHoriLineTerm ));
						if(y>400)	y = 400;
						dc.LineTo(70+(iVertLineTerm / 5)*iGCount,y);//625  / 
						if(iGCount%10==0)
						{
							++iDrawTextPos;
							sprintf(txt,"%d",iDL[cnt][(c-1)]);
							if(iDrawTextPos%2)
							{
								dc.SetBkColor(RGB(215,215,215));
								dc.FillRect( CRect( 70+(iVertLineTerm / 5)*iGCount-1,y-5,70+(iVertLineTerm / 5)*iGCount+1,y+5),&BrushRed);
								dc.TextOut(70+(iVertLineTerm / 5)*iGCount-15,y+5,txt,strlen(txt));
							}
							else
							{
								dc.SetBkColor(RGB(180,180,180));
								dc.FillRect( CRect( 70+(iVertLineTerm / 5)*iGCount-1,y-5,70+(iVertLineTerm / 5)*iGCount+1,y+5),&BrushRed);
								dc.TextOut(70+(iVertLineTerm / 5)*iGCount-15,y-3,txt,strlen(txt));
							}

						}
					}
				}
			}
			break;
		}
	}

	BrushWhite.DeleteObject();
	BrushBlack.DeleteObject();
	for(cnt=0;cnt<8;cnt++)	Pen[cnt].DeleteObject();
	PenWhite.DeleteObject();
	PenBlack.DeleteObject();
	dc.SelectObject(OldFont);
	Font.DeleteObject();
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CAGEGRAPH::OnSelchangeAgeList() 
{
	// TODO: Add your control notification handler code here
	iSetAge = m_ctlAgeList.GetCurSel()-1;
	Invalidate(FALSE);
}

void CAGEGRAPH::OnSelchangeSelectList() 
{
	// TODO: Add your control notification handler code here
	iSelectGraph = m_ctlSelectList.GetCurSel();
	Invalidate(FALSE);
}

void CAGEGRAPH::OnSelchangeType() 
{
	// TODO: Add your control notification handler code here
	iSetType = m_ctlTypeList.GetCurSel();
	switch(iSetType)
	{
	case 0:
		{
			m_ctlAgeList.EnableWindow();
			m_ctlSelectList.EnableWindow();
			m_ctlServerList.EnableWindow(FALSE);
			break;
		}
	case 1:
		{
			m_ctlAgeList.EnableWindow(FALSE);
			m_ctlSelectList.EnableWindow(FALSE);
			m_ctlServerList.EnableWindow();
			break;
		}
	case 2:
		{
			m_ctlAgeList.EnableWindow(FALSE);
			m_ctlSelectList.EnableWindow(FALSE);
			m_ctlServerList.EnableWindow();
			break;
		}
	}
	Invalidate(TRUE);
}

void CAGEGRAPH::OnSelchangeServerList() 
{
	// TODO: Add your control notification handler code here
	iServer = m_ctlServerList.GetCurSel();
	Invalidate(TRUE);
}
