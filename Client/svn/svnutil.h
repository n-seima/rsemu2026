#pragma once 
#pragma warning (disable : 4786)
#include "WWCommon.h"
#include <string>
#include <deque>

#define dREG_SVNINFO _T("Software\\TortoiseSVN") //HKEY_LOCAL_MACHINE//ProcPath, //HKEY_CURRRENT_USER//CurrentVersion


class cSvnUtil
{
public:
	enum
	{
		eMAX_PATH_LENGTH = 512,
	};
	std::deque<std::wstring>	m_dequeFileList;	//	처리할 파일들은 이곳으로..

private://	variable
	W2CHAR	m_strClientPath[eMAX_PATH_LENGTH];//TortoiseSVNProc.exe의 풀 경로
	W2CHAR	m_strServerPath[eMAX_PATH_LENGTH];//svn.exe의 풀 경로
	int		m_isFindSVNInfo;
	W2CHAR * m_strParam;

private://	function
	int procCommand();
	int	procIsCorrectFile(W2CHAR * _strFileName,int & isFolder,int & isLock);
	int procMakeParam(W2CHAR * _strCommand, W2CHAR * _strAddedParam);

public://	check svn infomation
	cSvnUtil()
	{
		m_isFindSVNInfo = 0;
		m_strParam = NULL;
	}
	int		chkSvnInfomation();	//	SVN의 모든 정보를 파악한다. 처음 시작시에 정상적으로 설치되어 있는지 확인한다.

	W2CHAR * getSvnClientProc();//	m_strClientPath를 돌려준다.
	W2CHAR * getSvnServerProc();//	m_strServerPath를 돌려준다.


public:	//	svn command for client
//	추가됨
	int		isCorrectFile(W2CHAR * _strFileName,int *_lpiIsFolder=NULL,int *_lpiIsLocked=NULL);	//	얘가 SVN에서 관리중인 파일이냐?
	int		addFile(W2CHAR * _strFileName);
//	추가됨

	int		cmdLock();		//	지정된 파일(혹은 폴더)을 GETLOCK하고, 결과를 돌려준다.
	int		cmdUnlock();		//	지정된 파일(혹은 폴더)을 RELEASELOCK하고, 결과를 돌려준다.
	int		cmdCommit();		//	지정된 파일(혹은 폴더)을 commit하고, 결과를 돌려준다.

	int		cmdUpdate();		//	서버로부터 업데이트 된 내용을 받아온다.
	int		cmdShowLog();		//	지정된 파일(혹은 폴더)의 지난 로그를 본다.
	int		cmdRename();		//	지정된 파일(혹은 폴더)의 이름을 변경한다.
	int		cmdDelete();		//	지정된 파일(혹은 폴더)을 삭제한다.

	//	백업, 이동에 대한 내용은 추후에...

public:	//	svn command for server
	void setSvnServerProc(W2CHAR * _path_fn);				//	서버 실행파일이 있는 폴더를 세팅한다.
	int cmdRunSvnServe(W2CHAR * _targetFolder,int _iPort);	//	daemon 모드로 실행한다.
};
