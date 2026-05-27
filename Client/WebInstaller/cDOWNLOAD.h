#ifndef _classDOWNLOAD
#define	_classDOWNLOAD

#include "SFC.H"

#define	dRECEIVE_BUFFER_SIZE	2048
#define	IDC_DOWNLOAD_FINISH		1000

class	cDOWNLOAD
{
public:	
	HWND			m_hWnd;

	char			m_strUrl[512];
	char			m_strFileName[512];
	_int64			m_dwCurrentFileSize,m_dwCurrentFileReceivedSize,m_dwBeginTime,m_dwBps,m_dwProgressTime;
	_int64			m_dwReceivedSize;
	int				m_iProgressRate;
	BOOL			m_bIsCancelWork,m_bIsPauseWork,m_bIsComplete;

	void			update(DWORD dwReceivedSize,DWORD dwDataSize);

					cDOWNLOAD();

	void			reset();
	void			setBeginTime();
	BOOL			downLoad(char *strUrl,void	(*update)(cDOWNLOAD*));

	void			cancel()			{m_bIsCancelWork=	TRUE;}
	void			pause()				{m_bIsPauseWork	=	TRUE;}
	void			resume()			{m_bIsPauseWork	=	FALSE;}
	void			setWnd(HWND hwnd)	{m_hWnd			=	hwnd;}
	BOOL			isComplete()		{return	m_bIsComplete;}

	void			excute(HWND hwnd);
	static	BOOL	IsValidAddress(char *address);
};

class CBSCallbackImpl : public IBindStatusCallback
{
public:
	CBSCallbackImpl(cDOWNLOAD *lpDownload,void	(*update)(cDOWNLOAD*));

	void			(*m_lpfpUpdate)(cDOWNLOAD*lpInfo);
	cDOWNLOAD		*m_lpDownloadInfo;

	// IUnknown methods
	STDMETHOD(QueryInterface)(REFIID riid, void **ppvObject);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

	// IBindStatusCallback methods
	STDMETHOD(OnStartBinding)(DWORD, IBinding *);
	STDMETHOD(GetPriority)(LONG *);
	STDMETHOD(OnLowResource)(DWORD);
	STDMETHOD(OnProgress)(ULONG ulProgress,ULONG ulProgressMax,ULONG ulStatusCode,LPCWSTR szStatusText);
	STDMETHOD(OnStopBinding)(HRESULT, LPCWSTR);
	STDMETHOD(GetBindInfo)(DWORD *, BINDINFO *);
	STDMETHOD(OnDataAvailable)(DWORD, DWORD, FORMATETC *, STGMEDIUM *);
	STDMETHOD(OnObjectAvailable)(REFIID, IUnknown *);


protected:
	ULONG	m_ulObjRefCount;
};

#endif