// PAGE_SUM.cpp : implementation file
//

#include "stdafx.h"
#include "AdminTool.h"
#include "PAGE_SUM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPAGE_SUM dialog
IMPLEMENT_DYNCREATE(CPAGE_SUM, CPropertyPage)

CPAGE_SUM::CPAGE_SUM() : CPropertyPage(CPAGE_SUM::IDD)
{
	//{{AFX_DATA_INIT(CPAGE_SUM)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPAGE_SUM::~CPAGE_SUM()
{
}

void CPAGE_SUM::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPAGE_SUM)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPAGE_SUM, CPropertyPage)
	//{{AFX_MSG_MAP(CPAGE_SUM)
	ON_BN_CLICKED(IDC_FOR_LEVEL, OnForLevel)
	ON_BN_CLICKED(IDC_PEIN, OnPein)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPAGE_SUM message handlers

void CPAGE_SUM::OnForLevel() 
{
	// TODO: 전체 캐릭터 레벨 분포표, 직업별 캐릭터 레벨 분포표
}

class	cPeinData
{
public:
	WORD		wRank;
	char		strId[dID_LENGTH];
	DWORD		dwPlayTime;
	WORD		wServerCount;
	WORD		wLevel;
	char		strServerName[32];
	char		strName[dNAME_LENGTH];

	cPeinData()
	{
		wRank = 0xffff;
		strcpy(strId,"");
		dwPlayTime  =0;
		wServerCount = 0;
		wLevel = 0;
		strcpy(strServerName,"");
		strcpy(strName,"");
	}
};

#define dMAX_PEIN			50000
static cPeinData			PeinData[dMAX_PEIN];
static WORD					g_wCount = 0;
char						g_strServerName[32];

void CPAGE_SUM::OnPein() 
{
	//	각 서버로 연결하면서 ID별로 사용시간 합할것
/*	strcpy(g_strServerName,"가디우스");
	if(!DB.Connect("211.189.122.103"	,strAvatarDBCId,strAvatarDBCPw,"Redstone_Avatar_Source",1))
	{
		MessageBox("유저디비 접속 불가능","ERROR");
		return;
	}
	GetSumPein();
	DB.Close();
	strcpy(g_strServerName,"제노스");
	if(!DB.Connect("211.189.122.106"	,strAvatarDBCId,strAvatarDBCPw,"Redstone_Avatar_Source",1))
	{
		MessageBox("유저디비 접속 불가능","ERROR");
		return;
	}
	GetSumPein();
	DB.Close();

	strcpy(g_strServerName,"에립트");
	if(!DB.Connect("211.189.122.117"	,strAvatarDBCId,strAvatarDBCPw,"Redstone_Avatar_Source",1))
	{
		MessageBox("유저디비 접속 불가능","ERROR");
		return;
	}
	GetSumPein();
	DB.Close();
	
	strcpy(g_strServerName,"프란델");
	if(!DB.Connect("211.189.122.91"	,strAvatarDBCId,strAvatarDBCPw,"Redstone_Avatar_Source",1))
	{
		MessageBox("유저디비 접속 불가능","ERROR");
		return;
	}
	GetSumPein();
	DB.Close();

	FILE * fp = NULL;
	fp=  fopen("d:\\PeinList.txt","w+");
	if(!fp)	return ;

	for(int i = 0 ;i<g_wCount ;i++)
	{
		fprintf(fp,"%s|%s|%s|%d|%d|%d \n",PeinData[i].strId,PeinData[i].strName,PeinData[i].strServerName,PeinData[i].wLevel,PeinData[i].dwPlayTime,PeinData[i].wServerCount);
	}
	fclose(fp);
*/
/*	MessageBox("211.189.122.103","start");
	if(!DB.Connect("211.189.122.103",strAvatarDBCId,strAvatarDBCPw,"Redstone_Avatar_Source",1))
	{
		MessageBox("유저디비 접속 불가능","ERROR");
		return;
	}
	SetAvaList();
	SetAvaSave();

	DB.Close();
*/
}

void	CPAGE_SUM::SetAvaSave()
{
	BOOL				bRet	=	TRUE;

	char curPath[512],savepath[512];
	GetCurrentDirectory(512,curPath);
	sprintf(savepath,"d:\\data\\");
	SetCurrentDirectory(savepath);

	HANDLE				hFind;
	WIN32_FIND_DATA		fd;

	hFind=FindFirstFile("*.*",&fd);
	while(hFind != INVALID_HANDLE_VALUE && bRet)
	{
		if ((fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0)
		{
			if(strstr(fd.cFileName,".dat")!=NULL)
			{
				FILE * fp = NULL;
				fp = fopen(fd.cFileName,"rb");
				if(fp)
				{
					cPLAYER_DATA		pData;	memset(&pData,0,sizeof(cPLAYER_DATA));
					fread(&pData,sizeof(cPLAYER_DATA),1,fp);
					fclose(fp);

//					DB.spSaveAva(0,&pData);
				}else{
					_log("FILE READ ERROR - %s",fd.cFileName);
				}
			}
		}
		bRet=FindNextFile(hFind,&fd);
	}
	FindClose(hFind);
	SetCurrentDirectory(curPath);
}

void	CPAGE_SUM::SetAvaList()
{
	_RecordsetPtr  pRs = NULL;
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int				i_RecCount;
	int				i,iCount=0;
	char			*pBuf = NULL;
	HRESULT			hr = CoInitialize(0);

	sprintf(str_Query,"SELECT data FROM Avatar_Current WHERE iLastField=65535");
	pRs.CreateInstance(__uuidof(Recordset)); 
	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)DB.GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	if(!FAILED(hr))
	{
		for(i = 0 ;i < i_RecCount ;i++)
		{
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			cPLAYER_DATA	pData;
			DB.VariantToBlob(v_bufferdata,&pData);
			pData.m_dwSaveCount++;
			char	fn[512];
			sprintf(fn,"d:/data/%s.dat",pData.m_strName);
			FILE * fp = fopen(fn,"wb");
			if(fp)
			{
				fwrite(&pData,sizeof(cPLAYER_DATA),1,fp);
				fclose(fp);
			}
//			DB.spSaveAva(&pData);
			pRs->MoveNext();
		}
	}
	pRs->Close();
	return;
}

void	CPAGE_SUM::GetSumPein()
{
	_RecordsetPtr  pRs = NULL;
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int				i_RecCount;
	int				i,iCount=0;
	char			*pBuf = NULL;
	HRESULT			hr = CoInitialize(0);

	sprintf(str_Query,"SELECT master,name,lev,eventplaytime FROM Avatar_Current WHERE (playtime > 1000) AND (lastdate > 20031228) AND (lev > 59)");
	pRs.CreateInstance(__uuidof(Recordset)); 
	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)DB.GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	if(!FAILED(hr))
	{
		char	strid[dID_LENGTH];
		char	strName[dNAME_LENGTH];
		DWORD	dwPlayTime = 0;
		WORD	wLevel = 0;
		for(i = 0 ;i < i_RecCount ;i++)
		{
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			strcpy(strid,(_bstr_t)v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			strcpy(strName,(_bstr_t)v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)2)->GetValue();
			wLevel = (WORD)v_bufferdata.iVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)3)->GetValue();
			dwPlayTime= (DWORD)v_bufferdata.iVal;

			//	같은 이름이 있나?
			int i = 0;
			for(i= 0;i<g_wCount;i++ )
			{
				if(stricmp(PeinData[i].strId,strid)==0)		//	빈자리
				{
					PeinData[i].dwPlayTime += dwPlayTime;
					PeinData[i].wServerCount ++;
					if(PeinData[i].wLevel<wLevel)
					{
						strcpy(PeinData[i].strServerName,g_strServerName);
						PeinData[i].wLevel = wLevel;
						strcpy(PeinData[i].strName,strName);
					}
					break;
				}
			}
			if(i<g_wCount)		goto GOTO_LOOP;
			//	빈 자리가 있나?
			for(i= 0;i<dMAX_PEIN;i++ )
			{
				if(PeinData[i].wRank == 0xffff)		//	빈자리
				{
					PeinData[i].dwPlayTime = dwPlayTime;
					strcpy(PeinData[i].strId,strid);
					PeinData[i].wRank = 0;
					g_wCount++;
					PeinData[i].wServerCount = 1;
					strcpy(PeinData[i].strServerName,g_strServerName);
					PeinData[i].wLevel = wLevel;
					strcpy(PeinData[i].strName,strName);
					break;
				}
			}
			if(i==dMAX_PEIN)
				MessageBox("It's excess Max User Count.","ERROR",MB_OK);
GOTO_LOOP:
			pRs->MoveNext();
		}
	}
	pRs->Close();
	return;
}