// 2005-12-15 Kwon Yong Jae (amadeusj@gmail.com)
// FTP 에 접속해서 파일을받거나 업로드할수 있음
// 방화벽이 설정되어있다면 FtpOpenFile() 함수가 블럭됨다 (오랜 시간후에 에러코드 12002 를 리턴한다)
// 그래서 Thread 에서 처리
//////////////////////////////////////////////////////////////////////
#pragma once


#include <windows.h>
#include <wininet.h>
#include <commctrl.h>
#pragma comment(lib, "WinInet")
#include "CommonLib.h"

class cFTPClient  
{
private:
	cThread m_cFTPOpenFileThread;	// FtpOpenFile() 함수가 방화벽에 의해 블럭 될수 있으므로 쓰레드에서 처리
	TCHAR m_szFileName[256];		// m_cFTPOpenFileThread 에서 사용
	BOOL m_bThreadBlock;			// m_cFTPOpenFileThread 가 불럭 중인지
	cLog m_cLog;					// 로그파일
	HINTERNET m_hFTP;
	HINTERNET m_hInternet;			// 인터넷 핸들
	HINTERNET m_hFTPFile;
	DWORD m_dwCurFileCalcByte;		// 현재 작업중인 파일의 용량		
	DWORD m_dwTotalFileCalcByte;	// 모든 작업중인 파일의 용량	
	BOOL m_bUpload;

public:
	// FTP 파일 정보
	typedef struct _stFILEINFO
	{
		BOOL bFile;					// 파일인지 폴더인지 구분해야한다
		DWORD dwSize;
		TCHAR szName[256];
		FILETIME ftCreateTime;		// 생성 날짜 (SYSTEMTIME 는 16바이트 이므로 FILETIME 을 사용한다)
		FILETIME ftLastAccessTime;	// 액세스 날짜 (SYSTEMTIME 는 16바이트 이므로 FILETIME 을 사용한다)
		FILETIME ftLastWriteTime;	// 수정 날짜 (SYSTEMTIME 는 16바이트 이므로 FILETIME 을 사용한다)
	}stFILEINFO;	

private:		
	BOOL setDirectory(const TCHAR *_pszFTPDirectory, BOOL _bCreateDirectory);	
	BOOL openFile(const TCHAR *_pszFileName, BOOL _bUpload);
	void checkDerectory(TCHAR *_pszFTPDirectory);
	void errorHandling(DWORD _dwErrorCode, const TCHAR *_pszMessage, ...);

	static DWORD FTPOpenFileThreadProc(void *_pcPtr, int _iThreadIdx);	// cThread Class 에 의해 사용됨

public:
	cFTPClient();
	~cFTPClient();
	BOOL connect(const TCHAR *_pszServer, int _iPort, const TCHAR *_pszID, const TCHAR *_pszPW);
	void disconnect();
	BOOL getFileExist(const TCHAR *_pszFTPDirectory, const TCHAR *_pszFileName);
	BOOL getFileSize(const TCHAR *_pszFTPDirectory, const TCHAR *_pszFileName, DWORD *_pdwSize);
	BOOL getFileList(const TCHAR *_pszFTPDirectory, vector<stFILEINFO> *_pvecData);
	BOOL downloadFile(const TCHAR *_pszFTPDirectory, const TCHAR *_pszFileName, const TCHAR *_pszLocalDirectory,const TCHAR *_lpstrDestFile=NULL);
	BOOL uploadFile(const TCHAR *_pszFTPDirectory, const TCHAR *_pszFileFullName);
	BOOL deleteFile(const TCHAR *_pszFTPDirectory, const TCHAR *_pszFileName);
	BOOL renameFile(const TCHAR *_pszFTPDirectory, const TCHAR *_pszOldFileName, const TCHAR *_pszNewFileName);
	
	inline DWORD getCurFileCalcByte(){return m_dwCurFileCalcByte;}		
	inline DWORD getTotalFileCalcByte(){return m_dwTotalFileCalcByte;}				
};


