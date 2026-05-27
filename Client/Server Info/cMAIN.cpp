#include <stdio.h>
#include "stdafx.h"

#include "cMAIN.H"
#include "cMESSAGE.H"

static char	*lpszAppName			=	"SERVERINFO";
static char	*lpszTitle				=	"서버 정보 입력";


cWND				*cMAIN::pWND			=	NULL;
cDIBWND				*cMAIN::pDIBWND			=	NULL;
HWND				cMAIN::hWND				=	NULL;
cLISTVIEW			cMAIN::s_lvMap;

HINSTANCE			cMAIN::hINST;

cTREEVIEW			cMAIN::s_treeMap;
int					cMAIN::s_iSelectMap;
int					cMAIN::s_iServerCount	=	0;
int					cMAIN::s_iSelectServer	=	0xffff;
cSERVER_INFO		cMAIN::s_aServerList[dMAX_SERVER_COUNT];
HTREEITEM			cMAIN::s_aTree[dMAX_SERVER_COUNT];
cSTRINGS			*cMAIN::s_pFiles		=	NULL;


BOOL				bRUNAPPLICATION	=	TRUE,bACTIVEAPP	=	FALSE;
char				l_strCurrentFileName[512]	=	"server_info.gsd";
char				l_strConfigFile[1024];

BOOL
cMAIN::Init(HINSTANCE hInst)
{
	sprintf(l_strConfigFile,"%s\\serverInfo.dat",_logFolder);

	if	(IsFile("setup.exe")==FALSE)
	{
		CRegKey		reg;

		if	(reg.Open( HKEY_LOCAL_MACHINE, "software\\L&K Logic Korea\\Red Stone") == ERROR_SUCCESS)
		{	
			DWORD	count	=	sizeof(_logFolder);

			if	(reg.QueryValue((LPTSTR )_logFolder,"Path",&count) == ERROR_SUCCESS)
			{	
				if	(count	<=	0)
					return cMSG::Error("Error in cMAIN::Init","can not find registry data of Red Stone");
			}
			else	
				return cMSG::Error("Error in cMAIN::Init","can not find registry data of Red Stone");
		}
		else
			return cMSG::Error("Error in cMAIN::Init","can not find registry data of Red Stone");
	}

	hINST	=	hInst;

	cFILE	file;

	if	(file.Open(l_strConfigFile,"rb"))
	{
		file.Read(l_strCurrentFileName,sizeof(l_strCurrentFileName));
		file.Close();
	}

	ReadData(l_strCurrentFileName);

	DialogBox(hInst,"MAIN",NULL,(DLGPROC)DlgProc);

	return FALSE;
}

//
//	창에 데이터를 채워준다.
void
cMAIN::FillData(HWND _hWnd)
{
	char	*strSelected	=	s_treeMap.Select();

	if (strcmp(strSelected,"")	==	0)
		return;

	cSERVER_INFO	*lpServer;
	cMAP_INFO		*lpMap	=	NULL;

	lpServer		=	FindServer(strSelected);

	if (lpServer	==	NULL)
	{
		lpMap			=	FindMap(strSelected);

		if	(!lpMap)
			return;

		lpServer		=	&s_aServerList[lpMap->m_iServerSerial];
	}

	if (lpServer)
		SetDlgItemText(_hWnd,IDC_SERVER_NAME,lpServer->m_strName);

	if (!lpMap)
		return;

//	SetDlgItemText(_hWnd,IDC_MAP_FILE,lpMap->m_strName);
}

//
//	비어있는 맵 시리얼 검색
int
cMAIN::FindValidMapSerial()
{
	int	i,j;
	int	iValidSerial=-1,iIndex;

	for (iIndex	=0;iValidSerial==-1;iIndex++)
	{
		iValidSerial	=	iIndex;

		for (i=0;i<s_iServerCount;i++)
		{
			for (j=0;j<s_aServerList[i].m_iCount;j++)
			{
				if (s_aServerList[i].m_aMap[j].m_iSerial	==	iValidSerial)
				{
					iValidSerial	=	-1;
					break;
				}
			}

			if (iValidSerial	==	-1)	break;
		}
	}

	return	iValidSerial;
}

//
//	서버 찾기
cSERVER_INFO*
cMAIN::FindServer(char *_strName,int _iExceptServer)
{
	for (int i=0;i<s_iServerCount;i++)
		if (stricmp(s_aServerList[i].m_strName,_strName)==0)
			if (i	!=_iExceptServer)
				return	&s_aServerList[i];

	return	NULL;
}

//
//	맵 찾기
cMAP_INFO*
cMAIN::FindMap(char *_strName,int _iExceptServer,int _iExceptMap)
{
	for (int i=0;i<s_iServerCount;i++)
	{
		cSERVER_INFO	*lpServer	=	&s_aServerList[i];

		for (int j=0;j<lpServer->m_iCount;j++)
		{
			if (stricmp(lpServer->m_aMap[j].m_strName,_strName)==0)	
			{
				if (i== _iExceptServer && _iExceptMap == j)	continue;

				s_iSelectMap	=	j;

				return	&lpServer->m_aMap[j];
			}
		}
	}

	return	NULL;
}

//
//	맵 찾기
cMAP_INFO*
cMAIN::FindMap(int _iSerial,int _iExceptServer,int _iExceptMap)
{
	for (int i=0;i<s_iServerCount;i++)
	{
		cSERVER_INFO	*lpServer	=	&s_aServerList[i];

		for (int j=0;j<lpServer->m_iCount;j++)
			if (lpServer->m_aMap[j].m_iSerial==_iSerial)	
			{
				if (i== _iExceptServer && _iExceptMap == j)	continue;

				s_iSelectMap	=	j;

				return	&lpServer->m_aMap[j];
			}
	}

	return	NULL;
}

//
//	맵 트리 맹글기
void
cMAIN::MakeMapTree(HWND _hWnd)
{
	HWND	hTree	=	GetDlgItem(_hWnd,IDC_TREE);

	int		i,j;

	s_treeMap.Init(hTree,hINST,NULL);
//	s_treeMap.SetStyle(WS_CHILD | WS_BORDER | WS_VISIBLE | TVS_SINGLEEXPAND | TVS_TRACKSELECT | TVS_NOSCROLL | TVS_NONEVENHEIGHT |TVS_DISABLEDRAGDROP | TVS_HASLINES);

	for (i=0;i<s_iServerCount;i++)
	{
		cSERVER_INFO	*lpServer	=	&s_aServerList[i];

		HTREEITEM		hTreeRoot	=	s_treeMap.Add(lpServer->m_strName,0);
		s_aTree[i]					=	hTreeRoot;

		for (j=0;j<lpServer->m_iCount;j++)
		{
			s_treeMap.Add(hTreeRoot,lpServer->m_aMap[j].m_strName,0);	//	계열 추가..
		}
	}

	s_treeMap.Select(s_aTree[0]);
}

//
//	맵 트리 맹글기
void
cMAIN::UpdateMapTree(HWND _hWnd)
{
	HWND	hTree	=	GetDlgItem(_hWnd,IDC_TREE);

	int		i,j;

	s_treeMap.Init(hTree,hINST,NULL);

	for (i=0;i<s_iServerCount;i++)
	{
		cSERVER_INFO	*lpServer	=	&s_aServerList[i];

		HTREEITEM		hTreeRoot	=	s_treeMap.Add(lpServer->m_strName,0);

		for (j=0;j<lpServer->m_iCount;j++)
		{
			s_treeMap.Add(hTreeRoot,lpServer->m_aMap[j].m_strName,0);	//	계열 추가..
		}
	}
}

BOOL
cMAIN::InitDlg(HWND _hWnd)
{
	MakeMapTree(_hWnd);

	FillData(_hWnd);

	FillMapList(_hWnd);

	return	TRUE;
}

BOOL
cMAIN::ReadData(char *_lpstrFn)
{
	cFILE	file;
	int		i;

	SetCurrentDirectory(_logFolder);

	if (!file.Open(_lpstrFn,"rb"))	return	FALSE;

	file.Read(&s_iServerCount,4);

	for (i=0;i<s_iServerCount;i++)
	{
		cSERVER_INFO	*lpServerInfo	=	&s_aServerList[i];

		file.Read(lpServerInfo,sizeof(cSERVER_INFO)-sizeof(lpServerInfo->m_aMap));

		if (lpServerInfo->m_iCount	>	0)
			file.Read(lpServerInfo->m_aMap,sizeof(cMAP_INFO)*lpServerInfo->m_iCount);
	}

	file.Close();

	return TRUE;
}

BOOL
cMAIN::SortServerMapList(HWND hwnd)
{
	int	i,j;

	for (i=0;i<s_iServerCount;i++)
	{
		cSERVER_INFO	*lpServerInfo	=	&s_aServerList[i];
		cSTRINGS		strings;

		for (j=0;j<lpServerInfo->m_iCount;j++)
		{
			char	strName[64];

			strcpy(strName,lpServerInfo->m_aMap[j].m_strName);

			int		iSerial	=	atoi(lpServerInfo->m_aMap[j].m_strName+1);

			if	(iSerial	>=	1000)
				sprintf(strName,"%s",lpServerInfo->m_aMap[j].m_strName);
		
			strings.Add(strName);
		}

		strings.Sort();

		for (j=0;j<strings.Count;j++)
		{
			strcpy(lpServerInfo->m_aMap[j].m_strName,strings.GetStr(j));

			lpServerInfo->m_aMap[j].m_iSerial	=	atoi(lpServerInfo->m_aMap[j].m_strName+1);
		}
	}

	s_treeMap.Clear();

	MakeMapTree(hwnd);

	FillData(hwnd);

	return	TRUE;
}

BOOL
cMAIN::SaveData(char *_strFn)
{
	cFILE	file;
	int		i;

	SetCurrentDirectory(_logFolder);

	file.Open(_strFn,"wb");

	file.Write(&s_iServerCount,4);

	for (i=0;i<s_iServerCount;i++)
	{
		cSERVER_INFO	*lpServerInfo	=	&s_aServerList[i];

		file.Write(lpServerInfo,sizeof(cSERVER_INFO)-sizeof(lpServerInfo->m_aMap));

		if (lpServerInfo->m_iCount	>	0)
			file.Write(&lpServerInfo->m_aMap[0],sizeof(cMAP_INFO)*lpServerInfo->m_iCount);
	}

	file.Close();

	return	TRUE;
}

//
//	종료
BOOL
cMAIN::Exit()
{
	if (!bRUNAPPLICATION)
		return FALSE;

	SaveData("server_info.gsd");
	
	bRUNAPPLICATION	=	FALSE;

	PostQuitMessage(0);

	delete s_pFiles;

	return TRUE;
}	//	cMAIN::Exit()

BOOL
cMAIN::Run()			//	윈도우가 활성화 중일때 계속 업데이트 시켜준다.
{
	if (!bRUNAPPLICATION)
		return FALSE;

	return TRUE;
}

/*****************************************************************************
								풀어주기
*****************************************************************************/

//
//	리사이즈
void
cMAIN::Resize()
{
}	//	cMAIN::Resize()

//
//	맵 변경
BOOL
cMAIN::UpdateSelectMap(HWND _hDlg)
{
	char	strName[100];
	int		i,iType;

	GetDlgItemText(_hDlg,IDC_MAP_FILE,strName,sizeof(strName));

	if (stricmp(strName,"")==0	)	return	cMSG::Error("error","please selet map");


	for (i=IDC_NORMAL;i<=IDC_RADIO12;i++)
	{
		if (IsDlgButtonChecked(_hDlg,i))
		{
			iType	=	i-IDC_NORMAL;
			break;
		}
	}

	GetSelectServer(_hDlg);

	HTREEITEM	hSelect			=	s_treeMap.GetSelect();
	char		*strMap			=	s_treeMap.Select();
	cMAP_INFO*	lpMapInfo		=	FindMap(strMap);

	if (!lpMapInfo)	return	FALSE;

	if (FindMap(strName,s_iSelectServer,s_iSelectMap))
		return	cMSG::Error("error","Already exist same map");

	strcpy(lpMapInfo->m_strName,strName);
	lpMapInfo->m_iServerSerial	=	s_iSelectServer;
	lpMapInfo->m_iType			=	iType;

	s_treeMap.SetText(hSelect,strName);

	return	TRUE;
}


//
//	지금 선택된 서버
int
cMAIN::GetSelectServer(HWND _hDlg)
{
	HTREEITEM	hSelect		=	s_treeMap.GetSelect();
	HTREEITEM	hTreeRoot	=	s_treeMap.GetRoot(hSelect);

	for (int i=0;i<s_iServerCount;i++)
	{
		if(s_aTree[i]	==	hTreeRoot)
		{
			s_iSelectServer	=	i;
			break;
		}
	}

	char *strMap	=	s_treeMap.Select();

	for (i=0;i<s_iServerCount;i++)
	{
		cSERVER_INFO	*lpServer	=	&s_aServerList[i];

		for (int j=0;j<lpServer->m_iCount;j++)
		{
			if (stricmp(lpServer->m_aMap[j].m_strName,strMap)==0)		s_iSelectMap	=	j;
		}
	}
	return	s_iSelectServer;
}	//	cMAIN::GetSelectServer(HWND _hDlg)

//
//	맵 추가
BOOL
cMAIN::AddMap(HWND _hDlg)
{
	char	strName[100];
	int		iSerial,i,iType,iSelectMapCount;

	int		iSelectMap		=	s_lvMap.GetSelectItem();
	int		*lpSelectMapList=	s_lvMap.GetSelectedItem(&iSelectMapCount);

	if	(iSelectMapCount	==	0)
		return	cMSG::Error("error","please select map");

	for (int iMap=0;iMap<iSelectMapCount;iMap++)
	{
		iSelectMap			=	lpSelectMapList[iMap];
		char	*lpstrName	=	s_lvMap.GetText(iSelectMap,1);

		strcpy(strName,lpstrName);

		if	(stricmp(strName,"")==0	)
			return	cMSG::Error("error","please select map");

		for (i=IDC_NORMAL;i<=IDC_RADIO12;i++)
		{
			if	(IsDlgButtonChecked(_hDlg,i))
			{
				iType	=	i-IDC_NORMAL;
				break;
			}
		}

		char	strNumber[32];

		strncpy(strNumber,strName+1,3);
		strNumber[3]	=	NULL;

		iSerial			=	atoi(strNumber);

		if	(FindMap(iSerial))
			return	cMSG::Error("error","Already exist unique serial");

		if	(FindMap(strName))
			return	cMSG::Error("error","Already exist same name map - %s",strName);

		GetSelectServer(_hDlg);

		HTREEITEM	hAddBranch	=	s_treeMap.Add(s_aTree[s_iSelectServer],strName,0);

		s_treeMap.Select(hAddBranch);

		cMAP_INFO*	lpMapInfo	=	&s_aServerList[s_iSelectServer].m_aMap[s_aServerList[s_iSelectServer].m_iCount];

		strcpy(lpMapInfo->m_strName,strName);
		lpMapInfo->m_iSerial		=	iSerial;
		lpMapInfo->m_iServerSerial	=	s_iSelectServer;
		lpMapInfo->m_iType			=	iType;
		s_aServerList[s_iSelectServer].m_iCount++;

		s_lvMap.SetText(iSelectMap,0,"O");
	}

	pKILL(lpSelectMapList);

	return	FALSE;
}	//	cMAIN::AddServer(HWND _hDlg)

//
//	서버 추가
BOOL
cMAIN::AddServer(HWND _hDlg)
{
	char	strName[100];

	GetDlgItemText(_hDlg,IDC_SERVER_NAME,strName,sizeof(strName));

	if	(stricmp(strName,"")==0		)
		return	cMSG::Error("error","please input server name");
	if	(FindServer(strName)		)
		return	cMSG::Error("error","already exist same name server");

	cSERVER_INFO	*lpServerInfo	=	&s_aServerList[s_iServerCount];

	lpServerInfo->m_iSerial	=	s_iServerCount;
	lpServerInfo->m_iCount	=	0;
	strcpy(lpServerInfo->m_strName,strName);

	s_aTree[s_iServerCount]		=	s_treeMap.Add(lpServerInfo->m_strName,0);
	s_treeMap.Select(s_aTree[s_iServerCount]);
	s_iServerCount++;

	FillData(_hDlg);

	return	FALSE;
}

//
//	선택한 서버 제거
BOOL
cMAIN::RemoveServer(HWND _hDlg)
{
	if (s_iServerCount	<=	0)	return FALSE;
	
	GetSelectServer(_hDlg);

	if (cMSG::YESNO("Remove Server","Do you want remove select server?\n\n%s",s_aServerList[s_iSelectServer].m_strName) == IDNO)	return	FALSE;

	GetSelectServer(_hDlg);

	for (int i=s_iSelectServer;i<s_iServerCount-1;i++)
	{
		cSERVER_INFO *lpFirst	=	&s_aServerList[i];
		cSERVER_INFO *lpSecond	=	&s_aServerList[i+1];

		memcpy(lpFirst,lpSecond,sizeof(cSERVER_INFO));

		lpFirst->m_iSerial	=	i;
		lpSecond->m_iSerial	=	0xffff;
	}

	s_iServerCount--;

	if (s_iServerCount	<=	0)
		s_treeMap.Clear();
	else
	{
		s_treeMap.Delete(s_aTree[s_iSelectServer]);
		s_treeMap.Select(s_aTree[0]);
	}

	s_iSelectServer	=	0;

	FillData(_hDlg);
	UpdateMapList();

	return	TRUE;
}

//
//	선택한 서버 제거
BOOL
cMAIN::RemoveMap(HWND _hDlg)
{
	char*		strMap	=	s_treeMap.Select();
	cMAP_INFO*	lpMap	=	FindMap(strMap);

	if (!lpMap)
		return	FALSE;

	if (cMSG::YESNO("Remove map","Do you want remove select map?\n\n%s",lpMap->m_strName) == IDNO)	return	FALSE;

	int	iIndex	=	s_pFiles->GetIndex(lpMap->m_strName);

	s_lvMap.SetText(iIndex,0,"X");

	HTREEITEM	hItem	=	s_treeMap.GetSelect();

	GetSelectServer(_hDlg);

	cSERVER_INFO	*lpServer	=	&s_aServerList[s_iSelectServer];

	for (int i=s_iSelectMap;i<lpServer->m_iCount-1;i++)
	{
		cMAP_INFO *lpFirst	=	&lpServer->m_aMap[i];
		cMAP_INFO *lpSecond	=	&lpServer->m_aMap[i+1];

		memcpy(lpFirst,lpSecond,sizeof(cMAP_INFO));
	}

	s_treeMap.Delete(hItem);
	lpServer->m_iCount--;

	s_treeMap.Select(s_aTree[s_iSelectServer]);

	FillData(_hDlg);

	return	TRUE;
}	//	cMAIN::RemoveServer(HWND _hDlg)

//
//	서버 추가
BOOL
cMAIN::UpdateSelectServer(HWND _hDlg)
{
	char	strName[100];

	GetDlgItemText(_hDlg,IDC_SERVER_NAME,strName,sizeof(strName));

	if (stricmp(strName,"")==0	)
		return	cMSG::Error("error","Please input server name.");
//	if (FindServer(strName))		return	cMSG::Error("error","같은 이름의 서버가 존재 합니다.");

	cSERVER_INFO	*lpServer;
	cMAP_INFO		*lpMap;

	char			*strSelected	=	s_treeMap.Select();

	lpServer		=	FindServer(strSelected);

	if (lpServer	==	NULL)
	{
		lpMap			=	FindMap(strSelected);
		if (!lpMap)	return FALSE;

		lpServer		=	&s_aServerList[lpMap->m_iServerSerial];
	}

	if (FindServer(lpServer->m_strName,lpServer->m_iSerial))
		return	cMSG::Error("error","Already exist same name server.");

	{
		HTREEITEM	hSelect		=	s_treeMap.GetSelect();
		HTREEITEM	hTreeRoot	=	s_treeMap.GetRoot(hSelect);

		for (int i=0;i<s_iServerCount;i++)
		{
			if(s_aTree[i]	==	hTreeRoot)
			{
				s_iSelectServer	=	i;
				break;
			}
		}
	}

	s_treeMap.SetText(s_aTree[s_iSelectServer],strName);

	strcpy(lpServer->m_strName,strName);

	return	FALSE;
}

void
cMAIN::ExportMapList()
{
	char	strFolder[512];

	GetCurrentDirectory(512,strFolder);
	SetCurrentDirectory(_logFolder);

	cFILE	file;

	for (int i=0;i<s_iServerCount;i++)
	{
		cSERVER_INFO	*lpServerInfo	=	&s_aServerList[i];

		file.Open(_ms("%s.txt",lpServerInfo->m_strName),"wb");

		for (int j=0;j<lpServerInfo->m_iCount;j++)
			fprintf(file.fp,"%s\n",lpServerInfo->m_aMap[j].m_strName);

		file.Close();
	}

	SetCurrentDirectory(strFolder);
}

//
//	맵 선택
BOOL
cMAIN::SelectMap(HWND _hWnd)
{
	char	strFname[1024];

	strcpy(strFname,"");

	if	(!cFILE::LoadBox(strFname,"Red stone Map Data File {*.rmd}\0*.rmd\0\0","rmd",_ms("%s/Data/Scenario/Red Stone/Map",_logFolder),"Select map"))
		return	FALSE;

	char	*lpFName	=	cSTRING::Minus(strFname,_ms("%s\\Data\\Scenario\\Red Stone\\Map\\",_logFolder));

	SetDlgItemText(_hWnd,IDC_MAP_FILE,lpFName);

	return	TRUE;
}

//
//	맵 선택
BOOL
cMAIN::Save(HWND _hWnd)
{
	char	strFname[1024];

	strcpy(strFname,l_strCurrentFileName);

	if	(!cFILE::SaveBox(strFname,"Game Server Data {*.gsd}\0*.gsd\0\0","gsd",_exportFolderName(l_strCurrentFileName),"Save server info"))
		return	FALSE;

	SaveData(strFname);

	cFILE	file;

	if	(file.Open(l_strConfigFile,"wb"))
	{
		file.Write(l_strCurrentFileName,sizeof(l_strCurrentFileName));
		file.Close();
	}

	return	TRUE;
}

//
//	맵 선택
BOOL
cMAIN::Load(HWND _hWnd)
{
	char	strFname[1024];

	strcpy(strFname,"");

	if	(!cFILE::LoadBox(strFname,"Game Server Data {*.gsd}\0*.gsd\0\0","gsd",_exportFolderName(l_strCurrentFileName),"Load server info"))
		return	FALSE;

	strcpy(l_strCurrentFileName,strFname);

	ReadData(strFname);

	s_treeMap.Clear();

	for (int i=0;i<s_iServerCount;i++)
	{
		cSERVER_INFO	*lpServer	=	&s_aServerList[i];

		HTREEITEM		hTreeRoot	=	s_treeMap.Add(lpServer->m_strName,0);
		s_aTree[i]					=	hTreeRoot;

		for (int j=0;j<lpServer->m_iCount;j++)
		{
			s_treeMap.Add(hTreeRoot,lpServer->m_aMap[j].m_strName,0);	//	계열 추가..
		}
	}

	s_treeMap.Select(s_aTree[0]);

	FillData(_hWnd);
	UpdateMapList();

	return	TRUE;
}	//	cMAIN::Load(HWND _hWnd)


void
cMAIN::FillMapList(HWND _hWnd)
{
	HWND	hList	=	GetDlgItem(_hWnd,IDC_MAP_LIST);

	s_lvMap.Init	(hList,hINST,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	s_lvMap.AddColumn(0,24 ,"In");
	s_lvMap.AddColumn(1,280,"Name");

	SetCurrentDirectory(_ms("%s/Data/Scenario/Red Stone/Map",_logFolder));

	s_pFiles	=	cFOLDER::GetFiles("*.rmd");

	if	(!s_pFiles)
		return;

//	링크된 녀석들 리스트

	for (int i=0;i<s_pFiles->Count;i++)
	{
		cSTRING	*lpFile	=	s_pFiles->Pick(i);

		if	(strlen(lpFile->String) < 5 || (lpFile->String[0] != '[' && lpFile->String[0] != '{'))
			continue;

		char	strNumber[6];

		memcpy(strNumber,lpFile->String+1,5);

		BOOL	bIsValidField	=	TRUE;

		for (int iSlot=0;iSlot<3;iSlot++)
		{
			if	(strNumber[iSlot]	<	'0' || strNumber[iSlot]	>	'9')
			{
				bIsValidField	=	FALSE;
				break;
			}
		}

		if	(strNumber[3]	>=	'0' && strNumber[3]	<=	'9')
		{
			if	(strNumber[4]	!=	'}')
				bIsValidField	=	FALSE;
		}

		if	(!bIsValidField)
			continue;

		if	(strNumber[3]	<	'0' || strNumber[3]	>	'9')
			strNumber[3]	=	NULL;
		else
			strNumber[4]	=	NULL;

		int	iIndex;

		if	(cMAIN::FindMap(lpFile->String))
			iIndex	=	s_lvMap.AddItem(0,"O");
		else
			iIndex	=	s_lvMap.AddItem(0,"X");

		if	(strNumber[3]	==	NULL)
			s_lvMap.SetItem(iIndex,1,lpFile->String);
		else
		{
			char	strMap[64];

			sprintf(strMap,"%s",lpFile->String);

			s_lvMap.SetItem(iIndex,1,strMap);
		}
	}
}

void
cMAIN::UpdateMapList()
{
	for (int i=0;i<s_pFiles->Count;i++)
	{
		cSTRING	*lpFile	=	s_pFiles->Pick(i);

		if	(cMAIN::FindMap(lpFile->String))
			s_lvMap.SetItem(i,0,"O");
		else
			s_lvMap.SetItem(i,0,"X");
	}
}

LRESULT CALLBACK
cMAIN::DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
	{	case	WM_INITDIALOG	: 
			InitDlg(hwnd);
			return (TRUE);

		case	WM_CLOSE	:
			Exit();
			break;

		case	WM_MOVE						:	
		case	WM_SIZE						:	
			Resize();
			break;

		case WM_NOTIFY :
			switch(((NM_TREEVIEW *)lParam)->hdr.code)
			{	case	TVN_SELCHANGED	:
					FillData(hwnd);
					break;
			}
			break;

		case WM_COMMAND		:
			switch(LOWORD(wParam))
			{
				case	IDOK					:
				{
					Exit();
					break;
				}

				case	IDC_EXPORT_MAP_LIST		:
				{
					ExportMapList();
					break;
				}

				case	IDC_SELECT_MAP			:
				{
					SelectMap(hwnd);
					break;
				}

				case	IDC_UPDATE_SERVER_INFO	:
				{
					UpdateSelectServer(hwnd);
					break;
				}
				case	IDC_UPDATE_MAP_INFO	:
				{
					UpdateSelectMap(hwnd);
					break;
				}

				case	IDC_ADD_SERVER			:
				{
					AddServer(hwnd);
					break;
				}

				case	IDC_REMOVE_SERVER		:
				{
					RemoveServer(hwnd);
					break;
				}

				case	IDC_REMOVE_MAP			:
				{
					RemoveMap(hwnd);
					break;
				}

				case	IDC_ADD_MAP				:
				{
					AddMap(hwnd);
					SortServerMapList(hwnd);
					break;
				}

				case	IDC_LOAD				:
					Load(hwnd);
					break;

				case	IDC_SAVE				:
					Save(hwnd);
					break;

				case	IDC_SORT				:
					SortServerMapList(hwnd);
					break;


				default :
					return FALSE;
			}
	}

	return( FALSE );
}