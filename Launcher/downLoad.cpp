#include "SFC.h"
#include "stdafx.h"
#include <mmsystem.h>
#include "downLoad.H"

#pragma comment(lib, "urlmon.lib")

//
//
CBSCallbackImpl::CBSCallbackImpl(cDOWNLOAD *_lpDownload,void (*_lpUpdateFunction)(cDOWNLOAD *))
{
	m_lpDownloadInfo	=	_lpDownload;
	m_lpfpUpdate		=	_lpUpdateFunction;
	m_ulObjRefCount		=	1;
}


STDMETHODIMP
CBSCallbackImpl::OnProgress(ULONG ulProgress,ULONG ulProgressMax,ULONG ulStatusCode,LPCWSTR szStatusText)
{
	USES_CONVERSION;

	if (ulStatusCode	==	BINDSTATUS_CACHEFILENAMEAVAILABLE)	strcpy_s(m_lpDownloadInfo->m_strFileName,W2CT(szStatusText));

	m_lpDownloadInfo->update(ulProgress,ulProgressMax);

	if	(m_lpfpUpdate)
		m_lpfpUpdate(m_lpDownloadInfo);

	if	(m_lpDownloadInfo->m_bIsCancelWork	==	TRUE)
		return E_ABORT;  // canceled by the user

	return S_OK;
}

// IUnknown
STDMETHODIMP
CBSCallbackImpl::QueryInterface(REFIID riid, void **ppvObject)
{
	*ppvObject = NULL;
	
	// IUnknown
	if (::IsEqualIID(riid, __uuidof(IUnknown)))
	{
		*ppvObject = this;
	}
	// IBindStatusCallback
	else 
	if (::IsEqualIID(riid, __uuidof(IBindStatusCallback)))
	{
		*ppvObject = static_cast<IBindStatusCallback *>(this);
	}

	if (*ppvObject)
	{
		return S_OK;
	}
	
	return E_NOINTERFACE;
}                                             

//-----------------------------------------------------------------------------
STDMETHODIMP_(ULONG)
CBSCallbackImpl::AddRef()
{
	return ++m_ulObjRefCount;
}
//-----------------------------------------------------------------------------
STDMETHODIMP_(ULONG)
CBSCallbackImpl::Release()
{
	return --m_ulObjRefCount;
}
//-----------------------------------------------------------------------------
// IBindStatusCallback
STDMETHODIMP 
CBSCallbackImpl::OnStartBinding(DWORD, IBinding *)
{
	return S_OK;
}
//-----------------------------------------------------------------------------
STDMETHODIMP 
CBSCallbackImpl::GetPriority(LONG *)
{
	return E_NOTIMPL;
}
//-----------------------------------------------------------------------------
STDMETHODIMP 
CBSCallbackImpl::OnLowResource(DWORD)
{
	return S_OK;
}
//-----------------------------------------------------------------------------
STDMETHODIMP 
CBSCallbackImpl::OnStopBinding(HRESULT, LPCWSTR)
{
	return S_OK;
}
//-----------------------------------------------------------------------------
STDMETHODIMP 
CBSCallbackImpl::GetBindInfo(DWORD *, BINDINFO *)
{
	return S_OK;
}
//-----------------------------------------------------------------------------
STDMETHODIMP 
CBSCallbackImpl::OnDataAvailable(DWORD, DWORD,FORMATETC *, STGMEDIUM *)
{
	return S_OK;
}
//-----------------------------------------------------------------------------
STDMETHODIMP 
CBSCallbackImpl::OnObjectAvailable(REFIID, IUnknown *)
{
	return S_OK;
}

//
//	생성자
cDOWNLOAD::cDOWNLOAD()
{
	reset();
}	//	cDOWNLOAD::cDOWNLOAD()

//
//	초기화
void
cDOWNLOAD::reset()
{
	m_hWnd						=	NULL;

	m_dwCurrentFileSize			=	0;
	m_dwReceivedSize			=	0;
	m_dwCurrentFileReceivedSize	=	0;
	m_dwBeginTime				=	0;
	m_dwBps						=	0;
	m_dwProgressTime			=	0;
	m_iProgressRate				=	0;
	m_dwBeginTime				=	timeGetTime();
}	//	cDOWNLOAD::reset()

//
//	다운로드 받기 시작한 시간 설정
void
cDOWNLOAD::setBeginTime()
{
	m_dwBeginTime				=	timeGetTime();
}	//	cDOWNLOAD::setBeginTime()

void
cDOWNLOAD::update(DWORD dwReceivedSize,DWORD dwDataSize)
{
	if	(dwDataSize		<=	0)
		return;

	m_dwCurrentFileSize			=	dwDataSize;
	m_dwCurrentFileReceivedSize	=	dwReceivedSize;

	m_iProgressRate				=	(int)(m_dwCurrentFileReceivedSize*1000/m_dwCurrentFileSize);
	m_dwProgressTime			=	timeGetTime() - m_dwBeginTime;

	if	(m_dwProgressTime > 0)
		m_dwBps					=	m_dwCurrentFileReceivedSize/m_dwProgressTime;	//	천분에 일초동안 다운받은 양-_-
}


//
//	다운로드 받기
BOOL
cDOWNLOAD::downLoad(char *strUrl,void (*update)(cDOWNLOAD*),int _iDownLoadOption)
{
	m_dwCurrentFileReceivedSize	=	0;
	m_bIsCancelWork				=	FALSE;
	m_bIsPauseWork				=	FALSE;
	m_bIsComplete				=	FALSE;

	m_dwBeginTime				=	timeGetTime();

	strcpy_s(m_strUrl,strUrl);

	USES_CONVERSION;

	if	(::IsValidURL(NULL,T2CW(strUrl),0)!= S_OK)
		return FALSE;

	CBSCallbackImpl bsc(this,update);

	HRESULT	hr	=	URLDownloadToCacheFile(NULL,m_strUrl,m_strFileName,_iDownLoadOption,0,&bsc);

	if	(hr != 0)
		return FALSE;

//	if (!m_bIsPauseWork)	SendMessage(m_hWnd,WM_COMMAND,IDC_DOWNLOAD_FINISH,0);
	m_dwReceivedSize			+=	m_dwCurrentFileReceivedSize;
	m_bIsComplete				=	TRUE;

	return	TRUE;
}	//	cDOWNLOAD::downLoad(char *strUrl,void (*update)(cDOWNLOAD_INFO*))

//
//	화일 다운로드 받기 시작
void
cDOWNLOAD::excute(HWND hwnd)
{
	ShellExecute(NULL,"open",m_strFileName,NULL,NULL,SW_SHOWNORMAL);

	return;
}	//	cDOWNLOAD::excute(HWND hwnd)

//
//	잘못된 주소다.
BOOL
cDOWNLOAD::IsValidAddress(char *address)
{
	USES_CONVERSION;

	if	(::IsValidURL(NULL,T2CW(address),0)	!=	S_OK)
		return TRUE;

	return	FALSE;
}	//	cDOWNLOAD::IsValidAddress(char *address)