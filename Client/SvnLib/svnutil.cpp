#include "svnutil.h"
#include <atlbase.h>
#include <assert.h>

#include "cSVNentriesParser.h"

using namespace W2Lib;

int
cSvnUtil::chkSvnInfomation()
{
	CRegKey		reg;

	if (reg.Open( HKEY_LOCAL_MACHINE, dREG_SVNINFO ) == ERROR_SUCCESS)
	{
		DWORD	count	=	eMAX_PATH_LENGTH;
		if (reg.QueryValue(m_strClientPath,_T("ProcPath"),&count) == ERROR_SUCCESS)
		{
			if (count	<=	0)
				return 0;
		}
	}

	if (reg.Open( HKEY_CURRENT_USER, dREG_SVNINFO) == ERROR_SUCCESS)
	{
		DWORD	count	=	eMAX_PATH_LENGTH;
		W2CHAR strVersion[eMAX_PATH_LENGTH];
		if (reg.QueryValue(strVersion,_T("CurrentVersion"),&count) == ERROR_SUCCESS)
		{
			if (count	<=	0)
				return 0;
			//	version parsing .. ex) 1,3,5,9302
		}
	}

	m_isFindSVNInfo = 1;
	return 1;
}

W2CHAR * 
cSvnUtil::getSvnClientProc()
{
	return m_strClientPath;
}

W2CHAR * 
cSvnUtil::getSvnServerProc()
{
	return m_strServerPath;
}

int
cSvnUtil::addFile(W2CHAR * _strFileName)
{
	m_dequeFileList.push_back(_strFileName);

	return	TRUE;
}

int
cSvnUtil::isCorrectFile(W2CHAR * _strFileName,int *_lpiIsFolder,int *_lpiIsLocked)
{
	int	iIsFoder,iIsLock;
	
	int	iResult	=	procIsCorrectFile(_strFileName,iIsFoder,iIsLock);

	if	(_lpiIsFolder)
		*_lpiIsFolder	=	iIsFoder;
	if	(_lpiIsLocked)
		*_lpiIsLocked	=	iIsLock;

	return	iResult;
}

//08. 02. 26 정해기 수정
/*
int
cSvnUtil::procIsCorrectFile(W2CHAR * _strFileName,int & isFolder, int & isLock)
{
	if	(!m_isFindSVNInfo)
		return 0;
	
	//	svn과 연결된 파일인지 확인한다.
	//	해당 폴더의 .svn 파일을 연다.
	W2CHAR strSvnEntryFile[1024];
	W2SPRINTF(strSvnEntryFile,_T("%s\\.svn\\entries"),cFileUtil::exportFolderName(_strFileName));

	const char * test = cUniCode::UnicodeToAscii(strSvnEntryFile);
	TiXmlDocument doc( test );
	bool loadOkay = doc.LoadFile(TIXML_ENCODING_UTF8);
	if ( !loadOkay )
		return 0;

	TiXmlNode* node = 0;
	TiXmlElement* itemElement = 0;
	TiXmlAttribute* itemAttribute = 0;
	TiXmlNode* childnode = 0;
	node = doc.FirstChild( "wc-entries" );
	if(!node)	return 0;

	childnode = node->FirstChild();

	//	svn은 하나의 wc-entries안에 각 파일, 폴더에 대한 정보를 가지고 있다.
	W2CHAR * _fn = cFileUtil::exportFileName(_strFileName);
	for(itemElement = childnode->ToElement();itemElement;itemElement = itemElement->NextSiblingElement())
	{
		W2CHAR * _tmp = cUniCode::AsciiToUnicode((char *)itemElement->Attribute("name"),CP_UTF8);
		if(!_tmp)	continue;
		if(W2STRICMP(_tmp , _fn)==0)
		{
			if(stricmp(itemElement->Attribute("kind"),"dir")==0)	isFolder = 1;
			else													isFolder = 0;
			if(itemElement->Attribute("lock-token"))	isLock = 1;
			else										isLock = 0;
			break;
		}
	}
	return 1;
}
*/
//08. 02. 26 정해기 수정
int
cSvnUtil::procIsCorrectFile(W2CHAR * _strFileName,int & isFolder, int & isLock)
{
	if	(!m_isFindSVNInfo)
		return 0;
	
	//	svn과 연결된 파일인지 확인한다.
	//	해당 폴더의 .svn 파일을 연다.
	TCHAR strSvnEntryFile[1024];
	W2SPRINTF(strSvnEntryFile,_T("%s\\.svn\\entries"),cFileUtil::exportFolderName(_strFileName));
	
	cSVNentriesParser svnParser;
	bool loadOkay	=	svnParser.loadFile(strSvnEntryFile);

	if	(!loadOkay)
		return 0;

	//	svn은 하나의 wc-entries안에 각 파일, 폴더에 대한 정보를 가지고 있다.
	TCHAR * _fn	=	cFileUtil::exportFileName(_strFileName);

	const cSVNentriesParser::stInfo* pstInfo = svnParser.getInfo(_fn);

	//해당 파일이 존재 하지 않으면
	if(pstInfo == NULL)
	{
		return 0;
	}
	else 
	{
		//락이 걸려 잇는지 판단 
		(pstInfo->bisLock) ? isLock = 1 : isLock = 0;
		//폴더인지 파일인지 검사
		(pstInfo->iFileFormat == cSVNentriesParser::eFormat_Folder) ? isFolder = 1 : isFolder = 0;
	}	

	return 1;
}


int
cSvnUtil::procMakeParam(W2CHAR * _strCommand, W2CHAR * _strAddedParam)
{
	if(!m_dequeFileList.size())
		return 0;

	int iParamSize = 80;	//	대략적인 기본 사이즈
	for(int i=m_dequeFileList.size();i>0;)
	{	--i;
		iParamSize += m_dequeFileList.at(i).length()+3;
	}
	if(m_strParam)
	{
		delete [] m_strParam;
		m_strParam = NULL;
	}

	m_strParam = new W2CHAR[iParamSize];
	assert(m_strParam);

	W2SPRINTF(m_strParam,_T("/command:%s %s /path:\""),_strCommand,_strAddedParam);

	for(int j=m_dequeFileList.size();j>0;)
	{	--j;
		//	해당 파일이 svn에 소속되어 있는지..
		int isLock=0,isFolder=0;
		std::wstring fn = m_dequeFileList.at(j);
		if(!procIsCorrectFile((WCHAR *)fn.c_str(),isFolder,isLock))	continue;

		if(W2STRICMP(_strCommand,_T("lock"))==0 && isLock)	continue;
		if(W2STRICMP(_strCommand,_T("unlock"))==0 && isLock==0)	continue;

		W2STRCAT(m_strParam, (WCHAR *)m_dequeFileList.at(j).c_str());
		if(j>0)
			W2STRCAT(m_strParam, _T("*"));
	}
	W2STRCAT(m_strParam, _T("\""));

	long tmpSize = W2STRLEN(m_strParam);

	return 1;
}

int 
cSvnUtil::procCommand()
{
	assert(m_strParam);


	STARTUPINFOW		si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof( STARTUPINFO)); 
	ZeroMemory( &pi, sizeof( PROCESS_INFORMATION)); 

	si.cb			=	sizeof( STARTUPINFO); 
	si.dwFlags		=	STARTF_USESHOWWINDOW; 
	si.wShowWindow	=	SW_SHOWNORMAL;

	W2CHAR	strPara[1024];

	W2SPRINTF(strPara,_T(" %s"),m_strParam);

	CreateProcess(getSvnClientProc(),strPara,NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if	(pi.hProcess) 
	{
		DWORD	dwExitCode	=	STILL_ACTIVE; 

		while(dwExitCode	==	STILL_ACTIVE) 
		{
			WaitForSingleObject( pi.hProcess, 1); 
			GetExitCodeProcess( pi.hProcess, &dwExitCode);
			Sleep(1);
		}
	}

	//::ShellExecute(NULL,_T("open"),getSvnClientProc(),m_strParam,_T(""),SW_SHOW);
	if	(m_strParam)
	{
		delete [] m_strParam;
		m_strParam = NULL;
	}
	m_dequeFileList.clear();
	return 1;
}

int 
cSvnUtil::cmdLock()
{
	if	(!procMakeParam(_T("lock"),_T(" /notempfile /closeonend:1")))
		return 0;

	return procCommand();
}

int 
cSvnUtil::cmdUnlock()
{
	//	수정중이라면???
	if	(!procMakeParam(_T("unlock"),_T(" /notempfile /closeonend:1")))
		return 0;

	return procCommand();
}

int	
cSvnUtil::cmdCommit()
{
	//	lock 상태라면..
	if	(!procMakeParam(_T("commit"),_T(" /notempfile /closeonend:1")))
		return 0;

	return procCommand();
}

int
cSvnUtil::cmdUpdate()
{
	if	(!procMakeParam(_T("update"),_T(" /notempfile /closeonend:1")))
		return 0;

	return procCommand();
}

int 
cSvnUtil::cmdShowLog()
{
	if	(m_dequeFileList.size()>1)
		return 0;	//	한개 이상은 볼 수 없다.
	if	(!procMakeParam(_T("log"),_T(" /notempfile /closeonend:1")))
		return 0;

	return procCommand();
}

int 
cSvnUtil::cmdRename()
{
	if	(m_dequeFileList.size()>1)
		return 0;	//	하나씩 변경하도록..

	if	(!procMakeParam(_T("rename"),_T(" /notempfile /closeonend:1")))
		return 0;

	return procCommand();
	//	commit까지
}

int 
cSvnUtil::cmdDelete()
{
	if	(!procMakeParam(_T("delete"),_T(" /notempfile /closeonend:1")))
		return 0;

	return procCommand();
	//	commit까지..
}

void
cSvnUtil::setSvnServerProc(W2CHAR * _path_fn)
{
	//	지정된 폴더에 svnserve.exe가 존재하는지 확인한다. 

	W2STRCPY(m_strServerPath,_path_fn);
}

int
cSvnUtil::cmdRunSvnServe(W2CHAR * _targetFolder,int _iPort)
{
	if(m_strServerPath[0] == NULL)
		return 0;

	W2CHAR	strParam[1024];
	W2SPRINTF(strParam,_T(" -d -r \"%s\" --listen-port %d"),_targetFolder,_iPort);

	::ShellExecute(NULL,_T("open"),getSvnServerProc(),strParam,_T(""),SW_SHOW);
	
	return 1;
}