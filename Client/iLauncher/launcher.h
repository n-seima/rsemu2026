#ifndef	_class_install_welcome_h
#define	_class_install_welcome_h

#include "main.h"
#include "bar_menu.h"
#include "image_scroll_bar.h"
#include "downLoad.h"
#include "cAgent.h"
#include "cNUX.h"

#define	dNOTICE_SIZE			4096
#define	dADRESS_SIZE			512
#define	dMAX_PATCH_FILE_COUNT	(1024*10)

class	cLauncher	:	public	cCommonFrame
{
public:
	enum
	{
		eMenu_HomePage,
		eMenu_Cancel,
		eMenu_Launch,
		eMenu_Folder,
		eMenu_Option,

		ePS_ReadyToLaunch	,
		ePS_Patch			,
		ePS_WaitPatchInfo	,
	};

	cThread			m_throadDownload;
	int				m_iRetryCount,m_iPatchStatus;
	BOOL			m_bIsServerClose;
	char			m_strPatchStatusText[256],m_strSpentTime[128],m_strDownLoadInfo[256],m_strCurrentDownLoadFileInfo[256],m_strDownloadSpeed[128];
	DWORD			m_dwBeginTime,m_dwReceivedSize;
	char			*m_pstrUpdateText;
	BOOL			m_bIsReloadUpdateText,m_bIsDrawPannel;

	char			m_strNotice[dNOTICE_SIZE];
	char			m_strGameFolder[dADRESS_SIZE];
	char			m_strPatchDataAddress[dADRESS_SIZE];
	int				m_iCurrentVersion,m_iLastestVersion,m_iServerStatus;
	int				m_aPatchVersionList[dMAX_PATCH_FILE_COUNT],m_iPatchFileCount,m_iFirstPatchFile,m_iCurrentPatchFile;
	char			m_strExtractFileName[64];
	int				m_iTimerCount,m_iProgressRate;
	BOOL			m_isExtract;
	DWORD			m_dwLastUpdateTime;
	BOOL			m_isCompleteUpdate;
	BOOL			m_isOperationSuccess,m_bIsDownloading;

	BOOL			m_bIsAutoRun,m_bIsCannotFindServer;

	CImageScrollBar	m_scrollBarUpdate,m_scrollBarNotice;
	HWND			m_hWnd;
	cAGENT			m_agent;

	cNUX_INFO*		m_lpExtractInfo;

	void			receiveNotice(void *_lpData);
	BOOL			connectLoginServer();
	void			connectEvent(WPARAM wParam, LPARAM lParam);
	void			changePatchStatus(int _iStatus);

	BOOL			run();
	BOOL			init(void *_lpPara);

	void			draw();

	void			drawReadyToLaunch();
	void			drawPatchStatus();
	void			drawWaitPatchInfo();

	void			readyToInit(void *_lpPara);
	void			readyToClose();
	void			readUpdateText();
	BOOL			downloadLoginServerIP();

	cDOWNLOAD		m_patch;
	cDOWNLOAD		m_downloadData;

	void			downLoad();
	void			progress(cDOWNLOAD *_lpInfo);
	BOOL			extractFile(char *_lpstrFilename);
	BOOL			launch();

	static	void	DownLoad();
	static	void	Progress(cDOWNLOAD *_lpInfo);
	static	void	ProgressExtract(void *_lpInfo);
};

extern	cLauncher	g_launcher;
extern	int			g_iLaunchType;	//	런쳐를 실행 시킨 방법

enum
{
	launch_by_normal,			//	그냥 런쳐를 실행 시켰다.
	launch_by_korea_home,		//	한국 홈페이지에서 액티브 액스를 이용해 실행 했다.
	launch_by_korea_channeling,	//	한국 채널링 서비스를 이용해 실행 했다.
	launch_by_japan_hangame	,	//	일본 한게임 채널링 서비스를 이용해 실행 했다.
};

#endif